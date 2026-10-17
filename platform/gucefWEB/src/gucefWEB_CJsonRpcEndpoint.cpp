/*
 *  gucefWEB: GUCEF module providing WEB application functionality
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#include <stdio.h>

#ifndef GUCEF_WEB_CJSONRPCENDPOINT_H
#include "gucefWEB_CJsonRpcEndpoint.h"
#define GUCEF_WEB_CJSONRPCENDPOINT_H
#endif /* GUCEF_WEB_CJSONRPCENDPOINT_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CIMONOTONICCLOCK_H
#include "gucefCORE_CIMonotonicClock.h"
#define GUCEF_CORE_CIMONOTONICCLOCK_H
#endif /* GUCEF_CORE_CIMONOTONICCLOCK_H ? */

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CJsonRpcEndpoint::ResponseEvent = "GUCEF::WEB::CJsonRpcEndpoint::ResponseEvent";
const CORE::CEvent CJsonRpcEndpoint::NotificationEvent = "GUCEF::WEB::CJsonRpcEndpoint::NotificationEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static bool
GetJsonCodec( CORE::CDStoreCodecRegistry::TDStoreCodecPtr& codec )
{GUCEF_TRACE;

    if ( CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( "json", codec, false ) && !codec.IsNULL() )
        return true;

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "JsonRpc: The json dstore codec is not available, is dstorepluginJSONPARSER loaded?" );
    return false;
}

/*-------------------------------------------------------------------------*/

static bool
ParseJson( const CORE::CString& text, CORE::CDataNode& root )
{GUCEF_TRACE;

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
    if ( text.ByteSize() < 2 || !GetJsonCodec( codec ) )
        return false;

    CORE::CDynamicBuffer buffer;
    buffer.CopyFrom( text.ByteSize() - 1, text.C_String() );    // ByteSize includes the null terminator
    CORE::CDynamicBufferAccess access( buffer );
    return codec->BuildDataTree( &root, &access );
}

/*-------------------------------------------------------------------------*/

static bool
IsNullType( Int32 nodeType )
{
    return GUCEF_DATATYPE_NIL == nodeType || GUCEF_DATATYPE_NULL == nodeType;
}

/**
 *  Writes a node tree as JSON text following the same conventions the json codec uses when reading:
 *  an object has its scalar members as attributes and its other members as children, an array has its items as
 *  children and a node of the nil type is null.
 *  The codec is only used for reading: its writer turns an object that has exactly one member and nothing else
 *  into a bare value, which would break messages such as { "url": "http://example.org/" }
 */
static void
AppendJsonString( std::string& out, const std::string& str )
{
    static const char* hexDigits = "0123456789abcdef";
    out += '"';
    for ( size_t i=0; i<str.size(); ++i )
    {
        unsigned char c = (unsigned char) str[ i ];
        switch ( c )
        {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
            {
                if ( c < 0x20 )
                {
                    out += "\\u00";
                    out += hexDigits[ c >> 4 ];
                    out += hexDigits[ c & 0xF ];
                }
                else
                {
                    out += (char) c;    // UTF-8 is passed through as is
                }
            }
        }
    }
    out += '"';
}

static void
AppendJsonScalar( std::string& out, const CORE::CVariant& value )
{
    if ( !value.IsInitialized() || value.IsNIL() || value.IsNULL() )
    {
        out += "null";
    }
    else
    if ( value.IsBoolean() )
    {
        out += value.AsBool() ? "true" : "false";
    }
    else
    if ( value.IsUnsignedInteger() )
    {
        out += CORE::UInt64ToString( value.AsUInt64() ).STL_String();
    }
    else
    if ( value.IsInteger() )
    {
        out += CORE::Int64ToString( value.AsInt64() ).STL_String();
    }
    else
    if ( value.IsFloat() )
    {
        Float64 number = value.AsFloat64();
        if ( number != number || number > 1.7976931348623157e308 || number < -1.7976931348623157e308 )
        {
            out += "null";      // JSON has no representation for NaN or infinity
        }
        else
        {
            char buffer[ 40 ];
            snprintf( buffer, sizeof( buffer ), "%.17g", number );
            out += buffer;
        }
    }
    else
    {
        AppendJsonString( out, value.AsString().STL_String() );
    }
}

