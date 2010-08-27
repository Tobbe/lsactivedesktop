#include "tdochostuihandler.h"
#include "webform.h"

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::QueryInterface(REFIID riid, void **ppv)
{
	return webf->QueryInterface(riid, ppv);
}

ULONG STDMETHODCALLTYPE TDocHostUIHandler::AddRef()
{
	return webf->AddRef();
}

ULONG STDMETHODCALLTYPE TDocHostUIHandler::Release()
{
	return webf->Release();
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
	pInfo->dwFlags = (webf->hasScrollbars ? 0 : DOCHOSTUIFLAG_SCROLL_NO) | DOCHOSTUIFLAG_NO3DOUTERBORDER;
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::ShowUI(DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::HideUI()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::UpdateUI()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::EnableModeless(BOOL fEnable)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::OnDocWindowActivate(BOOL fActivate)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::OnFrameWindowActivate(BOOL fActivate)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fRameWindow)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::GetOptionKeyPath(LPOLESTR *pchKey, DWORD dw)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::GetDropTarget(IDropTarget *pDropTarget, IDropTarget **ppDropTarget)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::GetExternal(IDispatch **ppDispatch)
{
	/*IWebBrowser2 *wb2 = webf->ibrowser;

	if (wb2 == NULL) {
		*ppDispatch = NULL;
		return S_FALSE;
	}

	wb2->get_Application(ppDispatch);

	if (ppDispatch == NULL) {
		return S_FALSE;
	}*/

	*ppDispatch = &webf->dispatch;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::TranslateUrl(DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut)
{
	*ppchURLOut = 0;

	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TDocHostUIHandler::FilterDataObject(IDataObject *pDO, IDataObject **ppDORet)
{
	*ppDORet = 0;

	return S_FALSE;
}
