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

#include <vector>

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_MT_CICLONEABLE_H
#include "gucefMT_CICloneable.h"
#define GUCEF_MT_CICLONEABLE_H
#endif /* GUCEF_MT_CICLONEABLE_H ? */

#ifndef GUCEF_MT_CILOCKABLE_H
#include "gucefMT_CILockable.h"
#define GUCEF_MT_CILOCKABLE_H
#endif /* GUCEF_MT_CILOCKABLE_H ? */

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#include "gucefMT_CMailBoxForCloneables.h"

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

CMailboxForCloneables::CMailboxForCloneables( void )
    : MT::CILockable()
    , m_mailQueue()
    , m_datalock()
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/

CMailboxForCloneables::~CMailboxForCloneables()
{GUCEF_TRACE;

    CObjectScopeLock lock( this );
    Clear();
}

/*--------------------------------------------------------------------------*/

void
CMailboxForCloneables::AddMail( const CICloneable* mail /* = GUCEF_NULL */ )
{GUCEF_TRACE;

    if ( GUCEF_NULL != mail )
    {
        CObjectScopeLock lock( this );
        m_mailQueue.push_back( mail->Clone() );
    }
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::PopMail( void )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    if ( !m_mailQueue.empty() )
    {
        CICloneable* obj = m_mailQueue.front();
        GUCEF_DELETE obj;
        m_mailQueue.pop_front();
        return true;
    }
    return false;
}

/*--------------------------------------------------------------------------*/

void
CMailboxForCloneables::Clear( void )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );
    while ( !m_mailQueue.empty() )
    {
        GUCEF_DELETE m_mailQueue.front();
        m_mailQueue.pop_front();
    }
}

/*--------------------------------------------------------------------------*/

void
CMailboxForCloneables::Delete( const CICloneable* mail )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    #if __cplusplus >= 201103L

    // C++11 added the erase()
    typename TMailQueue::iterator i( m_mailQueue.begin() );
    while ( i != m_mailQueue.end() )
    {
        if ( (*i) == mail )
        {
            GUCEF_DELETE mail;
            i = m_mailQueue.erase( i );
            continue;
        }
        ++i;
    }

    #else

    TMailQueue copyQueue;
    while ( !m_mailQueue.empty() )
    {
        if ( m_mailQueue.front() == mail )
            GUCEF_DELETE mail;
        else
            copyQueue.push_back( m_mailQueue.front() );  
        m_mailQueue.pop_front();
    }
    m_mailQueue = copyQueue;

    #endif
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::HasMail( void ) const
{GUCEF_TRACE;

    CObjectScopeLock lock( this );
    return !m_mailQueue.empty();
}

/*--------------------------------------------------------------------------*/

UInt32
CMailboxForCloneables::AmountOfMail( void ) const
{GUCEF_TRACE;

    CObjectScopeLock lock( this );
    return (UInt32) m_mailQueue.size();
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CMailboxForCloneables::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*--------------------------------------------------------------------------*/

TLockStatus
CMailboxForCloneables::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_datalock.Lock( lockWaitTimeoutInMs );
}

/*--------------------------------------------------------------------------*/

TLockStatus
CMailboxForCloneables::Unlock( void ) const
{GUCEF_TRACE;

    return m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::DoLock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return Lock( lockWaitTimeoutInMs );
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::DoUnlock( void ) const
{GUCEF_TRACE;

    return Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
