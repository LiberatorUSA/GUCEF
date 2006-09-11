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

/*
 *      The PH UDP Socket packet protocol:
 *
 *      1) Each packet should start with a 'magic number' (GUUDPSOCKETID) of sorts 
 *         to identify the packet as a GU protocol packet. 2 bytes.
 *      2) next is the packet checksum. 4 bytes.
 *      3) after that the packet type identifier. 2 bytes.
 *      4) flag to indicate wheter to deliver always (1) or only if it is the newest packet of this type (0). 1 byte.
 *      5) packet counter value for the packet type. 4 bytes.
 *      6) payload segment size. 2 bytes
 *      7) packet data payload. x bytes. with a max of ( max < 2 byte int max)
 *
 *      total packet header size: 15 bytes      
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>

#ifndef CIPHUDPSOCKETEVENTHANDLER_H
#include "CIPHUDPSocketEventHandler.h"
#define CIPHUDPSOCKETEVENTHANDLER_H
#endif /* CIPHUDPSOCKETEVENTHANDLER_H ? */

#include "CPHUDPSocket.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUPACKETHDRSIZE    15
#define GUUDPSOCKETID      63475
#define PACKETCOUNTERMAX   UINT32MAX-100;

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

CPHUDPSocket::CPHUDPSocket( UInt32 minpayloadsize  ,
                            UInt16 sendpackettypes ,
                            UInt16 rcvpackettypes  )
        : _sock( false )                                  ,
          _buffer( GUPACKETHDRSIZE + minpayloadsize )     ,
          _sendpackettypes( sendpackettypes )             ,
          _rcvpackettypes( rcvpackettypes )               ,
          _sendpcounters( new UInt32[ sendpackettypes ] ) ,
          _rcvpcounters( new UInt32[ rcvpackettypes ] )   ,
          _handler( NULL )
{TRACE;
        memset( _sendpcounters, 0, sendpackettypes*4 );
        memset( _rcvpcounters, 0, rcvpackettypes*4 );
        
        _sock.RegisterEventHandler( this );
}

/*-------------------------------------------------------------------------*/

CPHUDPSocket::~CPHUDPSocket()
{TRACE;
        delete []_sendpcounters;
        delete []_rcvpcounters;
}

/*-------------------------------------------------------------------------*/

bool 
CPHUDPSocket::ResolveDest( const GUCEF::CORE::CString& destaddrstr ,
                           UInt16 destport                         ,  
                           TIPAddress& dest                        )
{TRACE;
        return GUCEF::COMCORE::CUDPSocket::ResolveDest( destaddrstr ,
                                                        destport    ,
                                                        dest        );
}

/*-------------------------------------------------------------------------*/

void
CPHUDPSocket::BufferPacketSendInfo( const void* data         ,
                                    const UInt16 datasize    ,
                                    const UInt16 packettype  ,
                                    const bool deliveralways )
{TRACE;
        _buffer.SetDataSize( GUPACKETHDRSIZE + datasize );
        char* buffer = static_cast<char*>( _buffer.GetBufferPtr() );
        
        ( *((UInt16*) buffer) ) = GUUDPSOCKETID;
        ( *((UInt16*)(buffer+6)) ) = packettype;
        *(buffer+8) = deliveralways;
        ( *((UInt32*) (buffer+9)) ) = _sendpcounters[ packettype ]++;
        ( *((UInt16*) (buffer+13)) ) = datasize;                        
        memcpy( buffer+15, data, datasize );
        
        UInt32 sum( 0UL );
        for ( UInt16 i=0; i<datasize; ++i )
        {
                sum += *((char*)data)+i;
        }
        ( *((UInt32*) (buffer+2)) ) = sum;        
}                                    

/*-------------------------------------------------------------------------*/                         

Int32
CPHUDPSocket::SendPacketTo( const TIPAddress& dest ,
                            const void* data       , 
                            UInt16 datasize        ,
                            UInt16 packettype      ,
                            bool deliveralways     )
{TRACE;
        if ( packettype >= _sendpackettypes ) return -1;
        
        BufferPacketSendInfo( data          ,
                              datasize      ,
                              packettype    , 
                              deliveralways );
        
        return _sock.SendPacketTo( dest                       ,
                                   _buffer.GetBufferPtr()     ,
                                   GUPACKETHDRSIZE + datasize ); 
}

/*-------------------------------------------------------------------------*/

Int32 
CPHUDPSocket::SendPacketTo( const GUCEF::CORE::CString& dnsname ,
                            UInt16 port                         ,
                            const void* data                    , 
                            UInt16 datasize                     ,
                            UInt16 packettype                   ,
                            bool deliveralways                  )
{TRACE;
        if ( packettype >= _sendpackettypes ) return -1;        
        
        BufferPacketSendInfo( data          ,
                              datasize      ,
                              packettype    , 
                              deliveralways );
        
        return _sock.SendPacketTo( dnsname                    ,
                                   port                       ,
                                   _buffer.GetBufferPtr()     ,
                                   GUPACKETHDRSIZE + datasize );
}

