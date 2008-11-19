/*
 *  gucefMT: GUCEF module providing multithreading solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
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

    void Clear( void );

    void ClearAllExcept( const T& eventid );

    void Delete( const T& eventid );

    bool HasMail( void ) const;
    
    protected:
    
    void LockData() const;
    
    void UnlockData() const;

    private:

    CTMailBox( const CTMailBox& src );

    CTMailBox& operator=( const CTMailBox& src );

    private:
    struct SMailElement
    {
        T eventid;
        CICloneable* data;
    };
    typedef struct SMailElement TMailElement;
    typedef std::vector<TMailElement> TMailStack;

    TMailStack m_mailStack;
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
CTMailBox< T >::CTMailBox( const CTMailBox& src )
    : m_mailStack( src.m_mailStack )
{
}

/*--------------------------------------------------------------------------*/

template< typename T >
CTMailBox< T >::~CTMailBox()
{
}

/*--------------------------------------------------------------------------*/

template< typename T >
CTMailBox< T >&
CTMailBox< T >::operator=( const CTMailBox& src )
{
    if ( this != &src )
    {
        m_mailStack = src.m_mailStack;
    }
    return *this;
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
        TMailElement entry( m_mailStack[ m_mailStack.size()-1 ] );
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
void
CTMailBox< T >::Clear( void )
{
    m_datalock.Lock();
    typename TMailStack::iterator i( m_mailStack.begin() );
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
    typename TMailStack::iterator i( m_mailStack.begin() );
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
    typename TMailStack::iterator i( m_mailStack.begin() );
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CTMAILBOX_H  ? */
