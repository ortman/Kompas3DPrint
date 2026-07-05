#ifndef _Kompas3DPrint_Settings_hpp_
#define _Kompas3DPrint_Settings_hpp_

#include <CtrlLib/CtrlLib.h>
#include "../Kompas/Kompas3D.h"
#include "../Resources.h"

class Settings : public WithSettingsLay<TopWindow> {
private:
	Doc3D::ExportParams params;
	
public:
	Settings() {
		CtrlLayout(*this, t_("Settings"));
	}
	
	const Doc3D::ExportParams& GetExportParams() {
		return params;
	}
	
	bool Load() {
		std::string dir = Kompas3D::ConfigPath();
		if (dir.empty()) return false;
		VectorMap<String, String> ini = LoadIniFile((dir + "/Kompas3DPrint.ini").c_str());
		if (ini.GetCount() == 0) return false;
		params.format     = GetIniFmt( ini, SETTINGS_INI_FORMAT,        Doc3D::Format(Doc3D::Format::STL));
		params.objBody    = GetIniBool(ini, SETTINGS_INI_OBJ_BODY,      true);
		params.objSurface = GetIniBool(ini, SETTINGS_INI_OBJ_SURFACE,   false);
		params.units      = GetIniUnit(ini, SETTINGS_INI_UNITS,         Doc3D::MM);
		params.formatBIN  = GetIniBool(ini, SETTINGS_INI_FORMAT_BIN,    true);
		params.isLinear   = GetIniBool(ini, SETTINGS_INI_IS_LINEAR,     true);
		params.linearVal  = GetIniDbl( ini, SETTINGS_INI_LINEAR_VAL,    0.001);
		params.isAngle    = GetIniBool(ini, SETTINGS_INI_IS_ANGLE,      false);
		params.angleVal   = GetIniDbl( ini, SETTINGS_INI_ANGLE_VAL,     7.2);
		params.isRidge    = GetIniBool(ini, SETTINGS_INI_IS_RIDGE,      false);
		params.ridgeVal   = GetIniDbl( ini, SETTINGS_INI_RIDGE_VAL,     1);
		
		bool autoExportEn = GetIniBool(ini, SETTINGS_INI_AUTOEXPORT_EN, true);
		bool createFolder = GetIniBool(ini, SETTINGS_INI_CREATE_FOLDER, false);
		bool autoExportWE = GetIniBool(ini, SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS, false);
		String slicerPath = GetIniStr( ini, SETTINGS_INI_CURA_PATH,     "");
		return true;
	}
	
	bool Save() {
		std::string dir = Kompas3D::ConfigPath();
		if (dir.empty()) return false;
		FileOut ini((dir + "/Kompas3DPrint.ini").c_str());
		SetIniStr( ini, SETTINGS_INI_FORMAT,      params.format.Name());
		SetIniBool(ini, SETTINGS_INI_OBJ_BODY,    params.objBody);
		SetIniBool(ini, SETTINGS_INI_OBJ_SURFACE, params.objSurface);
		SetIniUnit(ini, SETTINGS_INI_UNITS,       params.units);
		SetIniBool(ini, SETTINGS_INI_FORMAT_BIN,  params.formatBIN);
		SetIniBool(ini, SETTINGS_INI_IS_LINEAR,   params.isLinear);
		SetIniDbl( ini, SETTINGS_INI_LINEAR_VAL,  params.linearVal);
		SetIniBool(ini, SETTINGS_INI_IS_ANGLE,    params.isAngle);
		SetIniDbl( ini, SETTINGS_INI_ANGLE_VAL,   params.angleVal);
		SetIniBool(ini, SETTINGS_INI_IS_RIDGE,    params.isRidge);
		SetIniDbl( ini, SETTINGS_INI_RIDGE_VAL,   params.ridgeVal);
		
		SetIniBool(ini, SETTINGS_INI_AUTOEXPORT_EN, true);
		SetIniBool(ini, SETTINGS_INI_CREATE_FOLDER, false);
		SetIniBool(ini, SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS, false);
		SetIniStr( ini, SETTINGS_INI_CURA_PATH,   "");
		ini.Close();
		return false;
	}
	
private:
	bool GetIniBool(VectorMap<String, String>& ini, const String& key, bool def) {
		int i = ini.Find(key);
		if (i < 0) return def;
		return ini[i] == "true" ? true : false;
	}
	
	int GetIniInt(VectorMap<String, String>& ini, const String& key, int def) {
		int i = ini.Find(key);
		if (i < 0) return def;
		return StrInt(ini[i]);
	}
	
	double GetIniDbl(VectorMap<String, String>& ini, const String& key, double def) {
		int i = ini.Find(key);
		if (i < 0) return def;
		return StrDbl(ini[i]);
	}
	
	const String& GetIniStr(VectorMap<String, String>& ini, const String& key, const String& def) {
		int i = ini.Find(key);
		return (i < 0) ? def : ini[i];
	}
	
	Doc3D::Format GetIniFmt(VectorMap<String, String>& ini, const String& key, Doc3D::Format def) {
		int i = ini.Find(key);
		if (i < 0) return def;
		return Doc3D::Format(ini[i].ToStd());
	}
	
	Doc3D::Unit GetIniUnit(VectorMap<String, String>& ini, const String& key, Doc3D::Unit def) {
		int i = ini.Find(key);
		if (i < 0) return def;
		String& txt = ini[i];
		if (txt == "cm") return Doc3D::SM;
		if (txt == "dm") return Doc3D::DM;
		if (txt == "m") return Doc3D::M;
		return Doc3D::MM;
	}
	
	void SetIniStr(FileOut &ini, const String& key, const String& val) {
		ini.Put(key + "=" + val + "\n");
	}
	
	void SetIniBool(FileOut &ini, const String& key, bool val) {
		SetIniStr(ini, key, val ? "true" : "false");
	}
	
	void SetIniDbl(FileOut &ini, const String& key, double val) {
		SetIniStr(ini, key, Format("%0.4f", val));
	}
	
	void SetIniUnit(FileOut &ini, const String& key, Doc3D::Unit val) {
		if (val == Doc3D::SM) SetIniStr(ini, key, "cm");
		else if (val == Doc3D::DM) SetIniStr(ini, key, "dm");
		else if (val == Doc3D::M) SetIniStr(ini, key, "m");
		else SetIniStr(ini, key, "mm");
	}
};

#endif
