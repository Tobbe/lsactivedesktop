#ifndef _WEBFORM_H__
#define _WEBFORM_H__

#include <mshtml.h>
#include <exdisp.h>
#include <windows.h>
#include <mshtmhst.h>
#include <mshtmdid.h>
#include <exdispid.h>
#include <tchar.h>
#include <list>
#include "toleclientsite.h"
#include "toleinplacesite.h"
#include "toleinplaceframe.h"
#include "tdochostuihandler.h"
#include "tdochostshowui.h"
#include "tdispatch.h"

typedef std::basic_string<TCHAR> tstring;


#define WEBFORM_CLASS (_T("WebformClass"))
// Create a Webfrom control with CreateWindow(WEBFORM_CLASS,_T("initial-url"),...)
// If you specify WS_VSCROLL style then the webform will be created with scrollbars.

#define WEBFN_LOADED 3
// This notification is sent via WM_COMMAND when you have called WebformGo(hWebF, url).
// It indicates that the page has finished loading.

class TWebf : public IUnknown {
public:
	long ref;
	TOleClientSite clientsite;
	TOleInPlaceSite site;
	TOleInPlaceFrame frame;
	TDocHostUIHandler uihandler;
	TDocHostShowUI showui;
	TDispatch dispatch;
	unsigned int isnaving;    // bitmask: 4=haven't yet finished Navigate call, 2=haven't yet received DocumentComplete, 1=haven't yet received BeforeNavigate

	HWND hhost;               // This is the window that hosts us
	IWebBrowser2 *ibrowser;   // Our pointer to the browser itself. Released in Close().
	DWORD cookie;             // By this cookie shall the watcher be known

	bool hasScrollbars;       // This is read from WS_VSCROLL|WS_HSCROLL at WM_CREATE
	TCHAR *url;               // This was the url that the user just clicked on
	TCHAR *kurl;              // Key\0Value\0Key2\0Value2\0\0 arguments for the url just clicked on
	HANDLE hf;

	TWebf(HWND hhost);
	~TWebf();
	static HWND create(HWND hWndParent, HINSTANCE hInstance);
	void CloseThread();
	void Close();
	void Go(const TCHAR *fn);

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	void BeforeNavigate2(const wchar_t *url, short *cancel);
	void DocumentComplete(const wchar_t *url); 
};

#endif
