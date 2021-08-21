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
CPubSubClientTopic::Publish( const CORE::CString& msgId, const CORE::CDynamicBuffer& payload )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.GetPrimaryPayload().LinkTo( payload );
    return Publish( msg );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( const CORE::CString& msgId, const CORE::CString& payload )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.GetPrimaryPayload().LinkTo( payload );
    return Publish( msg );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( const CORE::CVariant& msgId, const CORE::CVariant& payload )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.GetPrimaryPayload().LinkTo( payload );
    return Publish( msg );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( const CORE::CString& msgId, const CORE::CString& key, const CORE::CString& value )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePair( key, value );
    return Publish( msg );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( const CORE::CString& msgId, const CORE::CString& key, const CORE::CDynamicBuffer& value )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePair( key, value );
    return Publish( msg );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( const CORE::CString& msgId, const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePair( key, value );
    return Publish( msg );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientTopic::Publish( const CORE::CVariant& msgId, const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePair( key, value );
    return Publish( msg );
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( const CORE::CString& msgId, const CORE::CValueList& kvPairs )
{GUCEF_TRACE;

    CBasicPubSubMsg msg;
    msg.GetMsgId().LinkTo( msgId );
    msg.AddLinkedKeyValuePairs( kvPairs );
    return Publish( msg );
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( const CBasicPubSubMsg::TBasicPubSubMsgVector& msgs )
{GUCEF_TRACE;

    bool totalSuccess = true;
    CBasicPubSubMsg::TBasicPubSubMsgVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        totalSuccess = totalSuccess && Publish( (*i) );
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( const CIPubSubMsg::TIPubSubMsgConstRawPtrVector& msgs )
{GUCEF_TRACE;

    bool totalSuccess = true;
    CIPubSubMsg::TIPubSubMsgConstRawPtrVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        const CIPubSubMsg* rawMsgPtr = (*i);
        if ( GUCEF_NULL != rawMsgPtr )
            totalSuccess = totalSuccess && Publish( *rawMsgPtr );
        else
            totalSuccess = false;    
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopic::Publish( const TPubSubMsgsRefVector& msgs )
{GUCEF_TRACE;

    bool totalSuccess = true;
    TPubSubMsgsRefVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        const CIPubSubMsg* rawMsgPtr = (*i);
        if ( GUCEF_NULL != rawMsgPtr )
            totalSuccess = totalSuccess && Publish( *rawMsgPtr );
        else
            totalSuccess = false;    
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
