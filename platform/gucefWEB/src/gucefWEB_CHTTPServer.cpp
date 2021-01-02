/*
 *  gucefWEB: GUCEF module providing Web application functionality 
 *  for standardized protocols
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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_WEB_CDEFAULTHTTPSERVERROUTERCONTROLLER_H
#include "gucefWEB_CDefaultHTTPServerRouterController.h"
#define GUCEF_WEB_CDEFAULTHTTPSERVERROUTERCONTROLLER_H
#endif /* GUCEF_WEB_CDEFAULTHTTPSERVERROUTERCONTROLLER_H ? */

#ifndef GUCEF_WEB_CDEFAULTHTTPSERVERREQUESTHANDLER_H
#include "gucefWEB_CDefaultHttpServerRequestHandler.h"
#define GUCEF_WEB_CDEFAULTHTTPSERVERREQUESTHANDLER_H
#endif /* GUCEF_WEB_CDEFAULTHTTPSERVERREQUESTHANDLER_H ? */

#ifndef GUCEF_WEB_CASYNCHTTPSERVERREQUESTHANDLER_H
#include "gucefWEB_CAsyncHttpServerRequestHandler.h"
#define GUCEF_WEB_CASYNCHTTPSERVERREQUESTHANDLER_H
#endif /* GUCEF_WEB_CASYNCHTTPSERVERREQUESTHANDLER_H ? */

#include "gucefWEB_CHTTPServer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

TDefaultHttpServerRequestHandlerFactory g_defaultHttpServerRequestHandlerFactory;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPServer::CHTTPServer( THttpServerRequestHandlerFactory* requestHandlerFactory /* = GUCEF_NULL */ )
    : CORE::CObservingNotifier()              
    , m_tcpServerSocket( false )
    , m_requestHandler( GUCEF_NULL )
    , m_requestHandlerFactory( requestHandlerFactory )
    , m_keepAliveConnections( true )
    , m_processRequestsAsync( false )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_requestHandlerFactory )
    {
        m_requestHandlerFactory = &g_defaultHttpServerRequestHandlerFactory;
    }
    m_requestHandler = m_requestHandlerFactory->Create();

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CHTTPServer::CHTTPServer( CORE::CPulseGenerator& pulsGenerator                                       ,
                          THttpServerRequestHandlerFactory* requestHandlerFactory /* = GUCEF_NULL */ )
    : CORE::CObservingNotifier()                               
    , m_tcpServerSocket( pulsGenerator, false ) 
    , m_requestHandler( GUCEF_NULL )
    , m_requestHandlerFactory( requestHandlerFactory )
    , m_keepAliveConnections( true )
    , m_processRequestsAsync( false )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_requestHandlerFactory )
    {
        m_requestHandlerFactory = &g_defaultHttpServerRequestHandlerFactory;
    }
    m_requestHandler = m_requestHandlerFactory->Create();

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CHTTPServer::~CHTTPServer()
{GUCEF_TRACE;

    UnsubscribeAllFromObserver();
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Register event handlers
    TEventCallback callback( this, &CHTTPServer::OnClientConnectedEvent );
    SubscribeTo( &m_tcpServerSocket                              ,
                 COMCORE::CTCPServerSocket::ClientConnectedEvent ,
                 callback                                        );
}

/*-------------------------------------------------------------------------*/

