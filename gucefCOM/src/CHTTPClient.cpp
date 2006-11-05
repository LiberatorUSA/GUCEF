/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#include "CURLHandlerRegistry.h"      /* gucefCORE registry for URL handlers */
#define GUCEF_CORE_CURLHANDLERREGISTRY_H
#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */

#include "CHTTPClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CHTTPClient::ConnectingEvent = "GUCEF::COM::CHTTPClient::ConnectingEvent";
const CORE::CString CHTTPClient::ConnectedEvent = "GUCEF::COM::CHTTPClient::ConnectedEvent";
const CORE::CString CHTTPClient::DisconnectedEvent = "GUCEF::COM::CHTTPClient::DisconnectedEvent";
const CORE::CString CHTTPClient::ConnectionErrorEvent = "GUCEF::COM::CHTTPClient::ConnectionErrorEvent";        
const CORE::CString CHTTPClient::HTTPErrorEvent = "GUCEF::COM::CHTTPClient::HTTPErrorEvent";
const CORE::CString CHTTPClient::HTTPRedirectEvent = "GUCEF::COM::CHTTPClient::HTTPRedirectEvent";
const CORE::CString CHTTPClient::HTTPContentEvent = "GUCEF::COM::CHTTPClient::HTTPContentEvent";                
const CORE::CString CHTTPClient::HTTPDataRecievedEvent = "GUCEF::COM::CHTTPClient::HTTPDataRecievedEvent";
const CORE::CString CHTTPClient::HTTPDataSendEvent = "GUCEF::COM::CHTTPClient::HTTPDataSendEvent";        
const CORE::CString CHTTPClient::HTTPTransferFinishedEvent = "GUCEF::COM::CHTTPClient::HTTPTransferFinishedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPClient::CHTTPClient( void )
        : m_socket( false )                                          ,
          m_downloading( false )                                     ,
          m_recieved( 0 )                                            ,
          m_filesize( 0 )                                            ,
          m_connectingEventID( ConnectingEvent )                     ,
          m_connectedEventID( ConnectedEvent )                       ,
          m_disconnectedEventID( DisconnectedEvent )                 ,
          m_connectionErrorEventID( ConnectionErrorEvent )           ,
          m_HTTPErrorEventID( HTTPErrorEvent )                       ,
          m_HTTPRedirectEventID( HTTPRedirectEvent )                 ,
          m_HTTPContentEventID( HTTPContentEvent )                   ,
          m_HTTPDataRecievedEventID( HTTPDataRecievedEvent )         ,
          m_HTTPDataSendEventID( HTTPDataSendEvent )                 ,
          m_HTTPTransferFinishedEventID( HTTPTransferFinishedEvent )
{TRACE;

        SubscribeTo( &m_socket );
}

/*-------------------------------------------------------------------------*/
        
CHTTPClient::~CHTTPClient()
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::Close( void )
{TRACE;
        m_socket.Close();
}
        
/*-------------------------------------------------------------------------*/

bool
CHTTPClient::Post( const CORE::CString& host                      ,
                   UInt16 port                                    ,
                   const CORE::CString& path                      , 
                   const CORE::CValueList* valuelist /* = NULL */ )
{TRACE;
        m_socket.Close();
        
        m_recieved = 0;
        m_filesize = 0;
        UInt32 contentsize( 0 );
        if ( valuelist )
        {
                for ( UInt32 i=0; i<valuelist->GetCount(); ++i )
                {
                        contentsize += valuelist->GetPair( i ).Length()+1;
                }
        }                
                
        UInt32 mainmsglength = 97 + host.Length() + path.Length(); 
        char* sendbuffer = new char[ mainmsglength + contentsize ];
        sprintf( sendbuffer, "POST %s HTTP/1.1\r\nAccept: */*\r\nUser-Agent: gucefCOM-HTTP/1.0\r\n\r\nHost: %s\r\nContent-Length: %d\r\n\r\n", path.ReplaceChar( '\\', '/' ).C_String(), host.C_String(), contentsize );
        
        UInt32 offset( mainmsglength );
        if ( valuelist )
        {
                CORE::CString valueitem;
                for ( UInt32 i=0; i<valuelist->GetCount(); ++i )
                {
                        valueitem = valuelist->GetPair( i );
                        memcpy( sendbuffer+offset, valueitem.C_String(), valueitem.Length() );
                        offset += valueitem.Length();
                        if ( i+1 < valuelist->GetCount() )
                        {
                                sendbuffer[ offset ] = '&';
                                ++offset;
                        }                                        
                }
        }                
        
        if ( m_socket.ConnectTo( host ,
                                 port ) )
        {                                         
                bool success = m_socket.Send( sendbuffer, mainmsglength + contentsize );
                
                delete []sendbuffer;
                
                return success;
        }
        return false;                
}

