#include "ApplicationEvent.h"
#include "stdafx.h"
#include "Kompas3DPrint.h"

ApplicationEvent::ApplicationEvent(LPUNKNOWN pObject)
  : BaseEvent(pObject, DIID_ksKompasObjectNotify, NULL, -1, NULL) {
}

ApplicationEvent::~ApplicationEvent() {
}

BEGIN_EVENTSINK_MAP(ApplicationEvent, BaseEvent)
  ON_EVENT (ApplicationEvent, (unsigned int)-1, koCreateDocument,        CreateDocument,        VTS_DISPATCH VTS_I4)
  ON_EVENT (ApplicationEvent, (unsigned int)-1, koOpenDocument,          OpenDocumen,           VTS_DISPATCH VTS_I4)
END_EVENTSINK_MAP()

afx_msg VARIANT_BOOL ApplicationEvent::CreateDocument(LPDISPATCH newDoc, long docType) {    
  AdviseDoc( newDoc, docType );
  return true;
}

//-----------------------------------------------------------------------------
// koOpenDocumen - Документ открыт
// ---
afx_msg VARIANT_BOOL ApplicationEvent::OpenDocumen(LPDISPATCH newDoc, long docType) {
  AdviseDoc(newDoc, docType);
  return true;
}
