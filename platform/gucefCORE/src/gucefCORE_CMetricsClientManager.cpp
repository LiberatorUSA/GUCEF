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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefCORE_CMetricsClientManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CMetricsClientManager::AddMetricsClient( CIMetricsSystemClientPtr client )
{GUCEF_TRACE;

    m_clients.insert( client );
}

/*-------------------------------------------------------------------------*/

void
CMetricsClientManager::RemoveMetricsClient( CIMetricsSystemClientPtr client )
{GUCEF_TRACE;

    m_clients.erase( client );
}

/*-------------------------------------------------------------------------*/

void
CMetricsClientManager::ClearMetricsClients( void )
{GUCEF_TRACE;

    m_clients.clear();
}

/*-------------------------------------------------------------------------*/

CMetricsClientManager::CMetricsClientManager( void )
    : CIMetricsSystemClient( true )
    , m_clients()
    , m_dataLock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMetricsClientManager::~CMetricsClientManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CMetricsClientManager::Open( void )
{GUCEF_TRACE;

    bool totalSuccess = true;
    TMetricsSystemClientPtrSet::iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        CIMetricsSystemClientPtr client = (*i);
        totalSuccess &= client->Open();
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CMetricsClientManager::Close( void )
{GUCEF_TRACE;

    bool totalSuccess = true;
    TMetricsSystemClientPtrSet::iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        CIMetricsSystemClientPtr client = (*i);
        totalSuccess &= client->Close();
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CMetricsClientManager::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    TMetricsSystemClientPtrSet::iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        totalSuccess &= (*i)->SaveConfig( tree );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CMetricsClientManager::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    bool totalSuccess = true;
    TMetricsSystemClientPtrSet::iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        CIMetricsSystemClientPtr client = (*i);
        totalSuccess &= client->LoadConfig( treeroot );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

void
CMetricsClientManager::Increment( const CString& key, const Float32 frequency ) const
{GUCEF_TRACE;

    TMetricsSystemClientPtrSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        (*i)->Increment( key, frequency );        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CMetricsClientManager::Decrement( const CString& key, const Float32 frequency ) const
{GUCEF_TRACE;

    TMetricsSystemClientPtrSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        (*i)->Decrement( key, frequency );        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CMetricsClientManager::Count( const CString& key, const Int32 delta, const Float32 frequency ) const
{GUCEF_TRACE;

    TMetricsSystemClientPtrSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        (*i)->Count( key, delta, frequency );        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CMetricsClientManager::Gauge( const CString& key, const UInt32 value, const Float32 frequency ) const
{GUCEF_TRACE;

    TMetricsSystemClientPtrSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        (*i)->Gauge( key, value, frequency );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void 
CMetricsClientManager::Timing( const CString& key, const UInt32 ms, const Float32 frequency ) const
{GUCEF_TRACE;

    TMetricsSystemClientPtrSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        (*i)->Timing( key, ms, frequency );        
        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/
