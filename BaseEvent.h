/////////////////////////////////////////////////////////////////////////////
//
// Базовый клас для обработчиков событий
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
// Базовый клас для обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
class BaseEvent : public CCmdTarget
{
protected:
  static  CObList           m_EventList;  // Список обработчиков событий
          DWORD             m_dwCookie;   // Уникальный идентификатор соединения
           IID                m_iidEvents;  
          LPCONNECTIONPOINT  m_pConnPt;
          LPUNKNOWN         m_pContainer; // Источник событий
          
          // Вспомогательные данные
          CString           m_libName; 
          // Идентификационные данные
          LPDISPATCH        m_doc;        // Документ
          long              m_objType;    // Тип объекта
          ksFeaturePtr      m_obj3D;      // Объект для 3D
public:
            BaseEvent( LPUNKNOWN pObject, 
                       IID iidEvents, 
                       LPDISPATCH doc     = NULL, 
                       long objType       = -1, 
                       ksFeaturePtr obj3D = NULL);
  virtual ~ BaseEvent();
  
  virtual int  Advise  ();                          // Подписаться на получение событий
  virtual void Unadvise();                          // Отписаться от получения событий
  virtual void Clear();
          void Disconnect(); 

  static  void BaseEvent::TerminateEvents( void );  // Отписать все события
  static  void BaseEvent::TerminateEvents( IID iid, // Отписать заданные события
                                           LPDISPATCH doc     = NULL, 
                                           long objType       = -1, 
                                           ksFeaturePtr obj3D = NULL );  
  static  bool BaseEvent::FindEvent( IID iid, // Поиск подписки
                                     LPDISPATCH doc     = NULL, 
                                     long objType       = -1, 
                                     ksFeaturePtr obj3D = NULL );
  static  void BaseEvent::ListEvents();
  static CString GetDocName(LPDISPATCH doc); // Получить имя документа
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
