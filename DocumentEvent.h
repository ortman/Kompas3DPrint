////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - ���������� ������� �� ���������
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
// DocumentEvent  - ���������� ������� �� ���������
//
////////////////////////////////////////////////////////////////////////////////
class DocumentEvent : public BaseEvent
{
public:
  DocumentEvent( LPDISPATCH doc, bool m_selfAdvise = true );           
  virtual ~DocumentEvent();

protected:
  // kdBeginCloseDocument - ������ �������� ���������.
  afx_msg   VARIANT_BOOL BeginCloseDocument();
  // kdCloseDocument - �������� ������.
  afx_msg   VARIANT_BOOL CloseDocument();
  // kdBeginSaveDocument - ������ ���������� ���������.
  afx_msg   VARIANT_BOOL BeginSaveDocument( LPCTSTR fileName );
  // kdSaveDocument - �������� ��������.
  afx_msg   VARIANT_BOOL SaveDocument();
  // kdActiveDocument - �������� �������������.
  afx_msg   VARIANT_BOOL Activate();
  // kdDeactiveDocument - �������� ���������������.
  afx_msg   VARIANT_BOOL Deactivate();
  // kdBeginSaveAsDocument - ������ ���������� ��������� c ������ ������ (�� ������� ������ �����)
  afx_msg   VARIANT_BOOL BeginSaveAsDocument();
  // // kdDocumentFrameOpen - ���� ��������� ���������
  // afx_msg   VARIANT_BOOL DocumentFrameOpen( LPDISPATCH v );

  DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif 
