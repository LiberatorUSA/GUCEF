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

#ifndef GUCEF_MT_CTMAILBOX_H
#define GUCEF_MT_CTMAILBOX_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <deque>
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Basicly a tread-safe stack for passing data event data between
 *  multiple threads
 */
template < typename T >
class CTMailBox : public virtual MT::CILockable
{
    public:

    struct SMailElement
    {
        T eventid;
        CICloneable* data;
    };
    typedef struct SMailElement TMailElement;

    typedef std::vector< TMailElement, basic_allocator< TMailElement > > TMailVector;
    typedef std::deque< TMailElement, basic_allocator< TMailElement > >  TMailQueue;
    typedef typename std::deque< TMailElement >::iterator iterator;
    typedef typename std::deque< TMailElement >::const_iterator const_iterator;

    CTMailBox( void );

    virtual ~CTMailBox();

    /**
     *  Adds the given eventid and data to the FIFO stack.
     *  if data is non-NULL then it will be cloned.
     *
     *  @param eventid the ID of the event you wish to add to the mailbox
     *  @param data cloneable data container for optional event data.
     */
    bool AddMail( const T& eventid                     ,
                  const CICloneable* data = GUCEF_NULL );

    /**
     *  Attempts to retrieve mail from the mailbox.
     *
     *  Note that if data is non-NULL then you should delete the
     *  object after handling the mail message or you will create
     *  a memory-leak.
     *
     *  @param eventid the ID of the event
     *  @param data cloneable data container for optional event data.
     *  @return whether mail was successfully retrieved from the mailbox.
     */
    bool GetMail( T& eventid                      ,
                  CICloneable** data = GUCEF_NULL );

    /**
     *  Attempts to retrieve mail from the mailbox for inspection without removing it from the mail stack
     *  You can use this in combination with PopMail() for operations which may fail at which point
     *  the mail needs to be retained within the mailbox in order to try again on a later time
     *
     *  @param eventid the ID of the event
     *  @param data cloneable data container for optional event data.
     *  @return whether mail was successfully retrieved from the mailbox.
     */
    bool PeekMail( T& eventid         ,
                   CICloneable** data );

    /**
     *  Removes the first mail item in the mail stack
     *  Intended to be used in combination with PeekMail()
     */
    bool PopMail( void );

    /**
     *  Attempts to retrieve mail from the mailbox.
     *
     *  Note that if data given as part of a mail entry is non-NULL
     *  then you should delete the object after handling the mail
     *  message or you will create a memory-leak.
     *
     *  @param mailList storage where mail items will be placed
     *  @param maxMailItems if greater then -1 will place a maximum on
     *  @param maxMailItems the number of mail items that can be placed in the mail list
     *  @return whether mail was successfully retrieved from the mailbox.
     */
    bool GetMailList( TMailVector& mailList   ,
                      Int32 maxMailItems = -1 );

    void Clear( void );

    void ClearAllExcept( const T& eventid );

    void Delete( const T& eventid );

    bool HasMail( void ) const;

    UInt32 AmountOfMail( void ) const;

    void SetAcceptsNewMail( bool acceptNewMail );

    TLockStatus DoLock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const;

    TLockStatus DoUnlock( void ) const;

    typename TMailQueue::iterator begin( void );

    typename TMailQueue::iterator end( void );

    typename TMailQueue::iterator erase( typename TMailQueue::iterator& index );

    typename TMailQueue::const_iterator begin( void ) const;

    typename TMailQueue::const_iterator end( void ) const;

    virtual const CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual TLockStatus Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual TLockStatus Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    CTMailBox( const CTMailBox& src ) {}                              /**< not supported */
    CTMailBox& operator=( const CTMailBox& src ) { return *this; }    /**< not supported */

    private:

