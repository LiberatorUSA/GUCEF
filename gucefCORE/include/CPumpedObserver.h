/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_CORE_CPUMPEDOBSERVER_H
#define GUCEF_CORE_CPUMPEDOBSERVER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefMT_CTMailBox.h"
#include "CObserver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CObserverPump;

/*-------------------------------------------------------------------------*/

class GUCEFCORE_EXPORT_CPP CPumpedObserver : public CObserver
{
    public:
    
    CPumpedObserver( void );
    
    CPumpedObserver( const CPumpedObserver& src );
    
    virtual ~CPumpedObserver();
    
    CPumpedObserver& operator=( const CPumpedObserver& src );    
    
    protected:
    
    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events. 
     *
     *  Note that in contrast to OnNotify, which is performed in
     *  the calling thread, this call is always made in the main
     *  application thread. As such it is well suited for linking
     *  non-treadsafe code via an observer to a notifier that 
     *  operates from within another thread.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnPumpedNotify( CNotifier* notifier           ,
                                 const CEvent& eventid         ,
                                 CICloneable* eventdata = NULL ) = 0;
    
    protected:
    friend class CObserverPump;
    
    void OnUpdate( void );
    
    protected:
    
    /**
     *  Event callback member function.
     *  Places the given event in the mailbox where is awaits
     *  the next pump cycle.
     *
     *  Note: DO NOT OVERRIDE !!!     
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );

    private:   
    
    MT::CTMailBox< CEvent > m_mailbox;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPUMPEDOBSERVER_H  ? */
