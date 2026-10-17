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

#ifndef GUCEF_WEB_CJSONRPCENDPOINT_H
#define GUCEF_WEB_CJSONRPCENDPOINT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include <vector>

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_WEB_JSONRPCTYPES_H
#include "gucefWEB_JsonRpcTypes.h"
#define GUCEF_WEB_JSONRPCTYPES_H
#endif /* GUCEF_WEB_JSONRPCTYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  One side of a JSON-RPC conversation. JSON-RPC is symmetric: both sides can make calls, send notifications
 *  and answer calls, so the same class is the base of the client and of every connection of the server.
 *
 *  The endpoint sends messages through a CIJsonRpcTransport and is given the messages received by that transport
 *  through ProcessIncoming(). JSON is handled by the "json" dstore codec which is provided by a plugin
 *  (dstorepluginJSONPARSER) and found through the codec registry: without it nothing can be sent or received.
 *
 *  Events:
 *    ResponseEvent      TJsonRpcResponseEventData      the answer to a call, also a timeout or lost transport
 *    NotificationEvent  TJsonRpcNotificationEventData  the peer sent a notification
 *
 *  Calls the peer makes are given to the request handler, see CIJsonRpcRequestHandler.
 *  Not thread-safe: use from the thread which receives the messages.
 */
class GUCEF_WEB_PUBLIC_CPP CJsonRpcEndpoint : public CORE::CNotifier
{
    public:

    static const CORE::CEvent ResponseEvent;
    static const CORE::CEvent NotificationEvent;

    static void RegisterEvents( void );

    enum EProcessResult
    {
        PROCESS_OK        ,     /**< the message was valid and dealt with */
        PROCESS_INVALID   ,     /**< the message was not valid, the peer was told so where the protocol calls for that */
        PROCESS_DELETED         /**< this endpoint was deleted as a result of dealing with the message: do not touch it anymore */
    };

    /** One call or notification of a batch */
    struct SBatchItem
    {
        CORE::CString method;
        CORE::CDataNode params;
        bool hasParams;
        bool isNotification;                /**< no answer is expected, no id is assigned */
        UInt64 callId;                      /**< set by CallBatch() for calls */

        SBatchItem( void ) : method(), params(), hasParams( false ), isNotification( false ), callId( 0 ) {}
    };
    typedef std::vector< SBatchItem > TBatchItemVector;

    static const UInt32 DefaultCallTimeoutInMs = 30000;

    public:

    explicit CJsonRpcEndpoint( CIJsonRpcTransport* transport = GUCEF_NULL );

    virtual ~CJsonRpcEndpoint();

    /** The transport must outlive the endpoint or be unset (GUCEF_NULL) before it is destroyed */
    void SetTransport( CIJsonRpcTransport* transport );

    void SetDialect( EJsonRpcDialect dialect );

    EJsonRpcDialect GetDialect( void ) const;

    void SetRequestHandler( CIJsonRpcRequestHandler* handler );

    /**
     *  Adds a field to every call and notification this endpoint sends, next to "method" and "params".
     *  Allows protocols built on JSON-RPC to add to the envelope, for example the sessionId of the Chrome DevTools Protocol.
     *  A scalar value is added as is. Passing an empty name removes all such fields.
     */
    void SetOutgoingCallField( const CORE::CString& name, const CORE::CVariant& value );

    void SetDefaultCallTimeoutInMs( UInt32 timeoutInMs );

    /**
     *  Makes a call. The answer is delivered through ResponseEvent, which also reports a timeout or
     *  the loss of the transport as a local error.
     *  @param params       the parameters, an array or an object node. GUCEF_NULL for none
     *  @param callId       receives the id of the call, which the response carries
     *  @param timeoutInMs  0 uses the default
     */
    bool Call( const CORE::CString& method       ,
               const CORE::CDataNode* params     ,
               UInt64& callId                    ,
               UInt32 timeoutInMs = 0            );

