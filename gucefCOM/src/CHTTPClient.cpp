/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef CHTTPCLIENTINTERFACE_H
#include "CHTTPClientInterface.h"
#define CHTTPCLIENTINTERFACE_H
#endif /* CHTTPCLIENTINTERFACE_H ? */

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPClient::CHTTPClient( void )
        : m_socket( false )      ,
          m_iface( NULL )        ,
          m_downloading( false ) ,
          m_recieved( 0 )        ,
          m_filesize( 0 )
{TRACE;
        m_socket.SetInterface( this );
}

/*-------------------------------------------------------------------------*/
        
CHTTPClient::CHTTPClient( const CHTTPClient& src )
        : m_socket( false )
{TRACE;
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/
        
CHTTPClient::~CHTTPClient()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CHTTPClient& 
CHTTPClient::operator=( const CHTTPClient& src )
{TRACE;
        /* dummy, do not use */
        return *this;
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

void 
CHTTPClient::SetInterface( CHTTPClientInterface* iface )
{TRACE;
        m_iface = iface;
}
	
/*-------------------------------------------------------------------------*/

CHTTPClientInterface* 
CHTTPClient::GetInterface( void ) const
{TRACE;
        return m_iface;
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

                        if ( m_iface ) m_iface->OnHTTPError( *this, http_code );                        
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
                        if ( m_iface ) m_iface->OnRedirect( *this, tempChar );

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
                                        if ( m_iface ) m_iface->OnError(*this);
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
                                        if ( m_iface ) m_iface->OnFinish(*this);

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
                                if ( m_iface ) 
                                {
                                        //m_iface->OnContent( *this, resumeable, http_code );
                                        m_iface->OnDataRecieved( *this, data, m_filesize );
                                }                                        
                                m_recieved += m_filesize;
                                length -= m_filesize;

                                data = &data[m_filesize];
                                m_filesize = 0;
                        }
                }

                if(m_filesize > length && length != 0) 
                {
                        if ( m_iface ) 
                        {
                                m_iface->OnContent( *this, m_filesize, resumeable, http_code );
                                m_iface->OnDataRecieved( *this, data, length );
                        }                                
                        m_recieved += length;
                        m_filesize -= length;
                }
        }
        else 
        {
                m_recieved += length;                
                if ( m_iface )
                {
                        m_iface->OnContent( *this, size, resumeable, http_code );                                      	
                        m_iface->OnDataRecieved( *this, data, length );                        
                        m_iface->OnFinish(*this);                        
                }
                socket.Close();                   
        }                                    
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnDisconnect( COMCORE::CTCPClientSocket &socket )
{TRACE;
        m_downloading = false;
        if ( m_iface ) m_iface->OnDisconnected( *this );  
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnConnected( COMCORE::CTCPClientSocket &socket )
{TRACE;
        if ( m_iface ) m_iface->OnConnected( *this );        
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnConnecting( COMCORE::CTCPClientSocket& socket )
{TRACE;
        if ( m_iface ) m_iface->OnConnecting( *this );        
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClient::OnWrite( COMCORE::CTCPClientSocket &socket ,
                      const void* data                  ,
                      UInt32 length                     )
{TRACE;
        if ( m_iface ) m_iface->OnDataSend( *this             ,
                                            (const char*)data ,
                                            length            ); 
}                      

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
