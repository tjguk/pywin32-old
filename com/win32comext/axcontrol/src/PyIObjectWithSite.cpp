// This file implements the IObjectWithSite Interface and Gateway for Python.
// Generated by makegw.py

#include "axcontrol_pch.h"
#include "PyIObjectWithSite.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIObjectWithSite::PyIObjectWithSite(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyIObjectWithSite::~PyIObjectWithSite()
{
}

/* static */ IObjectWithSite *PyIObjectWithSite::GetI(PyObject *self)
{
	return (IObjectWithSite *)PyIUnknown::GetI(self);
}

// @pymethod |PyIObjectWithSite|SetSite|Description of SetSite.
PyObject *PyIObjectWithSite::SetSite(PyObject *self, PyObject *args)
{
	IObjectWithSite *pIOWS = GetI(self);
	if ( pIOWS == NULL )
		return NULL;
	// @pyparm <o PyIUnknown *>|pUnkSite||Description for pUnkSite
	PyObject *obpUnkSite;
	IUnknown * pUnkSite;
	if ( !PyArg_ParseTuple(args, "O:SetSite", &obpUnkSite) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (!PyCom_InterfaceFromPyInstanceOrObject(obpUnkSite, IID_IUnknown, (void **)&pUnkSite, TRUE /* bNoneOK */))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIOWS->SetSite( pUnkSite );
	if (pUnkSite) pUnkSite->Release();

	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIOWS, IID_IObjectWithSite );
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIObjectWithSite|GetSite|Description of GetSite.
PyObject *PyIObjectWithSite::GetSite(PyObject *self, PyObject *args)
{
	IObjectWithSite *pIOWS = GetI(self);
	if ( pIOWS == NULL )
		return NULL;
	// @pyparm <o PyIID>|riid||Description for riid
	PyObject *obriid;
	IID riid;
	void *ppvSite;
	if ( !PyArg_ParseTuple(args, "O:GetSite", &obriid) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	if (!PyWinObject_AsIID(obriid, &riid)) bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIOWS->GetSite( riid, &ppvSite );

	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIOWS, IID_IObjectWithSite );
	return PyCom_PyObjectFromIUnknown((IUnknown *)ppvSite, riid, FALSE);
}

// @object PyIObjectWithSite|Description of the interface
static struct PyMethodDef PyIObjectWithSite_methods[] =
{
	{ "SetSite", PyIObjectWithSite::SetSite, 1 }, // @pymeth SetSite|Description of SetSite
	{ "GetSite", PyIObjectWithSite::GetSite, 1 }, // @pymeth GetSite|Description of GetSite
	{ NULL }
};

PyComTypeObject PyIObjectWithSite::type("PyIObjectWithSite",
		&PyIUnknown::type,
		sizeof(PyIObjectWithSite),
		PyIObjectWithSite_methods,
		GET_PYCOM_CTOR(PyIObjectWithSite));
// ---------------------------------------------------
//
// Gateway Implementation
STDMETHODIMP PyGObjectWithSite::SetSite(
		/* [in] */ IUnknown * pUnkSite)
{
	PY_GATEWAY_METHOD;
	PyObject *obpUnkSite;
	obpUnkSite = PyCom_PyObjectFromIUnknown(pUnkSite, IID_IUnknown, TRUE);
	HRESULT hr=InvokeViaPolicy("SetSite", NULL, "O", obpUnkSite);
	Py_XDECREF(obpUnkSite);
	return hr;
}

STDMETHODIMP PyGObjectWithSite::GetSite(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void ** ppvSite)
{
	PY_GATEWAY_METHOD;
	PyObject *obriid;
	obriid = PyWinObject_FromIID(riid);
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("GetSite", &result, "O", obriid);
	Py_XDECREF(obriid);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	PyObject *obppvSite;
	if (!PyArg_Parse(result, "O" , &obppvSite))
		return MAKE_PYCOM_GATEWAY_FAILURE_CODE("GetSite");
	BOOL bPythonIsHappy = TRUE;
	if (bPythonIsHappy && !PyCom_InterfaceFromPyInstanceOrObject(obppvSite, riid, ppvSite, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) hr = MAKE_PYCOM_GATEWAY_FAILURE_CODE("GetSite");
	Py_DECREF(result);
	return hr;
}

