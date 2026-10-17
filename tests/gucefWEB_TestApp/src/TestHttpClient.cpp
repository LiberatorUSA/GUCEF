/*
 *  gucefWEB_TestApp: GUCEF test application for the WEB module
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

#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <thread>

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H
#include "gucefCORE_CUriResourceAccessorFactory.h"
#define GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H
#endif /* GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CRESOURCEMETADATA_H
#include "gucefCORE_CResourceMetaData.h"
#define GUCEF_CORE_CRESOURCEMETADATA_H
#endif /* GUCEF_CORE_CRESOURCEMETADATA_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_WEB_CWEBGLOBAL_H
#include "gucefWEB_CWebGlobal.h"
#define GUCEF_WEB_CWEBGLOBAL_H
#endif /* GUCEF_WEB_CWEBGLOBAL_H ? */

#ifndef GUCEF_CORE_CTHREADPOOL_H
#include "gucefCORE_CThreadPool.h"
#define GUCEF_CORE_CTHREADPOOL_H
#endif /* GUCEF_CORE_CTHREADPOOL_H ? */

#ifndef GUCEF_COMCORE_H
#include "gucefCOMCORE.h"
#define GUCEF_COMCORE_H
#endif /* GUCEF_COMCORE_H ? */

#ifndef GUCEF_WEB_H
#include "gucefWEB.h"
#define GUCEF_WEB_H
#endif /* GUCEF_WEB_H ? */

#ifndef GUCEF_WEB_CHTTPCLIENT_H
#include "gucefWEB_CHTTPClient.h"
#define GUCEF_WEB_CHTTPCLIENT_H
#endif /* GUCEF_WEB_CHTTPCLIENT_H ? */

#ifndef GUCEF_WEB_CHTTPCLIENTBACKENDSELECTION_H
#include "gucefWEB_CHttpClientBackendSelection.h"
#define GUCEF_WEB_CHTTPCLIENTBACKENDSELECTION_H
#endif /* GUCEF_WEB_CHTTPCLIENTBACKENDSELECTION_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestHttpClient.h"

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

using namespace GUCEF;

static const UInt16 SCRIPTED_SERVER_PORT = 47600;
static const UInt16 NOTHING_LISTENS_PORT = 47699;

/*-------------------------------------------------------------------------*/

static CORE::CString
ToCString( const CORE::CDynamicBuffer& buffer )
{
    return CORE::CString( buffer.AsConstTypePtr< char >(), buffer.GetDataSize() );
}

/*-------------------------------------------------------------------------*/

/** compares text and logs the actual text when it differs, so a failure tells what was received */
static bool
SameText( const CORE::CString& actual, const CORE::CString& expected )
{
    if ( actual == expected )
        return true;
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "HttpClient test: expected \"" + expected + "\" but got \"" + actual + "\"" );
    return false;
}

/*-------------------------------------------------------------------------*/

/**
 *  A HTTP server scripted for testing clients: it answers with exact bytes, sent in pieces on separate
 *  network writes so that headers and bodies arrive split over reads, and it runs on its own thread so that
 *  a client can block the test thread with Execute()
 */
class CScriptedHttpServer : public CORE::CObserver
{
    public:

    CScriptedHttpServer( void )
        : CORE::CObserver()
        , m_pulseGenerator( CORE::CPulseGenerator::CreateSharedObj() )
        , m_socket( m_pulseGenerator, false )
        , m_running( false )
        , m_thread()
        , m_connections()
        , m_lock()
        , m_requestCount( 0 )
        , m_lastCookieHeader()
    {
        SubscribeTo( &m_socket );
    }

    virtual ~CScriptedHttpServer()
    {
        Stop();
    }

    bool Start( UInt16 port )
    {
        if ( !m_socket.ListenOnPort( port ) )
            return false;
        m_running = true;
        m_thread = std::thread( [this]() { Run(); } );
        return true;
    }

    void Stop( void )
    {
        m_running = false;
        if ( m_thread.joinable() )
            m_thread.join();
        m_socket.Close();
    }

