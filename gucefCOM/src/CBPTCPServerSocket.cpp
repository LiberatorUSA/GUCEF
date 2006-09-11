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

#ifndef CBPTCPSERVERSOCKETINTERFACE_H
#include "CBPTCPServerSocketInterface.h"
#define CBPTCPSERVERSOCKETINTERFACE_H
#endif /* CBPTCPSERVERSOCKETINTERFACE_H ? */

#ifndef TCP_BASICPROTOCOL_H
#include "tcp_basicprotocol.h"
#define TCP_BASICPROTOCOL_H
#endif /* TCP_BASICPROTOCOL_H ? */

#include "CBPTCPServerSocket.h"

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

CBPTCPServerSocket::CBPTCPServerSocket( void )
        : m_socket( false )
{TRACE;
        /* dummy, do not use */
        assert( 0 );        
}

/*-------------------------------------------------------------------------*/

CBPTCPServerSocket::CBPTCPServerSocket( bool blocking )
        : m_socket( blocking ) ,
          m_iface( NULL ) 
{TRACE;
        m_socket.SetInterface( this );
}

/*-------------------------------------------------------------------------*/
        
CBPTCPServerSocket::~CBPTCPServerSocket()
{TRACE;
        m_socket.Close();
}

/*-------------------------------------------------------------------------*/

UInt32 
CBPTCPServerSocket::GetActiveCount( void ) const
{TRACE;
        return m_socket.GetActiveCount();
}

/*-------------------------------------------------------------------------*/
        
void 
CBPTCPServerSocket::SetInterface( CBPTCPServerSocketInterface *new_iface )
{TRACE;
        m_iface = new_iface;
}

/*-------------------------------------------------------------------------*/
        
CBPTCPServerSocketInterface* 
CBPTCPServerSocket::GetInterface( void ) const
{TRACE;
        return m_iface;
}

/*-------------------------------------------------------------------------*/

bool 
CBPTCPServerSocket::ListenOnPort( UInt16 port )
{TRACE;
        return m_socket.ListenOnPort( port );
}

/*-------------------------------------------------------------------------*/
        
void 
CBPTCPServerSocket::Close( void )
{TRACE;
        m_socket.Close();
}

/*-------------------------------------------------------------------------*/
        
UInt16 
CBPTCPServerSocket::GetPort( void ) const
{TRACE;
        return m_socket.GetPort();
}

/*-------------------------------------------------------------------------*/
        
bool 
CBPTCPServerSocket::IsActive( void ) const
{TRACE;
        return m_socket.IsActive();
}

/*-------------------------------------------------------------------------*/
        
