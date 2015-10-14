import os, sys
import distutils.command.build_ext
from distutils.dep_util import newer_group, newer
import glob
import re

from .._compat import *
from .. import config
from .. import logging
log = logging.logger(__package__)

class my_build_ext(distutils.command.build_ext.build_ext):

    def finalize_options(self):
        distutils.command.build_ext.build_ext.finalize_options(self)
        self.windows_h_version = None
        # The pywintypes library is created in the build_temp
        # directory, so we need to add this to library_dirs
        self.library_dirs.append(self.build_temp)
        self.mingw32 = (self.compiler == "mingw32")
        if self.mingw32:
            self.libraries.append("stdc++")

        self.excluded_extensions = [] # list of (ext, why)
        self.swig_cpp = True # hrm - deprecated - should use swig_opts=-c++??
        if not hasattr(self, 'plat_name'):
            # Old Python version that doesn't support cross-compile
            self.plat_name = distutils.util.get_platform()

    def _find_windows_h_version(self):
        """Search the known include directories for a file which might
        contain the Windows SDK version.
        """
        include_dirs = self.compiler.include_dirs + \
                       os.environ.get("INCLUDE", "").split(os.pathsep)
        log.debug("Include dirs: %s", include_dirs)
        
        if self.windows_h_version is None:
            for d in include_dirs:
                # We look for _WIN32_WINNT instead of WINVER as the Vista
                # SDK defines _WIN32_WINNT as WINVER and we aren't that clever
                # * Windows Server 2003 SDK sticks this version in WinResrc.h
                # * Vista SDKs stick the version in sdkddkver.h
                for header in ('WINDOWS.H', 'SDKDDKVER.H', "WinResrc.h"):
                    look = os.path.join(d, header)
                    if os.path.isfile(look):
                        # read the fist 100 lines, looking for #define WINVER 0xNN
                        # (Vista SDKs now define this based on _WIN32_WINNT,
                        # which should still be fine for old versions)
                        reob = re.compile("#define\W*_WIN32_WINNT\W*(0x[0-9a-fA-F]+)")
                        f = open(look, "r")
                        for i in range(500):
                            line = f.readline()
                            match = reob.match(line)
                            if match is not None:
                                self.windows_h_version = int(match.group(1), 16)
                                log.info("Found version 0x%x in %s" \
                                         % (self.windows_h_version, look))
                                break
                        else:
                            log.debug("No version in %r - looking for another...", look)
                    if self.windows_h_version is not None:
                        break
                if self.windows_h_version is not None:
                    break
            else:
                raise RuntimeError("Can't find a version in Windows.h")
    
    def _fixup_sdk_dirs(self):
        # Adjust paths etc for the platform SDK - this prevents the user from
        # needing to manually add these directories via the MSVC UI.  Note
        # that we currently ensure the SDK dirs are before the compiler
        # dirs, so its no problem if they have added these dirs to the UI)

        # (Note that just having them in INCLUDE/LIB does *not* work -
        # distutils thinks it knows better, and resets those vars (see notes
        # below about how the paths are put together)

        # Called after the compiler is initialized, but before the extensions
        # are built.  NOTE: this means setting self.include_dirs etc will
        # have no effect, so we poke our path changes directly into the
        # compiler (we can't call this *before* the compiler is setup, as
        # then our environment changes would have no effect - see below)

        # distutils puts the path together like so:
        # * compiler command line includes /I entries for each dir in
        #   ext.include_dir + build_ext.include_dir (ie, extension's come first)
        # * The compiler initialization sets the INCLUDE/LIB etc env vars to the
        #   values read from the registry (ignoring anything that was there)

        # We are also at the mercy of how MSVC processes command-line
        # includes vs env vars (presumably environment comes last) - so,
        # moral of the story:
        # * To get a path at the start, it must be at the start of
        #   ext.includes
        # * To get a path at the end, it must be at the end of
        #   os.environ("INCLUDE")
        # Note however that the environment tweaking can only be done after
        # the compiler has set these vars, which is quite late -
        # build_ext.run() - so global environment hacks are done in our
        # build_extensions() override)
        #
        # Also note that none of our extensions have individual include files
        # that must be first - so for practical purposes, any entry in
        # build_ext.include_dirs should 'win' over the compiler's dirs.
        assert self.compiler.initialized # if not, our env changes will be lost!

        is_64bit = self.plat_name == 'win-amd64'
        extra = os.path.join(config.platform_sdk, 'include')
        # should not be possible for the SDK dirs to already be in our
        # include_dirs - they may be in the registry etc from MSVC, but
        # those aren't reflected here...
        assert extra not in self.include_dirs
        # and we will not work as expected if the dirs don't exist
        assert os.path.isdir(extra), "%s doesn't exist!" % (extra,)
        self.compiler.add_include_dir(extra)
        # and again for lib dirs.
        extra = os.path.join(config.platform_sdk, 'lib')
        if is_64bit:
            extra = os.path.join(extra, 'x64')
            assert os.path.isdir(extra), extra
        assert extra not in self.library_dirs # see above
        assert os.path.isdir(extra), "%s doesn't exist!" % (extra,)
        self.compiler.add_library_dir(extra)
        # directx sdk sucks - how to locate it automatically?
        # Must manually set DIRECTX_SDK_DIR for now.
        # (but it appears November 2008 and later versions set DXSDK_DIR, so
        # we allow both, allowing our "old" DIRECTX_SDK_DIR to override things
        for dxsdk_dir_var in ("DIRECTX_SDK_DIR", "DXSDK_DIR"):
            dxsdk_dir = os.environ.get(dxsdk_dir_var)
            if dxsdk_dir:
                extra = os.path.join(dxsdk_dir, 'include')
                assert os.path.isdir(extra), "%s doesn't exist!" % (extra,)
                self.compiler.add_include_dir(extra)
                if is_64bit:
                    tail = 'x64'
                else:
                    tail = 'x86'
                extra = os.path.join(dxsdk_dir, 'lib', tail)
                assert os.path.isdir(extra), "%s doesn't exist!" % (extra,)
                self.compiler.add_library_dir(extra)
                break

        log.debug("After SDK processing, includes are %s", self.compiler.include_dirs)
        log.debug("After SDK processing, libs are %s", self.compiler.library_dirs)

        # Vista SDKs have a 'VC' directory with headers and libs for older
        # compilers.  We need to hack the support in here so that the
        # directories are after the compiler's own.  As noted above, the
        # only way to ensure they are after the compiler's is to put them
        # in the environment, which has the nice side-effect of working
        # for the rc executable.
        # We know its not needed on vs9...
        if config.build_version < 9.0:
            if os.path.isdir(os.path.join(config.platform_sdk, 'VC', 'INCLUDE')):
                os.environ["INCLUDE"] += ";" + os.path.join(config.platform_sdk, 'VC', 'INCLUDE')
                log.debug("Vista SDK found: %%INCLUDE%% now %s", os.environ["INCLUDE"])
            if os.path.isdir(os.path.join(config.platform_sdk, 'VC', 'LIB')):
                os.environ["LIB"] += ";" + os.path.join(config.platform_sdk, 'VC', 'LIB')
                log.debug("Vista SDK found: %%LIB%% now %s", os.environ["LIB"])

    def _build_scintilla(self):
        path = 'pythonwin\\Scintilla'
        makefile = 'makefile_pythonwin'
        makeargs = []

        if self.debug:
            makeargs.append("DEBUG=1")
        if not self.verbose:
            makeargs.append("/C") # nmake: /C Suppress output messages
            makeargs.append("QUIET=1")
        # We build the DLL into our own temp directory, then copy it to the
        # real directory - this avoids the generated .lib/.exp
        build_temp = os.path.abspath(os.path.join(self.build_temp, "scintilla"))
        self.mkpath(build_temp)
        # Use short-names, as the scintilla makefiles barf with spaces.
        if " " in build_temp:
            # ack - can't use win32api!!!  This is the best I could come up
            # with:
            # C:\>for %I in ("C:\Program Files",) do @echo %~sI
            # C:\PROGRA~1
            cs = os.environ.get('comspec', 'cmd.exe')
            cmd = cs + ' /c for %I in ("' + build_temp + '",) do @echo %~sI'
            build_temp = os.popen(cmd).read().strip()
            assert os.path.isdir(build_temp), build_temp
        makeargs.append("SUB_DIR_O=%s" % build_temp)
        makeargs.append("SUB_DIR_BIN=%s" % build_temp)
        makeargs.append("DIR_PYTHON=%s" % sys.prefix)

        cwd = os.getcwd()
        os.chdir(path)
        try:
            cmd = ["nmake.exe", "/nologo", "/f", makefile] + makeargs
            self.spawn(cmd)
        finally:
            os.chdir(cwd)

        # The DLL goes in the Pythonwin directory.
        if self.debug:
            base_name = "scintilla_d.dll"
        else:
            base_name = "scintilla.dll"
        self.copy_file(
                    os.path.join(self.build_temp, "scintilla", base_name),
                    os.path.join(self.build_lib, "pythonwin"))

    def _build_pycom_loader(self):
        # the base compiler strips out the manifest from modules it builds
        # which can't be done for this module - having the manifest is the
        # reason it needs to exist!
        # At least this is made easier by it not depending on Python itself,
        # so the compile and link are simple...
        suffix = "%d%d" % (sys.version_info[0], sys.version_info[1])
        if self.debug:
            suffix += '_d'
        src = r"com\win32com\src\PythonCOMLoader.cpp"
        build_temp = os.path.abspath(self.build_temp)
        obj = os.path.join(build_temp, os.path.splitext(src)[0]+".obj")
        dll = os.path.join(self.build_lib, "pywin32_system32", "pythoncomloader"+suffix+".dll")
        if self.force or newer_group([src], obj, 'newer'):
            ccargs = [self.compiler.cc, '/c']
            if self.debug:
                ccargs.extend(self.compiler.compile_options_debug)
            else:
                ccargs.extend(self.compiler.compile_options)
            ccargs.append('/Fo' + obj)
            ccargs.append(src)
            ccargs.append('/DDLL_DELEGATE=\\"pythoncom%s.dll\\"' % (suffix,))
            self.spawn(ccargs)

        deffile = r"com\win32com\src\PythonCOMLoader.def"
        if self.force or newer_group([obj, deffile], dll, 'newer'):
            largs = [self.compiler.linker, '/DLL', '/nologo', '/incremental:no']
            if self.debug:
                largs.append("/DEBUG")
            temp_manifest = os.path.join(build_temp, os.path.basename(dll) + ".manifest")
            largs.append('/MANIFESTFILE:' + temp_manifest)
            largs.append('/PDB:None')
            largs.append("/OUT:" + dll)
            largs.append("/DEF:" + deffile)
            largs.append("/IMPLIB:" + os.path.join(build_temp, "PythonCOMLoader"+suffix+".lib"))
            largs.append(obj)
            self.spawn(largs)
            # and the manifest if one exists.
            if os.path.isfile(temp_manifest):
                out_arg = '-outputresource:%s;2' % (dll,)
                self.spawn(['mt.exe', '-nologo', '-manifest', temp_manifest, out_arg])

    def build_extensions(self):
        # First, sanity-check the 'extensions' list
        self.check_extensions_list(self.extensions)

        self.found_libraries = {}

        if not hasattr(self.compiler, 'initialized'):
            # 2.3 and earlier initialized at construction
            self.compiler.initialized = True
        else:
            if not self.compiler.initialized:
                self.compiler.initialize()

        if config.platform_sdk:
            self._fixup_sdk_dirs()

        self._find_windows_h_version()
        # Here we hack a "pywin32" directory (one of 'win32', 'win32com',
        # 'pythonwin' etc), as distutils doesn't seem to like the concept
        # of multiple top-level directories.
        assert self.package is None
        for ext in self.extensions:
            try:
                self.package = ext.get_pywin32_dir()
            except AttributeError:
                raise RuntimeError("Not a win32 package!")
            self.build_extension(ext)

        for ext in self.extensions:
            if ext.is_win32_exe:
                ext.finalize_options(self)
                can_build, reason = ext.can_build(self)
                if not can_build:
                    self.excluded_extensions.append((ext, reason))
                    log.warn("Skipping %s: %s" % (ext.name, reason))
                    continue

                try:
                    self.package = ext.get_pywin32_dir()
                except AttributeError:
                    raise RuntimeError("Not a win32 package!")
                self.build_exefile(ext)

        # Not sure how to make this completely generic, and there is no
        # need at this stage.
        if sys.version_info > (2,6) and sys.version_info < (3, 3):
            # only stuff built with msvc9 needs this loader.
            self._build_pycom_loader()
        self._build_scintilla()
        # Copy cpp lib files needed to create Python COM extensions
        clib_files = (['win32', 'pywintypes%s.lib'],
                      ['win32com', 'pythoncom%s.lib'],
                      ['win32com', 'axscript%s.lib'])
        for clib_file in clib_files:
            target_dir = os.path.join(self.build_lib, clib_file[0], "libs")
            if not os.path.exists(target_dir):
                self.mkpath(target_dir)
            suffix = ""
            if self.debug:
                suffix = "_d"
            fname = clib_file[1] % suffix
            self.copy_file(
                    os.path.join(self.build_temp, fname), target_dir)
        # The MFC DLLs.
        try:
            target_dir = os.path.join(self.build_lib, "pythonwin")
            if sys.hexversion < 0x2060000:
                # hrm - there doesn't seem to be a 'redist' directory for this
                # compiler (even the installation CDs only seem to have the MFC
                # DLLs in the "win\system" directory - just grab it from
                # system32 (but we can't even use win32api for that!)
                src = os.path.join(os.environ.get('SystemRoot'), 'System32', 'mfc71.dll')
                if not os.path.isfile(src):
                    raise RuntimeError("Can't find %r" % (src,))
                self.copy_file(src, target_dir)
            else:
                # vs2008 or vs2010
                if sys.hexversion < 0x3030000:
                    product_key = r"SOFTWARE\Microsoft\VisualStudio\9.0\Setup\VC"
                    plat_dir_64 = "amd64"
                    mfc_dir = "Microsoft.VC90.MFC"
                    mfc_files = "mfc90.dll mfc90u.dll mfcm90.dll mfcm90u.dll Microsoft.VC90.MFC.manifest".split()
                else:
                    product_key = r"SOFTWARE\Microsoft\VisualStudio\10.0\Setup\VC"
                    plat_dir_64 = "x64"
                    mfc_dir = "Microsoft.VC100.MFC"
                    mfc_files = ["mfc100u.dll", "mfcm100u.dll"]

                # On a 64bit host, the value we are looking for is actually in
                # SysWow64Node - but that is only available on xp and later.
                access = _winreg.KEY_READ
                if sys.getwindowsversion()[0] >= 5:
                    access = access | 512 # KEY_WOW64_32KEY
                if self.plat_name == 'win-amd64':
                    plat_dir = plat_dir_64
                else:
                    plat_dir = "x86"
                # Find the redist directory.
                vckey = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, product_key,
                                        0, access)
                val, val_typ = _winreg.QueryValueEx(vckey, "ProductDir")
                mfc_dir = os.path.join(val, "redist", plat_dir, mfc_dir)
                if not os.path.isdir(mfc_dir):
                    raise RuntimeError("Can't find the redist dir at %r" % (mfc_dir))
                for f in mfc_files:
                    self.copy_file(
                            os.path.join(mfc_dir, f), target_dir)
        except (EnvironmentError, RuntimeError) as exc:
            log.exception("Can't find an installed VC for the MFC DLLs")


    def build_exefile(self, ext):
        sources = ext.sources
        if sources is None or type(sources) not in (list, tuple):
            raise DistutilsSetupError(
                  ("in 'ext_modules' option (extension '%s'), " +
                   "'sources' must be present and must be " +
                   "a list of source filenames") % ext.name)
        sources = list(sources)

        log.info("building exe '%s'", ext.name)

        fullname = self.get_ext_fullname(ext.name)
        if self.inplace:
            # ignore build-lib -- put the compiled extension into
            # the source tree along with pure Python modules

            modpath = string.split(fullname, '.')
            package = string.join(modpath[0:-1], '.')
            base = modpath[-1]

            build_py = self.get_finalized_command('build_py')
            package_dir = build_py.get_package_dir(package)
            ext_filename = os.path.join(package_dir,
                                        self.get_ext_filename(base))
        else:
            ext_filename = os.path.join(self.build_lib,
                                        self.get_ext_filename(fullname))
        depends = sources + ext.depends
        if not (self.force or newer_group(depends, ext_filename, 'newer')):
            log.debug("skipping '%s' executable (up-to-date)", ext.name)
            return
        else:
            log.info("building '%s' executable", ext.name)

        # First, scan the sources for SWIG definition files (.i), run
        # SWIG on 'em to create .c files, and modify the sources list
        # accordingly.
        sources = self.swig_sources(sources, ext)

        # Next, compile the source code to object files.

        # XXX not honouring 'define_macros' or 'undef_macros' -- the
        # CCompiler API needs to change to accommodate this, and I
        # want to do one thing at a time!

        # Two possible sources for extra compiler arguments:
        #   - 'extra_compile_args' in Extension object
        #   - CFLAGS environment variable (not particularly
        #     elegant, but people seem to expect it and I
        #     guess it's useful)
        # The environment variable should take precedence, and
        # any sensible compiler will give precedence to later
        # command line args.  Hence we combine them in order:
        extra_args = ext.extra_compile_args or []

        macros = ext.define_macros[:]
        for undef in ext.undef_macros:
            macros.append((undef,))
        # Note: custom 'output_dir' needed due to servicemanager.pyd and
        # pythonservice.exe being built from the same .cpp file - without
        # this, distutils gets confused, as they both try and use the same
        # .obj.
        output_dir = os.path.join(self.build_temp, ext.name)
        kw = {'output_dir': output_dir,
              'macros': macros,
              'include_dirs': ext.include_dirs,
              'debug': self.debug,
              'extra_postargs': extra_args,
              'depends': ext.depends,
        }
        objects = self.compiler.compile(sources, **kw)

        # XXX -- this is a Vile HACK!
        #
        # The setup.py script for Python on Unix needs to be able to
        # get this list so it can perform all the clean up needed to
        # avoid keeping object files around when cleaning out a failed
        # build of an extension module.  Since Distutils does not
        # track dependencies, we have to get rid of intermediates to
        # ensure all the intermediates will be properly re-built.
        #
        self._built_objects = objects[:]

        # Now link the object files together into a "shared object" --
        # of course, first we have to figure out all the other things
        # that go into the mix.
        if ext.extra_objects:
            objects.extend(ext.extra_objects)
        extra_args = ext.extra_link_args or []

        # 2.2 has no 'language' support
        kw = { 'libraries': self.get_libraries(ext),
               'library_dirs': ext.library_dirs,
               'runtime_library_dirs': ext.runtime_library_dirs,
               'extra_postargs': extra_args,
               'debug': self.debug,
               'build_temp': self.build_temp,
        }

        # Detect target language, if not provided
        language = ext.language or self.compiler.detect_language(sources)
        kw["target_lang"] = language

        self.compiler.link(
            "executable",
            objects, ext_filename, **kw)

    def build_extension(self, ext):
        # It is well known that some of these extensions are difficult to
        # build, requiring various hard-to-track libraries etc.  So we
        # check the extension list for the extra libraries explicitly
        # listed.  We then search for this library the same way the C
        # compiler would - if we can't find a  library, we exclude the
        # extension from the build.
        # Note we can't do this in advance, as some of the .lib files
        # we depend on may be built as part of the process - thus we can
        # only check an extension's lib files as we are building it.
        can_build, reason = ext.can_build(self)
        if not can_build:
            self.excluded_extensions.append((ext, reason))
            log.warn("Skipping %s: %s" % (ext.name, reason))
            return
        self.current_extension = ext

        ext.finalize_options(self)

        # ensure the SWIG .i files are treated as dependencies.
        for source in ext.sources:
            if source.endswith(".i"):
                self.find_swig() # for the side-effect of the environment value.
                # Find the swig_lib .i files we care about for dependency tracking.
                ext.swig_deps = glob.glob(os.path.join(os.environ["SWIG_LIB"], "python", "*.i"))
                ext.depends.extend(ext.swig_deps)
                break
        else:
            ext.swig_deps = None

        # some source files are compiled for different extensions
        # with special defines. So we cannot use a shared
        # directory for objects, we must use a special one for each extension.
        old_build_temp = self.build_temp
        want_static_crt = ext.want_static_crt 
        if want_static_crt:
            self.compiler.compile_options.remove('/MD')
            self.compiler.compile_options.append('/MT')
            self.compiler.compile_options_debug.remove('/MDd')
            self.compiler.compile_options_debug.append('/MTd')

        try:
            distutils.command.build_ext.build_ext.build_extension(self, ext)
            # XXX This has to be changed for mingw32
            # Get the .lib files we need.  This is limited to pywintypes,
            # pythoncom and win32ui - but the first 2 have special names
            extra = self.debug and "_d.lib" or ".lib"
            if ext.name in ("pywintypes", "pythoncom"):
                # The import libraries are created as PyWinTypes23.lib, but
                # are expected to be pywintypes.lib.
                name1 = "%s%d%d%s" % (ext.name, sys.version_info[0], sys.version_info[1], extra)
                name2 = "%s%s" % (ext.name, extra)
            elif ext.name in ("win32ui",):
                name1 = name2 = ext.name + extra
            else:
                name1 = name2 = None
            if name1 is not None:
                # The compiler always creates 'pywintypes22.lib', whereas we
                # actually want 'pywintypes.lib' - copy it over.
                # Worse: 2.3+ MSVCCompiler constructs the .lib file in the same
                # directory as the first source file's object file:
                #    os.path.dirname(objects[0])
                # rather than in the self.build_temp directory
                # 2.3+ - Wrong dir, numbered name
                src = os.path.join(old_build_temp,
                                   os.path.dirname(ext.sources[0]),
                                   name1)
                dst = os.path.join(old_build_temp, name2)
                if os.path.abspath(src) != os.path.abspath(dst):
                    self.copy_file(src, dst)#, update=1)
        finally:
            self.build_temp = old_build_temp
            if want_static_crt:
                self.compiler.compile_options.remove('/MT')
                self.compiler.compile_options.append('/MD')
                self.compiler.compile_options_debug.remove('/MTd')
                self.compiler.compile_options_debug.append('/MDd')

    def get_ext_filename(self, name):
        # The pywintypes and pythoncom extensions have special names
        extra_dll = self.debug and "_d.dll" or ".dll"
        extra_exe = self.debug and "_d.exe" or ".exe"
        # *sob* - python fixed this bug in python 3.1 (bug 6403)
        # So in the fixed versions we only get the base name, and if the
        # output name is simply 'dir\name' we need to nothing.

        # The pre 3.1 pywintypes
        if name == "pywin32_system32.pywintypes":
            return r"pywin32_system32\pywintypes%d%d%s" % (sys.version_info[0], sys.version_info[1], extra_dll)
        # 3.1+ pywintypes
        elif name == "pywintypes":
            return r"pywintypes%d%d%s" % (sys.version_info[0], sys.version_info[1], extra_dll)
        # pre 3.1 pythoncom
        elif name == "pywin32_system32.pythoncom":
            return r"pywin32_system32\pythoncom%d%d%s" % (sys.version_info[0], sys.version_info[1], extra_dll)
        # 3.1+ pythoncom
        elif name == "pythoncom":
            return r"pythoncom%d%d%s" % (sys.version_info[0], sys.version_info[1], extra_dll)
        # Pre 3.1 rest.
        elif name.endswith("win32.perfmondata"):
            return r"win32\perfmondata" + extra_dll
        elif name.endswith("win32.pythonservice"):
            return r"win32\pythonservice" + extra_exe
        elif name.endswith("pythonwin.Pythonwin"):
            return r"pythonwin\Pythonwin" + extra_exe
        elif name.endswith("isapi.PyISAPI_loader"):
            return r"isapi\PyISAPI_loader" + extra_dll
        # The post 3.1 rest
        elif name in ['perfmondata', 'PyISAPI_loader']:
            return name + extra_dll
        elif name in ['pythonservice', 'Pythonwin']:
            return name + extra_exe

        return distutils.command.build_ext.build_ext.get_ext_filename(self, name)

    def get_export_symbols(self, ext):
        if ext.is_regular_dll:
            return ext.export_symbols
        return distutils.command.build_ext.build_ext.get_export_symbols(self, ext)

    def find_swig(self):
        if "SWIG" in os.environ:
            swig = os.environ["SWIG"]
        else:
            # We know where our swig is
            swig = os.path.abspath(r"swig\swig.exe")
        lib = os.path.join(os.path.dirname(swig), "swig_lib")
        os.environ["SWIG_LIB"] = lib
        return swig

    def swig_sources(self, sources, ext=None):
        new_sources = []
        swig_sources = []
        swig_targets = {}
        # XXX this drops generated C/C++ files into the source tree, which
        # is fine for developers who want to distribute the generated
        # source -- but there should be an option to put SWIG output in
        # the temp dir.
        # Adding py3k to the mix means we *really* need to move to generating
        # to the temp dir...
        target_ext = '.cpp'
        for source in sources:
            (base, sext) = os.path.splitext(source)
            if sext == ".i":             # SWIG interface file
                if os.path.split(base)[1] in config.swig_include_files:
                    continue
                swig_sources.append(source)
                # Patch up the filenames for various special cases...
                if os.path.basename(base) in config.swig_interface_parents:
                    swig_targets[source] = base + target_ext
                elif self.current_extension.name == "winxpgui" and \
                     os.path.basename(base)=="win32gui":
                    # More vile hacks.  winxpmodule is built from win32gui.i -
                    # just different #defines are setup for windows.h.
                    new_target = os.path.join(os.path.dirname(base),
                                              "winxpgui_swig%s" % (target_ext,))
                    swig_targets[source] = new_target
                    new_sources.append(new_target)
                else:
                    new_target = '%s_swig%s' % (base, target_ext)
                    new_sources.append(new_target)
                    swig_targets[source] = new_target
            else:
                new_sources.append(source)

        if not swig_sources:
            return new_sources

        swig = self.find_swig()
        for source in swig_sources:
            swig_cmd = [swig, "-python", "-c++"]
            swig_cmd.append("-dnone",) # we never use the .doc files.
            swig_cmd.extend(self.current_extension.extra_swig_commands)
            if distutils.util.get_platform() == 'win-amd64':
                swig_cmd.append("-DSWIG_PY64BIT")
            else:
                swig_cmd.append("-DSWIG_PY32BIT")
            target = swig_targets[source]
            try:
                interface_parent = config.swig_interface_parents[
                                os.path.basename(os.path.splitext(source)[0])]
            except KeyError:
                # "normal" swig file - no special win32 issues.
                pass
            else:
                # Using win32 extensions to SWIG for generating COM classes.
                if interface_parent is not None:
                    # generating a class, not a module.
                    swig_cmd.append("-pythoncom")
                    if interface_parent:
                        # A class deriving from other than the default
                        swig_cmd.extend(
                                ["-com_interface_parent", interface_parent])

            # This 'newer' check helps python 2.2 builds, which otherwise
            # *always* regenerate the .cpp files, meaning every future
            # build for any platform sees these as dirty.
            # This could probably go once we generate .cpp into the temp dir.
            fqsource = os.path.abspath(source)
            fqtarget = os.path.abspath(target)
            rebuild = self.force or (ext and newer_group(ext.swig_deps + [fqsource], fqtarget))
            log.debug("should swig %s->%s=%s", source, target, rebuild)
            if rebuild:
                swig_cmd.extend(["-o", fqtarget, fqsource])
                log.info("swigging %s to %s", source, target)
                out_dir = os.path.dirname(source)
                cwd = os.getcwd()
                os.chdir(out_dir)
                try:
                    self.spawn(swig_cmd)
                finally:
                    os.chdir(cwd)
            else:
                log.info("skipping swig of %s", source)

        return new_sources
