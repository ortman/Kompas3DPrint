#ifndef _Kompas3DPrint_Settings_hpp_
#define _Kompas3DPrint_Settings_hpp_

#include <CtrlLib/CtrlLib.h>
#include "../Kompas/Kompas3D.h"
#include "../Resources.h"

#define SETTINGS_INI_BLOCK_MAIN             "Autoexport"
#define SETTINGS_INI_AUTOEXPORT_EN          "AutoexportOn"
#define SETTINGS_INI_FORMAT                 "Format"
#define SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS "AutoexportWhenExist"
#define SETTINGS_INI_CREATE_FOLDER          "CreateFolder"
#define SETTINGS_INI_OBJ_BODY               "ExportBody"
#define SETTINGS_INI_OBJ_SURFACE            "ExportSurface"
#define SETTINGS_INI_UNITS                  "Units"
#define SETTINGS_INI_FORMAT_BIN             "Binary"
#define SETTINGS_INI_IS_LINEAR              "LinearUse"
#define SETTINGS_INI_LINEAR_VAL             "LinearVal"
#define SETTINGS_INI_IS_ANGLE               "AngleUse"
#define SETTINGS_INI_ANGLE_VAL              "AngleVal"
#define SETTINGS_INI_IS_RIDGE               "RidgeUse"
#define SETTINGS_INI_RIDGE_VAL              "RidgeVal"
#define SETTINGS_INI_SLICER_PATH            "SlicerPath"
#define SETTINGS_INI_SLICER_FORMAT          "SlicerFormat"

class Settings : public WithSettingsLay<TopWindow> {
private:
	Doc3D::ExportParams params;
	
public:
	Settings(const std::vector<Doc3D::Format>& exportTypes) {
		CtrlLayout(*this, t_("Settings"));
		for (const Doc3D::Format& type : exportTypes) {
			dlAEFormat.Add(type.value, type.Name());
			dlSlicerFormat.Add(type.value, type.Name());
		}
		dlUnits.Add(Doc3D::MM, "мм");
		dlUnits.Add(Doc3D::SM, "см");
		dlUnits.Add(Doc3D::DM, "дм");
		dlUnits.Add(Doc3D::M,  "м");
		
		eLinear.MinMax(SETTINGS_LINEAR_MIN, SETTINGS_LINEAR_MAX);
		sLinear.MinMax(int(SETTINGS_LINEAR_MIN * 1000.), int(SETTINGS_LINEAR_MAX * 1000.));
		eAngle.MinMax(SETTINGS_ANGLE_MIN,   SETTINGS_ANGLE_MAX);
		sAngle.MinMax(int(SETTINGS_ANGLE_MIN * 1000.), int(SETTINGS_ANGLE_MAX * 1000.));
		eRidge.MinMax(SETTINGS_RIDGE_MIN,   SETTINGS_RIDGE_MAX);
		sRidge.MinMax(int(SETTINGS_RIDGE_MIN * 1000.), int(SETTINGS_RIDGE_MAX * 1000.));
		
		sLinear.WhenAction = [=]() { eLinear <<= double(int(~sLinear)) / 1000.;	};
		eLinear.WhenAction = [=]() { sLinear <<= int(double(~eLinear)  * 1000.);};
		sAngle.WhenAction  = [=]() { eAngle  <<= double(int(~sAngle))  / 1000.; };
		eAngle.WhenAction  = [=]() { sAngle  <<= int(double(~eAngle)   * 1000.);};
		sRidge.WhenAction  = [=]() { eRidge  <<= double(int(~sRidge))  / 1000.; };
		eRidge.WhenAction  = [=]() { sRidge  <<= int(double(~eRidge)   * 1000.);};
	}
	
	const Doc3D::ExportParams& GetExportParams() { return params;	}
	
	String GetSlicerPath() { return ~eSlicerPath; }
	
	Doc3D::Format GetSlicerFormat() { return (Doc3D::Format::Value)(int)~dlSlicerFormat; }
	
	bool Load() {
		std::string dir = Kompas3D::ConfigPath();
		if (dir.empty()) return false;
		VectorMap<String, String> ini = LoadIniFile((dir + "/Kompas3DPrint.ini").c_str());
		if (ini.GetCount() == 0) return false;
		oAutoExportEn   <<= GetIniBool(ini, SETTINGS_INI_AUTOEXPORT_EN, true);
		oCreateFolder   <<= GetIniBool(ini, SETTINGS_INI_CREATE_FOLDER, false);
		oRewriteOnly    <<= GetIniBool(ini, SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS, false);
		
		params.format     = GetIniFmt( ini, SETTINGS_INI_FORMAT,        Doc3D::Format(Doc3D::Format::STL));
		dlAEFormat      <<= params.format.value;
		oExportBodies   <<= params.objBody    = GetIniBool(ini, SETTINGS_INI_OBJ_BODY,      true);
		oExportSurfaces <<= params.objSurface = GetIniBool(ini, SETTINGS_INI_OBJ_SURFACE,   false);
		dlUnits         <<= params.units      = GetIniUnit(ini, SETTINGS_INI_UNITS,         Doc3D::MM);
		sTxtBin         <<= params.formatBIN  = GetIniBool(ini, SETTINGS_INI_FORMAT_BIN,    true);
		oLinear         <<= params.isLinear   = GetIniBool(ini, SETTINGS_INI_IS_LINEAR,     true);
		eLinear         <<= params.linearVal  = GetIniDbl( ini, SETTINGS_INI_LINEAR_VAL,    0.001);
		sLinear         <<= int(params.linearVal * 1000.);
		oAngle          <<= params.isAngle    = GetIniBool(ini, SETTINGS_INI_IS_ANGLE,      false);
		eAngle          <<= params.angleVal   = GetIniDbl( ini, SETTINGS_INI_ANGLE_VAL,     7.2);
		sAngle          <<= int(params.angleVal * 1000.);
		oRidge          <<= params.isRidge    = GetIniBool(ini, SETTINGS_INI_IS_RIDGE,      false);
		eRidge          <<= params.ridgeVal   = GetIniDbl( ini, SETTINGS_INI_RIDGE_VAL,     1);
		sRidge          <<= int(params.ridgeVal * 1000.);
		
		eSlicerPath     <<= GetIniStr(ini, SETTINGS_INI_SLICER_PATH,   GetIniStr(ini, "CuraPath", ""));
		dlSlicerFormat  <<= GetIniFmt(ini, SETTINGS_INI_SLICER_FORMAT, Doc3D::Format(Doc3D::Format::STL)).value;
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
		SetIniStr( ini, SETTINGS_INI_SLICER_PATH, ~eSlicerPath);
		SetIniStr( ini, SETTINGS_INI_SLICER_FORMAT, Doc3D::Format((Doc3D::Format::Value)(int)~dlSlicerFormat).Name());
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
