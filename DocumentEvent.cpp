////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - ���������� ������� �� ���������
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
  CString savePath = _T("");
  CString m3dPath = GetDocName();
  int pathLen = m3dPath.GetLength();
  if (pathLen < 5) return true;;
  if (m3dPath.Right(4) != _T(".m3d")) return true;
  char slash = '\\';
  int slashPos = m3dPath.ReverseFind(slash);
  if (slashPos < 0) {
    slash = '/';
    slashPos = m3dPath.ReverseFind(slash);
  }
  if (slashPos > 0) {
    CString folder = m3dPath.Left(slashPos + 1) + userSettings.format2Str();
    BOOL existFolder = DirectoryExists(folder);
    if (!existFolder && userSettings.createFolder) {
      existFolder = CreateDirectory(folder, NULL);
    }
    if (existFolder) {
      savePath = folder + slash + m3dPath.Mid(slashPos + 1, pathLen - slashPos - 5) + userSettings.getExt();
    } else {
      savePath = m3dPath.Left(pathLen-4) + userSettings.getExt();
    }
  } else {
    savePath = m3dPath.Left(pathLen-4) + userSettings.getExt();
  }

  if (m_doc) {
    ksDocument3DPtr doc3D;
    m_doc->QueryInterface(DIID_ksDocument3D, (LPVOID*)&doc3D);
    if (doc3D && userSettings.autoexportEn) {
      CStdioFile file;
      CFileStatus status;
      if (!userSettings.autoexportWhenExists || file.GetStatus(savePath, status)) {
        if (!Save(doc3D , savePath.GetBuffer(0))) {
          _bstr_t msg = L"Kompas3DPrint: �� ������� ��������� ���� :\"" + savePath + L"\"";
          kompas->ksError(msg);
        }
      }
    }
  }
  return true;
}
