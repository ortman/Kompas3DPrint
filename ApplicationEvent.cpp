////////////////////////////////////////////////////////////////////////////////
//
// ApplicationEvent  - ���������� ������� �� ����������
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Kompas3DPrint.h"

#ifndef _APPLICATIONEVENT_H
#include "ApplicationEvent.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
//
// ApplicationEvent - ���������� ������� �� ����������
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------
//-
// ---
ApplicationEvent::ApplicationEvent( LPUNKNOWN pObject, bool selfAdvise /*true*/ )
  : BaseEvent( pObject, DIID_ksKompasObjectNotify, NULL, -1, NULL, selfAdvise )
{
}


//-------------------------------------------------------------------------------
//
// ---
ApplicationEvent::~ApplicationEvent()
{
}


//-------------------------------------------------------------------------------
//
// ---
BEGIN_EVENTSINK_MAP(ApplicationEvent, BaseEvent)
  ON_EVENT (ApplicationEvent, (unsigned int)-1, koCreateDocument,        CreateDocument,        VTS_DISPATCH VTS_I4)
  // ON_EVENT (ApplicationEvent, (unsigned int)-1, koBeginOpenDocument,     BeginOpenDocumen,      VTS_BSTR)
  ON_EVENT (ApplicationEvent, (unsigned int)-1, koOpenDocument,          OpenDocumen,           VTS_DISPATCH VTS_I4)
  // ON_EVENT (ApplicationEvent, (unsigned int)-1, koChangeActiveDocument,  ChangeActiveDocument,  VTS_DISPATCH VTS_I4)
  // ON_EVENT (ApplicationEvent, (unsigned int)-1, koApplicatinDestroy,     ApplicationDestroy,    VTS_NONE)
  // ON_EVENT (ApplicationEvent, (unsigned int)-1, koBeginCreate,           BeginCreate,           VTS_I4)
  // ON_EVENT (ApplicationEvent, (unsigned int)-1, koBeginOpenFile,         BeginOpenFile,         VTS_NONE)
  // ON_EVENT (ApplicationEvent, (unsigned int)-1, koBeginCloseAllDocument, BeginCloseAllDocument, VTS_NONE)
  // ON_EVENT (ApplicationEvent, (unsigned int)-1, koBeginRequestFiles,     BeginRequestFiles,     VTS_I4 VTS_PVARIANT)
END_EVENTSINK_MAP()


//-----------------------------------------------------------------------------
// koCreateDocument - �������� ������
// ---
afx_msg VARIANT_BOOL ApplicationEvent::CreateDocument( LPDISPATCH newDoc, long docType ) 
{    
  // if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
  // {
  //   CString str;
  //   str.Format( _T("%s --> ApplicationEvent::CreateDocument\nnewDoc = %i\ndocType = %i"), (LPCTSTR)m_libName, newDoc, docType );
  //   kompas->ksMessage( (LPCTSTR)str );
  // }  
  // if ( theApp.m_auto )
    AdviseDoc( newDoc, docType );
  return true;
}


// //-----------------------------------------------------------------------------
// // koOpenDocumenBegin - ������ �������� ���������
// // ---
// afx_msg VARIANT_BOOL ApplicationEvent::BeginOpenDocumen( LPCTSTR fileName )
// {
//   if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
//   {
//     CString str( m_libName + _T(" --> ApplicationEvent::BeginOpenDocumen\nfileName = ") + fileName );
//     return !!kompas->ksYesNo( (LPCTSTR)str );
//   }
//   return true;
// }


//-----------------------------------------------------------------------------
// koOpenDocumen - �������� ������
// ---
afx_msg VARIANT_BOOL ApplicationEvent::OpenDocumen( LPDISPATCH newDoc, long docType ) 
{
  // if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
  // {
  //   CString str;
  //   str.Format( _T("%s --> ApplicationEvent::OpenDocumen\nnewDoc = %i\ndocType = %i"), (LPCTSTR)m_libName, newDoc, docType );
  //   kompas->ksMessage( (LPCTSTR)str );
  // }
  // if ( theApp.m_auto )
    AdviseDoc( newDoc, docType );
  return true;
}


// //-----------------------------------------------------------------------------
// // koActiveDocument - ������������ �� ������ �������� ��������
// // ---
// afx_msg VARIANT_BOOL ApplicationEvent::ChangeActiveDocument( LPDISPATCH newDoc, long docType )
// {
//   if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
//   {
//     CString str;
//     str.Format( _T("%s --> ApplicationEvent::ChangeActiveDocument\nnewDoc = %i\ndocType = %i"), (LPCTSTR)m_libName, newDoc, docType );
//     kompas->ksMessage( (LPCTSTR)str );
//   }
//   return true;
// }


