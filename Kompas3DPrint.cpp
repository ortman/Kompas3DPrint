#include <CtrlLib/CtrlLib.h>
#include "Kompas3DPrint.h"

using namespace Upp;
#include "Command/AboutDlg.hpp"
#include "Command/Settings.hpp"
#include "Command/Rack.hpp"
#include "Command/Gear.hpp"
#include "Command/Export.hpp"

std::unique_ptr<AboutDlg>    aboutDlg;
std::unique_ptr<Settings>    settings;
std::unique_ptr<Rack>        rack;
std::unique_ptr<Gear>        gear;
std::unique_ptr<Export>      exprt;


void MainStart() {
	aboutDlg = std::make_unique<AboutDlg>();
	settings = std::make_unique<Settings>();
	rack = std::make_unique<Rack>();
	gear = std::make_unique<Gear>();
	exprt = std::make_unique<Export>();
}

void LIBRARYENTRY(unsigned int comm) {
	switch (comm) {
		case MENU_SETTINGS:    settings->Open(); break;
		case MENU_OPEN_SLICER: exprt->Slicer(); break;
		case MENU_EXPORT_STL:
			exprt->SaveAs(settings->GetExportParams(Doc3D::Format::STL));
			break;
		case MENU_EXPORT_STEP:
			exprt->SaveAs(settings->GetExportParams(Doc3D::Format::STEP));
			break;
		case MENU_EXPORT_IGS:
			exprt->SaveAs(settings->GetExportParams(Doc3D::Format::IGES));
			break;
		case MENU_EXPORT_X_T:
			exprt->SaveAs(settings->GetExportParams(Doc3D::Format::XT));
			break;
		case MENU_EXPORT_ACIS:
			exprt->SaveAs(settings->GetExportParams(Doc3D::Format::ACIS));
			break;
		case MENU_EXPORT_VRLM:
			exprt->SaveAs(settings->GetExportParams(Doc3D::Format::VRLM));
			break;
		case MENU_ABOUT:       aboutDlg->Open(); break;
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
		if (!Kompas3D::Connect(false)) {
			DisableCtrls({
				&bSettings, &bAbout, &bOpenSlicer, &bGear, &bRack,
				&bExportSTL, &bExportSTEP, &bExportIGS, &bExportX_T, &bExportACIS, &bExportVRLM
			});
			ErrorOK("Kompas3D не запущен! Перерапустите приложение.");
			return;
		}
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
	if (Kompas3D::Connect(false)) MainStart();
	App().Run();
}