#ifndef _TOLEINPLACEFRAME_H__
#define _TOLEINPLACEFRAME_H__

#include <windows.h>
#include <mshtmhst.h>
#include <mshtmdid.h>
#include <exdispid.h>

class TWebf;

class TOleInPlaceFrame : public IOleInPlaceFrame {
public:
	TWebf *webf;

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// IOleWindow
	HRESULT STDMETHODCALLTYPE GetWindow(HWND *phwnd);
	HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);

	// IOleInPlaceUIWindow
	HRESULT STDMETHODCALLTYPE GetBorder(LPRECT lprectBorder);
	HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS pborderwidths);
	HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS pborderwidths);
	HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName);

	// IOleInPlaceFrame
	HRESULT STDMETHODCALLTYPE InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
	HRESULT STDMETHODCALLTYPE SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
	HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU hmenuShared);
	HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR pszStatusText);
	HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable);
	HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpmsg, WORD wID);
};

#endif
