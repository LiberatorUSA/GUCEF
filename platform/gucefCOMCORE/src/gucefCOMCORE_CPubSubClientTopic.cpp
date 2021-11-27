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

#include "gucefCOMCORE_CPubSubClientTopic.h"

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

const CORE::CEvent CPubSubClientTopic::ConnectedEvent = "GUCEF::COMCORE::CPubSubClientTopic::ConnectedEvent";
const CORE::CEvent CPubSubClientTopic::DisconnectedEvent = "GUCEF::COMCORE::CPubSubClientTopic::DisconnectedEvent";
const CORE::CEvent CPubSubClientTopic::ConnectionErrorEvent = "GUCEF::COMCORE::CPubSubClientTopic::ConnectionErrorEvent";
const CORE::CEvent CPubSubClientTopic::MsgsRecievedEvent = "GUCEF::COMCORE::CPubSubClientTopic::MsgsRecievedEvent";
const CORE::CEvent CPubSubClientTopic::MsgsPublishedEvent = "GUCEF::COMCORE::CPubSubClientTopic::MsgsPublishedEvent";
const CORE::CEvent CPubSubClientTopic::MsgsPublishFailureEvent = "GUCEF::COMCORE::CPubSubClientTopic::MsgsPublishFailureEvent";
const CORE::CEvent CPubSubClientTopic::LocalPublishQueueFullEvent = "GUCEF::COMCORE::CPubSubClientTopic::LocalPublishQueueFullEvent";
const CORE::CEvent CPubSubClientTopic::PublishThrottleEvent = "GUCEF::COMCORE::CPubSubClientTopic::PublishThrottleEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void 
CPubSubClientTopic::RegisterEvents( void )
{GUCEF_TRACE;

    ConnectedEvent.Initialize();
    DisconnectedEvent.Initialize();
    ConnectionErrorEvent.Initialize();
    MsgsRecievedEvent.Initialize();
    MsgsPublishedEvent.Initialize();
    MsgsPublishFailureEvent.Initialize();
    LocalPublishQueueFullEvent.Initialize();
    PublishThrottleEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPubSubClientTopic::CPubSubClientTopic( void )
    : CORE::CObservingNotifier()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPubSubClientTopic::~CPubSubClientTopic()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::AcknowledgeReceipt( const CIPubSubMsg& msg )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::AcknowledgeReceipt( const CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::RequestMaxSubscriptionMsgsPerSecRate( CORE::Int64 maxMsgsPerSec )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

const CPubSubClient*
CPubSubClientTopic::GetClient( void ) const
{GUCEF_TRACE;

    return const_cast< CPubSubClientTopic* >( this )->GetClient();
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::DeriveBookmarkFromMsg( const CIPubSubMsg& msg, CPubSubBookmark& bookmark ) const
{GUCEF_TRACE;
    
    bookmark.SetBookmarkType( CPubSubBookmark::BOOKMARK_TYPE_NOT_APPLICABLE );
    bookmark.SetBookmarkData( CORE::CVariant::Empty );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( TPublishActionId& publishActionId, const CORE::CString& msgId, const CORE::CDynamicBuffer& payload, bool notify )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.GetPrimaryPayload().LinkTo( payload );
    return Publish( publishActionId, msg, notify );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( TPublishActionId& publishActionId, const CORE::CString& msgId, const CORE::CString& payload, bool notify )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.GetPrimaryPayload().LinkTo( payload );
    return Publish( publishActionId, msg, notify );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( TPublishActionId& publishActionId, const CORE::CVariant& msgId, const CORE::CVariant& payload, bool notify )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.GetPrimaryPayload().LinkTo( payload );
    return Publish( publishActionId, msg, notify );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( TPublishActionId& publishActionId, const CORE::CString& msgId, const CORE::CString& key, const CORE::CString& value, bool notify )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePair( key, value );
    return Publish( publishActionId, msg, notify );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( TPublishActionId& publishActionId, const CORE::CString& msgId, const CORE::CString& key, const CORE::CDynamicBuffer& value, bool notify )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePair( key, value );
    return Publish( publishActionId, msg, notify );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( TPublishActionId& publishActionId, const CORE::CString& msgId, const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value, bool notify )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePair( key, value );
    return Publish( publishActionId, msg, notify );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( TPublishActionId& publishActionId, const CORE::CVariant& msgId, const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value, bool notify )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePair( key, value );
    return Publish( publishActionId, msg, notify );
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( TPublishActionId& publishActionId, const CORE::CString& msgId, const CORE::CValueList& kvPairs, bool notify )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePairs( kvPairs );
    return Publish( publishActionId, msg, notify );
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds, const CBasicPubSubMsg::TBasicPubSubMsgVector& msgs, bool notify )
{GUCEF_TRACE;

    bool totalSuccess = true;
    size_t preExistingActionIds = publishActionIds.size(); size_t n=0;
    CBasicPubSubMsg::TBasicPubSubMsgVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        CORE::UInt64 publishActionId = 0;
        if ( preExistingActionIds > n )
        {
            publishActionId = publishActionIds[ n ];
        }

        totalSuccess = totalSuccess && Publish( publishActionId, (*i), false );

        if ( preExistingActionIds > n )
            publishActionIds[ n ] = publishActionId;
        else
            publishActionIds.push_back( publishActionId );

        ++i; ++n;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgConstRawPtrVector& msgs, bool notify )
{GUCEF_TRACE;

    bool totalSuccess = true;
    size_t preExistingActionIds = publishActionIds.size(); size_t n=0;
    CIPubSubMsg::TIPubSubMsgConstRawPtrVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        CORE::UInt64 publishActionId = 0;
        if ( preExistingActionIds > n )
        {
            publishActionId = publishActionIds[ n ];
        }

        const CIPubSubMsg* rawMsgPtr = (*i);
        if ( GUCEF_NULL != rawMsgPtr )
            totalSuccess = totalSuccess && Publish( publishActionId, *rawMsgPtr, notify );
        else
            totalSuccess = false;    

        if ( preExistingActionIds > n )
            publishActionIds[ n ] = publishActionId;
        else
            publishActionIds.push_back( publishActionId );

        ++i; ++n;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgRawPtrVector& msgs, bool notify )
{GUCEF_TRACE;

    bool totalSuccess = true;
    size_t preExistingActionIds = publishActionIds.size(); size_t n=0;
    CIPubSubMsg::TIPubSubMsgRawPtrVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        CORE::UInt64 publishActionId = 0;
        if ( preExistingActionIds > n )
        {
            publishActionId = publishActionIds[ n ];
        }

        const CIPubSubMsg* rawMsgPtr = (*i);
        if ( GUCEF_NULL != rawMsgPtr )
            totalSuccess = totalSuccess && Publish( publishActionId, *rawMsgPtr, notify );
        else
            totalSuccess = false;    

        if ( preExistingActionIds > n )
            publishActionIds[ n ] = publishActionId;
        else
            publishActionIds.push_back( publishActionId );

        ++i; ++n;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgSPtrVector& msgs, bool notify )
{GUCEF_TRACE;

    bool totalSuccess = true;
    size_t preExistingActionIds = publishActionIds.size(); size_t n=0;
    CIPubSubMsg::TIPubSubMsgSPtrVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        CORE::UInt64 publishActionId = 0;
        if ( preExistingActionIds > n )
        {
            publishActionId = publishActionIds[ n ];
        }

        if ( !(*i).IsNULL() )
            totalSuccess = totalSuccess && Publish( publishActionId, *(*i).GetPointerAlways(), notify );
        else
            totalSuccess = false;    

        if ( preExistingActionIds > n )
            publishActionIds[ n ] = publishActionId;
        else
            publishActionIds.push_back( publishActionId );

        ++i; ++n;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds, const TPubSubMsgsRefVector& msgs, bool notify )
{GUCEF_TRACE;

    bool totalSuccess = true;
    size_t preExistingActionIds = publishActionIds.size(); size_t n=0;
    TPubSubMsgsRefVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        CORE::UInt64 publishActionId = 0;
        if ( preExistingActionIds > n )
        {
            publishActionId = publishActionIds[ n ];
        }

        const CIPubSubMsg* rawMsgPtr = (*i);
        if ( GUCEF_NULL != rawMsgPtr )
            totalSuccess = totalSuccess && Publish( publishActionId, *rawMsgPtr, notify );
        else
            totalSuccess = false;    

        if ( preExistingActionIds > n )
            publishActionIds[ n ] = publishActionId;
        else
            publishActionIds.push_back( publishActionId );

        ++i; ++n;
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
