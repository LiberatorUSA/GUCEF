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

#define GUCEF_CORE_COBSERVINGNOTIFIER_CPP
#include "CObservingNotifier.h"

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

CObservingNotifier::CObservingNotifier( void )
{
    m_observer.SetOwner( this );
}

/*-------------------------------------------------------------------------*/
    
CObservingNotifier::CObservingNotifier( const CObservingNotifier& src )
    : m_observer( src.m_observer )
{
    m_observer.SetOwner( this );
}

/*-------------------------------------------------------------------------*/
    
CObservingNotifier::~CObservingNotifier()
{
}

/*-------------------------------------------------------------------------*/
    
CObservingNotifier& 
CObservingNotifier::operator=( const CObservingNotifier& src )
{
    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
void 
CObservingNotifier::UnsubscribeAllFromObserver( void )
{
    LockData();
    m_observer.UnsubscribeAll();
    UnlockData();
}
    
/*-------------------------------------------------------------------------*/

UInt32 
CObservingNotifier::GetObserverSubscriptionCount( void )
{
    LockData();
    UInt32 retval( m_observer.GetSubscriptionCount() );
    UnlockData();
    return retval;
}

/*-------------------------------------------------------------------------*/

UInt32 
CObservingNotifier::GetObserverNotifierCount( void )
{
    LockData();
    UInt32 retval( m_observer.GetNotifierCount() );
    UnlockData();
    return retval;
}

/*-------------------------------------------------------------------------*/

void 
CObservingNotifier::SubscribeTo( CNotifier* notifier )
{
    LockData();
    notifier->Subscribe( &m_observer );
    UnlockData();
}

/*-------------------------------------------------------------------------*/
    
void 
CObservingNotifier::SubscribeTo( CNotifier* notifier  ,
                                   const UInt32 eventid )
{
    LockData();
    notifier->Subscribe( &m_observer ,
                         eventid     );
    UnlockData();
}                                   

/*-------------------------------------------------------------------------*/

void 
CObservingNotifier::UnsubscribeFrom( CNotifier* notifier )
{
    LockData();
    notifier->Unsubscribe( &m_observer );
    UnlockData();
}

/*-------------------------------------------------------------------------*/
                 
void 
CObservingNotifier::UnsubscribeFrom( CNotifier* notifier  ,
                                       const UInt32 eventid )
{
    LockData();
    notifier->Unsubscribe( &m_observer ,
                           eventid     );
    UnlockData();
}

/*-------------------------------------------------------------------------*/

CObserver& 
CObservingNotifier::AsObserver( void )
{
    return m_observer;
}

/*-------------------------------------------------------------------------*/

const CObserver& 
CObservingNotifier::AsObserver( void ) const
{
    return m_observer;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
