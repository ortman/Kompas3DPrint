/////////////////////////////////////////////////////////////////////////////
//
// ������� ����� ��� ������������ �������
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <afxctl.h>
#include <afxpriv.h>

// #include "eventsAuto.h"
#include "Kompas3DPrint.h"

#ifndef _BASEEVENT_H
#include "baseEvent.h"
#endif
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------------------
// 
// ---
CObList BaseEvent::m_EventList;

/////////////////////////////////////////////////////////////////////////////
//
// ������� ����� ��� ������������ �������
//
/////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------
//
// ---
BaseEvent::BaseEvent( LPUNKNOWN pObject, 
                      IID iidEvents,  
                      LPDISPATCH doc, /*NULL*/ 
                      long objType, /*-1*/ 
                      ksFeaturePtr obj3D /*NULL*/,
                      bool selfAdvise /*true*/) 
  : CCmdTarget   (),
    m_dwCookie   ( 0          ),
    m_pContainer ( pObject    ), // �������� �������
    m_iidEvents  ( iidEvents  ),
    m_doc        ( doc        ), // ��������
    m_objType    ( objType    ), // ��� �������
    m_obj3D      ( obj3D      ), // ������ ��� 3D
    m_selfAdvise ( selfAdvise ), // �������� �������������
    m_pConnPt    ( NULL       ) 
{
  m_libName.LoadString( IDR_LIBID );

  m_EventList.AddTail( this );

  if ( m_pContainer ) 
    m_pContainer->AddRef();
  
  if ( m_doc )
    m_doc->AddRef();
    
  if ( m_obj3D )
    m_obj3D->AddRef();

  ASSERT( !IsEqualIID(m_iidEvents, GUID_NULL) ); // �������� ��� �������
}


//-------------------------------------------------------------------------------
//
// ---
BaseEvent::~BaseEvent()
{
  // ������ ���� �� ������ ������������ �������
  POSITION pos = m_EventList.Find( this );
  if ( pos ) 
  {
    m_EventList.RemoveAt( pos );


  }

  Unadvise(); // ��������� �� ��������� �������
  if ( m_pContainer ) 
  {
    m_pContainer->Release();
    m_pContainer = NULL;     
  }

  if ( m_doc ) 
  {
    m_doc->Release();
    m_doc = NULL;
  }

  if ( m_obj3D ) 
  {
    m_obj3D->Release();
    m_obj3D = NULL;
  }
}


//-------------------------------------------------------------------------------
// �������� ��� ���������
// ---
CString BaseEvent::GetDocName()
{
  _bstr_t fileName;
  if( m_doc )
  { 
    ksDocument2DPtr doc2D;
    m_doc->QueryInterface( DIID_ksDocument2D, (LPVOID*)&doc2D );
    if( doc2D ) 
    {
      ksDocumentParamPtr docPar( kompas->GetParamStruct(ko_DocumentParam) );
      doc2D->ksGetObjParam( doc2D->Getreference(), docPar, ALLPARAM );
      fileName = docPar->fileName;
    }
    else
    {
      ksDocument3DPtr doc3D;
      m_doc->QueryInterface( DIID_ksDocument3D, (LPVOID*)&doc3D );
      if( doc3D ) 
      {  
        fileName = doc3D->fileName; 
      }
      else
      {
        ksSpcDocumentPtr docSpc( m_doc );
        m_doc->QueryInterface( DIID_ksSpcDocument, (LPVOID*)&docSpc );
        if( docSpc ) 
        {   
          ksDocumentParamPtr docPar( kompas->GetParamStruct(ko_DocumentParam) );
          docSpc->ksGetObjParam( docSpc->Getreference(), docPar, ALLPARAM );
          fileName = docPar->fileName;
        }
        else
        {
          ksDocumentTxtPtr docTxt( m_doc );
          m_doc->QueryInterface( DIID_ksDocumentTxt, (LPVOID*)&docTxt );
          if( docTxt ) 
          {   
            ksTextDocumentParamPtr docPar( kompas->GetParamStruct(ko_TextDocumentParam) );
            docTxt->ksGetObjParam( docTxt->Getreference(), docPar, ALLPARAM );
            fileName = docPar->fileName;
          }
        }
      }
    }
  }
  return (LPCTSTR)fileName;
}


