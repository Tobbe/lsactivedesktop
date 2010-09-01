#include <windows.h>
#include <mshtmhst.h>
#include <mshtmdid.h>
#include <exdispid.h>
#include <tchar.h>
#include <string>
#include "webform.h"
#include "TOleClientSite.h"
#include "toleinplaceframe.h"

TWebf::TWebf() :
	ref(1), ibrowser(NULL), cookie(0), isnaving(0), url(NULL), kurl(NULL),
	hasScrollbars(false), hhost(NULL), hf(INVALID_HANDLE_VALUE)
{
}

void TWebf::setupOle()
{
	RECT rc;
	GetClientRect(hhost, &rc);

	HRESULT hr;
	IOleObject* iole = 0;
	hr = CoCreateInstance(CLSID_WebBrowser, NULL, CLSCTX_INPROC_SERVER, IID_IOleObject, (void**)&iole);
	if (iole == 0) {
		return;
	}

	hr = iole->SetClientSite(this);
	if (hr != S_OK) {
		iole->Release();
		return;
	}

	hr = iole->SetHostNames(L"MyHost", L"MyDoc");
	if (hr != S_OK) {
		iole->Release();
		return;
	}

	hr = OleSetContainedObject(iole, TRUE);
	if (hr != S_OK) {
		iole->Release();
		return;
	}

	hr = iole->DoVerb(OLEIVERB_SHOW, 0, this, 0, hhost, &rc);
	if (hr != S_OK) {
		iole->Release();
		return;
	}

	bool connected = false;
	IConnectionPointContainer *cpc = 0;
	iole->QueryInterface(IID_IConnectionPointContainer, (void**)&cpc);
	if (cpc != 0) {
		IConnectionPoint *cp = 0;
		cpc->FindConnectionPoint(DIID_DWebBrowserEvents2, &cp);

		if (cp != 0) {
			cp->Advise(this, &cookie);
			cp->Release();
			connected = true;
		}

		cpc->Release();
	}

	if (!connected) {
		iole->Release();
		return;
	}

	iole->QueryInterface(IID_IWebBrowser2, (void**)&ibrowser);
	iole->Release();
}

void TWebf::Close()
{
	if (ibrowser != 0) {
		IConnectionPointContainer *cpc = 0;
		ibrowser->QueryInterface(IID_IConnectionPointContainer, (void**)&cpc);

		if (cpc != 0) {
			IConnectionPoint *cp = 0;
			cpc->FindConnectionPoint(DIID_DWebBrowserEvents2, &cp);

			if (cp != 0) {
				cp->Unadvise(cookie);
				cp->Release();
			}

			cpc->Release();
		}

		IOleObject *iole = 0;
		ibrowser->QueryInterface(IID_IOleObject, (void**)&iole);
		ibrowser->Release();
		ibrowser = 0;

		if (iole != 0) {
			iole->Close(OLECLOSE_NOSAVE);
			iole->Release();
		}
	}
}

TWebf::~TWebf()
{
	if (hf != INVALID_HANDLE_VALUE) {
		CloseHandle(hf);
	}
	
	hf = INVALID_HANDLE_VALUE;

	if (url != 0) {
		delete[] url;
	}

	if (kurl != 0) {
		delete[] kurl;
	}
}


void TCharToWide(const char *src, wchar_t *dst, int dst_size_in_wchars)
{
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, -1, dst, dst_size_in_wchars);
}

void TCharToWide(const wchar_t *src, wchar_t *dst, int dst_size_in_wchars)
{
	#pragma warning(suppress:4996)
	wcscpy(dst, src);
}

void TWebf::Go(const TCHAR *url)
{
	if (url == NULL || ibrowser == NULL) {
		return;
	}

	wchar_t ws[MAX_PATH];
	TCharToWide(url, ws, MAX_PATH);
	isnaving = 7;
	VARIANT v;
	v.vt = VT_I4;
	v.lVal = 0; // v.lVal = navNoHistory;
	ibrowser->Navigate(ws, &v, NULL, NULL, NULL);

	// (Special case: maybe it's already loaded by the time we get here!)
	if ((isnaving & 2) == 0) {
		WPARAM w = (GetWindowLong(hhost, GWL_ID) & 0xFFFF) | ((WEBFN_LOADED & 0xFFFF) << 16);
		PostMessage(GetParent(hhost), WM_COMMAND, w, (LPARAM)hhost);
	}

	isnaving &= ~4;
}

void TWebf::DocumentComplete(const wchar_t *)
{
	isnaving &= ~2;

	if (isnaving & 4) {
		return; // "4" means that we're in the middle of Go(), so the notification will be handled there
	}

	WPARAM w = (GetWindowLong(hhost, GWL_ID) & 0xFFFF) | ((WEBFN_LOADED & 0xFFFF) << 16);
	PostMessage(GetParent(hhost), WM_COMMAND, w, (LPARAM)hhost);
}

