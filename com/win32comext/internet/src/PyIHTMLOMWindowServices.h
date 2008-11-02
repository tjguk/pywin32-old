// This file declares the IHTMLOMWindowServices Interface and Gateway for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Gateway Declaration

class PyGHTMLOMWindowServices : public PyGatewayBase, public IHTMLOMWindowServices
{
protected:
	PyGHTMLOMWindowServices(PyObject *instance) : PyGatewayBase(instance) { ; }
	PYGATEWAY_MAKE_SUPPORT2(PyGHTMLOMWindowServices, IHTMLOMWindowServices, IID_IHTMLOMWindowServices, PyGatewayBase)

	// IHTMLOMWindowServices
	STDMETHOD(moveTo)(
		LONG x,
		LONG y);

	STDMETHOD(moveBy)(
		LONG x,
		LONG y);

	STDMETHOD(resizeTo)(
		LONG x,
		LONG y);

	STDMETHOD(resizeBy)(
		LONG x,
		LONG y);

};