    UInt32 GetRequestCount( void ) { MT::CScopeMutex lock( m_lock ); return m_requestCount; }
    CORE::CString GetLastCookieHeader( void ) { MT::CScopeMutex lock( m_lock ); return m_lastCookieHeader.c_str(); }

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static const CORE::CString name = "CScriptedHttpServer";
        return name;
    }

    protected:

    virtual void OnNotify( CORE::CNotifier* notifier, const CORE::CEvent& eventId, CORE::CICloneable* eventData ) GUCEF_VIRTUAL_OVERRIDE
    {
        if ( notifier == &m_socket )
        {
            if ( COMCORE::CTCPServerSocket::ClientConnectedEvent == eventId )
            {
                COMCORE::CTCPServerSocket::TClientConnectedEventData* data = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventData );
                COMCORE::CTCPServerConnection* connection = data->GetData().connection;
                SubscribeTo( connection );
                SConnection& state = m_connections[ connection ];
                state = SConnection();
                state.connection = connection;
            }
            else
            if ( COMCORE::CTCPServerSocket::ClientDisconnectedEvent == eventId )
            {
                COMCORE::CTCPServerSocket::TClientDisconnectedEventData* data = static_cast< COMCORE::CTCPServerSocket::TClientDisconnectedEventData* >( eventData );
                m_connections.erase( data->GetData().connectionInfo.connection );
            }
            return;
        }

        if ( COMCORE::CTCPConnection::DataRecievedEvent == eventId )
        {
            std::map< void*, SConnection >::iterator i = m_connections.find( notifier );
            if ( i == m_connections.end() )
                return;
            COMCORE::CTCPConnection::TDataRecievedEventData* data = static_cast< COMCORE::CTCPConnection::TDataRecievedEventData* >( eventData );
            (*i).second.request.append( data->GetData().AsConstTypePtr< char >(), data->GetData().GetDataSize() );
            TryAnswer( (*i).second );
        }
    }

    private:

    struct SConnection
    {
        COMCORE::CTCPServerConnection* connection;
        std::string request;
        std::vector< std::string > pieces;
        size_t nextPiece;
        bool answered;
        bool closeAfterAnswer;

        SConnection( void ) : connection( GUCEF_NULL ), request(), pieces(), nextPiece( 0 ), answered( false ), closeAfterAnswer( true ) {}
    };

    void Run( void )
    {
        while ( m_running )
        {
            m_pulseGenerator->ForceDirectPulse();
            SendPendingPieces();
            MT::ThreadDelay( 2 );
        }
    }

    /** one piece per connection per round, so the client receives them in separate reads */
    void SendPendingPieces( void )
    {
        std::vector< COMCORE::CTCPServerConnection* > toClose;
        for ( std::map< void*, SConnection >::iterator i = m_connections.begin(); i != m_connections.end(); ++i )
        {
            SConnection& state = (*i).second;
            if ( !state.answered || state.nextPiece >= state.pieces.size() )
                continue;
            const std::string& piece = state.pieces[ state.nextPiece++ ];
            if ( !piece.empty() )
                state.connection->Send( piece.data(), (UInt32) piece.size() );
            if ( state.nextPiece >= state.pieces.size() && state.closeAfterAnswer )
                toClose.push_back( state.connection );
        }
        for ( size_t c=0; c<toClose.size(); ++c )
            toClose[ c ]->Close();
    }

    static std::string HeaderValue( const std::string& headers, const std::string& name )
    {
        std::string lowerHeaders = headers, lowerName = "\n" + name + ":";
        for ( size_t i=0; i<lowerHeaders.size(); ++i ) lowerHeaders[ i ] = (char) tolower( lowerHeaders[ i ] );
        for ( size_t i=0; i<lowerName.size(); ++i ) lowerName[ i ] = (char) tolower( lowerName[ i ] );
        size_t pos = lowerHeaders.find( lowerName );
        if ( std::string::npos == pos )
            return std::string();
        size_t start = headers.find_first_not_of( " \t", pos + lowerName.size() );
        size_t end = headers.find( "\r\n", start );
        return headers.substr( start, end - start );
    }

    /** decodes a chunked request body, returns false while it is incomplete */
    static bool DecodeChunked( const std::string& raw, std::string& body )
    {
        size_t pos = 0;
        body.clear();
        while ( true )
        {
            size_t lineEnd = raw.find( "\r\n", pos );
            if ( std::string::npos == lineEnd )
                return false;
            unsigned long size = strtoul( raw.substr( pos, lineEnd - pos ).c_str(), GUCEF_NULL, 16 );
            pos = lineEnd + 2;
            if ( 0 == size )
                return raw.size() >= pos + 2;
            if ( raw.size() < pos + size + 2 )
                return false;
            body.append( raw, pos, size );
            pos += size + 2;
        }
    }

    void TryAnswer( SConnection& state )
    {
        if ( state.answered )
            return;
        size_t headerEnd = state.request.find( "\r\n\r\n" );
        if ( std::string::npos == headerEnd )
            return;
        std::string headers = state.request.substr( 0, headerEnd + 2 );
        std::string rawBody = state.request.substr( headerEnd + 4 );
        std::string body;
        if ( HeaderValue( headers, "Transfer-Encoding" ).find( "chunked" ) != std::string::npos )
        {
            if ( !DecodeChunked( rawBody, body ) )
                return;
        }
        else
        {
            size_t contentLength = (size_t) strtoul( HeaderValue( headers, "Content-Length" ).c_str(), GUCEF_NULL, 10 );
            if ( rawBody.size() < contentLength )
                return;
            body = rawBody.substr( 0, contentLength );
        }

        std::string method = headers.substr( 0, headers.find( ' ' ) );
        size_t pathStart = headers.find( ' ' ) + 1;
        std::string path = headers.substr( pathStart, headers.find( ' ', pathStart ) - pathStart );
        {
            MT::CScopeMutex lock( m_lock );
            ++m_requestCount;
            m_lastCookieHeader = HeaderValue( headers, "Cookie" );
        }

        state.answered = true;
        Route( state, method, path, headers, body );
    }

    static std::string Response( int status, const std::string& extraHeaders, const std::string& body, bool withLength = true )
    {
        char statusLine[ 64 ];
        sprintf( statusLine, "HTTP/1.1 %d X\r\n", status );
        std::string response = statusLine + extraHeaders;
        if ( withLength )
            response += "Content-Length: " + std::to_string( body.size() ) + "\r\n";
        return response + "\r\n" + body;
    }

    void Route( SConnection& state, const std::string& method, const std::string& path, const std::string& headers, const std::string& body )
    {
        std::vector< std::string >& p = state.pieces;
        if ( path == "/hello" )
        {
            // the header arrives in three reads, split in the middle of a line
            std::string response = Response( 200, "Content-Type: text/plain\r\nAccept-Ranges: bytes\r\n", "hello world" );
            p.push_back( response.substr( 0, 10 ) );
            p.push_back( response.substr( 10, 25 ) );
            p.push_back( response.substr( 35 ) );
        }
        else
        if ( path == "/chunked" )
        {
            // the chunks are split inside the size lines and the line ends
            p.push_back( "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n1" );
            p.push_back( "4\r\nThe quick " );
            p.push_back( "brown fox \r" );
            p.push_back( "\n1B;ext=1\r\njumps over the lazy dog!!!!\r\n0\r" );
            p.push_back( "\nTrailer: x\r\n\r\n" );
        }
        else
        if ( path == "/close" )
        {
            // no length, the body ends when the connection is closed
            p.push_back( "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\nuntil " );
            p.push_back( "the end" );
        }
        else
        if ( path == "/redirect/1" )
            p.push_back( Response( 302, "Location: /redirect/2\r\n", "go on" ) );
        else
        if ( path == "/redirect/2" )
            p.push_back( Response( 301, "Location: http://127.0.0.1:" + std::to_string( SCRIPTED_SERVER_PORT ) + "/hello\r\n", "moved" ) );
        else
        if ( path == "/loop" )
            p.push_back( Response( 302, "Location: /loop\r\n", "" ) );
        else
        if ( path == "/see-other" )
            p.push_back( Response( 303, "Location: echo\r\n", "" ) );
        else
        if ( path == "/temporary" )
            p.push_back( Response( 307, "Location: /echo\r\n", "" ) );
        else
        if ( path == "/echo" )
            p.push_back( Response( 200, "", method + "\n" + body ) );
        else
        if ( path == "/setcookie" )
            p.push_back( Response( 200, "Set-Cookie: session=abc; Path=/; HttpOnly\r\nSet-Cookie: theme=dark; Path=/app\r\n", "cookies set" ) );
        else
        if ( path == "/showcookies" || path == "/app/showcookies" )
            p.push_back( Response( 200, "", HeaderValue( headers, "Cookie" ) ) );
        else
        if ( path == "/range" )
        {
            std::string content = "0123456789abcdefghijklmnopqrstuvwxyz";
            std::string range = HeaderValue( headers, "Range" );
            unsigned long first = 0, last = 0;
            if ( 2 == sscanf( range.c_str(), "bytes=%lu-%lu", &first, &last ) )
                p.push_back( Response( 206, "", content.substr( first, last - first + 1 ) ) );
            else
            if ( 1 == sscanf( range.c_str(), "bytes=%lu-", &first ) )
                p.push_back( Response( 206, "", content.substr( first ) ) );
            else
                p.push_back( Response( 200, "", content ) );
        }
        else
        if ( path == "/large" )
        {
            std::string content( 2 * 1024 * 1024, 'x' );
            for ( size_t i=0; i<content.size(); i += 4096 )
                content[ i ] = (char) ( 'a' + ( i / 4096 ) % 26 );
            std::string response = Response( 200, "", content );
            for ( size_t i=0; i<response.size(); i += 256 * 1024 )
                p.push_back( response.substr( i, 256 * 1024 ) );
        }
        else
        if ( path == "/status/404" )
            p.push_back( Response( 404, "", "not here" ) );
        else
        if ( path == "/head" )
            p.push_back( "HTTP/1.1 200 OK\r\nContent-Length: 100\r\n\r\n" );
        else
        if ( path == "/continue" )
        {
            p.push_back( "HTTP/1.1 100 Continue\r\n\r\n" );
            p.push_back( Response( 200, "", "after continue" ) );
        }
        else
        if ( path == "/slow" )
        {
            // never answers
            state.closeAfterAnswer = false;
        }
        else
            p.push_back( Response( 404, "", "unknown" ) );
    }

    CORE::PulseGeneratorPtr m_pulseGenerator;
    COMCORE::CTCPServerSocket m_socket;
    volatile bool m_running;
    std::thread m_thread;
    std::map< void*, SConnection > m_connections;
    MT::CMutex m_lock;
    UInt32 m_requestCount;
    std::string m_lastCookieHeader;
};

