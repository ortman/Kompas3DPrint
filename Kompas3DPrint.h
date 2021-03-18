////////////////////////////////////////////////////////////////////////////////
//
// Step2_API7_3D.h - Библиотека на Visual C++
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _KAMPAS_3D_PRINT_H_
#define _KAMPAS_3D_PRINT_H_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// extern KompasAPI7::IApplicationPtr newKompasAPI;
extern KompasObjectPtr kompas;

//------------------------------------------------------------------------------
// Вспомогательные функции
// ---
// Загрузка строки из ресурсов
CString LoadStr     (int strID);

void AdviseDoc( LPDISPATCH doc, long docType, 
                bool fSelectMng = true, 
                bool fObject = true, 
                bool fStamp = true,
                bool fDocument = true,
                bool fSpecification = true,
                bool fSpcObject = true,
                long objType = -1);
                
void Save2STL(ksDocument3DPtr doc3D,  BSTR stlPath);

#endif // _KAMPAS_3D_PRINT_H_