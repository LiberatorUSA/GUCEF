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
    , m_isReadLocked( false )
{GUCEF_TRACE;

    assert( m_rwLock );
    m_isReadLocked = m_rwLock->ReaderStart();
}

/*--------------------------------------------------------------------------*/

CScopeReaderLock::~CScopeReaderLock()
{GUCEF_TRACE;

    if ( m_isReadLocked )
    {
        m_rwLock->ReaderStop();
    }
}

/*--------------------------------------------------------------------------*/

bool 
CScopeReaderLock::IsReadLocked( void ) const
{GUCEF_TRACE;

    return m_rwLock->ReaderCount() > 0;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeReaderLock::IsWriteLocked( void ) const
{GUCEF_TRACE;

    return m_rwLock->WriterCount() > 0;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeReaderLock::EarlyUnlock( void )
{GUCEF_TRACE;

    if ( m_isReadLocked )
    {        
        m_rwLock->ReaderStop();
        m_isReadLocked = false;
        return true;
    }
    return false;
}

/*--------------------------------------------------------------------------*/

CScopeWriterLock::CScopeWriterLock( const CReadWriteLock& rwLock )
    : m_rwLock( &rwLock )
    , m_isWriteLocked( false )
{GUCEF_TRACE;

    assert( m_rwLock );
    m_isWriteLocked = m_rwLock->WriterStart();
}

/*--------------------------------------------------------------------------*/

CScopeWriterLock::~CScopeWriterLock()
{GUCEF_TRACE;

    if ( m_isWriteLocked )
        m_rwLock->WriterStop();
}

/*--------------------------------------------------------------------------*/

bool 
CScopeWriterLock::IsReadLocked( void ) const
{GUCEF_TRACE;

    return m_rwLock->ReaderCount() > 0;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeWriterLock::IsWriteLocked( void ) const
{GUCEF_TRACE;

    return m_rwLock->WriterCount() > 0;
}

/*--------------------------------------------------------------------------*/

bool 
CScopeWriterLock::EarlyUnlock( void )
{GUCEF_TRACE;

    if ( m_isWriteLocked )
    {        
        m_rwLock->ReaderStop();
        m_isWriteLocked = false;
        return true;
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