//-------------------------------------------------------------------------------
// ����������� �� ��������� �������
// ---
int BaseEvent::Advise() 
{
  ASSERT( m_dwCookie == 0 ); // �������� ������������� ������

  // ����������� �� ��������� �������
  if ( m_pContainer ) 
  {
    LPCONNECTIONPOINTCONTAINER lpContainer = NULL;
    if ( SUCCEEDED(m_pContainer->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&lpContainer)) ) 
    {
      if ( lpContainer && SUCCEEDED(lpContainer->FindConnectionPoint(m_iidEvents, &m_pConnPt)) ) 
      {
        ASSERT( m_pConnPt != NULL );
        m_pConnPt->Advise( &m_xEventHandler, &m_dwCookie ); 
      }
      lpContainer->Release();
    }
  }

  // ��������� �������� �� �������
  if( !m_dwCookie )
  {
    delete this;
    return 0;
  }

  if ( IsSelfAdvise() )
  {
    /*if( IsEqualIID(m_iidEvents, DIID_ksKompasObjectNotify) && theApp.m_mes_APP )
    { 
      CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> ApplicationEvent::Advise") );
      kompas->ksMessage( str.GetBuffer(0) );
    }
    else if( IsEqualIID(m_iidEvents, DIID_ksDocument2DNotify) && theApp.m_mes_2DDOC )
    {
      CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> Document2DEvent::Advise\n��� ��������� = ") + GetDocName() );
      kompas->ksMessage( str.GetBuffer(0) );
    }
    else if( IsEqualIID(m_iidEvents, DIID_ksDocument3DNotify) && theApp.m_mes_3DDOC )
    {
      CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> Document3DEvent::Advise\n��� ��������� = ") + GetDocName() );
      kompas->ksMessage( str.GetBuffer(0) );
    }
    else */if( IsEqualIID(m_iidEvents, DIID_ksDocumentFileNotify) /*&& theApp.m_mes_DOC*/ )
    {
      CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> DocumentEvent::Advise\n��� ��������� = ") + GetDocName() );
      kompas->ksMessage( str.GetBuffer(0) );
    }
    // else if( IsEqualIID(m_iidEvents, DIID_ksObject2DNotify) && theApp.m_mes_OBJ_2DDOC )
    // {
    //   ksDocument2DPtr doc2D( m_doc );
    //   if( doc2D != NULL && doc2D->ksExistObj(m_objType) ) 
    //     doc2D->ksLightObj( m_objType, true );

    //   CString strType;
    //   strType.Format( _T("\n���/��������� ������� = %i"), m_objType );
    //   CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> Object2DEvent::Advise\n��� ��������� = ") + GetDocName() + strType );
    //   kompas->ksMessage( str.GetBuffer(0) );
      
    //   if( doc2D != NULL && doc2D->ksExistObj(m_objType) ) 
    //     doc2D->ksLightObj( m_objType, false );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksObject3DNotify) && theApp.m_mes_OBJ_3DDOC )
    // {
    //   ksDocument3DPtr doc3D( m_doc );
    //   ksChooseMngPtr chooseMng;
    //   if( doc3D != NULL  && m_obj3D != NULL && 
    //       ( chooseMng = doc3D->GetChooseMng() ) != NULL )         
    //     chooseMng->Choose( m_obj3D );

    //   CString strType;
    //   strType.Format( _T("\n��� ������� = %i"), m_objType );
    //   CString strObj3DName;
    //   if( m_obj3D )
    //      strObj3DName = (LPCTSTR)m_obj3D->Getname();
    //   CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> Object3DEvent::Advise\n��� ��������� = ") + GetDocName() + strType + "\n��� 3D ������� = " + strObj3DName );
    //   kompas->ksMessage( str.GetBuffer(0) );

    //   if( chooseMng != NULL )
    //     chooseMng->UnChoose( m_obj3D ); 
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksSelectionMngNotify) && theApp.m_mes_SELECT )
    // {
    //   CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> SelectMngEvent::Advise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksSpcDocumentNotify) && theApp.m_mes_SPCDOC )
    // {
    //   CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> SpcDocumentEvent::Advise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksSpcObjectNotify) && theApp.m_mes_OBJ_SPC )
    // {
    //   CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> SpcObjectEvent::Advise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksSpecificationNotify) && theApp.m_mes_SPC )
    // {
    //   CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> SpecificationEvent::Advise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksStampNotify) && theApp.m_mes_STAMP )
    // {
    //   CString str( _T("����������� �� ��������� �������\n") + m_libName + _T(" --> StampEvent::Advise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
  } 

  return m_dwCookie;
}


