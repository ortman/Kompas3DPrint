#ifndef _KompasAPI_Panel_h_
#define _KompasAPI_Panel_h_

#include <vector>
#include <string>
#include <memory>
#include "KompasEvent.h"

class PropertyManagerNotifyLoc;
class Panel {
public:
	class Property;
	class Tab;
	
private:
	IUnknown* pManager;
	std::vector<std::unique_ptr<Tab>> tabs;
	PropertyManagerNotifyLoc *comEvent = nullptr;
	
public:
	KompasEvent<bool(int buttonId)> WhenButtonClick;
	
	Panel(IUnknown* pManager, const std::string& name);
	~Panel();
	Tab& CreateTab(const std::string& name);
	Panel& Show(bool isShow = true);
	Panel& Hide() { return Show(false); }
};

class Panel::Property {
private:
	IUnknown* pProp;
public:
	Property(IUnknown* pProp, const std::string& name, double val = 0.0);
	~Property();
	operator double() const;
};

class Panel::Tab {
private:
	IUnknown* pTab;
	std::vector<std::unique_ptr<Property>> props;
public:
	Tab(IUnknown* pTab, const std::string& name);
	~Tab();
	Property& CreateProperty(const std::string& name, double val = 0.0);
};

#endif
