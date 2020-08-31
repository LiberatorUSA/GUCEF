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

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#include "CEvent.h"

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

CEvent::CEvent( void )
    : m_eventID( 0 ) ,
      m_eventName()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( const char* eventName )
    : m_eventID( 0 )           ,
      m_eventName( eventName )
{GUCEF_TRACE;

    // Wait for delayed initialization
}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( const CORE::CString& eventName )
    : m_eventID( 0 )           ,
      m_eventName( eventName )
{GUCEF_TRACE;

    // Wait for delayed initialization
}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( const UInt32 eventID     ,
                const CString& eventName )
    : m_eventID( eventID )     ,
      m_eventName( eventName )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( const CEvent& src )
    : m_eventID( src.m_eventID )     ,
      m_eventName( src.m_eventName )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEvent::~CEvent()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEvent&
CEvent::operator=( const CEvent& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_eventName = src.m_eventName;
        m_eventID = src.m_eventID;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CEvent&
CEvent::operator=( const CORE::CString& eventName )
{GUCEF_TRACE;

    if ( eventName != m_eventName )
    {
        m_eventName = eventName;
        m_eventID = 0;    // <- Wait for delayed initialization
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CEvent&
CEvent::operator=( const char* eventName )
{GUCEF_TRACE;

    if ( eventName != m_eventName )
    {
        m_eventName = eventName;
        m_eventID = 0;    // <- Wait for delayed initialization
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CEvent::operator==( const CEvent& other ) const
{GUCEF_TRACE;

    // If you hit the asserts below then the event object was not initialized
    assert( m_eventID > 0 );
    assert( other.m_eventID > 0 );
    return m_eventID == other.m_eventID;
}

/*-------------------------------------------------------------------------*/

bool
CEvent::operator!=( const CEvent& other ) const
{GUCEF_TRACE;

    // If you hit the asserts below then the event object was not initialized
    assert( m_eventID > 0 );
    assert( other.m_eventID > 0 );
    return m_eventID != other.m_eventID;
}

/*-------------------------------------------------------------------------*/

bool
CEvent::operator<( const CEvent& other ) const
{GUCEF_TRACE;

    // If you hit the asserts below then the event object was not initialized
    assert( m_eventID > 0 );
    assert( other.m_eventID > 0 );
    return m_eventID < other.m_eventID;
}

/*-------------------------------------------------------------------------*/

UInt32
CEvent::GetID( void ) const
{GUCEF_TRACE;

    // If you hit the asserts below then the event object was not initialized
    assert( m_eventID > 0 );
    return m_eventID;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CEvent::GetName( void ) const
{GUCEF_TRACE;

    return m_eventName;
}

/*-------------------------------------------------------------------------*/

void
CEvent::Initialize( void ) const
{GUCEF_TRACE;

    // For performance reasons we don't perform the lookup again on a non-zero id
    // Since this is for performance only we don't need to worry about thread safety on
    // the branch as a race condition merely results in an extra lookup
    if ( 0 == m_eventID )
    {
        /*
         *  In this rare occasion a const_cast is legit since we are preforming
         *  an action that has conceptually already occurred,.. initialization.
         *
         *  The fact that we cannot initialize objects right from the start
         *  while allowing event objects to be defined globally is an implementation level problem.
         */
        const_cast< CEvent& >( *this ) = CORE::CCoreGlobal::Instance()->GetNotificationIDRegistry().Lookup( m_eventName, true );
    }
}

/*-------------------------------------------------------------------------*/

bool
CEvent::IsInitialized( void ) const
{GUCEF_TRACE;

    return m_eventID > 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
