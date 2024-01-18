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

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#include "gucefMT_CScopeMutex.h"     /* Our mutex class definition */

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

CScopeMutex::CScopeMutex( const CMutex& mutex, UInt32 lockWaitTimeoutInMs )
    : m_mutex( &mutex )
    , m_isLocked( false )
{GUCEF_TRACE;

    assert( GUCEF_NULL != m_mutex );
        
    TLockStatus lockStatus = m_mutex->Lock( lockWaitTimeoutInMs );
    m_isLocked = LockStatusToLockSuccessStatusBool( lockStatus );
    if ( LOCKSTATUS_WAIT_TIMEOUT == lockStatus )
         throw timeout_exception();    
}

/*--------------------------------------------------------------------------*/

CScopeMutex::CScopeMutex( const CMutex& mutex, UInt32 lockWaitTimeoutInMs, TLockStatus& lockStatus ) 
    : m_mutex( &mutex )
    , m_isLocked( false )
{GUCEF_TRACE;

    assert( GUCEF_NULL != m_mutex );
        
    lockStatus = m_mutex->Lock( lockWaitTimeoutInMs );
    m_isLocked = LockStatusToLockSuccessStatusBool( lockStatus );
}

/*--------------------------------------------------------------------------*/

CScopeMutex::~CScopeMutex()
{GUCEF_TRACE;

    if ( m_isLocked )
        m_isLocked = !LockStatusToLockSuccessStatusBool( m_mutex->Unlock() );
}

/*--------------------------------------------------------------------------*/

bool 
CScopeMutex::IsLocked( void ) const
{GUCEF_TRACE;

    return m_isLocked;
}

/*--------------------------------------------------------------------------*/

bool
CScopeMutex::EarlyUnlock( void )
{GUCEF_TRACE;

    if ( m_isLocked )
    {
        m_isLocked = !LockStatusToLockSuccessStatusBool( m_mutex->Unlock() );
        return !m_isLocked;
    }
    return false;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeMutex::ReLock( UInt32 lockWaitTimeoutInMs )
{GUCEF_TRACE;

    if ( !m_isLocked )
    {
        TLockStatus lockStatus = m_mutex->Lock( lockWaitTimeoutInMs );
        m_isLocked = LockStatusToLockSuccessStatusBool( lockStatus );
        if ( LOCKSTATUS_WAIT_TIMEOUT == lockStatus )
             throw timeout_exception();
        return m_isLocked;
    }
    return false;
}

/*--------------------------------------------------------------------------*/

TLockStatus 
CScopeMutex::TryReLock( UInt32 lockWaitTimeoutInMs )
{GUCEF_TRACE;

    if ( !m_isLocked )
    {
        TLockStatus lockStatus = m_mutex->Lock( lockWaitTimeoutInMs );
        m_isLocked = LockStatusToLockSuccessStatusBool( lockStatus );
        return lockStatus;
    }
    return LOCKSTATUS_NOT_APPLICABLE;
}

/*--------------------------------------------------------------------------*/

const CMutex* 
CScopeMutex::GetMutex( void ) const
{GUCEF_TRACE;

    return m_mutex;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
