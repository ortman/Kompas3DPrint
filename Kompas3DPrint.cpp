
#include "stdafx.h"
#include <afxdllx.h>
#include "LIBTOOL.H"
#include "Kompas3DPrint.h"
#include "DocumentEvent.h"
#include "ApplicationEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-------------------------------------------------------------------------------
// ����������� ��������� ������������ � ������� ������������� DLL
// ---
static AFX_EXTENSION_MODULE Step2_API7_3DDLL = { NULL, NULL };
HINSTANCE g_hInstance = NULL;
KompasObjectPtr kompas = NULL;        

void OnProcessDetach();                 // ���������� ����������

// ���������� ����������
int oType = o3d_edge;										// ��� �������� ��� ������� ����������


//-------------------------------------------------------------------------------
// ����������� ����� �����
// ������������� � ���������� DLL
// ---
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  g_hInstance = hInstance;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("STEP2_API7_3D.AWX Initializing!\n");

		AfxInitExtensionModule(Step2_API7_3DDLL, hInstance);

		new CDynLinkLibrary(Step2_API7_3DDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("STEP2_API7_3D.AWX Terminating!\n");
    OnProcessDetach();
		AfxTermExtensionModule(Step2_API7_3DDLL);
	}
	return 1;   // ok
}


//-------------------------------------------------------------------------------
// �������� ������ � ������ API
// ---
// void GetNewKompasAPI() 
// {
//   if ( !( KompasAPI7::IApplication * )newKompasAPI ) 
//   {
//     CString filename;
    
//     if( ::GetModuleFileName(NULL, filename.GetBuffer(255), 255) ) 
//     {
//       filename.ReleaseBuffer( 255 );
//       CString libname;
      
//       libname = LoadStr( IDR_API7 );    // kAPI7.dll
//       filename.Replace( filename.Right(filename.GetLength() - (filename.ReverseFind('\\') + 1)), 
//         libname );
      
//       HINSTANCE hAppAuto = LoadLibrary( filename ); // ������������� kAPI7.dll
      
//       if(  hAppAuto ) 
//       {
//         // ��������� �� ������� ������������ ��������� KompasApplication  
//         typedef LPDISPATCH ( WINAPI * FCreateKompasApplication )(); 
        
//         FCreateKompasApplication pCreateKompasApplication = 
//           (FCreateKompasApplication)GetProcAddress( hAppAuto, "CreateKompasApplication" );	
        
//         if ( pCreateKompasApplication )
//           newKompasAPI = IDispatchPtr ( pCreateKompasApplication(), false /*AddRef*/); // �������� ��������� Application
//         FreeLibrary( hAppAuto );  
//       }
//     }
//   }
// }

KompasObjectPtr GetKompas() 
{
  if ( !(bool)kompas ) 
  {
    CString filename;
    if ( ::GetModuleFileName(NULL, filename.GetBuffer(255), 255) ) 
    {
      filename.ReleaseBuffer( 255 );
      CString libname;

      libname.LoadString( IDS_DLL_NAME );  // kAPI5.dll / klAPI5.dll
      

      filename.Replace( filename.Right(filename.GetLength() - (filename.ReverseFind('\\') + 1)), 
        libname );
      
      // ������������� appauto.dll
      HINSTANCE hAppAuto = ::LoadLibrary( filename ); 
      if ( hAppAuto ) 
      {
        typedef LPDISPATCH ( WINAPI *FCreateKompasObject )(); 
        FCreateKompasObject pCreateKompasObject = 
          (FCreateKompasObject)::GetProcAddress( hAppAuto, "CreateKompasObject" );  
        if ( pCreateKompasObject ) 
          kompas = IDispatchPtr( pCreateKompasObject(), false ); 
        ::FreeLibrary( hAppAuto );  
      }
    }
  }
  return kompas;
}

//-------------------------------------------------------------------------------
// ������ ������������� ��������
// ---
unsigned int WINAPI LIBRARYID()
{
  return IDR_LIBID;
}


// //-------------------------------------------------------------------------------
// // �������� ������� ��������
// // ---
// KompasAPI7::IKompasDocument3DPtr GetCurrentDocument()
// {
// 	KompasAPI7::IKompasDocument3DPtr doc = NULL;
	
// 	// ���� �� ������� ��������� ���������� IApplication � ���
//   if ( !(bool)newKompasAPI ) {
// 		// �������� ��������� ���������� 
//     GetNewKompasAPI();
// 	}
	
// 	// ���� ������� ��������� ���������� IApplication � ���
//   if ( newKompasAPI ) {
// 		// �������� �������� ��������
//     doc = newKompasAPI->GetActiveDocument();
// 	}
	
// 	return doc;
// }


//-------------------------------------------------------------------------------
// �������� ������������ �������
// ---

//-------------------------------------------------------------------------------
// �������� ������� ����������
// ---
void WINAPI LIBRARYENTRY( unsigned int comm )
{
	// �������� ������� ��������
  ::GetKompas();
	
	if (kompas) {
	  // KompasAPI7::IKompasDocument3DPtr doc( GetCurrentDocument() );
    ksDocument3DPtr doc3D( kompas->ActiveDocument3D() );
    if (doc3D) {
      // switch (comm) {
        // case 1: Save2STL( doc3D );										break;
      // }
    } else {
		  MessageT( (LPTSTR)(LPCTSTR)LoadStr(IDS_NOACTIVEDOC) );
    }
	}
	else
		MessageT( (LPTSTR)(LPCTSTR)LoadStr(IDS_NODOC) );
}

