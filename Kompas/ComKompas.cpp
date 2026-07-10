#include "ComKompas.h"

#import "ksConstants.tlb" rename_namespace("KConst") implementation_only
#import "ksConstants3D.tlb" rename_namespace("KConst3D") implementation_only
#import "kAPI5.tlb" named_guids rename_namespace("K5") implementation_only
#import "kAPI7.tlb" named_guids rename_namespace("K7") implementation_only

std::string GetClassName(IUnknown* pUnk) {
    if (!pUnk) return "Null pointer";

    // Запрашиваем IDispatch, который есть у 99% объектов Kompas API
    IDispatch* pDispatch = nullptr;
    if (SUCCEEDED(pUnk->QueryInterface(IID_IDispatch, (void**)&pDispatch))) {
        ITypeInfo* pTypeInfo = nullptr;
        // Запрашиваем информацию о типе напрямую у диспетчера
        if (SUCCEEDED(pDispatch->GetTypeInfo(0, LOCALE_USER_DEFAULT, &pTypeInfo))) {
            BSTR bstrName = nullptr;
            if (SUCCEEDED(pTypeInfo->GetDocumentation(MEMBERID_NIL, &bstrName, nullptr, nullptr, nullptr))) {
                std::string className = (const char*)_bstr_t(bstrName);
                SysFreeString(bstrName);
                pTypeInfo->Release();
                pDispatch->Release();
                return className; // Вернет реальное имя интерфейса, например "IPart3D" или "IDocument"
            }
            pTypeInfo->Release();
        }
        pDispatch->Release();
    }
    return "Unknown Kompas Object";
}