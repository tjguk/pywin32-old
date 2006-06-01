// This file implements the IShellLink Interface and Gateway for Python.
// Generated by makegw.py

#include "shell_pch.h"
#include "PyIShellLink.h"

// We should not be using this!
#define OleSetOleError PyCom_BuildPyException

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIShellLink::PyIShellLink(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyIShellLink::~PyIShellLink()
{
}

/* static */ IShellLink *PyIShellLink::GetI(PyObject *self)
{
	return (IShellLink *)PyIUnknown::GetI(self);
}

// @pymethod name, <o WIN32_FIND_DATA>|PyIShellLink|GetPath|Retrieves the path and file name of a shell link object
// @comm The AlternateFileName (8.3) member of WIN32_FIND_DATA does not return information
PyObject *PyIShellLink::GetPath(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	WIN32_FIND_DATA fd;
	// @pyparm int|fFlags||One of the following values:
	// @flagh Value|Description
	// @flag SLGP_SHORTPATH|Retrieves the standard short (8.3 format) file name.  
	// @flag SLGP_UNCPRIORITY|Retrieves the Universal Naming Convention (UNC) path name of the file.  
	// @flag SLGP_RAWPATH|Retrieves the raw path name. A raw path is something that might not exist and may include environment variables that need to be expanded. 
	// @pyparm int|cchMaxPath|_MAX_PATH|Description for cchMaxPath
	int cchMaxPath = _MAX_PATH;
	DWORD fFlags;
	if ( !PyArg_ParseTuple(args, "l|i:GetPath", &fFlags, &cchMaxPath) )
		return NULL;
	HRESULT hr;
	TCHAR *pszFile = (TCHAR *)malloc(cchMaxPath * sizeof(TCHAR));
	if (pszFile==NULL) {
		PyErr_SetString(PyExc_MemoryError, "allocating string buffer");
		return NULL;
	}
	ZeroMemory(&fd, sizeof(fd));
	PY_INTERFACE_PRECALL;
	hr = pISL->GetPath( pszFile, cchMaxPath, &fd, fFlags );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) ) {
		free(pszFile);
		return OleSetOleError(hr);
	}
	PyObject *obFD = PyObject_FromWIN32_FIND_DATAA(&fd);
	PyObject *obFile = PyWinObject_FromTCHAR(pszFile);
	PyObject *ret = Py_BuildValue("NN", obFile, obFD);
	free(pszFile);
	return ret;
}

// @pymethod string|PyIShellLink|GetIDList|Retrieves the list of item identifiers for a shell link object.
PyObject *PyIShellLink::GetIDList(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, ":GetIDList") )
		return NULL;
	HRESULT hr;
	LPITEMIDLIST pidl = NULL;
	PY_INTERFACE_PRECALL;
	hr = pISL->GetIDList( &pidl );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	return PyObject_FromPIDL(pidl, TRUE);
}

