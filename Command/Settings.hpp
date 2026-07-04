#ifndef _Kompas3DPrint_Settings_hpp_
#define _Kompas3DPrint_Settings_hpp_

#include <CtrlLib/CtrlLib.h>

#define SETTINGS_INI_BLOCK_MAIN                 "Autoexport"
#define SETTINGS_INI_AUTOEXPORT_EN              "AutoexportOn"
#define SETTINGS_INI_FORMAT                     "Format"
#define SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS     "AutoexportWhenExist"
#define SETTINGS_INI_CREATE_FOLDER              "CreateFolder"
#define SETTINGS_INI_OBJ_BODY                   "ExportBody"
#define SETTINGS_INI_OBJ_SURFACE                "ExportSurface"
#define SETTINGS_INI_UNITS                      "Units"
#define SETTINGS_INI_FORMAT_BIN                 "Binary"
#define SETTINGS_INI_IS_LINEAR                  "LinearUse"
#define SETTINGS_INI_LINEAR_VAL                 "LinearVal"
#define SETTINGS_INI_IS_ANGLE                   "AngleUse"
#define SETTINGS_INI_ANGLE_VAL                  "AngleVal"
#define SETTINGS_INI_IS_RIDGE                   "RidgeUse"
#define SETTINGS_INI_RIDGE_VAL                  "RidgeVal"
#define SETTINGS_INI_CURA_PATH                  "CuraPath"

#define SETTINGS_DEFAULT_AUTOEXPORT_EN          true
#define SETTINGS_DEFAULT_FORMAT                 format_STL
#define SETTINGS_DEFAULT_AUTOEXPORT_WHEN_EXISTS false
#define SETTINGS_DEFAULT_CREATE_FOLDER          false
#define SETTINGS_DEFAULT_OBJ_BODY               true
#define SETTINGS_DEFAULT_OBJ_SURFACE            false
#define SETTINGS_DEFAULT_UNITS                  ksLUnMM
#define SETTINGS_DEFAULT_FORMAT_BIN             true
#define SETTINGS_DEFAULT_IS_LINEAR              true
#define SETTINGS_DEFAULT_LINEAR_VAL             0.001
#define SETTINGS_DEFAULT_IS_ANGLE               false
#define SETTINGS_DEFAULT_ANGLE_VAL              7.2
#define SETTINGS_DEFAULT_IS_RIDGE               false
#define SETTINGS_DEFAULT_RIDGE_VAL              1

#include "../Kompas/Doc3D.h"

class Settings : public WithSettingsLay<TopWindow> {
public:
	Settings() {
		CtrlLayout(*this, t_("Settings"));
	}
	
	Doc3D::ExportParams GetExportParams(Doc3D::Format format) {
		Doc3D::ExportParams params = {format};
		return params;
	}
};

#endif
