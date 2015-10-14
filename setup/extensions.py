import os, sys
import distutils.core
import re
import string

from ._compat import *
from . import config
from . import logging
log = logging.logger(__package__)

class WinExt(distutils.core.Extension):
    
    is_win32_exe = False
    want_static_crt = False
    pywin32_dir = None
    
    # Base class for all win32 extensions, with some predefined
    # library and include dirs, and predefined windows libraries.
    # Additionally a method to parse .def files into lists of exported
    # symbols, and to read
    def __init__ (self, name, sources,
                  include_dirs=[],
                  define_macros=None,
                  undef_macros=None,
                  library_dirs=[],
                  libraries="",
                  runtime_library_dirs=None,
                  extra_objects=None,
                  extra_compile_args=None,
                  extra_link_args=None,
                  export_symbols=None,
                  export_symbol_file=None,
                  pch_header=None,
                  windows_h_version=None, # min version of windows.h needed.
                  extra_swig_commands=None,
                  is_regular_dll=False, # regular Windows DLL?
                  # list of headers which may not be installed forcing us to
                  # skip this extension
                  optional_headers=[],
                  base_address = None,
                  depends=None,
                  platforms=None, # none means 'all platforms'
                  unicode_mode=None, # 'none'==default or specifically true/false.
                  implib_name=None,
                  delay_load_libraries="",
                 ):
        libary_dirs = library_dirs,
        include_dirs = ['com/win32com/src/include',
                        'win32/src'] + include_dirs
        libraries=libraries.split()
        self.delay_load_libraries=delay_load_libraries.split()
        libraries.extend(self.delay_load_libraries)

        if export_symbol_file:
            export_symbols = export_symbols or []
            export_symbols.extend(self.parse_def_file(export_symbol_file))

        # Some of our swigged files behave differently in distutils vs
        # MSVC based builds.  Always define DISTUTILS_BUILD so they can tell.
        define_macros = define_macros or []
        define_macros.append(("DISTUTILS_BUILD", None))
        define_macros.append(("_CRT_SECURE_NO_WARNINGS", None))
        self.pch_header = pch_header
        self.extra_swig_commands = extra_swig_commands or []
        self.windows_h_version = windows_h_version
        self.optional_headers = optional_headers
        self.is_regular_dll = is_regular_dll
        self.base_address = base_address
        self.platforms = platforms
        self.implib_name = implib_name
        distutils.core.Extension.__init__ (self, name, sources,
                            include_dirs,
                            define_macros,
                            undef_macros,
                            library_dirs,
                            libraries,
                            runtime_library_dirs,
                            extra_objects,
                            extra_compile_args,
                            extra_link_args,
                            export_symbols)
        self.depends = depends or [] # stash it here, as py22 doesn't have it.
        self.unicode_mode = unicode_mode

    def parse_def_file(self, path):
        # Extract symbols to export from a def-file
        result = []
        for line in open(path).readlines():
            line = line.rstrip()
            if line and line[0] in string.whitespace:
                tokens = line.split()
                if not tokens[0][0] in string.ascii_letters:
                    continue
                result.append(','.join(tokens))
        return result

    def get_source_files(self, dsp):
        result = []
        if dsp is None:
            return result
        dsp_path = os.path.dirname(dsp)
        seen_swigs = []
        for line in open(dsp, "r"):
            fields = line.strip().split("=", 2)
            if fields[0]=="SOURCE":
                ext = os.path.splitext(fields[1])[1].lower()
                if ext in ['.cpp', '.c', '.i', '.rc', '.mc']:
                    pathname = os.path.normpath(os.path.join(dsp_path, fields[1]))
                    result.append(pathname)
                    if ext == '.i':
                        seen_swigs.append(pathname)

        # ack - .dsp files may have references to the generated 'foomodule.cpp'
        # from 'foo.i' - but we now do things differently...
        for ss in seen_swigs:
            base, ext = os.path.splitext(ss)
            nuke = base + "module.cpp"
            try:
                result.remove(nuke)
            except ValueError:
                pass
        # Sort the sources so that (for example) the .mc file is processed first,
        # building this may create files included by other source files.
        build_order = ".i .mc .rc .cpp".split()
        decorated = [(build_order.index(os.path.splitext(fname)[-1].lower()), fname)
                     for fname in result]
        decorated.sort()
        result = [item[1] for item in decorated]
        return result

    def finalize_options(self, build_ext):
        # distutils doesn't define this function for an Extension - it is
        # our own invention, and called just before the extension is built.
        if not build_ext.mingw32:
            if self.pch_header:
                self.extra_compile_args = self.extra_compile_args or []
                # /YX doesn't work in vs2008 or vs2003/64
                if build_ext.plat_name == 'win32' and config.build_version < 9.0:
                    self.extra_compile_args.append("/YX"+self.pch_header)
                pch_name = os.path.join(build_ext.build_temp, self.name) + ".pch"
                self.extra_compile_args.append("/Fp"+pch_name)

            # bugger - add this to python!
            if build_ext.plat_name=="win32":
                self.extra_link_args.append("/MACHINE:x86")
            else:
                self.extra_link_args.append("/MACHINE:%s" % build_ext.plat_name[4:])

            # Old vs2003 needs this defined (Python itself uses it)
            if config.build_version < 9.0 and build_ext.plat_name=="win-amd64":
                self.extra_compile_args.append('/D_M_X64')

            # Put our DLL base address in (but not for our executables!)
            if not self.is_win32_exe:
                base = self.base_address
                if not base:
                    base = self.__class__.dll_base_addresses[self.name]
                self.extra_link_args.append("/BASE:0x%x" % (base,))

            # like Python, always use debug info, even in release builds
            # (note the compiler doesn't include debug info, so you only get
            # basic info - but its better than nothing!)
            # For now use the temp dir - later we may package them, so should
            # maybe move them next to the output file.
            pch_dir = os.path.join(build_ext.build_temp)
            if not build_ext.debug:
                self.extra_compile_args.append("/Zi")
            self.extra_compile_args.append("/Fd%s\%s_vc.pdb" %
                                          (pch_dir, self.name))
            self.extra_link_args.append("/DEBUG")
            self.extra_link_args.append("/PDB:%s\%s.pdb" %
                                       (pch_dir, self.name))
            # enable unwind semantics - some stuff needs it and I can't see
            # it hurting
            self.extra_compile_args.append("/EHsc")

            if self.delay_load_libraries:
                self.libraries.append("delayimp")
                for delay_lib in self.delay_load_libraries:
                    self.extra_link_args.append("/delayload:%s.dll" % delay_lib)

            # If someone needs a specially named implib created, handle that
            if self.implib_name:
                implib = os.path.join(build_ext.build_temp, self.implib_name)
                if build_ext.debug:
                    suffix = "_d"
                else:
                    suffix = ""
                self.extra_link_args.append("/IMPLIB:%s%s.lib" % (implib, suffix))
            # Try and find the MFC source code, so we can reach inside for
            # some of the ActiveX support we need.  We need to do this late, so
            # the environment is setup correctly.
            # Only used by the win32uiole extensions, but I can't be
            # bothered making a subclass just for this - so they all get it!
            found_mfc = False
            for incl in os.environ.get("INCLUDE", "").split(os.pathsep):
                # first is a "standard" MSVC install, second is the Vista SDK.
                for candidate in ("..\src\occimpl.h", "..\..\src\mfc\occimpl.h"):
                    check = os.path.join(incl, candidate)
                    if os.path.isfile(check):
                        self.extra_compile_args.append('/DMFC_OCC_IMPL_H=\\"%s\\"' % candidate)
                        found_mfc = True
                        break
                if found_mfc:
                    break
            # Handle Unicode - if unicode_mode is None, then it means True
            # for py3k, false for py2
            unicode_mode = self.unicode_mode
            if unicode_mode is None:
                unicode_mode = config.UNICODE_MODE
            if unicode_mode:
                self.extra_compile_args.append("/DUNICODE")
                self.extra_compile_args.append("/D_UNICODE")
                self.extra_compile_args.append("/DWINNT")
                # Unicode, Windows executables seem to need this magic:
                if "/SUBSYSTEM:WINDOWS" in self.extra_link_args:
                    self.extra_link_args.append("/ENTRY:wWinMainCRTStartup")
    
    def get_pywin32_dir(self):
        return self.pywin32_dir
    
    def can_build(self, builder):
        """Return (can_build, why_not) where can_build is a boolean & why_not is a string
        when why_not is False (and ignored otherwise).
        """
        return True, None

    def _why_cant_build(self, builder):
        """Return None if no reason not to build, or a string containing a reason
        
        Called by build_ext.build_extension before attempting to build
        """
        if self.windows_h_version is not None and \
           self.windows_h_version > builder.windows_h_version:
            return "WINDOWS.H with version 0x%x is required, but only " \
                   "version 0x%x is installed." \
                   % (self.windows_h_version, builder.windows_h_version)

        look_dirs = builder.include_dirs
        for h in self.optional_headers:
            for d in look_dirs:
                if os.path.isfile(os.path.join(d, h)):
                    break
            else:
                log.debug("Looked for %s in %s", h, look_dirs)
                return "The header '%s' can not be located" % (h,)

        common_dirs = builder.compiler.library_dirs[:]
        common_dirs += os.environ.get("LIB", "").split(os.pathsep)
        patched_libs = []
        for lib in self.libraries:
            if lib.lower() in builder.found_libraries:
                found = builder.found_libraries[lib.lower()]
            else:
                look_dirs = common_dirs + self.library_dirs
                found = builder.compiler.find_library_file(look_dirs, lib, builder.debug)
                if not found:
                    log.debug("Looked for %s in %s", lib, look_dirs)
                    return "No library '%s'" % lib
                builder.found_libraries[lib.lower()] = found
            patched_libs.append(os.path.splitext(os.path.basename(found))[0])

        if self.platforms and builder.plat_name not in ext.platforms:
            return "Only available on platforms %s" % (self.platforms,)

        # We update the .libraries list with the resolved library name.
        # This is really only so "_d" works.
        self.libraries = patched_libs
        return None # no reason - it can be built!

