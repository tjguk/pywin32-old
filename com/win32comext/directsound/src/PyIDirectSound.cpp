// This file implements the IDirectSound Interface for Python.

#include "directsound_pch.h"
#include "PySoundObjects.h"
#include "PyIDirectSound.h"
#include "PyIDirectSoundBuffer.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIDirectSound::PyIDirectSound(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyIDirectSound::~PyIDirectSound()
{
}

/* static */ IDirectSound *PyIDirectSound::GetI(PyObject *self)
{
	return (IDirectSound *)PyIUnknown::GetI(self);
}

// @pymethod |PyIDirectSound|Initialize|Description of Initialize.
PyObject *PyIDirectSound::Initialize(PyObject *self, PyObject *args)
{
	PyObject *obGUID;

	IDirectSound *pIDS = GetI(self);
	if ( pIDS == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, "|O:Initialize", &obGUID) )
		return NULL;

	GUID guid;
	LPGUID pguid = NULL;
	if (!obGUID && obGUID != Py_None)
	{
		if (!PyWinObject_AsIID(obGUID, &guid))
			return NULL;

		pguid = &guid;
	}

	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIDS->Initialize(pguid);
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr)) {
		PyWin_SetAPIError("Initialize", hr);
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIDirectSound|SetCooperativeLevel|Description of SetCooperativeLevel.
PyObject *PyIDirectSound::SetCooperativeLevel(PyObject *self, PyObject *args)
{
	int level;
	PyObject *obHWND = NULL;
	HWND hwnd;

	IDirectSound *pIDS = GetI(self);
	if ( pIDS == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, "Oi:SetCooperativeLevel", &obHWND, &level) )
		return NULL;

	if (obHWND == Py_None)
	{
		hwnd = GetForegroundWindow();
		if (hwnd == NULL)
		{
	        hwnd = GetDesktopWindow();
	    }
	}
	else if (PyInt_Check(obHWND))
	{
		hwnd = (HWND)PyInt_AS_LONG(obHWND);
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "argument 1 must be a window handle or None");
		return NULL;
	}

	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIDS->SetCooperativeLevel(hwnd, level);
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr)) {
		PyWin_SetAPIError("SetCooperativeLevel", hr);
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIDirectSound|CreateSoundBuffer|Description of CreateSoundBuffer.
PyObject *PyIDirectSound::CreateSoundBuffer(PyObject *self, PyObject *args)
{
	PyObject *obDSBD = NULL;
	PyObject *obUnk = NULL;
	IUnknown *pUnkIn = NULL;

	IDirectSound *pIDS = GetI(self);
	if ( pIDS == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, "O|O:CreateSoundBuffer", &obDSBD, &obUnk) )
		return NULL;

	if (!PyDSBUFFERDESC_Check(obDSBD)) {
		PyErr_SetString(PyExc_TypeError, "Argument 1 must be of type PyDSBUFFERDESC");
		return NULL;
	}

	if (!PyCom_InterfaceFromPyInstanceOrObject(obUnk, IID_IUnknown, (void **)&pUnkIn, TRUE)) {
		return NULL;
	}
	

	DSBUFFERDESC *pdsbd = &((PyDSBUFFERDESC*)obDSBD)->m_dsbd;
	HRESULT hr;
	IDirectSoundBuffer *buffer;

	PY_INTERFACE_PRECALL;
	hr = pIDS->CreateSoundBuffer(pdsbd, &buffer, pUnkIn);
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr)) {
		PyWin_SetAPIError("CreateSoundBuffer", hr);
		return NULL;
	}

	PyIDirectSoundBuffer *rc = new PyIDirectSoundBuffer(buffer);

	Py_INCREF(self);
	rc->m_DS = self;

	return rc;
}

// @pymethod |PyIDirectSound|GetCaps|Description of GetCaps.
PyObject *PyIDirectSound::GetCaps(PyObject *self, PyObject *args)
{
	IDirectSound *pIDS = GetI(self);
	if ( pIDS == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, ":GetCaps") )
		return NULL;


	HRESULT hr;
	PyDSCAPS *caps = new PyDSCAPS();
	PY_INTERFACE_PRECALL;
	hr = pIDS->GetCaps(caps->GetCAPS());
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr)) {
		PyWin_SetAPIError("GetCaps", hr);
		return NULL;
	}

	Py_INCREF(caps);
	return caps;
}

// @pymethod |PyIDirectSound|Compact|Description of Compact.
PyObject *PyIDirectSound::Compact(PyObject *self, PyObject *args)
{
	IDirectSound *pIDS = GetI(self);
	if ( pIDS == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, ":Compact") )
		return NULL;


	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIDS->Compact();
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr)) {
		PyWin_SetAPIError("GetCaps", hr);
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod |PyIDirectSound|GetSpeakerConfig|Description of GetSpeakerConfig.
PyObject *PyIDirectSound::GetSpeakerConfig(PyObject *self, PyObject *args)
{
	IDirectSound *pIDS = GetI(self);
	if ( pIDS == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, ":GetSpeakerConfig") )
		return NULL;


	HRESULT hr;
	DWORD config;
	PY_INTERFACE_PRECALL;
	hr = pIDS->GetSpeakerConfig(&config);
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr)) {
		PyWin_SetAPIError("GetSpeakerConfig", hr);
		return NULL;
	}

	return PyInt_FromLong(config);
}

// @pymethod |PyIDirectSound|SetSpeakerConfig|Description of SetSpeakerConfig.
PyObject *PyIDirectSound::SetSpeakerConfig(PyObject *self, PyObject *args)
{
	DWORD config;
	IDirectSound *pIDS = GetI(self);
	if ( pIDS == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, "i:SetSpeakerConfig", &config) )
		return NULL;


	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIDS->SetSpeakerConfig(config);
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr)) {
		PyWin_SetAPIError("SetSpeakerConfig", hr);
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

// @object PyIDirectSound|Description of the interface
static struct PyMethodDef PyIDirectSound_methods[] =
{
	{ "Initialize", PyIDirectSound::Initialize, 1 }, // @pymeth Initialize|Description of Initialize.
	{ "SetCooperativeLevel", PyIDirectSound::SetCooperativeLevel, 1 }, // @pymeth SetCooperativeLevel|Description of SetCooperativeLevel.
	{ "CreateSoundBuffer", PyIDirectSound::CreateSoundBuffer, 1 }, // @pymeth CreateSoundBuffer|Description of CreateSoundBuffer.
	{ "GetCaps", PyIDirectSound::GetCaps, 1 }, // @pymeth GetCaps|Description of GetCaps.
	{ "Compact", PyIDirectSound::Compact, 1 }, // @pymeth Compact|Description of Compact.
	{ NULL }
};

PyComTypeObject PyIDirectSound::type("PyIDirectSound",
		&PyIUnknown::type,
		sizeof(PyIDirectSound),
		PyIDirectSound_methods,
		GET_PYCOM_CTOR(PyIDirectSound));
