// This file declares the IEnumFORMATETC Interface and Gateway for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Interface Declaration

class PyIEnumFORMATETC : public PyIUnknown
{
public:
	MAKE_PYCOM_CTOR(PyIEnumFORMATETC);
	static IEnumFORMATETC *GetI(PyObject *self);
	static PyComTypeObject type;

	virtual PyObject *iter();
	virtual PyObject *iternext();

	// The Python methods
	static PyObject *Next(PyObject *self, PyObject *args);
	static PyObject *Skip(PyObject *self, PyObject *args);
	static PyObject *Reset(PyObject *self, PyObject *args);
	static PyObject *Clone(PyObject *self, PyObject *args);

protected:
	PyIEnumFORMATETC(IUnknown *pdisp);
	~PyIEnumFORMATETC();
};
// ---------------------------------------------------
//
// Gateway Declaration

class PyGEnumFORMATETC : public PyGatewayBase, public IEnumFORMATETC
{
protected:
	PyGEnumFORMATETC(PyObject *instance) : PyGatewayBase(instance) { ; }
	PYGATEWAY_MAKE_SUPPORT2(PyGEnumFORMATETC, IEnumFORMATETC, IID_IEnumFORMATETC, PyGatewayBase)

	// IEnumFORMATETC
	STDMETHOD(Next)(
		ULONG celt,
		FORMATETC *pi,
		ULONG __RPC_FAR * pcEltsfetched);

	STDMETHOD(Skip)(
		ULONG celt);

	STDMETHOD(Reset)(
		void);

	STDMETHOD(Clone)(
		IEnumFORMATETC __RPC_FAR *__RPC_FAR * ppepi);
};
