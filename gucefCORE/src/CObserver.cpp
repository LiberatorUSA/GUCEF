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

#include "CNotificationIDRegistry.h"
#include "CNotifier.h"

#define GUCEF_CORE_COBSERVER_CPP
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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CObserver::CObserver( void )
{TRACE;

    /*
     *  Make sure that the standard notification events are registered
     */
    CNotifier::RegisterEvents();

    /*
     *  Get the unique event id's for the standard notification events
     */
    CNotificationIDRegistry* registry = CNotificationIDRegistry::Instance();
    m_notifierModifyEvent = registry->Lookup( CNotifier::ModifyEvent );
    m_notifierDestructionEvent = registry->Lookup( CNotifier::DestructionEvent );            
    m_notifierSubscribeEvent = registry->Lookup( CNotifier::SubscribeEvent );
    m_notifierUnsubscribeEvent = registry->Lookup( CNotifier::UnsubscribeEvent );
}

/*-------------------------------------------------------------------------*/

CObserver::CObserver( const CObserver& src )
{TRACE;

    //@TODO: makeme
    assert( 0 );
}

/*-------------------------------------------------------------------------*/

CObserver::~CObserver()
{TRACE;

    LockData();
    
    /*
     *  Neatly unsubscribe from all notifiers
     */
    TNotifierList listcopy( m_notifiers );
    TNotifierList::iterator i( listcopy.begin() );
    while ( i != listcopy.end() )
    {
        /*
         *  It might not be evident here but it is the notifier
         *  who updates our administration which is why we had to make a copy of
         *  our notifier list.
         */
        (*i).notifier->OnObserverDestroy( this );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

CObserver&
CObserver::operator=( const CObserver& src )
{TRACE;

    assert( 0 );
    if ( this != &src )
    {
    }
    return *this; //@TODO: makeme
}

/*-------------------------------------------------------------------------*/

void 
CObserver::UnsubscribeAll( void )
{TRACE;

    LockData();
    
    /*
     *  Neatly unsubscribe from all notifiers
     */
    TNotifierList listcopy( m_notifiers );
    TNotifierList::iterator i( listcopy.begin() );
    while ( i != listcopy.end() )
    {
        /*
         *  It might not be evident here but it is the notifier
         *  who updates our administration which is why we had to make a copy of
         *  our notifier list.
         */
        (*i).notifier->Unsubscribe( this );
        ++i;
    }
    
    UnlockData();    
}

/*-------------------------------------------------------------------------*/                        
                   
void 
CObserver::SubscribeTo( CNotifier* notifier )
{TRACE;

    LockData();
    
    TNotifierList::iterator i( m_notifiers.begin() );
    while ( i != m_notifiers.end() )
    {
        if ( (*i).notifier == notifier )
        {
            /*
             *  Already subscribed to this notifier
             */
            (*i).refCount++;
            UnlockData(); 
            return;
        }
        ++i;
    }
    
    /*
     *  If we get here then this is a new notifier and it should be added 
     *  to our list
     */
    TNotifierRef newNotifierEntry;
    newNotifierEntry.notifier = notifier;
    newNotifierEntry.refCount = 1;
    m_notifiers.push_back( newNotifierEntry );
    
    UnlockData();         
}

/*-------------------------------------------------------------------------*/                            
           
void          
CObserver::UnlinkFrom( CNotifier* notifier                   ,
                       const bool forAllEvents /* = false */ )
{TRACE;

    LockData();
    
    TNotifierList::iterator i( m_notifiers.begin() );
    while ( i != m_notifiers.end() )
    {
        if ( (*i).notifier == notifier )
        {
            if ( !forAllEvents )
            {
                /*
                 *  Found the notifier in question.
                 *  reducing refrence count
                 */
                (*i).refCount--;
                if ( (*i).refCount == 0 )
                {
                    /*
                     *  Remove the notifier from our list
                     */
                    m_notifiers.erase( i ); 
                }
                UnlockData(); 
                return;
            }

            m_notifiers.erase( i );
            UnlockData(); 
            return;
        }
        ++i;
    }
    
    UnlockData();   
}

/*-------------------------------------------------------------------------*/

UInt32 
CObserver::GetSubscriptionCount( void )
{TRACE;
    
    UInt32 subscriptionCount( 0 );
    LockData();
    TNotifierList::const_iterator i( m_notifiers.begin() );
    while ( i != m_notifiers.end() )
    {
        subscriptionCount += (*i).refCount;
        ++i;
    }
    UnlockData(); 
    return subscriptionCount;       
}

/*-------------------------------------------------------------------------*/

UInt32 
CObserver::GetNotifierCount( void ) const
{TRACE;

    return (UInt32) m_notifiers.size();
}

/*-------------------------------------------------------------------------*/

void
CObserver::UnsubscribeFrom( CNotifier& notifier )
{TRACE;

    assert( &notifier );
    notifier.Unsubscribe( this );
}

/*-------------------------------------------------------------------------*/

void 
CObserver::LockData( void )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
void 
CObserver::UnlockData( void )
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
