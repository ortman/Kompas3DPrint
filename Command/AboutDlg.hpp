#ifndef _Kompas3DPrint_AboutDlg_hpp_
#define _Kompas3DPrint_AboutDlg_hpp_

#include <CtrlLib/CtrlLib.h>
#define LAYOUTFILE <Kompas3DPrint/Layouts.lay>
#include <CtrlCore/lay.h>

class AboutDlg : public WithAboutLay<TopWindow> {
public:
	AboutDlg() {
		CtrlLayout(*this, t_("About"));
	}
};

#endif