bool 
CBPTCPServerSocket::IsBlocking( void ) const
{TRACE;
        return m_socket.IsBlocking();
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CBPTCPServerSocket::GetMaxConnections( void ) const
{TRACE;
        return m_socket.GetMaxConnections();
}

/*-------------------------------------------------------------------------*/

bool
CBPTCPServerSocket::ParseTCPTransmission( const char *data        ,
                                          UInt16 size             ,
                                          const char*& transstart ,
                                          UInt16& transsize       )
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
CBPTCPServerSocket::OnClientRead( COMCORE::CTCPServerSocket& socket     ,
                                  COMCORE::CTCPServerConnection& client ,
                                  UInt32 connection                     ,
                                  const char *data                      ,
                                  UInt16 size                           ,
                                  UInt16 &keepbytes                     )
{TRACE;
        const char* dataptr( data );
        const char* transstart;
        UInt16 transsize( 0 );
        UInt16 totalused( 0 );
        while ( ParseTCPTransmission( dataptr        ,
                                      size-totalused ,
                                      transstart     ,
                                      transsize      ) )
        {
                totalused += transsize;
                if ( m_iface )        
                {
                        m_iface->OnClientRead( *this                                                              ,
                                               *static_cast<CBPTCPServerConnection*>(m_connections[ connection ]) ,
                                               connection                                                         ,
                                               dataptr                                                            ,
                                               transsize                                                          );
                }                                         
                dataptr += transsize;                        
        }
        keepbytes = size - totalused;        
}

/*-------------------------------------------------------------------------*/                                  

void 
CBPTCPServerSocket::OnClientWrite( COMCORE::CTCPServerSocket& socket     ,
                                   COMCORE::CTCPServerConnection& client ,
                                   UInt32 connection                     ,
                                   const char *data                      ,
                                   UInt32 size                           )
{TRACE;
        if ( m_iface )        
        {
                m_iface->OnClientWrite( *this                                                              ,
                                        *static_cast<CBPTCPServerConnection*>(m_connections[ connection ]) ,
                                        connection                                                         ,
                                        data                                                               ,
                                        size                                                               );
        }
}

/*-------------------------------------------------------------------------*/                                   

void 
CBPTCPServerSocket::OnClientConnected( COMCORE::CTCPServerSocket& socket        ,
                                       COMCORE::CTCPServerConnection& clientcon ,
                                       UInt32 connection                        )
{TRACE;
        if ( m_iface )        
        {
                m_iface->OnClientConnected( *this                                                              , 
                                            *static_cast<CBPTCPServerConnection*>(m_connections[ connection ]) , 
                                            connection                                                         );
        }
}

/*-------------------------------------------------------------------------*/                                       

void 
CBPTCPServerSocket::OnClientDisconnect( COMCORE::CTCPServerSocket& socket     ,
                                        COMCORE::CTCPServerConnection& client ,
                                        UInt32 connection                     ,
                                        bool clientclose                      )
{TRACE;
        if ( m_iface )        
        {
                m_iface->OnClientDisconnect( *this                                                              ,
                                             *static_cast<CBPTCPServerConnection*>(m_connections[ connection ]) ,
                                             connection                                                         ,
                                             clientclose                                                        );
        }
}

/*-------------------------------------------------------------------------*/                                        

void 
CBPTCPServerSocket::OnClientError( COMCORE::CTCPServerSocket& socket     ,
                                   COMCORE::CTCPServerConnection& client ,
                                   UInt32 connection                     ,
                                   TSocketError *errorcode               )
{TRACE;
        if ( m_iface )        
        {
                m_iface->OnClientError( *this                                                              ,  
                                        *static_cast<CBPTCPServerConnection*>(m_connections[ connection ]) ,
                                        connection                                                         ,
                                        errorcode                                                          );
        }
}

/*-------------------------------------------------------------------------*/                                   

void 
CBPTCPServerSocket::OnOpen( COMCORE::CTCPServerSocket &socket )
{TRACE;
        if ( m_iface )        
        {
                m_iface->OnOpen( *this );
        }        
}

/*-------------------------------------------------------------------------*/

void 
CBPTCPServerSocket::OnClose( COMCORE::CTCPServerSocket &socket )
{TRACE;
        if ( m_iface )        
        {
                m_iface->OnClose( *this );
        }
}

/*-------------------------------------------------------------------------*/

void 
CBPTCPServerSocket::OnMaxConnectionsChanged( COMCORE::CTCPServerSocket &socket ,
                                             UInt32 old_max                    ,
                                             UInt32 new_max                    )
{TRACE;        
        if ( old_max < new_max )
        {
                m_connections.SetArraySize( new_max );
                for ( UInt32 i=old_max; i<new_max; ++i )
                {
                        m_connections.SetEntry( i, new CBPTCPServerConnection( socket.GetConnection( i ) ) );
                }                        
        }
        else
        {
                for ( UInt32 i=new_max; i<old_max; ++i )
                {
                        delete static_cast< CBPTCPServerConnection* >( m_connections[ i ] );                        
                }
                m_connections.SetArraySize( new_max );
        }
        if ( m_iface )        
        {
                m_iface->OnMaxConnectionsChanged( *this   ,
                                                  old_max ,
                                                  new_max );
        }
}                                             

/*-------------------------------------------------------------------------*/

void 
CBPTCPServerSocket::OnError( COMCORE::CTCPServerSocket &socket ,
                             TSocketError errorcode            )
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
                             