// //-----------------------------------------------------------------------------
// // koApplicatinDestroy - �������� ����������
// // ---
// afx_msg VARIANT_BOOL ApplicationEvent::ApplicationDestroy()
// {
//   if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
//   {
//     CString str( m_libName + _T(" --> ApplicationEvent::ApplicationDestroy") );
//     kompas->ksMessage( (LPCTSTR)str );
//   }
  
//   // ������������ �� ���� �������
//   BaseEvent::TerminateEvents( GUID_NULL );
//   return true;
// }


// //-----------------------------------------------------------------------------
// // koBeginCreate - ������ �������� ���������(�� ������� ������ ����)
// // ---
// afx_msg VARIANT_BOOL ApplicationEvent::BeginCreate( long docType )
// {
//   bool res = true;
//   if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
//   {
//     CString str;
//     str.Format( _T("%s --> ApplicationEvent::BeginCreate\ndocType = %i\n"), (LPCTSTR)m_libName, docType );
//     str += _T("�� - ������� ������\n") 
//            _T("��� - ��������� ����������� ������ �������� �����\n")
//            _T("������ - �� ��������� ����");
//     int comm = kompas->ksYesNo( (LPCTSTR)str );
//     switch ( comm ) 
//     {
//       case 1: 
//       {
//         ksDocumentParamPtr docParam( kompas->GetParamStruct( ko_DocumentParam ) );
//         docParam->Init();
//         docParam->type = lt_DocSheetStandart;
//         ksDocument2DPtr doc( kompas->Document2D() );
//         doc->ksCreateDocument( docParam );
//         res = !doc->Getreference(); // ���� �������� ������ ����������� ������ �� �����
//         break;
//       }  
//       case -1:
//         res = false;
//         break;
//     }
//   }
//   return res;
// }


// //-----------------------------------------------------------------------------
// // koBeginOpenFile - ������ �������� ���������(�� ������� ������ �����)
// // ---
// afx_msg VARIANT_BOOL ApplicationEvent::BeginOpenFile()
// {
//   bool res = true;
//   if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
//   {
//     CString str( m_libName + _T(" --> ApplicationEvent::BeginOpenFile\n")
//                  _T("�� - ������� ������\n")
//                  _T("��� - ��������� ����������� ������ �������� �����\n")
//                   _T("������ - �� ��������� ����") );
//     int comm = kompas->ksYesNo( (LPCTSTR)str );
//     switch ( comm ) 
//     {
//       case 1: 
//       {
//         ksDocumentParamPtr docParam( kompas->GetParamStruct( ko_DocumentParam ) );
//         docParam->Init();
//         docParam->type = lt_DocSheetStandart;
//         ksDocument2DPtr doc( kompas->Document2D() );
//         res = !doc->ksCreateDocument( docParam );
//         break;
//       }  
//       case -1:
//         res = false;
//         break;
//     }
//   }
//   return res;
// }


// //-----------------------------------------------------------------------------
// // koBeginCloseAllDocument - ������ �������� ���� �������� ����������
// // ---
// afx_msg VARIANT_BOOL ApplicationEvent::BeginCloseAllDocument()
// {
//   bool res = true;
//   if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
//   {
//     CString str;
//     str.Format( _T("%s --> ApplicationEvent::BeginCloseAllDocument\n������� ���?"), (LPCTSTR)m_libName );
//     res = !!kompas->ksYesNo( (LPCTSTR)str );
//   }
//   return res;
// }


// //-----------------------------------------------------------------------------
// // koBeginRequestFiles - ������ �������� ���� �������� ����������
// // ---
// afx_msg VARIANT_BOOL ApplicationEvent::BeginRequestFiles( long requestID, VARIANT * files )
// {
//   VARIANT_BOOL res = VARIANT_TRUE;
//   if ( IsSelfAdvise() /*&& theApp.m_mes_APP*/ )
//   {
//     if ( requestID == koRFCopyBilletPart )
//     {
      
//       _bstr_t file = kompas->ksChoiceFile( _T("m3d"), _T("������-������ (*.m3d)|*.m3d"), 1 );
//       if ( file.length() > 0 )
//       {
//         files->vt = VT_BSTR;
//         files->bstrVal = file.copy(); 
//       }  
//     }
//   }
//   return res;
// }

