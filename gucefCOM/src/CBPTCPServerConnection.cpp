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

#ifndef TCP_BASICPROTOCOL_H
#include "tcp_basicprotocol.h"
#define TCP_BASICPROTOCOL_H
#endif /* TCP_BASICPROTOCOL_H ? */

#include "CBPTCPServerConnection.h"

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

void 
CBPTCPServerConnection::Close( void )
{
        m_socket->Close();        
}

/*-------------------------------------------------------------------------*/                              

bool 
CBPTCPServerConnection::Send( const char *buffer , 
                              UInt32 length      )
{
        m_sendbuffer.SetDataSize( length+4 );
        UInt16 payloadsize( static_cast<UInt16>( length ) );
        m_sendbuffer[ 0 ] = BP_TCP_TRANSMISSION_BEGIN;
        memcpy( static_cast<UInt8*>(m_sendbuffer.GetBufferPtr())+1, &payloadsize, 2 );
        memcpy( static_cast<UInt8*>(m_sendbuffer.GetBufferPtr())+3, buffer, length );
        m_sendbuffer[ 3+length ] = BP_TCP_TRANSMISSION_END;
        
        return m_socket->Send( static_cast<char*>(m_sendbuffer.GetBufferPtr()) ,
                               3+length                                        );
}

/*-------------------------------------------------------------------------*/                              

bool 
CBPTCPServerConnection::IsActive( void ) const
{
        return m_socket->IsActive();
}

/*-------------------------------------------------------------------------*/

void 
CBPTCPServerConnection::SetMaxRead( UInt32 mr )
{
        m_socket->SetMaxRead( mr );
}

/*-------------------------------------------------------------------------*/

UInt32 
CBPTCPServerConnection::GetMaxRead( void ) const
{
        return m_socket->GetMaxRead();
}

/*-------------------------------------------------------------------------*/

CBPTCPServerConnection::CBPTCPServerConnection( COMCORE::CTCPServerConnection *socket )
        : m_socket( socket )
{
}

/*-------------------------------------------------------------------------*/                                                

CBPTCPServerConnection::~CBPTCPServerConnection()
{
        m_socket->Close();
}

/*-------------------------------------------------------------------------*/
        
CBPTCPServerConnection::TSocketType 
CBPTCPServerConnection::GetType( void ) const
{
        return m_socket->GetType();
}

/*-------------------------------------------------------------------------*/

UInt32 
CBPTCPServerConnection::GetSocketID( void ) const
{
        return m_socket->GetSocketID();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
