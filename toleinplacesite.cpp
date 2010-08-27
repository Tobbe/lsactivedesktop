#include <windows.h>
#include <mshtmhst.h>
#include <mshtmdid.h>
#include <exdispid.h>
#include <tchar.h>
#include <list>
#include <string>

#include "toleinplacesite.h"
#include "webform.h"

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::QueryInterface(REFIID riid, void **ppv)
{
	return webf->QueryInterface(riid, ppv);
}

ULONG STDMETHODCALLTYPE TOleInPlaceSite::AddRef()
{
	return webf->AddRef();
}

ULONG STDMETHODCALLTYPE TOleInPlaceSite::Release()
{
	return webf->Release();
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::GetWindow(HWND *phwnd)
{
	*phwnd = webf->hhost;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::ContextSensitiveHelp(BOOL fEnterMode)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::CanInPlaceActivate()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::OnInPlaceActivate()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::OnUIActivate()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::GetWindowContext(IOleInPlaceFrame **ppFrame,
										   IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect,
										   LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO info)
{
	*ppFrame = &webf->frame;
	webf->frame.AddRef();
	*ppDoc = NULL;
	info->fMDIApp = FALSE;
	info->hwndFrame = webf->hhost;
	info->haccel = 0;
	info->cAccelEntries = 0;
	GetClientRect(webf->hhost, lprcPosRect);
	GetClientRect(webf->hhost, lprcClipRect);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::Scroll(SIZE scrollExtant)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::OnUIDeactivate(BOOL fUndoable)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::OnInPlaceDeactivate()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::DiscardUndoState()
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::DeactivateAndUndo()
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE TOleInPlaceSite::OnPosRectChange(LPCRECT lprcPosRect)
{
	IOleInPlaceObject *iole = NULL;
	webf->ibrowser->QueryInterface(IID_IOleInPlaceObject, (void**)&iole);

	if (iole != NULL) {
		iole->SetObjectRects(lprcPosRect, lprcPosRect);
		iole->Release();
	}

	return S_OK;
}
