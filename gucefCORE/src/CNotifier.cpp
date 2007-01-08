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
    : CITypeNamed()      ,
      m_isBusy( false )  ,
      m_observers()      ,
      m_eventobservers() ,
      m_eventMailStack() ,
      m_cmdMailStack()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CNotifier::CNotifier( const CNotifier& notifier )
    : CITypeNamed()      ,
      m_isBusy( false )  ,
      m_observers()      ,
      m_eventobservers() ,
      m_eventMailStack() ,
      m_cmdMailStack()
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
    
    if ( !m_isBusy )
    {    
        m_isBusy = true;
        
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
                                
            m_isBusy = false;
            ProcessMailbox();            
        }
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_SUBSCRIBE;
        cmdMailElement.eventID = CEvent();
        cmdMailElement.observer = observer;
        cmdMailElement.notify = true;
        m_cmdMailStack.push_back( cmdMailElement );        
    }
    UnlockData();   
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::Subscribe( CObserver* observer   ,
                      const CEvent& eventid )
{TRACE;

    LockData();
    
    if ( !m_isBusy )
    {
        m_isBusy = true;
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
                    m_isBusy = false; 
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
                observer->LinkTo( this );

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
                observer->LinkTo( this );

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
            observer->LinkTo( this );

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
        
        m_isBusy = false;
        ProcessMailbox();
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_SUBSCRIBE;
        cmdMailElement.eventID = eventid;
        cmdMailElement.observer = observer;
        cmdMailElement.notify = true;
        m_cmdMailStack.push_back( cmdMailElement );    
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
    
    if ( !m_isBusy )
    {
        m_isBusy = true;
        
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
        
        if ( notifyObserver )
        {
            /*
             *  Now remove the observer's reference to the notifier
             *  for all events
             */
            observer->UnlinkFrom( this, true );

            /*
             *  Send the standard un-subscribe event
             */
            observer->OnNotify( this             ,
                                UnsubscribeEvent );
        }

        m_isBusy = false;
        ProcessMailbox();        
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_UNSUBSCRIBE;
        cmdMailElement.eventID = CEvent();
        cmdMailElement.observer = observer;
        cmdMailElement.notify = notifyObserver;
        m_cmdMailStack.push_back( cmdMailElement );
    }    
}

/*-------------------------------------------------------------------------*/                
 
void 
CNotifier::Unsubscribe( CObserver* observer   ,
                        const CEvent& eventid )
{TRACE;

    LockData();
    
    if ( !m_isBusy )
    {
        m_isBusy = true;
        
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
                    m_isBusy = false;
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
             *  Send the standard un-subscribe event
             */
            observer->OnNotify( this             ,
                                UnsubscribeEvent );
        }

        m_isBusy = false;
        ProcessMailbox();        
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_UNSUBSCRIBE;
        cmdMailElement.eventID = eventid;
        cmdMailElement.observer = observer;
        cmdMailElement.notify = true;
        m_cmdMailStack.push_back( cmdMailElement );    
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
     *  We will use the flag 'm_isBusy' to indicate whether
     *  we are busy. It is assumed that the code handling that flag can only be accessed
     *  from the same thread since the LockData() call should hold any other thread if
     *  we are operating in a multi threaded environment
     *  This action is required because a notification can trigger some action that results
     *  in a new notification request on the same notifier this could muck up our administration
     *  It could also cause stack overflows if the event chain where long enough.
     */
    LockData();
    
    if ( !m_isBusy )
    {
        m_isBusy = true;
        
        CObserver* oPtr = NULL;
        TObserverSet notifiedObservers;
        
        TObserverList::iterator i = m_observers.begin();
        while ( i != m_observers.end() )
        {
            if ( (*i).second )
            {
                // Check if we have not already notified this observer
                oPtr = (*i).first;
                if ( notifiedObservers.find( oPtr ) == notifiedObservers.end() )
                {
                    // Perform the notification
                    oPtr->OnNotify( this      ,
                                    eventid   ,
                                    eventData );
                    
                    // Add the observer to our 'notified' list
                    notifiedObservers.insert( oPtr );                      
                    
                    // Process command mail if needed
                    if ( !m_cmdMailStack.empty() )
                    {
                        // We have command mail
                        m_isBusy = false;
                        ProcessCmdMailbox();
                        m_isBusy = true;
                        
                        // the administration has been altered, we now have no choice 
                        // but to start from the beginning
                        i = m_observers.begin();
                        continue;
                    }
                }
            }
            ++i;    
        }
        
        /*
         *  Notify observers that are subscribed to this specific event
         */
        bool notified = false;
        TObserverSet& observers = m_eventobservers[ eventid ];
        notifiedObservers.clear();

        TObserverSet::iterator n = observers.begin();
        while ( n != observers.end() )
        {
            // Check if we have not already notified this observer
            notified = false;
            oPtr = (*n);
            if ( notifiedObservers.find( oPtr ) == notifiedObservers.end() )
            {
                // Perform the notification
                i = m_observers.find( oPtr );
                if ( i == m_observers.end() )
                {
                    oPtr->OnNotify( this      ,
                                    eventid   ,
                                    eventData );
                    notified = true;
                }
                else
                {
                    if ( !(*i).second )
                    {
                        oPtr->OnNotify( this      ,
                                        eventid   ,
                                        eventData );
                        notified = true;
                    }
                }
                
                if ( notified )
                {
                    // Add the observer to our 'notified' list
                    notifiedObservers.insert( oPtr );                      
                    
                    // Process command mail if needed
                    if ( !m_cmdMailStack.empty() )
                    {
                        // We have command mail
                        m_isBusy = false;
                        ProcessCmdMailbox();
                        m_isBusy = true;
                        
                        // the administration has been altered, we now have no choice 
                        // but to start from the beginning
                        n = observers.begin();
                        continue;
                    }
                }
            }                
            ++n;
        }
        
        // Now we check if any requests where received while we where busy
        m_isBusy = false;
        ProcessMailbox();        
    }
    else
    {
        // This notifier is already busy processing a notification
        // We will store the request until we are done
        m_eventMailStack.push_back( TEventMailElement( eventid, eventData ) );
    }
    
    UnlockData();    
}

/*-------------------------------------------------------------------------*/

void
CNotifier::ProcessCmdMailbox( void )
{TRACE;

    // Now we check if any command requests where received while we where busy
    while ( !m_cmdMailStack.empty() )
    {
        TCmdMailElement cmdMailElement( m_cmdMailStack[ m_cmdMailStack.size()-1 ] );
        m_cmdMailStack.pop_back();
        
        switch ( cmdMailElement.cmdType )
        {
            case REQUEST_SUBSCRIBE :
            {
                if ( cmdMailElement.eventID.IsInitialized() )
                {
                    Subscribe( cmdMailElement.observer ,
                               cmdMailElement.eventID  ); 
                }
                else
                {
                    Subscribe( cmdMailElement.observer );
                }
                break;
            }
            case REQUEST_UNSUBSCRIBE :
            {
                if ( cmdMailElement.eventID.IsInitialized() )
                {
                    Unsubscribe( cmdMailElement.observer ,
                                 cmdMailElement.eventID  ); 
                }
                else
                {
                    UnsubscribeFromAllEvents( cmdMailElement.observer , 
                                              cmdMailElement.notify   );
                }
                break;                    
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::ProcessEventMailbox( void )
{TRACE;
    
    // Now we check if any notifications requests where received while we where busy
    if ( !m_eventMailStack.empty() )
    {
        TEventMailElement entry( m_eventMailStack[ m_eventMailStack.size()-1 ] );            
        m_eventMailStack.pop_back();        
        
        // Dispatch the message in our mailbox
        // If the mailbox has any more items remaining we will end up back here 
        // for the next mail item
        NotifyObservers( entry.first, entry.second );            
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::ProcessMailbox( void )
{TRACE;

    ProcessCmdMailbox();
    ProcessEventMailbox();
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
