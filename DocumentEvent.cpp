////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DocumentEvent.h"
#include "Kompas3DPrint.h"

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

afx_msg VARIANT_BOOL DocumentEvent::SaveDocument() {
  // CString str( m_libName + " --> DocumentEvent::BeginCloseDocument" );
  // str += "\nИмя документа = " + GetDocName();
  // return !!kompas->ksYesNo( str.GetBuffer(0) );
  CString str = GetDocName();
  str += ".stl";
  // kompas->ksMessage(str.GetBuffer(0));

  ksDocument3DPtr doc3D( kompas->ActiveDocument3D());
  if (doc3D) {
    Save2STL(doc3D , str.GetBuffer(0));
  }
  return true;
}
