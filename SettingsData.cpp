#include "SettingsData.h"
#include <ldefin2d.h>

SettingsData::SettingsData() {
  resetToDefault();
}

void SettingsData::resetToDefault() {
  autoexportEn = SETTINGS_DEFAULT_AUTOEXPORT_EN;
  autoexportWhenExists = SETTINGS_DEFAULT_AUTOEXPORT_WHEN_EXISTS;
  createStlFolder = SETTINGS_DEFAULT_CREATE_STL_FOLDER;
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

void SettingsData::SetPath(CString iniFilePath) {
  iniFile.SetPath(iniFilePath);
}

CString SettingsData::GetPath() {
  return iniFile.path;
}

CString SettingsData::GetError() {
  return iniFile.error;
}

bool SettingsData::read() {
  if (!iniFile.ReadFile()) return false;
  autoexportEn  = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_AUTOEXPORT_EN, SETTINGS_DEFAULT_AUTOEXPORT_EN);
  autoexportWhenExists = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS,
      SETTINGS_DEFAULT_AUTOEXPORT_WHEN_EXISTS);
  createStlFolder = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_CREATE_STL_FOLDER, SETTINGS_DEFAULT_CREATE_STL_FOLDER);
  objBody       = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_OBJ_BODY, SETTINGS_DEFAULT_OBJ_BODY);
  objSurface    = iniFile.GetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_OBJ_SURFACE, SETTINGS_DEFAULT_OBJ_SURFACE);
  CString unitsStr = iniFile.GetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS);
  if (unitsStr == "סל") units = ksLUnSM;
  else if (unitsStr == "הל") units = ksLUnDM;
  else if (unitsStr == "ל") units = ksLUnM;
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
  SetPath((wchar_t*)configPath);
  return read();
}

bool SettingsData::write() {
  iniFile.Reset();
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_AUTOEXPORT_EN, autoexportEn, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS, autoexportWhenExists, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_CREATE_STL_FOLDER, createStlFolder, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_OBJ_BODY, objBody, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_OBJ_SURFACE, objSurface, true);
  if (units == ksLUnSM) iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS, "סל", true);
  else if (units == ksLUnDM) iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS, "הל", true);
  else if (units == ksLUnM) iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS, "ל", true);
  else iniFile.SetValue(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_UNITS, "לל", true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_FORMAT_BIN, formatBIN, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_LINEAR, isLinear, true);
  iniFile.SetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_LINEAR_VAL, linearVal, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_ANGLE, isAngle, true);
  iniFile.SetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_ANGLE_VAL, angleVal, true);
  iniFile.SetValueB(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_IS_RIDGE, isRidge, true);
  iniFile.SetValueF(SETTINGS_INI_BLOCK_MAIN, SETTINGS_INI_RIDGE_VAL, ridgeVal, true);
  return iniFile.WriteFile();
}