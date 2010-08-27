#ifndef _TDISPATCH_H__
#define _TDISPATCH_H__

#include <windows.h>
#include <mshtmhst.h>

class TWebf;

struct TDispatch : public IDispatch {
	TWebf *webf;

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// IDispatch
	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);
	HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid,
		ITypeInfo **ppTInfo);
	HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid,
		LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
	HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid,
		LCID lcid, WORD wFlags, DISPPARAMS *Params, VARIANT *pVarResult,
		EXCEPINFO *pExcepInfo, UINT *puArgErr);
};

#endif;
