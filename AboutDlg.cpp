#include "AboutDlg.h"

TAboutDlg::TAboutDlg(CWnd* pParent /*=NULL*/)
        : CDialog(TAboutDlg::IDD, pParent) {

}

BEGIN_MESSAGE_MAP( TAboutDlg, CDialog )
  ON_NOTIFY(NM_CLICK, IDC_SYSLINK_GITHUB, &TAboutDlg::OnGithubSyslinkClick) 
END_MESSAGE_MAP()

void TAboutDlg::OnGithubSyslinkClick(NMHDR *pNMHDR, LRESULT *pResult) {
  ShellExecuteW(NULL, L"open", _T("https://github.com/ortman/Kompas3DPrint/releases"), NULL, NULL, SW_SHOWNORMAL);
  *pResult = 0; 
}