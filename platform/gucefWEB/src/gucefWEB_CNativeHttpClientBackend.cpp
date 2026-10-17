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

#include <string>
#include <vector>
#include <stdio.h>

#ifndef GUCEF_CORE_COBSERVER_H
#include "CObserver.h"
#define GUCEF_CORE_COBSERVER_H
#endif /* GUCEF_CORE_COBSERVER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#endif /* GUCEF_COMCORE_CTCPCLIENTSOCKET_H ? */

#ifndef GUCEF_WEB_CHTTPCLIENTBACKENDREQUESTBASE_H
#include "gucefWEB_CHttpClientBackendRequestBase.h"
#define GUCEF_WEB_CHTTPCLIENTBACKENDREQUESTBASE_H
#endif /* GUCEF_WEB_CHTTPCLIENTBACKENDREQUESTBASE_H ? */

#include "gucefWEB_CNativeHttpClientBackend.h"

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

const CString CNativeHttpClientBackend::BackendName = "native";

static const UInt32 UploadChunkSize = 64 * 1024;
static const UInt32 MaxHeaderSize = 256 * 1024;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  A single request carried out over a TCP connection driven by the pulses of the pulse generator it is given
 */
class CNativeHttpClientRequest : public CHttpClientBackendRequestBase ,
                                 public CORE::CObserver
{
    public:

    CNativeHttpClientRequest( const CHttpRequestData& request          ,
                              const CHttpClientRequestOptions& options ,
                              CHttpResponseData& response              ,
                              CHttpClientTransferInfo& transferInfo    ,
                              CORE::PulseGeneratorPtr pulseGenerator   ,
                              CIHttpClientBackendObserver& observer    );

    virtual ~CNativeHttpClientRequest() GUCEF_VIRTUAL_OVERRIDE;

    bool Start( void );

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual void OnNotify( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventId               ,
                           CORE::CICloneable* eventData = GUCEF_NULL ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnFinish( void ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    enum EBodyFraming
    {
        BODY_NONE,
        BODY_CONTENT_LENGTH,
        BODY_CHUNKED,
        BODY_UNTIL_CLOSE
    };

    enum EChunkState
    {
        CHUNK_SIZE_LINE,
        CHUNK_DATA,
        CHUNK_DATA_END,
        CHUNK_TRAILERS
    };

    void BuildRequestHeader( std::string& header ) const;
    void SendRequest( void );
    void SendMoreRequestBody( void );
    void ProcessReceivedData( const char* data, UInt32 dataSize );
    bool ParseResponseHeader( const std::string& headerBlock );
    void ProcessBodyData( const char* data, UInt32 dataSize );
    void ProcessChunkedData( const char* data, UInt32 dataSize );

    COMCORE::CTCPClientSocket m_socket;

    bool m_finalHeaderReceived;
    std::string m_headerBuffer;
    EBodyFraming m_framing;
    UInt64 m_bodyBytesLeft;
    EChunkState m_chunkState;
    std::string m_chunkLine;
    bool m_uploadComplete;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static std::string
ToStdString( const CString& str )
{
    return std::string( str.C_String(), str.ByteSize() > 0 ? str.ByteSize()-1 : 0 );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CNativeHttpClientRequest::CNativeHttpClientRequest( const CHttpRequestData& request          ,
                                                    const CHttpClientRequestOptions& options ,
                                                    CHttpResponseData& response              ,
                                                    CHttpClientTransferInfo& transferInfo    ,
                                                    CORE::PulseGeneratorPtr pulseGenerator   ,
                                                    CIHttpClientBackendObserver& observer    )
    : CHttpClientBackendRequestBase( request, options, response, transferInfo, observer )
    , CORE::CObserver()
    , m_socket( pulseGenerator, false )
    , m_finalHeaderReceived( false )
    , m_headerBuffer()
    , m_framing( BODY_NONE )
    , m_bodyBytesLeft( 0 )
    , m_chunkState( CHUNK_SIZE_LINE )
    , m_chunkLine()
    , m_uploadComplete( true )
{GUCEF_TRACE;

    SubscribeTo( &m_socket );
}

/*-------------------------------------------------------------------------*/

CNativeHttpClientRequest::~CNativeHttpClientRequest()
{GUCEF_TRACE;

    UnsubscribeAllFromObserver();
    m_socket.Close();
}

/*-------------------------------------------------------------------------*/

const CString&
CNativeHttpClientRequest::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::WEB::CNativeHttpClientRequest";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

bool
CNativeHttpClientRequest::Start( void )
{GUCEF_TRACE;

    bool useProxy = !m_options.proxyHost.IsNULLOrEmpty();
    CString host = useProxy ? m_options.proxyHost : m_request.GetHostName();
    UInt16 port = useProxy ? m_options.proxyPort : m_request.GetPort();

    if ( !m_socket.ConnectTo( host, port, false ) )
    {
        Finish( false, "Unable to connect to " + host + ":" + CORE::ToString( port ) );
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void
CNativeHttpClientRequest::OnFinish( void )
{GUCEF_TRACE;

    m_socket.Close();
}

/*-------------------------------------------------------------------------*/

void
CNativeHttpClientRequest::BuildRequestHeader( std::string& header ) const
{GUCEF_TRACE;

    // A proxy is given the absolute URL, a server the path
    CString target;
    if ( !m_options.proxyHost.IsNULLOrEmpty() )
    {
        target = m_request.GetUrl();
    }
    else
    {
        target = m_request.requestUri.IsNULLOrEmpty() ? CString( "/" ) : m_request.requestUri;
        if ( !m_request.requestUriParams.IsNULLOrEmpty() )
            target += "?" + m_request.requestUriParams;
    }

    header.reserve( 512 );
    header = ToStdString( GetRequestMethod() ) + " " + ToStdString( target ) + " HTTP/1.1\r\n";
    header += "Host: " + ToStdString( m_request.requestHost ) + "\r\n";
    header += "Connection: close\r\n";

    CString::StringVector fields;
    GetRequestHeaderFields( fields );
    for ( size_t i=0; i<fields.size(); ++i )
    {
        header.append( fields[ i ].C_String(), fields[ i ].ByteSize() > 0 ? fields[ i ].ByteSize()-1 : 0 );
        header += "\r\n";
    }

    if ( HasRequestBody() )
    {
        if ( IsRequestBodySizeKnown() )
            header += "Content-Length: " + ToStdString( CORE::ToString( GetRequestBodySize() ) ) + "\r\n";
        else
            header += "Transfer-Encoding: chunked\r\n";
    }
    header += "\r\n";
}

/*-------------------------------------------------------------------------*/

void
CNativeHttpClientRequest::SendRequest( void )
{GUCEF_TRACE;

    std::string header;
    BuildRequestHeader( header );

    if ( !IsRequestBodyStreamed() && GetRequestBodySize() > 0 )
    {
        // Small bodies held in memory go out together with the header
        size_t headerSize = header.size();
        header.resize( headerSize + (size_t) GetRequestBodySize() );
        ReadRequestBody( &header[ headerSize ], (UInt32) GetRequestBodySize() );
        m_socket.Send( header.data(), (UInt32) header.size() );
    }
    else
    {
        m_socket.Send( header.data(), (UInt32) header.size() );
        if ( IsRequestBodyStreamed() )
        {
            m_uploadComplete = false;
            SendMoreRequestBody();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CNativeHttpClientRequest::SendMoreRequestBody( void )
{GUCEF_TRACE;

    // Keep a limited amount queued in the socket so that bodies of any size are streamed
    std::vector< char > buffer;
    bool chunked = !IsRequestBodySizeKnown();
    while ( !m_uploadComplete && !IsFinished() && m_socket.GetBufferedDataToSendInBytes() < UploadChunkSize )
    {
        if ( buffer.empty() )
            buffer.resize( UploadChunkSize + 32 );

        // room is left in front for the chunk size line
        UInt32 bytesRead = ReadRequestBody( &buffer[ 16 ], UploadChunkSize );
        if ( chunked )
        {
            // chunk: size in hex, CRLF, data, CRLF. A zero size chunk ends the body
            char sizeLine[ 16 ];
            int sizeLineLength = sprintf( sizeLine, "%X\r\n", (unsigned int) bytesRead );
            char* chunkStart = &buffer[ 16 ] - sizeLineLength;
            memcpy( chunkStart, sizeLine, sizeLineLength );
            buffer[ 16 + bytesRead ] = '\r';
            buffer[ 16 + bytesRead + 1 ] = '\n';
            // a zero size chunk followed by the empty trailer line ("0\r\n\r\n") ends the body
            m_socket.Send( chunkStart, sizeLineLength + bytesRead + 2 );
            if ( 0 == bytesRead )
                m_uploadComplete = true;
        }
        else
        {
            if ( bytesRead > 0 )
                m_socket.Send( &buffer[ 16 ], bytesRead );
            if ( GetRequestBodyBytesRead() >= GetRequestBodySize() )
                m_uploadComplete = true;
            else
            if ( 0 == bytesRead )
            {
                Finish( false, "The request body source ended before its stated size" );
                return;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CNativeHttpClientRequest::ParseResponseHeader( const std::string& headerBlock )
{GUCEF_TRACE;

    // The status line, then the header lines. The base class takes care of the fields and their folding
    size_t lineEnd = headerBlock.find( '\n' );
    if ( !OnResponseStatusLine( headerBlock.data(), (UInt32) ( std::string::npos == lineEnd ? headerBlock.size() : lineEnd ) ) )
        return false;

    size_t pos = std::string::npos == lineEnd ? headerBlock.size() : lineEnd + 1;
    while ( pos < headerBlock.size() )
    {
        size_t end = headerBlock.find( '\n', pos );
        if ( std::string::npos == end )
            end = headerBlock.size();
        OnResponseHeaderLine( headerBlock.data() + pos, (UInt32) ( end - pos ) );
        pos = end + 1;
    }

    // How the body is delimited, RFC 9112 section 6.3
    int status = m_response.statusCode;
    if ( IsHeadRequest() || ( status >= 100 && status < 200 ) || 204 == status || 304 == status )
        m_framing = BODY_NONE;
    else
    if ( IsResponseChunked() )
        m_framing = BODY_CHUNKED;
    else
    if ( HasResponseContentLength() )
    {
        m_framing = GetResponseContentLength() > 0 ? BODY_CONTENT_LENGTH : BODY_NONE;
        m_bodyBytesLeft = GetResponseContentLength();
    }
    else
        m_framing = BODY_UNTIL_CLOSE;
    return true;
}

/*-------------------------------------------------------------------------*/

void
CNativeHttpClientRequest::ProcessChunkedData( const char* data, UInt32 dataSize )
{GUCEF_TRACE;

    UInt32 offset = 0;
    while ( offset < dataSize && !IsFinished() )
    {
        switch ( m_chunkState )
        {
            case CHUNK_SIZE_LINE:
            case CHUNK_DATA_END:
            case CHUNK_TRAILERS:
            {
                // Lines are gathered until complete since they can be split over reads
                const char* lineEnd = (const char*) memchr( data + offset, '\n', dataSize - offset );
                UInt32 take = GUCEF_NULL == lineEnd ? dataSize - offset : (UInt32) ( lineEnd - ( data + offset ) ) + 1;
                m_chunkLine.append( data + offset, take );
                offset += take;
                if ( m_chunkLine.size() > MaxHeaderSize )
                {
                    Finish( false, "Malformed chunked response body" );
                    return;
                }
                if ( GUCEF_NULL == lineEnd )
                    break;

                std::string line;
                line.swap( m_chunkLine );
                while ( !line.empty() && ( '\n' == line[ line.size()-1 ] || '\r' == line[ line.size()-1 ] ) )
                    line.erase( line.size()-1 );

                if ( CHUNK_DATA_END == m_chunkState )
                {
                    // the CRLF after the data of a chunk, anything else means the chunk size was wrong
                    if ( !line.empty() )
                    {
                        Finish( false, "Malformed chunked response body: chunk data longer than its size" );
                        return;
                    }
                    m_chunkState = CHUNK_SIZE_LINE;
                }
                else
                if ( CHUNK_TRAILERS == m_chunkState )
                {
                    // trailer fields are ignored, the empty line ends the body
                    if ( line.empty() )
                    {
                        Finish( true, CString::Empty );
                        return;
                    }
                }
                else
                {
                    // size in hex, optionally followed by chunk extensions
                    unsigned long long chunkSize = 0;
                    if ( 1 != sscanf( line.c_str(), "%llx", &chunkSize ) )
                    {
                        Finish( false, "Malformed chunked response body" );
                        return;
                    }
                    if ( 0 == chunkSize )
                        m_chunkState = CHUNK_TRAILERS;
                    else
                    {
                        m_bodyBytesLeft = (UInt64) chunkSize;
                        m_chunkState = CHUNK_DATA;
                    }
                }
                break;
            }
            case CHUNK_DATA:
            {
                UInt32 available = dataSize - offset;
                UInt32 take = m_bodyBytesLeft < available ? (UInt32) m_bodyBytesLeft : available;
                OnResponseBody( data + offset, take );
                offset += take;
                m_bodyBytesLeft -= take;
                if ( 0 == m_bodyBytesLeft )
                    m_chunkState = CHUNK_DATA_END;
                break;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CNativeHttpClientRequest::ProcessBodyData( const char* data, UInt32 dataSize )
{GUCEF_TRACE;

    switch ( m_framing )
    {
        case BODY_CONTENT_LENGTH:
        {
            UInt32 take = m_bodyBytesLeft < dataSize ? (UInt32) m_bodyBytesLeft : dataSize;
            OnResponseBody( data, take );
            m_bodyBytesLeft -= take;
            if ( 0 == m_bodyBytesLeft )
                Finish( true, CString::Empty );
            break;
        }
        case BODY_CHUNKED:
        {
            ProcessChunkedData( data, dataSize );
            break;
        }
        case BODY_UNTIL_CLOSE:
        {
            OnResponseBody( data, dataSize );
            break;
        }
        default:
        {
            break;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CNativeHttpClientRequest::ProcessReceivedData( const char* data, UInt32 dataSize )
{GUCEF_TRACE;

    OnDataReceived( dataSize );

    while ( !m_finalHeaderReceived && dataSize > 0 && !IsFinished() )
    {
        // Gather the header, it can be split over any number of reads
        size_t searchFrom = m_headerBuffer.size() > 3 ? m_headerBuffer.size() - 3 : 0;
        m_headerBuffer.append( data, dataSize );
        size_t headerEnd = m_headerBuffer.find( "\r\n\r\n", searchFrom );
        size_t terminatorSize = 4;
        if ( std::string::npos == headerEnd )
        {
            headerEnd = m_headerBuffer.find( "\n\n", searchFrom );
            terminatorSize = 2;
        }
        if ( std::string::npos == headerEnd )
        {
            if ( m_headerBuffer.size() > MaxHeaderSize )
                Finish( false, "The response header is too large" );
            return;
        }

        std::string headerBlock = m_headerBuffer.substr( 0, headerEnd );
        size_t bodyStartInBuffer = headerEnd + terminatorSize;
        size_t alreadyBuffered = m_headerBuffer.size() - dataSize;   // bytes from earlier reads
        UInt32 consumedFromThisRead = (UInt32) ( bodyStartInBuffer - alreadyBuffered );
        m_headerBuffer.clear();

        if ( !ParseResponseHeader( headerBlock ) )
        {
            Finish( false, "The server did not reply with a valid HTTP response" );
            return;
        }

        data += consumedFromThisRead;
        dataSize -= consumedFromThisRead;

        if ( m_response.statusCode >= 100 && m_response.statusCode < 200 )
        {
            // An interim response such as 100 Continue, the actual response follows
            continue;
        }

        m_finalHeaderReceived = true;
        OnResponseHeadersComplete();
        if ( BODY_NONE == m_framing )
        {
            Finish( true, CString::Empty );
            return;
        }
    }

    if ( m_finalHeaderReceived && dataSize > 0 && !IsFinished() )
        ProcessBodyData( data, dataSize );
}

/*-------------------------------------------------------------------------*/

void
CNativeHttpClientRequest::OnNotify( CORE::CNotifier* notifier                 ,
                                    const CORE::CEvent& eventId               ,
                                    CORE::CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;

    if ( notifier != &m_socket || IsFinished() )
        return;

    if ( COMCORE::CTCPClientSocket::ConnectedEvent == eventId )
    {
        OnConnected();
        SendRequest();
    }
    else
    if ( COMCORE::CTCPClientSocket::DataRecievedEvent == eventId )
    {
        COMCORE::CTCPClientSocket::TDataRecievedEventData* receivedData = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventData );
        if ( GUCEF_NULL != receivedData )
        {
            const CORE::CDynamicBuffer& buffer = receivedData->GetData();
            ProcessReceivedData( buffer.AsConstTypePtr< char >(), buffer.GetDataSize() );
        }
    }
    else
    if ( COMCORE::CTCPClientSocket::DataSentEvent == eventId )
    {
        COMCORE::CTCPClientSocket::TDataSentEventData* sentData = static_cast< COMCORE::CTCPClientSocket::TDataSentEventData* >( eventData );
        if ( GUCEF_NULL != sentData )
            OnDataSent( sentData->GetData().GetDataSize() );
        if ( !m_uploadComplete && !IsFinished() )
            SendMoreRequestBody();
    }
    else
    if ( COMCORE::CTCPClientSocket::DisconnectedEvent == eventId )
    {
        if ( m_finalHeaderReceived && BODY_UNTIL_CLOSE == m_framing )
            Finish( true, CString::Empty );
        else
            Finish( false, "The connection was closed before the response was complete" );
    }
    else
    if ( COMCORE::CTCPClientSocket::SocketErrorEvent == eventId )
    {
        COMCORE::CTCPClientSocket::TSocketErrorEventData* errorData = static_cast< COMCORE::CTCPClientSocket::TSocketErrorEventData* >( eventData );
        Int32 errorCode = GUCEF_NULL != errorData ? errorData->GetData() : -1;
        Finish( false, "Socket error " + CORE::ToString( errorCode ), errorCode );
    }
}

/*-------------------------------------------------------------------------*/

CNativeHttpClientBackend::CNativeHttpClientBackend( void )
    : CIHttpClientBackend()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CNativeHttpClientBackend::~CNativeHttpClientBackend()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CString
CNativeHttpClientBackend::GetBackendName( void ) const
{GUCEF_TRACE;

    return BackendName;
}

/*-------------------------------------------------------------------------*/

bool
CNativeHttpClientBackend::SupportsScheme( const CString& scheme ) const
{GUCEF_TRACE;

    return scheme.Equals( "http", false );
}

/*-------------------------------------------------------------------------*/

CIHttpClientBackendRequestPtr
CNativeHttpClientBackend::StartRequest( const CHttpRequestData& request          ,
                                        const CHttpClientRequestOptions& options ,
                                        CHttpResponseData& response              ,
                                        CHttpClientTransferInfo& transferInfo    ,
                                        CORE::PulseGeneratorPtr pulseGenerator   ,
                                        CIHttpClientBackendObserver& observer    )
{GUCEF_TRACE;

    if ( !SupportsScheme( request.requestProtocol ) )
    {
        transferInfo.success = false;
        transferInfo.errorCode = -1;
        transferInfo.errorDescription = "The native HTTP client backend does not support \"" + request.requestProtocol + "\"";
        return CIHttpClientBackendRequestPtr();
    }

    CNativeHttpClientRequest* nativeRequest = GUCEF_NEW CNativeHttpClientRequest( request, options, response, transferInfo, pulseGenerator, observer );
    CIHttpClientBackendRequestPtr requestPtr( nativeRequest );
    if ( !nativeRequest->Start() )
        return CIHttpClientBackendRequestPtr();
    return requestPtr;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
