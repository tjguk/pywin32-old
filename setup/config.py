import os, sys
import distutils.msvccompiler

try:
    unicode
except NameError:
    UNICODE_MODE = True
else:
    UNICODE_MODE = False

from . import sdk

BUILD_ID = "219.5" # may optionally include a ".{patchno}" suffix.
PYWIN32_VERSION = "%d.%d.%s%s" % (sys.version_info[0], sys.version_info[1],
                              BUILD_ID, ".0" if "." not in BUILD_ID else "")

# some modules need a static CRT to avoid problems caused by them having a
# manifest.
#
# Now a subclass attribute
#
#~ static_crt_modules = ["winxpgui"]

# .i files that are #included, and hence are not part of the build.  Our .dsp
# parser isn't smart enough to differentiate these.
swig_include_files = "mapilib adsilib".split()
# Special definitions for SWIG.
swig_interface_parents = {
    # source file base,     "base class" for generated COM support
    'mapi':                 None, # not a class, but module
    'PyIMailUser':          'IMAPIContainer',
    'PyIABContainer':       'IMAPIContainer',
    'PyIAddrBook':          'IMAPIProp',
    'PyIAttach':            'IMAPIProp',
    'PyIDistList':          'IMAPIContainer',
    'PyIMailUser':          'IMAPIContainer',
    'PyIMAPIContainer':     'IMAPIProp',
    'PyIMAPIFolder':        'IMAPIContainer',
    'PyIMAPIProp':          '', # '' == default base
    'PyIMAPISession':       '',
    'PyIMAPIStatus':       'IMAPIProp',
    'PyIMAPITable':         '',
    'PyIMessage':           'IMAPIProp',
    'PyIMsgServiceAdmin':   '',
    'PyIMsgStore':          'IMAPIProp',
    'PyIProfAdmin':         '',
    'PyIProfSect':          'IMAPIProp',
    'PyIConverterSession':    '',
    # exchange and exchdapi
    'exchange':             None,
    'exchdapi':             None,
    'PyIExchangeManageStore': '',
    # ADSI
    'adsi':                 None, # module
    'PyIADsContainer':      'IDispatch',
    'PyIADsDeleteOps':      'IDispatch',
    'PyIADsUser':           'IADs',
    'PyIDirectoryObject':   '',
    'PyIDirectorySearch':   '',
    'PyIDsObjectPicker':   '',
    'PyIADs':   'IDispatch',
}

def _find_platform_sdk_dir():
    # Finding the Platform SDK install dir is a treat. There can be some
    # dead ends so we only consider the job done if we find the "windows.h"
    # landmark.
    # 1. The use might have their current environment setup for the
    #    SDK, in which case the "MSSdk" env var is set.
    sdks = set()    
    #
    # There may be several SDKs on the machine which satisfy our needs.
    # Use a range of techniques to gather them and the apply some heuristic
    # to decide which to select. (Probably: the highest-numbered one)
    #
    
    #
    # Special-case the MSSDK env var: if it is set to a useful SDK, just
    # use it regardless of other possibilities.
    #
    sdkdir = os.environ.get("MSSdk")
    if sdkdir and sdk.is_useful(sdkdir):
        return sdkdir
        
    # 2. The "Install Dir" value in the
    #    HKLM\Software\Microsoft\MicrosoftSDK\Directories registry key
    #    sometimes points to the right thing. However, after upgrading to
    #    the "Platform SDK for Windows Server 2003 SP1" this is dead end.
    #
    sdkdir = sdk.from_registry_value(r"Software\Microsoft\MicrosoftSDK\Directories", "Install Dir")
    if sdkdir and sdk.is_useful(sdkdir):
        sdks.add(sdkdir)
    
    # 3. Each installed SDK (not just the platform SDK) seems to have GUID
    #    subkey of HKLM\Software\Microsoft\MicrosoftSDK\InstalledSDKs and
    #    it *looks* like the latest installed Platform SDK will be the
    #    only one with an "Install Dir" sub-value.
    for sdkdir in sdk.from_registry_keys(r"Software\Microsoft\MicrosoftSDK\InstalledSDKs", "InstallDir"):
        if sdkdir and sdk.is_useful(sdkdir):
            sdks.add(sdkdir)

    sdkdir = sdk.from_registry_value(r"Software\Microsoft\Microsoft SDKs\Windows", "CurrentInstallFolder")
    if sdkdir and sdk.is_useful(sdkdir):
        sdks.add(sdkdir)
    
    # 4a. Vista's SDK when the CurrentInstallFolder isn't a complete installation
    # NB Try to find the most recent one which has a complete install; this
    # involves selecting them all and then selecting the last based on the
    # version number
    for sdkdir in sdk.from_registry_keys(r"Software\Microsoft\Microsoft SDKs\Windows", "InstallationFolder"):
        if sdkdir and sdk.is_useful(sdkdir):
            sdks.add(sdkdir)
    
    #~ log.debug("Found SDKs at: %s", "\n".join(sorted(sdks)))
    if sdks:
        sdkdir = sdk.most_useful(sdks)
        return sdkdir

    # 5. Failing this just try a few well-known default install locations.
    progfiles = os.environ.get("ProgramFiles", r"C:\Program Files")
    defaultlocs = [
        os.path.join(progfiles, "Microsoft Platform SDK"),
        os.path.join(progfiles, "Microsoft SDK"),
    ]
    for sdkdir in defaultlocs:
        #~ log.debug("PSDK: try default location: '%s'" % sdkdir)
        if sdk.is_useful(sdkdir):
            return sdkdir
    
    raise RuntimeError("No SDK to be found")

platform_sdk = _find_platform_sdk_dir()
build_version = distutils.msvccompiler.get_build_version()