/*-------------------------------------------------------------------------*/

/** a source of the given text which, like a stream, does not tell its size */
class CStreamSource : public CORE::CIOAccess
{
    public:

    CStreamSource( const std::string& text ) : m_text( text ), m_pos( 0 ) {}

    virtual void Open( void ) GUCEF_VIRTUAL_OVERRIDE { m_pos = 0; }
    virtual void Close( void ) GUCEF_VIRTUAL_OVERRIDE {}
    virtual bool Opened( void ) const GUCEF_VIRTUAL_OVERRIDE { return true; }
    virtual UInt32 Read( void* dest, UInt32 esize, UInt32 elements ) GUCEF_VIRTUAL_OVERRIDE
    {
        size_t count = std::min( (size_t) esize * elements, m_text.size() - m_pos );
        memcpy( dest, m_text.data() + m_pos, count );
        m_pos += count;
        return esize > 0 ? (UInt32) ( count / esize ) : 0;
    }
    virtual UInt64 Tell( void ) const GUCEF_VIRTUAL_OVERRIDE { return m_pos; }
    virtual Int32 Seek( Int64 offset, Int32 origin ) GUCEF_VIRTUAL_OVERRIDE { return -1; }
    virtual char GetChar( void ) GUCEF_VIRTUAL_OVERRIDE { char c = 0; Read( &c, 1, 1 ); return c; }
    virtual bool Eof( void ) const GUCEF_VIRTUAL_OVERRIDE { return m_pos >= m_text.size(); }
    virtual bool IsReadable( void ) const GUCEF_VIRTUAL_OVERRIDE { return true; }
    virtual bool IsWriteable( void ) const GUCEF_VIRTUAL_OVERRIDE { return false; }
    virtual bool IsValid( void ) GUCEF_VIRTUAL_OVERRIDE { return true; }
    virtual void Flush( void ) GUCEF_VIRTUAL_OVERRIDE {}
    virtual UInt64 GetSize( void ) const GUCEF_VIRTUAL_OVERRIDE { return 0; }
    virtual MT::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE { return GUCEF_NEW CStreamSource( m_text ); }

    private:

    std::string m_text;
    size_t m_pos;
};

/*-------------------------------------------------------------------------*/

/** records the events of the classic API */
class CClassicRecorder : public CORE::CObserver
{
    public:

    CClassicRecorder( void ) : events(), body(), contentSize( 0 ), httpCode( 0 ), errorCode( 0 ), finished( false ), disconnected( false ) {}

    virtual void OnNotify( CORE::CNotifier* notifier, const CORE::CEvent& eventId, CORE::CICloneable* eventData ) GUCEF_VIRTUAL_OVERRIDE
    {
        if ( WEB::CHTTPClient::ConnectingEvent == eventId ) events.push_back( "connecting" );
        else if ( WEB::CHTTPClient::ConnectedEvent == eventId ) events.push_back( "connected" );
        else if ( WEB::CHTTPClient::HTTPContentEvent == eventId )
        {
            events.push_back( "content" );
            WEB::CHTTPClient::THTTPContentEventData* data = static_cast< WEB::CHTTPClient::THTTPContentEventData* >( eventData );
            contentSize = data->GetData().contentSize;
            httpCode = data->GetData().HTTPcode;
        }
        else if ( WEB::CHTTPClient::HTTPDataRecievedEvent == eventId )
        {
            WEB::CHTTPClient::THTTPDataRecievedEventData* data = static_cast< WEB::CHTTPClient::THTTPDataRecievedEventData* >( eventData );
            body += ToCString( data->GetData() );
        }
        else if ( WEB::CHTTPClient::HTTPErrorEvent == eventId )
        {
            events.push_back( "error" );
            errorCode = static_cast< WEB::CHTTPClient::THTTPErrorEventData* >( eventData )->GetData();
        }
        else if ( WEB::CHTTPClient::DisconnectedEvent == eventId ) { events.push_back( "disconnected" ); disconnected = true; }
        else if ( WEB::CHTTPClient::HTTPTransferFinishedEvent == eventId ) { events.push_back( "finished" ); finished = true; }
        else if ( WEB::CHTTPClient::ConnectionErrorEvent == eventId ) { events.push_back( "connectionerror" ); disconnected = true; }
    }

