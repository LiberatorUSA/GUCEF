/*
 *  gucefCOM: GUCEF module providing communication implementations 
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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERROUTERCONTROLLER_H
#include "gucefCOM_CDefaultHTTPServerRouterController.h"
#define GUCEF_COM_CDEFAULTHTTPSERVERROUTERCONTROLLER_H
#endif /* GUCEF_COM_CDEFAULTHTTPSERVERROUTERCONTROLLER_H ? */

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERREQUESTHANDLER_H
#include "gucefCOM_CDefaultHttpServerRequestHandler.h"
#define GUCEF_COM_CDEFAULTHTTPSERVERREQUESTHANDLER_H
#endif /* GUCEF_COM_CDEFAULTHTTPSERVERREQUESTHANDLER_H ? */

#include "gucefCOM_CHTTPServer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPServer::CHTTPServer( CIHTTPServerRouterController* routerController /* = GUCEF_NULL */ ,
                          CIHttpServerRequestHandler* requestHandler     /* = GUCEF_NULL */ )
    : CObserver()              
    , m_tcpServerSocket( false )
    , m_routerController( NULL )
    , m_keepAliveConnections( true )
{GUCEF_TRACE;

    if ( GUCEF_NULL == routerController )
    {
        m_routerController = new CDefaultHTTPServerRouterController( *this );
    }
    else
    {
        m_routerController = routerController;
    }

    if ( GUCEF_NULL == requestHandler )
    {
        m_requestHandler = new CDefaultHttpServerRequestHandler( m_routerController );
    }
    else
    {
        m_requestHandler = requestHandler;
    }

    m_tcpServerSocket.Subscribe( this );
}

/*-------------------------------------------------------------------------*/

CHTTPServer::CHTTPServer( CORE::CPulseGenerator& pulsGenerator                              ,
                          CIHTTPServerRouterController* routerController /* = GUCEF_NULL */ ,
                          CIHttpServerRequestHandler* requestHandler     /* = GUCEF_NULL */ )
    : CObserver()                               
    , m_tcpServerSocket( pulsGenerator, false ) 
    , m_routerController( NULL )                
    , m_keepAliveConnections( true )
{GUCEF_TRACE;

    if ( NULL == routerController )
    {
        m_routerController = new CDefaultHTTPServerRouterController( *this );
    }
    else
    {
        m_routerController = routerController;
    }

    if ( GUCEF_NULL == requestHandler )
    {
        m_requestHandler = new CDefaultHttpServerRequestHandler( m_routerController );
    }
    else
    {
        m_requestHandler = requestHandler;
    }

    m_tcpServerSocket.Subscribe( this );
}

/*-------------------------------------------------------------------------*/

CHTTPServer::~CHTTPServer()
{GUCEF_TRACE;

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
CHTTPServer::OnNotify( CORE::CNotifier* notifier                 ,
                       const CORE::CEvent& eventid               ,
                       CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( COMCORE::CTCPServerSocket::ClientConnectedEvent == eventid )
    {
        const COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata );
        const COMCORE::CTCPServerSocket::TConnectionInfo& storage = eData->GetData();

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Client connected" );

        // Subscribe to the connection
        storage.connection->Subscribe( this );
    }
    else
    if ( COMCORE::CTCPServerConnection::DataRecievedEvent == eventid )
    {
        const COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventdata );
        const CORE::CDynamicBuffer& receivedData = eData->GetData();
        COMCORE::CTCPServerConnection* connection = static_cast< COMCORE::CTCPServerConnection* >( notifier );

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): " + CORE::UInt32ToString( receivedData.GetDataSize() ) + " Bytes received from client " + connection->GetRemoteHostName() );

        // Process the request
        CORE::CDynamicBuffer responseBuffer;
        ProcessReceivedData( receivedData, responseBuffer );

        // Send the reponse back to the client
        connection->Send( responseBuffer.GetConstBufferPtr(), responseBuffer.GetDataSize() );
        if ( !m_keepAliveConnections )
            connection->Close();
    }
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
CHTTPServer::ProcessReceivedData( const CORE::CDynamicBuffer& inputBuffer ,
                                  CORE::CDynamicBuffer& outputBuffer      )
{GUCEF_TRACE;

    try
    {
        CHttpResponseData* returnData = NULL;
        CHttpRequestData* requestData = ParseRequest( inputBuffer );

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): About to process request of type: " + requestData->requestType );

        if ( NULL != requestData )
        {
            if ( requestData->requestType == "GET" )
            {
                returnData = m_requestHandler->OnRead( *requestData );
            }
            else
            if ( requestData->requestType == "HEAD" )
            {
                returnData = m_requestHandler->OnReadMetaData( *requestData );
            }
            else
            if ( requestData->requestType == "POST" )
            {
                returnData = m_requestHandler->OnCreate( *requestData );
            }
            else
            if ( requestData->requestType == "PUT" )
            {
                returnData = m_requestHandler->OnUpdate( *requestData, false );
            }
            else
            if ( requestData->requestType == "PATCH" )
            {
                returnData = m_requestHandler->OnUpdate( *requestData, true );
            }
            else
            if ( requestData->requestType == "DELETE" )
            {
                returnData = m_requestHandler->OnDelete( *requestData );
            }
        }

        if ( NULL != returnData )
        {
            ParseResponse( *returnData  ,
                           outputBuffer );
        }
    }
    catch ( std::exception& )
    {
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
    catch ( std::exception& )
    {
        return 0;
    }
}

