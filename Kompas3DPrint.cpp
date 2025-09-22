
#include "Kompas3DPrint.h"
#include <afxdllx.h>
#include <libtool.h>
#include "DocumentEvent.h"
#include "ApplicationEvent.h"
#include "SettingsDlg.h"
#include "AboutDlg.h"

//-------------------------------------------------------------------------------
// ����������� ��������� ������������ � ������� ������������� DLL
// ---
static AFX_EXTENSION_MODULE Kompas3DPrint_API7_3DDLL = {NULL, NULL};
HINSTANCE g_hInstance = NULL;
KompasObjectPtr kompas = NULL;
ApplicationEvent *aplEvent = NULL;
SettingsData userSettings;

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
    userSettings.readDefaultSettings(kompas);
  }
  return kompas;
}

//-------------------------------------------------------------------------------
// ������ ������������� ��������
// ---
unsigned int WINAPI LIBRARYID() {
  return IDR_LIBID;
}

CString getFileDlgFilter(D3FormatConvType format) {
  switch (format) {
    case format_STEP:
    case format_STEP_AP203:
    case format_STEP_AP214:
    case format_STEP_AP242: return _T("STEP Models (*.stp)|*.stp|");
    case format_IGES: return _T("IGES Models (*.igs)|*.igs|");
    case format_SAT: return _T("SAT Models (*.sat)|*.sat|");
    case format_XT: return userSettings.formatBIN ? _T("Parasolid Models (*.x_b)|*.x_b|") : _T("Parasolid Models (*.x_t)|*.x_t|");
    case format_VRML: return _T("VRLM Models (*.wrl)|*.wrl|");
    default: return _T("STL Models (*.stl)|*.stl|");
  }
}

bool CommandSaveAs(D3FormatConvType format) {
  CString m3dPath = BaseEvent::GetDocName(kompas->ActiveDocument3D());
  if (m3dPath == "") m3dPath = "������.m3d";
  CFileDialog fileDlg(FALSE, NULL, m3dPath.Mid(0, m3dPath.GetLength() - 4) + SettingsData::getExt(format), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, getFileDlgFilter(format));
  if (fileDlg.DoModal() == IDOK) {
    if (Save(kompas->ActiveDocument3D(), format, fileDlg.GetPathName())) return true;
    Message("�� ���� �������������� ������.");
  }
  return false;
}

//-------------------------------------------------------------------------------
// �������� ������� ����������
// ---
void WINAPI LIBRARYENTRY(unsigned int comm) {
	// �������� ������� ��������
  ::GetKompas();
  if (kompas) {
    switch (comm) {
      case MENU_SETTINGS: {
        TSettingsDlg *dlg = new TSettingsDlg();
        if (dlg) {
			    EnableTaskAccess(0);        // ������� ������ � �������
          dlg->DoModal();
			    EnableTaskAccess(1);        // ������� ������ � �������
          delete dlg;
          userSettings.read();
        }
        break;
      }
      case MENU_EXPORT_STL: 
        CommandSaveAs(format_STL);
        break;
      case MENU_EXPORT_STEP:
        CommandSaveAs(format_STEP_AP214);
        break;
      case MENU_EXPORT_IGS:
        CommandSaveAs(format_IGES);
        break;
      case MENU_EXPORT_ACIS:
        CommandSaveAs(format_SAT);
        break;
      case MENU_EXPORT_X_T:
        CommandSaveAs(format_XT);
        break;
      case MENU_EXPORT_VRLM:
        CommandSaveAs(format_VRML);
        break;
      case MENU_OPEN_CURA: {
        CString tempStl = GetTmpSTLPath();
        if (Save(kompas->ActiveDocument3D(), format_STL, tempStl)) {
          TCHAR szPath[] = _T("");
          SHELLEXECUTEINFO ExecInfo;
          memset(&ExecInfo, 0, sizeof(SHELLEXECUTEINFO));
          ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
          ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
          ExecInfo.lpVerb = _T("open"); 
          ExecInfo.lpFile = userSettings.curaPath;
          ExecInfo.lpParameters = "\"" + tempStl + "\"";
          ShellExecuteEx(&ExecInfo);
        }
        break;
      }
      case MENU_ABOUT: {
        TAboutDlg *dlg = new TAboutDlg();
        if (dlg) {
			    EnableTaskAccess(0);        // ������� ������ � �������
          dlg->DoModal();
			    EnableTaskAccess(1);        // ������� ������ � �������
          delete dlg;
        }
        break;
      }
    }
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
    userSettings.readDefaultSettings(kompas);
  }

  if (kompas && aplEvent == NULL) {
    aplEvent = new ApplicationEvent( kompas );
    aplEvent->Advise();
    AdviseDocuments();
  }

  return false;
}

CString GetTmpSTLPath() {
  wchar_t wcharPath[MAX_PATH];
  if (kompas && GetTempPath(MAX_PATH, wcharPath)) {
    CString m3dPath = BaseEvent::GetDocName(kompas->ActiveDocument3D());
    int pathLen = m3dPath.GetLength();
    if (pathLen < 5) return CString(wcharPath) + "������.stl";
    char slash = '\\';
    int slashPos = m3dPath.ReverseFind(slash);
    if (slashPos < 0) {
      slash = '/';
      slashPos = m3dPath.ReverseFind(slash);
    }
    if (slashPos > 0) {
      return CString(wcharPath) + m3dPath.Mid(slashPos + 1, pathLen - slashPos - 5) + ".stl";
    }
  }
  return "";
}


bool Save(LPDISPATCH doc3d, D3FormatConvType format, CString &path) {
	if (!doc3d) return false;
  ksDocument3DPtr doc;
  doc3d->QueryInterface(DIID_ksDocument3D, (LPVOID*)&doc);
	if (!doc) return false;
  ksAdditionFormatParamPtr formatParam = doc->AdditionFormatParam();
  formatParam->Init();
  formatParam->SetObjectsOptions(ksD3COBodyes, userSettings.objBody);
  formatParam->SetObjectsOptions(ksD3COSurfaces, userSettings.objSurface);
  formatParam->format = format;
  formatParam->topolgyIncluded = false;
  formatParam->lengthUnits = userSettings.units;
  /// TODO: ������-�� ��� ��������� formatBinary=true, ����������� � ���������. �_�?! 
  // false - ��������, ��� ������������ ������������ kompas API, ������� ���������� ��������
  formatParam->formatBinary = !userSettings.formatBIN;
  long stepType = 0;
  if (userSettings.isLinear) {
    stepType |= ksSpaceStep;
    formatParam->step = userSettings.linearVal;
  } else {
    formatParam->step = SETTINGS_LINEAR_MAX;
  }
  if (userSettings.isAngle) {
    stepType |= ksDeviationStep;
    formatParam->angle = userSettings.angleVal * M_PI / 180.0;
  } else {
    formatParam->angle = SETTINGS_ANGLE_MAX * M_PI / 180.0;
  }
  if (userSettings.isRidge) {
    stepType |= ksMetricStep;
    formatParam->length = userSettings.ridgeVal;
  } else {
    formatParam->length = SETTINGS_RIDGE_MAX;
  }
  formatParam->stepType = stepType;
  return doc->SaveAsToAdditionFormat(path.GetBuffer(0), formatParam);
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
void AdviseDoc(LPDISPATCH doc, long docType) { 
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