//-------------------------------------------------------------------------------
// ���������� �� ��������� �������
// ---
void BaseEvent::Unadvise()
{
  if ( m_pConnPt != NULL )              // �������� ����
  {
    m_pConnPt->Unadvise( m_dwCookie );  // ���������� �� ��������� �������
    m_pConnPt->Release();                // ���������� 
    m_pConnPt  = NULL;
  } 
  m_dwCookie = 0;

  if ( IsSelfAdvise() )
  {
    /*if( IsEqualIID(m_iidEvents, DIID_ksKompasObjectNotify) && theApp.m_mes_APP )
    { 
      CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> ApplicationEvent::Unadvise") );
      kompas->ksMessage( str.GetBuffer(0) );
    }
    else if( IsEqualIID(m_iidEvents, DIID_ksDocument2DNotify) && theApp.m_mes_2DDOC )
    {
      CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> Document2DEvent::Unadvise\n��� ��������� = ") + GetDocName() );
      kompas->ksMessage( str.GetBuffer(0) );
    }
    else if( IsEqualIID(m_iidEvents, DIID_ksDocument3DNotify) && theApp.m_mes_3DDOC )
    {
      CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> Document3DEvent::Unadvise\n��� ��������� = ") + GetDocName() );
      kompas->ksMessage( str.GetBuffer(0) );
    }
    else */if( IsEqualIID(m_iidEvents, DIID_ksDocumentFileNotify) /*&& theApp.m_mes_DOC*/ )
    {
      CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> DocumentEvent::Unadvise\n��� ��������� = ") + GetDocName() );
      kompas->ksMessage( str.GetBuffer(0) );
    }
    // else if( IsEqualIID(m_iidEvents, DIID_ksObject2DNotify) && theApp.m_mes_OBJ_2DDOC )
    // {
    //   ksDocument2DPtr doc2D( m_doc );
    //   if( doc2D != NULL  && doc2D->ksExistObj(m_objType) ) 
    //     doc2D->ksLightObj( m_objType, true );

    //   CString strType;
    //   strType.Format( _T("\n��� �������/��������� = %i"), m_objType );
    //   CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> Object2DEvent::Unadvise\n��� ��������� = ") + GetDocName() + strType );
    //   kompas->ksMessage( str.GetBuffer(0) );

    //   if( doc2D != NULL && doc2D->ksExistObj(m_objType) ) 
    //     doc2D->ksLightObj( m_objType, false );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksObject3DNotify) && theApp.m_mes_OBJ_3DDOC )
    // {
    //   ksDocument3DPtr doc3D( m_doc );
    //   ksChooseMngPtr chooseMng;
    //   if( doc3D != NULL  && m_obj3D != NULL && 
    //       ( chooseMng = doc3D->GetChooseMng() ) != NULL )         
    //     chooseMng->Choose( m_obj3D );

    //   CString strType;
    //   strType.Format( _T("\n��� ������� = %i"), m_objType );
    //   CString strObj3DName;
    //   if( m_obj3D )
    //      strObj3DName = (LPCTSTR)m_obj3D->Getname();
    //   CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> Object3DEvent::Unadvise\n��� ��������� = ") + GetDocName() + strType + _T("\n��� 3D ������� = ") + strObj3DName );
    //   kompas->ksMessage( str.GetBuffer(0) );

    //   if( chooseMng != NULL )
    //     chooseMng->UnChoose( m_obj3D );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksSelectionMngNotify) && theApp.m_mes_SELECT )
    // {
    //   CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> SelectMngEvent::Unadvise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksSpcDocumentNotify) && theApp.m_mes_SPCDOC )
    // {
    //   CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> SpcDocumentEvent::Unadvise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksSpcObjectNotify) && theApp.m_mes_OBJ_SPC )
    // {
    //   CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> SpcObjectEvent::Unadvise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksSpecificationNotify) && theApp.m_mes_SPC )
    // {
    //   CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> SpecificationEvent::Unadvise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
    // else if( IsEqualIID(m_iidEvents, DIID_ksStampNotify) && theApp.m_mes_STAMP )
    // {
    //   CString str( _T("���������� �� ��������� �������\n") + m_libName + _T(" --> StampEvent::Unadvise\n��� ��������� = ") + GetDocName() );
    //   kompas->ksMessage( str.GetBuffer(0) );
    // }
  } 
}


//-----------------------------------------------------------------------------
// �������� ��� �������
// ---
void BaseEvent::TerminateEvents( void )
{
  INT_PTR count = m_EventList.GetCount();
  for ( INT_PTR i = 0; i < count ; i++ )
  {
    BaseEvent* headEvent = (BaseEvent*)m_EventList.RemoveHead();
    headEvent->Disconnect(); 
  }
}


