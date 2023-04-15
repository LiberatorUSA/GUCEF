/*
 *  gucefMT: GUCEF module providing multithreading solutions
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#include "gucefMT_CObjectScopeReadOnlyLock.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CObjectScopeReadOnlyLock::CObjectScopeReadOnlyLock( const CILockable* lockableObject, UInt32 lockWaitTimeoutInMs )
    : m_lockableObject( lockableObject )
    , m_isLocked( false )
{GUCEF_TRACE;

    if ( GUCEF_NULL != lockableObject )
    {
        TLockStatus lockStatus = m_lockableObject->ReadOnlyLock( lockWaitTimeoutInMs );
        m_isLocked = LockStatusToLockSuccessStatusBool( lockStatus );
        if ( lockStatus == LOCKSTATUS_WAIT_TIMEOUT )
            throw timeout_exception();
    }
}

/*--------------------------------------------------------------------------*/

CObjectScopeReadOnlyLock::CObjectScopeReadOnlyLock( const CILockable& lockableObject, UInt32 lockWaitTimeoutInMs )
    : m_lockableObject( &lockableObject )
    , m_isLocked( false )
{GUCEF_TRACE;

    assert( 0 != m_lockableObject );        
    TLockStatus lockStatus = m_lockableObject->ReadOnlyLock( lockWaitTimeoutInMs );
    m_isLocked = LockStatusToLockSuccessStatusBool( lockStatus );
    if ( lockStatus == LOCKSTATUS_WAIT_TIMEOUT )
        throw timeout_exception();
}

/*--------------------------------------------------------------------------*/

CObjectScopeReadOnlyLock::CObjectScopeReadOnlyLock( const CILockable* lockableObject, UInt32 lockWaitTimeoutInMs, TLockStatus& lockStatus )
    : m_lockableObject( lockableObject )
    , m_isLocked( false )
{GUCEF_TRACE;

    if ( GUCEF_NULL != lockableObject )
    {
        lockStatus = m_lockableObject->ReadOnlyLock( lockWaitTimeoutInMs );
        m_isLocked = LockStatusToLockSuccessStatusBool( lockStatus );
    }
}

/*--------------------------------------------------------------------------*/

CObjectScopeReadOnlyLock::CObjectScopeReadOnlyLock( const CILockable& lockableObject, UInt32 lockWaitTimeoutInMs, TLockStatus& lockStatus )
    : m_lockableObject( &lockableObject )
    , m_isLocked( false )
{GUCEF_TRACE;

    assert( 0 != m_lockableObject );        
    lockStatus = m_lockableObject->ReadOnlyLock( lockWaitTimeoutInMs );
    m_isLocked = LockStatusToLockSuccessStatusBool( lockStatus );
}

/*--------------------------------------------------------------------------*/

CObjectScopeReadOnlyLock::~CObjectScopeReadOnlyLock()
{GUCEF_TRACE;

    if ( m_isLocked )
    {
        m_isLocked = !m_lockableObject->ReadOnlyUnlock();
    }
}

/*--------------------------------------------------------------------------*/

bool 
CObjectScopeReadOnlyLock::IsLocked( void ) const
{GUCEF_TRACE;

    return m_isLocked;
}

/*--------------------------------------------------------------------------*/

bool 
CObjectScopeReadOnlyLock::EarlyReaderUnlock( void )
{GUCEF_TRACE;

    if ( m_isLocked )
    {
        m_isLocked = !m_lockableObject->ReadOnlyUnlock();
        return !m_isLocked;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