static void AppendJsonNode( std::string& out, const CORE::CDataNode& node );

static void
AppendJsonNode( std::string& out, const CORE::CDataNode& node )
{
    Int32 nodeType = node.GetNodeType();
    if ( GUCEF_DATATYPE_ARRAY == nodeType )
    {
        out += '[';
        bool first = true;
        for ( CORE::CDataNode::const_iterator i = node.ConstBegin(); i != node.ConstEnd(); ++i )
        {
            if ( !first )
                out += ',';
            first = false;
            AppendJsonNode( out, *(*i) );
        }
        out += ']';
    }
    else
    if ( GUCEF_DATATYPE_OBJECT == nodeType && ( node.GetAttCount() > 0 || node.HasChildren() || !node.HasValue() ) )
    {
        out += '{';
        bool first = true;
        for ( UInt32 i=0; i<node.GetAttCount(); ++i )
        {
            const CORE::CDataNode::TKeyValuePair* attribute = node.GetAttribute( i );
            if ( !first )
                out += ',';
            first = false;
            AppendJsonString( out, (*attribute).first.STL_String() );
            out += ':';
            AppendJsonScalar( out, (*attribute).second );
        }
        for ( CORE::CDataNode::const_iterator i = node.ConstBegin(); i != node.ConstEnd(); ++i )
        {
            if ( !first )
                out += ',';
            first = false;
            AppendJsonString( out, (*i)->GetName().STL_String() );
            out += ':';
            AppendJsonNode( out, *(*i) );
        }
        out += '}';
    }
    else
    if ( IsNullType( nodeType ) )
    {
        out += "null";
    }
    else
    {
        AppendJsonScalar( out, node.GetValue() );
    }
}

static bool
StoreJson( const CORE::CDataNode& root, CORE::CString& text )
{GUCEF_TRACE;

    std::string json;
    AppendJsonNode( json, root );
    text = CORE::CString( json );
    return true;
}

/*-------------------------------------------------------------------------*/

static bool
IsContainerType( Int32 nodeType )
{
    return GUCEF_DATATYPE_ARRAY == nodeType || GUCEF_DATATYPE_OBJECT == nodeType;
}

/*-------------------------------------------------------------------------*/


/**
 *  Adds a member to a JSON object. Per the JSON codec scalars become attributes of the object,
 *  objects and arrays child nodes and null a child of the nil type
 */
static void
AddMember( CORE::CDataNode& parent, const CORE::CString& name, const CORE::CDataNode& value )
{GUCEF_TRACE;

    Int32 nodeType = value.GetNodeType();
    if ( IsContainerType( nodeType ) )
    {
        CORE::CDataNode copy( value );
        copy.SetName( name );
        parent.AddChild( copy );
    }
    else
    if ( IsNullType( nodeType ) )
    {
        parent.AddChild( name, GUCEF_DATATYPE_NIL );
    }
    else
    {
        parent.SetAttribute( name, value.GetValue() );
    }
}

/*-------------------------------------------------------------------------*/

static bool
HasMember( const CORE::CDataNode& object, const CORE::CString& name )
{GUCEF_TRACE;

    return GUCEF_NULL != object.GetAttribute( name ) || GUCEF_NULL != object.Find( name );
}

/**
 *  The reverse of AddMember: gets a member of a JSON object as a node. For a scalar the value is in the node
 */
