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

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef GUCEF_CORE_COBSERVER_H
#include "CObserver.h"
#define GUCEF_CORE_COBSERVER_H
#endif /* GUCEF_CORE_COBSERVER_H ? */

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

#include "CNotifierImplementor.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CNotifierImplementor::CNotifierImplementor( CNotifier* ownerNotifier )
    : m_isBusy( false )                 ,
      m_observers()                     ,
      m_eventobservers()                ,
      m_eventMailStack()                ,
      m_cmdMailStack()                  ,
      m_ownerNotifier( ownerNotifier )  ,
      m_scheduledForDestruction( false )
{GUCEF_TRACE;

    assert( m_ownerNotifier != NULL );
}

/*-------------------------------------------------------------------------*/

CNotifierImplementor::CNotifierImplementor( CNotifier* ownerNotifier ,
                                            const CNotifier& src     )
    : m_isBusy( false )                 ,
      m_observers()                     ,
      m_eventobservers()                ,
      m_eventMailStack()                ,
      m_cmdMailStack()                  ,
      m_ownerNotifier( ownerNotifier )  ,
      m_scheduledForDestruction( false )
{GUCEF_TRACE;

    //@TODO: makeme
    assert( 0 );
    
    assert( m_ownerNotifier != NULL );
}

/*-------------------------------------------------------------------------*/

CNotifierImplementor&
CNotifierImplementor::operator=( const CNotifierImplementor& src )
{GUCEF_TRACE;

    assert( 0 ); //@TODO: makeme

    if ( this != &src )
    {
    }
    return *this; 
}    

/*-------------------------------------------------------------------------*/

CNotifierImplementor::~CNotifierImplementor()
{GUCEF_TRACE;
   
    /*
     *  The notifier should no longer be busy with some action if it is being destroyed.
     *  If it is still busy then the code would resume after the object is deleted which
     *  causes undefined behavior possibly resulting in a crash.
     *  If you hit this assert then you have found a bug in the logic of this class
     */
    assert( !m_isBusy );    
}

/*-------------------------------------------------------------------------*/

CNotifierImplementor*
CNotifierImplementor::Create( CNotifier* ownerNotifier )
{GUCEF_TRACE;

    // perform in-scope creation
    return new CNotifierImplementor( ownerNotifier );
}

/*-------------------------------------------------------------------------*/

