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

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefCOMCORE_CPubSubClient.h"    

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CPubSubClient::TopicAccessCreatedEvent = "GUCEF::COMCORE::CPubSubClient::TopicAccessCreatedEvent";
const CORE::CEvent CPubSubClient::TopicAccessDestroyedEvent = "GUCEF::COMCORE::CPubSubClient::TopicAccessDestroyedEvent";
const CORE::CEvent CPubSubClient::TopicAccessAutoCreatedEvent = "GUCEF::COMCORE::CPubSubClient::TopicAccessAutoCreatedEvent";
const CORE::CEvent CPubSubClient::TopicAccessAutoDestroyedEvent = "GUCEF::COMCORE::CPubSubClient::TopicAccessAutoDestroyedEvent";
const CORE::CEvent CPubSubClient::TopicDiscoveryEvent = "GUCEF::COMCORE::CPubSubClient::TopicDiscoveryEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubClient::CPubSubClient( void )
    : CORE::CObservingNotifier()
    , CORE::CIConfigurable()
    , m_opaqueUserData( GUCEF_NULL )
{GUCEF_TRACE;

    RegisterEvents();    
}

/*-------------------------------------------------------------------------*/

CPubSubClient::CPubSubClient( const CPubSubClient& src )
    : CORE::CObservingNotifier( src )
    , CORE::CIConfigurable( src )
    , m_opaqueUserData( src.m_opaqueUserData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClient::~CPubSubClient()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void 
CPubSubClient::RegisterEvents( void )
{GUCEF_TRACE;
    
    TopicAccessCreatedEvent.Initialize();
    TopicAccessDestroyedEvent.Initialize();
    TopicAccessAutoCreatedEvent.Initialize();
    TopicAccessAutoDestroyedEvent.Initialize();
    TopicDiscoveryEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPubSubClient& 
CPubSubClient::operator=( const CPubSubClient& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CORE::CIConfigurable::operator=( *this );
        CORE::CObservingNotifier::operator=( *this );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubClient::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::COMCORE::CPubSubClient";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

CPubSubClientTopic* 
CPubSubClient::GetOrCreateTopicAccess( const CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    CPubSubClientTopic* topicAccess = GetTopicAccess( topicName );
    if ( GUCEF_NULL == topicAccess )
    {
        topicAccess = CreateTopicAccess( topicName );
    }
    return topicAccess;
}

/*-------------------------------------------------------------------------*/

CPubSubClientTopic* 
CPubSubClient::CreateTopicAccess( const CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    const CPubSubClientTopicConfig* topicConfig = GetTopicConfig( topicName );
    if ( GUCEF_NULL != topicConfig )
    {
        CPubSubClientTopic* topicAccess = CreateTopicAccess( *topicConfig );
        return topicAccess;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClient::GetMultiTopicAccess( const CString& topicName          ,
                                    PubSubClientTopicSet& topicAccess )
{GUCEF_TRACE;

    // The default implementation here assumes no 1:N pattern matching access is supported
    // As such it redirects to the basic GetTopicAccess()
    // Backends should override this if they support pattern matching access

    CPubSubClientTopic* tAccess = GetTopicAccess( topicName );
    if ( GUCEF_NULL != tAccess )
    {
        topicAccess.insert( tAccess );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClient::GetOrCreateMultiTopicAccess( const CString& topicName          ,
                                            PubSubClientTopicSet& topicAccess )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    // In the multi-topic scenario we'd expect the topic name to be a pattern to match
    // As such the config applies to everything matching the pattern
    // This also means there is no way of knowing if we obtained the 'correct' nr of topics here, just something vs nothing.
    if ( !GetMultiTopicAccess( topicName, topicAccess ) || topicAccess.empty()  )
    {
        return CreateMultiTopicAccess( topicName, topicAccess );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClient::CreateMultiTopicAccess( const CPubSubClientTopicConfig& topicConfig ,
                                       PubSubClientTopicSet& topicAccess           )
{GUCEF_TRACE;

    // The default implementation here assumes no 1:N pattern matching access is supported
    // As such it redirects to the basic CreateTopicAccess()
    // Backends should override this if they support pattern matching access

    CPubSubClientTopic* tAccess = CreateTopicAccess( topicConfig );
    if ( GUCEF_NULL != tAccess )
    {
        topicAccess.insert( tAccess );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClient::CreateMultiTopicAccess( const CString& topicName          ,
                                       PubSubClientTopicSet& topicAccess )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    // In the multi-topic scenario we'd expect the topic name to be a pattern to match
    // As such the config applies to everything matching the pattern
    const CPubSubClientTopicConfig* topicConfig = GetTopicConfig( topicName );
    if ( GUCEF_NULL != topicConfig )
    {
        return CreateMultiTopicAccess( *topicConfig, topicAccess );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClient::GetAvailableTopicNameList( CORE::CString::StringSet& topicNameList             ,
                                          const CORE::CString::StringSet& globPatternFilters  )
{GUCEF_TRACE;

    // By default not supported
    return false;
}

/*-------------------------------------------------------------------------*/

void 
CPubSubClient::SetOpaqueUserData( void* opaqueUserData )
{GUCEF_TRACE;

   MT::CObjectScopeLock lock( this );
   m_opaqueUserData = opaqueUserData;
}

/*-------------------------------------------------------------------------*/
    
void*
CPubSubClient::GetOpaqueUserData( void ) const
{GUCEF_TRACE;

    return m_opaqueUserData;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