static bool
GetMember( const CORE::CDataNode& object, const CORE::CString& name, CORE::CDataNode& value )
{GUCEF_TRACE;

    const CORE::CDataNode::TKeyValuePair* attribute = object.GetAttribute( name );
    if ( GUCEF_NULL != attribute )
    {
        value = CORE::CDataNode();
        value.SetValue( (*attribute).second );
        value.SetNodeType( (*attribute).second.GetTypeId() );
        return true;
    }
    const CORE::CDataNode* child = object.Find( name );
    if ( GUCEF_NULL != child )
    {
        value = *child;
        value.SetName( CORE::CString::Empty );
        return true;
    }
    return false;
}

static void
AddId( CORE::CDataNode& message, const CORE::CVariant& id )
{GUCEF_TRACE;

    if ( !id.IsInitialized() || id.IsNULL() || id.IsNIL() )
        message.AddChild( "id", GUCEF_DATATYPE_NIL );
    else
        message.SetAttribute( "id", id );
}

static CORE::CVariant
GetId( const CORE::CDataNode& message )
{GUCEF_TRACE;

    const CORE::CDataNode::TKeyValuePair* attribute = message.GetAttribute( "id" );
    if ( GUCEF_NULL != attribute )
        return (*attribute).second;
    return CORE::CVariant();    // absent or null
}

