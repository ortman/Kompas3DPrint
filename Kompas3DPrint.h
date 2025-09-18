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

CString LoadStr(int strID);
void AdviseDoc(LPDISPATCH doc, long docType);
bool Save(LPDISPATCH doc3d, D3FormatConvType format, CString &path);
CString GetTmpSTLPath();

#endif // _KAMPAS_3D_PRINT_H_