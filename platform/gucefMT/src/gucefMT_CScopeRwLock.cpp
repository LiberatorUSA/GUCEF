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
{GUCEF_TRACE;

    assert( m_rwLock );
    m_rwLock->ReaderStart();
}

/*--------------------------------------------------------------------------*/

CScopeReaderLock::~CScopeReaderLock()
{GUCEF_TRACE;

    m_rwLock->ReaderStop();
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

CScopeWriterLock::CScopeWriterLock( const CReadWriteLock& rwLock )
    : m_rwLock( &rwLock )
{GUCEF_TRACE;

    assert( m_rwLock );
    m_rwLock->WriterStart();
}

/*--------------------------------------------------------------------------*/

CScopeWriterLock::~CScopeWriterLock()
{GUCEF_TRACE;

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