static CORE::CString
CallKey( const CORE::CVariant& id )
{
    return id.AsString();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::ParseJsonText( const CORE::CString& jsonText, CORE::CDataNode& root )
{GUCEF_TRACE;

    root.Clear();
    return ParseJson( jsonText, root );
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::ToJsonText( const CORE::CDataNode& root, CORE::CString& jsonText )
{GUCEF_TRACE;

    return StoreJson( root, jsonText );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcEndpoint::RegisterEvents( void )
{GUCEF_TRACE;

    ResponseEvent.Initialize();
    NotificationEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CJsonRpcEndpoint::CJsonRpcEndpoint( CIJsonRpcTransport* transport )
    : CORE::CNotifier()
    , m_transport( transport )
    , m_requestHandler( GUCEF_NULL )
    , m_dialect( JSONRPC_DIALECT_STRICT_2_0 )
    , m_defaultCallTimeoutInMs( DefaultCallTimeoutInMs )
    , m_nextCallId( 1 )
    , m_pendingCalls()
    , m_outgoingCallFields()
    , m_isWaiting( false )
    , m_waitDone( false )
    , m_waitKey()
    , m_waitResponse()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CJsonRpcEndpoint::~CJsonRpcEndpoint()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CJsonRpcEndpoint::SetTransport( CIJsonRpcTransport* transport )
{GUCEF_TRACE;

    m_transport = transport;
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcEndpoint::SetDialect( EJsonRpcDialect dialect )
{GUCEF_TRACE;

    m_dialect = dialect;
}

/*-------------------------------------------------------------------------*/

EJsonRpcDialect
CJsonRpcEndpoint::GetDialect( void ) const
{GUCEF_TRACE;

    return m_dialect;
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcEndpoint::SetRequestHandler( CIJsonRpcRequestHandler* handler )
{GUCEF_TRACE;

    m_requestHandler = handler;
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcEndpoint::SetOutgoingCallField( const CORE::CString& name, const CORE::CVariant& value )
{GUCEF_TRACE;

    if ( name.IsNULLOrEmpty() )
        m_outgoingCallFields.clear();
    else
        m_outgoingCallFields[ name ] = value;
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcEndpoint::SetDefaultCallTimeoutInMs( UInt32 timeoutInMs )
{GUCEF_TRACE;

    m_defaultCallTimeoutInMs = timeoutInMs;
}

/*-------------------------------------------------------------------------*/

UInt32
CJsonRpcEndpoint::GetPendingCallCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_pendingCalls.size();
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcEndpoint::InitMessage( CORE::CDataNode& message ) const
{GUCEF_TRACE;

    message = CORE::CDataNode( GUCEF_DATATYPE_OBJECT );
    if ( JSONRPC_DIALECT_STRICT_2_0 == m_dialect )
        message.SetAttribute( "jsonrpc", "2.0" );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcEndpoint::AddCallFields( CORE::CDataNode& message ) const
{GUCEF_TRACE;

    for ( std::map< CORE::CString, CORE::CVariant >::const_iterator i = m_outgoingCallFields.begin(); i != m_outgoingCallFields.end(); ++i )
        message.SetAttribute( (*i).first, (*i).second );
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::SendNode( const CORE::CDataNode& message )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_transport )
        return false;

    CORE::CString text;
    if ( !StoreJson( message, text ) )
        return false;
    return m_transport->SendMessageText( text );
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::BuildCall( const CORE::CString& method       ,
                             const CORE::CDataNode* params     ,
                             bool isNotification               ,
                             UInt64& callId                    ,
                             UInt32 timeoutInMs                ,
                             CORE::CDataNode& message          )
{GUCEF_TRACE;

    if ( method.IsNULLOrEmpty() )
        return false;
    if ( GUCEF_NULL != params && !IsContainerType( params->GetNodeType() ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "JsonRpc: The parameters of a call must be an array or an object" );
        return false;
    }

    InitMessage( message );
    message.SetAttribute( "method", method );
    if ( !isNotification )
    {
        callId = m_nextCallId++;
        message.SetAttribute( "id", callId );

        SPendingCall pending;
        pending.callId = CORE::CVariant( callId );
        pending.deadlineInMs = CORE::CSystemMonotonicClock::Instance().GetMonotonicTimeInMs() + ( 0 == timeoutInMs ? m_defaultCallTimeoutInMs : timeoutInMs );
        m_pendingCalls[ CallKey( pending.callId ) ] = pending;
    }
    if ( GUCEF_NULL != params )
        AddMember( message, "params", *params );
    AddCallFields( message );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::Call( const CORE::CString& method       ,
                        const CORE::CDataNode* params     ,
                        UInt64& callId                    ,
                        UInt32 timeoutInMs                )
{GUCEF_TRACE;

    CORE::CDataNode message;
    callId = 0;

    // The call is registered before it is sent as an answer can arrive before the send even returns
    if ( !BuildCall( method, params, false, callId, timeoutInMs, message ) )
        return false;

    if ( !SendNode( message ) )
    {
        m_pendingCalls.erase( CallKey( CORE::CVariant( callId ) ) );
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::Notify( const CORE::CString& method, const CORE::CDataNode* params )
{GUCEF_TRACE;

    CORE::CDataNode message;
    UInt64 unused = 0;
    if ( !BuildCall( method, params, true, unused, 0, message ) )
        return false;
    return SendNode( message );
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::CallBatch( TBatchItemVector& items, UInt32 timeoutInMs )
{GUCEF_TRACE;

    if ( items.empty() )
        return false;

    CORE::CDataNode batch( GUCEF_DATATYPE_ARRAY );
    for ( size_t i=0; i<items.size(); ++i )
    {
        CORE::CDataNode message;
        if ( !BuildCall( items[ i ].method, items[ i ].hasParams ? &items[ i ].params : GUCEF_NULL, items[ i ].isNotification, items[ i ].callId, timeoutInMs, message ) )
        {
            // forget the calls of this batch which were already registered
            for ( size_t n=0; n<i; ++n )
                if ( !items[ n ].isNotification )
                    m_pendingCalls.erase( CallKey( CORE::CVariant( items[ n ].callId ) ) );
            return false;
        }
        batch.AddChild( message );
    }

    if ( !SendNode( batch ) )
    {
        for ( size_t n=0; n<items.size(); ++n )
            if ( !items[ n ].isNotification )
                m_pendingCalls.erase( CallKey( CORE::CVariant( items[ n ].callId ) ) );
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::CallAndWait( const CORE::CString& method   ,
                               const CORE::CDataNode* params ,
                               SJsonRpcResponse& response    ,
                               UInt32 timeoutInMs            )
{GUCEF_TRACE;

    if ( m_isWaiting )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "JsonRpc: CallAndWait() is not supported while another CallAndWait() is in progress" );
        return false;
    }

    m_isWaiting = true;
    m_waitDone = false;

    UInt64 callId = 0;
    m_waitKey = CallKey( CORE::CVariant( (UInt64) m_nextCallId ) );     // the id the call is about to get
    if ( !Call( method, params, callId, timeoutInMs ) )
    {
        m_isWaiting = false;
        return false;
    }

    // Time passes through the application's pulse generator which also drives the sockets
    CORE::PulseGeneratorPtr pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();
    while ( !m_waitDone )
    {
        pulseGenerator->ForceDirectPulse();
        if ( !ExpireTimedOutCalls() )
            return false;       // we were deleted
        if ( !m_waitDone )
            MT::ThreadDelay( 1 );
    }

    response = m_waitResponse;
    m_isWaiting = false;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::DeliverResponse( SJsonRpcResponse& response )
{GUCEF_TRACE;

    if ( m_isWaiting && !m_waitDone && CallKey( response.id ) == m_waitKey )
    {
        m_waitResponse = response;
        m_waitDone = true;
        return true;
    }

    TJsonRpcResponseEventData eData( response );
    return NotifyObservers( ResponseEvent, &eData );
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::ExpireTimedOutCalls( UInt32* expiredCount )
{GUCEF_TRACE;

    if ( GUCEF_NULL != expiredCount )
        *expiredCount = 0;
    if ( m_pendingCalls.empty() )
        return true;

    UInt64 now = CORE::CSystemMonotonicClock::Instance().GetMonotonicTimeInMs();
    std::vector< CORE::CVariant > expired;
    for ( TPendingCallMap::iterator i = m_pendingCalls.begin(); i != m_pendingCalls.end(); ++i )
        if ( (*i).second.deadlineInMs <= now )
            expired.push_back( (*i).second.callId );

    for ( size_t n=0; n<expired.size(); ++n )
    {
        m_pendingCalls.erase( CallKey( expired[ n ] ) );

        SJsonRpcResponse response;
        response.id = expired[ n ];
        response.isError = true;
        response.isLocalError = true;
        response.error.code = JSONRPC_ERROR_CALL_TIMEOUT;
        response.error.message = "The call was not answered in time";
        if ( GUCEF_NULL != expiredCount )
            ++(*expiredCount);
        if ( !DeliverResponse( response ) )
            return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::FailAllPendingCalls( Int32 errorCode, const CORE::CString& errorMessage )
{GUCEF_TRACE;

    TPendingCallMap pending;
    pending.swap( m_pendingCalls );

    for ( TPendingCallMap::iterator i = pending.begin(); i != pending.end(); ++i )
    {
        SJsonRpcResponse response;
        response.id = (*i).second.callId;
        response.isError = true;
        response.isLocalError = true;
        response.error.code = errorCode;
        response.error.message = errorMessage;
        if ( !DeliverResponse( response ) )
            return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::SendResponse( const CORE::CVariant& callId, const CORE::CDataNode& result )
{GUCEF_TRACE;

    CORE::CDataNode message;
    InitMessage( message );
    AddId( message, callId );
    AddMember( message, "result", result );
    return SendNode( message );
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcEndpoint::SendErrorResponse( const CORE::CVariant& callId       ,
                                     Int32 errorCode                    ,
                                     const CORE::CString& errorMessage  ,
                                     const CORE::CDataNode* errorData   )
{GUCEF_TRACE;

    CORE::CDataNode message;
    InitMessage( message );
    AddId( message, callId );
    CORE::CDataNode* error = message.AddChild( "error", GUCEF_DATATYPE_OBJECT );
    error->SetAttribute( "code", errorCode );
    error->SetAttribute( "message", errorMessage );
    if ( GUCEF_NULL != errorData )
        AddMember( *error, "data", *errorData );
    return SendNode( message );
}

/*-------------------------------------------------------------------------*/

CJsonRpcEndpoint::EProcessResult
CJsonRpcEndpoint::ProcessIncoming( const CORE::CString& messageText )
{GUCEF_TRACE;

    CORE::CDataNode root;
    if ( !ParseJson( messageText, root ) )
    {
        // the id of the message is not known: null
        SendErrorResponse( CORE::CVariant(), JSONRPC_ERROR_PARSE, "Parse error" );
        return PROCESS_INVALID;
    }

    if ( GUCEF_DATATYPE_ARRAY != root.GetNodeType() )
        return ProcessMessageNode( root, GUCEF_NULL );

    // A batch: the responses to its calls are sent together as one batch
    if ( 0 == root.GetNrOfChildNodes() )
    {
        SendErrorResponse( CORE::CVariant(), JSONRPC_ERROR_INVALID_REQUEST, "Invalid request: empty batch" );
        return PROCESS_INVALID;
    }

    CORE::CDataNode batchResponses( GUCEF_DATATYPE_ARRAY );
    EProcessResult overall = PROCESS_OK;
    for ( CORE::CDataNode::const_iterator i = root.ConstBegin(); i != root.ConstEnd(); ++i )
    {
        EProcessResult result = ProcessMessageNode( *(*i), &batchResponses );
        if ( PROCESS_DELETED == result )
            return PROCESS_DELETED;
        if ( PROCESS_INVALID == result )
            overall = PROCESS_INVALID;
    }
    if ( batchResponses.HasChildren() )
        SendNode( batchResponses );
    return overall;
}

/*-------------------------------------------------------------------------*/

CJsonRpcEndpoint::EProcessResult
CJsonRpcEndpoint::ProcessMessageNode( const CORE::CDataNode& node, CORE::CDataNode* batchResponses )
{GUCEF_TRACE;

    // Responses we generate go into the batch if there is one, otherwise they are sent right away
    CORE::CDataNode response;
    bool haveResponse = false;
    EProcessResult result = PROCESS_OK;

    InitMessage( response );

    if ( GUCEF_DATATYPE_OBJECT != node.GetNodeType() )
    {
        AddId( response, CORE::CVariant() );
        CORE::CDataNode* error = response.AddChild( "error", GUCEF_DATATYPE_OBJECT );
        error->SetAttribute( "code", (Int32) JSONRPC_ERROR_INVALID_REQUEST );
        error->SetAttribute( "message", "Invalid request: a message must be an object" );
        haveResponse = true;
        result = PROCESS_INVALID;
    }
    else
    {
        const CORE::CDataNode::TKeyValuePair* version = node.GetAttribute( "jsonrpc" );
        bool versionOk = JSONRPC_DIALECT_LENIENT == m_dialect || ( GUCEF_NULL != version && (*version).second.AsString() == "2.0" );
        bool hasMethod = GUCEF_NULL != node.GetAttribute( "method" ) && node.GetAttribute( "method" )->second.IsString();
        bool hasId = HasMember( node, "id" );

        if ( hasMethod && versionOk )
        {
            CORE::CString method = node.GetAttribute( "method" )->second.AsString();

            CORE::CDataNode params;
            bool hasParams = GetMember( node, "params", params );
            if ( hasParams && !IsContainerType( params.GetNodeType() ) )
            {
                // the parameters have to be structured
                if ( hasId )
                {
                    AddId( response, GetId( node ) );
                    CORE::CDataNode* error = response.AddChild( "error", GUCEF_DATATYPE_OBJECT );
                    error->SetAttribute( "code", (Int32) JSONRPC_ERROR_INVALID_PARAMS );
                    error->SetAttribute( "message", "Invalid params: must be an array or an object" );
                    haveResponse = true;
                }
                result = PROCESS_INVALID;
            }
            else
            if ( !hasId )
            {
                SJsonRpcNotification notification;
                notification.method = method;
                notification.params = params;
                notification.hasParams = hasParams;
                notification.envelope = node;
                TJsonRpcNotificationEventData eData( notification );
                if ( !NotifyObservers( NotificationEvent, &eData ) )
                    return PROCESS_DELETED;
            }
            else
            {
                SJsonRpcRequest request;
                request.id = GetId( node );
                request.method = method;
                request.params = params;
                request.hasParams = hasParams;
                request.envelope = node;

                SJsonRpcResponse handlerResponse;
                EJsonRpcRequestResult handled = JSONRPC_REQUEST_HANDLED;
                if ( GUCEF_NULL == m_requestHandler )
                {
                    handlerResponse.isError = true;
                    handlerResponse.error.code = JSONRPC_ERROR_METHOD_NOT_FOUND;
                    handlerResponse.error.message = "Method not found";
                }
                else
                {
                    try
                    {
                        handled = m_requestHandler->OnJsonRpcRequest( *this, request, handlerResponse );
                    }
                    catch ( ... )
                    {
                        handled = JSONRPC_REQUEST_HANDLED;
                        handlerResponse = SJsonRpcResponse();
                        handlerResponse.isError = true;
                        handlerResponse.error.code = JSONRPC_ERROR_INTERNAL;
                        handlerResponse.error.message = "Internal error";
                    }
                }

                if ( JSONRPC_REQUEST_HANDLED == handled )
                {
                    AddId( response, request.id );
                    if ( handlerResponse.isError )
                    {
                        CORE::CDataNode* error = response.AddChild( "error", GUCEF_DATATYPE_OBJECT );
                        error->SetAttribute( "code", handlerResponse.error.code );
                        error->SetAttribute( "message", handlerResponse.error.message );
                        if ( handlerResponse.error.hasData )
                            AddMember( *error, "data", handlerResponse.error.data );
                    }
                    else
                    {
                        AddMember( response, "result", handlerResponse.result );
                    }
                    haveResponse = true;
                }
            }
        }
        else
        if ( !hasMethod && ( HasMember( node, "result" ) || HasMember( node, "error" ) ) )
        {
            if ( !versionOk )
                return PROCESS_INVALID;
            return HandleResponseNode( node );
        }
        else
        {
            // neither a call nor an answer
            AddId( response, hasId ? GetId( node ) : CORE::CVariant() );
            CORE::CDataNode* error = response.AddChild( "error", GUCEF_DATATYPE_OBJECT );
            error->SetAttribute( "code", (Int32) JSONRPC_ERROR_INVALID_REQUEST );
            error->SetAttribute( "message", "Invalid request" );
            haveResponse = true;
            result = PROCESS_INVALID;
        }
    }

    if ( haveResponse )
    {
        if ( GUCEF_NULL != batchResponses )
            batchResponses->AddChild( response );
        else
            SendNode( response );
    }
    return result;
}

/*-------------------------------------------------------------------------*/

CJsonRpcEndpoint::EProcessResult
CJsonRpcEndpoint::HandleResponseNode( const CORE::CDataNode& node )
{GUCEF_TRACE;

    CORE::CVariant id = GetId( node );
    TPendingCallMap::iterator pending = m_pendingCalls.find( CallKey( id ) );
    if ( pending == m_pendingCalls.end() )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "JsonRpc: Received a response to a call which is not pending (late or unknown id): " + id.AsString() );
        return PROCESS_OK;
    }

    SJsonRpcResponse response;
    response.id = (*pending).second.callId;
    response.envelope = node;
    m_pendingCalls.erase( pending );

    CORE::CDataNode error;
    if ( GetMember( node, "error", error ) )
    {
        response.isError = true;
        const CORE::CDataNode::TKeyValuePair* code = error.GetAttribute( "code" );
        const CORE::CDataNode::TKeyValuePair* message = error.GetAttribute( "message" );
        response.error.code = GUCEF_NULL != code ? (Int32) (*code).second.AsInt32() : 0;
        response.error.message = GUCEF_NULL != message ? (*message).second.AsString() : CORE::CString();
        response.error.hasData = GetMember( error, "data", response.error.data );
    }
    else
    {
        GetMember( node, "result", response.result );
    }

    if ( !DeliverResponse( response ) )
        return PROCESS_DELETED;
    return PROCESS_OK;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
