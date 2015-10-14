import os, sys
from . import commands

# Some nasty hacks to prevent most of our extensions using a manifest, as
# the manifest - even without a reference to the CRT assembly - is enough
# to prevent the extension from loading.  For more details, see
# http://bugs.python.org/issue7833 - that issue has a patch, but it is
# languishing and will probably never be fixed for Python 2.6...
if sys.version_info > (2,6):
    from distutils.spawn import spawn
    from distutils.msvc9compiler import MSVCCompiler
    MSVCCompiler._orig_spawn = MSVCCompiler.spawn
    MSVCCompiler._orig_link = MSVCCompiler.link

    # We need to override this method for versions where issue7833 *has* landed
    # (ie, 2.7 and 3.2+)
    def manifest_get_embed_info(self, target_desc, ld_args):
        _want_assembly_kept = getattr(self, '_want_assembly_kept', False)
        if not _want_assembly_kept:
            return None
        for arg in ld_args:
            if arg.startswith("/MANIFESTFILE:"):
                orig_manifest = arg.split(":", 1)[1]
                if target_desc==self.EXECUTABLE:
                    rid = 1
                else:
                    rid = 2
                return orig_manifest, rid
        return None
    # always monkeypatch it in even though it will only be called in 2.7
    # and 3.2+.
    MSVCCompiler.manifest_get_embed_info = manifest_get_embed_info

    def monkeypatched_spawn(self, cmd):
        is_link = cmd[0].endswith("link.exe") or cmd[0].endswith('"link.exe"')
        is_mt = cmd[0].endswith("mt.exe") or cmd[0].endswith('"mt.exe"')
        _want_assembly_kept = getattr(self, '_want_assembly_kept', False)
        if not _want_assembly_kept and is_mt:
            # We don't want mt.exe run...
            return
        if not _want_assembly_kept and is_link:
            # remove /MANIFESTFILE:... and add MANIFEST:NO
            # (but note that for winxpgui, which specifies a manifest via a
            # .rc file, this is ignored by the linker - the manifest specified
            # in the .rc file is still added)
            for i in range(len(cmd)):
                if cmd[i].startswith("/MANIFESTFILE:"):
                    cmd[i] = "/MANIFEST:NO"
                    break
        if _want_assembly_kept and is_mt:
            # We want mt.exe run with the original manifest
            for i in range(len(cmd)):
                if cmd[i] == "-manifest":
                    cmd[i+1] = cmd[i+1] + ".orig"
                    break
        self._orig_spawn(cmd)
        if _want_assembly_kept and is_link:
            # We want a copy of the original manifest so we can use it later.
            for i in range(len(cmd)):
                if cmd[i].startswith("/MANIFESTFILE:"):
                    mfname = cmd[i][14:]
                    shutil.copyfile(mfname, mfname + ".orig")
                    break

    def monkeypatched_link(self, target_desc, objects, output_filename, *args, **kw):
        # no manifests for 3.3+ 
        self._want_assembly_kept = sys.version_info < (3,3) and \
                                   (os.path.basename(output_filename).startswith("PyISAPI_loader.dll") or \
                                    os.path.basename(output_filename).startswith("perfmondata.dll") or \
                                    os.path.basename(output_filename).startswith("win32ui.pyd") or \
                                    target_desc==self.EXECUTABLE)
        try:
            return self._orig_link(target_desc, objects, output_filename, *args, **kw)
        finally:
            delattr(self, '_want_assembly_kept')
    MSVCCompiler.spawn = monkeypatched_spawn
    MSVCCompiler.link = monkeypatched_link