    /**
     *  Makes a call and waits for the answer by pumping the application's pulse generator.
     *  The answer is returned and not delivered as an event.
     *  @return true if an answer, which can be an error, came back. false if the call could not even be made
     */
    bool CallAndWait( const CORE::CString& method   ,
                      const CORE::CDataNode* params ,
                      SJsonRpcResponse& response    ,
                      UInt32 timeoutInMs = 0        );

    /** Sends a message which expects no answer */
    bool Notify( const CORE::CString& method, const CORE::CDataNode* params );

    /**
     *  Sends several calls and notifications as a single batch message. The ids of the calls are set in the items.
     *  Their answers are delivered as individual responses.
     */
    bool CallBatch( TBatchItemVector& items, UInt32 timeoutInMs = 0 );

    /** Answers a call, to be used for calls whose handler deferred the answer */
    bool SendResponse( const CORE::CVariant& callId, const CORE::CDataNode& result );

    bool SendErrorResponse( const CORE::CVariant& callId       ,
                            Int32 errorCode                    ,
                            const CORE::CString& errorMessage  ,
                            const CORE::CDataNode* errorData = GUCEF_NULL );

    /**
     *  Helpers for the JSON conventions used by the endpoint: parsing goes through the "json" dstore codec,
     *  writing is done here as the codec's writer turns an object with exactly one member into a bare value.
     *  A JSON object is a node with its scalar members as attributes and its object and array members as children,
     *  an array is a node of the array type with its items as children and null is a node of the nil type.
     */
    static bool ParseJsonText( const CORE::CString& jsonText, CORE::CDataNode& root );

    static bool ToJsonText( const CORE::CDataNode& root, CORE::CString& jsonText );

    /** To be called by the owner of the transport for every complete message that was received */
    EProcessResult ProcessIncoming( const CORE::CString& messageText );

    /**
     *  Delivers a timeout response for every call which was not answered in time. To be called regularly,
     *  CJsonRpcClient does this on a timer.
     *  @return false if this endpoint was deleted as a result of delivering a response
     */
    bool ExpireTimedOutCalls( UInt32* expiredCount = GUCEF_NULL );

    /**
     *  Delivers an error response for every call still waiting, to be used when the transport is lost
     *  @return false if this endpoint was deleted as a result of delivering a response
     */
    bool FailAllPendingCalls( Int32 errorCode, const CORE::CString& errorMessage );

    UInt32 GetPendingCallCount( void ) const;

    private:

    CJsonRpcEndpoint( const CJsonRpcEndpoint& src );
    CJsonRpcEndpoint& operator=( const CJsonRpcEndpoint& src );

    struct SPendingCall
    {
        CORE::CVariant callId;
        UInt64 deadlineInMs;
    };
    typedef std::map< CORE::CString, SPendingCall > TPendingCallMap;

    void InitMessage( CORE::CDataNode& message ) const;

    void AddCallFields( CORE::CDataNode& message ) const;

    bool BuildCall( const CORE::CString& method, const CORE::CDataNode* params, bool isNotification, UInt64& callId, UInt32 timeoutInMs, CORE::CDataNode& message );

    bool SendNode( const CORE::CDataNode& message );

    EProcessResult ProcessMessageNode( const CORE::CDataNode& node, CORE::CDataNode* batchResponses );

    EProcessResult HandleResponseNode( const CORE::CDataNode& node );

    bool DeliverResponse( SJsonRpcResponse& response );

    private:

    CIJsonRpcTransport* m_transport;
    CIJsonRpcRequestHandler* m_requestHandler;
    EJsonRpcDialect m_dialect;
    UInt32 m_defaultCallTimeoutInMs;
    UInt64 m_nextCallId;
    TPendingCallMap m_pendingCalls;
    std::map< CORE::CString, CORE::CVariant > m_outgoingCallFields;

    // state of CallAndWait()
    bool m_isWaiting;
    bool m_waitDone;
    CORE::CString m_waitKey;
    SJsonRpcResponse m_waitResponse;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CJSONRPCENDPOINT_H ? */
