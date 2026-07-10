#include "ComKompas.h"
#include "Panel.h"
#include "Node.h"

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
Panel::Panel(IUnknown* m, const std::string& name) : pManager(m) {
	if (pManager) pManager->AddRef();
	K7::IPropertyManagerPtr manager(pManager);
	if (!manager) throw Kompas3DException("Can not get PropertyManager");
	manager->Caption = Node::Utf8ToCp1251(name).c_str();
	manager->SpecToolbar = KConst::pnEnterEscHelp;
	comEvent = new PropertyManagerNotifyLoc(this);
	comEvent->Subscribe(pManager);
}

Panel::~Panel() {
    if (comEvent) {
		comEvent->Unsubscribe(pManager);
		delete comEvent;
    }
	if (pManager) pManager->Release();
}

Panel::Tab& Panel::CreateTab(const std::string& name) {
	K7::IPropertyManagerPtr manager(pManager);
	if (!manager) throw Kompas3DException("Can not get PropertyManager");
	K7::IPropertyTabsPtr pTabs = manager->PropertyTabs;
	if (!pTabs) throw Kompas3DException("Can not get PropertyTabs of PropertyManager");
	K7::IPropertyTabPtr pTab = pTabs->Add(Node::Utf8ToCp1251(name).c_str());
	if (!pTab) throw Kompas3DException("Can not create Tab in PropertyManager");
	tabs.push_back(std::make_unique<Panel::Tab>(pTab.GetInterfacePtr(), name));
	return *tabs.back();
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

// Tab
Panel::Tab::Tab(IUnknown* p, const std::string& name) : pTab(p) {
	if (pTab) pTab->AddRef();
}

Panel::Tab::~Tab() {
	if (pTab) pTab->Release();
}

Panel::Property& Panel::Tab::CreateProperty(const std::string& name, double val) {
	K7::IPropertyTabPtr tab(pTab);
	if (!tab) throw Kompas3DException("Can not get Tab");
	K7::IPropertyControlsPtr ctrls = tab->PropertyControls;
	if (!ctrls) throw Kompas3DException("Can not get PropertyControls of Tab");
	K7::IPropertyControlPtr c = ctrls->Add(KConst::ksControlEditReal);
	if (!c) throw Kompas3DException("Can not create Control of Tab");
	props.push_back(std::make_unique<Panel::Property>(c.GetInterfacePtr(), name, val));
	return *props.back();
}

// Property
Panel::Property::Property(IUnknown* p, const std::string& name, double val) : pProp(p) {
	if (pProp) pProp->AddRef();
	K7::IPropertyControlPtr c(pProp);
	if (!c) throw Kompas3DException("Can not get Control");
	c->Name = Node::Utf8ToCp1251(name).c_str();
	c->Value = val;
}

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