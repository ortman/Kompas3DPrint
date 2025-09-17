/////////////////////////////////////////////////////////////////////////////
//
// ������� ���� ��� ������������ �������
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _BASEEVENT_H
#define _BASEEVENT_H

#include "stdafx.h"
#include <afxctl.h>
#include <afxpriv.h>
#include <ldefin2d.h>
   
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
//
// ������� ���� ��� ������������ �������
//
/////////////////////////////////////////////////////////////////////////////
class BaseEvent : public CCmdTarget
{
protected:
  static  CObList           m_EventList;  // ������ ������������ �������
          DWORD             m_dwCookie;   // ���������� ������������� ����������
           IID                m_iidEvents;  
          LPCONNECTIONPOINT  m_pConnPt;
          LPUNKNOWN         m_pContainer; // �������� �������
          
          // ��������������� ������
          CString           m_libName; 
          // ����������������� ������
          LPDISPATCH        m_doc;        // ��������
          long              m_objType;    // ��� �������
          ksFeaturePtr      m_obj3D;      // ������ ��� 3D
public:
            BaseEvent( LPUNKNOWN pObject, 
                       IID iidEvents, 
                       LPDISPATCH doc     = NULL, 
                       long objType       = -1, 
                       ksFeaturePtr obj3D = NULL);
  virtual ~ BaseEvent();
  
  virtual int  Advise  ();                          // ����������� �� ��������� �������
  virtual void Unadvise();                          // ���������� �� ��������� �������
  virtual void Clear();
          void Disconnect(); 

  static  void BaseEvent::TerminateEvents( void );  // �������� ��� �������
  static  void BaseEvent::TerminateEvents( IID iid, // �������� �������� �������
                                           LPDISPATCH doc     = NULL, 
                                           long objType       = -1, 
                                           ksFeaturePtr obj3D = NULL );  
  static  bool BaseEvent::FindEvent( IID iid, // ����� ��������
                                     LPDISPATCH doc     = NULL, 
                                     long objType       = -1, 
                                     ksFeaturePtr obj3D = NULL );
  static  void BaseEvent::ListEvents();
  static CString GetDocName(LPDISPATCH doc); // �������� ��� ���������
protected: 
  BEGIN_INTERFACE_PART(EventHandler, IDispatch)
    INIT_INTERFACE_PART(BaseEvent, EventHandler)
    STDMETHOD(GetTypeInfoCount)(unsigned int*);
    STDMETHOD(GetTypeInfo)(unsigned int, LCID, ITypeInfo**);
    STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
    STDMETHOD(Invoke)(DISPID, REFIID, LCID, unsigned short, DISPPARAMS*,
              VARIANT*, EXCEPINFO*, unsigned int*);
  END_INTERFACE_PART(EventHandler)

  DECLARE_INTERFACE_MAP()
  DECLARE_EVENTSINK_MAP()
};

////////////////////////////////////////////////////////////////////////////////

#endif 
