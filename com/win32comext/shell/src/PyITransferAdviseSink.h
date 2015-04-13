// This file declares the ITransferAdviseSink Interface and Gateway for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Interface Declaration

class PyITransferAdviseSink : public PyIUnknown
{
public:
	MAKE_PYCOM_CTOR(PyITransferAdviseSink);
	static ITransferAdviseSink *GetI(PyObject *self);
	static PyComTypeObject type;

	// The Python methods
	static PyObject *UpdateProgress(PyObject *self, PyObject *args);
	static PyObject *UpdateTransferState(PyObject *self, PyObject *args);
	static PyObject *ConfirmOverwrite(PyObject *self, PyObject *args);
	static PyObject *ConfirmEncryptionLoss(PyObject *self, PyObject *args);
	static PyObject *FileFailure(PyObject *self, PyObject *args);
	static PyObject *SubStreamFailure(PyObject *self, PyObject *args);
	static PyObject *PropertyFailure(PyObject *self, PyObject *args);

protected:
	PyITransferAdviseSink(IUnknown *pdisp);
	~PyITransferAdviseSink();
};
// ---------------------------------------------------
//
// Gateway Declaration

class PyGTransferAdviseSink : public PyGatewayBase, public ITransferAdviseSink
{
protected:
	PyGTransferAdviseSink(PyObject *instance) : PyGatewayBase(instance) { ; }
	PYGATEWAY_MAKE_SUPPORT2(PyGTransferAdviseSink, ITransferAdviseSink, IID_ITransferAdviseSink, PyGatewayBase)



	// ITransferAdviseSink
	STDMETHOD(UpdateProgress)(
		ULONGLONG ullSizeCurrent,
		ULONGLONG ullSizeTotal,
		int nFilesCurrent,
		int nFilesTotal,
		int nFoldersCurrent,
		int nFoldersTotal);

	STDMETHOD(UpdateTransferState)(
		TRANSFER_ADVISE_STATE ts);

	STDMETHOD(ConfirmOverwrite)(
		IShellItem * psiSource,
		IShellItem * psiDestParent,
		LPCWSTR pszName);

	STDMETHOD(ConfirmEncryptionLoss)(
		IShellItem * psiSource);

	STDMETHOD(FileFailure)(
		IShellItem * psi,
		LPCWSTR pszItem,
		HRESULT hrError,
		LPWSTR pszRename,
		ULONG cchRename);

	STDMETHOD(SubStreamFailure)(
		IShellItem * psi,
		LPCWSTR pszStreamName,
		HRESULT hrError);

	STDMETHOD(PropertyFailure)(
		IShellItem * psi,
		const PROPERTYKEY * pkey,
		HRESULT hrError);

};
