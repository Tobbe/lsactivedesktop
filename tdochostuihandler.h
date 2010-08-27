#ifndef _TDOCHOSTUIHANDLER_H__
#define _TDOCHOSTUIHANDLER_H__

#include <windows.h>
#include <mshtmhst.h>
#include <mshtmdid.h>
#include <exdispid.h>

class TWebf;

class TDocHostUIHandler : public IDocHostUIHandler {
public:
	TWebf *webf;

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// IDocHostUIHandler
	HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);
	HRESULT STDMETHODCALLTYPE GetHostInfo(DOCHOSTUIINFO *pInfo);
	HRESULT STDMETHODCALLTYPE ShowUI(DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc);
	HRESULT STDMETHODCALLTYPE HideUI();
	HRESULT STDMETHODCALLTYPE UpdateUI();
	HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable);
	HRESULT STDMETHODCALLTYPE OnDocWindowActivate(BOOL fActivate);
	HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(BOOL fActivate);
	HRESULT STDMETHODCALLTYPE ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fRameWindow);
	HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID);
	HRESULT STDMETHODCALLTYPE GetOptionKeyPath(LPOLESTR *pchKey, DWORD dw);
	HRESULT STDMETHODCALLTYPE GetDropTarget(IDropTarget *pDropTarget, IDropTarget **ppDropTarget);
	HRESULT STDMETHODCALLTYPE GetExternal(IDispatch **ppDispatch);
	HRESULT STDMETHODCALLTYPE TranslateUrl(DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut);
	HRESULT STDMETHODCALLTYPE FilterDataObject(IDataObject *pDO, IDataObject **ppDORet);
};

#endif
