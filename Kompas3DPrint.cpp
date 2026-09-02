#include <CtrlLib/CtrlLib.h>
#include "Kompas3DPrint.h"

using namespace Upp;

#include "Command/Slicer.hpp"
#include "Command/AboutDlg.hpp"
#include "Command/Settings.hpp"
//#include "Command/Rack.hpp"
//#include "Command/Gear.hpp"
#include "Command/Export.hpp"
//#include "Command/Threads.hpp"
//#include "Command/StandardParts.hpp"

Slicer slicer;
std::unique_ptr<AboutDlg>    aboutDlg;
std::unique_ptr<Export>      exprt;
std::unique_ptr<Settings>    settings;
//std::unique_ptr<Rack>        rack;
//std::unique_ptr<Gear>        gear;
//std::unique_ptr<Threads>     threads;
//std::unique_ptr<StandardParts> standardParts;

void MainStart() {
	StdLogSetup(LOG_FILE, "C:/smcLog/kompas3dprint.log");
	aboutDlg = std::make_unique<AboutDlg>();
	exprt    = std::make_unique<Export>();
	settings = std::make_unique<Settings>(exprt->GetTypes());
	//rack     = std::make_unique<Rack>();
	//gear     = std::make_unique<Gear>();
	//threads  = std::make_unique<Threads>();
	//standardParts = std::make_unique<StandardParts>();
}

void Export(Doc3D::Format format) {
	settings->Load();
	Doc3D::ExportParams params = settings->GetExportParams();
	params.format = format;
	if (exprt->SaveAs(params).IsEmpty()) {
		Kompas3D::Error("Ошибка экспорта");
	} else {
		Kompas3D::Error("Успешно экспортирован");
	}
}

void Kompas3D::RunCommand(uint32_t comm) {
	switch (comm) {
		case MENU_SETTINGS:    settings->Load(); settings->Open(); break;
		case MENU_OPEN_SLICER: {
			settings->Load();
			Doc3D::ExportParams params = settings->GetExportParams();
			params.format = settings->GetSlicerFormat();
			String exportPath = exprt->SaveAs(params, true);
			if (exportPath.IsEmpty()) {
				Kompas3D::Error("Ошибка экспорта");
			} else {
				slicer.Open(exportPath);
			}
			break;
		}
		case MENU_EXPORT_STL:  Export(Doc3D::Format::STL); break;
		case MENU_EXPORT_STEP: Export(Doc3D::Format::STEP); break;
		case MENU_EXPORT_IGS:  Export(Doc3D::Format::IGES); break;
		case MENU_EXPORT_X_T:  Export(Doc3D::Format::PARASOLID); break;
		case MENU_EXPORT_ACIS: Export(Doc3D::Format::ACIS); break;
		case MENU_EXPORT_VRLM: Export(Doc3D::Format::VRLM); break;
		case MENU_ABOUT:       aboutDlg->Open(); break;
		//case MENU_RACK:        rack->Start(); break;
		//case MENU_GEAR:        gear->Start(); break;
		//case MENU_THREADS:     threads->Start(); break;
		//case MENU_STANDARD:    standardParts->Start(); break;
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
		bSettings   << [=]() { Kompas3D::RunCommand(MENU_SETTINGS); };
		bOpenSlicer << [=]() { Kompas3D::RunCommand(MENU_OPEN_SLICER); };
		bExportSTL  << [=]() { Kompas3D::RunCommand(MENU_EXPORT_STL); };
		bExportSTEP << [=]() { Kompas3D::RunCommand(MENU_EXPORT_STEP); };
		bExportIGS  << [=]() { Kompas3D::RunCommand(MENU_EXPORT_IGS); };
		bExportX_T  << [=]() { Kompas3D::RunCommand(MENU_EXPORT_X_T); };
		bExportACIS << [=]() { Kompas3D::RunCommand(MENU_EXPORT_ACIS); };
		bExportVRLM << [=]() { Kompas3D::RunCommand(MENU_EXPORT_VRLM); };
		bAbout      << [=]() { Kompas3D::RunCommand(MENU_ABOUT); };
		bGear       << [=]() { Kompas3D::RunCommand(MENU_GEAR); };
		bRack       << [=]() { Kompas3D::RunCommand(MENU_RACK); };
		bThreads    << [=]() { Kompas3D::RunCommand(MENU_THREADS); };
		bStRun      << [=]() { Kompas3D::RunCommand(MENU_STANDARD); };
	}
};

GUI_APP_MAIN {
	if (Kompas3D::ComConnect()) {
		MainStart();
		App().Run();
	} else {
		ErrorOK("Kompas3D не запущен! Перерапустите приложение.");
	}
}