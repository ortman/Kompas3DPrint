#include "SettingsDlg.h"
#include <libtool.h>

extern KompasObjectPtr kompas;

ksLengthUnitsEnum _unitsData[] = {
  ksLUnMM,
  ksLUnSM,
  ksLUnDM,
  ksLUnM
};

TSettingsDlg::TSettingsDlg(CWnd* pParent /*=NULL*/)
        : CDialog(TSettingsDlg::IDD, pParent) {
  if (kompas) {
    _bstr_t configPath = kompas->ksSystemPath(sptCONFIG_FILES);
    configPath += "\\Kompas3DPrint.ini";
    userSettings.SetPath((wchar_t*)configPath);
    userSettings.read();
  }
}

void TSettingsDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_AUTOEXPORT_EN, cAutoexportEn);

  DDX_Control(pDX, IDC_OBJ_BODY, cObjBody);
  DDX_Control(pDX, IDC_OBJ_SURFACE, cObjSurface);

  DDX_Control(pDX, IDC_UNITS, cbUnits);

  DDX_Control(pDX, IDC_FORMAT_TXT, rFormatTXT);
  DDX_Control(pDX, IDC_FORMAT_BIN, rFormatBIN);
  
  DDX_Control(pDX, IDC_LINEAR_CHECK, cLinear);
  DDX_Control(pDX, IDC_LINEAR_TRACKBAR, sLinear);
  DDX_Control(pDX, IDC_LINEAR_VAL, eLinearVal);
  
  DDX_Control(pDX, IDC_ANGLE_CHECK, cAngle);
  DDX_Control(pDX, IDC_ANGLE_TRACKBAR, sAngle);
  DDX_Control(pDX, IDC_ANGLE_VAL, eAngleVal);
  
  DDX_Control(pDX, IDC_RIDGE_CHECK, cRidge);
  DDX_Control(pDX, IDC_RIDGE_VAL, eRidgeVal);
}

BEGIN_MESSAGE_MAP( TSettingsDlg, CDialog )
  ON_WM_PAINT()
  ON_WM_HSCROLL()
  ON_EN_KILLFOCUS(IDC_LINEAR_VAL, &TSettingsDlg::OnLinearValKillFocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_VAL, &TSettingsDlg::OnAngleValKillFocus)
  ON_EN_KILLFOCUS(IDC_RIDGE_VAL, &TSettingsDlg::OnRidgeValKillFocus)
END_MESSAGE_MAP()

BOOL TSettingsDlg::OnInitDialog() {
  TCHAR textVal[32];
	CDialog::OnInitDialog();

  cAutoexportEn.SetCheck(userSettings.autoexportEn);

  cObjBody.SetCheck(userSettings.objBody);
  cObjSurface.SetCheck(userSettings.objSurface);

  cbUnits.Clear();
  cbUnits.AddString(_T("миллиметры"));
  cbUnits.AddString(_T("сантиметры"));
  cbUnits.AddString(_T("дециметры"));
  cbUnits.AddString(_T("метры"));
  for (int i = 0; i < 4; i++) {
    cbUnits.SetItemDataPtr(i, &_unitsData[i]);
  }
  for (int i = 0; i < cbUnits.GetCount(); i++) {
    if (*(ksLengthUnitsEnum*)cbUnits.GetItemDataPtr(i) == userSettings.units) {
        cbUnits.SetCurSel(i);
        break;
    }
  }
  
  rFormatTXT.SetCheck(!userSettings.formatBIN);
  rFormatBIN.SetCheck(userSettings.formatBIN);

  cLinear.SetCheck(userSettings.isLinear);
  sLinear.SetRange(0, 999);
  sLinear.SetPos(1000-(int)(userSettings.linearVal * 1000.0));
  swprintf(textVal, 32, L"%.3f", userSettings.linearVal);
  eLinearVal.SetWindowText(textVal);
  
  cAngle.SetCheck(userSettings.isAngle);
  sAngle.SetRange(0, 899);
  sAngle.SetPos(900-(int)(userSettings.angleVal * 10.0));
  swprintf(textVal, 32, L"%.1f", userSettings.angleVal);
  eAngleVal.SetWindowText(textVal);
  
  cRidge.SetCheck(userSettings.isRidge);
  swprintf(textVal, 32, L"%f", userSettings.ridgeVal);
  eRidgeVal.SetWindowText(textVal);

  UpdateData(FALSE);
  ksConvertLangWindow(m_hWnd);
	return TRUE;
}

