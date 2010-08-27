#ifndef _TOLEINPLACESITE_H__
#define _TOLEINPLACESITE_H__

#include <windows.h>
#include <mshtmhst.h>
#include <mshtmdid.h>
#include <exdispid.h>

class TWebf;

class TOleInPlaceSite : public IOleInPlaceSite {
public:
	TWebf *webf;

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// IOleWindow
	HRESULT STDMETHODCALLTYPE GetWindow(HWND *phwnd);
	HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);

	// IOleInPlaceSite
	HRESULT STDMETHODCALLTYPE CanInPlaceActivate();
	HRESULT STDMETHODCALLTYPE OnInPlaceActivate();
	HRESULT STDMETHODCALLTYPE OnUIActivate();
	HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame **ppFrame,
		IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect,
		LPOLEINPLACEFRAMEINFO info);
	HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant);
	HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable);
	HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate();
	HRESULT STDMETHODCALLTYPE DiscardUndoState();
	HRESULT STDMETHODCALLTYPE DeactivateAndUndo();
	HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect);
};

#endif