    std::vector< std::string > events;
    CORE::CString body;
    UInt32 contentSize;
    int httpCode;
    int errorCode;
    bool finished;
    bool disconnected;
};

/*-------------------------------------------------------------------------*/

static WEB::CHttpRequestData
MakeRequest( const CORE::CString& path, const CORE::CString& method = "GET" )
{
    WEB::CHttpRequestData request;
    request.SetUrl( "http://127.0.0.1:" + CORE::ToString( SCRIPTED_SERVER_PORT ) + path );
    request.requestType = method;
    return request;
}

/*-------------------------------------------------------------------------*/

static void
PerformUrlAndCookieTests( void )
{
    GUCEF_TESTFW_TESTCASE( "Test: URLs are split into the request fields and put back together" )
        try
        {
            WEB::CHttpRequestData request;
            ASSERT_TRUE( request.SetUrl( "HTTPS://Example.org:8443/a/b.html?x=1&y=2#frag" ) );
            ASSERT_TRUE( request.requestProtocol == "https" );
            ASSERT_TRUE( request.requestHost == "Example.org:8443" );
            ASSERT_TRUE( request.requestUri == "/a/b.html" );
            ASSERT_TRUE( request.requestUriParams == "x=1&y=2" );
            ASSERT_TRUE( request.GetHostName() == "Example.org" && 8443 == request.GetPort() );
            ASSERT_TRUE( request.GetUrl() == "https://Example.org:8443/a/b.html?x=1&y=2" );

            ASSERT_TRUE( request.SetUrl( "http://host" ) );
            ASSERT_TRUE( request.requestUri == "/" && 80 == request.GetPort() && request.GetUrl() == "http://host/" );
            ASSERT_TRUE( request.SetUrl( "https://host?q" ) );
            ASSERT_TRUE( request.requestUri == "/" && request.requestUriParams == "q" && 443 == request.GetPort() );
            ASSERT_TRUE( request.SetUrl( "http://[::1]:8080/x" ) );
            ASSERT_TRUE( request.GetHostName() == "::1" && 8080 == request.GetPort() );
            ASSERT_FALSE( request.SetUrl( "no scheme" ) );
            ASSERT_FALSE( request.SetUrl( "http:///path" ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: The cookie jar applies the rules of RFC 6265" )
        try
        {
            WEB::CHttpCookieJar jar;
            ASSERT_TRUE( jar.SetCookieFromHeader( "a=1", "www.example.org", "/docs/page", false ) );
            ASSERT_TRUE( jar.SetCookieFromHeader( "b=2; Domain=.example.org; Path=/", "www.example.org", "/", false ) );
            ASSERT_TRUE( jar.SetCookieFromHeader( "c=3; Path=/docs/deep; Secure", "www.example.org", "/", true ) );
            ASSERT_TRUE( jar.SetCookieFromHeader( "d=4; Max-Age=0", "www.example.org", "/", false ) );     // expired at once
            ASSERT_FALSE( jar.SetCookieFromHeader( "e=5; Domain=other.org", "www.example.org", "/", false ) );
            ASSERT_FALSE( jar.SetCookieFromHeader( "f=6; Domain=org", "www.example.org", "/", false ) );
            ASSERT_FALSE( jar.SetCookieFromHeader( "g=7; Secure", "www.example.org", "/", false ) );     // secure needs https
            ASSERT_FALSE( jar.SetCookieFromHeader( "novalue", "www.example.org", "/", false ) );
            ASSERT_TRUE( 3 == jar.GetCookieCount() );

            // a (host only, default path /docs), b (domain wide), c (secure, /docs/deep)
            ASSERT_TRUE( jar.GetCookieHeaderValue( "www.example.org", "/docs/deep/x", true ) == "c=3; a=1; b=2" );
            ASSERT_TRUE( jar.GetCookieHeaderValue( "www.example.org", "/docs/deep/x", false ) == "a=1; b=2" );
            ASSERT_TRUE( jar.GetCookieHeaderValue( "sub.example.org", "/docs", false ) == "b=2" );
            ASSERT_TRUE( jar.GetCookieHeaderValue( "www.example.org", "/documents", false ) == "b=2" );
            ASSERT_TRUE( jar.GetCookieHeaderValue( "notexample.org", "/", false ).IsNULLOrEmpty() );

            // replacing and expiring
            ASSERT_TRUE( jar.SetCookieFromHeader( "b=22; Domain=example.org; Path=/", "example.org", "/", false ) );
            ASSERT_TRUE( jar.GetCookieHeaderValue( "example.org", "/", false ) == "b=22" );
            ASSERT_TRUE( jar.SetCookieFromHeader( "b=gone; Domain=example.org; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT", "example.org", "/", false ) );
            ASSERT_TRUE( jar.GetCookieHeaderValue( "example.org", "/", false ).IsNULLOrEmpty() );

            // persistent cookies survive a save and load, session cookies are kept as well
            ASSERT_TRUE( jar.SetCookieFromHeader( "p=persist; Expires=Wed, 09 Jun 2100 10:18:14 GMT; HttpOnly", "www.example.org", "/", false ) );
            CORE::CString path = CORE::CombinePath( CORE::TempDir(), "gucefWEB_TestApp_cookies.txt" );
            ASSERT_TRUE( jar.SaveToNetscapeFile( path ) );
            WEB::CHttpCookieJar loaded;
            ASSERT_TRUE( loaded.LoadFromNetscapeFile( path ) );
            ASSERT_TRUE( loaded.GetCookieCount() == jar.GetCookieCount() );
            ASSERT_TRUE( loaded.GetCookieHeaderValue( "www.example.org", "/docs/deep/x", true ) == jar.GetCookieHeaderValue( "www.example.org", "/docs/deep/x", true ) );
            WEB::CHttpCookieJar::TCookieVector cookies;
            loaded.GetAllCookies( cookies );
            bool foundPersistent = false;
            for ( size_t i=0; i<cookies.size(); ++i )
            {
                if ( cookies[ i ].name == "p" )
                {
                    foundPersistent = true;
                    ASSERT_TRUE( cookies[ i ].httpOnly && cookies[ i ].isPersistent && 2100 == cookies[ i ].expires.GetYear() );
                }
            }
            ASSERT_TRUE( foundPersistent );
            loaded.RemoveSessionCookies();
            ASSERT_TRUE( 1 == loaded.GetCookieCount() );
            CORE::DeleteFile( path );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END
}

/** loads a plugin from the module directory, the debug build of it is tried first in debug builds */
static bool
LoadHttpClientPlugin( const CORE::CString& pluginName )
{GUCEF_TRACE;

    #ifdef GUCEF_DEBUG_MODE
    CORE::CString firstPluginPath = "$MODULEDIR$/" + pluginName + "_d";
    CORE::CString secondPluginPath = "$MODULEDIR$/" + pluginName;
    #else
    CORE::CString firstPluginPath = "$MODULEDIR$/" + pluginName;
    CORE::CString secondPluginPath = "$MODULEDIR$/" + pluginName + "_d";
    #endif

    // a group of its own so that it can be unloaded without taking the plugins of other tests with it
    CORE::CPluginControl& pluginControl = CORE::CCoreGlobal::Instance()->GetPluginControl();
    if ( pluginControl.AddPluginFromDir( firstPluginPath, pluginName + "_tests", true ) )
        return true;
    return pluginControl.AddPluginFromDir( secondPluginPath, pluginName + "_tests", true );
}

/*-------------------------------------------------------------------------*/

/** runs the tests which apply to every backend, with the client using only the given backend */
static void
PerformBackendTests( WEB::CHTTPClient& client, const std::string& backendName )
{GUCEF_TRACE;

    CORE::CString::StringVector preferred;
    preferred.push_back( backendName.c_str() );
    client.SetPreferredBackends( preferred );

    GUCEF_TESTFW_TESTCASE( ( std::string( "Test (" ) + backendName + "): The backend is chosen for http" ).c_str() )
        try
        {
            ASSERT_TRUE( client.GetBackendNameForScheme( "http" ) == backendName.c_str() );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( std::string( "Test (" ) + backendName + "): Execute reads responses split over reads, chunked and close delimited" ).c_str() )
        try
        {
            WEB::CHttpResponseData response;
            WEB::CHttpClientTransferInfo info;
            ASSERT_TRUE( client.Execute( MakeRequest( "/hello" ), client.GetDefaultOptions(), response, &info ) );
            ASSERT_TRUE( 200 == response.statusCode && SameText( ToCString( response.content ), "hello world" ) );
            ASSERT_TRUE( response.contentType == "text/plain" );
            ASSERT_TRUE( info.success && info.backendName == backendName && 0 == info.redirectCount );
            ASSERT_TRUE( info.bytesReceived > 11 && info.bytesSent > 0 );

            WEB::CHttpResponseData chunked;
            ASSERT_TRUE( client.Execute( MakeRequest( "/chunked" ), chunked ) );
            ASSERT_TRUE( 200 == chunked.statusCode && SameText( ToCString( chunked.content ), "The quick brown fox jumps over the lazy dog!!!!" ) );

            WEB::CHttpResponseData closed;
            ASSERT_TRUE( client.Execute( MakeRequest( "/close" ), closed ) );
            ASSERT_TRUE( SameText( ToCString( closed.content ), "until the end" ) );

            WEB::CHttpResponseData interim;
            ASSERT_TRUE( client.Execute( MakeRequest( "/continue" ), interim ) );
            ASSERT_TRUE( 200 == interim.statusCode && SameText( ToCString( interim.content ), "after continue" ) );

            WEB::CHttpResponseData head;
            ASSERT_TRUE( client.Execute( MakeRequest( "/head", "HEAD" ), head ) );
            ASSERT_TRUE( 200 == head.statusCode && 0 == head.content.GetDataSize() );

            WEB::CHttpResponseData notFound;
            ASSERT_TRUE( client.Execute( MakeRequest( "/status/404" ), notFound ) );     // a response is a success
            ASSERT_TRUE( 404 == notFound.statusCode && SameText( ToCString( notFound.content ), "not here" ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( std::string( "Test (" ) + backendName + "): Redirects are followed and rewritten as required" ).c_str() )
        try
        {
            WEB::CHttpResponseData response;
            WEB::CHttpClientTransferInfo info;
            ASSERT_TRUE( client.Execute( MakeRequest( "/redirect/1" ), client.GetDefaultOptions(), response, &info ) );
            ASSERT_TRUE( 200 == response.statusCode && SameText( ToCString( response.content ), "hello world" ) );
            ASSERT_TRUE( 2 == info.redirectCount && info.effectiveUrl.HasSubstr( "/hello", true ) > 0 );

            // without following the redirect itself is the response, including its body
            WEB::CHttpClientRequestOptions noRedirects = client.GetDefaultOptions();
            noRedirects.followRedirects = false;
            WEB::CHttpResponseData redirect;
            ASSERT_TRUE( client.Execute( MakeRequest( "/redirect/1" ), noRedirects, redirect ) );
            ASSERT_TRUE( 302 == redirect.statusCode && redirect.location == "/redirect/2" && SameText( ToCString( redirect.content ), "go on" ) );

            // endless redirects end in an error
            WEB::CHttpClientRequestOptions fewRedirects = client.GetDefaultOptions();
            fewRedirects.maxRedirects = 3;
            ASSERT_FALSE( client.Execute( MakeRequest( "/loop" ), fewRedirects, response, &info ) );
            ASSERT_TRUE( 3 == info.redirectCount );

            // 303 continues as GET without the body, 307 repeats the POST with its body
            WEB::CHttpRequestData post = MakeRequest( "/see-other", "POST" );
            post.content.CopyFrom( 4, "data" );
            WEB::CHttpResponseData seeOther;
            ASSERT_TRUE( client.Execute( post, seeOther ) );
            ASSERT_TRUE( SameText( ToCString( seeOther.content ), "GET\n" ) );

            WEB::CHttpRequestData temporary = MakeRequest( "/temporary", "POST" );
            temporary.content.CopyFrom( 4, "data" );
            WEB::CHttpResponseData repeated;
            ASSERT_TRUE( client.Execute( temporary, repeated ) );
            ASSERT_TRUE( SameText( ToCString( repeated.content ), "POST\ndata" ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( std::string( "Test (" ) + backendName + "): Cookies from responses are sent with later requests" ).c_str() )
        try
        {
            WEB::CHttpCookieJarPtr jar( GUCEF_NEW WEB::CHttpCookieJar() );
            client.SetCookieJar( jar );

            WEB::CHttpResponseData response;
            ASSERT_TRUE( client.Execute( MakeRequest( "/setcookie" ), response ) );
            ASSERT_TRUE( 2 == jar->GetCookieCount() );

            ASSERT_TRUE( client.Execute( MakeRequest( "/showcookies" ), response ) );
            ASSERT_TRUE( SameText( ToCString( response.content ), "session=abc" ) );
            ASSERT_TRUE( client.Execute( MakeRequest( "/app/showcookies" ), response ) );
            ASSERT_TRUE( SameText( ToCString( response.content ), "theme=dark; session=abc" ) );

            // a cookie header given by the caller is combined with the jar
            WEB::CHttpRequestData withCookie = MakeRequest( "/showcookies" );
            withCookie.otherHeaders[ "Cookie" ] = "own=1";
            ASSERT_TRUE( client.Execute( withCookie, response ) );
            ASSERT_TRUE( SameText( ToCString( response.content ), "own=1; session=abc" ) );

            client.SetCookieJar( WEB::CHttpCookieJarPtr() );
            ASSERT_TRUE( client.Execute( MakeRequest( "/showcookies" ), response ) );
            ASSERT_TRUE( 0 == response.content.GetDataSize() );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( std::string( "Test (" ) + backendName + "): Ranges, streamed downloads and streamed uploads" ).c_str() )
        try
        {
            WEB::CHttpClientRequestOptions range = client.GetDefaultOptions();
            range.hasRange = true;
            range.rangeStart = 10;
            range.rangeLength = 5;
            WEB::CHttpResponseData partial;
            ASSERT_TRUE( client.Execute( MakeRequest( "/range" ), range, partial ) );
            ASSERT_TRUE( 206 == partial.statusCode && SameText( ToCString( partial.content ), "abcde" ) );
            range.rangeLength = -1;
            range.rangeStart = 30;
            ASSERT_TRUE( client.Execute( MakeRequest( "/range" ), range, partial ) );
            ASSERT_TRUE( SameText( ToCString( partial.content ), "uvwxyz" ) );

            // the body goes to the sink, not into the response
            CORE::CDynamicBuffer downloaded;
            WEB::CHttpClientRequestOptions toSink = client.GetDefaultOptions();
            toSink.responseBodySink = CORE::IOAccessPtr( GUCEF_NEW CORE::CDynamicBufferAccess( &downloaded, false ) );
            WEB::CHttpResponseData large;
            ASSERT_TRUE( client.Execute( MakeRequest( "/large" ), toSink, large ) );
            ASSERT_TRUE( 0 == large.content.GetDataSize() );
            ASSERT_TRUE( 2 * 1024 * 1024 == downloaded.GetDataSize() );
            ASSERT_TRUE( 'a' == downloaded.AsConstTypePtr< char >()[ 0 ] && 'b' == downloaded.AsConstTypePtr< char >()[ 4096 ] && 'x' == downloaded.AsConstTypePtr< char >()[ 1 ] );

            // a body of unknown size is sent chunked
            WEB::CHttpClientRequestOptions upload = client.GetDefaultOptions();
            upload.requestBody = CORE::IOAccessPtr( GUCEF_NEW CStreamSource( "streamed request body" ) );
            WEB::CHttpResponseData echo;
            ASSERT_TRUE( client.Execute( MakeRequest( "/echo", "PUT" ), upload, echo ) );
            ASSERT_TRUE( SameText( ToCString( echo.content ), "PUT\nstreamed request body" ) );

            // and one of known size with a Content-Length
            CORE::CDynamicBuffer source;
            source.CopyFrom( 10, "known size" );
            upload.requestBody = CORE::IOAccessPtr( GUCEF_NEW CORE::CDynamicBufferAccess( source ) );
            ASSERT_TRUE( client.Execute( MakeRequest( "/echo", "POST" ), upload, echo ) );
            ASSERT_TRUE( SameText( ToCString( echo.content ), "POST\nknown size" ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( std::string( "Test (" ) + backendName + "): Timeouts and connection failures are reported" ).c_str() )
        try
        {
            WEB::CHttpClientRequestOptions quick = client.GetDefaultOptions();
            quick.totalTimeoutInMs = 300;
            WEB::CHttpResponseData response;
            WEB::CHttpClientTransferInfo info;
            ASSERT_FALSE( client.Execute( MakeRequest( "/slow" ), quick, response, &info ) );
            ASSERT_TRUE( info.timedOut && !info.success && info.durationInMs >= 300 && info.durationInMs < 5000 );

            WEB::CHttpRequestData nowhere;
            nowhere.SetUrl( "http://127.0.0.1:" + CORE::ToString( NOTHING_LISTENS_PORT ) + "/" );
            ASSERT_FALSE( client.Execute( nowhere, client.GetDefaultOptions(), response, &info ) );
            ASSERT_TRUE( !info.success && !info.errorDescription.IsNULLOrEmpty() );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( std::string( "Test (" ) + backendName + "): ExecuteAsync carries out requests on a thread pool and returns futures" ).c_str() )
        try
        {
            client.SetThreadPoolName( "HttpClientTestPool" );

            // several at the same time
            std::vector< CORE::CFutureResult > futures;
            for ( int i=0; i<5; ++i )
                futures.push_back( client.ExecuteAsync( MakeRequest( i % 2 ? "/hello" : "/chunked" ) ) );
            for ( size_t i=0; i<futures.size(); ++i )
            {
                CORE::CTaskPtr task = futures[ i ].GetResult( 10000 );
                WEB::CHttpClientRequestTaskData* data = WEB::CHTTPClient::GetRequestTaskData( task );
                ASSERT_TRUE( GUCEF_NULL != data );
                if ( GUCEF_NULL != data )
                {
                    ASSERT_TRUE( data->transferInfo.success && 200 == data->response.statusCode );
                    ASSERT_TRUE( SameText( ToCString( data->response.content ), ( i % 2 ? "hello world" : "The quick brown fox jumps over the lazy dog!!!!" ) ) );
                }
            }

            // cancelling the future cancels the request
            CORE::CFutureResult slow = client.ExecuteAsync( MakeRequest( "/slow" ) );
            MT::ThreadDelay( 200 );
            ASSERT_TRUE( slow.RequestCancellation() );
            CORE::CTaskPtr task = slow.GetResult( 10000 );
            WEB::CHttpClientRequestTaskData* data = WEB::CHTTPClient::GetRequestTaskData( task );
            ASSERT_TRUE( GUCEF_NULL != data && data->transferInfo.wasCancelled && !data->transferInfo.success );

            client.SetThreadPool( CORE::ThreadPoolPtr() );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( std::string( "Test (" ) + backendName + "): The classic API raises its events as before" ).c_str() )
        try
        {
            CClassicRecorder recorder;
            recorder.SubscribeTo( &client );
            ASSERT_TRUE( client.Get( "http://127.0.0.1:" + CORE::ToString( SCRIPTED_SERVER_PORT ) + "/hello" ) );
            for ( int i=0; i<5000 && !recorder.finished; ++i )
            {
                CORE::CCoreGlobal::Instance()->GetPulseGenerator()->ForceDirectPulse();
                MT::ThreadDelay( 1 );
            }
            ASSERT_TRUE( recorder.finished && recorder.disconnected );
            ASSERT_TRUE( SameText( recorder.body, "hello world" ) );
            ASSERT_TRUE( 11 == recorder.contentSize && 200 == recorder.httpCode );
            ASSERT_TRUE( recorder.events.size() >= 5 );
            if ( recorder.events.size() >= 5 )
            {
                ASSERT_TRUE( recorder.events[ 0 ] == "connecting" && recorder.events[ 1 ] == "connected" && recorder.events[ 2 ] == "content" );
                ASSERT_TRUE( recorder.events[ recorder.events.size()-2 ] == "disconnected" && recorder.events.back() == "finished" );
            }

            // an error status is reported as such
            CClassicRecorder errorRecorder;
            errorRecorder.SubscribeTo( &client );
            recorder.UnsubscribeAllFromObserver();
            ASSERT_TRUE( client.Get( "127.0.0.1", SCRIPTED_SERVER_PORT, "/status/404" ) );
            for ( int i=0; i<5000 && !errorRecorder.disconnected; ++i )
            {
                CORE::CCoreGlobal::Instance()->GetPulseGenerator()->ForceDirectPulse();
                MT::ThreadDelay( 1 );
            }
            ASSERT_TRUE( 404 == errorRecorder.errorCode && !errorRecorder.finished );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

}

/*-------------------------------------------------------------------------*/

/** the http and https schemes are served by the URI resource accessors of the WEB module */
static void
PerformUriAccessorTests( void )
{GUCEF_TRACE;

    GUCEF_TESTFW_TESTCASE( "Test: The http URI resource accessor reads, describes, writes and deletes resources" )
        try
        {
            CORE::CUriResourceAccessorFactory& factory = CORE::CCoreGlobal::Instance()->GetUriResourceAccessorFactory();
            ASSERT_TRUE( factory.IsConstructible( "http" ) && factory.IsConstructible( "https" ) );

            CORE::CUriResourceAccessorPtr accessor = factory.Create( "http" );
            ASSERT_FALSE( accessor.IsNULL() );
            if ( accessor.IsNULL() )
                return;

            CORE::CString base = "http://127.0.0.1:" + CORE::ToString( SCRIPTED_SERVER_PORT );

            // whole resources
            CORE::CDynamicBuffer content;
            ASSERT_TRUE( accessor->GetResource( CORE::CUri( base + "/hello" ), content ) );
            ASSERT_TRUE( SameText( ToCString( content ), "hello world" ) );
            CORE::CDynamicBuffer missing;
            ASSERT_FALSE( accessor->GetResource( CORE::CUri( base + "/status/404" ), missing ) );

            // parts of resources
            CORE::CDynamicBuffer part;
            ASSERT_TRUE( accessor->GetPartialResource( CORE::CUri( base + "/range" ), 10, 5, part ) );
            ASSERT_TRUE( SameText( ToCString( part ), "abcde" ) );
            part.Clear();
            ASSERT_TRUE( accessor->GetPartialResource( CORE::CUri( base + "/range" ), 30, -1, part ) );
            ASSERT_TRUE( SameText( ToCString( part ), "uvwxyz" ) );

            // read access to a resource
            CORE::IOAccessPtr access;
            ASSERT_TRUE( accessor->GetResourceAccess( CORE::CUri( base + "/hello" ), access, CORE::CUriResourceAccessor::URI_RESOURCEACCESS_MODE_READ ) );
            ASSERT_FALSE( access.IsNULL() );
            if ( !access.IsNULL() )
            {
                char text[ 32 ] = { 0 };
                ASSERT_TRUE( 11 == access->Read( text, 1, 31 ) );
                ASSERT_TRUE( SameText( text, "hello world" ) );
            }
            ASSERT_FALSE( accessor->GetResourceAccess( CORE::CUri( base + "/hello" ), access, CORE::CUriResourceAccessor::URI_RESOURCEACCESS_MODE_WRITE ) );

            // meta data
            CORE::CResourceMetaData metaData;
            ASSERT_TRUE( accessor->GetResourceMetaData( CORE::CUri( base + "/hello" ), metaData ) );
            ASSERT_TRUE( metaData.hasResourceSizeInBytes && 11 == metaData.resourceSizeInBytes );
            ASSERT_TRUE( metaData.hasMimeType && metaData.mimeType == "text/plain" );
            ASSERT_TRUE( metaData.hasName && metaData.name == "hello" );

            // writing: the server answers a PUT to /echo with success and a DELETE as well
            CORE::CDynamicBuffer upload;
            upload.CopyFrom( 4, "data" );
            ASSERT_TRUE( accessor->UpdateResource( CORE::CUri( base + "/echo" ), upload ) );
            ASSERT_TRUE( accessor->CreateResource( CORE::CUri( base + "/echo" ), upload ) );
            ASSERT_TRUE( accessor->DeleteResource( CORE::CUri( base + "/echo" ) ) );
            ASSERT_FALSE( accessor->DeleteResource( CORE::CUri( base + "/status/404" ) ) );

            // what HTTP cannot do
            ASSERT_FALSE( accessor->UpdatePartialResource( CORE::CUri( base + "/echo" ), upload ) );
            ASSERT_FALSE( accessor->IsACollectionResource( CORE::CUri( base + "/" ) ) );
            CORE::CUriResourceAccessor::CUriResourceAccessorOperations ops;
            ASSERT_TRUE( accessor->GetSupportedOperations( CORE::CUri( base + "/hello" ), ops ) );
            ASSERT_TRUE( ops.getResource && ops.getPartialResource && ops.getResourceMetaData && ops.deleteResource && !ops.collectionResolution );

            // other schemes are refused
            ASSERT_FALSE( accessor->GetResource( CORE::CUri( "file:///etc/hostname" ), missing ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/

void
PerformHttpClientTests( void )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "COMMENCING HTTP client TESTS" );

    GUCEF_TESTFW_SUITE_SCOPE( "HttpClient" );

    PerformUrlAndCookieTests();

    CScriptedHttpServer server;
    bool serverStarted = server.Start( SCRIPTED_SERVER_PORT );

    WEB::CHTTPClient client;

    GUCEF_TESTFW_TESTCASE( "Test: Backends are selected by scheme" )
        try
        {
            ASSERT_TRUE( serverStarted );
            #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
            // WinHTTP is part of the operating system and comes before the native backend in the default order
            ASSERT_TRUE( client.GetBackendNameForScheme( "http" ) == "winhttp" );
            ASSERT_TRUE( client.GetBackendNameForScheme( "https" ) == "winhttp" );
            #else
            ASSERT_TRUE( client.GetBackendNameForScheme( "http" ) == "native" );
            #endif

            // https needs a backend from a plugin, without one the request fails with an explanation
            if ( client.GetBackendNameForScheme( "https" ).IsNULLOrEmpty() )
            {
                WEB::CHttpRequestData request;
                request.SetUrl( "https://127.0.0.1:1/" );
                WEB::CHttpResponseData response;
                WEB::CHttpClientTransferInfo info;
                ASSERT_FALSE( client.Execute( request, client.GetDefaultOptions(), response, &info ) );
                ASSERT_TRUE( info.errorDescription.HasSubstr( "https", true ) >= 0 );
            }
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    PerformBackendTests( client, "native" );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    // WinHTTP is an operating system facility, no plugin needed. Windows build unverified
    if ( WEB::CWebGlobal::Instance()->GetHttpClientBackendFactory().IsConstructible( "winhttp" ) )
        PerformBackendTests( client, "winhttp" );
    #endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */

    PerformUriAccessorTests();

    // The curl backend comes from a plugin, when it is available the same tests run with it
    client.SetPreferredBackends( WEB::CHttpClientBackendSelection::GetDefaultPreferredBackends() );
    bool curlPluginLoaded = LoadHttpClientPlugin( "httpclientpluginCURL" ) && WEB::CWebGlobal::Instance()->GetHttpClientBackendFactory().IsConstructible( "curl" );

    GUCEF_TESTFW_TESTCASE( "Test: The curl backend plugin provides https and is preferred by default" )
        try
        {
            ASSERT_TRUE( curlPluginLoaded );
            ASSERT_TRUE( client.GetBackendNameForScheme( "http" ) == "curl" );
            ASSERT_TRUE( client.GetBackendNameForScheme( "https" ) == "curl" );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    if ( curlPluginLoaded )
    {
        PerformBackendTests( client, "curl" );

        GUCEF_TESTFW_TESTCASE( "Test (curl): A public https site can be fetched, when online" )
            try
            {
                WEB::CHttpClientRequestOptions options = client.GetDefaultOptions();
                options.connectTimeoutInMs = 5000;
                options.totalTimeoutInMs = 15000;
                WEB::CHttpResponseData response;
                WEB::CHttpClientTransferInfo info;
                WEB::CHttpRequestData request;
                request.SetUrl( "https://example.com/" );
                if ( !client.Execute( request, options, response, &info ) )
                {
                    // Being offline is not a failure of the client
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "HttpClient test: skipping public https check, request failed: " + info.errorDescription );
                }
                else
                {
                    ASSERT_TRUE( 200 == response.statusCode );
                    ASSERT_TRUE( info.backendName == "curl" );
                    ASSERT_TRUE( response.content.GetDataSize() > 0 );
                }

                // A certificate which does not match the host has to be refused
                request.SetUrl( "https://wrong.host.badssl.com/" );
                if ( client.Execute( request, options, response, &info ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "HttpClient test: a request with an invalid certificate was accepted" );
                    ASSERT_TRUE( false );
                }
            }
            catch ( ... )
            {
                ERRORHERE;
            }
        GUCEF_TESTFW_TESTCASE_END

        // The backend object of the plugin has to be gone before its code is unloaded
        CORE::CString::StringVector nativeOnly;
        nativeOnly.push_back( "native" );
        client.SetPreferredBackends( nativeOnly );
        CORE::CCoreGlobal::Instance()->GetPluginControl().UnloadPluginGroup( "httpclientpluginCURL_tests" );
    }

    server.Stop();

    CORE::CLogStreamScope::FlushLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ALL HTTP client TESTS COMPLETED" );
}

/*-------------------------------------------------------------------------*/
