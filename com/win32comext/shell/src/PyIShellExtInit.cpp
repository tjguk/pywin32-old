// This file implements the IShellExtInit Interface and Gateway for Python.
// Generated by makegw.py

#include "shell_pch.h"
#include "PyIShellExtInit.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIShellExtInit::PyIShellExtInit(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyIShellExtInit::~PyIShellExtInit()
{
}

/* static */ IShellExtInit *PyIShellExtInit::GetI(PyObject *self)
{
	return (IShellExtInit *)PyIUnknown::GetI(self);
}

// @pymethod |PyIShellExtInit|Initialize|Description of Initialize.
PyObject *PyIShellExtInit::Initialize(PyObject *self, PyObject *args)
{
	IShellExtInit *pISEI = GetI(self);
	if ( pISEI == NULL )
		return NULL;
	// @pyparm <o PyIDL>|pFolder||Description for pFolder
	// @pyparm <o PyIDataObject *>|pDataObject||Description for pDataObject
	// @pyparm <o PyHANDLE>|hkey||Description for hkey
	PyObject *obpFolder;
	PyObject *obpDataObject;
	PyObject *obhkey;
	LPITEMIDLIST pFolder;
	IDataObject *pDataObject;
	HKEY hkey;
	if ( !PyArg_ParseTuple(args, "OOO:Initialize", &obpFolder, &obpDataObject, &obhkey) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (!PyObject_AsPIDL(obpFolder, &pFolder)) bPythonIsHappy = FALSE;
	// XXX - no IDataObject support
	if (!PyCom_InterfaceFromPyInstanceOrObject(obpDataObject, IID_IUnknown, (void **)&pDataObject, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!PyWinObject_AsHANDLE(obhkey, (HANDLE *)&hkey, FALSE)) bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pISEI->Initialize( pFolder, pDataObject, hkey );
	PyObject_FreePIDL(pFolder);
	if (pDataObject) pDataObject->Release();
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pISEI, IID_IShellExtInit );
	Py_INCREF(Py_None);
	return Py_None;

}

// @object PyIShellExtInit|Description of the interface
static struct PyMethodDef PyIShellExtInit_methods[] =
{
	{ "Initialize", PyIShellExtInit::Initialize, 1 }, // @pymeth Initialize|Description of Initialize
	{ NULL }
};

PyComTypeObject PyIShellExtInit::type("PyIShellExtInit",
		&PyIUnknown::type,
		sizeof(PyIShellExtInit),
		PyIShellExtInit_methods,
		GET_PYCOM_CTOR(PyIShellExtInit));
// ---------------------------------------------------
//
// Gateway Implementation
STDMETHODIMP PyGShellExtInit::Initialize(
		/* [unique][in] */ LPCITEMIDLIST pFolder,
		/* [unique][in] */ IDataObject * pDataObject,
		/* [unique][in] */ HKEY hkey)
{
	PY_GATEWAY_METHOD;
	PyObject *obpFolder;
	PyObject *obpDataObject;
	obpFolder = PyObject_FromPIDL(pFolder, FALSE);
	// XXX - no IDataObject support
	obpDataObject = PyCom_PyObjectFromIUnknown(pDataObject, IID_IUnknown, TRUE);
	HRESULT hr=InvokeViaPolicy("Initialize", NULL, "OOi", obpFolder, obpDataObject, hkey);
	Py_XDECREF(obpFolder);
	Py_XDECREF(obpDataObject);
	return hr;
}

