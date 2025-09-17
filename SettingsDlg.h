#ifndef _SETTINGS_DLG_H_
#define _SETTINGS_DLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000  

// Идентификаторы основных ресурсов 
#include "resource.h"
#include "StdAfx.h"
#include <libtool.h>
#include "SettingsData.h"

class TSettingsDlg : public CDialog {
  private:
    SettingsData userSettings;
  public:
    reference rGroup;
    TSettingsDlg(CWnd* pParent = NULL);
    enum { IDD = SETTINGS_DIALOG };

    CButton cAutoexportEn;
    CComboBox cbFormat;
    CButton cAutoexportWhenExists;
    CButton cCreateFolder;
    CButton cObjBody;
    CButton cObjSurface;
    CComboBox cbUnits;
    CButton rFormatTXT;
    CButton rFormatBIN;
    CButton cLinear;
    CSliderCtrl sLinear;
    CEdit eLinearVal;
    CButton cAngle;
    CSliderCtrl sAngle;
    CEdit eAngleVal;
    CButton cRidge;
    CEdit eRidgeVal;
    CEdit eCuraPath;

  protected:
    int m_bitMapId;
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnPaint();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnLinearValKillFocus();
    afx_msg void OnAngleValKillFocus();
    afx_msg void OnRidgeValKillFocus();
    afx_msg void OnSaveButtonClicked();

  DECLARE_MESSAGE_MAP()
};

#endif // _SETTINGS_DLG_H_