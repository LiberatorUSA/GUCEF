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

#include <assert.h>

#ifndef CBPTCPCLIENTSOCKETINTERFACE_H
#include "CBPTCPClientSocketInterface.h"
#define CBPTCPCLIENTSOCKETINTERFACE_H
#endif /* CBPTCPCLIENTSOCKETINTERFACE_H ? */

#ifndef TCP_BASICPROTOCOL_H
#include "tcp_basicprotocol.h"
#define TCP_BASICPROTOCOL_H
#endif /* TCP_BASICPROTOCOL_H ? */

#include "CBPTCPClientSocket.h"

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

using namespace GUCEF::CORE;
using namespace GUCEF::COMCORE;

/*-------------------------------------------------------------------------*/

CBPTCPClientSocket::CBPTCPClientSocket( bool isblocking )
        : m_socket( isblocking ) ,
          m_iface( NULL )
{TRACE;
        m_socket.SetInterface( this );
}

/*-------------------------------------------------------------------------*/

CBPTCPClientSocket::CBPTCPClientSocket( const CBPTCPClientSocket& src )
        : m_socket( false )
{TRACE;
        /* dummy, do not use */
        assert( 0 );
}

/*-------------------------------------------------------------------------*/

CBPTCPClientSocket::~CBPTCPClientSocket()
{TRACE;
        m_socket.Close();
}

/*-------------------------------------------------------------------------*/

CBPTCPClientSocket&
CBPTCPClientSocket::operator=( const CBPTCPClientSocket& src )
{TRACE;       
        
        /* dummy, do not use */
        assert( 0 );
        return *this;  
}

/*-------------------------------------------------------------------------*/

bool 
CBPTCPClientSocket::ConnectTo( const CORE::CString& address , 
                               UInt16 port                  )
{TRACE;
        return m_socket.ConnectTo( address ,
                                   port    );
}

/*-------------------------------------------------------------------------*/                               

bool 
CBPTCPClientSocket::Reconnect( void )
{TRACE;
        return m_socket.Reconnect();
}

/*-------------------------------------------------------------------------*/
        
void 
CBPTCPClientSocket::Close( void )
{TRACE;
        m_socket.Close();
}

/*-------------------------------------------------------------------------*/

bool 
CBPTCPClientSocket::Send( const void* data         ,
                          UInt32 length            ,
                          UInt32 timeout /* = 0 */ )
{TRACE;
        
        UInt16 sendlength( static_cast<UInt16>(length) );
        m_sendbuffer.SetDataSize( length+4 );
        m_sendbuffer[ 0 ] = BP_TCP_TRANSMISSION_BEGIN;        
        m_sendbuffer.CopyFrom( 1, 2, &sendlength );
        m_sendbuffer.CopyFrom( 3, length, data );
        m_sendbuffer[ length+3 ] = BP_TCP_TRANSMISSION_END;
        
        return m_socket.Send( m_sendbuffer.GetBufferPtr() ,
                              length+3                    ,
                              timeout                     );
}

/*-------------------------------------------------------------------------*/                          
                       
bool 
CBPTCPClientSocket::Send( const CORE::CString& data )
{TRACE;
        return Send( data.C_String() ,
                     data.Length()   ,
                     0               );
}

/*-------------------------------------------------------------------------*/

