#ifndef _KompasAPI_Panel_h_
#define _KompasAPI_Panel_h_

#include <vector>
#include <string>
#include <variant>
#include "KompasEvent.h"

class PropertyManagerNotifyLoc;
class Panel {
public:
	class Tab;
	class Property;
	
private:
	IUnknown* pManager = nullptr;
	std::string name;
	std::vector<Panel::Tab*> tabs;
	PropertyManagerNotifyLoc *comEvent = nullptr;
	
	static Panel* currentPanel;
	
public:
	
	KompasEvent<bool(int buttonId)> WhenButtonClick;
	
	Panel(const char* name) : name(name), pManager(nullptr) {
		currentPanel = this;
	}
	~Panel();
	bool Create();
	Panel& Update();
	Panel& Show(bool isShow = true);
	Panel& Hide() { return Show(false); }
	friend PropertyManagerNotifyLoc;
};

class Panel::Tab {
private:
	IUnknown* pTab;
	std::string name;
	
public:
	static Tab* currentTab;
	std::vector<Panel::Property*> props;

	Tab(const char* name) : name(name) {
		if (Panel::currentPanel) {
			Panel::currentPanel->tabs.push_back(this);
		}
		currentTab = this;
	}
	~Tab();
	Tab& Clear();
	Tab& Add(Panel::Property& prop);
	friend class Panel;
	friend PropertyManagerNotifyLoc;
};

using PropertyVariant = std::variant<int, double, std::string>;

class Panel::Property {
protected:
	IUnknown* pProp;
	std::string name;
	PropertyVariant defaultVal;
	int type;
	static int nextId;

	Property(const char* name, int type, PropertyVariant val) : name(name), type(type), defaultVal(val) {
		if (Panel::Tab::currentTab) Panel::Tab::currentTab->props.push_back(this);
	}
	
	virtual void Create(IUnknown* pControls);
	int GetId();
	
public:
	KompasEvent<void(void)> WhenChange;
	
	virtual ~Property();
	void SetName(const std::string& name);

	friend class Panel;
	friend PropertyManagerNotifyLoc;
};

class PropertyI : public Panel::Property {
public:
	PropertyI(const char* name, int val) : Property(name, 2, val) {}
	operator int() const;
	int operator=(int);
};

class PropertyD : public Panel::Property {
public:
	PropertyD(const char* name, double val) : Property(name, 3, val) {}
	operator double() const;
	double operator=(double);
};

class PropertyList : public Panel::Property {
public:
	PropertyList(const char* name) : Property(name, 7, "") {}
	PropertyList& Add(PropertyVariant val);
	PropertyList& Clear();
	operator PropertyVariant() const;
	PropertyVariant operator=(PropertyVariant);
};

class PropertyButton : public Panel::Property {
public:
	KompasEvent<void(void)> WhenClick;
	PropertyButton(const char* name) : Property(name, 13, "") {}
	virtual void Create(IUnknown* pControls) override;
};


#endif
