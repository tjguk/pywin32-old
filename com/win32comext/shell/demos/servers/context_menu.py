# A sample context menu handler.
# Adds a 'Hello from Python' menu entry to .py files.  When clicked, a
# simple message box is displayed.
#
# To demostrate:
# * Execute this script to register the context menu.
# * Open Windows Explorer, and browse to a directory with a .py file.
# * Right-Click on a .py file - locate and click on 'Hello from Python' on
#   the context menu.

import pythoncom
from win32com.shell import shell, shellcon
import win32gui
import win32con

IContextMenu_Methods = ["QueryContextMenu", "InvokeCommand", "GetCommandString"]
IShellExtInit_Methods = ["Initialize"]

class ShellExtension:
    _reg_progid_ = "Python.ShellExtension.ContextMenu"
    _reg_desc_ = "Python Sample Shell Extension (context menu)"
    _reg_clsid_ = "{CED0336C-C9EE-4a7f-8D7F-C660393C381F}"
    _com_interfaces_ = [shell.IID_IShellExtInit, shell.IID_IContextMenu, shell.IID_IExtractIcon, pythoncom.IID_IPersistFile]
    _public_methods_ = IContextMenu_Methods + IShellExtInit_Methods

    def Initialize(self, folder, dataobj, hkey):
        print "Init", folder, dataobj, hkey

    def QueryContextMenu(self, hMenu, indexMenu, idCmdFirst, idCmdLast, uFlags):
        print "QCM", hMenu, indexMenu, idCmdFirst, idCmdLast, uFlags
        idCmd = idCmdFirst
        items = []
        if (uFlags & 0x000F) == shellcon.CMF_NORMAL: # Check == here, since CMF_NORMAL=0
            print "CMF_NORMAL..."
            items.append("&Hello from Python")
        elif uFlags & shellcon.CMF_VERBSONLY:
            print "CMF_VERBSONLY..."
            items.append("&Hello from Python - shortcut")
        elif uFlags & shellcon.CMF_EXPLORE:
            print "CMF_EXPLORE..."
            items.append("&Hello from Python - normal file, right-click in Explorer")
        elif uFlags & CMF_DEFAULTONLY:
            print "CMF_DEFAULTONLY...\r\n"
        else:
            print "** unknown flags", uFlags
        win32gui.InsertMenu(hMenu, indexMenu,
                            win32con.MF_SEPARATOR|win32con.MF_BYPOSITION,
                            0, None)
        indexMenu += 1
        for item in items:
            win32gui.InsertMenu(hMenu, indexMenu,
                                win32con.MF_STRING|win32con.MF_BYPOSITION,
                                idCmd, item)
            indexMenu += 1
            idCmd += 1

        win32gui.InsertMenu(hMenu, indexMenu,
                            win32con.MF_SEPARATOR|win32con.MF_BYPOSITION,
                            0, None)
        indexMenu += 1
        return idCmd-idCmdFirst # Must return number of menu items we added.

    def InvokeCommand(self, ci):
        mask, hwnd, verb, params, dir, nShow, hotkey, hicon = ci
        win32gui.MessageBox(hwnd, "Hello", "Wow", win32con.MB_OK)

    def GetCommandString(self, cmd, typ):
        return "Hello from Python!!"

def DllRegisterServer():
    import _winreg
    key = _winreg.CreateKey(_winreg.HKEY_CLASSES_ROOT,
                            "Python.File\\shellex")
    subkey = _winreg.CreateKey(key, "ContextMenuHandlers")
    subkey2 = _winreg.CreateKey(subkey, "PythonSample")
    _winreg.SetValueEx(subkey2, None, 0, _winreg.REG_SZ, ShellExtension._reg_clsid_)
    print ShellExtension._reg_desc_, "registration complete."

def DllUnregisterServer():
    import _winreg
    try:
        key = _winreg.DeleteKey(_winreg.HKEY_CLASSES_ROOT,
                                "Python.File\\shellex\\ContextMenuHandlers\\PythonSample")
    except WindowsError, details:
        import errno
        if details.errno != errno.ENOENT:
            raise
    print ShellExtension._reg_desc_, "unregistration complete."

if __name__=='__main__':
    from win32com.server import register
    register.UseCommandLine(ShellExtension,
                   finalize_register = DllRegisterServer,
                   finalize_unregister = DllUnregisterServer)