/*-------------------------------------------------------------------------*/

void 
CPHUDPSocket::OnUDPSocketEvent( GUCEF::COMCORE::CUDPSocket& socket                ,
                                GUCEF::COMCORE::CUDPSocket::TUDPSocketEvent event )
{TRACE;
        if ( !_handler ) return;
        
        switch ( event )
        {
                case GUCEF::COMCORE::CUDPSocket::UDP_PACKET_RECIEVED :
                {                        
                        OnPacketRecieved();                        
                        return;
                }
                default :
                {
                        _handler->OnPHUDPSocketEvent( *this, event );
                }                                               
        }
}                                

/*-------------------------------------------------------------------------*/

void
CPHUDPSocket::OnPacketRecieved( void )
{TRACE;
        /*
         *      Obtain the packet and hope that the user has set the
         *      correct minimal buffer size
         */
        char* buffer = static_cast<char*>( _buffer.GetBufferPtr() );
        TIPAddress src;
        Int32 rbytes = _sock.Recieve( src                           ,
                                      buffer                        ,
                                      (UInt16)_buffer.GetDataSize() );

        /*
         *      Begin various checks before parsing packet info
         */                                      
        if ( rbytes > GUPACKETHDRSIZE )             
        {
                UInt16 datasize( *(UInt16*)(buffer+13) );
                if ( rbytes == GUPACKETHDRSIZE + datasize )
                {
                        char* usrdata( buffer+GUPACKETHDRSIZE );
                        UInt32 sum( 0UL );
                        for ( UInt16 i=0; i<datasize; ++i )
                        {
                                sum += *(usrdata+i);        
                        }
                        if ( sum == *(UInt32*)(buffer+2) )
                        {
                                /*
                                 *      Now we know that the packet is in all likelyhood
                                 *      a valid packet. We will check if the packet should be
                                 *      dropped if it's to old and pass to the handler if 
                                 *      possible
                                 */
                                UInt16 packettype( *(UInt16*)(buffer+6) );                                
                                if ( packettype < _rcvpackettypes )
                                {
                                        UInt32 packetnumber( *(UInt32*)(buffer+6) );
                                        if ( _rcvpcounters[ packettype ] < packetnumber )
                                        {
                                                _rcvpcounters[ packettype ] = packetnumber;
                                                
                                                _handler->OnPacketRecieved( *this        ,
                                                                            src          ,
                                                                            usrdata      ,
                                                                            datasize     ,
                                                                            packettype   ,
                                                                            packetnumber );
                                                return;
                                        }
                                        else                        
                                        if ( ( *(buffer+8) ) ) // alwaysdeliver data field
                                        {
                                                _handler->OnPacketRecieved( *this        ,
                                                                            src          ,
                                                                            usrdata      ,
                                                                            datasize     ,
                                                                            packettype   ,
                                                                            packetnumber );
                                                return;
                                        }
                                }
                        }                                                        
                }                                                
        }
        DEBUGOUTPUT( "CPHUDPSocket: dropped packet" );
}
                 
/*-------------------------------------------------------------------------*/

bool 
CPHUDPSocket::ResolveSrc( const TIPAddress& src            ,
                          GUCEF::CORE::CString& srcaddrstr ,
                          UInt16& srcport                  )
{TRACE; 
        return GUCEF::COMCORE::CUDPSocket::ResolveSrc( src        ,
                                                       srcaddrstr ,
                                                       srcport    );    
}                                           

/*-------------------------------------------------------------------------*/
       
bool 
CPHUDPSocket::Open( void )
{TRACE;
        return _sock.Open();
}       

/*-------------------------------------------------------------------------*/ 
        
bool 
CPHUDPSocket::Open( UInt16 port )
{TRACE;
        return _sock.Open( port );
}

/*-------------------------------------------------------------------------*/

bool 
CPHUDPSocket::Open( const GUCEF::CORE::CString& localaddr ,
                    UInt16 port                           )
{TRACE;
        return _sock.Open( localaddr ,
                           port      );
}

/*-------------------------------------------------------------------------*/  
        
void 
CPHUDPSocket::Close( bool force )
{TRACE;
        return _sock.Close( force );               
}

/*-------------------------------------------------------------------------*/

bool 
CPHUDPSocket::IsActive( void ) const
{TRACE;
        return _sock.IsActive();
}

/*-------------------------------------------------------------------------*/
 
void 
CPHUDPSocket::RegisterEventHandler( CIPHUDPSocketEventHandler* handler )
{TRACE;
        _handler = handler;        
}

/*-------------------------------------------------------------------------*/

UInt16 
CPHUDPSocket::GetPort( void ) const
{TRACE;
        return _sock.GetPort();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
