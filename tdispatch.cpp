#include "tdispatch.h"
#include "webform.h"

HRESULT STDMETHODCALLTYPE TDispatch::QueryInterface(REFIID riid, void **ppv)
{
	return webf->QueryInterface(riid, ppv);
}

ULONG STDMETHODCALLTYPE TDispatch::AddRef()
{
	return webf->AddRef();
}

ULONG STDMETHODCALLTYPE TDispatch::Release()
{
	return webf->Release();
}

HRESULT STDMETHODCALLTYPE TDispatch::GetTypeInfoCount(UINT *pctinfo)
{
	*pctinfo = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDispatch::GetTypeInfo(UINT iTInfo, LCID lcid,
	ITypeInfo **ppTInfo)
{
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE TDispatch::GetIDsOfNames(REFIID riid,
	LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE TDispatch::Invoke(DISPID dispIdMember, REFIID riid,
	LCID lcid, WORD wFlags, DISPPARAMS *Params, VARIANT *pVarResult,
	EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	switch (dispIdMember) { // DWebBrowserEvents2
			case DISPID_BEFORENAVIGATE2:
				break;
			case DISPID_DOCUMENTCOMPLETE:
				webf->DocumentComplete(Params->rgvarg[0].pvarVal->bstrVal);
				break;
			case DISPID_AMBIENT_DLCONTROL:
				pVarResult->vt = VT_I4;
				pVarResult->lVal = DLCTL_DLIMAGES | DLCTL_VIDEOS | DLCTL_BGSOUNDS | DLCTL_SILENT;
				break;
			default:
				return DISP_E_MEMBERNOTFOUND;
	}

	return S_OK;
}
