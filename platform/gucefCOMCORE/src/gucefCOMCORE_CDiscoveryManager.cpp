/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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
        totalSuccess = c->Register( entry ) && totalSuccess;
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

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CDiscoveryManager::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::CORE::CDiscoveryManager";
    return classTypeName; 
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
