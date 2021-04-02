#ifndef _ABOUT_DLG_H_
#define _ABOUT_DLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000  

// Идентификаторы основных ресурсов 
#include "resource.h"
#include "StdAfx.h"
#include <libtool.h>

class TAboutDlg : public CDialog {
  public:
    TAboutDlg(CWnd* pParent = NULL);
    enum { IDD = ABOUT_DIALOG };
};

#endif