class WinExt_pythonwin(WinExt):
    
    pywin32_dir = "pythonwin"
    
    def __init__ (self, name, **kw):
        if 'unicode_mode' not in kw:
            kw['unicode_mode']=None
        kw.setdefault("extra_compile_args", []).extend(
                            ['-D_AFXDLL', '-D_AFXEXT','-D_MBCS'])

        WinExt.__init__(self, name, **kw)

class WinExt_pythonwin_exe(WinExt_pythonwin):
    is_win32_exe = True
    
class WinExt_win32(WinExt):
    pywin32_dir = "win32"

class WinExt_win32_static_crt(WinExt_win32):
    want_static_crt = True

class WinExt_pythonservice_exe(WinExt_win32):
    is_win32_exe = True

class WinExt_ISAPI(WinExt):
    pywin32_dir = "isapi"

# Note this is used only for "win32com extensions", not pythoncom
# itself - thus, output is "win32comext"
class WinExt_win32com(WinExt):
    def __init__ (self, name, **kw):
        kw["libraries"] = kw.get("libraries", "") + " oleaut32 ole32"

        # COM extensions require later windows headers.
        if not kw.get("windows_h_version"):
            kw["windows_h_version"] = 0x500
        WinExt.__init__(self, name, **kw)
    def get_pywin32_dir(self):
        return "win32comext/" + self.name