/*-------------------------------------------------------------------------*/

bool 
CHTTPClient::Post( const CORE::CString& urlstring                 ,
                   const CORE::CValueList* valuelist /* = NULL */ )
{TRACE;
        CORE::CString host;
        UInt16 port;
        CORE::CString path;

        if ( ParseURL( urlstring ,
                       host      ,
                       port      ,
                       path      ) )
        {      
                return Post( host       ,
                             port       ,
                             path       ,
                             valuelist  );        
        }                       
        return false;
}                

/*-------------------------------------------------------------------------*/
                   
bool 
CHTTPClient::Get( const CORE::CString& host                      ,
                  UInt16 port                                    ,
                  const CORE::CString& path                      ,
                  const UInt32 byteoffset /* = 0 */              ,
                  const CORE::CValueList* valuelist /* = NULL */ )
{TRACE;
        m_socket.Close();
        
        m_recieved = 0;
        m_filesize = 0;
        UInt32 contentsize( 0 );
        CORE::CString valuepath( path );
        
        if ( valuelist )
        {
                for ( UInt32 i=0; i<valuelist->GetCount(); ++i )
                {
                        contentsize += valuelist->GetPair( i ).Length()+1;
                }
                        
                if ( valuelist->GetCount() )
                {
                        valuepath += '?';
                        for ( UInt32 i=0; i<valuelist->GetCount(); ++i )
                        {
                                valuepath += valuelist->GetPair( i );
                                if ( i+1 < valuelist->GetCount() )
                                {
                                        valuepath += '&';
                                }
                        }
                }                
        }
        
        valuepath = valuepath.ReplaceChar( '\\', '/' );
        
        if ( valuepath.Length() )
        {
                if ( valuepath[ 0 ] != '/' )
                {
                        CORE::CString tmp( "/" );
                        valuepath = tmp + valuepath;
                }
        }
        
        char* buffer( NULL );
        if ( byteoffset == 0 )
        {
                buffer = new char[ 100 + valuepath.Length() + host.Length() ];
                sprintf( buffer, "GET %s HTTP/1.1\r\nHost: %s\r\nAccept: */*\r\nUser-Agent: gucefCOM-HTTP/1.0 (Windows;)\r\n\r\n", valuepath.C_String(), host.C_String() );
        }
        else
        {
                CORE::CString intstr;
                intstr.SetInt( byteoffset );                
                
                buffer = new char[ 100 + valuepath.Length() + host.Length() + intstr.Length() ];
                sprintf( buffer, "GET %s HTTP/1.1\r\nHost: %s\r\nRange: bytes=%d-\r\nUser-Agent: gucefCOM-HTTP/1.0 (Linux;)\r\n\r\n", valuepath.C_String(), host.C_String(), byteoffset );        
        }


        if ( m_socket.ConnectTo( host ,
                                 port ) )
        {                                         
                bool success = m_socket.Send( buffer, (UInt32)strlen( buffer ) );
                
                delete []buffer;
                
                return success;
        }
        return false;                       
}

/*-------------------------------------------------------------------------*/                  

