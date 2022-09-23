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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H */

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#include "CPumpedObserver.h"

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

/**
 *  Internally used data container class used for mailing
 *  extra data using the CMailBox class.
 */
class CMailElement : public CICloneable
{
    private:

    CICloneable* m_eventdata;
    CNotifier* m_notifier;
    CIEventHandlerFunctorBase* m_callback;

    CMailElement( void ); /**< dummy, dont use */

    private:
    friend class CPumpedObserver;

    CMailElement( CNotifier* notifier                              ,
                  CICloneable* eventdata                           ,
                  CIEventHandlerFunctorBase* callback = GUCEF_NULL )
        : m_notifier( notifier )  
        , m_eventdata( eventdata )
        , m_callback( callback )
    {GUCEF_TRACE;

    }

    CMailElement( const CMailElement& src )
        : m_notifier( src.m_notifier )  
        , m_eventdata( src.m_eventdata )
        , m_callback( src.m_callback )
    {GUCEF_TRACE;

    }

    virtual ~CMailElement()
    {GUCEF_TRACE;

    }

    CMailElement& operator=( const CMailElement& src )
    {GUCEF_TRACE;

        if ( this != &src )
        {
            m_notifier = src.m_notifier;
            m_eventdata = src.m_eventdata;
            m_callback = src.m_callback;
        }
        return *this;
    }

    inline CNotifier* GetNotifier( void ) const
    {GUCEF_TRACE;

        return m_notifier;
    }

    inline CICloneable* GetData( void ) const
    {GUCEF_TRACE;

        return m_eventdata;
    }

    inline CIEventHandlerFunctorBase* GetCallback( void ) const
    {GUCEF_TRACE;

        return m_callback;
    }