CIHttpServerRequestHandler* 
CHTTPServer::GetRequestHandler( void ) const
{GUCEF_TRACE;

    return m_requestHandler;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPServer::ListenOnPort( const UInt16 port )
{GUCEF_TRACE;

    return m_tcpServerSocket.ListenOnPort( port );
}

/*-------------------------------------------------------------------------*/

bool 
CHTTPServer::Listen( void )
{GUCEF_TRACE;

    return m_tcpServerSocket.Listen();
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::Close( void )
{GUCEF_TRACE;

    m_tcpServerSocket.Close();
}

/*-------------------------------------------------------------------------*/

bool 
CHTTPServer::SetPort( UInt16 port )
{GUCEF_TRACE;
    
    return m_tcpServerSocket.SetPort( port );
}

/*-------------------------------------------------------------------------*/

UInt16
CHTTPServer::GetPort( void ) const
{GUCEF_TRACE;

    return m_tcpServerSocket.GetPort();
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::OnClientConnectedEvent( CORE::CNotifier* notifier                 ,
                                     const CORE::CEvent& eventid               ,
                                     CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    const COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata );
    const COMCORE::CTCPServerSocket::TConnectionInfo& storage = eData->GetData();

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Client connected" );

    // Subscribe to the connection
    TEventCallback callback( this, &CHTTPServer::OnClientDataReceivedEvent );
    SubscribeTo( storage.connection                               ,
                 COMCORE::CTCPServerConnection::DataRecievedEvent ,
                 callback                                         );
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::OnClientDataReceivedEvent( CORE::CNotifier* notifier                 ,
                                        const CORE::CEvent& eventid               ,
                                        CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    const COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventdata );
    const CORE::CDynamicBuffer& receivedData = eData->GetData();
    COMCORE::CTCPServerConnection* connection = static_cast< COMCORE::CTCPServerConnection* >( notifier );

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): " + CORE::UInt32ToString( receivedData.GetDataSize() ) + " Bytes received from client " + connection->GetRemoteHostName() );

    if ( m_processRequestsAsync )
    {
        // Use a worker thread to actually process the request
        ASyncProcessReceivedData( connection, receivedData );
    }
    else
    {
        // Syncronously process the request in a blocking manner
        CORE::CDynamicBuffer responseBuffer;
        SyncProcessReceivedData( connection, receivedData, responseBuffer );

        // Send the reponse back to the client
        connection->Send( responseBuffer );
        if ( !m_keepAliveConnections )
            connection->Close();
    }
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::OnNotify( CORE::CNotifier* notifier                 ,
                       const CORE::CEvent& eventid               ,
                       CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::ExtractCommaSeparatedValues( const CString& stringToExtractFrom ,
                                          TStringVector& list                ) const
{GUCEF_TRACE;

    // Create a list and make sure we trim each element
    TStringVector elements = stringToExtractFrom.ParseElements( ',' );
    TStringVector::iterator i = elements.begin();
    while ( i != elements.end() )
    {
        list.push_back( (*i).RemoveChar( ' ' ) );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::StripItems( TStringVector& list ,
                         char stripDelim     ) const
{GUCEF_TRACE;

    TStringVector::iterator i = list.begin();
    while ( i != list.end() )
    {
        (*i) = (*i).SubstrToChar( stripDelim, true );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::ASyncProcessReceivedData( COMCORE::CTCPServerConnection* connection ,
                                       const CORE::CDynamicBuffer& inputBuffer   )
{GUCEF_TRACE;

    CAsyncHttpRequestData requestData;
    if ( !ParseRequest( inputBuffer, requestData ) )
        return;

    requestData.clientConnectionId = connection->GetConnectionIndex();
    requestData.remoteClientAddress = connection->GetRemoteHostAddress();
    requestData.keepConnectionsAlive = m_keepAliveConnections;
    requestData.httpServer = this;

    CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();
    if ( taskManager.QueueTask( CAsyncHttpServerRequestHandler::TaskType, &requestData, GUCEF_NULL, &AsObserver() ) )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Queued task to ASync handle the request processing" );
    }
    else
    {
        // Since we are doing async processing the failure to queue a processing task would leave a
        // client in limbo up to their timeout. To prevent this we will handle this scenario here
        // directly

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Unable to ASync queue the request processing (QueueTask failed). As such will respond with 503" );
        
        CHttpResponseData returnData;
        returnData.statusCode = 503;   // Send 'Server too busy' because this is per configured capacity constraints

        CORE::CDynamicBuffer outputBuffer;
        returnData.Serialize( outputBuffer );

        connection->Send( outputBuffer );
        connection->Close();
    }
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::SyncProcessReceivedData( COMCORE::CTCPServerConnection* connection ,
                                      const CORE::CDynamicBuffer& inputBuffer   ,
                                      CORE::CDynamicBuffer& outputBuffer        )
{GUCEF_TRACE;

    try
    {
        CHttpRequestData requestData; 
        if ( !ParseRequest( inputBuffer, requestData ) )
            return;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): About to process request of type: " + requestData.requestType );

        CHttpResponseData response;
        m_requestHandler->OnRequest( requestData, response );
        response.Serialize( outputBuffer );

        connection->Send( outputBuffer );
        if ( !m_keepAliveConnections )
            connection->Close();    
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Exception sync processing received data: " + CORE::CString( e.what() ) );
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CHTTPServer::ParseHeaderFields( const char* bufferPtr       ,
                                const UInt32 bufferSize     ,
                                TStringVector& headerFields ) const
{GUCEF_TRACE;

    try
    {
        UInt32 startIndex = 0;
        UInt32 headerSize = 0;

        // According to the RFC lines are separated using '\r\n'
        for ( UInt32 i=0; i<bufferSize; ++i )
        {
            // Check for the end of line delimiter, cariage return first
            if ( bufferPtr[ i ] == '\r' )
            {
                if ( i+1 < bufferSize )
                {
                    // Check for line feed
                    if ( bufferPtr[ i+1 ] == '\n' )
                    {
                        ++i;
                        if ( i+1 < bufferSize )
                        {
                            // Add the segment to our list
                            headerFields.push_back( CString( bufferPtr+startIndex, i-1-startIndex ) );
                            startIndex = i+1;

                            // Check for empty line which is the end of header delimiter
                            // We start with cariage return
                            if ( bufferPtr[ i+1 ] == '\r' )
                            {
                                ++i;
                                if ( i+1 < bufferSize )
                                {
                                    // Check for line feed
                                    if ( bufferPtr[ i+1 ] == '\n' )
                                    {
                                        // Proper end of header delimiter found, we can stop
                                        headerSize = i+2;
                                        break;
                                    }

                                    // If we get here:
                                    // Some HTTP1.0 implementations add an extra '/r' after a '/r/n' so we have to be robust
                                    // and tolerate this condition
                                }
                                else
                                {
                                    // Not a well formatted HTTP header
                                    headerSize = i;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            // Not a well formatted HTTP header
                            headerSize = i;
                            break;
                        }
                    }
                }
                else
                {
                    // Not a well formatted HTTP header
                    headerSize = i;
                    break;
                }
            }
        }
        return headerSize;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Exception parsing HTTP request header fields: " + CORE::CString( e.what() ) );
        return 0;
    }
}

/*-------------------------------------------------------------------------*/

bool
CHTTPServer::ParseRequest( const CORE::CDynamicBuffer& inputBuffer ,
                           CHttpRequestData& request               )
{GUCEF_TRACE;

    try
    {
        if ( inputBuffer.GetDataSize() == 0 )
        {
            // Invalid input
            return false;
        }

        // Parse all the HTTP Header fields out of the buffer
        TStringVector headerFields;
        UInt32 headerSize = ParseHeaderFields( static_cast< const char* >( inputBuffer.GetConstBufferPtr() ) ,
                                               inputBuffer.GetDataSize()                                     ,
                                               headerFields                                                  );

        // Sanity check on the parsed result
        if ( headerSize == 0 || headerFields.size() == 0 )
        {
            return false;
        }

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Finished parsing request header" );

        CString temp = headerFields.front().CompactRepeatingChar( ' ' );
        headerFields.erase( headerFields.begin() );

        // Parse the request type from the first line
        request.requestType = temp.SubstrToChar( ' ', true );
        temp = temp.CutChars( request.requestType.Length()+1, true );

        // Parse the request URI
        request.requestUri = temp.SubstrToChar( ' ', true );
        temp = temp.CutChars( request.requestUri.Length()+1, true );

        // Parse the request protocol and its version
        request.requestProtocol = temp.SubstrToChar( '/', true );
        request.requestProtocolVersion = temp.CutChars( request.requestProtocol.Length()+1, true );

        // Parse all the subsequent HTTP header fields
        TStringVector::iterator i = headerFields.begin();
        while ( i != headerFields.end() )
        {
            // Parse the header name and header value out of the header field
            CString& headerField = (*i);
            CString headerName = headerField.SubstrToChar( ':', true );
            CString headerValue( headerField.C_String() + headerName.Length(), headerField.Length() - headerName.Length() );

            // Remove additional spaces and lowercase the headername for easy comparison
            headerName = headerName.RemoveChar( ' ' ).Lowercase();
            headerValue = headerValue.RemoveChar( ' ' );

            //  Remove the ':' prefix
            headerValue = headerValue.CutChars( 1, true );

            // Now that we have formatted the header name + value we can use them
            if ( headerName == "accept" )
            {
                ExtractCommaSeparatedValues( headerValue                     ,
                                             request.resourceRepresentations );
                StripItems( request.resourceRepresentations, ';' );
            }
            else
            if ( headerName == "content-type" )
            {
                request.contentRepresentation = headerValue;
            }
            else
            if ( headerName == "if-match" )
            {
                ExtractCommaSeparatedValues( headerValue              ,
                                             request.resourceVersions );
            }
            else
            if ( headerName == "cookie" )
            {
                request.transactionID = headerValue;
            }
            else
            if ( headerName == "host" )
            {
                request.requestHost = headerValue;
            }
            ++i;
        }

        // Set the content as a sub-segment of our data buffer
        if ( inputBuffer.GetDataSize() - headerSize > 0 )
        {
            request.content.LinkTo( inputBuffer.GetConstBufferPtr( headerSize ) ,
                                    inputBuffer.GetDataSize() - headerSize      );
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Exception parsing request: " + CORE::CString( e.what() ) );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/
                         
bool
CHTTPServer::Lock( void ) const
{GUCEF_TRACE;
    
    return m_lock.Lock();
}

/*-------------------------------------------------------------------------*/
                         
bool
CHTTPServer::Unlock( void ) const
{GUCEF_TRACE;
    
    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------*/

CIHTTPServerRouterController* 
CHTTPServer::GetRouterController( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_requestHandler )
    {
        return m_requestHandler->GetRouterController();
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

THttpServerRequestHandlerFactory*
CHTTPServer::GetRequestHandlerFactory( void ) const
{GUCEF_TRACE;

    return m_requestHandlerFactory;
}

/*-------------------------------------------------------------------------*/

bool 
CHTTPServer::SendResponseASync( UInt32 connectionId                  ,
                                const CORE::CDynamicBuffer& response ,
                                const COMCORE::CIPAddress& remoteIP  )
{GUCEF_TRACE;

    return m_tcpServerSocket.SendToConnection( connectionId                 , 
                                               response.GetConstBufferPtr() ,
                                               response.GetDataSize()       ,
                                               &remoteIP                    );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