void TSettingsDlg::OnPaint() {
  CPaintDC dc(this);
}

void TSettingsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
  if (nSBCode != TB_THUMBTRACK) return;
  CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
  TCHAR textVal[32];
  if (pSlider == &sLinear) {
    double val = (double)(1000 - pSlider->GetPos()) / 1000.0;
    swprintf(textVal, 32, L"%.3f", val);
    eLinearVal.SetWindowText(textVal);
    cLinear.SetCheck(TRUE);
  } else if (pSlider == &sAngle) {
    double val = (double)(900 - pSlider->GetPos()) / 10.0;
    swprintf(textVal, 32, L"%.1f", val);
    eAngleVal.SetWindowText(textVal);
    cAngle.SetCheck(TRUE);
  }
}

double checkVal(CEdit *edit, wchar_t const * const _Format, double min, double max, CButton *checkBox) {
  TCHAR textVal[32];
  if (edit == NULL || checkBox == NULL) return min;
  CString sVal;
  edit->GetWindowText(sVal);
  double val = wcstod(sVal, NULL);
  checkBox->SetCheck(TRUE);
  if (val < min) {
    val = min;
  } else if (val > max) {
    val = max;
  }
  swprintf(textVal, 32, _Format, val);
  edit->SetWindowText(textVal);
  return val;
}

void TSettingsDlg::OnLinearValKillFocus() {
  double val = checkVal(&eLinearVal, L"%.3f", SETTINGS_LINEAR_MIN, SETTINGS_LINEAR_MAX, &cLinear);
  sLinear.SetPos(1000-(int)(val * 1000.0));
}

void TSettingsDlg::OnAngleValKillFocus() {
  double val = checkVal(&eAngleVal, L"%.1f", SETTINGS_ANGLE_MIN, SETTINGS_ANGLE_MAX, &cAngle);
  sAngle.SetPos(900-(int)(val * 10.0));
}

void TSettingsDlg::OnRidgeValKillFocus() {
  checkVal(&eRidgeVal, L"%f", SETTINGS_RIDGE_MIN, SETTINGS_RIDGE_MAX, &cRidge);
}

void TSettingsDlg::OnOK() {
  CString sVal;
  userSettings.autoexportEn = cAutoexportEn.GetCheck();
  userSettings.objBody = cObjBody.GetCheck();
  userSettings.objSurface = cObjSurface.GetCheck();
  userSettings.units = *(ksLengthUnitsEnum*) cbUnits.GetItemDataPtr(cbUnits.GetCurSel());
  userSettings.formatBIN = rFormatBIN.GetCheck();
  userSettings.isLinear = cLinear.GetCheck();
  eLinearVal.GetWindowText(sVal);
  userSettings.linearVal = wcstod(sVal, NULL);
  userSettings.isAngle = cAngle.GetCheck();
  eAngleVal.GetWindowText(sVal);
  userSettings.angleVal = wcstod(sVal, NULL);
  userSettings.isRidge = cRidge.GetCheck();
  eRidgeVal.GetWindowText(sVal);
  userSettings.ridgeVal = wcstod(sVal, NULL);

  if (userSettings.write()) {
    CDialog::OnOK();
  } else if (kompas) {
    _bstr_t msg = L"Не удалось сохранить настройки в файл :\"" + userSettings.GetPath() + 
        L"\". \nОшибка: \"" + userSettings.GetError() + L"\"";
    kompas->ksError(msg);
  }
}