    virtual CICloneable* Clone( void ) const
    {GUCEF_TRACE;

        return new CMailElement( *this );
    }
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPumpedObserver::CPumpedObserver( bool allowSameThreadEventsToFlowThrough )
    : CObserver()                                                      
    , m_pulseGenerator( CCoreGlobal::Instance()->GetPulseGenerator() ) 
    , m_propagatePulseEvent( false )
    , m_allowSameThreadEventsToFlowThrough( allowSameThreadEventsToFlowThrough )
    , m_mailbox()
{GUCEF_TRACE;

    assert( GUCEF_NULL != m_pulseGenerator );
    RegisterPulseGeneratorEventHandlers();
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::CPumpedObserver( PulseGeneratorPtr pulseGenerator        ,
                                  bool allowSameThreadEventsToFlowThrough )
    : CObserver()                       
    , m_pulseGenerator( pulseGenerator ) 
    , m_propagatePulseEvent( false )
    , m_allowSameThreadEventsToFlowThrough( allowSameThreadEventsToFlowThrough )
    , m_mailbox()
{GUCEF_TRACE;
    
    RegisterPulseGeneratorEventHandlers();
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::CPumpedObserver( const CPumpedObserver& src )
    : CObserver( src )        
    , m_pulseGenerator( GUCEF_NULL ) 
    , m_propagatePulseEvent( src.m_propagatePulseEvent )
    , m_allowSameThreadEventsToFlowThrough( src.m_allowSameThreadEventsToFlowThrough )
    , m_mailbox()
{GUCEF_TRACE;

    m_pulseGenerator = src.m_pulseGenerator;

    RegisterPulseGeneratorEventHandlers();
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::RegisterPulseGeneratorEventHandlers( void )
{GUCEF_TRACE;

    if ( !m_pulseGenerator.IsNULL() )
    {
        TEventCallback callback( this, &CPumpedObserver::OnPulse );
        SubscribeTo( m_pulseGenerator.GetPointerAlways() ,
                     CPulseGenerator::PulseEvent         ,
                     callback                            );

        m_pulseGenerator->RequestPeriodicPulses( this );
    }
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::~CPumpedObserver()
{GUCEF_TRACE;

    SetPulseGenerator( PulseGeneratorPtr() );
    SignalUpcomingObserverDestruction();    
    ClearMailbox( false );
}

/*-------------------------------------------------------------------------*/

void 
CPumpedObserver::ClearMailbox( bool acceptNewMail )
{GUCEF_TRACE;

    if ( !acceptNewMail )
        m_mailbox.SetAcceptsNewMail( acceptNewMail );

    CEvent mailEventID;
    CICloneable* dataptr( GUCEF_NULL );
    CMailElement* maildata( GUCEF_NULL );
    while ( m_mailbox.GetMail( mailEventID ,
                               &dataptr    ) )
    {
        maildata = static_cast< CMailElement* >( dataptr );
        delete maildata->GetCallback();
        delete maildata->GetData();
        delete maildata;
    }

    if ( acceptNewMail )
        m_mailbox.SetAcceptsNewMail( acceptNewMail );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver&
CPumpedObserver::operator=( const CPumpedObserver& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CPumpedObserver::SetPulseGenerator( PulseGeneratorPtr newPulseGenerator )
{GUCEF_TRACE;

    PulseGeneratorPtr oldPulseGenerator = m_pulseGenerator;
    m_pulseGenerator.Unlink();
    
    if ( !oldPulseGenerator.IsNULL() )
    {
        oldPulseGenerator->RequestStopOfPeriodicUpdates( this );
        UnsubscribeFrom( *oldPulseGenerator.GetPointerAlways() ); 
    }

    m_pulseGenerator = newPulseGenerator;
    RegisterPulseGeneratorEventHandlers();
}

/*-------------------------------------------------------------------------*/

PulseGeneratorPtr 
CPumpedObserver::GetPulseGenerator( void ) const
{GUCEF_TRACE;

    return m_pulseGenerator;
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnPulse( CNotifier* notifier                       ,
                          const CEvent& eventid                     ,
                          CICloneable* eventdata /* = GUCEF_NULL */ )

{GUCEF_TRACE;

    CEvent mailEventID;
    CICloneable* dataptr( GUCEF_NULL );
    CMailElement* maildata( GUCEF_NULL );
    while ( m_mailbox.GetMail( mailEventID ,
                               &dataptr    ) )
    {        
        maildata = static_cast< CMailElement* >( dataptr );
        CIEventHandlerFunctorBase* callback = maildata->GetCallback();
        if ( GUCEF_NULL == callback )
        {
            OnPumpedNotify( maildata->GetNotifier() ,
                            mailEventID             ,
                            maildata->GetData()     );
        }
        else
        {
            OnPumpedFunctorNotify( maildata->GetNotifier() ,
                                   mailEventID             ,
                                   maildata->GetData()     ,
                                   callback                );
        
            delete callback;
        }

        delete maildata->GetData();
        delete maildata;
    }

    if ( m_propagatePulseEvent )
        OnPumpedNotify( notifier, eventid, eventdata );
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::SetPropagatePulseEvent( bool propagatePulseEventMsg )
{GUCEF_TRACE;

    m_propagatePulseEvent = propagatePulseEventMsg;
}

/*-------------------------------------------------------------------------*/

bool
CPumpedObserver::GetPropagatePulseEvent( void ) const
{GUCEF_TRACE;

    return m_propagatePulseEvent;
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnNotify( CNotifier* notifier                       ,
                           const CEvent& eventId                     ,
                           CICloneable* eventData /* = GUCEF_NULL */ )
{GUCEF_TRACE;

    if ( !m_pulseGenerator.IsNULL()                                               && 
         m_allowSameThreadEventsToFlowThrough                                     &&
         ( MT::GetCurrentTaskID() == m_pulseGenerator->GetPulseDriverThreadId() ) )                                               
    {
        // We are already in the thread that will pump the events
        OnPumpedNotify( notifier  , 
                        eventId   ,
                        eventData );
        return;
    }
    
    AddEventToMailbox( notifier, eventId, eventData );
}

/*-------------------------------------------------------------------------*/

bool 
CPumpedObserver::AddEventToMailbox( CNotifier* notifier    , 
                                    const CEvent& eventId  ,
                                    CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL != eventData )
        eventData = eventData->Clone();

    CMailElement maildata( notifier, eventData );
    return m_mailbox.AddMail( eventId, &maildata );
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnFunctorNotify( CNotifier* notifier                 ,
                                  const CEvent& eventid               ,
                                  CICloneable* eventdata              ,
                                  CIEventHandlerFunctorBase* callback )
{GUCEF_TRACE;

    if ( !m_pulseGenerator.IsNULL()                                               &&
         m_allowSameThreadEventsToFlowThrough                                     &&
         ( MT::GetCurrentTaskID() == m_pulseGenerator->GetPulseDriverThreadId() ) )                                               
    {
        // We are already in the thread that will pump the events
        OnPumpedFunctorNotify( notifier  , 
                               eventid   ,
                               eventdata ,
                               callback  );
        return;
    }
    
    if ( GUCEF_NULL != eventdata )
    {
        eventdata = eventdata->Clone();
    }
    if ( GUCEF_NULL != callback )
    {
        callback = static_cast< CIEventHandlerFunctorBase* >( callback->Clone() );
    }    

    CMailElement maildata( notifier  ,
                           eventdata ,
                           callback  );

    m_mailbox.AddMail( eventid   ,
                       &maildata );
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnPumpedNotify( CNotifier* notifier                       ,
                                 const CEvent& eventid                     ,
                                 CICloneable* eventdata /* = GUCEF_NULL */ )

{GUCEF_TRACE;

    // dummy, override in decending classes
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnPumpedFunctorNotify( CNotifier* notifier                 ,
                                        const CEvent& eventId               ,
                                        CICloneable* eventdData             ,
                                        CIEventHandlerFunctorBase* callback )

{GUCEF_TRACE;

    if ( GUCEF_NULL != callback )    
    {
        callback->OnNotify( notifier   ,
                            eventId    ,
                            eventdData );
    }
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::ProxySubscribeTo( CNotifier* threadedNotifier         ,
                                   const CEvent& eventid               ,
                                   CIEventHandlerFunctorBase& callback )
{GUCEF_TRACE;

    assert( GUCEF_NULL != &callback );

    TEventCallbackProxy callbackProxy( this, &CPumpedObserver::OnFunctorNotify, &callback );
    threadedNotifier->Subscribe( this           ,
                                 eventid        ,
                                 &callbackProxy );
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CPumpedObserver::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

bool
CPumpedObserver::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPumpedObserver::Unlock( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

const CString&
CPumpedObserver::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CPumpedObserver";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
