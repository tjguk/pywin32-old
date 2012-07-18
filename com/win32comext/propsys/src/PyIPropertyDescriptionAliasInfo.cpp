// This file implements the IPropertyDescriptionAliasInfo Interface and Gateway for Python.
// Generated by makegw.py
#include "PyIPropertyDescription.h"
#include "PyIPropertyDescriptionAliasInfo.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIPropertyDescriptionAliasInfo::PyIPropertyDescriptionAliasInfo(IUnknown *pdisp):
	PyIPropertyDescription(pdisp)
{
	ob_type = &type;
}

PyIPropertyDescriptionAliasInfo::~PyIPropertyDescriptionAliasInfo()
{
}

/* static */ IPropertyDescriptionAliasInfo *PyIPropertyDescriptionAliasInfo::GetI(PyObject *self)
{
	return (IPropertyDescriptionAliasInfo *)PyIPropertyDescription::GetI(self);
}

// @pymethod <o PyIPropertyDescription>|PyIPropertyDescriptionAliasInfo|GetSortByAlias|Returns the primary column used for sorting
PyObject *PyIPropertyDescriptionAliasInfo::GetSortByAlias(PyObject *self, PyObject *args)
{
	IPropertyDescriptionAliasInfo *pIPDAI = GetI(self);
	if ( pIPDAI == NULL )
		return NULL;
	IID riid = IID_IPropertyDescription;
	void *pv;
	// @pyparm <o PyIID>|riid|IID_IPropertyDescription|The interface to return
	if ( !PyArg_ParseTuple(args, "|O&:GetSortByAlias", PyWinObject_AsIID, &riid))
		return NULL;

	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIPDAI->GetSortByAlias( riid, &pv);
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIPDAI, IID_IPropertyDescriptionAliasInfo );
	return PyCom_PyObjectFromIUnknown((IUnknown *)pv, riid);
}

// @pymethod <o PyIPropertyDescriptionList>|PyIPropertyDescriptionAliasInfo|GetAdditionalSortByAliases|Returns secondary sorting columns
PyObject *PyIPropertyDescriptionAliasInfo::GetAdditionalSortByAliases(PyObject *self, PyObject *args)
{
	IPropertyDescriptionAliasInfo *pIPDAI = GetI(self);
	if ( pIPDAI == NULL )
		return NULL;
	IID riid = IID_IPropertyDescriptionList;
	void *pv;
	// @pyparm <o PyIID>|riid|IID_IPropertyDescriptionList|The interface to return
	if ( !PyArg_ParseTuple(args, "|O&:GetAdditionalSortByAliases", PyWinObject_AsIID, &riid))
		return NULL;

	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pIPDAI->GetAdditionalSortByAliases( riid, &pv );
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pIPDAI, IID_IPropertyDescriptionAliasInfo );
	return PyCom_PyObjectFromIUnknown((IUnknown *)pv, riid);
}

// @object PyIPropertyDescriptionAliasInfo|Interface that gives access to the sorting columns for a property
static struct PyMethodDef PyIPropertyDescriptionAliasInfo_methods[] =
{
	{ "GetSortByAlias", PyIPropertyDescriptionAliasInfo::GetSortByAlias, 1 }, // @pymeth GetSortByAlias|Returns the primary column used for sorting
	{ "GetAdditionalSortByAliases", PyIPropertyDescriptionAliasInfo::GetAdditionalSortByAliases, 1 }, // @pymeth GetAdditionalSortByAliases|Returns secondary sorting columns
	{ NULL }
};

PyComTypeObject PyIPropertyDescriptionAliasInfo::type("PyIPropertyDescriptionAliasInfo",
		&PyIPropertyDescription::type,
		sizeof(PyIPropertyDescriptionAliasInfo),
		PyIPropertyDescriptionAliasInfo_methods,
		GET_PYCOM_CTOR(PyIPropertyDescriptionAliasInfo));
