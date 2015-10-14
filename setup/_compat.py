__all__ = ["_winreg"]

try:
    import _winreg
except ImportError:
    import winreg as _winreg
