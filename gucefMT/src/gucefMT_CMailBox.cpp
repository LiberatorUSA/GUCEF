/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefMT_CMailBox.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CMailBox::CMailBox( void )
{
}

/*--------------------------------------------------------------------------*/

CMailBox::CMailBox( const CMailBox& src )
    : m_mailStack( src.m_mailStack )
{
}

/*--------------------------------------------------------------------------*/

CMailBox::~CMailBox()
{
}

/*--------------------------------------------------------------------------*/

CMailBox&
CMailBox::operator=( const CMailBox& src )
{
    if ( this != &src )
    {
        m_mailStack = src.m_mailStack;
    }
    return *this;
}

/*--------------------------------------------------------------------------*/

void
CMailBox::AddMail( const UInt32 eventid                 ,
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

bool 
CMailBox::GetMail( UInt32& eventid    ,
                   CICloneable** data )
{
    m_datalock.Lock();
    if ( m_mailStack.size() )
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

void 
CMailBox::Clear( void )
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
    
void 
CMailBox::ClearAllExcept( const UInt32 eventid )
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
    
void 
CMailBox::Delete( const UInt32 eventid )
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

bool 
CMailBox::HasMail( void ) const
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

/*--------------------------------------------------------------------------*/
