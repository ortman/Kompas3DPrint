#include "ComKompas.h"
#include "Panel.h"
#include "Node.h"
#include "Kompas3D.h"

_variant_t ToVariantT(const PropertyVariant& boxVar) {
    return std::visit([](const auto& arg) -> _variant_t {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>) {
            return _variant_t(arg.c_str()); // Создаст VARIANT с типом VT_BSTR
        } else {
            return _variant_t(arg); // Для int (VT_I4), double (VT_R8) и т.д.
        }
    }, boxVar);
}

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
			case KConst::prControlCommand: {
				VARIANT& buttonId = pDispParams->rgvarg[pDispParams->cArgs - 2];
				if (panel && buttonId.vt == VT_I4) {
					for (Panel::Tab* t : panel->tabs) {
						for (Panel::Property* p : t->props) {
							if (p->GetId() == buttonId.lVal) {
								PropertyButton* b = dynamic_cast<PropertyButton*>(p);
								if (b) b->WhenClick();
							}
						}
					}
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
					K7::IPropertyControlPtr c = ctrls->Add((KConst::ControlTypeEnum)p->type);
					if (c) p->Create(c.GetInterfacePtr());
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

void Panel::Property::Create(IUnknown* pControls) {
	pProp = pControls;
	pProp->AddRef();
	K7::IPropertyControlPtr c = pProp;
	c->Name = Node::Utf8ToCp1251(name).c_str();
	c->Value = ToVariantT(defaultVal);
	c->Id = ++nextId;
}

int Panel::Property::GetId() {
	K7::IPropertyControlPtr c = pProp;
	return c->Id;
}

void Panel::Property::SetName(const std::string& name) {
	K7::IPropertyControlPtr c = pProp;
	c->Name = Node::Utf8ToCp1251(name).c_str();
}

int Panel::Property::nextId = 1;

PropertyList& PropertyList::Add(PropertyVariant val) {
	K7::IPropertyListPtr prop(pProp);
	prop->Add(ToVariantT(val));
	return *this;
}

PropertyList& PropertyList::Clear() {
	K7::IPropertyListPtr prop(pProp);
	prop->ClearList();
	return *this;
}

double PropertyD::operator=(double val) {
	K7::IPropertyControlPtr prop(pProp);
	prop->Value = val;
	return val;
}

PropertyD::operator double() const {
	K7::IPropertyControlPtr prop(pProp);
	return prop->Value;
}

int PropertyI::operator=(int val) {
	K7::IPropertyControlPtr prop(pProp);
	prop->Value = val;
	return val;
}

PropertyI::operator int() const {
	K7::IPropertyControlPtr prop(pProp);
	return prop->Value;
}

void PropertyButton::Create(IUnknown* pControls) {
	pProp = pControls;
	pProp->AddRef();
	K7::IPropertyControlPtr c = pProp;
	c->Name = Node::Utf8ToCp1251(name).c_str();
	c->Id = ++nextId;
}