void AdviseDocuments() {
  if ( kompas ) 
  {
    // ��������� ���������
    ksIteratorPtr iter = kompas->GetIterator();       

    // ������� �������� ��� �������� �� ����������
    if ( iter != NULL && iter->ksCreateIterator(ALL_DOCUMENTS, 0) ) 
    {
      reference refDoc = iter->ksMoveIterator( "F" ); // ������ ��������
      while ( refDoc )
      {
        // �������� �� ������� ���������
        AdviseDoc( kompas->ksGetDocumentByReference(refDoc), 
                   kompas->ksGetDocumentType( refDoc ) );

        // C�������� ��������
        refDoc = iter->ksMoveIterator( "N" );           
      }
      
      // ������� ��������
      iter->ksDeleteIterator();                       
    }
  }
}

//-----------------------------------------------------------------------------
// �������� �������� ���������� �� ��������� �������
// ---
bool WINAPI LibInterfaceNotifyEntry( IDispatch *application ) 
{
  // AFX_MANAGE_STATE(AfxGetStaticModuleState());

  // ������ ���������� ���������� ������
  if (kompas == NULL && application) {
    kompas = application;
    kompas.AddRef();
  }

  if (kompas) {
    // ���������� ������� ���������� ������
    ApplicationEvent * aplEvent = new ApplicationEvent( kompas );        
    // �������� �� ������� ���������� ������
    aplEvent->Advise(); 
  
    AdviseDocuments();
  }

  return false;
}

//-------------------------------------------------------------------------------
// �������� ��������� ����������� 3D
// ---
KompasAPI7::ISymbols3DContainerPtr GetSymbols3DContainer( KompasAPI7::IKompasDocument3DPtr & doc )
{
  if ( doc )
  {
    // �������� ��������� ����������� 3D
    return doc->GetTopPart(); 
  }
  return NULL;
}


//-----------------------------------------------------------------------------
// ������� ����������
// ---
BOOL __stdcall  UserFilterProc( IEntity * e)
{
  if( e && (!oType || e->GetType() == oType) )
    return TRUE;
  else
    return FALSE;
}

void Save2STL( ksDocument3DPtr doc, BSTR stlPath ) {
	// if (doc) {
		// ������������� ��������� ��������� 3D �� API7 � API5
		IDocument3DPtr doc3D( IUnknownPtr(ksTransferInterface( doc, ksAPI3DCom, 0/*����� ��������*/ ), false/*AddRef*/) );
		if (doc3D) {
			IAdditionFormatParam* formatParam = doc3D->AdditionFormatParam();
			formatParam->Init();
			formatParam->SetFormat(format_STL);
			formatParam->SetTopolgyIncluded(false);
			formatParam->SetFormatBinary(false);
			formatParam->SetStep(0.001);
			doc3D->SaveAsToAdditionFormat(stlPath, formatParam);
		}
	// }
}

//-------------------------------------------------------------------------------
// ���������� ����������
// ---
void OnProcessDetach()
{
  // newKompasAPI = NULL;
  kompas = NULL;
}


//------------------------------------------------------------------------------
// ��������� ������ �� �������
// ---
CString LoadStr( int strID ) 
{
	TCHAR temp[_MAX_PATH];
	LoadString( Step2_API7_3DDLL.hModule, strID, temp, _MAX_PATH ); 
	return temp;
}


//-----------------------------------------------------------------------------
// �������� �� ������� ���������
// ---
void AdviseDoc( LPDISPATCH doc, long docType, 
                bool fSelectMng /*true*/, 
                bool fObject /*true*/, 
                bool fStamp /*true*/,
                bool fDocument/*true*/,
                bool fSpecification/*true*/,
                bool fSpcObject/*true*/,
                long objType/*-1*/) 
{ 
  if ( !doc )
    return;
    // kompas->ksError( "Hi1" );
  // ������� ���������, ���������� ��� ������������� �������
  if ( !BaseEvent::FindEvent( DIID_ksDocumentFileNotify, doc ) ) 
  {
    // kompas->ksError( "Hi2" );
    bool fFileDoc = !fSelectMng && !fObject && !fStamp && !fDocument && !fSpecification && !fSpcObject;

    // ���������� ������� �� ���������
    DocumentEvent * docEvent = new DocumentEvent( doc, fFileDoc ); 
    // �������� �� ������� ���������
    int advise = docEvent->Advise(); 
    
    // ��������� �������� �� ������� ���������
    if ( !advise )
      return;
      
    // kompas->ksError( "Hi3" );
  }
  else
    kompas->ksError( "�� ������� ��������� ��� �����������" );

  switch ( docType ) 
  {
    case lt_DocPart3D        : // 5 - 3d-�������� ������
    case lt_DocAssemble3D    : // 6 - 3d-�������� ������
    {
      break; 
    }
  }
}
