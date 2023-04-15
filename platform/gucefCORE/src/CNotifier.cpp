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

#ifndef GUCEF_CORE_CNOTIFIERIMPLEMENTOR_H
#include "CNotifierImplementor.h"
#define GUCEF_CORE_CNOTIFIERIMPLEMENTOR_H
#endif /* GUCEF_CORE_CNOTIFIERIMPLEMENTOR_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#include "CNotifier.h"

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

CNotifier::CNotifier( bool registerStdEvents )
    : CINotifier( registerStdEvents )
    , m_imp( GUCEF_NULL )
{GUCEF_TRACE;

    if ( registerStdEvents )
        RegisterEvents();

    m_imp = CNotifierImplementor::Create( this );
    assert( m_imp != GUCEF_NULL );
}

/*-------------------------------------------------------------------------*/

CNotifier::CNotifier( const CNotifier& src )
    : CINotifier()      
    , m_imp( GUCEF_NULL )
{GUCEF_TRACE;

    m_imp = CNotifierImplementor::Create( this, src );
    assert( m_imp != GUCEF_NULL );
}

/*-------------------------------------------------------------------------*/

CNotifier&
CNotifier::operator=( const CNotifier& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CINotifier::operator=( src );

        if ( GUCEF_NULL != src.m_imp )
        {
            (*m_imp) = (*src.m_imp);
        }
        else
        {
            m_imp = GUCEF_NULL;
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CNotifier::~CNotifier()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        m_imp->OnDeathOfOwnerNotifier();
    }
    m_imp = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::Subscribe( CObserver* observer )
{GUCEF_TRACE;

    if ( GUCEF_NULL != observer && GUCEF_NULL != m_imp )
    {
        return m_imp->Subscribe( observer );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::Subscribe( CObserver* observer                                    ,
                      const CEvent& eventid                                  ,
                      CIEventHandlerFunctorBase* callback /* = GUCEF_NULL */ )
{GUCEF_TRACE;

    if ( GUCEF_NULL != observer && GUCEF_NULL != m_imp )
    {
        return m_imp->Subscribe( observer ,
                                 eventid  ,
                                 callback );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CNotifier::Unsubscribe( CObserver* observer )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        m_imp->Unsubscribe( observer );
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::Unsubscribe( CObserver* observer   ,
                        const CEvent& eventid )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        m_imp->Unsubscribe( observer ,
                            eventid  );
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::UnsubscribeAllFromNotifier( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        m_imp->UnsubscribeAllFromNotifier( false );
    }
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::NotifyObservers( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        return m_imp->NotifyObservers();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::NotifyObservers( const CEvent& eventid  ,
                            CICloneable* eventData ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        return m_imp->NotifyObservers( eventid   ,
                                       eventData );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::NotifyObservers( CNotifier& sender                   ,
                            const CEvent& eventid               ,
                            CICloneable* eventData /* = NULL */ ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        return m_imp->NotifyObservers( sender    ,
                                       eventid   ,
                                       eventData );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::NotifySpecificObserver( CNotifier& sender           ,
                                   CObserver& specificObserver ,
                                   const CEvent& eventid       ,
                                   CICloneable* eventData      ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        return m_imp->NotifySpecificObserver( sender           ,
                                              specificObserver ,
                                              eventid          ,
                                              eventData        );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::NotifySpecificObserver( CObserver& specificObserver ,
                                   const CEvent& eventid       ,
                                   CICloneable* eventData      ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        return m_imp->NotifySpecificObserver( specificObserver ,
                                              eventid          ,
                                              eventData        );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CNotifier::OnObserverDestroy( CObserver* observer )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        m_imp->OnObserverDestroy( observer );
    }
}

/*-------------------------------------------------------------------------*/

const CString&
CNotifier::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::CORE::CNotifier";
    return typeName;
}

/*-------------------------------------------------------------------------*/

void
CNotifier::ScheduleForDestruction( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        m_imp->ScheduleForDestruction();
    }
}

/*-------------------------------------------------------------------------*/

UInt32 
CNotifier::GetSubscriptionCountForObserver( CObserver* observer ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_imp )
    {
        return m_imp->GetSubscriptionCountForObserver( observer );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CNotifier::OnObserverDestruction( CObserver* observer )
{GUCEF_TRACE;

    /* can be implemented in a descending class to add observer destruction handling */
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CNotifier::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CNotifier::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    /* can be implemented in a descending class to add thread-safety */
    return MT::LOCKSTATUS_NOT_APPLICABLE;
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CNotifier::Unlock( void ) const
{GUCEF_TRACE;

    /* can be implemented in a descending class to add thread-safety */
    return MT::LOCKSTATUS_NOT_APPLICABLE;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
