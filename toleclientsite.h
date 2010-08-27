#ifndef _TOLECLIENTSITE_H__
#define _TOLECLIENTSITE_H__

#include <windows.h>
#include <mshtmhst.h>
#include <mshtmdid.h>
#include <exdispid.h>
#include <tchar.h>
#include <list>
#include <string>

class TWebf;

class TOleClientSite : public IOleClientSite {
public:
	TWebf *webf;

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// IOleClientSite
	HRESULT STDMETHODCALLTYPE SaveObject();
	HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk);
	HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer **ppContainer);
	HRESULT STDMETHODCALLTYPE ShowObject();
	HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow);
	HRESULT STDMETHODCALLTYPE RequestNewObjectLayout();
};

#endif
