/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>
#include "CNotificationIDRegistry.h"
#include "CObserver.h"

#include "CNotifier.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CEvent CNotifier::SubscribeEvent = "GUCEF::CORE::CNotifier::SubscribeEvent";
const CEvent CNotifier::UnsubscribeEvent = "GUCEF::CORE::CNotifier::UnsubscribeEvent";
const CEvent CNotifier::ModifyEvent = "GUCEF::CORE::CNotifier::ModifyEvent";
const CEvent CNotifier::DestructionEvent = "GUCEF::CORE::CNotifier::DestructionEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CNotifier::CNotifier( void )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CNotifier::CNotifier( const CNotifier& notifier )
{TRACE;

    //@TODO: makeme
    assert( 0 );
}

/*-------------------------------------------------------------------------*/

CNotifier&
CNotifier::operator=( const CNotifier& src )
{TRACE;

    assert( 0 );

    if ( this != &src )
    {
    }
    return *this; //@TODO: makeme
}    

/*-------------------------------------------------------------------------*/

CNotifier::~CNotifier()
{TRACE;

    LockData();
    
    /*
     *  Notify observers that the notifier is die-ing
     */
    ForceNotifyObserversOnce( DestructionEvent );
    
    /*
     *  Now we will remove references to this notifier at the observers
     */   
    TObserverList::iterator i = m_observers.begin();
    while ( i != m_observers.end() )
    {
        (*i).first->UnlinkFrom( this, true );
        ++i;
    }    
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::RegisterEvents( void )
{TRACE;

    ModifyEvent.Initialize();
    DestructionEvent.Initialize();
    SubscribeEvent.Initialize();
    UnsubscribeEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void
CNotifier::ForceNotifyObserversOnce( const CEvent& eventid , 
                                     CICloneable* data     )
{TRACE;

    TObserverList::iterator i = m_observers.begin();
    while ( i != m_observers.end() )
    {
        (*i).first->OnNotify( this    ,
                              eventid ,
                              data    );
        ++i;        
    } 
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::Subscribe( CObserver* observer )
{TRACE;

    LockData();
    
    /*
     *  Add the observer to our list of observers as needed.
     *  This will automatically subscribe the observer to 
     *  all notification events.
     */
    TObserverList::iterator i = m_observers.find( observer );
    m_observers[ observer ] = true;

    if ( i == m_observers.end() )
    {
        /*
         *  Establish our bi-directional communication path
         */
        observer->LinkTo( this );

        /*
         *  Send the standard subscription event
         */
        observer->OnNotify( this           ,
                            SubscribeEvent ,
                            NULL           );            
    }
    
    UnlockData();   
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::Subscribe( CObserver* observer   ,
                      const CEvent& eventid )
{TRACE;

    LockData();
    
    bool notifyAboutSubscription = false;
    
    /*
     *  We filter subscription requests for standard events because there is
     *  no need to store them. All observers are subscribed to these events at all times anyway.
     */
    if ( ( eventid != DestructionEvent ) &&
         ( eventid != ModifyEvent )      &&
         ( eventid != SubscribeEvent )   &&
         ( eventid != UnsubscribeEvent )  ) 
    {
        TNotificationList::iterator n( m_eventobservers.find( eventid ) );
        if ( n != m_eventobservers.end() )
        { 
            TObserverSet& eventObservers = (*n).second;
            TObserverSet::iterator i( eventObservers.find( observer ) );
            if ( i != eventObservers.end() )
            {
                /*
                 *  The observer is already subscribed to this event
                 */
                UnlockData(); 
                return;     
            }

            /*
             *  If we get here then we have a new observer on our hands for the given 
             *  known event, as such we need to update our administration.
             */
            eventObservers.insert( observer );

            /*
             *  Establish our bi-directional communication path for the given event
             */
            observer->SubscribeTo( this );

            /*
             *  Send the standard subscription event
             */
            observer->OnNotify( this           ,
                                SubscribeEvent ,
                                NULL           );
        }
        else
        {
            /*
             *  If we get here then the event is unknown to us at this time.
             *  We will add an entry to the list for this event and hook up the
             *  subscriber.
             *  Adding the entry is accomplished by using the index operator on the map
             */
            TObserverSet& eventObservers = m_eventobservers[ eventid ];
            eventObservers.insert( observer );

            /*
             *  Establish our bi-directional communication path for the given event
             */
            observer->SubscribeTo( this );

            /*
             *  Send the standard subscription event
             */
            observer->OnNotify( this           ,
                                SubscribeEvent ,
                                NULL           );
        }
    }
    else
    {
        notifyAboutSubscription = true;
    }

    /*
     *  Add the observer to our list of observers as needed.
     *  This will automatically subscribe the observer to the
     *  four standard notification events.
     */
    TObserverList::iterator i = m_observers.find( observer );
    if ( i == m_observers.end() )
    {
        m_observers[ observer ] = false;
        
        /*
         *  Establish our bi-directional communication path for standard events
         */
        observer->SubscribeTo( this );

        if ( notifyAboutSubscription )
        {
            /*
             *  Send the standard subscription event
             */
            observer->OnNotify( this           ,
                                SubscribeEvent ,
                                NULL           );            
        }               
    }
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CNotifier::Unsubscribe( CObserver* observer )
{TRACE;

    LockData();
    UnsubscribeFromAllEvents( observer ,
                              true     );
    UnlockData();                                  
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::UnsubscribeFromAllEvents( CObserver* observer       ,
                                     const bool notifyObserver )
{TRACE;

    /*
     *  Unsubscribe the observer from all standard 
     *  notification events.
     */
    TObserverList::iterator observerEntry = m_observers.find( observer );
    if ( observerEntry != m_observers.end() )
    {
        m_observers.erase( observerEntry );                    
    }
    
    /*
     *  Unsubscribe the observer from all custom events
     */
    TNotificationList::iterator n( m_eventobservers.begin() );
    while ( n != m_eventobservers.end() )
    {        
        TObserverSet& eventObservers = (*n).second;    
        TObserverSet::iterator i( eventObservers.find( observer ) );
        if ( i != eventObservers.end() )
        {
            /*
             *  Remove the reference to the given observer
             */
            eventObservers.erase( i );
        }
        
        ++n;
    }
    
    /*
     *  Now remove the observer's reference to the notifier
     *  for all events
     */
    observer->UnlinkFrom( this, true );

    if ( notifyObserver )
    {
        /*
         *  Send the standard unsubscribe event
         */
        observer->OnNotify( this             ,
                            UnsubscribeEvent );
    }
}

/*-------------------------------------------------------------------------*/                
 
void 
CNotifier::Unsubscribe( CObserver* observer   ,
                        const CEvent& eventid )
{TRACE;

    LockData();
    
    if ( ( eventid != DestructionEvent ) &&
         ( eventid != SubscribeEvent )   &&
         ( eventid != UnsubscribeEvent ) &&
         ( eventid != ModifyEvent )       )
    {
        TObserverSet& eventObservers = m_eventobservers[ eventid ];    
        TObserverSet::iterator i( eventObservers.begin() );
        while ( i != eventObservers.end() )
        {
            if ( (*i) == observer )
            {
                /*
                 *  We found the observer and we will remove it from
                 *  our notification list for this event.
                 */
                eventObservers.erase( i );
                UnlockData(); 
                return;     
            }
            ++i;
        }

        /*
         *  Now remove the observer's reference to the notifier
         *  for 1 event, effectively reducing the observer's 
         *  reference count for this notifier.
         */
        observer->UnlinkFrom( this );

        /*
         *  Send the standard unsubscribe event
         */
        observer->OnNotify( this             ,
                            UnsubscribeEvent );
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/                        

void 
CNotifier::NotifyObservers( void )
{TRACE;

    LockData();
    ForceNotifyObserversOnce( ModifyEvent );
    UnlockData();
}

/*-------------------------------------------------------------------------*/
    
void 
CNotifier::NotifyObservers( const CEvent& eventid  ,
                            CICloneable* eventData )
{TRACE;

    /*
     *  We will have to copy the lists here because the notification may trigger
     *  an administration mutation that could muck up our notification process.
     */
    LockData();
    TObserverList observers( m_observers );
    TObserverList::iterator i = observers.begin();
    while ( i != observers.end() )
    {
        if ( (*i).second )
        {
            (*i).first->OnNotify( this      ,
                                  eventid   ,
                                  eventData );
        }
        ++i;    
    }
    
    /*
     *  Notify observers that are subscribed to this specific event
     */
    TObserverSet eventObservers( m_eventobservers[ eventid ] );
    TObserverSet::iterator n = eventObservers.begin();
    while ( n != eventObservers.end() )
    {
        i = observers.find( (*n) );
        if ( i == observers.end() )
        {
            (*n)->OnNotify( this      ,
                            eventid   ,
                            eventData );
        }
        else
        {
            if ( !(*i).second )
            {
                (*n)->OnNotify( this      ,
                                eventid   ,
                                eventData );
            }
        }
        ++n;
    }
    UnlockData();    
}

/*-------------------------------------------------------------------------*/

void
CNotifier::NotifyObservers( CNotifier& sender                   ,
                            const CEvent& eventid               ,
                            CICloneable* eventData /* = NULL */ )
{TRACE;

    // Use the same-class friend relationship to access the NotifyObservers()
    // member function of the given notifier.
    sender.NotifyObservers( eventid, eventData );
}                            

/*-------------------------------------------------------------------------*/

void 
CNotifier::OnObserverDestroy( CObserver* observer )
{TRACE;

    LockData();
    UnsubscribeFromAllEvents( observer ,
                              false    );
    UnlockData();                                  
}

/*-------------------------------------------------------------------------*/

CString 
CNotifier::GetType( void ) const
{TRACE;

    return "GUCEF::CORE::CNotifier";
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::LockData( void ) const
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
void 
CNotifier::UnlockData( void ) const
{TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
