#ifndef _KompasAPI_Panel_h_
#define _KompasAPI_Panel_h_

#include <vector>
#include <string>
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
	
	Panel(const std::string& name) : name(name), pManager(nullptr) {
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
	Tab(const std::string& name) : name(name) {
		if (Panel::currentPanel) {
			Panel::currentPanel->tabs.push_back(this);
		}
		currentTab = this;
	}
	~Tab();
	friend class Panel;
};

class Panel::Property {
private:
	IUnknown* pProp;
	std::string name;
	double defaultVal = 0.0;

public:
	Property(const std::string& name, double val = 0.0) : name(name), defaultVal(val) {
		if (Panel::Tab::currentTab) {
			Tab::currentTab->props.push_back(this);
		}
	}
	~Property();
	operator double() const;
	double operator=(double);
	friend class Panel;
};

#endif
