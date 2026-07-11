#include "ComKompas.h"
#include "Panel.h"
#include "Node.h"
#include "Kompas3D.h"

class PropertyManagerNotifyLoc : public ComEvent {
private:
	Panel* panel;
public:
	PropertyManagerNotifyLoc(Panel* p) : ComEvent(K7::DIID_ksPropertyManagerNotify), panel(p) {}
	
    STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                        DISPPARAMS* pDispParams, VARIANT* pVarResult,
                        EXCEPINFO* pExcepInfo, UINT* puArgErr) override {
		switch((int)dispIdMember) {
			case KConst::prButtonClick: {
				VARIANT& buttonId = pDispParams->rgvarg[pDispParams->cArgs - 1];
				if (panel && buttonId.vt == VT_I4) {
					VariantInit(pVarResult);
					pVarResult->vt = VT_BOOL;
					pVarResult->boolVal = panel->WhenButtonClick(buttonId.lVal);
				}
				break;
			}
		}
        return S_OK;
    }
};

// Panel
Panel::~Panel() {
    if (comEvent) {
		comEvent->Unsubscribe(pManager);
		delete comEvent;
    }
	if (pManager) pManager->Release();
	currentPanel = nullptr;
}

bool Panel::Create() {
	try {
		pManager = Kompas3D::CreatePropertyManager();
		K7::IPropertyManagerPtr manager(pManager);
		manager->Caption = Node::Utf8ToCp1251(name).c_str();
		manager->SpecToolbar = KConst::pnEnterEscHelp;
		comEvent = new PropertyManagerNotifyLoc(this);
		comEvent->Subscribe(pManager);
		K7::IPropertyTabsPtr pTabs = manager->PropertyTabs;
		if (!pTabs) throw Kompas3DException("Can not get PropertyTabs of PropertyManager");
		for (Tab* t : tabs) {
			K7::IPropertyTabPtr pTab = pTabs->Add(Node::Utf8ToCp1251(t->name).c_str());
			if (pTab) {
				pTab.AddRef();
				t->pTab = pTab.GetInterfacePtr();
				K7::IPropertyControlsPtr ctrls = pTab->PropertyControls;
				if (!ctrls) throw Kompas3DException("Can not get PropertyControls of Tab");
				for (Property* p : t->props) {
					K7::IPropertyControlPtr c = ctrls->Add(KConst::ksControlEditReal);
					if (c) {
						c.AddRef();
						c->Name = Node::Utf8ToCp1251(p->name).c_str();
						c->Value = p->defaultVal;
						p->pProp = c.GetInterfacePtr();
					}
				}
			}
		}
	} catch (const Kompas3DException&) {
		return false;
	}
	return true;
}

Panel& Panel::Show(bool isShow) {
	K7::IPropertyManagerPtr manager(pManager);
	if (!manager) throw Kompas3DException("Can not get PropertyManager");
	if (isShow) {
		manager->ShowTabs();
	} else {
		manager->HideTabs();
	}
	return *this;
}

Panel* Panel::currentPanel = nullptr;

// Tab
Panel::Tab::~Tab() {
	if (pTab) pTab->Release();
	currentTab = nullptr;
}

Panel::Tab* Panel::Tab::currentTab = nullptr;

// Property
Panel::Property::~Property() {
	if (pProp) pProp->Release();
}

Panel::Property::operator double() const {
	K7::IPropertyControlPtr prop(pProp);
	return prop->Value;
}

double Panel::Property::operator=(double val) {
	K7::IPropertyControlPtr prop(pProp);
	prop->Value = val;
	return val;
}