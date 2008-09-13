// This file implements the IStorage Interface and Gateway for Python.
// Generated by makegw.py

#include "stdafx.h"
#include "PythonCOM.h"
#include "PythonCOMServer.h"
#include "PyIStorage.h"
#include "PyWinObjects.h" // Until this uses the new PyTime API...

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIStorage::PyIStorage(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyIStorage::~PyIStorage()
{
}

/* static */ IStorage *PyIStorage::GetI(PyObject *self)
{
	return (IStorage *)PyIUnknown::GetI(self);
}

// @pymethod <o PyIStream>|PyIStorage|CreateStream|Creates and opens a stream object with the specified name contained in this storage object. All elements within a storage object � both streams and other storage objects � are kept in the same name space.
PyObject *PyIStorage::CreateStream(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm string|pwcsName||Name of the new stream
	// @pyparm int|grfMode||Specifies the access mode to use when opening the newly created stream.
	// @pyparm int|reserved1|0|Reserved - must be zero.
	// @pyparm int|reserved2|0|Reserved - must be zero.
	DWORD grfMode;
	DWORD reserved1 = 0;
	DWORD reserved2 = 0;
	PyObject *obName;
	if ( !PyArg_ParseTuple(args, "Oi|ii:CreateStream", &obName, &grfMode, &reserved1, &reserved2) )
		return NULL;
	IStream *ppstm;
	BOOL bPythonIsHappy = TRUE;
	BSTR bstrName;
	bPythonIsHappy = PyWinObject_AsBstr(obName, &bstrName);
	if (!bPythonIsHappy) return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->CreateStream( bstrName, grfMode, reserved1, reserved2, &ppstm );
	PyWinObject_FreeBstr(bstrName);
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);

	return PyCom_PyObjectFromIUnknown(ppstm, IID_IStream, FALSE);
}

// @pymethod <o PyIStream>|PyIStorage|OpenStream|Opens an existing stream object within this storage object in the specified access mode.
PyObject *PyIStorage::OpenStream(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm string|pwcsName||Description for pwcsName
	// @pyparm object|reserved1||A reserved param.  Always pass None.  NULL is always passed to the COM function
	// @pyparm int|grfMode||Specifies the access mode to be assigned to the open stream. See the STGM enumeration values for descriptions of the possible values. . Whatever other modes you may choose, you must at least specify STGM_SHARE_EXCLUSIVE when calling this method.
	// @pyparm int|reserved2|0|Reserved - must be zero.
	PyObject *obName;
	DWORD grfMode;
	PyObject *obreserved1;
	DWORD reserved2 = 0;
	if ( !PyArg_ParseTuple(args, "OOi|i:OpenStream", &obName, &obreserved1, &grfMode, &reserved2) )
		return NULL;
	if (obreserved1 != Py_None) {
		PyErr_SetString(PyExc_TypeError, "The 'reserved' parameter (param 2) must be None");
		return NULL;
	}
	IStream *ppstm;
	BOOL bPythonIsHappy = TRUE;
	BSTR name;
	bPythonIsHappy = PyWinObject_AsBstr(obName, &name);
	if (!bPythonIsHappy) return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->OpenStream( name, NULL, grfMode, reserved2, &ppstm );
	PyWinObject_FreeBstr(name);
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);

	return PyCom_PyObjectFromIUnknown(ppstm, IID_IStream, FALSE);
}

// @pymethod <o PyIStorage>|PyIStorage|CreateStorage|Creates and opens a new storage object nested within this storage object.
PyObject *PyIStorage::CreateStorage(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm string|pwcsName||The name of the newly created stream.
	// @pyparm int|grfMode||Specifies the access mode to use when opening the newly created storage object.
	// @pyparm int|dwStgFmt||Documented as "reserved"!
	// @pyparm int|reserved2|0|Description for reserved2
	PyObject *obName;
	DWORD grfMode;
	DWORD dwStgFmt;
	DWORD reserved2 = 0;
	if ( !PyArg_ParseTuple(args, "Oii|i:CreateStorage", &obName, &grfMode, &dwStgFmt, &reserved2) )
		return NULL;
	IStorage *ppstg;
	BOOL bPythonIsHappy = TRUE;
	BSTR name;
	bPythonIsHappy = PyWinObject_AsBstr(obName, &name);
	if (!bPythonIsHappy) return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->CreateStorage( name, grfMode, dwStgFmt, reserved2, &ppstg );
	PY_INTERFACE_POSTCALL;
	PyWinObject_FreeBstr(name);
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);

	return PyCom_PyObjectFromIUnknown(ppstg, IID_IStorage, FALSE);
}

