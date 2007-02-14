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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <limits.h>
#include "CMsgException.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_CPP
#include "CNotificationIDRegistry.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CNotificationIDRegistry* CNotificationIDRegistry::m_instance = NULL;
MT::CMutex               CNotificationIDRegistry::m_dataLock;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_IMPLEMENT_MSGEXCEPTION( CNotificationIDRegistry, EKeyAlreadyRegistered );
GUCEF_IMPLEMENT_MSGEXCEPTION( CNotificationIDRegistry, EEmptyKeyString );
GUCEF_IMPLEMENT_MSGEXCEPTION( CNotificationIDRegistry, EUnknownKey );
GUCEF_IMPLEMENT_MSGEXCEPTION( CNotificationIDRegistry, EInvalidKey );
GUCEF_IMPLEMENT_MSGEXCEPTION( CNotificationIDRegistry, EInvalidEventID );

/*-------------------------------------------------------------------------*/

CNotificationIDRegistry::CNotificationIDRegistry( void )
    : m_lastid( 1 )
{TRACE;

    /* nothing to do here */
}

/*-------------------------------------------------------------------------*/

CNotificationIDRegistry::CNotificationIDRegistry( const CNotificationIDRegistry& src )
{TRACE;

    /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CNotificationIDRegistry::~CNotificationIDRegistry()
{TRACE;

    /* nothing to do here */    
}

/*-------------------------------------------------------------------------*/

CNotificationIDRegistry&
CNotificationIDRegistry::operator=( const CNotificationIDRegistry& src )
{TRACE;

    /* dummy, do not use */

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CNotificationIDRegistry* 
CNotificationIDRegistry::Instance( void )
{TRACE;

    m_dataLock.Lock();

    if ( m_instance == NULL )
    {
        m_instance = new CNotificationIDRegistry();
    }

    m_dataLock.Unlock();
    return m_instance;
}

/*-------------------------------------------------------------------------*/

void 
CNotificationIDRegistry::Deinstance( void )
{TRACE;

    m_dataLock.Lock();

    delete m_instance;
    m_instance = NULL;

    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

CEvent 
CNotificationIDRegistry::Register( const std::string& keyvalue                   ,
                                   const bool okIfAlreadyRegisterd /* = false */ )
{TRACE;
    
    m_dataLock.Lock();
    
    if ( (size_t)0 != keyvalue.length() )
    {    
        TRegistryList::const_iterator i = m_list.find( keyvalue );
        if ( i == m_list.end() )
        {
            if ( GUCEFCORE_UINT32MAX > m_lastid )
            {                
                m_list[ keyvalue ] = m_lastid;
                ++m_lastid;
                m_dataLock.Unlock();
                return CEvent( m_lastid-1, keyvalue );
            }
        
            /*
             *  Because all event id's have been used at least once we
             *  now have to perform a linear search for available event id's
             *  In practice it is rather unlikely this code will ever be used due to the insane
             *  amount of event registration actions required.
             */
            for ( UInt32 n=0; n<ULONG_MAX; ++n )
            {                     
                bool found = false;
                i = m_list.begin();
                while ( i != m_list.end() )
                {
                    if ( (*i).second == n )
                    {
                        found = true;
                        break;
                    }
                    ++i;    
                }
                if ( !found )
                {
                    m_list[ keyvalue ] = n;
                    m_dataLock.Unlock();
                    return CEvent( n, keyvalue );
                }
            }
        }
    
        if ( !okIfAlreadyRegisterd )
        {
            m_dataLock.Unlock();
            GUCEF_EMSGTHROW( EKeyAlreadyRegistered, "CNotificationIDRegistry: Key is already registerd" );
        }
        m_dataLock.Unlock();
        return CEvent( (*i).second, keyvalue );
    }
    else
    {
        m_dataLock.Unlock();
        GUCEF_EMSGTHROW( EEmptyKeyString, "CNotificationIDRegistry: Empty key string" );        
    }

    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/
    
void 
CNotificationIDRegistry::Unregister( const std::string& keyvalue                  ,
                                     const bool okIfUnknownKeyGiven /* = false */ )
{TRACE;

    m_dataLock.Lock();

    if ( keyvalue.size() > 0 )
    {
        TRegistryList::iterator i = m_list.find( keyvalue );
        if ( i != m_list.end() )
        {
            m_list.erase( i );    
        }
        else
        {
            m_dataLock.Unlock();
            GUCEF_EMSGTHROW( EUnknownKey, "CNotificationIDRegistry::Unregister(): unknown notification key string identifier" );
        }
    }
    else
    {
        m_dataLock.Unlock();
        GUCEF_EMSGTHROW( EUnknownKey, "CNotificationIDRegistry::Unregister(): invalid notification key string identifier" );
    }

    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

CEvent 
CNotificationIDRegistry::Lookup( const std::string& keyvalue ,
                                 const bool registerUnknown  )
{TRACE;

    m_dataLock.Unlock();

    TRegistryList::iterator i = m_list.find( keyvalue );
    if ( i != m_list.end() )
    {
        m_dataLock.Unlock();
        return CEvent( (*i).second, keyvalue );
    }
    
    if ( registerUnknown )
    {
        Register( keyvalue );
        m_dataLock.Unlock();
        return Lookup( keyvalue );
    }

    m_dataLock.Unlock();

    GUCEF_EMSGTHROW( EInvalidKey, "CNotificationIDRegistry::Lookup(): invalid notification key identifier" );    
}

/*-------------------------------------------------------------------------*/

std::string 
CNotificationIDRegistry::Lookup( const CEvent& eventID ) const
{TRACE;

    m_dataLock.Lock();
    
    UInt32 id( eventID.GetID() );
    
    if ( id > 0 )
    {
        TRegistryList::const_iterator i = m_list.begin();
        while ( i != m_list.end() )
        {
            if ( (*i).second == id )
            {
                m_dataLock.Unlock();
                return (*i).first;
            }
            ++i;            
        }
    }
    
    m_dataLock.Unlock();

    GUCEF_EMSGTHROW( EInvalidEventID, "CNotificationIDRegistry::Lookup(): invalid eventID" );
}

/*-------------------------------------------------------------------------*/

bool
CNotificationIDRegistry::IsRegistered( const std::string& keyvalue ) const
{TRACE;

    m_dataLock.Lock();

    TRegistryList::const_iterator i = m_list.find( keyvalue );
    bool found = i != m_list.end();

    m_dataLock.Unlock();

    return found;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
