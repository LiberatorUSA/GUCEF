/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#include "gucefCORE_CIObserver.h"

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
   
CIObserver::CIObserver( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CIObserver::CIObserver( const CIObserver& src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CIObserver::~CIObserver()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIObserver&
CIObserver::operator=( const CIObserver& src )
{GUCEF_TRACE;

    return *this;
}   

/*-------------------------------------------------------------------------*/

CObserverScopeLock::CObserverScopeLock( const CIObserver* lockableObserver )
    : m_lockableObserver( lockableObserver )
    , m_isLocked( false )
{GUCEF_TRACE;

    if ( GUCEF_NULL != lockableObserver )    
        m_isLocked = lockableObserver->NotificationLock();
}

/*--------------------------------------------------------------------------*/

CObserverScopeLock::CObserverScopeLock( const CIObserver& lockableObserver )
    : m_lockableObserver( &lockableObserver )
    , m_isLocked( false )
{GUCEF_TRACE;

    assert( 0 != m_lockableObserver );        
    m_isLocked = m_lockableObserver->NotificationLock();
}

/*--------------------------------------------------------------------------*/

CObserverScopeLock::~CObserverScopeLock()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_lockableObserver && m_isLocked )
    {
        m_isLocked = !m_lockableObserver->NotificationUnlock();
    }
}

/*--------------------------------------------------------------------------*/

bool 
CObserverScopeLock::IsLocked( void ) const
{GUCEF_TRACE;

    return m_isLocked;
}

/*--------------------------------------------------------------------------*/

bool 
CObserverScopeLock::EarlyUnlock( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_lockableObserver && m_isLocked )
    {
        m_isLocked = !m_lockableObserver->NotificationUnlock();
        return !m_isLocked;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CObserverScopeReadOnlyLock::CObserverScopeReadOnlyLock( const CIObserver* lockableObserver )
    : m_lockableObserver( lockableObserver )
    , m_isLocked( false )
{GUCEF_TRACE;

    if ( GUCEF_NULL != lockableObserver )    
        m_isLocked = m_lockableObserver->NotificationReadOnlyLock();
}

/*--------------------------------------------------------------------------*/

CObserverScopeReadOnlyLock::CObserverScopeReadOnlyLock( const CIObserver& lockableObserver )
    : m_lockableObserver( &lockableObserver )
    , m_isLocked( false )
{GUCEF_TRACE;

    assert( 0 != m_lockableObserver );        
    m_isLocked = m_lockableObserver->NotificationReadOnlyLock();
}

/*--------------------------------------------------------------------------*/

CObserverScopeReadOnlyLock::~CObserverScopeReadOnlyLock()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_lockableObserver && m_isLocked )
    {
        m_isLocked = !m_lockableObserver->NotificationReadOnlyUnlock();
    }
}

/*--------------------------------------------------------------------------*/

bool 
CObserverScopeReadOnlyLock::IsLocked( void ) const
{GUCEF_TRACE;

    return m_isLocked;
}

/*--------------------------------------------------------------------------*/

bool 
CObserverScopeReadOnlyLock::EarlyReaderUnlock( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_lockableObserver && m_isLocked )
    {
        m_isLocked = !m_lockableObserver->NotificationReadOnlyUnlock();
        return !m_isLocked;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
