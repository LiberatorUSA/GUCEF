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

#include "gucefMT_CMailboxForCloneables.h"

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
    , m_mailStack()
    , m_datalock()
{

}

/*--------------------------------------------------------------------------*/

CMailboxForCloneables::~CMailboxForCloneables()
{
    CObjectScopeLock lock( this );

    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        delete (*i);
        m_mailStack.erase( i );
        i = m_mailStack.begin();
    }
}

/*--------------------------------------------------------------------------*/

void
CMailboxForCloneables::AddMail( const CICloneable* mail /* = GUCEF_NULL */ )
{
    if ( GUCEF_NULL != mail )
    {
        CObjectScopeLock lock( this );
        m_mailStack.push_back( mail->Clone() );
    }
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::PopMail( void )
{
    CObjectScopeLock lock( this );

    if ( !m_mailStack.empty() )
    {
        CICloneable* obj = m_mailStack[ m_mailStack.size()-1 ];
        delete obj;
        m_mailStack.pop_back();
        return true;
    }
    return false;
}

/*--------------------------------------------------------------------------*/

void
CMailboxForCloneables::Clear( void )
{
    CObjectScopeLock lock( this );

    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        delete (*i);
        ++i;
    }
    m_mailStack.clear();
}

/*--------------------------------------------------------------------------*/

void
CMailboxForCloneables::Delete( const CICloneable* mail )
{
    CObjectScopeLock lock( this );

    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        if ( (*i) == mail )
        {
            delete mail;
            m_mailStack.erase( i );
            i = m_mailStack.begin();
            continue;
        }
        ++i;
    }
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::HasMail( void ) const
{
    CObjectScopeLock lock( this );
    return !m_mailStack.empty();
}

/*--------------------------------------------------------------------------*/

UInt32
CMailboxForCloneables::AmountOfMail( void ) const
{
    CObjectScopeLock lock( this );
    return (UInt32) m_mailStack.size();
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CMailboxForCloneables::AsLockable( void ) const
{
    return this;
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::Lock( UInt32 lockWaitTimeoutInMs ) const
{
    return m_datalock.Lock( lockWaitTimeoutInMs );
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::Unlock( void ) const
{
    return m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::DoLock( UInt32 lockWaitTimeoutInMs ) const
{
    return Lock( lockWaitTimeoutInMs );
}

/*--------------------------------------------------------------------------*/

bool
CMailboxForCloneables::DoUnlock( void ) const
{
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
