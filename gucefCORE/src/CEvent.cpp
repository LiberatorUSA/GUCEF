/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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
    : m_eventID( 0 )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( const CORE::CString& eventName )
    : m_eventID( 0 )           ,
      m_eventName( eventName )
{TRACE;
    
    // Wait for lazy initialization
}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( const UInt32 eventID     ,
                const CString& eventName )
    : m_eventID( eventID )     ,
      m_eventName( eventName )
{TRACE;
    
}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( const CEvent& src )
    : m_eventID( src.m_eventID )     ,
      m_eventName( src.m_eventName )
{TRACE;
    
}

/*-------------------------------------------------------------------------*/

CEvent::~CEvent()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CEvent&
CEvent::operator=( const CEvent& src )
{TRACE;

    if ( &src != this )
    {
        m_eventID = src.m_eventID;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CEvent&
CEvent::operator=( const CORE::CString& eventName )
{TRACE;
    
    m_eventName = eventName;
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CEvent::operator==( const CEvent& other ) const
{TRACE;
    
    return m_eventID == other.m_eventID;
}

/*-------------------------------------------------------------------------*/
    
bool
CEvent::operator!=( const CEvent& other ) const
{TRACE;
    
    return m_eventID != other.m_eventID;
}

/*-------------------------------------------------------------------------*/

bool
CEvent::operator<( const CEvent& other ) const
{TRACE;
    
    return m_eventID < other.m_eventID;
}

/*-------------------------------------------------------------------------*/

UInt32
CEvent::GetID( void ) const
{TRACE;

    return m_eventID;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CEvent::GetName( void ) const
{TRACE;

    return m_eventName;
}

/*-------------------------------------------------------------------------*/

void
CEvent::Initialize( void ) const
{TRACE;
    /*
     *  In this rare occasion a const_cast is legit since we are preforming 
     *  an action that has conceptually already occurred,.. initialization.
     *
     *  The fact that we cannot initialize objects right from the start  
     *  while allowing event objects to be defined globally is an implementation level problem.
     */
    const_cast< CEvent& >( *this ) = CNotificationIDRegistry::Instance()->Lookup( m_eventName, true );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/