    TMailQueue m_mailQueue;
    bool m_acceptsNewMail;
    CMutex m_datalock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTMailBox< T >::CTMailBox( void )
    : MT::CILockable()
    , m_mailQueue()
    , m_acceptsNewMail( true )
    , m_datalock()
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/

template< typename T >
CTMailBox< T >::~CTMailBox()
{GUCEF_TRACE;

    m_acceptsNewMail = false;
    Clear();
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::AddMail( const T& eventid                     ,
                         const CICloneable* data /* = NULL */ )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    if ( m_acceptsNewMail )
    {
        TMailElement entry;
        entry.eventid = eventid;
        if ( GUCEF_NULL != data )
        {
            entry.data = data->Clone();
        }
        else
        {
            entry.data = GUCEF_NULL;
        }
        m_mailQueue.push_back( entry );
        
        return true;
    }
    return false;
}

/*--------------------------------------------------------------------------*/

template< typename T >
void 
CTMailBox< T >::SetAcceptsNewMail( bool acceptNewMail )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );
    m_acceptsNewMail = acceptNewMail;
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::GetMail( T& eventid         ,
                         CICloneable** data )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    if ( !m_mailQueue.empty() )
    {
        TMailElement& entry = m_mailQueue.front();
        eventid = entry.eventid;

        if ( GUCEF_NULL != data )
            *data = entry.data;
        else
            GUCEF_DELETE entry.data;

        m_mailQueue.pop_front();
        return true;
    }

    if ( GUCEF_NULL != data )
        *data = GUCEF_NULL;
    return false;
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::PeekMail( T& eventid         ,
                          CICloneable** data )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    if ( !m_mailQueue.empty() )
    {
        TMailElement& entry = m_mailQueue.front();
        eventid = entry.eventid;
        if ( GUCEF_NULL != data )
            *data = entry.data;

        return true;
    }
    if ( GUCEF_NULL != data )
        *data = GUCEF_NULL;
    return false;
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::PopMail( void )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    if ( !m_mailQueue.empty() )
    {
        TMailElement& entry = m_mailQueue.front();
        GUCEF_DELETE entry.data;
        m_mailQueue.pop_front();
        return true;
    }
    return false;
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::GetMailList( TMailVector& mailList ,
                             Int32 maxMailItems    )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    Int32 mailItemsRead = 0;
    while ( mailItemsRead < maxMailItems || maxMailItems < 0 )
    {
        if ( !m_mailQueue.empty() )
        {
            mailList.push_back( m_mailQueue.front() );
            m_mailQueue.pop_front();

            ++mailItemsRead;
        }
        else
        {
            break;
        }
    }

    return mailItemsRead > 0;
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::Clear( void )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    typename TMailQueue::iterator i( m_mailQueue.begin() );
    while ( i != m_mailQueue.end() )
    {
        GUCEF_DELETE (*i).data;
        ++i;
    }
    m_mailQueue.clear();
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::ClearAllExcept( const T& eventid )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    #if __cplusplus >= 201103L

    // C++11 added the erase()
    typename TMailQueue::iterator i( m_mailQueue.begin() );
    while ( i != m_mailQueue.end() )
    {
        if ( (*i).eventid != eventid )
        {
            GUCEF_DELETE (*i).data;
            i = m_mailQueue.erase( i );
            continue;
        }
        ++i;
    }

    #else

    TMailQueue copyQueue;
    while ( !m_mailQueue.empty() )
    {
        if ( m_mailQueue.front().eventid == eventid )
            copyQueue.push_back( m_mailQueue.front() );
        else
            GUCEF_DELETE m_mailQueue.front().data;
        m_mailQueue.pop_front();
    }
    m_mailQueue = copyQueue;

    #endif
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::Delete( const T& eventid )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    #if __cplusplus >= 201103L

    // C++11 added the erase()
    typename TMailQueue::iterator i( m_mailQueue.begin() );
    while ( i != m_mailQueue.end() )
    {
        if ( (*i).eventid == eventid )
        {
            GUCEF_DELETE (*i).data;
            i = m_mailQueue.erase( i );
            continue;
        }
        ++i;
    }

    #else

    TMailQueue copyQueue;
    while ( !m_mailQueue.empty() )
    {
        if ( m_mailQueue.front().eventid != eventid )
            copyQueue.push_back( m_mailQueue.front() );
        else
            GUCEF_DELETE m_mailQueue.front().data;
        m_mailQueue.pop_front();
    }
    m_mailQueue = copyQueue;

    #endif
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::HasMail( void ) const
{GUCEF_TRACE;

    CObjectScopeLock lock( this );
    return m_mailQueue.size() > 0;
}

/*--------------------------------------------------------------------------*/

template< typename T >
UInt32
CTMailBox< T >::AmountOfMail( void ) const
{GUCEF_TRACE;

    CObjectScopeLock lock( this );
    return (UInt32) m_mailQueue.size();
}

/*-------------------------------------------------------------------------*/

template< typename T >
const MT::CILockable*
CTMailBox< T >::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*--------------------------------------------------------------------------*/

template< typename T >
TLockStatus
CTMailBox< T >::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_datalock.Lock( lockWaitTimeoutInMs );
}

/*--------------------------------------------------------------------------*/

template< typename T >
TLockStatus
CTMailBox< T >::Unlock( void ) const
{GUCEF_TRACE;

    return m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
TLockStatus
CTMailBox< T >::DoLock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return Lock( lockWaitTimeoutInMs );
}

/*--------------------------------------------------------------------------*/

template< typename T >
TLockStatus
CTMailBox< T >::DoUnlock( void ) const
{GUCEF_TRACE;

    return Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
typename CTMailBox< T >::TMailQueue::iterator
CTMailBox< T >::erase( typename TMailQueue::iterator& index )
{GUCEF_TRACE;

    return m_mailQueue.erase( index );
}

/*--------------------------------------------------------------------------*/

template< typename T >
typename CTMailBox< T >::TMailQueue::iterator
CTMailBox< T >::begin( void )
{GUCEF_TRACE;

    return m_mailQueue.begin();
}

/*--------------------------------------------------------------------------*/

template< typename T >
typename CTMailBox< T >::TMailQueue::iterator
CTMailBox< T >::end( void )
{GUCEF_TRACE;

    return m_mailQueue.end();
}

/*--------------------------------------------------------------------------*/

template< typename T >
typename CTMailBox< T >::TMailQueue::const_iterator
CTMailBox< T >::begin( void ) const
{GUCEF_TRACE;

    return m_mailQueue.begin();
}

/*--------------------------------------------------------------------------*/

template< typename T >
typename CTMailBox< T >::TMailQueue::const_iterator
CTMailBox< T >::end( void ) const
{GUCEF_TRACE;

    return m_mailQueue.end();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CTMAILBOX_H  ? */