bool
CBPTCPClientSocket::ParseTCPTransmission( const char *data        ,
                                          UInt32 size             ,
                                          const char*& transstart ,
                                          UInt32& transsize       )
{TRACE;
        const char* dataptr = data;
        for ( UInt32 i=0; i<size; ++i )
        {
                if ( data[ i ] == BP_TCP_TRANSMISSION_BEGIN )
                {                        
                        /*
                         *      Found start marker
                         */
                        if ( i+4 < size )
                        {
                                dataptr = &data[ i+1 ];
                                UInt16 tsize = *(reinterpret_cast<const UInt16*>( dataptr ));
                                dataptr += 2;
                                
                                if ( i+4+tsize <= size )
                                {
                                        if ( dataptr[ tsize ] == BP_TCP_TRANSMISSION_END )
                                        {                                                
                                                /*
                                                 *      Found end marker so
                                                 *      the size is prob. correct
                                                 */
                                                transsize = tsize;
                                                transstart = dataptr;
                                                return true; 
                                        }
                                        else
                                        {       
                                                transstart = data;
                                                transsize = 0;                                
                                                return false;
                                        }                                                
                                }
                                else
                                {       
                                        transstart = data;
                                        transsize = 0;                                
                                        return false;
                                }                                                                        
                        }
                        else
                        {       
                                transstart = data;
                                transsize = 0;                                
                                return false;
                        }                        
                }
        }
        transstart = data;
        transsize = 0;
        return true;
}

/*-------------------------------------------------------------------------*/

void 
CBPTCPClientSocket::SetMaxRead( UInt32 mr )
{TRACE;
        m_socket.SetMaxRead( mr );
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CBPTCPClientSocket::GetMaxRead( void ) const
{TRACE;
        return m_socket.GetMaxRead();
}

/*-------------------------------------------------------------------------*/

bool 
CBPTCPClientSocket::IsActive( void ) const
{TRACE;
        return m_socket.IsActive();
}

/*-------------------------------------------------------------------------*/

bool 
CBPTCPClientSocket::IsBlocking( void ) const
{TRACE;
        return m_socket.IsBlocking();
}

/*-------------------------------------------------------------------------*/
        
void 
CBPTCPClientSocket::SetInterface( CBPTCPClientSocketInterface* iface )
{TRACE;
        m_iface = iface;
}

/*-------------------------------------------------------------------------*/
        
CBPTCPClientSocketInterface* 
CBPTCPClientSocket::GetInterface( void ) const
{TRACE;
        return m_iface;
}

/*-------------------------------------------------------------------------*/

void 
CBPTCPClientSocket::OnRead( CTCPClientSocket &socket ,
                            const char *data         ,
                            UInt32 size              ,
                            UInt32 &keepbytes        )
{TRACE;
        const char* dataptr( data );
        const char* transstart;
        UInt32 transsize( 0 );
        UInt32 totalused( 0 );
        while ( ParseTCPTransmission( dataptr        ,
                                      size-totalused ,
                                      transstart     ,
                                      transsize      ) )
        {
                totalused += transsize;
                if ( m_iface )
                {
                        m_iface->OnRead( *this      , 
                                         transstart ,
                                         transsize  );
                }                                         
                dataptr += transsize;                        
        }
        keepbytes = size - totalused;
}

/*-------------------------------------------------------------------------*/                            

void 
CBPTCPClientSocket::OnWrite( CTCPClientSocket &socket ,
                             const void* data         ,
                             UInt32 length            )
{TRACE;
        if ( m_iface )
        {
                m_iface->OnWrite( *this  ,
                                  data   ,
                                  length );
        }
}                             

/*-------------------------------------------------------------------------*/

void 
CBPTCPClientSocket::OnConnecting( CTCPClientSocket &socket )
{TRACE;
        if ( m_iface )
        {
                m_iface->OnConnecting( *this );
        }
}

/*-------------------------------------------------------------------------*/

void 
CBPTCPClientSocket::OnConnected( CTCPClientSocket &socket )
{TRACE;
        if ( m_iface )
        {
                m_iface->OnConnected( *this );
        }
}

/*-------------------------------------------------------------------------*/

void 
CBPTCPClientSocket::OnDisconnect( CTCPClientSocket &socket )
{TRACE;
        if ( m_iface )
        {
                m_iface->OnDisconnect( *this );
        }
}

/*-------------------------------------------------------------------------*/

void 
CBPTCPClientSocket::OnError( CTCPClientSocket &socket ,
                             TSocketError errorcode   )
{TRACE;
        if ( m_iface )
        {
                m_iface->OnError( *this     ,
                                  errorcode );
        }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
                             