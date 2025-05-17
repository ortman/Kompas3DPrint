#include "SettingsData.h"
//#include <ldefin2d.h>

SettingsData::SettingsData() {
  resetToDefault();
}

void SettingsData::resetToDefault() {
  autoexportEn = SETTINGS_DEFAULT_AUTOEXPORT_EN;
  format = SETTINGS_DEFAULT_FORMAT;
  autoexportWhenExists = SETTINGS_DEFAULT_AUTOEXPORT_WHEN_EXISTS;
  createFolder = SETTINGS_DEFAULT_CREATE_FOLDER;
  objBody = SETTINGS_DEFAULT_OBJ_BODY;
  objSurface = SETTINGS_DEFAULT_OBJ_SURFACE;
  units = SETTINGS_DEFAULT_UNITS;
  formatBIN = SETTINGS_DEFAULT_FORMAT_BIN;
  isLinear = SETTINGS_DEFAULT_IS_LINEAR;
  linearVal = SETTINGS_DEFAULT_LINEAR_VAL;
  isAngle = SETTINGS_DEFAULT_IS_ANGLE;
  angleVal = SETTINGS_DEFAULT_ANGLE_VAL;
  isRidge = SETTINGS_DEFAULT_IS_RIDGE;
  ridgeVal = SETTINGS_DEFAULT_RIDGE_VAL;
}

void SettingsData::setPath(CString iniFilePath) {
  iniFile.SetPath(iniFilePath);
}

CString SettingsData::getPath() {
  return iniFile.path;
}

CString SettingsData::getError() {
  return iniFile.error;
}

CString SettingsData::format2Str(D3FormatConvType format) {
  switch (format) {
    case format_STEP_AP203: return _T("STEP_AP203");
    case format_STEP_AP214: return _T("STEP_AP214");
    case format_STEP_AP242: return _T("STEP_AP242");
    case format_IGES: return _T("IGES");
    case format_SAT: return _T("ACIS");
    case format_XT: return _T("PARASOLID");
    case format_VRML: return _T("VRML");
    default: return _T("STL");
  }
}

CString SettingsData::format2Str() {
  return format2Str(format);
}

CString SettingsData::getExt(D3FormatConvType format) {
  switch (format) {
    case format_STEP_AP203:
    case format_STEP_AP214:
    case format_STEP_AP242: return _T(".stp");
    case format_IGES: return _T(".igs");
    case format_SAT: return _T(".sat");
    case format_XT: return _T(".x_t");
    case format_VRML: return _T(".wrl");
    default: return _T(".stl");
  }
}

CString SettingsData::getExt() {
  return getExt(format);
}

bool SettingsData::read() {
  if (!iniFile.ReadFile()) return false;
  autoexportEn  = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_AUTOEXPORT_EN, SETTINGS_DEFAULT_AUTOEXPORT_EN);
  CString formatStr = iniFile.GetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_FORMAT);
  if (formatStr == "STEP_AP203") format = format_STEP_AP203;
  else if (formatStr == "STEP_AP214") format = format_STEP_AP214;
  else if (formatStr == "STEP_AP242") format = format_STEP_AP242;
  else if (formatStr == "IGES") format = format_IGES;
  else if (formatStr == "ACIS") format = format_SAT;
  else if (formatStr == "PARASOLID") format = format_XT;
  else if (formatStr == "VRML") format = format_VRML;
  else format = format_STL;
  autoexportWhenExists = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS,
      SETTINGS_DEFAULT_AUTOEXPORT_WHEN_EXISTS);
  createFolder = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_CREATE_FOLDER, SETTINGS_DEFAULT_CREATE_FOLDER);
  objBody       = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_OBJ_BODY, SETTINGS_DEFAULT_OBJ_BODY);
  objSurface    = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_OBJ_SURFACE, SETTINGS_DEFAULT_OBJ_SURFACE);
  CString unitsStr = iniFile.GetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS);
  if (unitsStr == "cm") units = ksLUnSM;
  else if (unitsStr == "dm") units = ksLUnDM;
  else if (unitsStr == "m") units = ksLUnM;
  else units = ksLUnMM;
  formatBIN  = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_FORMAT_BIN, SETTINGS_DEFAULT_FORMAT_BIN);
  isLinear   = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_LINEAR, SETTINGS_DEFAULT_IS_LINEAR);
  linearVal  = iniFile.GetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_LINEAR_VAL, SETTINGS_DEFAULT_LINEAR_VAL,
      SETTINGS_LINEAR_MIN, SETTINGS_LINEAR_MAX);
  isAngle    = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_ANGLE, SETTINGS_DEFAULT_IS_ANGLE);
  angleVal   = iniFile.GetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_ANGLE_VAL, SETTINGS_DEFAULT_ANGLE_VAL,
      SETTINGS_ANGLE_MIN, SETTINGS_ANGLE_MAX);
  isRidge    = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_RIDGE, SETTINGS_DEFAULT_IS_RIDGE);
  ridgeVal   = iniFile.GetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_RIDGE_VAL, SETTINGS_DEFAULT_RIDGE_VAL,
      SETTINGS_RIDGE_MIN, SETTINGS_RIDGE_MAX);
  return true;
}

bool SettingsData::readDefaultSettings(KompasObjectPtr kompas) {
  if (!kompas) return false;
  _bstr_t configPath = kompas->ksSystemPath(sptCONFIG_FILES);
  configPath += "\\Kompas3DPrint.ini";
  setPath((wchar_t*)configPath);
  return read();
}

bool SettingsData::write() {
  iniFile.Reset();
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_AUTOEXPORT_EN, autoexportEn, true);
  iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_FORMAT, format2Str(format), true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS, autoexportWhenExists, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_CREATE_FOLDER, createFolder, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_OBJ_BODY, objBody, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_OBJ_SURFACE, objSurface, true);
  if (units == ksLUnSM) iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS, "cm", true);
  else if (units == ksLUnDM) iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS, "dm", true);
  else if (units == ksLUnM) iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS, "m", true);
  else iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS, "mm", true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_FORMAT_BIN, formatBIN, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_LINEAR, isLinear, true);
  iniFile.SetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_LINEAR_VAL, linearVal, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_ANGLE, isAngle, true);
  iniFile.SetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_ANGLE_VAL, angleVal, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_RIDGE, isRidge, true);
  iniFile.SetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_RIDGE_VAL, ridgeVal, true);
  return iniFile.WriteFile();
}