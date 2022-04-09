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

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"          /* GUCEF CORE library API */
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_COMCORE_H
#include "gucefCOMCORE.h"       /* GUCEF COMCORE library API */
#define GUCEF_COMCORE_H
#endif /* GUCEF_COMCORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define ERRORHERE { GUCEF_ERROR_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, GUCEF::CORE::CString( "Test failed @ " ) + GUCEF::CORE::CString( __FILE__ ) + ':' + GUCEF::CORE::Int32ToString( __LINE__ ) ); assert(0); }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef std::vector< CORE::CDynamicBuffer >         TDynamicBufferVector;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static const CORE::CString testPlayload1 = "PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST_PAYLOAD_THIS_IS_A_TEST";
static const CORE::CString testKVKey1 = "KV_KEY1";
static const CORE::CString testKVValue1 = "KV_VALUE1";
static const CORE::CString testKVKey2 = "KV_KEY2";
static const CORE::CString testKVValue2 = "KV_VALUE2";
static const CORE::CString testMsgId1 = "MSG_ID_1";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
GenerateTestBinaryPayload( CORE::CDynamicBuffer& buffer, UInt32 startNum, UInt32 endNum )
{GUCEF_TRACE;

    for ( UInt32 i=startNum; i<endNum; ++i )
    {
        buffer.AppendValue( i );
    }    
}

/*-------------------------------------------------------------------------*/

void
PerformPubSubMsgBinaryStorageTest( void )
{GUCEF_TRACE;

    try
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Running PerformPubSubMsgBinaryStorageTest with PubSubMsgContainerBinarySerializer with format version: " + CORE::ToString( COMCORE::CPubSubMsgContainerBinarySerializer::CurrentFormatVersion ) );
        
        CORE::CDateTime testStart = CORE::CDateTime::NowLocalDateTime();
        
        CORE::CDynamicBuffer testPlayload2;
        GenerateTestBinaryPayload( testPlayload2, 0, 100 );

        COMCORE::CIPubSubMsg::TKeyValuePair pubsubKvPair1;
        pubsubKvPair1.first.LinkTo( testKVKey1 );
        pubsubKvPair1.second.LinkTo( testKVValue1 );

        COMCORE::CIPubSubMsg::TKeyValuePair pubsubKvPair2;
        pubsubKvPair2.first.LinkTo( testKVKey2 );
        pubsubKvPair2.second.LinkTo( testKVValue2 );

        CORE::CDynamicBuffer inMemStorage1( true );
        CORE::CDynamicBuffer inMemStorage2( true );

        COMCORE::CPubSubMsgBinarySerializerOptions serializerOptions;
        serializerOptions.msgIdIncluded = true;
        serializerOptions.msgIndexIncluded = true;
        serializerOptions.msgDateTimeIncluded = true;
        serializerOptions.msgDateTimeAsMsSinceUnixEpochInUtc = true;
        serializerOptions.msgPrimaryPayloadIncluded = true;
        serializerOptions.msgKeyValuePairsIncluded = true;
        serializerOptions.msgMetaDataKeyValuePairsIncluded = true;
        
        COMCORE::CBasicPubSubMsg::TBasicPubSubMsgVector msgs;
        msgs.resize( 100 );        
        for ( UInt32 i=0; i<100; ++i )
        {
            COMCORE::CBasicPubSubMsg& msg = msgs[ i ];
            msg.GetPrimaryPayload().LinkTo( testPlayload1 );
            msg.GetMsgDateTime() = testStart;
            msg.GetMsgId().LinkTo( testMsgId1 );
            msg.AddLinkedKeyValuePair( pubsubKvPair1 );
            msg.AddLinkedKeyValuePair( pubsubKvPair2 );
        }
        
        UInt32 bytesWritten = 0;
        if ( !COMCORE::CPubSubMsgContainerBinarySerializer::Serialize( serializerOptions, msgs, 0, inMemStorage1, bytesWritten ) )
            ERRORHERE;
        if ( bytesWritten != inMemStorage1.GetDataSize() )
            ERRORHERE;

        bool isCorrupted = false;
        COMCORE::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex msgIndex;
        COMCORE::CBasicPubSubMsg::TBasicPubSubMsgVector msgs2;
        if ( !COMCORE::CPubSubMsgContainerBinarySerializer::DeserializeWithRebuild( msgs2, true, msgIndex, inMemStorage1, isCorrupted ) )
            ERRORHERE;
        if ( isCorrupted )
            ERRORHERE;
        if ( msgIndex.size() != msgs.size() )            
            ERRORHERE;
    }
    catch ( CORE::CMsgException& e )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, CORE::CString( "unhandled GUCEF exception during PubSubMsgBinaryStorage test: " ) + e.what() );
        ERRORHERE;
    }
    catch ( std::exception& e )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, CORE::CString( "unhandled std exception during PubSubMsgBinaryStorage test: " ) + e.what() );
        ERRORHERE;
    }
    catch ( ... )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "unhandled exception during PubSubMsgBinaryStorage test" );
        ERRORHERE;
    }
}

/*-------------------------------------------------------------------------*/