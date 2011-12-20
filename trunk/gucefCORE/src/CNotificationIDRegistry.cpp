/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <limits.h>

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_CMSGEXCEPTION_H
#include "CMsgException.h"
#define GUCEF_CORE_CMSGEXCEPTION_H
#endif /* GUCEF_CORE_CMSGEXCEPTION_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

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
{GUCEF_TRACE;

    /* nothing to do here */
}

/*-------------------------------------------------------------------------*/

CNotificationIDRegistry::~CNotificationIDRegistry()
{GUCEF_TRACE;

    /* nothing to do here */    
}

/*-------------------------------------------------------------------------*/

CNotificationIDRegistry* 
CNotificationIDRegistry::Instance( void )
{GUCEF_TRACE;

    if ( m_instance == NULL )
    {
        m_instance = new CNotificationIDRegistry();
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CNotificationIDRegistry Singleton created" );
    }

    return m_instance;
}

/*-------------------------------------------------------------------------*/

void 
CNotificationIDRegistry::Deinstance( void )
{GUCEF_TRACE;

    m_dataLock.Lock();

    delete m_instance;
    m_instance = NULL;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CNotificationIDRegistry Singleton destroyed" );

    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

CEvent 
CNotificationIDRegistry::Register( const CString& keyvalue                       ,
                                   const bool okIfAlreadyRegisterd /* = false */ )
{GUCEF_TRACE;
    
    m_dataLock.Lock();
    
    if ( keyvalue.Length() > 0 )
    {    
        TRegistryList::const_iterator i = m_list.find( keyvalue );
        if ( i == m_list.end() )
        {
            if ( GUCEFCORE_UINT32MAX > m_lastid )
            {                
                m_list[ keyvalue ] = m_lastid;
                ++m_lastid;
                m_dataLock.Unlock();
                
                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Event registered with ID " + UInt32ToString( m_lastid-1 ) + " and name \"" + keyvalue + "\"" );
                
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
CNotificationIDRegistry::Unregister( const CString& keyvalue                      ,
                                     const bool okIfUnknownKeyGiven /* = false */ )
{GUCEF_TRACE;

    m_dataLock.Lock();

    if ( keyvalue.Length() > 0 )
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
CNotificationIDRegistry::Lookup( const CString& keyvalue     ,
                                 const bool registerUnknown  )
{GUCEF_TRACE;

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

CString 
CNotificationIDRegistry::Lookup( const CEvent& eventID ) const
{GUCEF_TRACE;

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
CNotificationIDRegistry::IsRegistered( const CString& keyvalue ) const
{GUCEF_TRACE;

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