/*-------------------------------------------------------------------------*/

CHttpRequestData*
CHTTPServer::ParseRequest( const CORE::CDynamicBuffer& inputBuffer )
{GUCEF_TRACE;

    try
    {
        if ( inputBuffer.GetDataSize() == 0 )
        {
            // Invalid input
            return NULL;
        }

        // Parse all the HTTP Header fields out of the buffer
        TStringVector headerFields;
        UInt32 headerSize = ParseHeaderFields( static_cast< const char* >( inputBuffer.GetConstBufferPtr() ) ,
                                               inputBuffer.GetDataSize()                                     ,
                                               headerFields                                                  );

        // Sanity check on the parsed result
        if ( headerSize == 0 || headerFields.size() == 0 )
        {
            return NULL;
        }

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Finished parsing request header" );

        CHttpRequestData* request = new CHttpRequestData();

        CString temp = headerFields.front().CompactRepeatingChar( ' ' );
        headerFields.erase( headerFields.begin() );

        // Parse the request type from the first line
        request->requestType = temp.SubstrToChar( ' ', true );
        temp = temp.CutChars( request->requestType.Length()+1, true );

        // Parse the request URI
        request->requestUri = temp.SubstrToChar( ' ', true );

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
                ExtractCommaSeparatedValues( headerValue                      ,
                                             request->resourceRepresentations );
                StripItems( request->resourceRepresentations, ';' );
            }
            else
            if ( headerName == "content-type" )
            {
                ExtractCommaSeparatedValues( headerValue               ,
                                             request->resourceVersions );
                StripItems( request->resourceRepresentations, ';' );
            }
            else
            if ( headerName == "if-match" )
            {
                ExtractCommaSeparatedValues( headerValue               ,
                                             request->resourceVersions );
            }
            else
            if ( headerName == "cookie" )
            {
                request->transactionID = headerValue;
            }
            else
            if ( headerName == "host" )
            {
                request->requestHost = headerValue;
            }
            ++i;
        }

        // Set the content as a sub-segment of our data buffer
        if ( inputBuffer.GetDataSize() - headerSize > 0 )
        {
            request->content.LinkTo( inputBuffer.GetConstBufferPtr( headerSize ) ,
                                     inputBuffer.GetDataSize() - headerSize      );
        }

        return request;
    }
    catch ( std::exception& )
    {
        return GUCEF_NULL;
    }
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::ParseResponse( const CHttpResponseData& returnData  ,
                            CORE::CDynamicBuffer& outputBuffer )
{GUCEF_TRACE;

    CString response = "HTTP/1.1 " + CORE::Int32ToString( returnData.statusCode ) + "\r\nServer: gucefCOM\r\n";
    if ( m_keepAliveConnections )
        response += "Connection: keep-alive\r\n";
    else
        response += "Connection: close\r\n";
    if ( !returnData.location.IsNULLOrEmpty() )
    {
        response += "Content-Location: " + returnData.location + "\r\n";
    }
    if ( !returnData.eTag.IsNULLOrEmpty() )
    {
        response += "ETag: " + returnData.eTag + "\r\n";
    }
    if ( !returnData.cacheability.IsNULLOrEmpty() )
    {
        response += "Cache-Control: " + returnData.cacheability + "\r\n";
    }
    if ( !returnData.lastModified.IsNULLOrEmpty() )
    {
        response += "Last-Modified: " + returnData.lastModified + "\r\n";
    }
    if ( returnData.content.GetDataSize() > 0 )
    {
        response += "Content-Length: " + CORE::UInt32ToString( returnData.content.GetDataSize() ) + "\r\n";
        response += "Content-Type: " + returnData.contentType + "\r\n";
    }

    // You cannot send back accept types to the client using a standard HTTP header since
    // it is a HTTP request only header. We don't want the client to aimlessly retry different
    // representations either on PUT/POST so we do want to inform the client of the accepted types.
    // The client will need to have support for this operation to take advantage of it since it is a custom HTTP header
    if ( returnData.statusCode == 415 )
    {
        response += "Accept: ";
        TStringVector::const_iterator i = returnData.acceptedTypes.begin();
        while ( i != returnData.acceptedTypes.end() )
        {
            response += (*i) + ',';
            ++i;
        }
        response += "\r\n";
    }

    // If the operation was and a subset of allowed operations where specified
    // we will send those to the client.
    if ( returnData.statusCode == 405 && returnData.allowedMethods.size() > 0 )
    {
        response += "Allow: ";
        TStringVector::const_iterator i = returnData.allowedMethods.begin();
        while ( i != returnData.allowedMethods.end() )
        {
            response += (*i) + ',';
            ++i;
        }
        response += "\r\n";
    }

    // Add the end of HTTP header delimiter
    response += "\r\n";

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPServer(" + CORE::PointerToString( this ) + "): Finished building response header: " + response );

    // Copy the HTTP header into the buffer
    outputBuffer.CopyFrom( response.Length(), response.C_String() );

    // Copy the HTTP message body into the buffer
    if ( returnData.content.GetDataSize() > 0 )
    {
        outputBuffer.CopyFrom( response.Length(), returnData.content.GetDataSize(), returnData.content.GetConstBufferPtr() );
    }
}

/*-------------------------------------------------------------------------*/

CIHTTPServerRouterController* 
CHTTPServer::GetRouterController( void ) const
{GUCEF_TRACE;

    return m_routerController;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
