////////////////////////////////////////////////////////////////////////////////
//
// ApplicationEvent  - ���������� ������� �� ����������
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

/*
enum ksKompasObjectNotifyEnum
{
  koCreateDocument = 1,
  koBeginOpenDocument = 2,
  koOpenDocument = 3,
  koChangeActiveDocument = 4,
  koApplicatinDestroy = 5,
  koBeginCreate = 6,
  koBeginOpenFile = 7,
  koBeginCloseAllDocument = 8
};
*/

////////////////////////////////////////////////////////////////////////////////
//
// ApplicationEvent  - ���������� ������� �� ����������
//
////////////////////////////////////////////////////////////////////////////////
class ApplicationEvent : public BaseEvent
{
public:
  ApplicationEvent( LPUNKNOWN pObject,
                    bool m_selfAdvise = true );           
  virtual ~ApplicationEvent();

protected:
  // koCreateDocument - �������� ������
  afx_msg VARIANT_BOOL CreateDocument( LPDISPATCH newDoc, long docType );
  // koOpenDocumenBegin - ������ �������� ���������
  afx_msg VARIANT_BOOL BeginOpenDocumen( LPCTSTR fileName );
  // koOpenDocumen - �������� ������
  afx_msg VARIANT_BOOL OpenDocumen( LPDISPATCH newDoc, long docType );
  // koActiveDocument - ������������ �� ������ �������� ��������
  afx_msg VARIANT_BOOL ChangeActiveDocument( LPDISPATCH newDoc, long docType );
  // koApplicatinDestroy - �������� ����������
  afx_msg VARIANT_BOOL ApplicationDestroy();
  // koBeginCreate - ������ �������� ���������(�� ������� ������ ����)
  afx_msg VARIANT_BOOL BeginCreate( long docType );
  // koBeginOpenFile - ������ �������� ���������(�� ������� ������ �����)
  afx_msg VARIANT_BOOL BeginOpenFile();
  // koBeginCloseAllDocument - ������ �������� ���� �������� ����������
  afx_msg VARIANT_BOOL BeginCloseAllDocument();

  // koBeginRequestFiles - ������ �������� ���� �������� ����������
  afx_msg VARIANT_BOOL BeginRequestFiles( long requestID, VARIANT * files );

  DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif 
