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

#include "gucefMT_CScopeRwLock.h"

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

CScopeReaderLock::CScopeReaderLock( const CReadWriteLock& rwLock )
    : m_rwLock( &rwLock )
    , m_lockState( TRWLockStates::RWLOCK_OPERATION_FAILED )
{GUCEF_TRACE;

    assert( m_rwLock );
    m_lockState = m_rwLock->ReaderStart();
    m_isReadLocked = TRWLockStates::RWLOCK_OPERATION_SUCCESS == m_lockState;
}

/*--------------------------------------------------------------------------*/

CScopeReaderLock::~CScopeReaderLock()
{GUCEF_TRACE;

    if ( m_isReadLocked )
    {
        m_lockState = m_rwLock->ReaderStop();
        m_isReadLocked = TRWLockStates::RWLOCK_OPERATION_SUCCESS == m_lockState;
    }
}

/*--------------------------------------------------------------------------*/

bool 
CScopeReaderLock::HasActiveReaders( void ) const
{GUCEF_TRACE;

    return m_rwLock->ActiveReaderCount() > 0;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeReaderLock::IsScopeReadLocked( void ) const
{GUCEF_TRACE;

    return m_isReadLocked;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeReaderLock::HasActiveWriters( void ) const
{GUCEF_TRACE;

    return m_rwLock->ActiveWriterCount() > 0;
}

/*--------------------------------------------------------------------------*/

CScopeReaderLock::TRWLockStates 
CScopeReaderLock::EarlyUnlock( void )
{GUCEF_TRACE;

    if ( m_isReadLocked )
    {        
        m_lockState = m_rwLock->ReaderStop();
        m_isReadLocked = TRWLockStates::RWLOCK_OPERATION_SUCCESS == m_lockState;
    }
    return TRWLockStates::RWLOCK_OPERATION_SUCCESS;
}

/*--------------------------------------------------------------------------*/

CScopeWriterLock::CScopeWriterLock( const CReadWriteLock& rwLock )
    : m_rwLock( &rwLock )
    , m_lockState( TRWLockStates::RWLOCK_OPERATION_FAILED )
{GUCEF_TRACE;

    assert( m_rwLock );
    do
    {
        m_lockState = m_rwLock->WriterStart();
        m_isWriteLocked = TRWLockStates::RWLOCK_OPERATION_SUCCESS == m_lockState;
    }
    while ( !m_isWriteLocked );
}

/*--------------------------------------------------------------------------*/

CScopeWriterLock::CScopeWriterLock( CScopeReaderLock& readerToTransition )
    : m_rwLock( readerToTransition.m_rwLock )
    , m_lockState( TRWLockStates::RWLOCK_OPERATION_FAILED )
{GUCEF_TRACE;

    assert( m_rwLock );
    do
    {
        if ( readerToTransition.m_isReadLocked )
        {
            m_lockState = m_rwLock->TransitionReaderToWriter();
            m_isWriteLocked = TRWLockStates::RWLOCK_OPERATION_SUCCESS == m_lockState;
            if ( m_isWriteLocked )
                readerToTransition.m_isReadLocked = false;
        }
        else
        {
            m_lockState = m_rwLock->WriterStart();
            m_isWriteLocked = TRWLockStates::RWLOCK_OPERATION_SUCCESS == m_lockState;
        }
    }
    while ( !m_isWriteLocked );
}

/*--------------------------------------------------------------------------*/

CScopeWriterLock::~CScopeWriterLock()
{GUCEF_TRACE;

    if ( m_isWriteLocked )
    {
        do
        {
            m_lockState = m_rwLock->WriterStop();
            m_isWriteLocked = TRWLockStates::RWLOCK_OPERATION_SUCCESS == m_lockState;
        }
        while ( m_isWriteLocked );
    }
}

/*--------------------------------------------------------------------------*/

bool 
CScopeWriterLock::HasActiveReaders( void ) const
{GUCEF_TRACE;

    return m_rwLock->ActiveReaderCount() > 0;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeWriterLock::HasActiveWriters( void ) const
{GUCEF_TRACE;

    return m_rwLock->ActiveWriterCount() > 0;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeWriterLock::IsScopeWriteLocked( void ) const
{GUCEF_TRACE;

    return m_isWriteLocked;
}

/*--------------------------------------------------------------------------*/

CScopeReaderLock::TRWLockStates
CScopeWriterLock::EarlyUnlock( void )
{GUCEF_TRACE;

    if ( m_isWriteLocked )
    {        
        m_lockState = m_rwLock->WriterStop();
        m_isWriteLocked = TRWLockStates::RWLOCK_OPERATION_SUCCESS == m_lockState;
        return m_lockState;
    }
    return TRWLockStates::RWLOCK_OPERATION_SUCCESS;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
