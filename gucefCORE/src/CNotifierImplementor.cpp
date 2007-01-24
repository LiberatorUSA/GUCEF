 /*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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
{TRACE;

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
{TRACE;

    //@TODO: makeme
    assert( 0 );
    
    assert( m_ownerNotifier != NULL );
}

/*-------------------------------------------------------------------------*/

CNotifierImplementor&
CNotifierImplementor::operator=( const CNotifierImplementor& src )
{TRACE;

    assert( 0 ); //@TODO: makeme

    if ( this != &src )
    {
    }
    return *this; 
}    

/*-------------------------------------------------------------------------*/

CNotifierImplementor::~CNotifierImplementor()
{TRACE;
   
    /*
     *  The notifier should no longer be busy with some action if it is beeing destroyed.
     *  If it is still busy then the code would resume after the object is deleted which
     *  causes undefined behavior possibly resulting in a crash.
     *  If you hit this assert then you have found a bug in the logic of this class
     */
    assert( !m_isBusy );    
}

/*-------------------------------------------------------------------------*/

CNotifierImplementor*
CNotifierImplementor::Create( CNotifier* ownerNotifier )
{TRACE;

    // perform in-scope creation
    return new CNotifierImplementor( ownerNotifier );
}

/*-------------------------------------------------------------------------*/

CNotifierImplementor*
CNotifierImplementor::Create( CNotifier* ownerNotifier ,
                              const CNotifier& src     )
{TRACE;

    // perform in-scope creation
    return new CNotifierImplementor( ownerNotifier ,
                                     src           );
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::Destroy( CNotifierImplementor* obj )
{TRACE;
    
    // perform an in-scope destruction
    delete obj;
}

/*-------------------------------------------------------------------------*/

bool
CNotifierImplementor::ForceNotifyObserversOnce( const CEvent& eventid , 
                                                CICloneable* data     )
{TRACE;

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
CNotifierImplementor::Subscribe( CObserver* observer )
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
                return false;
            }
            
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
CNotifierImplementor::Subscribe( CObserver* observer   ,
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
        if ( ( eventid != CNotifier::DestructionEvent ) &&
             ( eventid != CNotifier::ModifyEvent )      &&
             ( eventid != CNotifier::SubscribeEvent )   &&
             ( eventid != CNotifier::UnsubscribeEvent )  ) 
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
                observer->LinkTo( m_ownerNotifier );

                /*
                 *  Send the standard subscription event
                 */
                observer->OnNotify( m_ownerNotifier            ,
                                    CNotifier::SubscribeEvent  ,
                                    NULL                       );
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
                observer->LinkTo( m_ownerNotifier );

                /*
                 *  Send the standard subscription event
                 */
                observer->OnNotify( m_ownerNotifier            ,
                                    CNotifier::SubscribeEvent  ,
                                    NULL                       );
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
        m_cmdMailStack.push_back( cmdMailElement );    
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::Unsubscribe( CObserver* observer )
{TRACE;

    LockData();
    UnsubscribeFromAllEvents( observer ,
                              true     );
    UnlockData();                                  
}

/*-------------------------------------------------------------------------*/

void 
CNotifierImplementor::UnsubscribeFromAllEvents( CObserver* observer       ,
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
        cmdMailElement.eventID = CEvent();
        cmdMailElement.observer = observer;
        cmdMailElement.notify = notifyObserver;
        m_cmdMailStack.push_back( cmdMailElement );
    }    
}

/*-------------------------------------------------------------------------*/                
 
void 
CNotifierImplementor::Unsubscribe( CObserver* observer   ,
                                   const CEvent& eventid )
{TRACE;

    LockData();
    
    if ( !m_isBusy )
    {
        m_isBusy = true;
        
        if ( ( eventid != CNotifier::DestructionEvent ) &&
             ( eventid != CNotifier::SubscribeEvent )   &&
             ( eventid != CNotifier::UnsubscribeEvent ) &&
             ( eventid != CNotifier::ModifyEvent )       )
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
        cmdMailElement.notify = true;
        m_cmdMailStack.push_back( cmdMailElement );    
    }    
    UnlockData();
}

/*-------------------------------------------------------------------------*/                        

bool 
CNotifierImplementor::NotifyObservers( void )
{TRACE;

    LockData();
    bool aliveState = ForceNotifyObserversOnce( CNotifier::ModifyEvent );
    UnlockData();
    return aliveState;
}

/*-------------------------------------------------------------------------*/
    
bool 
CNotifierImplementor::NotifyObservers( const CEvent& eventid  ,
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
                    oPtr->OnNotify( m_ownerNotifier  ,
                                    eventid          ,
                                    eventData        );
                    notified = true;
                }
                else
                {
                    if ( !(*i).second )
                    {
                        oPtr->OnNotify( m_ownerNotifier ,
                                        eventid         ,
                                        eventData       );
                        notified = true;
                    }
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
        m_eventMailStack.push_back( TEventMailElement( eventid, eventData ) );
    }
    
    UnlockData();    
    
    return true;
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::ProcessCmdMailbox( void )
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
CNotifierImplementor::ProcessEventMailbox( void )
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
CNotifierImplementor::ProcessMailbox( void )
{TRACE;

    ProcessCmdMailbox();
    ProcessEventMailbox();
}

/*-------------------------------------------------------------------------*/

bool
CNotifierImplementor::NotifyObservers( CNotifier& sender                   ,
                                       const CEvent& eventid               ,
                                       CICloneable* eventData /* = NULL */ )
{TRACE;

    // Use the friend relationship to access the NotifyObservers()
    // member function of the given notifier.
    return sender.NotifyObservers( eventid, eventData );
}                            

/*-------------------------------------------------------------------------*/

void 
CNotifierImplementor::OnObserverDestroy( CObserver* observer )
{TRACE;

    LockData();

    UnsubscribeFromAllEvents( observer ,
                              false    );
                              
    UnlockData();                                  
}

/*-------------------------------------------------------------------------*/

void
CNotifierImplementor::OnDeathOfOwnerNotifier( void )
{TRACE;

    /*
     *  Notify observers that the notifier is die-ing
     */
    ForceNotifyObserversOnce( CNotifier::DestructionEvent );
    
    /*
     *  Now we will remove references to the notifier at the observers
     */   
    TObserverList::iterator i = m_observers.begin();
    while ( i != m_observers.end() )
    {
        (*i).first->UnlinkFrom( m_ownerNotifier, true );
        ++i;
    }

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
         *  We will simply set a flag signalling that the notifier should be deleted
         *  once it finishes it's current operation
         */
        m_scheduledForDestruction = true;
        UnlockData();
    }    
}

/*-------------------------------------------------------------------------*/

void 
CNotifierImplementor::LockData( void ) const
{TRACE;

    if ( m_ownerNotifier != NULL )
    {
        m_ownerNotifier->LockData();
    }
}

/*-------------------------------------------------------------------------*/
    
void 
CNotifierImplementor::UnlockData( void ) const
{TRACE;

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
