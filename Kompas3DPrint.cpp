#include <CtrlLib/CtrlLib.h>
#include "Kompas3DPrint.h"

using namespace Upp;
#define LAYOUTFILE <Kompas3DPrint/Layouts.lay>
#include <CtrlCore/lay.h>

class AboutDlg : public WithAboutLay<TopWindow> {
public:
	AboutDlg() {
		CtrlLayout(*this, t_("About"));
		bOk << [=]() {
			PromptOK("Кнопка нажата!");
		};
	}
};

AboutDlg* aboutDlg = nullptr;

int DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		AppInitEnvironment__();
		GUI_APP_MAIN_HOOK
		UPP::Ctrl::InitWin32(hInstance);
	} else if (dwReason == DLL_PROCESS_DETACH) {
		UPP::Ctrl::CloseTopCtrls();
		UPP::Ctrl::ExitWin32();
		UPP::Ctrl::ShutdownThreads();
	}
	return 1;
}

int LIBRARYID() {
    return IDR_LIBID;
}

void LIBRARYENTRY(unsigned int comm) {
	if (!aboutDlg) {
		aboutDlg = new AboutDlg();
		aboutDlg->Run(true);
	}
}

bool LibInterfaceNotifyEntry(IDispatch *application) {
	return false;
}