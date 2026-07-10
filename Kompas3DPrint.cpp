#include <CtrlLib/CtrlLib.h>
#include "Kompas3DPrint.h"

using namespace Upp;

#include "Command/AboutDlg.hpp"
#include "Command/Settings.hpp"
#include "Command/Rack.hpp"
#include "Command/Gear.hpp"
#include "Command/Export.hpp"
#include "Command/Threads.hpp"

std::unique_ptr<AboutDlg>    aboutDlg;
std::unique_ptr<Export>      exprt;
std::unique_ptr<Settings>    settings;
std::unique_ptr<Rack>        rack;
std::unique_ptr<Gear>        gear;
std::unique_ptr<Threads>     threads;

void MainStart() {
	aboutDlg = std::make_unique<AboutDlg>();
	exprt    = std::make_unique<Export>();
	settings = std::make_unique<Settings>(exprt->GetTypes());
	settings->Load();
	rack     = std::make_unique<Rack>();
	gear     = std::make_unique<Gear>();
	threads  = std::make_unique<Threads>();
}

void Export(Doc3D::Format format) {
	Doc3D::ExportParams params = settings->GetExportParams();
	params.format = format;
	exprt->SaveAs(params);
}

void LIBRARYENTRY(unsigned int comm) {
	switch (comm) {
		case MENU_SETTINGS:    settings->Open(); break;
		case MENU_OPEN_SLICER: {
			Doc3D::ExportParams params = settings->GetExportParams();
			params.format = settings->GetSlicerFormat();
			exprt->Slicer(params, settings->GetSlicerPath());
			break;
		}
		case MENU_EXPORT_STL:  Export(Doc3D::Format::STL); break;
		case MENU_EXPORT_STEP: Export(Doc3D::Format::STEP); break;
		case MENU_EXPORT_IGS:  Export(Doc3D::Format::IGES); break;
		case MENU_EXPORT_X_T:  Export(Doc3D::Format::PARASOLID); break;
		case MENU_EXPORT_ACIS: Export(Doc3D::Format::ACIS); break;
		case MENU_EXPORT_VRLM: Export(Doc3D::Format::VRLM); break;
		case MENU_ABOUT:       aboutDlg->Open(); break;
		case MENU_RACK:        rack->Start(); break;
		case MENU_GEAR:        gear->Start(); break;
		case MENU_THREADS:     threads->Start(); break;
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
		if (!Kompas3D::Connect()) {
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
		bThreads    << [=]() { LIBRARYENTRY(MENU_THREADS); };
	}
};

GUI_APP_MAIN {
	if (Kompas3D::Connect()) MainStart();
	App().Run();
}