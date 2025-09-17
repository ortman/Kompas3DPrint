/////////////////////////////////////////////////////////////////////////////
//
// ������� ����� ��� ������������ �������
//
/////////////////////////////////////////////////////////////////////////////

#include "baseEvent.h"

#include "Kompas3DPrint.h"
 
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
BaseEvent::BaseEvent(LPUNKNOWN pObject, 
                     IID iidEvents,  
                     LPDISPATCH doc, /*NULL*/ 
                     long objType, /*-1*/ 
                     ksFeaturePtr obj3D /*NULL*/):
            CCmdTarget(),
            m_dwCookie(0),
            m_pContainer(pObject),  // �������� �������
            m_iidEvents(iidEvents),
            m_doc(doc),             // ��������
            m_objType(objType),     // ��� �������
            m_obj3D(obj3D),         // ������ ��� 3D
            m_pConnPt(NULL) {
  m_libName.LoadString(IDR_LIBID);
  m_EventList.AddTail(this);
  if (m_pContainer) m_pContainer->AddRef();
  if (m_doc) m_doc->AddRef();
  if (m_obj3D) m_obj3D->AddRef();
  ASSERT( !IsEqualIID(m_iidEvents, GUID_NULL) ); // �������� ��� �������
}

//-------------------------------------------------------------------------------
//
// ---
BaseEvent::~BaseEvent() {
  // ������ ���� �� ������ ������������ �������
  POSITION pos = m_EventList.Find(this);
  if (pos) {
    m_EventList.RemoveAt(pos);
  }

  Unadvise(); // ��������� �� ��������� �������
  if (m_pContainer) {
    m_pContainer->Release();
    m_pContainer = NULL;     
  }

  if (m_doc) {
    m_doc->Release();
    m_doc = NULL;
  }

  if (m_obj3D) {
    m_obj3D->Release();
    m_obj3D = NULL;
  }
}


//-------------------------------------------------------------------------------
// �������� ��� ���������
// ---
CString BaseEvent::GetDocName(LPDISPATCH doc) {
  _bstr_t fileName;
  if (doc) { 
    ksDocument2DPtr doc2D;
    doc->QueryInterface(DIID_ksDocument2D, (LPVOID*)&doc2D);
    if (doc2D) {
      ksDocumentParamPtr docPar(kompas->GetParamStruct(ko_DocumentParam));
      doc2D->ksGetObjParam(doc2D->Getreference(), docPar, ALLPARAM);
      fileName = docPar->fileName;
    } else {
      ksDocument3DPtr doc3D;
      doc->QueryInterface(DIID_ksDocument3D, (LPVOID*)&doc3D);
      if (doc3D) {  
        fileName = doc3D->fileName; 
      } else {
        ksSpcDocumentPtr docSpc(doc);
        doc->QueryInterface(DIID_ksSpcDocument, (LPVOID*)&docSpc);
        if (docSpc) {   
          ksDocumentParamPtr docPar(kompas->GetParamStruct(ko_DocumentParam));
          docSpc->ksGetObjParam(docSpc->Getreference(), docPar, ALLPARAM);
          fileName = docPar->fileName;
        } else {
          ksDocumentTxtPtr docTxt(doc);
          doc->QueryInterface(DIID_ksDocumentTxt, (LPVOID*)&docTxt);
          if (docTxt) {   
            ksTextDocumentParamPtr docPar(kompas->GetParamStruct(ko_TextDocumentParam));
            docTxt->ksGetObjParam(docTxt->Getreference(), docPar, ALLPARAM);
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
int BaseEvent::Advise() {
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

  return m_dwCookie;
}


//-------------------------------------------------------------------------------
// ���������� �� ��������� �������
// ---
void BaseEvent::Unadvise() {
  if ( m_pConnPt != NULL )              // �������� ����
  {
    m_pConnPt->Unadvise( m_dwCookie );  // ���������� �� ��������� �������
    m_pConnPt->Release();                // ���������� 
    m_pConnPt  = NULL;
  } 
  m_dwCookie = 0;
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
      str += _T("\nApplicationEvent,");
      str += _T(" ��� ��������� = ") + GetDocName(event->m_doc);
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksDocument2DNotify) )
    {
      str += _T("\nDocument2DEvent,");
      str += " ��� ��������� = " + GetDocName(event->m_doc);
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksDocument3DNotify) )
    {
      str += _T("\nDocument3DEvent,");
      str += _T(" ��� ��������� = ") + GetDocName(event->m_doc); 
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksDocumentFileNotify) )
    {
      str += "\nDocumentEvent,";
      str += " ��� ��������� = " + GetDocName(event->m_doc);
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksObject2DNotify) )
    {
      str += _T("\nObject2DEvent,");
      str += _T(" ��� ��������� = ") + GetDocName(event->m_doc);
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksObject3DNotify) )
    {
      str += _T("\nObject3DEvent,");
      str += " ��� ��������� = " + GetDocName(event->m_doc);       
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksSelectionMngNotify) )
    {
      str += "\nSelectMngEvent,";
      str += _T(" ��� ��������� = ") + GetDocName(event->m_doc);
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksSpcDocumentNotify) )
    {
      str += _T("\nSpcDocumentEvent,");
      str += _T(" ��� ��������� = ") + GetDocName(event->m_doc);
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksSpcObjectNotify) )
    {
      str += _T("\nSpcObjectEvent,");
      str += _T(" ��� ��������� = ") + GetDocName(event->m_doc);  
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksSpecificationNotify) )
    {
      str += _T("\nSpecificationEvent,");
      str += _T(" ��� ��������� = ") + GetDocName(event->m_doc);
    }
    else if( IsEqualIID(event->m_iidEvents, DIID_ksStampNotify) )
    {
      str += _T("\nStampEvent,");
      str += _T(" ��� ��������� = ") + GetDocName(event->m_doc);
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


