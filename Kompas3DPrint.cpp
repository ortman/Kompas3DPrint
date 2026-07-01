#include <CtrlLib/CtrlLib.h>
#include "Kompas3DPrint.h"

using namespace Upp;
#include "Command/AboutDlg.hpp"
#include "Command/SettingsDlg.hpp"
#include "Command/Rack.hpp"
#include "Command/Gear.hpp"
#include "Command/Export.hpp"


std::unique_ptr<AboutDlg>    aboutDlg;
std::unique_ptr<SettingsDlg> settingsDlg;
std::unique_ptr<Rack>        rack;
std::unique_ptr<Gear>        gear;
std::unique_ptr<Export>      exportTo;

void MainStart() {
	aboutDlg = std::make_unique<AboutDlg>();
	settingsDlg = std::make_unique<SettingsDlg>();
	rack = std::make_unique<Rack>();
	gear = std::make_unique<Gear>();
	exportTo = std::make_unique<Export>();
}

void LIBRARYENTRY(unsigned int comm) {
	switch (comm) {
		case MENU_SETTINGS:    settingsDlg->Run(); break;
		case MENU_OPEN_SLICER: exportTo->Slicer(); break;
		case MENU_EXPORT_STL:  exportTo->STL(); break;
		case MENU_EXPORT_STEP: exportTo->STEP(); break;
		case MENU_EXPORT_IGS:  exportTo->IGES(); break;
		case MENU_EXPORT_X_T:  exportTo->XT(); break;
		case MENU_EXPORT_ACIS: exportTo->SAT(); break;
		case MENU_EXPORT_VRLM: exportTo->VRLM(); break;
		case MENU_ABOUT:       aboutDlg->Run(); break;
		case MENU_RACK:        rack->Start(); break;
		case MENU_GEAR:        gear->Start(); break;
	}
}

int DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		AppInitEnvironment__();
		GUI_APP_MAIN_HOOK
		UPP::Ctrl::InitWin32(hInstance);
		MainStart();
	} else if (dwReason == DLL_PROCESS_DETACH) {
		UPP::Ctrl::CloseTopCtrls();
		UPP::Ctrl::ExitWin32();
		UPP::Ctrl::ShutdownThreads();
	}
	return 1;
}

class App : public WithAppLay<TopWindow> {
public:
	App() {
		CtrlLayout(*this, LIB_NAME);
		bSettings   << [=]() { LIBRARYENTRY(MENU_SETTINGS); };
		bOpenSlicer << [=]() { LIBRARYENTRY(MENU_OPEN_SLICER); };
		bExportSTL  << [=]() { LIBRARYENTRY(MENU_EXPORT_STL); };
		bExportSTEP << [=]() { LIBRARYENTRY(MENU_EXPORT_STEP); };
		bExportIGS  << [=]() { LIBRARYENTRY(MENU_EXPORT_IGS); };
		bExportX_T  << [=]() { LIBRARYENTRY(MENU_EXPORT_X_T); };
		bExportACIS << [=]() { LIBRARYENTRY(MENU_EXPORT_ACIS); };
		bExportVRLM << [=]() { LIBRARYENTRY(MENU_EXPORT_VRLM); };
		bAbout      << [=]() { LIBRARYENTRY(MENU_ABOUT); };
		bGear       << [=]() { LIBRARYENTRY(MENU_GEAR); };
		bRack       << [=]() { LIBRARYENTRY(MENU_RACK); };
	}
};

GUI_APP_MAIN {
	MainStart();
	App().Run();
}