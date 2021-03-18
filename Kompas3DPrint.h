////////////////////////////////////////////////////////////////////////////////
//
// Kompas 3D Print
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _KAMPAS_3D_PRINT_H_
#define _KAMPAS_3D_PRINT_H_

#include "stdafx.h"
#include "resource.h"       // main symbols

extern KompasObjectPtr kompas;

CString LoadStr (int strID);

void AdviseDoc(LPDISPATCH doc, long docType, 
               bool fSelectMng = true, 
               bool fObject = true, 
               bool fStamp = true,
               bool fDocument = true,
               bool fSpecification = true,
               bool fSpcObject = true,
               long objType = -1);
                
void Save2STL(ksDocument3DPtr doc3D,  BSTR stlPath);

#endif // _KAMPAS_3D_PRINT_H_