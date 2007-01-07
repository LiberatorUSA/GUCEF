/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
    TMailStack::iterator i( m_mailStack.begin() );
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
    TMailStack::iterator i( m_mailStack.begin() );
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
    TMailStack::iterator i( m_mailStack.begin() );
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CTMAILBOX_H  ? */