bool 
CHTTPClient::Get( const CORE::CString& urlstring                 ,
                  const UInt32 byteoffset /* = 0 */              ,
                  const CORE::CValueList* valuelist /* = NULL */ )
{TRACE;
        CORE::CString host;
        UInt16 port;
        CORE::CString path;

        if ( ParseURL( urlstring ,
                       host      ,
                       port      ,
                       path      ) )
        {      
                return Get( host       ,
                            port       ,
                            path       ,
                            byteoffset ,
                            valuelist  );        
        }                       
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::ParseURL( const CORE::CString& urlstring ,
                       CORE::CString& host            ,
                       UInt16& port                   ,
                       CORE::CString& path            )
{TRACE;
        if ( urlstring.Length() < 10 ) return false;
        
        CORE::CString lowcaseurl( urlstring.Lowercase() );
        lowcaseurl = lowcaseurl.ReplaceChar( '\\', '/' );
                
        if ( 0 != lowcaseurl.HasSubstr( "http://", true ) )
        {       
                /*
                 *      This client only accepts http URL's,.. D'oh
                 */
                return false;
        }
        
        /*
         *      Parse till the end of the domain segment and get the port
         *      if it is included
         */
        CORE::CString remainder;
        remainder.Set( lowcaseurl.C_String()+7, lowcaseurl.Length()-7 );
        CORE::CString segmenta( remainder.SubstrToChar( '/' ) );
        CORE::CString segmentb( remainder.SubstrToChar( ':' ) );
        port = 80; 
        if ( segmenta.Length() < segmentb.Length() )
        {
                host = segmenta;
                path = remainder.C_String() + segmenta.Length()+1;
        }
        else
        {
                host = segmentb;
                
                remainder = remainder.C_String() + segmentb.Length()+1;
                segmenta = remainder.SubstrToChar( '/' );
                port = (UInt16)segmenta.GetInt();
                path = remainder.C_String() + segmenta.Length()+1;
        }
        return true;
}                       

/*-------------------------------------------------------------------------*/

UInt32
CHTTPClient::GetBytesRecieved( void ) const
{TRACE;
        return m_recieved;
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnRead( COMCORE::CTCPClientSocket &socket ,
                     const char *data                  ,
                     UInt32 length                     ,
                     UInt32 &keepbytes                 )
{TRACE;       
        THTTPCODE http_code = HTTPCODE_DEFAULT;
        UInt32 size( 0 );
        bool resumeable( false );
        
        DEBUGOUTPUTsi( "CHTTPClient::OnRead(): bytes: ", length );
        
        if( !m_downloading )
        {
                if ( length < 13 )
                {
                        DEBUGOUTPUT( "Invalid HTTP header recieved" );
                        return;
                }                
                
                UInt32 i, i2, i3;
                char *headers, *tempChar;

                /*
                 *	Check if the server is using the HTTP protocol.
                 */
                if( ( strncmp( data, "HTTP/1.1", 8 ) != 0 ) &&
                    ( strncmp( data, "HTTP/1.0", 8 ) != 0 ) )
                {
                        DEBUGOUTPUT("Doesnt look like HTTP protocol");
                        return;
                }
                
                /*
                 *      Parse the error code from the string.
                 */
                char codestr[ 4 ];
                codestr[ 3 ] = '\0';
                strncpy( codestr, data+9, 4 );
                http_code = (THTTPCODE) CORE::Str_To_Int( codestr );                        

                /*
                 *      Check if an error occured.
                 *      If so we will stop processing the data.
                 *      There are codes that are not errors so we need to
                 *      check for them
                 */
                if( http_code >= 400 )
                {
                        DEBUGOUTPUTsi( "CHTTPClient: HTTP Error: ", http_code );
                        
                        THTTPErrorEventData eventData( http_code );
                        NotifyObservers( m_HTTPErrorEventID, &eventData );                        
                        return;
                }
                
                /*
                 *      Move to the next segment end delimiter
                 *      This will give use the size of the header segment
                 */
                for(i = 0; data[i] != '\r' || data[i+2] != '\r'; i++) {}
                i += 2;

                /*
                 *      Create a buffer for the header data and copy
                 *      the header info into it.
                 */
                headers = new char[i+1];
                strncpy(headers, data, i);
                headers[i] = '\0';
                
                DEBUGOUTPUTsi( "header size: ", i );
                DEBUGOUTPUT( headers );                
                
                /*
                 *      Check wheter we are beeing told to look somewhere else for
                 *      the requested recource
                 */                 
                if( http_code == HTTPCODE_302_FOUND ) 
                {
                        m_recieved = size = 0;

                        tempChar = strstr(headers, "\nLocation");

                        i2 = 9;

                        if(tempChar[i2] == ':') i2++;
                        if(tempChar[i2] == ' ') i2++;

                        for(i3 = i2; tempChar[i3] != '\r' && tempChar[i3] != '\n'; i3++) {}

                        tempChar[i3] = '\0';

                        socket.Close();

                        tempChar = &tempChar[i2];
                        
                        THTTPRedirectEventData eventData( tempChar );
                        NotifyObservers( m_HTTPRedirectEventID, &eventData );                        

                        Get( tempChar, 0 );

                        delete[] headers;

                        return ;
                }
                
                m_downloading = true;
                
                /*
                 *      Parse the content-length integer
                 */
                tempChar = strstr(headers, "\nContent-Length");
                if(tempChar != NULL) 
                {
                        char *tempChar2;
                        UInt32 length2;
                        i2 = 15;

         	        if(tempChar[i2] == ':') i2++;
        	        if(tempChar[i2] == ' ') i2++;

	        	for(i3 = i2; tempChar[i3] != '\r' && tempChar[i3] != '\n'; i3++) {}

                        length2 = i3 - i2;

                        tempChar2 = new char[length2 + 1];

                        strncpy(tempChar2, &tempChar[i2], length2 + 1);
                        tempChar2[length2] = '\0';

                        size = CORE::Str_To_Int(tempChar2);

                        delete[] tempChar2;
                }
                else 
                {
                        size = 0;
                }

        	m_recieved = 0;

                /*
                 *      Check whether the transfer is resumeable
                 */
                tempChar = strstr(headers, "\nAccept-Ranges");
                
                if(tempChar != NULL) 
                {
                        i2 = 14;

         	        if(tempChar[i2] == ':') i2++;
        	        if(tempChar[i2] == ' ') i2++;

                        if(strncmp(&tempChar[i2], "bytes", 5) == 0) resumeable = true;
                        else resumeable = false;
                }
                else resumeable = false;

                /*
                 *      Clean up and reposition the pointer to remove the header
                 */
                delete[] headers;

		i += 2;
                length -= i;
                data = &data[i];
	}
        
        if ( 0 == size )
	{
                /*
                 *      Since we did not find a content-length segment we will have to
                 *      check the data for content manually and try to determine the
                 *      content-length if there is any.
                 */
                UInt32 i;
                char *tempChar;                
                
                while(m_filesize < length) 
                {
                        if(m_filesize == 0) 
                        {
                                for(i = 0; (data[i] == '\r' || data[i] == '\n') && i < length; i++) {}

                                if(i != 0) 
                                {
                                        length -= i;
                                        data = &data[i];
                                }

                                if(length == 0) return;

                                for(i = 0; data[i] != '\r' && data[i] != '\n' && i < length; i++) {}

                                if(i >= length) 
                                {
                                        // Bad data recieved
                                        NotifyObservers( m_connectionErrorEventID );

                                        m_recieved = size = 0;
                                        socket.Close();

                                        return;
                                }

                                tempChar = new char[i + 1];
                                strncpy(tempChar, data, i + 1);
                                tempChar[i] = '\0';
                                sscanf(tempChar, "%x", &m_filesize);
                                delete[] tempChar;
   
                                DEBUGOUTPUTi( m_filesize );

                                if(m_filesize == 0) 
                                {
                                        NotifyObservers( m_HTTPTransferFinishedEventID );

                                        size = m_recieved;
                                        socket.Close();

                                        return;
                                }

                                if(data[i] == '\r' && data[i + 1] == '\n') i += 2;
                                else i++;

                                length -= i;
                                data = &data[i];
                        }

                        if(m_filesize < length) 
                        {                                
                                // Notify observers about the HTTP transfer payload contents we recieved
                                CORE::CDynamicBuffer linkBuffer;
                                linkBuffer.LinkTo( data, m_filesize );
                                THTTPDataRecievedEventData cBuffer( &linkBuffer );
                                NotifyObservers( m_HTTPDataRecievedEventID, &cBuffer );
                                                                 
                                m_recieved += m_filesize;
                                length -= m_filesize;

                                data = &data[m_filesize];
                                m_filesize = 0;
                        }
                }

                if(m_filesize > length && length != 0) 
                {

                    // Notify observers about the HTTP payload content info we recieved
                    struct SHTTPContentEventData cedStruct;
                    cedStruct.contentSize = m_filesize;
                    cedStruct.resumeSupported = resumeable;
                    cedStruct.HTTPcode = http_code;
                    THTTPContentEventData contentEventData( cedStruct );
                    NotifyObservers( m_HTTPContentEventID, &contentEventData );
                                
                    // Notify observers about the HTTP transfer payload contents we recieved
                    CORE::CDynamicBuffer linkBuffer;
                    linkBuffer.LinkTo( data, length );
                    THTTPDataRecievedEventData cBuffer( &linkBuffer );
                    NotifyObservers( m_HTTPDataRecievedEventID, &cBuffer );
                                                          
                    m_recieved += length;
                    m_filesize -= length;
                }
        }
        else 
        {
            m_recieved += length;                

            // Notify observers about the HTTP payload content info we recieved
            struct SHTTPContentEventData cedStruct;
            cedStruct.contentSize = size;
            cedStruct.resumeSupported = resumeable;
            cedStruct.HTTPcode = http_code;
            THTTPContentEventData contentEventData( cedStruct );
            NotifyObservers( m_HTTPContentEventID, &contentEventData );
                                                      	
            // Notify observers about the HTTP transfer payload contents we recieved
            CORE::CDynamicBuffer linkBuffer;
            linkBuffer.LinkTo( data, length );
            THTTPDataRecievedEventData cBuffer( &linkBuffer );
            NotifyObservers( m_HTTPDataRecievedEventID, &cBuffer );                      
                    
            NotifyObservers( m_HTTPTransferFinishedEventID );                        

            socket.Close();                   
        }                                    
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnDisconnect( COMCORE::CTCPClientSocket &socket )
{TRACE;

        m_downloading = false;
        NotifyObservers( m_disconnectedEventID );
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnConnected( COMCORE::CTCPClientSocket &socket )
{TRACE;

        NotifyObservers( m_connectedEventID );        
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnConnecting( COMCORE::CTCPClientSocket& socket )
{TRACE;

        NotifyObservers( m_connectingEventID );        
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnWrite( COMCORE::CTCPClientSocket &socket ,
                      const void* data                  ,
                      UInt32 length                     )
{TRACE;
        
    // Notify observers about the data dispatch
    CORE::CDynamicBuffer linkBuffer;
    linkBuffer.LinkTo( data, length );
    THTTPDataRecievedEventData cBuffer( &linkBuffer );
    NotifyObservers( m_HTTPDataSendEventID, &cBuffer );
}                      

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::IsConnected( void ) const
{TRACE;
    
    return m_socket.IsActive();
}

/*-------------------------------------------------------------------------*/

void
CHTTPClient::RegisterEvents( void )
{TRACE;
        CORE::CNotificationIDRegistry* registry = CORE::CNotificationIDRegistry::Instance();
        
        registry->Register( CHTTPClient::ConnectingEvent, true );
        registry->Register( CHTTPClient::ConnectedEvent, true );
        registry->Register( CHTTPClient::DisconnectedEvent, true );
        registry->Register( CHTTPClient::ConnectionErrorEvent, true );
        registry->Register( CHTTPClient::HTTPErrorEvent, true );
        registry->Register( CHTTPClient::HTTPRedirectEvent, true );
        registry->Register( CHTTPClient::HTTPContentEvent, true );
        registry->Register( CHTTPClient::HTTPDataRecievedEvent, true );
        registry->Register( CHTTPClient::HTTPDataSendEvent, true );
        registry->Register( CHTTPClient::HTTPTransferFinishedEvent, true );
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetConnectingEventID( void ) const
{TRACE;

    return m_connectingEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetConnectedEventID( void ) const
{TRACE;

    return m_connectedEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetDisconnectedEventID( void ) const
{TRACE;

    return m_disconnectedEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetConnectionErrorEventID( void ) const
{TRACE;

    return m_connectionErrorEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetHTTPErrorEventID( void ) const
{TRACE;

    return m_HTTPErrorEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetHTTPRedirectEventID( void ) const
{TRACE;

    return m_HTTPRedirectEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetHTTPContentEventID( void ) const
{TRACE;

    return m_HTTPContentEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetHTTPDataRecievedEventID( void ) const
{TRACE;

    return m_HTTPDataRecievedEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetHTTPDataSendEventID( void ) const
{TRACE;
    
    return m_HTTPDataSendEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CHTTPClient::GetHTTPTransferFinishedEventID( void ) const
{TRACE;

    return m_HTTPTransferFinishedEventID;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
