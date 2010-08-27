#ifndef _TDOCHOSTSHOWUI_H__
#define _TDOCHOSTSHOWUI_H__

#include <windows.h>
#include <mshtmhst.h>

class TWebf;

class TDocHostShowUI : public IDocHostShowUI {
public:
	TWebf *webf;

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// IDocHostShowUI
	HRESULT STDMETHODCALLTYPE ShowMessage(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult);
	HRESULT STDMETHODCALLTYPE ShowHelp(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand, DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit);
};

#endif