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
    : CITypeNamed() ,
      m_imp( NULL )
{GUCEF_TRACE;

    RegisterEvents();

    m_imp = CNotifierImplementor::Create( this );
    assert( m_imp != NULL );
}

/*-------------------------------------------------------------------------*/

CNotifier::CNotifier( const CNotifier& src )
    : CITypeNamed() ,
      m_imp( NULL )
{GUCEF_TRACE;

    RegisterEvents();

    m_imp = CNotifierImplementor::Create( this, src );
    assert( m_imp != NULL );
}

/*-------------------------------------------------------------------------*/

CNotifier&
CNotifier::operator=( const CNotifier& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        if ( NULL != src.m_imp )
        {
            (*m_imp) = (*src.m_imp);
        }
        else
        {
            m_imp = NULL;
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CNotifier::~CNotifier()
{GUCEF_TRACE;

    if ( NULL != m_imp )
    {
        m_imp->OnDeathOfOwnerNotifier();
    }
    m_imp = NULL;
}

/*-------------------------------------------------------------------------*/

void
CNotifier::RegisterEvents( void )
{GUCEF_TRACE;

    ModifyEvent.Initialize();
    DestructionEvent.Initialize();
    SubscribeEvent.Initialize();
    UnsubscribeEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void
CNotifier::Subscribe( CObserver* observer )
{GUCEF_TRACE;

    if ( NULL != m_imp )
    {
        m_imp->Subscribe( observer );
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::Subscribe( CObserver* observer                              ,
                      const CEvent& eventid                            ,
                      CIEventHandlerFunctorBase* callback /* = NULL */ )
{GUCEF_TRACE;

    if ( NULL != m_imp )
    {
        m_imp->Subscribe( observer ,
                          eventid  ,
                          callback );
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::Unsubscribe( CObserver* observer )
{GUCEF_TRACE;

    if ( NULL != m_imp )
    {
        m_imp->Unsubscribe( observer );
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::Unsubscribe( CObserver* observer   ,
                        const CEvent& eventid )
{GUCEF_TRACE;

    if ( NULL != m_imp )
    {
        m_imp->Unsubscribe( observer ,
                            eventid  );
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::UnsubscribeAllFromNotifier( void )
{GUCEF_TRACE;

    if ( NULL != m_imp )
    {
        m_imp->UnsubscribeAllFromNotifier();
    }
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::NotifyObservers( void )
{GUCEF_TRACE;

    if ( NULL != m_imp )
    {
        return m_imp->NotifyObservers();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::NotifyObservers( const CEvent& eventid  ,
                            CICloneable* eventData )
{GUCEF_TRACE;

    if ( NULL != m_imp )
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
                            CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;

    if ( NULL != m_imp )
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
                                   CICloneable* eventData      )
{GUCEF_TRACE;

    if ( NULL != m_imp )
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
                                   CICloneable* eventData      )
{GUCEF_TRACE;

    if ( NULL != m_imp )
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

    if ( NULL != m_imp )
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

    if ( NULL != m_imp )
    {
        m_imp->ScheduleForDestruction();
    }
}

/*-------------------------------------------------------------------------*/

void
CNotifier::OnObserverDestruction( CObserver* observer )
{GUCEF_TRACE;

    /* can be implemented in a descending class to add observer destruction handling */
}

/*-------------------------------------------------------------------------*/

void
CNotifier::LockData( void ) const
{GUCEF_TRACE;

    /* can be implemented in a descending class to add thread-safety */
}

/*-------------------------------------------------------------------------*/

void
CNotifier::UnlockData( void ) const
{GUCEF_TRACE;

    /* can be implemented in a descending class to add thread-safety */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
