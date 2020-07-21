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
    
    typedef std::vector< TMailElement > TMailList;
    typedef typename std::vector< TMailElement >::iterator iterator;
    typedef typename std::vector< TMailElement >::const_iterator const_iterator;

    CTMailBox( void );

    virtual ~CTMailBox();

    /**
     *  Adds the given eventid and data to the FIFO stack.
     *  if data is non-NULL then it will be cloned.
     *
     *  @param eventid the ID of the event you wish to add to the mailbox
     *  @param data cloneable data container for optional event data.
     */
    void AddMail( const T& eventid               ,
                  const CICloneable* data = NULL );

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
    bool GetMail( T& eventid         ,
                  CICloneable** data );

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
    bool GetMailList( TMailList& mailList     ,
                      Int32 maxMailItems = -1 );

    void Clear( void );

    void ClearAllExcept( const T& eventid );

    void Delete( const T& eventid );

    bool HasMail( void ) const;
    
    UInt32 AmountOfMail( void ) const;

    bool DoLock( void ) const;
    
    bool DoUnlock( void ) const;
    
    typename TMailList::iterator begin( void );
    
    typename TMailList::iterator end( void );
    
    void erase( typename TMailList::iterator& index );

    typename TMailList::const_iterator begin( void ) const;
    
    typename TMailList::const_iterator end( void ) const;

    protected:

    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    CTMailBox( const CTMailBox& src );

    CTMailBox& operator=( const CTMailBox& src );

    private:
    
    TMailList m_mailStack;
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
    , m_mailStack()
    , m_datalock()
{
}

/*--------------------------------------------------------------------------*/

template< typename T >
CTMailBox< T >::~CTMailBox()
{
    CObjectScopeLock lock( this );

    TMailElement* entry;
    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        entry = &(*i);
        delete entry->data;
        m_mailStack.erase( i );
        i = m_mailStack.begin();
    }
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::AddMail( const T& eventid                     ,
                         const CICloneable* data /* = NULL */ )
{
    CObjectScopeLock lock( this );

    TMailElement entry;
    entry.eventid = eventid;
    if ( data )
    {
        entry.data = data->Clone();
    }
    else
    {
        entry.data = NULL;
    }
    m_mailStack.insert( m_mailStack.begin(), entry );
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::GetMail( T& eventid         ,
                         CICloneable** data )
{
    CObjectScopeLock lock( this );

    if ( !m_mailStack.empty() )
    {
        TMailElement& entry = m_mailStack[ m_mailStack.size()-1 ];
        eventid = entry.eventid;
        *data = entry.data;

        m_mailStack.pop_back();
        return true;
    }
    *data = GUCEF_NULL;
    return false;
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::PeekMail( T& eventid         ,
                          CICloneable** data )
{
    CObjectScopeLock lock( this );

    if ( !m_mailStack.empty() )
    {
        TMailElement& entry = m_mailStack[ m_mailStack.size()-1 ];
        eventid = entry.eventid;
        *data = entry.data;

        return true;
    }
    *data = GUCEF_NULL;
    return false;
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::PopMail( void )
{
    CObjectScopeLock lock( this );

    if ( !m_mailStack.empty() )
    {
        TMailElement& entry = m_mailStack[ m_mailStack.size()-1 ];
        delete data = entry.data;
        m_mailStack.pop_back();
        return true;
    }
    return false;
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::GetMailList( TMailList& mailList ,
                             Int32 maxMailItems  )
{
    CObjectScopeLock lock( this );
    
    Int32 mailItemsRead = 0;
    while ( mailItemsRead < maxMailItems || maxMailItems < 0 )
    {    
        if ( !m_mailStack.empty() )
        {
            mailList.push_back( m_mailStack[ m_mailStack.size()-1 ] );
            m_mailStack.pop_back();
            
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
{
    CObjectScopeLock lock( this );

    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        delete (*i).data;
        ++i;
    }
    m_mailStack.clear();
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::ClearAllExcept( const T& eventid )
{
    CObjectScopeLock lock( this );

    TMailElement* entry;
    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        entry = &(*i);
        if ( entry->eventid != eventid )
        {
            delete entry->data;
            m_mailStack.erase( i );
            i = m_mailStack.begin();
            continue;
        }
        ++i;
    }
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::Delete( const T& eventid )
{
    CObjectScopeLock lock( this );

    TMailElement* entry;
    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        entry = &(*i);
        if ( entry->eventid == eventid )
        {
            delete entry->data;
            m_mailStack.erase( i );
            i = m_mailStack.begin();
            continue;
        }
        ++i;
    }
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::HasMail( void ) const
{
    CObjectScopeLock lock( this );
    return m_mailStack.size() > 0;
}

/*--------------------------------------------------------------------------*/

template< typename T >
UInt32
CTMailBox< T >::AmountOfMail( void ) const
{
    CObjectScopeLock lock( this );
    return (UInt32) m_mailStack.size();
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::Lock( void ) const
{
    return m_datalock.Lock();
}    

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::Unlock( void ) const
{
    return m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::DoLock( void ) const
{
    return Lock();
}    

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::DoUnlock( void ) const
{
    return Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::erase( typename TMailList::iterator& index )
{
    m_mailStack.erase( index );
}

/*--------------------------------------------------------------------------*/

template< typename T >
typename CTMailBox< T >::TMailList::iterator
CTMailBox< T >::begin( void )
{
    return m_mailStack.begin();
}

/*--------------------------------------------------------------------------*/
    
template< typename T >
typename CTMailBox< T >::TMailList::iterator
CTMailBox< T >::end( void )
{
    return m_mailStack.end();
}

/*--------------------------------------------------------------------------*/

template< typename T >
typename CTMailBox< T >::TMailList::const_iterator
CTMailBox< T >::begin( void ) const
{
    return m_mailStack.begin();
}
    
/*--------------------------------------------------------------------------*/
    
template< typename T >
typename CTMailBox< T >::TMailList::const_iterator
CTMailBox< T >::end( void ) const
{
    return m_mailStack.end();
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
