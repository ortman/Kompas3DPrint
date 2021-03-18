
#include "Kompas3DPrint.h"
#include <afxdllx.h>
#include "LIBTOOL.H"
#include "DocumentEvent.h"
#include "ApplicationEvent.h"

//-------------------------------------------------------------------------------
// ����������� ��������� ������������ � ������� ������������� DLL
// ---
static AFX_EXTENSION_MODULE Kompas3DPrint_API7_3DDLL = {NULL, NULL};
HINSTANCE g_hInstance = NULL;
KompasObjectPtr kompas = NULL;
ApplicationEvent *aplEvent = NULL;

void OnProcessDetach();                 // ���������� ����������

// ���������� ����������
int oType = o3d_edge;										// ��� �������� ��� ������� ����������


//-------------------------------------------------------------------------------
// ����������� ����� �����
// ������������� � ���������� DLL
// ---
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
  g_hInstance = hInstance;
	if (dwReason == DLL_PROCESS_ATTACH) {
		TRACE0("KOMPAS3DPRINT_API7_3D.AWX Initializing!\n");
		AfxInitExtensionModule(Kompas3DPrint_API7_3DDLL, hInstance);
		new CDynLinkLibrary(Kompas3DPrint_API7_3DDLL);
	}	else if (dwReason == DLL_PROCESS_DETACH) {
		TRACE0("KOMPAS3DPRINT_API7_3D.AWX Terminating!\n");
    OnProcessDetach();
		AfxTermExtensionModule(Kompas3DPrint_API7_3DDLL);
	}
	return 1;   // ok
}

KompasObjectPtr GetKompas() {
  if (!kompas) {
    CString filename;
    if (::GetModuleFileName(NULL, filename.GetBuffer(255), 255)) {
      filename.ReleaseBuffer(255);
      CString libname;

      libname.LoadString(IDS_DLL_NAME);  // kAPI5.dll / klAPI5.dll
      
      filename.Replace(filename.Right(filename.GetLength() - (filename.ReverseFind('\\') + 1)), libname);
      
      // ������������� appauto.dll
      HINSTANCE hAppAuto = ::LoadLibrary(filename); 
      if (hAppAuto) {
        typedef LPDISPATCH (WINAPI *FCreateKompasObject)(); 
        FCreateKompasObject pCreateKompasObject = 
            (FCreateKompasObject)::GetProcAddress(hAppAuto, "CreateKompasObject");  
        if (pCreateKompasObject) {
          kompas = IDispatchPtr(pCreateKompasObject(), false); 
        }
        ::FreeLibrary(hAppAuto);  
      }
    }
  }
  return kompas;
}

//-------------------------------------------------------------------------------
// ������ ������������� ��������
// ---
unsigned int WINAPI LIBRARYID() {
  return IDR_LIBID;
}

//-------------------------------------------------------------------------------
// �������� ������� ����������
// ---
void WINAPI LIBRARYENTRY(unsigned int comm) {
	// �������� ������� ��������
  ::GetKompas();
	
	if (kompas) {
    ksDocument3DPtr doc3D( kompas->ActiveDocument3D() );
    if (doc3D) {
      // switch (comm) {
        // case 1: Save2STL( doc3D );										break;
      // }
    } else {
		  MessageT((LPTSTR)(LPCTSTR)LoadStr(IDS_NOACTIVEDOC));
    }
	} else {
		MessageT((LPTSTR)(LPCTSTR)LoadStr(IDS_NODOC));
  }
}

void AdviseDocuments() {
  if (kompas) {
    // ��������� ���������
    ksIteratorPtr iter = kompas->GetIterator();       

    // ������� �������� ��� �������� �� ����������
    if (iter != NULL && iter->ksCreateIterator(ALL_DOCUMENTS, 0)) {
      reference refDoc = iter->ksMoveIterator("F"); // ������ ��������
      while (refDoc) {
        // �������� �� ������� ���������
        AdviseDoc(kompas->ksGetDocumentByReference(refDoc), 
                  kompas->ksGetDocumentType( refDoc ));

        // C�������� ��������
        refDoc = iter->ksMoveIterator("N");           
      }
      
      // ������� ��������
      iter->ksDeleteIterator();                       
    }
  }
}

//-----------------------------------------------------------------------------
// �������� �������� ���������� �� ��������� �������
// ---
bool WINAPI LibInterfaceNotifyEntry(IDispatch *application) {
  // AFX_MANAGE_STATE(AfxGetStaticModuleState());

  // ������ ���������� ���������� ������
  if (kompas == NULL && application) {
    kompas = application;
    kompas.AddRef();
  }

  if (kompas && aplEvent == NULL) {
    aplEvent = new ApplicationEvent( kompas );
    aplEvent->Advise();
    AdviseDocuments();
  }

  return false;
}

//-------------------------------------------------------------------------------
// �������� ��������� ����������� 3D
// ---
KompasAPI7::ISymbols3DContainerPtr GetSymbols3DContainer(KompasAPI7::IKompasDocument3DPtr & doc) {
  if (doc) return doc->GetTopPart();
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
	if (doc) {
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
	}
}

//-------------------------------------------------------------------------------
// ���������� ����������
// ---
void OnProcessDetach() {
  if (aplEvent != NULL) {
    aplEvent->Unadvise();
    delete aplEvent;
    aplEvent = NULL;
  }
  kompas = NULL;
}

//------------------------------------------------------------------------------
// ��������� ������ �� �������
// ---
CString LoadStr( int strID ) {
	TCHAR temp[_MAX_PATH];
	LoadString(Kompas3DPrint_API7_3DDLL.hModule, strID, temp, _MAX_PATH); 
	return temp;
}

//-----------------------------------------------------------------------------
// �������� �� ������� ���������
// ---
void AdviseDoc(LPDISPATCH doc, long docType, 
               bool fSelectMng /*true*/, 
               bool fObject /*true*/, 
               bool fStamp /*true*/,
               bool fDocument/*true*/,
               bool fSpecification/*true*/,
               bool fSpcObject/*true*/,
               long objType/*-1*/) { 
  if (!doc) return;
  // ������� ���������, ���������� ��� ������������� �������
  if (!BaseEvent::FindEvent(DIID_ksDocumentFileNotify, doc)) {
    // ���������� ������� �� ���������
    DocumentEvent * docEvent = new DocumentEvent(doc); 
    // �������� �� ������� ���������
    int advise = docEvent->Advise(); 
    
    // ��������� �������� �� ������� ���������
    if (!advise) return;
  } else kompas->ksError( "�� ������� ��������� ��� �����������" );

  switch (docType) {
    case lt_DocPart3D:      // 5 - 3d-�������� ������
    case lt_DocAssemble3D:  // 6 - 3d-�������� ������
      break; 
  }
}
