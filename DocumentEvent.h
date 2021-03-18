////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обрабокчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _DOCUMENTEVENT_H
#define _DOCUMENTEVENT_H

#ifndef _BASEEVENT_H
#include "BaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000     

/*
enum ksDocumentFileNotifyEnum
{
  kdBeginCloseDocument = 1,
  kdCloseDocument = 2,
  kdBeginSaveDocument = 3,
  kdSaveDocument = 4,
  kdActiveDocument = 5,
  kdDeactiveDocument = 6,
  kdBeginSaveAsDocument = 7
};
*/

////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обрабокчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////
class DocumentEvent : public BaseEvent
{
public:
  DocumentEvent( LPDISPATCH doc, bool m_selfAdvise = true );           
  virtual ~DocumentEvent();

protected:
  // kdBeginCloseDocument - Начало закрытия документа.
  afx_msg   VARIANT_BOOL BeginCloseDocument();
  // kdCloseDocument - Документ закрыт.
  afx_msg   VARIANT_BOOL CloseDocument();
  // kdBeginSaveDocument - Начало сохранения документа.
  afx_msg   VARIANT_BOOL BeginSaveDocument( LPCTSTR fileName );
  // kdSaveDocument - Документ сохранен.
  afx_msg   VARIANT_BOOL SaveDocument();
  // kdActiveDocument - Документ активизирован.
  afx_msg   VARIANT_BOOL Activate();
  // kdDeactiveDocument - Документ деактивизирован.
  afx_msg   VARIANT_BOOL Deactivate();
  // kdBeginSaveAsDocument - Начало сохранения документа c другим именем (до диалога выбора имени)
  afx_msg   VARIANT_BOOL BeginSaveAsDocument();
  // // kdDocumentFrameOpen - Окно документа открылось
  // afx_msg   VARIANT_BOOL DocumentFrameOpen( LPDISPATCH v );

  DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif 
