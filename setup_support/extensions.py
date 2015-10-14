import os, sys
import distutils.core
import string

from . import config

class WinExt(distutils.core.Extension):
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
        self.is_win32_exe = False

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

