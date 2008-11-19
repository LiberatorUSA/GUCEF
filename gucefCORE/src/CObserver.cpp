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
#include "CNotificationIDRegistry.h"
#include "CNotifier.h"

#include "CObserver.h"

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CObserver::CObserver( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CObserver::CObserver( const CObserver& src )
{GUCEF_TRACE;

    //@TODO: makeme
    assert( 0 );
}

/*-------------------------------------------------------------------------*/

CObserver::~CObserver()
{GUCEF_TRACE;

    LockData();
    
    /*
     *  Neatly un-subscribe from all notifiers
     */
    TNotifierList::iterator i( m_notifiers.begin() );
    while ( i != m_notifiers.end() )
    {
        (*i).notifier->OnObserverDestroy( this );
        ++i;
    }
    m_notifiers.clear();
}

/*-------------------------------------------------------------------------*/

CObserver&
CObserver::operator=( const CObserver& src )
{GUCEF_TRACE;

    assert( 0 );
    if ( this != &src )
    {
    }
    return *this; //@TODO: makeme
}

/*-------------------------------------------------------------------------*/

void 
CObserver::UnsubscribeAllFromObserver( void )
{GUCEF_TRACE;

    LockData();
    
    /*
     *  Neatly un-subscribe from all notifiers
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
{GUCEF_TRACE;

    notifier->Subscribe( this );
}

/*-------------------------------------------------------------------------*/                        

void
CObserver::SubscribeTo( CNotifier* notifier                              ,
                        const CEvent& eventid                            ,
                        CIEventHandlerFunctorBase* callback /* = NULL */ )
{GUCEF_TRACE;

    notifier->Subscribe( this     ,
                         eventid  ,
                         callback );
}

/*-------------------------------------------------------------------------*/
                   
void 
CObserver::LinkTo( CNotifier* notifier )
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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
{GUCEF_TRACE;
    
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
{GUCEF_TRACE;

    return (UInt32) m_notifiers.size();
}

/*-------------------------------------------------------------------------*/

void
CObserver::UnsubscribeFrom( CNotifier& notifier )
{GUCEF_TRACE;

    assert( &notifier );
    notifier.Unsubscribe( this );
}

/*-------------------------------------------------------------------------*/

void 
CObserver::LockData( void ) const
{GUCEF_TRACE;

    // dummy to avoid mandatory implementation by descending classes
}

/*-------------------------------------------------------------------------*/
    
void 
CObserver::UnlockData( void ) const
{GUCEF_TRACE;

    // dummy to avoid mandatory implementation by descending classes
}

/*-------------------------------------------------------------------------*/

void
CObserver::OnNotify( CNotifier* notifier                 ,
                     const CEvent& eventid               ,
                     CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    // dummy to avoid mandatory implementation by descending classes
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
