// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef _AFX_STDAFX_H_
#define _AFX_STDAFX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#import "ksConstants.tlb" no_namespace
#import "ksConstants3D.tlb" no_namespace  

#pragma warning( disable : 4278 )

#import "ConvertLibInterfaces.tlb" no_namespace named_guids
#import "kAPI2D5COM.tlb" no_namespace named_guids
#import "kAPI3D5COM.tlb" no_namespace named_guids

#import "kAPI5.tlb" no_namespace named_guids
#import "kAPI7.tlb" /*no_namespace*/ named_guids

#import "KGAX.tlb" no_namespace named_guids
#import "ksTreeView.tlb" no_namespace named_guids
#import "MyConverter.tlb" no_namespace named_guids
#import "VCHatch.tlb" no_namespace named_guids
#pragma warning( once : 4278 ) 

#endif // _AFX_STDAFX_H_
