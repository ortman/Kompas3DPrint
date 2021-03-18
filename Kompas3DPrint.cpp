
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
// Специальная структура используемая в течении инициализации DLL
// ---
static AFX_EXTENSION_MODULE Step2_API7_3DDLL = { NULL, NULL };
HINSTANCE g_hInstance = NULL;
KompasObjectPtr kompas = NULL;        

void OnProcessDetach();                 // Отключение библиотеки

// Глобальные переменные
int oType = o3d_edge;										// Тип объектов для функции фильтрации


//-------------------------------------------------------------------------------
// Стандартная точка входа
// Инициализация и завершение DLL
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
// Получить доступ к новому API
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
      
//       HINSTANCE hAppAuto = LoadLibrary( filename ); // идентификатор kAPI7.dll
      
//       if(  hAppAuto ) 
//       {
//         // Указатель на функцию возвращающую интерфейс KompasApplication  
//         typedef LPDISPATCH ( WINAPI * FCreateKompasApplication )(); 
        
//         FCreateKompasApplication pCreateKompasApplication = 
//           (FCreateKompasApplication)GetProcAddress( hAppAuto, "CreateKompasApplication" );	
        
//         if ( pCreateKompasApplication )
//           newKompasAPI = IDispatchPtr ( pCreateKompasApplication(), false /*AddRef*/); // Получаем интерфейс Application
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
      
      // Идентификатор appauto.dll
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
// Задать идентификатор ресурсов
// ---
unsigned int WINAPI LIBRARYID()
{
  return IDR_LIBID;
}


// //-------------------------------------------------------------------------------
// // Получить текущий документ
// // ---
// KompasAPI7::IKompasDocument3DPtr GetCurrentDocument()
// {
// 	KompasAPI7::IKompasDocument3DPtr doc = NULL;
	
// 	// Если не получен интерфейс приложения IApplication в СОМ
//   if ( !(bool)newKompasAPI ) {
// 		// Получить интерфейс приложения 
//     GetNewKompasAPI();
// 	}
	
// 	// Если получен интерфейс приложения IApplication в СОМ
//   if ( newKompasAPI ) {
// 		// Получить активный документ
//     doc = newKompasAPI->GetActiveDocument();
// 	}
	
// 	return doc;
// }


//-------------------------------------------------------------------------------
// Описания используемых функций
// ---

//-------------------------------------------------------------------------------
// Головная функция библиотеки
// ---
void WINAPI LIBRARYENTRY( unsigned int comm )
{
	// Получить текущий документ
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
    // Интерфейс итератора
    ksIteratorPtr iter = kompas->GetIterator();       

    // Создаем итератор для хождения по документам
    if ( iter != NULL && iter->ksCreateIterator(ALL_DOCUMENTS, 0) ) 
    {
      reference refDoc = iter->ksMoveIterator( "F" ); // Первый документ
      while ( refDoc )
      {
        // Подписка на события документа
        AdviseDoc( kompas->ksGetDocumentByReference(refDoc), 
                   kompas->ksGetDocumentType( refDoc ) );

        // Cледующий документ
        refDoc = iter->ksMoveIterator( "N" );           
      }
      
      // Удалить итератор
      iter->ksDeleteIterator();                       
    }
  }
}

//-----------------------------------------------------------------------------
// Провести подписку библиотеки на сообщения системы
// ---
bool WINAPI LibInterfaceNotifyEntry( IDispatch *application ) 
{
  // AFX_MANAGE_STATE(AfxGetStaticModuleState());

  // Захват интерфейса приложения КОМПАС
  if (kompas == NULL && application) {
    kompas = application;
    kompas.AddRef();
  }

  if (kompas) {
    // Обработчик событий приложения КОМПАС
    ApplicationEvent * aplEvent = new ApplicationEvent( kompas );        
    // Подписка на события приложения КОМПАС
    aplEvent->Advise(); 
  
    AdviseDocuments();
  }

  return false;
}

//-------------------------------------------------------------------------------
// Получить контейнер обозначений 3D
// ---
KompasAPI7::ISymbols3DContainerPtr GetSymbols3DContainer( KompasAPI7::IKompasDocument3DPtr & doc )
{
  if ( doc )
  {
    // Получаем контейнер обозначений 3D
    return doc->GetTopPart(); 
  }
  return NULL;
}


//-----------------------------------------------------------------------------
// Функция фильтрации
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
		// Преобразовать интерфейс документа 3D из API7 в API5
		IDocument3DPtr doc3D( IUnknownPtr(ksTransferInterface( doc, ksAPI3DCom, 0/*любой документ*/ ), false/*AddRef*/) );
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
// Завершение приложения
// ---
void OnProcessDetach()
{
  // newKompasAPI = NULL;
  kompas = NULL;
}


//------------------------------------------------------------------------------
// Загрузить строку из ресурса
// ---
CString LoadStr( int strID ) 
{
	TCHAR temp[_MAX_PATH];
	LoadString( Step2_API7_3DDLL.hModule, strID, temp, _MAX_PATH ); 
	return temp;
}


//-----------------------------------------------------------------------------
// Подписка на события документа
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
  // События документа, необходимы для своевременной отписки
  if ( !BaseEvent::FindEvent( DIID_ksDocumentFileNotify, doc ) ) 
  {
    // kompas->ksError( "Hi2" );
    bool fFileDoc = !fSelectMng && !fObject && !fStamp && !fDocument && !fSpecification && !fSpcObject;

    // Обработчик событий от документа
    DocumentEvent * docEvent = new DocumentEvent( doc, fFileDoc ); 
    // Подписка на события документа
    int advise = docEvent->Advise(); 
    
    // Неудачная подписка на события документа
    if ( !advise )
      return;
      
    // kompas->ksError( "Hi3" );
  }
  else
    kompas->ksError( "На события документа уже подписались" );

  switch ( docType ) 
  {
    case lt_DocPart3D        : // 5 - 3d-документ модель
    case lt_DocAssemble3D    : // 6 - 3d-документ сборка
    {
      break; 
    }
  }
}