// @pymethod |PyIShellLink|SetIDList|Sets the list of item identifiers for a shell link object.
PyObject *PyIShellLink::SetIDList(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	LPITEMIDLIST pidl;
	PyObject *obpidl;
	if ( !PyArg_ParseTuple(args, "O:SetIDList", &obpidl) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (bPythonIsHappy && !PyObject_AsPIDL( obpidl, &pidl )) bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetIDList( pidl );
	PY_INTERFACE_POSTCALL;

	PyObject_FreePIDL(pidl);

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod |PyIShellLink|GetDescription|Description of GetDescription.
PyObject *PyIShellLink::GetDescription(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	// @pyparm int|cchMaxName|1024|Description for cchMaxName
	int cchMaxName = 1024;
	if ( !PyArg_ParseTuple(args, "|i:GetDescription", &cchMaxName) )
		return NULL;
	HRESULT hr;
	TCHAR *pszName = (TCHAR *)malloc(cchMaxName * sizeof(TCHAR) );
	if (pszName==NULL) {
		PyErr_SetString(PyExc_MemoryError, "allocating string buffer");
		return NULL;
	}
	PY_INTERFACE_PRECALL;
	hr = pISL->GetDescription( pszName, cchMaxName );
	PY_INTERFACE_POSTCALL;

	PyObject *ret;
	if ( FAILED(hr) )
		ret = OleSetOleError(hr);
	else
		ret = PyWinObject_FromTCHAR(pszName);
	free(pszName);
	return ret;
}

// @pymethod |PyIShellLink|SetDescription|Description of SetDescription.
PyObject *PyIShellLink::SetDescription(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	PyObject *obName;
	if ( !PyArg_ParseTuple(args, "O:SetDescription", &obName) )
		return NULL;
	TCHAR *pszName;
	if (!PyWinObject_AsTCHAR(obName, &pszName))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetDescription( pszName );
	PY_INTERFACE_POSTCALL;
	PyWinObject_FreeTCHAR(pszName);

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod |PyIShellLink|GetWorkingDirectory|Description of GetWorkingDirectory.
PyObject *PyIShellLink::GetWorkingDirectory(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	// @pyparm int|cchMaxName|1024|Description for cchMaxName
	int cchMaxName = 1024;
	if ( !PyArg_ParseTuple(args, "|i:GetWorkingDirectory", &cchMaxName) )
		return NULL;
	HRESULT hr;
	TCHAR *pszName = (TCHAR *)malloc(cchMaxName * sizeof(TCHAR) );
	if (pszName==NULL) {
		PyErr_SetString(PyExc_MemoryError, "allocating string buffer");
		return NULL;
	}
	PY_INTERFACE_PRECALL;
	hr = pISL->GetWorkingDirectory( pszName, cchMaxName );
	PY_INTERFACE_POSTCALL;

	PyObject *ret;
	if ( FAILED(hr) )
		ret = OleSetOleError(hr);
	else
		ret = PyWinObject_FromTCHAR(pszName);
	free(pszName);
	return ret;
}

// @pymethod |PyIShellLink|SetWorkingDirectory|Description of SetWorkingDirectory.
PyObject *PyIShellLink::SetWorkingDirectory(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	PyObject *obName;
	if ( !PyArg_ParseTuple(args, "O:SetWorkingDirectory", &obName) )
		return NULL;
	TCHAR *pszName;
	if (!PyWinObject_AsTCHAR(obName, &pszName))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetWorkingDirectory( pszName );
	PY_INTERFACE_POSTCALL;
	PyWinObject_FreeTCHAR(pszName);

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod string|PyIShellLink|GetArguments|Retrieves the command-line arguments associated with a shell link object.
PyObject *PyIShellLink::GetArguments(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	// @pyparm int|cchMaxName|1024|Number of characters to fetch.
	int cchMaxName = 1024;
	if ( !PyArg_ParseTuple(args, "|i:GetArguments", &cchMaxName) )
		return NULL;
	HRESULT hr;
	TCHAR *pszName = (TCHAR *)malloc(cchMaxName * sizeof(TCHAR) );
	if (pszName==NULL) {
		PyErr_SetString(PyExc_MemoryError, "allocating string buffer");
		return NULL;
	}
	PY_INTERFACE_PRECALL;
	hr = pISL->GetArguments( pszName, cchMaxName );
	PY_INTERFACE_POSTCALL;

	PyObject *ret;
	if ( FAILED(hr) )
		ret = OleSetOleError(hr);
	else
		ret = PyWinObject_FromTCHAR(pszName);
	free(pszName);
	return ret;
}

// @pymethod |PyIShellLink|SetArguments|Sets the command-line arguments associated with a shell link object.
PyObject *PyIShellLink::SetArguments(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	PyObject *obArgs;
	// @pyparm string|args||The new arguments.
	if ( !PyArg_ParseTuple(args, "O:SetArguments", &obArgs) )
		return NULL;
	TCHAR *pszArgs;
	if (!PyWinObject_AsTCHAR(obArgs, &pszArgs))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetArguments( pszArgs );
	PY_INTERFACE_POSTCALL;
	PyWinObject_FreeTCHAR(pszArgs);

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod int|PyIShellLink|GetHotkey|Retrieves the hot key for a shell link object.
PyObject *PyIShellLink::GetHotkey(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	HRESULT hr;
	WORD hotkey;
	PY_INTERFACE_PRECALL;
	hr = pISL->GetHotkey( &hotkey );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	return PyInt_FromLong(hotkey);
	// @comm Note: My tests do not seem to be working. at least, the values returned
	// seem not to match what the documentation says should be returned.
	// I would expect with a Hotkey of CTRL-ALT-T, to get an integer where
	// integer & 256 == ord('T'), i.e. 116 or 84, instead I get 1620
}

// @pymethod |PyIShellLink|SetHotkey|Sets the hot key for a shell link object.
PyObject *PyIShellLink::SetHotkey(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	// @pyparm int|wHotkey||The virtual key code is in the low-order byte, and the modifier
	// flags are in the high-order byte. The modifier flags can be a combination of the
	// values specified in the description of the <om PyIShellLink::GetHotkey> method.
	WORD wHotkey;
	if ( !PyArg_ParseTuple(args, "i:SetHotkey", &wHotkey) )
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetHotkey( wHotkey );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod int|PyIShellLink|GetShowCmd|Retrieves the show (SW_) command for a shell link object.
PyObject *PyIShellLink::GetShowCmd(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	HRESULT hr;
	int iShowCmd;
	PY_INTERFACE_PRECALL;
	hr = pISL->GetShowCmd( &iShowCmd );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	return PyInt_FromLong(iShowCmd);
}

// @pymethod |PyIShellLink|SetShowCmd|Sets the show (SW_) command for a shell link object.
PyObject *PyIShellLink::SetShowCmd(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	// @pyparm int|iShowCmd||The new show command value.
	int iShowCmd;
	if ( !PyArg_ParseTuple(args, "i:SetShowCmd", &iShowCmd) )
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetShowCmd( iShowCmd );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod string|PyIShellLink|GetIconLocation|Retrieves the location (path and index) of the icon for a shell link object.
PyObject *PyIShellLink::GetIconLocation(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	// @pyparm int|cchMaxPath|_MAX_PATH|Number of characters to allocate for the result string.
	int cchIconPath = _MAX_PATH;
	if ( !PyArg_ParseTuple(args, "|i:GetIconLocation", &cchIconPath) )
		return NULL;
	TCHAR *pszIconPath = (TCHAR *)malloc(cchIconPath * sizeof(TCHAR) );
	if (pszIconPath==NULL) {
		PyErr_SetString(PyExc_MemoryError, "allocating string buffer");
		return NULL;
	}
	HRESULT hr;
	int iIcon;
	PY_INTERFACE_PRECALL;
	hr = pISL->GetIconLocation( pszIconPath, cchIconPath, &iIcon );
	PY_INTERFACE_POSTCALL;

	PyObject *ret;
	if ( FAILED(hr) )
		ret = OleSetOleError(hr);
	else
		ret = Py_BuildValue("Ni", PyWinObject_FromTCHAR(pszIconPath), iIcon);
	free(pszIconPath);
	return ret;
}

// @pymethod |PyIShellLink|SetIconLocation|Sets the location (path and index) of the icon for a shell link object.
PyObject *PyIShellLink::SetIconLocation(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	PyObject *obIconPath;
	// @pyparm string|iconPath||Path to the file with the icon.
	// @pyparm int|iIcon||Index of the icon.
	int iIcon;
	if ( !PyArg_ParseTuple(args, "Oi:SetIconLocation", &obIconPath, &iIcon) )
		return NULL;
	TCHAR *pszIconPath;
	if (!PyWinObject_AsTCHAR(obIconPath, &pszIconPath))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetIconLocation( pszIconPath, iIcon );
	PY_INTERFACE_POSTCALL;
	PyWinObject_FreeTCHAR(pszIconPath);

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIShellLink|SetRelativePath|Sets the relative path for a shell link object.
PyObject *PyIShellLink::SetRelativePath(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	PyObject *obPathRel;
	// @pyparm string|relPath||The relative path.
	// @pyparm int|reserved|0|Reserved - must be zero.
	DWORD dwReserved = 0;
	if ( !PyArg_ParseTuple(args, "O|l:SetRelativePath", &obPathRel, &dwReserved) )
		return NULL;
	TCHAR *pszPathRel;
	if (!PyWinObject_AsTCHAR(obPathRel, &pszPathRel))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetRelativePath( pszPathRel, dwReserved );
	PY_INTERFACE_POSTCALL;
	PyWinObject_FreeTCHAR(pszPathRel);

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;
	// @comm This mechanism allows for moved link files
	// to reestablish connection with relative files through
	// similar-prefix comparisons
}

// @pymethod |PyIShellLink|Resolve|Resolves a shell link by searching for the shell link object and updating the
// shell link path and its list of identifiers (if necessary)
PyObject *PyIShellLink::Resolve(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	// @pyparm HWND|hwnd||The parent window of a dialog which will pop up if resolution fails.
	// @pyparm int|fFlags||One of the following constants:
	// @flagh Value|Description
	// @flag SLR_INVOKE_MSI|Call the Microsoft Windows Installer. 
	// @flag SLR_NOLINKINFO |Disable distributed link tracking. By default, distributed
	//			link tracking tracks removable media across multiple devices based on the
	//			volume name. It also uses the UNC path to track remote file systems whose
	//			drive letter has changed. Setting SLR_NOLINKINFO disables both types of tracking. 
	// @flag SLR_NO_UI|Do not display a dialog box if the link cannot be resolved. When
	//			SLR_NO_UI is set, the high-order word of fFlags can be set to a time-out value
	//			that specifies the maximum amount of time to be spent resolving the link. The
	//			function returns if the link cannot be resolved within the time-out duration.
	//			If the high-order word is set to zero, the time-out duration will be set to the
	//			default value of 3,000 milliseconds (3 seconds). To specify a value, set the high
	//			word of fFlags to the desired time-out duration, in milliseconds. 
	// @flag SLR_NOUPDATE|Do not update the link information. 
	// @flag SLR_NOSEARCH|Do not execute the search heuristics. 
	// @flag SLR_NOTRACK|Do not use distributed link tracking. 
	// @flag SLR_UPDATE|If the link object has changed, update its path and list of identifiers. If SLR_UPDATE is set, you do not need to call IPersistFile::IsDirty to determine whether or not the link object has changed. 

	HWND hwnd;
	DWORD fFlags;
	if ( !PyArg_ParseTuple(args, "ll:Resolve", &hwnd, &fFlags) )
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->Resolve( hwnd, fFlags );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIShellLink|SetPath|Sets the path and file name of a shell link object.
PyObject *PyIShellLink::SetPath(PyObject *self, PyObject *args)
{
	IShellLink *pISL = GetI(self);
	if ( pISL == NULL )
		return NULL;
	PyObject *obName;
	if ( !PyArg_ParseTuple(args, "O:SetDescription", &obName) )
		return NULL;
	TCHAR *pszName;
	if (!PyWinObject_AsTCHAR(obName, &pszName))
		return NULL;
	// @pyparm string|path||The path and filename of the link.
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISL->SetPath( pszName );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return OleSetOleError(hr);
	Py_INCREF(Py_None);
	return Py_None;
}

// @object PyIShellLink|Description of the interface
static struct PyMethodDef PyIShellLink_methods[] =
{
	{ "GetPath", PyIShellLink::GetPath, 1 }, // @pymeth GetPath|Retrieves the path and file name of a shell link object.
	{ "GetIDList", PyIShellLink::GetIDList, 1 }, // @pymeth GetIDList|Retrieves the list of item identifiers for a shell link object.
	{ "SetIDList", PyIShellLink::SetIDList, 1 }, // @pymeth SetIDList|Sets the list of item identifiers for a shell link object.
	{ "GetDescription", PyIShellLink::GetDescription, 1 }, // @pymeth GetDescription|Description of GetDescription
	{ "SetDescription", PyIShellLink::SetDescription, 1 }, // @pymeth SetDescription|Description of SetDescription
	{ "GetWorkingDirectory", PyIShellLink::GetWorkingDirectory, 1 }, // @pymeth GetWorkingDirectory|Description of GetWorkingDirectory
	{ "SetWorkingDirectory", PyIShellLink::SetWorkingDirectory, 1 }, // @pymeth SetWorkingDirectory|Description of SetWorkingDirectory
	{ "GetArguments", PyIShellLink::GetArguments, 1 }, // @pymeth GetArguments|Retrieves the command-line arguments associated with a shell link object.
	{ "SetArguments", PyIShellLink::SetArguments, 1 }, // @pymeth SetArguments|Sets the command-line arguments associated with a shell link object.
	{ "GetHotkey", PyIShellLink::GetHotkey, 1 }, // @pymeth GetHotkey|Retrieves the hot key for a shell link object.
	{ "SetHotkey", PyIShellLink::SetHotkey, 1 }, // @pymeth SetHotkey|Sets the hot key for a shell link object.
	{ "GetShowCmd", PyIShellLink::GetShowCmd, 1 }, // @pymeth GetShowCmd|Retrieves the show (SW_) command for a shell link object.
	{ "SetShowCmd", PyIShellLink::SetShowCmd, 1 }, // @pymeth SetShowCmd|Sets the show (SW_) command for a shell link object.
	{ "GetIconLocation", PyIShellLink::GetIconLocation, 1 }, // @pymeth GetIconLocation|Retrieves the location (path and index) of the icon for a shell link object.
	{ "SetIconLocation", PyIShellLink::SetIconLocation, 1 }, // @pymeth SetIconLocation|Sets the location (path and index) of the icon for a shell link object.
	{ "SetRelativePath", PyIShellLink::SetRelativePath, 1 }, // @pymeth SetRelativePath|Sets the relative path for a shell link object.
	{ "Resolve", PyIShellLink::Resolve, 1 }, // @pymeth Resolve|Resolves a shell link
	{ "SetPath", PyIShellLink::SetPath, 1 }, // @pymeth SetPath|Sets the path and file name of a shell link object.
	{ NULL }
};

PyComTypeObject PyIShellLink::type("PyIShellLink",
		&PyIUnknown::type,
		sizeof(PyIShellLink),
		PyIShellLink_methods,
		GET_PYCOM_CTOR(PyIShellLink));
// ---------------------------------------------------
//
// Gateway Implementation
// Python never needs to implement this!!!
/**********************************
STDMETHODIMP PyGShellLink::GetPath(
		 LPSTR pszFile,
		int cchMaxPath,
		WIN32_FIND_DATAA * pfd,
		DWORD fFlags)
{
	PY_GATEWAY_METHOD;
// *** The input argument pszFile of type "LPSTR" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPSTR' (pszFile) is unknown.
	PyObject *obpszFile = PyObject_FromLPSTR(pszFile);
	if (obpszFile==NULL) return PyCom_HandlePythonFailureToCOM();
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetPath", &result, "Oil", obpszFile, cchMaxPath, fFlags);
	Py_DECREF(obpszFile);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
// *** The output argument pfd of type "WIN32_FIND_DATAA *" was not processed ***
//     The type 'WIN32_FIND_DATAA *' (pfd) is unknown.
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGShellLink::GetIDList(
		LPITEMIDLIST * ppidl)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetIDList", &result);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
// *** The output argument ppidl of type "LPITEMIDLIST *" was not processed ***
//     The type 'LPITEMIDLIST *' (ppidl) is unknown.
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGShellLink::SetIDList(
		LPCITEMIDLIST pidl)
{
	PY_GATEWAY_METHOD;
// *** The input argument pidl of type "LPCITEMIDLIST" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPCITEMIDLIST' (pidl) is unknown.
	PyObject *obpidl = PyObject_FromLPCITEMIDLIST(pidl);
	if (obpidl==NULL) return PyCom_HandlePythonFailureToCOM();
	HRESULT hr=InvokeViaPolicy("SetIDList", NULL, "O", obpidl);
	Py_DECREF(obpidl);
	return hr;
}

STDMETHODIMP PyGShellLink::GetDescription(
		LPSTR pszName,
		int cchMaxName)
{
	PY_GATEWAY_METHOD;
// *** The input argument pszName of type "LPSTR" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPSTR' (pszName) is unknown.
	PyObject *obpszName = PyObject_FromLPSTR(pszName);
	if (obpszName==NULL) return PyCom_HandlePythonFailureToCOM();
	HRESULT hr=InvokeViaPolicy("GetDescription", NULL, "Oi", obpszName, cchMaxName);
	Py_DECREF(obpszName);
	return hr;
}

STDMETHODIMP PyGShellLink::SetDescription(
		LPCSTR pszName)
{
	PY_GATEWAY_METHOD;
// *** The input argument pszName of type "LPCSTR" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPCSTR' (pszName) is unknown.
	PyObject *obpszName = PyObject_FromLPCSTR(pszName);
	if (obpszName==NULL) return PyCom_HandlePythonFailureToCOM();
	HRESULT hr=InvokeViaPolicy("SetDescription", NULL, "O", obpszName);
	Py_DECREF(obpszName);
	return hr;
}

STDMETHODIMP PyGShellLink::GetWorkingDirectory(
		LPSTR pszDir,
		int cchMaxPath)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetWorkingDirectory", &result, "i", cchMaxPath);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
// *** The output argument pszDir of type "LPSTR" was not processed ***
//     The type 'LPSTR' (pszDir) is unknown.
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGShellLink::SetWorkingDirectory(
		LPCSTR pszDir)
{
	PY_GATEWAY_METHOD;
// *** The input argument pszDir of type "LPCSTR" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPCSTR' (pszDir) is unknown.
	PyObject *obpszDir = PyObject_FromLPCSTR(pszDir);
	if (obpszDir==NULL) return PyCom_HandlePythonFailureToCOM();
	HRESULT hr=InvokeViaPolicy("SetWorkingDirectory", NULL, "O", obpszDir);
	Py_DECREF(obpszDir);
	return hr;
}

STDMETHODIMP PyGShellLink::GetArguments(
		LPSTR pszArgs,
		int cchMaxPath)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetArguments", &result, "i", cchMaxPath);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
// *** The output argument pszArgs of type "LPSTR" was not processed ***
//     The type 'LPSTR' (pszArgs) is unknown.
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGShellLink::SetArguments(
		LPCSTR pszArgs)
{
	PY_GATEWAY_METHOD;
// *** The input argument pszArgs of type "LPCSTR" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPCSTR' (pszArgs) is unknown.
	PyObject *obpszArgs = PyObject_FromLPCSTR(pszArgs);
	if (obpszArgs==NULL) return PyCom_HandlePythonFailureToCOM();
	HRESULT hr=InvokeViaPolicy("SetArguments", NULL, "O", obpszArgs);
	Py_DECREF(obpszArgs);
	return hr;
}

STDMETHODIMP PyGShellLink::GetHotkey(
		WORD * pwHotkey)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetHotkey", &result);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
// *** The output argument pwHotkey of type "WORD *" was not processed ***
//     The type 'WORD *' (pwHotkey) is unknown.
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGShellLink::SetHotkey(
		WORD wHotkey)
{
	PY_GATEWAY_METHOD;
	HRESULT hr=InvokeViaPolicy("SetHotkey", NULL, "i", wHotkey);
	return hr;
}

STDMETHODIMP PyGShellLink::GetShowCmd(
		int * piShowCmd)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetShowCmd", &result);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
// *** The output argument piShowCmd of type "int *" was not processed ***
//     The type 'int *' (piShowCmd) is unknown.
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGShellLink::SetShowCmd(
		int iShowCmd)
{
	PY_GATEWAY_METHOD;
	HRESULT hr=InvokeViaPolicy("SetShowCmd", NULL, "i", iShowCmd);
	return hr;
}

STDMETHODIMP PyGShellLink::GetIconLocation(
		LPSTR pszIconPath,
		int cchIconPath,
		int * piIcon)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetIconLocation", &result, "i", cchIconPath);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
// *** The output argument pszIconPath of type "LPSTR" was not processed ***
//     The type 'LPSTR' (pszIconPath) is unknown.
// *** The output argument piIcon of type "int *" was not processed ***
//     The type 'int *' (piIcon) is unknown.
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGShellLink::SetIconLocation(
		LPCSTR pszIconPath,
		int iIcon)
{
	PY_GATEWAY_METHOD;
// *** The input argument pszIconPath of type "LPCSTR" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPCSTR' (pszIconPath) is unknown.
	PyObject *obpszIconPath = PyObject_FromLPCSTR(pszIconPath);
	if (obpszIconPath==NULL) return PyCom_HandlePythonFailureToCOM();
	HRESULT hr=InvokeViaPolicy("SetIconLocation", NULL, "Oi", obpszIconPath, iIcon);
	Py_DECREF(obpszIconPath);
	return hr;
}

STDMETHODIMP PyGShellLink::SetRelativePath(
		LPCSTR pszPathRel,
		DWORD dwReserved)
{
	PY_GATEWAY_METHOD;
// *** The input argument pszPathRel of type "LPCSTR" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPCSTR' (pszPathRel) is unknown.
	PyObject *obpszPathRel = PyObject_FromLPCSTR(pszPathRel);
	if (obpszPathRel==NULL) return PyCom_HandlePythonFailureToCOM();
	HRESULT hr=InvokeViaPolicy("SetRelativePath", NULL, "Ol", obpszPathRel, dwReserved);
	Py_DECREF(obpszPathRel);
	return hr;
}

STDMETHODIMP PyGShellLink::Resolve(
		HWND hwnd,
		DWORD fFlags)
{
	PY_GATEWAY_METHOD;
	HRESULT hr=InvokeViaPolicy("Resolve", NULL, "ll", hwnd, fFlags);
	return hr;
}

STDMETHODIMP PyGShellLink::SetPath(
		LPCSTR pszFile)
{
	PY_GATEWAY_METHOD;
// *** The input argument pszFile of type "LPCSTR" was not processed ***
//   - Please ensure this conversion function exists, and is appropriate
//   - The type 'LPCSTR' (pszFile) is unknown.
	PyObject *obpszFile = PyObject_FromLPCSTR(pszFile);
	if (obpszFile==NULL) return PyCom_HandlePythonFailureToCOM();
	HRESULT hr=InvokeViaPolicy("SetPath", NULL, "O", obpszFile);
	Py_DECREF(obpszFile);
	return hr;
}

******************************/
