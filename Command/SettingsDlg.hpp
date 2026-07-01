#ifndef _Kompas3DPrint_Settings_hpp_
#define _Kompas3DPrint_Settings_hpp_

class SettingsDlg : public WithSettingsLay<TopWindow> {
public:
	SettingsDlg() {
		CtrlLayout(*this, t_("Settings"));
	}
};

#endif
