#ifndef _SETTINGS_DATA_H_
#define _SETTINGS_DATA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000  

// Идентификаторы основных ресурсов 
#include "resource.h"
#include "StdAfx.h"
#include "iniFile.h"

#define SETTINGS_LINEAR_MAX 1.0
#define SETTINGS_LINEAR_MIN 0.001
#define SETTINGS_ANGLE_MAX 90.0
#define SETTINGS_ANGLE_MIN 0.1
#define SETTINGS_RIDGE_MIN 0.01
#define SETTINGS_RIDGE_MAX 100

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

class SettingsData {
  private:
    CIniFile iniFile;
  public:
    bool autoexportEn;
    D3FormatConvType format;
    bool autoexportWhenExists;
    bool createFolder;
    bool objBody, objSurface;
    ksLengthUnitsEnum units;
    bool formatBIN;
    bool isLinear;
    double linearVal;
    bool isAngle;
    double angleVal;
    bool isRidge;
    double ridgeVal;
    CString curaPath;

    SettingsData();
    void resetToDefault();
    void setPath(CString iniFilePath);
    CString getPath();
    CString getError();
    static CString format2Str(D3FormatConvType format);
    CString format2Str();
    static CString getExt(D3FormatConvType format);
    CString getExt();
    bool read();
    bool readDefaultSettings(KompasObjectPtr kompas);
    bool write();
    static CString FindCuraPath();
};

#endif // _SETTINGS_DATA_H_