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
#include "gucefMT_ETypes.h"
#include "gucefMT_CMutex.h"
#include "gucefMT_CICloneable.h"
#include "gucefMT_macros.h"

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
class CTMailBox
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
    
    void LockData() const;
    
    void UnlockData() const;
    
    typename TMailList::iterator begin( void );
    
    typename TMailList::iterator end( void );
    
    void erase( typename TMailList::iterator& index );

    typename TMailList::const_iterator begin( void ) const;
    
    typename TMailList::const_iterator end( void ) const;

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
{
}

/*--------------------------------------------------------------------------*/

template< typename T >
CTMailBox< T >::~CTMailBox()
{
    m_datalock.Lock();
    TMailElement* entry;
    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        entry = &(*i);
        delete entry->data;
        m_mailStack.erase( i );
        i = m_mailStack.begin();
    }
    m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::AddMail( const T& eventid                     ,
                         const CICloneable* data /* = NULL */ )
{
    m_datalock.Lock();
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
    m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::GetMail( T& eventid         ,
                         CICloneable** data )
{
    m_datalock.Lock();
    if ( !m_mailStack.empty() )
    {
        TMailElement& entry = m_mailStack[ m_mailStack.size()-1 ];
        eventid = entry.eventid;
        *data = entry.data;

        m_mailStack.pop_back();
        m_datalock.Unlock();
        return true;
    }
    *data = NULL;
    m_datalock.Unlock();
    return false;
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::GetMailList( TMailList& mailList ,
                             Int32 maxMailItems  )
{
    m_datalock.Lock();
    
    Int32 mailItemsRead = 0;
    while ( mailItemsRead < maxMailItems && maxMailItems > -1 )
    {    
        if ( !m_mailStack.empty() )
        {
            mailList.push_back( m_mailStack[ m_mailStack.size()-1 ] );
            m_mailStack.pop_back();
            
            ++mailItemsRead;
        }
    }
    
    m_datalock.Unlock();
    return mailItemsRead > 0;
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::Clear( void )
{
    m_datalock.Lock();
    typename TMailList::iterator i( m_mailStack.begin() );
    while ( i != m_mailStack.end() )
    {
        delete (*i).data;
        ++i;
    }
    m_mailStack.clear();
    m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::ClearAllExcept( const T& eventid )
{
    m_datalock.Lock();
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
    m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::Delete( const T& eventid )
{
    m_datalock.Lock();
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
    m_datalock.Unlock();
}

/*--------------------------------------------------------------------------*/

template< typename T >
bool
CTMailBox< T >::HasMail( void ) const
{
    return m_mailStack.size() > 0;
}

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::LockData( void ) const
{
    m_datalock.Lock();
}    

/*--------------------------------------------------------------------------*/

template< typename T >
void
CTMailBox< T >::UnlockData( void ) const
{
    m_datalock.Unlock();
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
