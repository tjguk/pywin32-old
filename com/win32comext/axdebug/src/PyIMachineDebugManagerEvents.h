// This file declares the IMachineDebugManagerEvents Interface and Gateway for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Interface Declaration

class PyIMachineDebugManagerEvents : public PyIUnknown
{
public:
	MAKE_PYCOM_CTOR(PyIMachineDebugManagerEvents);
	static IMachineDebugManagerEvents *GetI(PyObject *self);
	static PyComTypeObject type;

	// The Python methods
	static PyObject *onAddApplication(PyObject *self, PyObject *args);
	static PyObject *onRemoveApplication(PyObject *self, PyObject *args);

protected:
	PyIMachineDebugManagerEvents(IUnknown *pdisp);
	~PyIMachineDebugManagerEvents();
};
// ---------------------------------------------------
//
// Gateway Declaration

class PyGMachineDebugManagerEvents : public PyGatewayBase, public IMachineDebugManagerEvents
{
protected:
	PyGMachineDebugManagerEvents(PyObject *instance) : PyGatewayBase(instance) { ; }
	PYGATEWAY_MAKE_SUPPORT(PyGMachineDebugManagerEvents, IMachineDebugManagerEvents, IID_IMachineDebugManagerEvents)

	// IMachineDebugManagerEvents
	STDMETHOD(onAddApplication)(
		IRemoteDebugApplication __RPC_FAR * pda,
		DWORD dwAppCookie);

	STDMETHOD(onRemoveApplication)(
		IRemoteDebugApplication __RPC_FAR * pda,
		DWORD dwAppCookie);

};