//-----------------------------------------------------------------------------
// �������� ��� ������� �� GUID � ���������
// ---
void BaseEvent::TerminateEvents( IID iid, LPDISPATCH doc/*NULL*/, 
                                          long objType/*-1*/, 
                                          ksFeaturePtr obj3D/*NULL*/ )
{
  INT_PTR count = m_EventList.GetCount();
  for ( INT_PTR i = count - 1; i >= 0; i-- )
  {
    CObject* obj = m_EventList.GetAt( m_EventList.FindIndex( i ) );
    BaseEvent* event = (BaseEvent*) obj;  
    if ( event && 
         ( IsEqualIID( iid, GUID_NULL) ||
           IsEqualIID( event->m_iidEvents, iid ) ) &&
         ( !doc          || event->m_doc == doc ) &&
         ( obj3D == NULL || event->m_obj3D == obj3D ) &&
         ( objType == -1 || event->m_objType == objType ) )        
    {         
      event->Disconnect();  // � ����������� ����� ������ �� ������ RemoveAt(pos)
    }
  }
}

//-----------------------------------------------------------------------------
// ���������� ������
// ---
void BaseEvent::Clear() {
  if ( m_pContainer ) 
  {
    m_pContainer->Release();
    m_pContainer = NULL;     
  }

  if ( m_doc ) 
  {
    m_doc->Release();
    m_doc = NULL;
  }

  if ( m_obj3D ) 
  {
    m_obj3D->Release();
    m_obj3D = NULL;
  }
  m_iidEvents = GUID_NULL;
}

//-----------------------------------------------------------------------------
// �������������
// ---
void BaseEvent::Disconnect()
{
  Unadvise();
  Clear();
  ExternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
bool BaseEvent::FindEvent( IID iid, LPDISPATCH doc/*NULL*/, 
                                    long objType/*-1*/, 
                                    ksFeaturePtr obj3D/*NULL*/ ) 
{
  INT_PTR count = m_EventList.GetCount();
  for ( INT_PTR i = count - 1; i >= 0; i-- )
  {
    CObject* obj = m_EventList.GetAt(  m_EventList.FindIndex( i ) );
    BaseEvent* event = (BaseEvent*) obj;  
    if ( event && 
         ( IsEqualIID( iid, GUID_NULL) || 
           IsEqualIID( event->m_iidEvents, iid ) ) &&
         ( !doc          || event->m_doc == doc ) &&
         ( ( obj3D == NULL && !(bool)event->m_obj3D   ) || (event->m_obj3D   == obj3D  ) ) &&
         ( ( objType == -1 && !event->m_objType ) || (event->m_objType == objType) ) )         
      return true;
  }
  return false;
}


//-----------------------------------------------------------------------------
//
// ---
void BaseEvent::ListEvents()
{
  CString str( _T("����������� �������:") );

  INT_PTR count = m_EventList.GetCount();
  for ( INT_PTR i = count - 1; i >= 0; i-- )
  {
    CObject* obj = m_EventList.GetAt(  m_EventList.FindIndex( i ) );
    BaseEvent* event = (BaseEvent*) obj;     
    
    if( IsEqualIID(event->m_iidEvents, DIID_ksKompasObjectNotify) )
    { 
      str += _T("\nApplicationEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? _T("��,") : _T("���,");
      str += _T(" ��� ��������� = ") + event->GetDocName();
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksDocument2DNotify) )
    {
      str += _T("\nDocument2DEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? _T("��,") : _T("���,");
      str += " ��� ��������� = " + event->GetDocName();
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksDocument3DNotify) )
    {
      str += _T("\nDocument3DEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? _T("��,") : _T("���,");
      str += _T(" ��� ��������� = ") + event->GetDocName(); 
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksDocumentFileNotify) )
    {
      str += "\nDocumentEvent, �������� �������������: ";
      str += event->IsSelfAdvise() ? "��," : "���,";
      str += " ��� ��������� = " + event->GetDocName();
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksObject2DNotify) )
    {
      str += _T("\nObject2DEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? _T("��,") : _T("���,");
      str += _T(" ��� ��������� = ") + event->GetDocName();
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksObject3DNotify) )
    {
      str += _T("\nObject3DEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? _T("��,") : _T("���,");
      str += " ��� ��������� = " + event->GetDocName();       
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksSelectionMngNotify) )
    {
      str += "\nSelectMngEvent, �������� �������������: ";
      str += event->IsSelfAdvise() ? "��," : "���,";
      str += _T(" ��� ��������� = ") + event->GetDocName();
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksSpcDocumentNotify) )
    {
      str += _T("\nSpcDocumentEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? _T("��,") : _T("���,");
      str += _T(" ��� ��������� = ") + event->GetDocName();
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksSpcObjectNotify) )
    {
      str += _T("\nSpcObjectEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? "��," : "���,";
      str += _T(" ��� ��������� = ") + event->GetDocName();  
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksSpecificationNotify) )
    {
      str += _T("\nSpecificationEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? _T("��,") : _T("���,");
      str += _T(" ��� ��������� = ") + event->GetDocName();
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksStampNotify) )
    {
      str += _T("\nStampEvent, �������� �������������: ");
      str += event->IsSelfAdvise() ? _T("��,") : _T("���,");
      str += _T(" ��� ��������� = ") + event->GetDocName();
    }
  }
  kompas->ksMessage( str.GetBuffer(0) );
}