CNotifierImplementor*
CNotifierImplementor::Create( CNotifier* ownerNotifier ,
                              const CNotifier& src     )
{GUCEF_TRACE;

    // perform in-scope creation
    return new CNotifierImplementor( ownerNotifier ,
                                     src           );
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::Destroy( CNotifierImplementor* obj )
{GUCEF_TRACE;
    
    // perform an in-scope destruction
    delete obj;
}

/*-------------------------------------------------------------------------*/

bool
CNotifierImplementor::ForceNotifyObserversOnce( const CEvent& eventid , 
                                                CICloneable* data     )
{GUCEF_TRACE;

    m_isBusy = true;
    
    CObserver* oPtr = NULL;
    TObserverSet notifiedObservers;
            
    TObserverList::iterator i = m_observers.begin();
    while ( i != m_observers.end() )
    {       
        // Check if we have not already notified this observer
        oPtr = (*i).first;
        if ( notifiedObservers.find( oPtr ) == notifiedObservers.end() )
        {
            (*i).first->OnNotify( m_ownerNotifier ,
                                  eventid         ,
                                  data            );
                                  
            // Check if someone deleted our owner notifier
            if ( m_ownerNotifier == NULL )
            {
                // Gracefully handle the destruction sequence
                m_isBusy = false;
                Destroy( this );
                return false;
            }
                                
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
        ++i;        
    } 
    
    m_isBusy = false;
    
    if ( m_scheduledForDestruction )
    {
        delete m_ownerNotifier;
        return false;
    }
        
    return true;
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::UnsubscribeAllFromNotifier( void )
{GUCEF_TRACE;
    
    LockData();

    if ( !m_isBusy )
    {
        // Now we will remove references to the notifier at the observers  
        TObserverList::iterator i = m_observers.begin();
        while ( i != m_observers.end() )
        {
            (*i).first->UnlinkFrom( m_ownerNotifier, true );
            ++i;
        }

        m_observers.clear();
        m_eventobservers.clear();
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_UNSUBSCRIBE_ALL;
        cmdMailElement.eventID = CEvent();     // <- not used in this context
        cmdMailElement.observer = NULL;        // <- not used in this context
        cmdMailElement.callback = NULL;        // <- not used in this context
        cmdMailElement.notify = true;          // <- not used in this context
        m_cmdMailStack.push_back( cmdMailElement );
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void 
CNotifierImplementor::Subscribe( CObserver* observer )
{GUCEF_TRACE;

    LockData();
    
    if ( !m_isBusy )
    {    
        m_isBusy = true;
        
        /*
         *  Add the observer to our list of observers as needed.
         *  This will automatically subscribe the observer to 
         *  all notification events.
         *
         *  Note that the boolean value indicates whether the observer is
         *  subscribed to all events
         */
        TObserverList::iterator i = m_observers.find( observer );
        m_observers[ observer ] = true;

        if ( i == m_observers.end() )
        {
            /*
             *  Establish our bi-directional communication path
             */
            observer->LinkTo( m_ownerNotifier );

            /*
             *  Send the standard subscription event
             */
            observer->OnNotify( m_ownerNotifier            ,
                                CNotifier::SubscribeEvent  ,
                                NULL                       );
                                
            m_isBusy = false;
            
            if ( m_scheduledForDestruction )
            {
                UnlockData();
                delete m_ownerNotifier;
                return;
            }
            
            ProcessMailbox();            
        }
        m_isBusy = false;        
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_SUBSCRIBE;
        cmdMailElement.eventID = CEvent();     // <- not used in this context
        cmdMailElement.observer = observer;
        cmdMailElement.callback = NULL;
        cmdMailElement.notify = true;
        m_cmdMailStack.push_back( cmdMailElement );        
    }
    UnlockData();   
}

/*-------------------------------------------------------------------------*/

void 
CNotifierImplementor::Subscribe( CObserver* observer                              ,
                                 const CEvent& eventid                            ,
                                 CIEventHandlerFunctorBase* callback /* = NULL */ )
{GUCEF_TRACE;

    LockData();
    
    if ( !m_isBusy )
    {
        m_isBusy = true;
        bool notifyAboutSubscription = false;
        
        /*
         *  We filter subscription requests for standard events because there is
         *  no need to store them. All observers are subscribed to these events at all times anyway.
         */
        if ( ( eventid != CNotifier::DestructionEvent ) &&
             ( eventid != CNotifier::ModifyEvent )      &&
             ( eventid != CNotifier::SubscribeEvent )   &&
             ( eventid != CNotifier::UnsubscribeEvent )  ) 
        {
            // Get a list of all observers that listen to this specific event
            TNotificationList::iterator n( m_eventobservers.find( eventid ) );
            if ( n != m_eventobservers.end() )
            { 
                // Check if a subscription is already present for the given observer
                TEventNotificationMap& eventObservers = (*n).second;
                TEventNotificationMap::iterator i( eventObservers.find( observer ) );
                if ( i != eventObservers.end() )
                {
                    // The observer is already subscribed to this event
                    // All we have to do is make sure the callback is up-todate
                    delete (*i).second;
                    (*i).second = NULL;
                    if ( NULL != callback )
                    { 
                        (*i).second = static_cast< CIEventHandlerFunctorBase* >( callback->Clone() );
                    }
                    
                    m_isBusy = false; 
                    UnlockData(); 
                    return;     
                }
            
                /*
                 *  If we get here then we have a new observer on our hands for the given 
                 *  known event, as such we need to update our administration.
                 */
                if ( NULL != callback )
                { 
                    eventObservers[ observer ] = static_cast< CIEventHandlerFunctorBase* >( callback->Clone() );
                }
                else
                { 
                    eventObservers[ observer ] = NULL;
                }
            }
            else
            {
                /*
                 *  If we get here then the event is unknown to us at this time.
                 *  We will add an entry to the list for this event and hook up the
                 *  subscriber.
                 *  Adding the event entry is accomplished by using the index operator on the map
                 */
                TEventNotificationMap& eventObservers = m_eventobservers[ eventid ];
                if ( NULL != callback )
                { 
                    eventObservers[ observer ] = static_cast< CIEventHandlerFunctorBase* >( callback->Clone() );
                }
                else
                { 
                    eventObservers[ observer ] = NULL;
                }
            }
            
            // Establish our bi-directional communication path for the given event
            observer->LinkTo( m_ownerNotifier );

            // Send the standard subscription event
            observer->OnNotify( m_ownerNotifier            ,
                                CNotifier::SubscribeEvent  ,
                                NULL                       );
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
            // Because the observer is not yet in the list we can add it as an observer
            // that only listens to specific events
            m_observers[ observer ] = false;
            
            // Establish our bi-directional communication path for standard events
            observer->LinkTo( m_ownerNotifier );

            if ( notifyAboutSubscription )
            {
                /*
                 *  Send the standard subscription event
                 */
                observer->OnNotify( m_ownerNotifier            ,
                                    CNotifier::SubscribeEvent  ,
                                    NULL                       );            
            }               
        }
        
        m_isBusy = false;
        
        if ( m_scheduledForDestruction )
        {
            UnlockData();
            delete m_ownerNotifier;
            return;
        }        
        
        ProcessMailbox();
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_SUBSCRIBE;
        cmdMailElement.eventID = eventid;
        cmdMailElement.observer = observer;
        cmdMailElement.notify = true;
        if ( callback != NULL )
        {
            cmdMailElement.callback = static_cast< CIEventHandlerFunctorBase* >( callback->Clone() );
        }
        else
        {
            cmdMailElement.callback = NULL;
        }        
        m_cmdMailStack.push_back( cmdMailElement );    
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::Unsubscribe( CObserver* observer )
{GUCEF_TRACE;

    LockData();
    UnsubscribeFromAllEvents( observer ,
                              true     );
    UnlockData();                                  
}

/*-------------------------------------------------------------------------*/

void 
CNotifierImplementor::UnsubscribeFromAllEvents( CObserver* observer       ,
                                                const bool notifyObserver )
{GUCEF_TRACE;
    
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
            TEventNotificationMap& eventObservers = (*n).second;    
            TEventNotificationMap::iterator i( eventObservers.find( observer ) );
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
            observer->UnlinkFrom( m_ownerNotifier, true );

            /*
             *  Send the standard un-subscribe event
             */
            observer->OnNotify( m_ownerNotifier             ,
                                CNotifier::UnsubscribeEvent );
        }

        m_isBusy = false;
        
        if ( m_scheduledForDestruction )
        {
            UnlockData();
            delete m_ownerNotifier;
            return;
        }
                
        ProcessMailbox();        
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_UNSUBSCRIBE;
        cmdMailElement.eventID = CEvent();    // <- not used in this context
        cmdMailElement.observer = observer;
        cmdMailElement.callback = NULL;
        cmdMailElement.notify = notifyObserver;
        m_cmdMailStack.push_back( cmdMailElement );
    }    
}

/*-------------------------------------------------------------------------*/                
 
void 
CNotifierImplementor::Unsubscribe( CObserver* observer   ,
                                   const CEvent& eventid )
{GUCEF_TRACE;

    LockData();
    
    if ( !m_isBusy )
    {
        m_isBusy = true;
        
        if ( ( eventid != CNotifier::DestructionEvent ) &&
             ( eventid != CNotifier::SubscribeEvent )   &&
             ( eventid != CNotifier::UnsubscribeEvent ) &&
             ( eventid != CNotifier::ModifyEvent )       )
        {
            TEventNotificationMap& eventObservers = m_eventobservers[ eventid ];    
            TEventNotificationMap::iterator i( eventObservers.begin() );
            while ( i != eventObservers.end() )
            {
                if ( (*i).first == observer )
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
            observer->UnlinkFrom( m_ownerNotifier );

            /*
             *  Send the standard un-subscribe event
             */
            observer->OnNotify( m_ownerNotifier             ,
                                CNotifier::UnsubscribeEvent );
        }

        m_isBusy = false;
        
        if ( m_scheduledForDestruction )
        {
            UnlockData();
            delete m_ownerNotifier;
            return;
        }
                
        ProcessMailbox();        
    }
    else
    {
        TCmdMailElement cmdMailElement;
        cmdMailElement.cmdType = REQUEST_UNSUBSCRIBE;
        cmdMailElement.eventID = eventid;
        cmdMailElement.observer = observer;
        cmdMailElement.callback = NULL;    // <- not used in this context
        cmdMailElement.notify = true;
        m_cmdMailStack.push_back( cmdMailElement );    
    }    
    UnlockData();
}

/*-------------------------------------------------------------------------*/                        

bool 
CNotifierImplementor::NotifyObservers( void )
{GUCEF_TRACE;

    LockData();
    bool aliveState = ForceNotifyObserversOnce( CNotifier::ModifyEvent );
    UnlockData();
    return aliveState;
}

/*-------------------------------------------------------------------------*/
    
bool 
CNotifierImplementor::NotifyObservers( const CEvent& eventid  ,
                                       CICloneable* eventData )
{GUCEF_TRACE;

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

        // First we process observers that are subscribed to all events        
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
                    // If an observer is subscribed to all events we never used specialized callbacks
                    // instead we always use the standard OnNotify()
                    oPtr->OnNotify( m_ownerNotifier ,
                                    eventid         ,
                                    eventData       );
                    
                    // Check if someone deleted our owner notifier
                    if ( m_ownerNotifier == NULL )
                    {
                        // Gracefully handle the destruction sequence
                        m_isBusy = false;
                        Destroy( this );
                        return false;
                    }
                                        
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
        
        // Notify observers that are subscribed to this specific event
        bool notified = false;
        TEventNotificationMap& observers = m_eventobservers[ eventid ];
        notifiedObservers.clear();

        TEventNotificationMap::iterator n = observers.begin();
        while ( n != observers.end() )
        {
            // Check if we have not already notified this observer
            notified = false;
            oPtr = (*n).first;
            if ( notifiedObservers.find( oPtr ) == notifiedObservers.end() )
            {
                // Perform the notification
                i = m_observers.find( oPtr );
                if ( i != m_observers.end() )
                {
                    // Check if this observer is also registered as interested in all
                    // events. In such a case we don't have to notify this observer
                    // any more because it has already been notified in the generic
                    // event notification section above this one.
                    if ( !(*i).second )
                    {
                        // Check if we should perform notification using the generic
                        // handler or a user specified callback.
                        CIEventHandlerFunctorBase* callback = (*n).second;
                        if ( NULL != callback )
                        {
                            callback->OnNotify( m_ownerNotifier ,
                                                eventid         ,
                                                eventData       );
                        }
                        else
                        {
                            oPtr->OnNotify( m_ownerNotifier ,
                                            eventid         ,
                                            eventData       );
                        }
                        notified = true;
                    }                    
                }
                else
                {
                    // We should not get here, but we will pass on the event anyway
                    GUCEF_ASSERT_ALWAYS;
                    oPtr->OnNotify( m_ownerNotifier  ,
                                    eventid          ,
                                    eventData        );
                    notified = true;
                }
                
                if ( notified )
                {
                    // Check if someone deleted our owner notifier
                    if ( m_ownerNotifier == NULL )
                    {
                        // Gracefully handle the destruction sequence
                        m_isBusy = false;
                        Destroy( this );
                        return false;
                    }                    
                    
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
        
        
        m_isBusy = false;

        if ( m_scheduledForDestruction )
        {
            UnlockData();
            delete m_ownerNotifier;
            return false;
        }        
        
        // Now we check if any requests where received while we where busy
        ProcessMailbox();        
    }
    else
    {
        // This notifier is already busy processing a notification
        // We will store the request until we are done
        if ( eventData != NULL )
        {
            m_eventMailStack.push_back( TEventMailElement( eventid, eventData->Clone() ) );
        }
        else
        {
            m_eventMailStack.push_back( TEventMailElement( eventid, NULL ) );
        }
    }
    
    UnlockData();    
    
    return true;
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::ProcessCmdMailbox( void )
{GUCEF_TRACE;

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
                               cmdMailElement.eventID  ,
                               cmdMailElement.callback ); 
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
            case REQUEST_UNSUBSCRIBE_ALL :
            {
                UnsubscribeAllFromNotifier();
                break;
            }
            default :
            {
                // We should not get here
                GUCEF_UNREACHABLE;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::ProcessEventMailbox( void )
{GUCEF_TRACE;
    
    // Now we check if any notifications requests where received while we where busy
    if ( !m_eventMailStack.empty() )
    {
        TEventMailElement entry( m_eventMailStack[ m_eventMailStack.size()-1 ] );            
        m_eventMailStack.pop_back();        
        
        // Dispatch the message in our mailbox
        // If the mailbox has any more items remaining we will end up back here 
        // for the next mail item
        NotifyObservers( entry.first, entry.second );            
        
        delete entry.second;
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::ProcessMailbox( void )
{GUCEF_TRACE;

    ProcessCmdMailbox();
    ProcessEventMailbox();
}

/*-------------------------------------------------------------------------*/

bool
CNotifierImplementor::NotifyObservers( CNotifier& sender                   ,
                                       const CEvent& eventid               ,
                                       CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;

    // Use the friend relationship to access the NotifyObservers()
    // member function of the given notifier.
    return sender.NotifyObservers( eventid, eventData );
}                            

/*-------------------------------------------------------------------------*/

void 
CNotifierImplementor::OnObserverDestroy( CObserver* observer )
{GUCEF_TRACE;

    LockData();

    UnsubscribeFromAllEvents( observer ,
                              false    );
                              
    UnlockData();                                  
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::OnDeathOfOwnerNotifier( void )
{GUCEF_TRACE;

    /*
     *  Notify observers that the notifier is die-ing
     */
    ForceNotifyObserversOnce( CNotifier::DestructionEvent );
    
    /*
     *  Now we will remove references to the notifier at the observers
     */   
    UnsubscribeAllFromNotifier();

    /*
     *  This is an important step,..
     *  We null the owner pointer which functions as a flag signaling that
     *  the owner notifier is now considered to be destroyed.
     */
    m_ownerNotifier = NULL;
    
    if ( !m_isBusy )
    {
        /*
         *  We are not busy, meaning we did not arrive here trough some action
         *  trigger by the notifier itself. Thus we can safely delete ourselves 
         *  right now.
         */
        Destroy( this );          
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::ScheduleForDestruction( void )
{
   LockData();
   
   if ( !m_isBusy )
    {
        /*
         *  We are not busy, meaning we did not arrive here trough some action
         *  trigger by the notifier itself. Thus we can safely delete the notifier
         */
        UnlockData();
        delete m_ownerNotifier;          
    }
    else
    {
        /*
         *  We are busy and the user has opted to perform a delayed delete
         *  We will simply set a flag signaling that the notifier should be deleted
         *  once it finishes it's current operation
         */
        m_scheduledForDestruction = true;
        UnlockData();
    }    
}

/*-------------------------------------------------------------------------*/

void 
CNotifierImplementor::LockData( void ) const
{GUCEF_TRACE;

    if ( m_ownerNotifier != NULL )
    {
        m_ownerNotifier->LockData();
    }
}

/*-------------------------------------------------------------------------*/
    
void 
CNotifierImplementor::UnlockData( void ) const
{GUCEF_TRACE;

    if ( m_ownerNotifier != NULL )
    {
        m_ownerNotifier->UnlockData();
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
