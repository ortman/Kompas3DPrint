#ifndef _KompasAPI_Panel_h_
#define _KompasAPI_Panel_h_

#include <vector>
#include <string>
#include <variant>
#include "KompasEvent.h"

class PropertyManagerNotifyLoc;
class Panel {
public:
	class Property;
	class Tab;
	
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
	Panel& Show(bool isShow = true);
	Panel& Hide() { return Show(false); }
};

class Panel::Tab {
private:
	IUnknown* pTab;
	std::string name;
	std::vector<Panel::Property*> props;
	
	static Tab* currentTab;
	
public:
	Tab(const char* name) : name(name) {
		if (Panel::currentPanel) {
			Panel::currentPanel->tabs.push_back(this);
		}
		currentTab = this;
	}
	~Tab();
	friend class Panel;
};

using PropertyVariant = std::variant<int, double, std::string>;

class Panel::Property {
private:
	IUnknown* pProp;
	std::string name;
	PropertyVariant defaultVal;

public:
	Property(const char* name, PropertyVariant val) : name(name), defaultVal(val) {
		if (Panel::Tab::currentTab) {
			Tab::currentTab->props.push_back(this);
		}
	}
	~Property();
	template <typename T>
	operator T() const;
	template <typename T>
	T operator=(T);
	friend class Panel;
};

#endif