//-------------------------------------------------------------------------------
//
// ---
BEGIN_INTERFACE_MAP(BaseEvent, CCmdTarget)
END_INTERFACE_MAP()


//-------------------------------------------------------------------------------
//
// ---
BEGIN_EVENTSINK_MAP(BaseEvent, CCmdTarget)
END_EVENTSINK_MAP()


//-------------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) BaseEvent::XEventHandler::AddRef()
{
  METHOD_PROLOGUE( BaseEvent, EventHandler )
  return (ULONG)pThis->ExternalAddRef();
}


//-------------------------------------------------------------------------------
//
// ---
STDMETHODIMP BaseEvent::XEventHandler::GetIDsOfNames( REFIID riid, LPOLESTR* rgszNames, 
                                                      unsigned int cNames, LCID lcid, 
                                                      DISPID* rgdispid )
{
  METHOD_PROLOGUE( BaseEvent, EventHandler )
  ASSERT_VALID( pThis );

  return ResultFromScode( E_NOTIMPL );
}


//-------------------------------------------------------------------------------
//
// ---
STDMETHODIMP BaseEvent::XEventHandler::GetTypeInfo( unsigned int itinfo, LCID lcid, 
                                                    ITypeInfo** pptinfo )
{
  METHOD_PROLOGUE( BaseEvent, EventHandler )
  ASSERT_VALID( pThis );

  return ResultFromScode( E_NOTIMPL );
}


//-------------------------------------------------------------------------------
//
// ---
STDMETHODIMP BaseEvent::XEventHandler::GetTypeInfoCount( unsigned int* pctinfo )
{
  METHOD_PROLOGUE( BaseEvent, EventHandler )
  *pctinfo = 0;
  return NOERROR;
}


//-------------------------------------------------------------------------------
// ��������� �������
// ---
STDMETHODIMP BaseEvent::XEventHandler::Invoke( DISPID           dispidMember, 
                                               REFIID           riid, 
                                               LCID             lcid, 
                                               unsigned short   wFlags, 
                                               DISPPARAMS     * lpDispparams, 
                                               VARIANT        * pvarResult, 
                                               EXCEPINFO      * pexcepinfo, 
                                               unsigned int   * puArgErr )
{
  METHOD_PROLOGUE( BaseEvent, EventHandler )
  ASSERT_VALID( pThis );

  AFX_EVENT event( AFX_EVENT::event, dispidMember, lpDispparams, pexcepinfo, puArgErr );

  
  // ����������: OnEvent � ���������� ����������� ���������� FALSE, ��� ���������� ������,
  //             ���������� ���������� TRUE, ����� �� ����������� �������� ������, ������� 
  //             ����� OnEvent �������� �������� GetEventSinkEntry           
            
  BOOL eventHandled = TRUE; // ������� �� ��������������, ���������� ������� TRUE
  if (pThis->GetEventSinkEntry(1, &event) != NULL)
    eventHandled = pThis->OnEvent( 1, &event, NULL );

  if ( pvarResult != NULL ) {
    VariantClear( pvarResult );
  
    V_VT  (pvarResult) = VT_BOOL;
    V_BOOL(pvarResult) = eventHandled;
  
  }
  return event.m_hResult;
}


//-------------------------------------------------------------------------------
//
// ---
STDMETHODIMP BaseEvent::XEventHandler::QueryInterface( REFIID iid, LPVOID* ppvObj )
{
  METHOD_PROLOGUE( BaseEvent, EventHandler )

  *ppvObj = NULL;
  if ( IID_IUnknown == iid || IID_IDispatch == iid || iid == pThis->m_iidEvents )
    *ppvObj = this;

  if ( NULL != *ppvObj )
  {
    ((LPUNKNOWN)*ppvObj)->AddRef();
    return NOERROR;
  }

  return (HRESULT)pThis->ExternalQueryInterface( &iid, ppvObj );
}


//-------------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) BaseEvent::XEventHandler::Release()
{
  METHOD_PROLOGUE( BaseEvent, EventHandler )
  return (ULONG)pThis->ExternalRelease();
}