HRESULT STDMETHODCALLTYPE TWebf::QueryInterface(REFIID riid, void **ppv)
{
	*ppv = NULL;

	/*if (riid == IID_IUnknown) {
		*ppv = static_cast<IUnknown*>(this);
	} else */if (riid == IID_IUnknown || riid == IID_IOleClientSite) {
		*ppv = static_cast<IOleClientSite*>(this); //&clientsite;
	} else if (riid == IID_IOleWindow || riid == IID_IOleInPlaceSite) {
		*ppv = static_cast<IOleInPlaceSite*>(this);//&site;
	} else if (riid == IID_IOleInPlaceUIWindow) {
		*ppv = static_cast<IOleInPlaceUIWindow*>(this);//&frame;
	} else if (riid == IID_IOleInPlaceFrame) {
		*ppv = static_cast<IOleInPlaceFrame*>(this);//&frame;
	} else if (riid == IID_IDispatch) {
		*ppv = static_cast<IDispatch*>(this); //&dispatch;
	} else if (riid == IID_IDocHostUIHandler) {
		*ppv = static_cast<IDocHostUIHandler*>(this); //&uihandler;
	} else if (riid == IID_IDocHostShowUI) {
		*ppv = static_cast<IDocHostShowUI*>(this);
	}

	if (*ppv != NULL) {
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE TWebf::AddRef()
{
	return InterlockedIncrement(&ref);
}

ULONG STDMETHODCALLTYPE TWebf::Release()
{
	int tmp = InterlockedDecrement(&ref);
	
	if (tmp == 0) {
		delete this;
	}
	
	return tmp;
}

LRESULT CALLBACK TWebf::WebformWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*if (msg == WM_NCCREATE) {
		TWebf *webf = (TWebf*)((LPCREATESTRUCT(lParam))->lpCreateParams);
		webf->Close();
		webf->Release();

		//#pragma warning(suppress:4244)
		//SetWindowLongPtr(hwnd, 0, (LONG_PTR)webf);

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}*/

	if (msg == WM_CREATE) {
		TWebf *webf = (TWebf*)((LPCREATESTRUCT(lParam))->lpCreateParams);
		webf->hWnd = hwnd;

		#pragma warning(suppress:4244)
		SetWindowLongPtr(hwnd, 0, (LONG_PTR)webf);
		CREATESTRUCT *cs = (CREATESTRUCT*)lParam;

		if (cs->style & (WS_HSCROLL | WS_VSCROLL)) {
			SetWindowLongPtr(hwnd, GWL_STYLE, cs->style & ~(WS_HSCROLL | WS_VSCROLL));
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	#pragma warning(suppress:4312)
	TWebf *webf = (TWebf*)GetWindowLongPtr(hwnd, 0);

	if (webf == NULL) {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return webf->InstanceWndProc(msg, wParam, lParam);
}

LRESULT TWebf::InstanceWndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_DESTROY:
			Close();
			Release();
			SetWindowLongPtr(hWnd, 0, 0);
			break;
		case WM_SETTEXT:
			Go((TCHAR*)lParam);
			break;
		case WM_SIZE:
			if (ibrowser != NULL) {
				ibrowser->put_Width(LOWORD(lParam));
				ibrowser->put_Height(HIWORD(lParam));
			}
			break;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));

			FillRect(ps.hdc, &ps.rcPaint, brush);

			DeleteObject(brush);
			EndPaint(hWnd, &ps);

			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void TWebf::create(HWND hWndParent, HINSTANCE hInstance, LONG_PTR id, UINT x, UINT y, bool showScrollbars)
{
	hasScrollbars = showScrollbars;

	WNDCLASSEX wcex = {0};
	if (!GetClassInfoEx(hInstance, WEBFORM_CLASS, &wcex)) {
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)TWebf::WebformWndProc;
		wcex.hInstance = hInstance;
		wcex.lpszClassName = WEBFORM_CLASS;
		wcex.cbWndExtra = sizeof(TWebf*);

		if(!RegisterClassEx(&wcex)) {
			MessageBox(NULL, "Could not auto register the webform", "TWebf::create", MB_OK);

			return;
		}
	}

	hWnd = CreateWindow(
		WEBFORM_CLASS,
		_T("http://tlundberg.com"),
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		x, y, 1300, 300, hWndParent, (HMENU)id, hInstance, (LPVOID)this);

	hhost = hWnd;

	setupOle();

	Go("http://tlundberg.com");
}

HRESULT STDMETHODCALLTYPE TWebf::Invoke(DISPID dispIdMember, REFIID riid,
	LCID lcid, WORD wFlags, DISPPARAMS *Params, VARIANT *pVarResult,
	EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	switch (dispIdMember) { // DWebBrowserEvents2
		case DISPID_BEFORENAVIGATE2:
			break;
		case DISPID_DOCUMENTCOMPLETE:
			DocumentComplete(Params->rgvarg[0].pvarVal->bstrVal);
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

HRESULT STDMETHODCALLTYPE TWebf::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
	pInfo->dwFlags = (hasScrollbars ? 0 : DOCHOSTUIFLAG_SCROLL_NO) | DOCHOSTUIFLAG_NO3DOUTERBORDER;
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TWebf::GetExternal(IDispatch **ppDispatch)
{
	*ppDispatch = static_cast<IDispatch*>(this);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE TWebf::GetWindow(HWND *phwnd)
{
	*phwnd = hhost;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE TWebf::GetWindowContext(IOleInPlaceFrame **ppFrame,
	IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect,
	LPOLEINPLACEFRAMEINFO info)
{
	*ppFrame = static_cast<IOleInPlaceFrame*>(this);
	AddRef();
	*ppDoc = NULL;
	info->fMDIApp = FALSE;
	info->hwndFrame = hhost;
	info->haccel = 0;
	info->cAccelEntries = 0;
	GetClientRect(hhost, lprcPosRect);
	GetClientRect(hhost, lprcClipRect);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE TWebf::OnPosRectChange(LPCRECT lprcPosRect)
{
	IOleInPlaceObject *iole = NULL;
	ibrowser->QueryInterface(IID_IOleInPlaceObject, (void**)&iole);

	if (iole != NULL) {
		iole->SetObjectRects(lprcPosRect, lprcPosRect);
		iole->Release();
	}

	return S_OK;
}
