#pragma once
#include <windows.h>

#ifdef _MSC_VER
	#import "ksConstants.tlb" rename_namespace("KConst") no_implementation
	#import "ksConstants3D.tlb" rename_namespace("KConst3D") no_implementation
	#import "kAPI5.tlb" named_guids rename_namespace("K5") no_implementation
	#import "kAPI7.tlb" named_guids rename_namespace("K7") no_implementation
#else
	#include <ksConstants.tlh>
	#include <ksConstants3D.tlh>
	#include <kAPI5.tlh>
	#include <kAPI7.tlh>
#endif

class ComEvent : public IDispatch {
protected:
    ULONG m_refCount = 1;
    REFIID iid;
	DWORD cookie = 0;

public:
	ComEvent(REFIID riid) : iid(riid) {}
	
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IDispatch || riid == iid) {
            *ppvObject = static_cast<IDispatch*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    STDMETHODIMP_(ULONG) AddRef() override { return InterlockedIncrement(&m_refCount); }
    STDMETHODIMP_(ULONG) Release() override {
        ULONG res = InterlockedDecrement(&m_refCount);
        if (res == 0) delete this;
        return res;
    }

    // IDispatch (для событий заглушки не вызываются, важен только Invoke)
    STDMETHODIMP GetTypeInfoCount(UINT* pctinfo) override { return E_NOTIMPL; }
    STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override { return E_NOTIMPL; }
    STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) override { return E_NOTIMPL; }
    
	HRESULT Subscribe(IUnknown* obj) {
	    if (!obj) return E_POINTER;
	
	    IConnectionPointContainer* pCPC = nullptr;
	    IConnectionPoint* pCP = nullptr;
	
	    // 1. Запрашиваем контейнер точек подключения у объекта КОМПАС
	    HRESULT hr = obj->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
	    if (FAILED(hr)) return hr;
	
	    // 2. Ищем точку именно под ksKompasObjectNotify
	    hr = pCPC->FindConnectionPoint(iid, &pCP);
	    pCPC->Release();
	    if (FAILED(hr)) return hr;
	
	    // 3. Передаем наш Sink и получаем токен отписки (Cookie)
	    hr = pCP->Advise(static_cast<IUnknown*>(this), &cookie);
	    pCP->Release();
	
	    return hr;
	}
	
	HRESULT Unsubscribe(IUnknown* obj) {
	    IConnectionPointContainer* pCPC = nullptr;
	    IConnectionPoint* pCP = nullptr;
	
	    HRESULT hr = obj->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
	    if (FAILED(hr)) return hr;
	
	    hr = pCPC->FindConnectionPoint(iid, &pCP);
	    pCPC->Release();
	    if (FAILED(hr)) return hr;
	
	    // Разрываем соединение
	    hr = pCP->Unadvise(cookie);
	    pCP->Release();
	
	    return hr;
	}
};

#include <string>

std::string GetClassName(IUnknown* pUnk);
