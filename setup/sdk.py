import os, sys

from ._compat import *
from . import logging
log = logging.logger(__package__)

def is_useful(dirpath, landmarks={"include/windows.h"}):
    """SDKs come in all levels of completeness: it's quite usual to
    have an SDK directory with only a few files, or only binaries or
    only a particular subset of the SDKs we'd need. We look for a few
    landmark files which would make the SDK usable by us.
    
    NB since the os.path.exists check will fail just as well if the entire
    directory doesn't exist, we can happily pass any directory to this
    function to get a useful result without needing to check first whether
    it exists!
    """
    log.debug("Checking SDK %s for usefulness", dirpath)
    return all(os.path.exists(os.path.join(dirpath, landmark)) for landmark in landmarks)

def most_useful(dirpaths):
    """From a set of SDK directories, all of which have been deemed useful,
    select one to use. It's assumed that the final segment of the directory
    name represents a version (6.0, 7.1A etc.) and that sorting this
    lexicographically will give a meaningful "latest" one.
    """
    return max(dirpaths, key=os.path.basename)

def from_registry_key(hkey, value):
    try:
        sdkdir, _ = _winreg.QueryValueEx(hkey, value)
    except EnvironmentError:
        return None
    else:
        return os.path.abspath(sdkdir).rstrip("\\")
    
def from_registry_value(subkey, value, hive=_winreg.HKEY_LOCAL_MACHINE):
    """Look for a possible sdk directory from a registry value. Either
    of the subkey and the value might not exist, and
    the resulting SDK dir might not be useful.
    """
    log.debug("Check for SDK in %s:%s", subkey, value)
    try:
        hkey = _winreg.OpenKey(hive, subkey)
    except EnvironmentError:
        return None
    else:
        return from_registry_key(hkey, value)

def from_registry_keys(subkey, value, hive=_winreg.HKEY_LOCAL_MACHINE):
    """Look for possible sdk directories from a defined value in the 
    keys below a registry key. Any of the keys or values might not exist.
    """
    log.debug("Check for SDK in %s:%s", subkey, value)
    try:
        hkey = _winreg.OpenKey(hive, subkey)
    except EnvironmentError:
        pass
    else:
        i = 0
        while True:
            try:
                sdk_version = _winreg.EnumKey(hkey, i)
            except EnvironmentError:
                break
            sdk_version_key = _winreg.OpenKey(hkey, sdk_version)
            yield from_registry_key(sdk_version_key, value)

            i += 1

