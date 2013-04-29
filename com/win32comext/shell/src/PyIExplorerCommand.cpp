// This file implements the IExplorerCommand Interface and Gateway for Python.
// Generated by makegw.py

#include "shell_pch.h"
#include "PyIExplorerCommand.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIExplorerCommand::PyIExplorerCommand(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyIExplorerCommand::~PyIExplorerCommand()
{
}

/* static */ IExplorerCommand *PyIExplorerCommand::GetI(PyObject *self)
{
	return (IExplorerCommand *)PyIUnknown::GetI(self);
}

// @pymethod unicode|PyIExplorerCommand|GetTitle|Description of GetTitle.
PyObject *PyIExplorerCommand::GetTitle(PyObject *self, PyObject *args)
{
	IExplorerCommand *pIEC = GetI(self);
	if ( pIEC == NULL )
		return NULL;
	// @pyparm <o PyIShellItemArray>|psiItemArray||Description for psiItemArray
	PyObject *obpsiItemArray;
	IShellItemArray * psiItemArray;
	if ( !PyArg_ParseTuple(args, "O:GetTitle", &obpsiItemArray) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (bPythonIsHappy && !PyCom_InterfaceFromPyInstanceOrObject(obpsiItemArray, IID_IShellItemArray, (void **)&psiItemArray, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	WCHAR *pszName = 0;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIEC->GetTitle( psiItemArray, &pszName );
	if (psiItemArray) psiItemArray->Release();
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIEC, IID_IExplorerCommand );
	PyObject *ret = PyWinObject_FromWCHAR(pszName);
	CoTaskMemFree(pszName);
	return ret;
}

// @pymethod unicode|PyIExplorerCommand|GetIcon|Description of GetIcon.
PyObject *PyIExplorerCommand::GetIcon(PyObject *self, PyObject *args)
{
	IExplorerCommand *pIEC = GetI(self);
	if ( pIEC == NULL )
		return NULL;
	// @pyparm <o PyIShellItemArray>|psiItemArray||Description for psiItemArray
	PyObject *obpsiItemArray;
	IShellItemArray * psiItemArray;
	if ( !PyArg_ParseTuple(args, "O:GetIcon", &obpsiItemArray) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (bPythonIsHappy && !PyCom_InterfaceFromPyInstanceOrObject(obpsiItemArray, IID_IShellItemArray, (void **)&psiItemArray, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	HRESULT hr;
	WCHAR *pszIcon = 0;
	PY_INTERFACE_PRECALL;
	hr = pIEC->GetIcon( psiItemArray, &pszIcon );
	if (psiItemArray) psiItemArray->Release();
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIEC, IID_IExplorerCommand );
	PyObject *ret = PyWinObject_FromWCHAR(pszIcon);
	CoTaskMemFree(pszIcon);
	return ret;
}

// @pymethod unicode|PyIExplorerCommand|GetToolTip|Description of GetToolTip.
PyObject *PyIExplorerCommand::GetToolTip(PyObject *self, PyObject *args)
{
	IExplorerCommand *pIEC = GetI(self);
	if ( pIEC == NULL )
		return NULL;
	// @pyparm <o PyIShellItemArray>|psiItemArray||Description for psiItemArray
	PyObject *obpsiItemArray;
	IShellItemArray * psiItemArray;
	if ( !PyArg_ParseTuple(args, "O:GetToolTip", &obpsiItemArray) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (bPythonIsHappy && !PyCom_InterfaceFromPyInstanceOrObject(obpsiItemArray, IID_IShellItemArray, (void **)&psiItemArray, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	HRESULT hr;
	WCHAR *psz = 0;
	PY_INTERFACE_PRECALL;
	hr = pIEC->GetToolTip(psiItemArray, &psz);
	if (psiItemArray) psiItemArray->Release();
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIEC, IID_IExplorerCommand );
	PyObject *ret = PyWinObject_FromWCHAR(psz);
	CoTaskMemFree(psz);
	return ret;
}

// @pymethod <o PyIID>|PyIExplorerCommand|GetCanonicalName|Description of GetCanonicalName.
PyObject *PyIExplorerCommand::GetCanonicalName(PyObject *self, PyObject *args)
{
	IExplorerCommand *pIEC = GetI(self);
	if ( pIEC == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, ":GetCanonicalName") )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	GUID guid;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIEC->GetCanonicalName( &guid );
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIEC, IID_IExplorerCommand );
	return PyWinObject_FromIID(guid);
}

// @pymethod int|PyIExplorerCommand|GetState|Description of GetState.
PyObject *PyIExplorerCommand::GetState(PyObject *self, PyObject *args)
{
	IExplorerCommand *pIEC = GetI(self);
	if ( pIEC == NULL )
		return NULL;
	// @pyparm <o PyIShellItemArray>|psiItemArray||Description for psiItemArray
	// @pyparm int|fOkToBeSlow||Description for fOkToBeSlow
	PyObject *obpsiItemArray;
	IShellItemArray* psiItemArray;
	BOOL fOkToBeSlow;
	if ( !PyArg_ParseTuple(args, "Oi:GetState", &obpsiItemArray, &fOkToBeSlow) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (bPythonIsHappy && !PyCom_InterfaceFromPyInstanceOrObject(obpsiItemArray, IID_IShellItemArray, (void **)&psiItemArray, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	HRESULT hr;
	DWORD cmdState;
	PY_INTERFACE_PRECALL;
	hr = pIEC->GetState(psiItemArray, fOkToBeSlow, &cmdState );
	if (psiItemArray) psiItemArray->Release();
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIEC, IID_IExplorerCommand );
	return PyLong_FromUnsignedLong(cmdState);
}

// @pymethod |PyIExplorerCommand|Invoke|Description of Invoke.
PyObject *PyIExplorerCommand::Invoke(PyObject *self, PyObject *args)
{
	IExplorerCommand *pIEC = GetI(self);
	if ( pIEC == NULL )
		return NULL;
	// @pyparm <o PyIShellItemArray>|psiItemArray||Description for psiItemArray
	// @pyparm <o PyIBindCtx>|pbc||Description for pbc
	PyObject *obpsiItemArray;
	PyObject *obpbc;
	IShellItemArray *psiItemArray;
	IBindCtx *pbc;
	if ( !PyArg_ParseTuple(args, "OO:Invoke", &obpsiItemArray, &obpbc) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (bPythonIsHappy && !PyCom_InterfaceFromPyInstanceOrObject(obpsiItemArray, IID_IShellItemArray, (void **)&psiItemArray, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (bPythonIsHappy && !PyCom_InterfaceFromPyInstanceOrObject(obpbc, IID_IBindCtx, (void **)&pbc, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIEC->Invoke( psiItemArray, pbc );
	if (psiItemArray) psiItemArray->Release();
	if (pbc) pbc->Release();

	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIEC, IID_IExplorerCommand );
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod int|PyIExplorerCommand|GetFlags|Description of GetFlags.
PyObject *PyIExplorerCommand::GetFlags(PyObject *self, PyObject *args)
{
	IExplorerCommand *pIEC = GetI(self);
	if ( pIEC == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, ":GetFlags") )
		return NULL;
	HRESULT hr;
	DWORD flags;
	PY_INTERFACE_PRECALL;
	hr = pIEC->GetFlags( &flags );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIEC, IID_IExplorerCommand );
	return PyLong_FromUnsignedLong(flags);
}

// @pymethod <o PyIEnumExplorerCommand>|PyIExplorerCommand|EnumSubCommands|Description of EnumSubCommands.
PyObject *PyIExplorerCommand::EnumSubCommands(PyObject *self, PyObject *args)
{
	IExplorerCommand *pIEC = GetI(self);
	if ( pIEC == NULL )
		return NULL;
	IEnumExplorerCommand *pEnum;
	if ( !PyArg_ParseTuple(args, ":EnumSubCommands") )
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIEC->EnumSubCommands( &pEnum );
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIEC, IID_IExplorerCommand );
	return PyCom_PyObjectFromIUnknown(pEnum, IID_IEnumExplorerCommand, FALSE);
}

// @object PyIExplorerCommand|Description of the interface
static struct PyMethodDef PyIExplorerCommand_methods[] =
{
	{ "GetTitle", PyIExplorerCommand::GetTitle, 1 }, // @pymeth GetTitle|Description of GetTitle
	{ "GetIcon", PyIExplorerCommand::GetIcon, 1 }, // @pymeth GetIcon|Description of GetIcon
	{ "GetToolTip", PyIExplorerCommand::GetToolTip, 1 }, // @pymeth GetToolTip|Description of GetToolTip
	{ "GetCanonicalName", PyIExplorerCommand::GetCanonicalName, 1 }, // @pymeth GetCanonicalName|Description of GetCanonicalName
	{ "GetState", PyIExplorerCommand::GetState, 1 }, // @pymeth GetState|Description of GetState
	{ "Invoke", PyIExplorerCommand::Invoke, 1 }, // @pymeth Invoke|Description of Invoke
	{ "GetFlags", PyIExplorerCommand::GetFlags, 1 }, // @pymeth GetFlags|Description of GetFlags
	{ "EnumSubCommands", PyIExplorerCommand::EnumSubCommands, 1 }, // @pymeth EnumSubCommands|Description of EnumSubCommands
	{ NULL }
};

PyComTypeObject PyIExplorerCommand::type("PyIExplorerCommand",
		&PyIUnknown::type,
		sizeof(PyIExplorerCommand),
		PyIExplorerCommand_methods,
		GET_PYCOM_CTOR(PyIExplorerCommand));
// ---------------------------------------------------
//
// Gateway Implementation
STDMETHODIMP PyGExplorerCommand::GetTitle(
		/* [unique][in] */ IShellItemArray * psiItemArray,
		/* [string][out] */ LPWSTR * ppszName)
{
	PY_GATEWAY_METHOD;
	PyObject *obpsiItemArray;
	obpsiItemArray = PyCom_PyObjectFromIUnknown(psiItemArray, IID_IShellItemArray, TRUE);
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetTitle", &result, "O", obpsiItemArray);
	Py_XDECREF(obpsiItemArray);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	if (!PyWinObject_AsTaskAllocatedWCHAR(result, ppszName))
		hr = PyCom_SetAndLogCOMErrorFromPyException("GetTitle", IID_IExplorerCommand);
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGExplorerCommand::GetIcon(
		/* [unique][in] */ IShellItemArray * psiItemArray,
		/* [string][out] */ LPWSTR * ppszIcon)
{
	PY_GATEWAY_METHOD;
	PyObject *obpsiItemArray;
	obpsiItemArray = PyCom_PyObjectFromIUnknown(psiItemArray, IID_IShellItemArray, TRUE);
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetIcon", &result, "O", obpsiItemArray);
	Py_XDECREF(obpsiItemArray);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	if (!PyWinObject_AsTaskAllocatedWCHAR(result, ppszIcon))
		hr = PyCom_SetAndLogCOMErrorFromPyException("GetIcon", IID_IExplorerCommand);
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGExplorerCommand::GetToolTip(
		/* [unique][in] */ IShellItemArray * psiItemArray,
		/* [string][out] */ LPWSTR * ppszInfotip)
{
	PY_GATEWAY_METHOD;
	PyObject *obpsiItemArray;
	obpsiItemArray = PyCom_PyObjectFromIUnknown(psiItemArray, IID_IShellItemArray, TRUE);
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetToolTip", &result, "O", obpsiItemArray);
	Py_XDECREF(obpsiItemArray);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	if (!PyWinObject_AsTaskAllocatedWCHAR(result, ppszInfotip))
		hr = PyCom_SetAndLogCOMErrorFromPyException("GetToolTip", IID_IExplorerCommand);
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGExplorerCommand::GetCanonicalName(
		/* [out] */ GUID * pguidCommandName)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetCanonicalName", &result);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	if (!PyWinObject_AsIID(result, pguidCommandName))
		hr = PyCom_SetAndLogCOMErrorFromPyException("GetCanonicalName", IID_IExplorerCommand);
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGExplorerCommand::GetState(
		/* [in] */ IShellItemArray * psiItemArray,
		/* [in] */ BOOL fOkToBeSlow,
		/* [out] */ EXPCMDSTATE * pCmdState)
{
	PY_GATEWAY_METHOD;
	PyObject *obpsiItemArray;
	obpsiItemArray = PyCom_PyObjectFromIUnknown(psiItemArray, IID_IShellItemArray, TRUE);
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetState", &result, "Oi", obpsiItemArray, fOkToBeSlow);
	Py_XDECREF(obpsiItemArray);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	*pCmdState = PyLong_AsUnsignedLongMask(result);
	hr = PyCom_SetAndLogCOMErrorFromPyException("GetState", IID_IExplorerCommand);
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGExplorerCommand::Invoke(
		/* [in] */ IShellItemArray * psiItemArray,
		/* [unique][in] */ IBindCtx * pbc)
{
	PY_GATEWAY_METHOD;
	PyObject *obpsiItemArray;
	PyObject *obpbc;
	obpsiItemArray = PyCom_PyObjectFromIUnknown(psiItemArray, IID_IShellItemArray, TRUE);
	obpbc = PyCom_PyObjectFromIUnknown(pbc, IID_IBindCtx, TRUE);
	HRESULT hr=InvokeViaPolicy("Invoke", NULL, "OO", obpsiItemArray, obpbc);
	Py_XDECREF(obpsiItemArray);
	Py_XDECREF(obpbc);
	return hr;
}

STDMETHODIMP PyGExplorerCommand::GetFlags(
		/* [out] */ EXPCMDFLAGS * pFlags)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetFlags", &result);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	*pFlags = PyLong_AsUnsignedLongMask(result);
	hr = PyCom_SetAndLogCOMErrorFromPyException("GetFlags", IID_IExplorerCommand);
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGExplorerCommand::EnumSubCommands(
		/* [out] */ IEnumExplorerCommand ** ppEnum)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("EnumSubCommands", &result);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	if (!PyCom_InterfaceFromPyInstanceOrObject(result, IID_IEnumExplorerCommand, (void **)ppEnum, FALSE /* bNoneOK */))
		hr = PyCom_SetAndLogCOMErrorFromPyException("EnumSubCommands", IID_IExplorerCommand);
	Py_DECREF(result);
	return hr;
}

