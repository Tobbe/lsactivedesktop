#include "toleclientsite.h"
#include "webform.h"

HRESULT STDMETHODCALLTYPE TOleClientSite::QueryInterface(REFIID riid, void **ppv)
{
	return webf->QueryInterface(riid, ppv);
}

ULONG STDMETHODCALLTYPE TOleClientSite::AddRef()
{
	return webf->AddRef();
}

ULONG STDMETHODCALLTYPE TOleClientSite::Release()
{
	return webf->Release();
}

HRESULT STDMETHODCALLTYPE TOleClientSite::SaveObject()
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE TOleClientSite::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE TOleClientSite::GetContainer(IOleContainer **ppContainer)
{
	*ppContainer = 0;
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE TOleClientSite::ShowObject()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE TOleClientSite::OnShowWindow(BOOL fShow)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE TOleClientSite::RequestNewObjectLayout()
{
	return E_NOTIMPL;
}
