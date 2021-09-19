////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DocumentEvent.h"
#include "Kompas3DPrint.h"
#include "SettingsData.h"

extern SettingsData userSettings;

DocumentEvent::DocumentEvent(LPDISPATCH doc)
  : BaseEvent(doc, DIID_ksDocumentFileNotify, doc, -1, NULL) {
}

DocumentEvent::~DocumentEvent() {
}

BEGIN_EVENTSINK_MAP(DocumentEvent, BaseEvent)
  ON_EVENT (DocumentEvent, (unsigned int)-1, kdSaveDocument,        SaveDocument,        VTS_NONE)
  ON_EVENT (DocumentEvent, (unsigned int)-1, kdBeginCloseDocument,  BeginCloseDocument,  VTS_NONE)
END_EVENTSINK_MAP()

afx_msg VARIANT_BOOL DocumentEvent::BeginCloseDocument() {
  Unadvise();
  return true;
}

BOOL DirectoryExists(LPCTSTR szPath) {
  DWORD dwAttrib = GetFileAttributes(szPath);
  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

afx_msg VARIANT_BOOL DocumentEvent::SaveDocument() {
  CString stlPath = _T("");
  CString path = GetDocName();
  int pathLen = path.GetLength();
  if (pathLen < 5) return true;;
  if (path.Right(4) != _T(".m3d")) return true;
  int slash = path.ReverseFind('\\');
  if (slash < 0) slash = path.ReverseFind('/');
  if (slash > 0) {
    // CString folderPath = 
    CString STLfolder = path.Left(slash + 1) + _T("STL");
    BOOL existSTLFolder = DirectoryExists(STLfolder);
    if (!existSTLFolder) {
      STLfolder = path.Left(slash + 1) + _T("stl");
      existSTLFolder = DirectoryExists(STLfolder);
    }
    if (existSTLFolder) {

      stlPath = STLfolder + '\\' + path.Mid(slash + 1, pathLen - slash - 5) + _T(".stl");
    } else {
      stlPath = path.Left(pathLen-4) + _T(".stl");
    }
  } else {
    stlPath = path.Left(pathLen-4) + _T(".stl");
  }

  if (m_doc) {
    ksDocument3DPtr doc3D;
    m_doc->QueryInterface(DIID_ksDocument3D, (LPVOID*)&doc3D);
    if (doc3D && userSettings.autoexportEn) {
      CStdioFile file;
      CFileStatus status;
      if (!userSettings.autoexportWhenExists || file.GetStatus(stlPath,status)) {
        Save2STL(doc3D , stlPath.GetBuffer(0));
      }
    }
  }
  return true;
}
