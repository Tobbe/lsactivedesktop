#include "tdochostshowui.h"
#include "webform.h"

HRESULT STDMETHODCALLTYPE TDocHostShowUI::QueryInterface(REFIID riid, void **ppv)
{
	return webf->QueryInterface(riid, ppv);
}

ULONG STDMETHODCALLTYPE TDocHostShowUI::AddRef()
{
	return webf->AddRef();
}

ULONG STDMETHODCALLTYPE TDocHostShowUI::Release()
{
	return webf->Release();
}

HRESULT STDMETHODCALLTYPE TDocHostShowUI::ShowMessage(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult)
{
	*plResult = IDCANCEL;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostShowUI::ShowHelp(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand, DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit)
{
	return S_OK;
}
