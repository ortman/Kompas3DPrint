#include "ComKompas.h"
#include "Kompas3D.h"
#include "Doc3D.h"

IUnknown* pKompas = nullptr;
IUnknown* pKompas7 = nullptr;
bool comInit = false;

class KompasObjectNotifyLoc : public ComEvent {
public:
	KompasObjectNotifyLoc() : ComEvent(K5::DIID_ksKompasObjectNotify) {}
	
    STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                        DISPPARAMS* pDispParams, VARIANT* pVarResult,
                        EXCEPINFO* pExcepInfo, UINT* puArgErr) override {
		switch((int)dispIdMember) {
			case KConst::koOpenDocument: {
				VARIANT& varDoc = pDispParams->rgvarg[pDispParams->cArgs - 1];
				VARIANT& varType = pDispParams->rgvarg[pDispParams->cArgs - 2];
				if (varDoc.vt == VT_DISPATCH && varType.vt == VT_I4 && Kompas3D::WhenOpenDocument) {
					VariantInit(pVarResult);
					pVarResult->vt = VT_BOOL;
					Doc3D doc(varDoc.pdispVal);
					pVarResult->boolVal = Kompas3D::WhenOpenDocument(doc, varType.lVal);
				}
				break;
			}
			case KConst::koCreateDocument: {
				VARIANT& varDoc = pDispParams->rgvarg[pDispParams->cArgs - 1];
				VARIANT& varType = pDispParams->rgvarg[pDispParams->cArgs - 2];
				if (varDoc.vt == VT_DISPATCH && varType.vt == VT_I4 && Kompas3D::WhenCreateDocument) {
					VariantInit(pVarResult);
					pVarResult->vt = VT_BOOL;
					Doc3D doc(varDoc.pdispVal);
					pVarResult->boolVal = Kompas3D::WhenCreateDocument(doc, varType.lVal);
				}
				break;
			}
		}
        return S_OK;
    }
};

KompasObjectNotifyLoc kompasNotify;

bool Kompas3D::Connect(bool open, bool visible) {
	if (pKompas && pKompas7) return true;
	HRESULT hr;
	if (!comInit) {
		hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		comInit = SUCCEEDED(hr);
	}
	if (!comInit) return false;
	K5::KompasObjectPtr kompas = nullptr;
	hr = kompas.GetActiveObject(L"KOMPAS.Application.5");
	if (FAILED(hr)) {
		if (open) {
			hr = kompas.CreateInstance(L"KOMPAS.Application.5");
		} else {
			return false;
		}
	}
	if (SUCCEEDED(hr)) {
		kompas->Visible = visible;
		pKompas = kompas.Detach();
		K7::IApplicationPtr kompas7 = nullptr;
		hr = kompas7.GetActiveObject(L"KOMPAS.Application.7");
		if (SUCCEEDED(hr)) {
			pKompas7 = kompas7.Detach();
			kompasNotify.Subscribe(pKompas);
			return true;
		} else {
			pKompas->Release();
		}
	}
	return false;
}

void Kompas3D::Disconnect() {
	if (pKompas) {
		kompasNotify.Unsubscribe(pKompas);
		pKompas->Release();
	}
	if (pKompas7) pKompas7->Release();
	if (comInit) CoUninitialize();
}

Doc3D Kompas3D::GetActiveDocument3D() {
	if (!Connect()) return nullptr;
	K5::KompasObjectPtr kompas(pKompas);
	//kompas->AddRef();
	K5::ksDocument3DPtr doc = kompas->ActiveDocument3D();
	if (doc) {
		return Doc3D(doc.GetInterfacePtr());
	}
	return nullptr;
}

template <typename T>
T Kompas3D::GetParamStruct(int type) {
	if (!Connect()) return nullptr;
	K5::KompasObjectPtr kompas(pKompas);
	return kompas->GetParamStruct(type);
}

template <typename T>
T Kompas3D::ToApi7(IUnknown* k5) {
	if (!Connect()) return nullptr;
	K5::KompasObjectPtr kompas(pKompas);
	return kompas->TransferInterface(k5, KConst::ksAPI7Dual, 0);
}

Panel Kompas3D::CreatePanel(const std::string& name) {
	if (!Connect()) throw Kompas3DException("Kompas not connected");
	K7::IApplicationPtr kompas7(pKompas7);
	if (!pKompas7) throw Kompas3DException("Kompas not connected");
	K7::IPropertyManagerPtr manager = kompas7->CreatePropertyManager(true);
	if (!manager) throw Kompas3DException("Can not create PropertyManager");
	return Panel(manager.GetInterfacePtr(), name);
}

void Kompas3D::Message(const std::string& txt) {
	if (Connect()) {
		K5::KompasObjectPtr kompas(pKompas);
		kompas->ksMessage(Node::Utf8ToCp1251(txt).c_str());
	}
}

void Kompas3D::Error(const std::string& txt) {
	if (Connect()) {
		K5::KompasObjectPtr kompas(pKompas);
		kompas->ksError(Node::Utf8ToCp1251(txt).c_str());
	}
}

std::string Kompas3D::SystemPath(long type) {
	if (!Connect()) return std::string();
	K5::KompasObjectPtr kompas(pKompas);
	return Node::Cp1251ToUtf8(kompas->ksSystemPath(type));
}

std::string Kompas3D::ConfigPath() { return SystemPath(KConst::ksConfigurations); }

KompasEvent<bool(Doc3D&, int)> Kompas3D::WhenCreateDocument;
KompasEvent<bool(Doc3D&, int)> Kompas3D::WhenOpenDocument;

#define KOMPAS_PARAM(p) template K5::p##Ptr Kompas3D::GetParamStruct(int);
KOMPAS_PARAM(ksRectangleParam)
KOMPAS_PARAM(ksRegularPolygonParam)
KOMPAS_PARAM(ksEllipseParam)
KOMPAS_PARAM(ksEllipseArcParam)
KOMPAS_PARAM(ksEllipseArcParam1)
KOMPAS_PARAM(ksUserParam)

#define KOMPAS_API7(p) template K7::p##Ptr Kompas3D::ToApi7(IUnknown*);
KOMPAS_API7(IEmbodimentsManager)
KOMPAS_API7(IThread)