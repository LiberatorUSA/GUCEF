/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#include "gucefCOMCORE_CDiscoveryManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CDiscoveryManager::CDiscoveryManager( void )
    : CORE::CIConfigurable()
    , CIDiscovery()
    , m_clients()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDiscoveryManager::~CDiscoveryManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::RegisterDiscoveryClient( CIDiscoveryClientPtr discoveryClient )
{GUCEF_TRACE;

    m_clients.insert( discoveryClient );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::UnregisterDiscoveryClient( const CORE::CString& discoveryClientName )
{GUCEF_TRACE;

    TDiscoveryClientSet::iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        if ( discoveryClientName == (*i)->GetDiscoveryClientName() )
        {
            m_clients.erase( i );
            return true;
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::FindCapability( const CORE::CString& capabilityId ,
                                   TIDiscoveryEntrySet& entries      ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    TDiscoveryClientSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        totalSuccess &= (*i)->FindCapability( capabilityId, entries );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::FindApplication( const CORE::CString& applicationId ,
                                    TIDiscoveryEntrySet& entries        ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    TDiscoveryClientSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        totalSuccess &= (*i)->FindApplication( applicationId, entries );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::FindHost( const COMCORE::CHostAddress& host ,
                             TIDiscoveryEntrySet& entries      ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    TDiscoveryClientSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        totalSuccess &= (*i)->FindHost( host, entries );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::Register( CIDiscoveryEntryPtr entry )
{GUCEF_TRACE;

    bool totalSuccess = true;
    TDiscoveryClientSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        CIDiscoveryClientPtr c = (*i);
        totalSuccess &= c->Register( entry );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::Unregister( CIDiscoveryEntryPtr entry )
{GUCEF_TRACE;

    bool totalSuccess = true;
    TDiscoveryClientSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        CIDiscoveryClientPtr c = (*i);
        totalSuccess &= c->Unregister( entry );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    TDiscoveryClientSet::const_iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        totalSuccess &= (*i)->SaveConfig( tree );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CDiscoveryManager::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    bool totalSuccess = true;
    TDiscoveryClientSet::iterator i = m_clients.begin();
    while ( i != m_clients.end() )
    {
        CIDiscoveryClientPtr c = (*i);
        totalSuccess &= c->LoadConfig( treeroot );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
