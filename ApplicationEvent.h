////////////////////////////////////////////////////////////////////////////////
//
// ApplicationEvent  - обрабокчик событий от приложения
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONEVENT_H
#define _APPLICATIONEVENT_H

#ifndef _BASEEVENT_H
#include "BaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ApplicationEvent : public BaseEvent
{
public:
  ApplicationEvent(LPUNKNOWN pObject);           
  virtual ~ApplicationEvent();

protected:
  afx_msg VARIANT_BOOL CreateDocument(LPDISPATCH newDoc, long docType);
  afx_msg VARIANT_BOOL OpenDocumen(LPDISPATCH newDoc, long docType);

  DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif 
