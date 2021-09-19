// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef _AFX_STDAFX_H_
#define _AFX_STDAFX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _USE_MATH_DEFINES
#include <cmath>

#define VC_EXTRALEAN    // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>      // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>    // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>      // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//-------------------------------------------------------------------------------
// ksConstants.tlb
// ---
#import "ksConstants.tlb" no_namespace
#import "ksConstants3D.tlb" no_namespace  

//-------------------------------------------------------------------------------
// kAPI5.tlb
// ---
#pragma warning( disable : 4278 )
#import "kAPI5.tlb" no_namespace named_guids
//#import "kAPI7.tlb" no_namespace named_guids
#pragma warning( once : 4278 ) 

#include <ldefin2d.h>

#endif // _AFX_STDAFX_H_