// @pymethod <o PyIStorage>|PyIStorage|OpenStorage|Opens an existing storage object with the specified name in the specified access mode.
PyObject *PyIStorage::OpenStorage(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm string|pwcsName||Name of the storage, or None.
	// @pyparm <o PyIStorage>|pstgPriority||If the pstgPriority parameter is not None, it is a <o PyIStorage> object to a previous opening of an element of the storage object, 
	// usually one that was opened in priority mode. The storage object should be closed and re-opened 
	// according to grfMode. When the <om PyIStorage.OpenStorage> method returns, pstgPriority is no longer valid - use the result value. 
	// If the pstgPriority parameter is None, it is ignored.
	// @pyparm int|grfMode||Specifies the access mode to use when opening the storage object. See the STGM enumeration values for descriptions of the possible values. Whatever other modes you may choose, you must at least specify STGM_SHARE_EXCLUSIVE when calling this method. 
	// @pyparm <o SNB>|snbExclude||Reserved for later - Must be None
	// @pyparm int|reserved|0|Reserved integer param.
	PyObject *obName;
	PyObject *obpstgPriority;
	DWORD grfMode;
	DWORD reserved = 0;
	char *temp = NULL;
	if ( !PyArg_ParseTuple(args, "OOi|zi:OpenStorage", &obName, &obpstgPriority, &grfMode, &temp, &reserved) )
		return NULL;
	IStorage *pstgPriority;
	IStorage *ppstg;
	BOOL bPythonIsHappy = TRUE;
	BSTR bstrName;
	bPythonIsHappy = PyWinObject_AsBstr(obName, &bstrName, TRUE);
	if (!PyCom_InterfaceFromPyObject(obpstgPriority, IID_IStorage, (void **)&pstgPriority, TRUE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->OpenStorage( bstrName, pstgPriority, grfMode, NULL, reserved, &ppstg );
	if ( pstgPriority != NULL )
		pstgPriority->Release();
	PyWinObject_FreeBstr(bstrName);
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);

	return PyCom_PyObjectFromIUnknown(ppstg, IID_IStorage, FALSE);
}

// @pymethod |PyIStorage|CopyTo|Copies the entire contents of an open storage object to another storage object.
PyObject *PyIStorage::CopyTo(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm [<o PyIID>,]|rgiidExclude||List of IID's to be excluded.
	// @pyparm <o SNB>|snbExclude||Reserved for later - Must be None
	// @pyparm <o PyIStorage>|stgDest||The open storage object into which this storage object is to be copied. 
	// The destination storage object can be a different implementation of the <o PyIStorage> interface from the source storage object. 
	// Thus, <om IStorage::CopyTo> can only use publicly available methods of the destination storage object. 
	// If stgDest is open in transacted mode, it can be reverted by calling its <om PyIStorage::Revert> method. 
	DWORD ciidExclude = 0;
	PyObject *obSeqExclude;
	PyObject *obpstgDest;
	char *temp;
	if ( !PyArg_ParseTuple(args, "OzO:CopyTo", &obSeqExclude, &temp, &obpstgDest) )
		return NULL;
	IID *pExclude;
	if (obSeqExclude==Py_None)
		pExclude = NULL;
	else {
		if (!PySequence_Check(obSeqExclude)) {
			PyErr_SetString(PyExc_TypeError, "Argument 1 must be a sequence of IID's, or None");
			return NULL;
		}
		ciidExclude = PySequence_Length(obSeqExclude);
		pExclude = new IID[ciidExclude];
		if (pExclude==NULL) {
			PyErr_SetString(PyExc_MemoryError, "Allocating array of IID's");
			return NULL;
		}
		for (DWORD i=0;i<ciidExclude;i++) {
			PyObject *ob = PySequence_GetItem(obSeqExclude, (int)i);
			BOOL ok = PyWinObject_AsIID(ob, pExclude+i);
			Py_XDECREF(ob);
			if (!ok) {
				delete [] pExclude;
				return NULL;
			}
		}
	}
	IStorage *pstgDest;
	BOOL bPythonIsHappy = TRUE;
	if (!PyCom_InterfaceFromPyObject(obpstgDest, IID_IStorage, (void **)&pstgDest, FALSE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) {
		return NULL;
		delete [] pExclude;
	}
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->CopyTo( ciidExclude, pExclude, NULL, pstgDest );
	delete [] pExclude;
	pstgDest->Release();
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod |PyIStorage|MoveElementTo|Copies or moves a substorage or stream from this storage object to another storage object.
PyObject *PyIStorage::MoveElementTo(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm string|name||A string that contains the name of the element in this storage object to be moved or copied.
	// @pyparm <o PyIStorage>|stgDest||<o PyIStorage> for the destination storage object.
	// @pyparm string|newName||A string that contains the new name for the element in its new storage object.
	// @pyparm int|grfFlags||Specifies whether the operation should be a move (STGMOVE_MOVE) or a copy (STGMOVE_COPY). See the STGMOVE enumeration.
	PyObject *obName, *obNewName;
	PyObject *obpstgDest;
	DWORD grfFlags;
	if ( !PyArg_ParseTuple(args, "OOOi:MoveElementTo", &obName, &obpstgDest, &obNewName, &grfFlags) )
		return NULL;
	IStorage *pstgDest;
	BOOL bPythonIsHappy = TRUE;
	BSTR bstrName, bstrNewName;
	bPythonIsHappy = PyWinObject_AsBstr(obName, &bstrName);
	if (!PyWinObject_AsBstr(obNewName, &bstrNewName))
		bPythonIsHappy = FALSE;
	if (!PyCom_InterfaceFromPyObject(obpstgDest, IID_IStorage, (void **)&pstgDest, FALSE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->MoveElementTo( bstrName, pstgDest, bstrNewName, grfFlags );
	pstgDest->Release();
	PY_INTERFACE_POSTCALL;
	PyWinObject_FreeBstr(bstrName);
	PyWinObject_FreeBstr(bstrNewName);
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIStorage|Commit|Ensures that any changes made to a storage object open in transacted mode are reflected in the parent storage; 
// for a root storage, reflects the changes in the actual device, for example, a file on disk. 
// For a root storage object opened in direct mode, this method has no effect except to flush all memory buffers to the disk. For non-root storage objects in direct mode, this method has no effect.
PyObject *PyIStorage::Commit(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm int|grfCommitFlags||Controls how the changes are committed to the storage object. See the STGC enumeration for a definition of these values. 
	DWORD grfCommitFlags;
	if ( !PyArg_ParseTuple(args, "i:Commit", &grfCommitFlags) )
		return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->Commit( grfCommitFlags );
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIStorage|Revert|Discards all changes that have been made to the storage object since the last commit.
PyObject *PyIStorage::Revert(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, ":Revert") )
		return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->Revert( );
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod <o PyIEnumSTATSTG>|PyIStorage|EnumElements|Retrieves an enumerator object that can be used to enumerate the storage and stream objects contained within this storage object.
PyObject *PyIStorage::EnumElements(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm int|reserved1|0|Reserved - must be zero.
	// @pyparm object|reserved2|None|A reserved param.  Always pass None.  NULL is always passed to the COM function
	// @pyparm int|reserved3|0|Reserved - must be zero.
	DWORD reserved1 = 0;
	char *szreserved2 = NULL;
	DWORD reserved3 = 0;
	if ( !PyArg_ParseTuple(args, "|izi:EnumElements", &reserved1, &szreserved2, &reserved3) )
		return NULL;
	IEnumSTATSTG *ppenum;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->EnumElements( reserved1, NULL, reserved3, &ppenum );
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);

	PyObject *obppenum = PyCom_PyObjectFromIUnknown(ppenum, IID_IEnumSTATSTG, FALSE);
	PyObject *pyretval = Py_BuildValue("O", obppenum);
	Py_XDECREF(obppenum);
	return pyretval;
}

// @pymethod |PyIStorage|DestroyElement|Removes the specified storage or stream from this storage object.
PyObject *PyIStorage::DestroyElement(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm string|name||The name of the element to be removed.
	PyObject *obName;
	if ( !PyArg_ParseTuple(args, "O:DestroyElement", &obName) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	BSTR bstrName;
	bPythonIsHappy = PyWinObject_AsBstr(obName, &bstrName);
	if (!bPythonIsHappy) return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->DestroyElement( bstrName );
	PY_INTERFACE_POSTCALL;
	PyWinObject_FreeBstr(bstrName);
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIStorage|RenameElement|Renames the specified substorage or stream in this storage object.
PyObject *PyIStorage::RenameElement(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm string|oldName||The name of the substorage or stream to be changed.
	// @pyparm string|newName||The new name for the specified sustorage or stream.
	PyObject *obNewName, *obOldName;
	if ( !PyArg_ParseTuple(args, "OO:RenameElement", &obOldName, &obNewName) )
		return NULL;
	BOOL bPythonIsHappy = TRUE;
	BSTR bstrNewName, bstrOldName;
	bPythonIsHappy = PyWinObject_AsBstr(obOldName, &bstrOldName);
	if (!PyWinObject_AsBstr(obNewName, &bstrNewName))
		bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->RenameElement( bstrOldName, bstrNewName );
	PyWinObject_FreeBstr(bstrOldName);
	PyWinObject_FreeBstr(bstrNewName);
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIStorage|SetElementTimes|Sets the modification, access, and creation times of the specified storage element, if supported by the underlying file system.
PyObject *PyIStorage::SetElementTimes(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm string|name||The name of the storage object element whose times are to be modified. If NULL, the time is set on the root storage rather than one of its elements. 
	// @pyparm <o PyTime>|ctime||Either the new creation time for the element or None if the creation time is not to be modified.
	// @pyparm <o PyTime>|atime||Either the new access time for the element or None if the access time is not to be modified.
	// @pyparm <o PyTime>|mtime||Either the new modification time for the element or None if the modification time is not to be modified.
	PyObject *obName;
	PyObject *obpctime;
	PyObject *obpatime;
	PyObject *obpmtime;
	if ( !PyArg_ParseTuple(args, "OOOO:SetElementTimes", &obName, &obpctime, &obpatime, &obpmtime) )
		return NULL;
	FILETIME *pctime=NULL, ctime;
	FILETIME *patime=NULL, atime;
	FILETIME *pmtime=NULL, mtime;
	if (obpctime!=Py_None) {
		pctime = &ctime;
		if (!PyTime_Check(obpctime)) {
			PyErr_SetString(PyExc_TypeError, "The argument must be a PyTime object");
			return NULL;
		}
		if (!((PyTime *)obpctime)->GetTime(pctime)) return NULL;
	}
	if (obpatime != Py_None) {
		patime = &atime;
		if (!PyTime_Check(obpatime)) {
			PyErr_SetString(PyExc_TypeError, "The argument must be a PyTime object");
			return NULL;
		}
		if (!((PyTime *)obpatime)->GetTime(patime)) return NULL;
	}
	if (obpmtime != Py_None) {
		pmtime = &mtime;
		if (!PyTime_Check(obpmtime)) {
			PyErr_SetString(PyExc_TypeError, "The argument must be a PyTime object");
			return NULL;
		}
		if (!((PyTime *)obpmtime)->GetTime(pmtime)) return NULL;
	}
	BSTR bstrName;
	if (!PyWinObject_AsBstr(obName, &bstrName))
		return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->SetElementTimes( bstrName, pctime, patime, pmtime );
	PyWinObject_FreeBstr(bstrName);
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod |PyIStorage|SetClass|Assigns the specified CLSID to this storage object.
PyObject *PyIStorage::SetClass(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm <o PyIID>|clsid||The class identifier (CLSID) that is to be associated with the storage object.
	PyObject *obclsid;
	if ( !PyArg_ParseTuple(args, "O:SetClass", &obclsid) )
		return NULL;
	IID clsid;
	BOOL bPythonIsHappy = TRUE;
	if (!PyWinObject_AsIID(obclsid, &clsid)) bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->SetClass( clsid );
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod |PyIStorage|SetStateBits|Stores up to 32 bits of state information in this storage object.
PyObject *PyIStorage::SetStateBits(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm int|grfStateBits||Specifies the new values of the bits to set. No legal values are defined for these bits; they are all reserved for future use and must not be used by applications.
	// @pyparm int|grfMask||A binary mask indicating which bits in grfStateBits are significant in this call. 
	DWORD grfStateBits;
	DWORD grfMask;
	if ( !PyArg_ParseTuple(args, "ii:SetStateBits", &grfStateBits, &grfMask) )
		return NULL;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->SetStateBits( grfStateBits, grfMask );
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);
	Py_INCREF(Py_None);
	return Py_None;

}

// @pymethod <o STATSTG>|PyIStorage|Stat|Retrieves the STATSTG structure for this open storage object.
PyObject *PyIStorage::Stat(PyObject *self, PyObject *args)
{
	IStorage *pIS = GetI(self);
	if ( pIS == NULL )
		return NULL;
	// @pyparm int|grfStatFlag||Specifies that some of the fields in the STATSTG structure are not returned, thus saving a memory allocation operation. Values are taken from the STATFLAG enumeration. 
	DWORD grfStatFlag = 0;
	if ( !PyArg_ParseTuple(args, "|i:Stat", &grfStatFlag) )
		return NULL;
	STATSTG pstatstg;
	PY_INTERFACE_PRECALL;
	HRESULT hr = pIS->Stat( &pstatstg, grfStatFlag );
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIS, IID_IStorage);

	PyObject *obpstatstg = PyCom_PyObjectFromSTATSTG(&pstatstg);
	// STATSTG doco says our responsibility to free
	if ((pstatstg).pwcsName) CoTaskMemFree((pstatstg).pwcsName);
	return obpstatstg;
}

// @object PyIStorage|Description of the interface
static struct PyMethodDef PyIStorage_methods[] =
{
	{ "CreateStream", PyIStorage::CreateStream, 1 }, // @pymeth CreateStream|Creates and opens a stream object with the specified name contained in this storage object.
	{ "OpenStream", PyIStorage::OpenStream, 1 }, // @pymeth OpenStream|Opens an existing stream object.
	{ "CreateStorage", PyIStorage::CreateStorage, 1 }, // @pymeth CreateStorage|Creates and opens a new storage object nested within this storage object.
	{ "OpenStorage", PyIStorage::OpenStorage, 1 }, // @pymeth OpenStorage|Opens an existing storage object with the specified name in the specified access mode.
	{ "CopyTo", PyIStorage::CopyTo, 1 }, // @pymeth CopyTo|Copies the entire contents of an open storage object to another storage object.
	{ "MoveElementTo", PyIStorage::MoveElementTo, 1 }, // @pymeth MoveElementTo|Copies or moves a substorage or stream from this storage object to another storage object.
	{ "Commit", PyIStorage::Commit, 1 }, // @pymeth Commit|Ensures that any changes made to a storage object open in transacted mode are reflected in the parent storage.
	{ "Revert", PyIStorage::Revert, 1 }, // @pymeth Revert|Discards all changes that have been made to the storage object since the last commit.
	{ "EnumElements", PyIStorage::EnumElements, 1 }, // @pymeth EnumElements|Retrieves an enumerator object that can be used to enumerate the storage and stream objects contained within this storage object.
	{ "DestroyElement", PyIStorage::DestroyElement, 1 }, // @pymeth DestroyElement|Removes the specified storage or stream from this storage object.
	{ "RenameElement", PyIStorage::RenameElement, 1 }, // @pymeth RenameElement|Renames the specified substorage or stream in this storage object.
	{ "SetElementTimes", PyIStorage::SetElementTimes, 1 }, // @pymeth SetElementTimes|Sets the modification, access, and creation times of the specified storage element, if supported by the underlying file system.
	{ "SetClass", PyIStorage::SetClass, 1 }, // @pymeth SetClass|Assigns the specified CLSID to this storage object.
	{ "SetStateBits", PyIStorage::SetStateBits, 1 }, // @pymeth SetStateBits|Stores up to 32 bits of state information in this storage object.
	{ "Stat", PyIStorage::Stat, 1 }, // @pymeth Stat|Retrieves the STATSTG structure for this open storage object.
	{ NULL }
};

PyComTypeObject PyIStorage::type("PyIStorage",
		&PyIUnknown::type, // @base PyIStorage|PyIUnknown
		sizeof(PyIStorage),
		PyIStorage_methods,
		GET_PYCOM_CTOR(PyIStorage));
// ---------------------------------------------------
//
// Gateway Implementation

STDMETHODIMP PyGStorage::CreateStream(
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsName,
		/* [in] */ DWORD grfMode,
		/* [in] */ DWORD reserved1,
		/* [in] */ DWORD reserved2,
		/* [out] */ IStream __RPC_FAR *__RPC_FAR * ppstm)
{
	PY_GATEWAY_METHOD;
	if (ppstm==NULL) return E_POINTER;
	PyObject *result;
	PyObject *obName = PyWinObject_FromWCHAR(pwcsName);
	HRESULT hr=InvokeViaPolicy("CreateStream", &result, "Oiii", obName, grfMode, reserved1, reserved2);
	Py_XDECREF(obName);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	PyObject *obppstm;
	if (!PyArg_Parse(result, "O" , &obppstm)) return
		MAKE_PYCOM_GATEWAY_FAILURE_CODE("CreateStream");
	BOOL bPythonIsHappy = TRUE;
	if (!PyCom_InterfaceFromPyObject(obppstm, IID_IStream, (void **)ppstm, FALSE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) hr = MAKE_PYCOM_GATEWAY_FAILURE_CODE("CreateStream");
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGStorage::OpenStream(
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsName,
		/* [unique][in] */ void __RPC_FAR * reserved1,
		/* [in] */ DWORD grfMode,
		/* [in] */ DWORD reserved2,
		/* [out] */ IStream __RPC_FAR *__RPC_FAR * ppstm)
{
	PY_GATEWAY_METHOD;
	if (ppstm==NULL) return E_POINTER;
	PyObject *result;
	PyObject *obName = PyWinObject_FromWCHAR(pwcsName);
	HRESULT hr=InvokeViaPolicy("OpenStream", &result, "Ozii", obName, NULL, grfMode, reserved2);
	Py_XDECREF(obName);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	PyObject *obppstm;
	if (!PyArg_Parse(result, "O" , &obppstm)) return
		MAKE_PYCOM_GATEWAY_FAILURE_CODE("OpenStream");
	BOOL bPythonIsHappy = TRUE;
	if (!PyCom_InterfaceFromPyObject(obppstm, IID_IStream, (void **)ppstm, FALSE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) hr = MAKE_PYCOM_GATEWAY_FAILURE_CODE("OpenStream");
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGStorage::CreateStorage(
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsName,
		/* [in] */ DWORD grfMode,
		/* [in] */ DWORD dwStgFmt,
		/* [in] */ DWORD reserved2,
		/* [out] */ IStorage __RPC_FAR *__RPC_FAR * ppstg)
{
	PY_GATEWAY_METHOD;
	if (ppstg==NULL) return E_POINTER;
	PyObject *result;
	PyObject *obName = PyWinObject_FromWCHAR(pwcsName);
	HRESULT hr=InvokeViaPolicy("CreateStorage", &result, "Oiii", obName, grfMode, dwStgFmt, reserved2);
	Py_XDECREF(obName);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	PyObject *obppstg;
	if (!PyArg_Parse(result, "O" , &obppstg))
		return MAKE_PYCOM_GATEWAY_FAILURE_CODE("CreateStorage");
	BOOL bPythonIsHappy = TRUE;
	if (!PyCom_InterfaceFromPyObject(obppstg, IID_IStorage, (void **)ppstg, FALSE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) hr = MAKE_PYCOM_GATEWAY_FAILURE_CODE("CreateStorage");
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGStorage::OpenStorage(
		/* [string][unique][in] */ const OLECHAR __RPC_FAR * pwcsName,
		/* [unique][in] */ IStorage __RPC_FAR * pstgPriority,
		/* [in] */ DWORD grfMode,
		/* [unique][in] */ SNB snbExclude,
		/* [in] */ DWORD reserved,
		/* [out] */ IStorage __RPC_FAR *__RPC_FAR * ppstg)
{
	PY_GATEWAY_METHOD;
// *** The input argument snbExclude of type "SNB" was not processed ***
//   - None will always be passed to the Python function as a placeholder
//   - The type 'SNB' (snbExclude) is unknown.
	if (ppstg==NULL) return E_POINTER;
	PyObject *obpstgPriority = PyCom_PyObjectFromIUnknown(pstgPriority, IID_IStorage, TRUE);
	PyObject *result;
	PyObject *obName = PyWinObject_FromWCHAR(pwcsName);
	HRESULT hr=InvokeViaPolicy("OpenStorage", &result, "OOizi", obName, obpstgPriority, grfMode, NULL, reserved);
	Py_XDECREF(obName);
	Py_XDECREF(obpstgPriority);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	PyObject *obppstg;
	if (!PyArg_Parse(result, "O" , &obppstg))
		return MAKE_PYCOM_GATEWAY_FAILURE_CODE("OpenStorage");
	BOOL bPythonIsHappy = TRUE;
	if (!PyCom_InterfaceFromPyObject(obppstg, IID_IStorage, (void **)ppstg, FALSE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) hr = MAKE_PYCOM_GATEWAY_FAILURE_CODE("OpenStorage");
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGStorage::CopyTo(
		/* [in] */ DWORD ciidExclude,
		/* [size_is][unique][in] */ const IID __RPC_FAR * rgiidExclude,
		/* [unique][in] */ SNB snbExclude,
		/* [unique][in] */ IStorage __RPC_FAR * pstgDest)
{
	PY_GATEWAY_METHOD;
// *** The input argument snbExclude of type "SNB" was not processed ***
//   - None will always be passed to the Python function as a placeholder
//   - The type 'SNB' (snbExclude) is unknown.
	PyObject *obrgiidExclude = PyWinObject_FromIID(*rgiidExclude);
	PyObject *obpstgDest = PyCom_PyObjectFromIUnknown(pstgDest, IID_IStorage, TRUE);
	HRESULT hr=InvokeViaPolicy("CopyTo", NULL, "iOzO", ciidExclude, obrgiidExclude, NULL, obpstgDest);
	Py_XDECREF(obrgiidExclude);
	Py_XDECREF(obpstgDest);
	return hr;
}

STDMETHODIMP PyGStorage::MoveElementTo(
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsName,
		/* [unique][in] */ IStorage __RPC_FAR * pstgDest,
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsNewName,
		/* [in] */ DWORD grfFlags)
{
	PY_GATEWAY_METHOD;
	PyObject *obpstgDest = PyCom_PyObjectFromIUnknown(pstgDest, IID_IStorage, TRUE);
	PyObject *obName = PyWinObject_FromWCHAR(pwcsName);
	PyObject *obNewName = PyWinObject_FromWCHAR(pwcsNewName);
	HRESULT hr=InvokeViaPolicy("MoveElementTo", NULL, "OOOi", obName, obpstgDest, obNewName, grfFlags);
	Py_XDECREF(obpstgDest);
	Py_XDECREF(obName);
	Py_XDECREF(obNewName);
	return hr;
}

STDMETHODIMP PyGStorage::Commit(
		/* [in] */ DWORD grfCommitFlags)
{
	PY_GATEWAY_METHOD;
	HRESULT hr=InvokeViaPolicy("Commit", NULL, "i", grfCommitFlags);
	return hr;
}

STDMETHODIMP PyGStorage::Revert(
		void)
{
	PY_GATEWAY_METHOD;
	HRESULT hr=InvokeViaPolicy("Revert", NULL, "i");
	return hr;
}

STDMETHODIMP PyGStorage::EnumElements(
		/* [in] */ DWORD reserved1,
		/* [size_is][unique][in] */ void __RPC_FAR * reserved2,
		/* [in] */ DWORD reserved3,
		/* [out] */ IEnumSTATSTG __RPC_FAR *__RPC_FAR * ppenum)
{
	PY_GATEWAY_METHOD;
	if (ppenum==NULL) return E_POINTER;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("EnumElements", &result, "izi", reserved1, NULL, reserved3);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	PyObject *obppenum;
	if (!PyArg_Parse(result, "O" , &obppenum))
		return MAKE_PYCOM_GATEWAY_FAILURE_CODE("Revert");
	BOOL bPythonIsHappy = TRUE;
	if (!PyCom_InterfaceFromPyObject(obppenum, IID_IEnumSTATSTG, (void **)ppenum, FALSE /* bNoneOK */))
		 bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) hr = MAKE_PYCOM_GATEWAY_FAILURE_CODE("Revert");
	Py_DECREF(result);
	return hr;
}

STDMETHODIMP PyGStorage::DestroyElement(
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsName)
{
	PY_GATEWAY_METHOD;
	PyObject *obName = PyWinObject_FromWCHAR(pwcsName);
	HRESULT hr=InvokeViaPolicy("DestroyElement", NULL, "O", obName);
	Py_XDECREF(obName);
	return hr;
}

STDMETHODIMP PyGStorage::RenameElement(
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsOldName,
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsNewName)
{
	PY_GATEWAY_METHOD;
	PyObject *obOldName = PyWinObject_FromWCHAR(pwcsOldName);
	PyObject *obNewName = PyWinObject_FromWCHAR(pwcsNewName);
	HRESULT hr=InvokeViaPolicy("RenameElement", NULL, "OO", obOldName, obNewName);
	Py_XDECREF(obOldName);
	Py_XDECREF(obNewName);
	return hr;
}

STDMETHODIMP PyGStorage::SetElementTimes(
		/* [string][in] */ const OLECHAR __RPC_FAR * pwcsName,
		/* [in] */ const FILETIME __RPC_FAR * pctime,
		/* [in] */ const FILETIME __RPC_FAR * patime,
		/* [in] */ const FILETIME __RPC_FAR * pmtime)
{
	PY_GATEWAY_METHOD;
	PyObject *obpctime = new PyTime(*pctime);
	PyObject *obpatime = new PyTime(*patime);
	PyObject *obpmtime = new PyTime(*pmtime);
	PyObject *obName = PyWinObject_FromWCHAR(pwcsName);
	HRESULT hr=InvokeViaPolicy("SetElementTimes", NULL, "OOOO", obName, obpctime, obpatime, obpmtime);
	Py_XDECREF(obpctime);
	Py_XDECREF(obpatime);
	Py_XDECREF(obpmtime);
	Py_XDECREF(obName);
	return hr;
}

STDMETHODIMP PyGStorage::SetClass(
		/* [in] */ REFCLSID clsid)
{
	PY_GATEWAY_METHOD;
	PyObject *obclsid = PyWinObject_FromIID(clsid);
	HRESULT hr=InvokeViaPolicy("SetClass", NULL, "O", obclsid);
	Py_XDECREF(obclsid);
	return hr;
}

STDMETHODIMP PyGStorage::SetStateBits(
		/* [in] */ DWORD grfStateBits,
		/* [in] */ DWORD grfMask)
{
	PY_GATEWAY_METHOD;
	HRESULT hr=InvokeViaPolicy("SetStateBits", NULL, "ii", grfStateBits, grfMask);
	return hr;
}

STDMETHODIMP PyGStorage::Stat(
		/* [out] */ STATSTG __RPC_FAR * pstatstg,
		/* [in] */ DWORD grfStatFlag)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr=InvokeViaPolicy("Stat", &result, "i", grfStatFlag);
	if (FAILED(hr)) return hr;
	// Process the Python results, and convert back to the real params
	PyObject *obpstatstg;
	if (!PyArg_Parse(result, "O" , &obpstatstg))
		return MAKE_PYCOM_GATEWAY_FAILURE_CODE("Stat");
	BOOL bPythonIsHappy = TRUE;
	if (!PyCom_PyObjectAsSTATSTG(obpstatstg, pstatstg, 0/*flags*/)) bPythonIsHappy = FALSE;
	if (!bPythonIsHappy) hr = MAKE_PYCOM_GATEWAY_FAILURE_CODE("Stat");
	Py_DECREF(result);
	return hr;
}

