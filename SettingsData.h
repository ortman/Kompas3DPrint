#ifndef _SETTINGS_DATA_H_
#define _SETTINGS_DATA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000  

// �������������� �������� �������� 
#include "resource.h"
#include "StdAfx.h"
#include "iniFile.h"

#define SETTINGS_LINEAR_MAX 1.0
#define SETTINGS_LINEAR_MIN 0.001
#define SETTINGS_ANGLE_MAX 90.0
#define SETTINGS_ANGLE_MIN 0.1
#define SETTINGS_RIDGE_MIN 0.01
#define SETTINGS_RIDGE_MAX 100

#define SETTINGS_INI_BLOCK_MAIN                 "STL"
#define SETTINGS_INI_AUTOEXPORT_EN              "��������_�����������"
#define SETTINGS_INI_AUTOEXPORT_WHEN_EXISTS     "�����������_������_�_������������_STL"
#define SETTINGS_INI_OBJ_BODY                   "��������������_����"
#define SETTINGS_INI_OBJ_SURFACE                "��������������_�����������"
#define SETTINGS_INI_UNITS                      "�������_���������"
#define SETTINGS_INI_FORMAT_BIN                 "��������_������"
#define SETTINGS_INI_IS_LINEAR                  "������������_��������_����������"
#define SETTINGS_INI_LINEAR_VAL                 "������������_��������_����������"
#define SETTINGS_INI_IS_ANGLE                   "������������_�������_����������"
#define SETTINGS_INI_ANGLE_VAL                  "������������_�������_����������"
#define SETTINGS_INI_IS_RIDGE                   "������������_�����_�����"
#define SETTINGS_INI_RIDGE_VAL                  "������������_�����_�����"

#define SETTINGS_DEFAULT_AUTOEXPORT_EN          true
#define SETTINGS_DEFAULT_AUTOEXPORT_WHEN_EXISTS false
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
    bool autoexportWhenExists;
    bool objBody, objSurface;
    ksLengthUnitsEnum units;
    bool formatBIN;
    bool isLinear;
    double linearVal;
    bool isAngle;
    double angleVal;
    bool isRidge;
    double ridgeVal;

    SettingsData();
    void resetToDefault();
    void SetPath(CString iniFilePath);
    CString GetPath();
    CString GetError();
    bool read();
    bool readDefaultSettings(KompasObjectPtr kompas);
    bool write();
};

#endif // _SETTINGS_DATA_H_