# Exchange extensions get special treatment:
# * Look for the Exchange SDK in the registry.
# * Output directory is different than the module's basename.
# * Require use of the Exchange 2000 SDK - this works for both VC6 and 7
class WinExt_win32com_mapi(WinExt_win32com):
    
    # 'win32com.mapi.exchange' and 'win32com.mapi.exchdapi' currently only
    # ones with this special requirement
    pywin32_dir = "win32comext/mapi"
    
    def __init__ (self, name, **kw):
        # The Exchange 2000 SDK seems to install itself without updating
        # LIB or INCLUDE environment variables.  It does register the core
        # directory in the registry tho - look it up
        sdk_install_dir = None
        libs = kw.get("libraries", "")
        keyname = "SOFTWARE\Microsoft\Exchange\SDK"
        flags = _winreg.KEY_READ
        try:
            flags |= _winreg.KEY_WOW64_32KEY
        except AttributeError:
            pass # this version doesn't support 64 bits, so must already be using 32bit key.
        for root in _winreg.HKEY_LOCAL_MACHINE, _winreg.HKEY_CURRENT_USER:
            try:
                keyob = _winreg.OpenKey(root, keyname, 0, flags)
                value, type_id = _winreg.QueryValueEx(keyob, "INSTALLDIR")
                if type_id == _winreg.REG_SZ:
                    sdk_install_dir = value
                    break
            except WindowsError:
                pass
        if sdk_install_dir is not None:
            d = os.path.join(sdk_install_dir, "SDK", "Include")
            if os.path.isdir(d):
                kw.setdefault("include_dirs", []).insert(0, d)
            d = os.path.join(sdk_install_dir, "SDK", "Lib")
            if os.path.isdir(d):
                kw.setdefault("library_dirs", []).insert(0, d)

        # The stand-alone exchange SDK has these libs
        if distutils.util.get_platform() == 'win-amd64':
            # Additional utility functions are only available for 32-bit builds.
            pass
        else:
            libs += " version user32 advapi32 Ex2KSdk sadapi netapi32"
        kw["libraries"] = libs
        WinExt_win32com.__init__(self, name, **kw)

    def _why_cant_build(self, builder):
        why = WinExt_win32com._why_cant_build(self, builder)
        if why is None:
            # Exclude exchange 32-bit utility libraries from 64-bit
            # builds. Note that the exchange module now builds, but only
            # includes interfaces for 64-bit builds.
            if builder.plat_name == 'win-amd64':
                return "No 64-bit library for utility functions available."
        else:
            return why

class WinExt_win32com_axdebug(WinExt_win32com):
    def __init__ (self, name, **kw):
        # Later SDK versions again ship with activdbg.h, but if we attempt
        # to use our own copy of that file with that SDK, we fail to link.
        if os.path.isfile(os.path.join(config.platform_sdk, "include", "activdbg.h")):
            kw.setdefault('extra_compile_args', []).append("/DHAVE_SDK_ACTIVDBG")
        WinExt_win32com.__init__(self, name, **kw)

# A hacky extension class for pywintypesXX.dll and pythoncomXX.dll
class WinExt_system32(WinExt):
    pywin32_dir = "pywin32_system32"
