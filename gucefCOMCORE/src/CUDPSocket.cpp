/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#include <string.h>       /* needed for memset() */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_COMCORE_SOCKETUTILS_H
#include "socketutils.h"
#define GUCEF_COMCORE_SOCKETUTILS_H
#endif /* GUCEF_COMCORE_SOCKETUTILS_H ? */

#include "CUDPSocket.h" /* definition of the class implemented here */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CUDPSocket::UDPSocketErrorEvent = "GUCEF::COMCORE::CUDPSocket::UDPSocketErrorEvent";
const CORE::CEvent CUDPSocket::UDPSocketClosedEvent = "GUCEF::COMCORE::CUDPSocket::UDPSocketClosedEvent";
const CORE::CEvent CUDPSocket::UDPSocketOpenedEvent = "GUCEF::COMCORE::CUDPSocket::UDPSocketOpenedEvent";
const CORE::CEvent CUDPSocket::UDPPacketRecievedEvent = "GUCEF::COMCORE::CUDPSocket::UDPPacketRecievedEvent";

#define PULSEUPDATEINTERVAL 10

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Forward declarations of types used
 */
struct CUDPSocket::SUDPSockData
{
    SOCKET sockid;
    struct sockaddr_in localaddress;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CUDPSocket::CUDPSocket( CORE::CPulseGenerator& pulseGenerator ,
                        bool blocking                         )
    : CSocket()                           ,
      _blocking( blocking )               ,
      m_port( 0 )                         ,
      _data( NULL )                       ,
      m_buffer()                          ,
      m_pulseGenerator( &pulseGenerator )
{GUCEF_TRACE;

    RegisterEvents();

    _data = new TUDPSockData;
    memset( _data, 0, sizeof( TUDPSockData ) );

    m_buffer.SetBufferSize( 1024 );

    SubscribeTo( m_pulseGenerator                              ,
                 CORE::CPulseGenerator::PulseEvent             ,
                 &TEventCallback( this, &CUDPSocket::OnPulse ) );
    SubscribeTo( m_pulseGenerator                                                  ,
                 CORE::CPulseGenerator::DestructionEvent                           ,
                 &TEventCallback( this, &CUDPSocket::OnPulseGeneratorDestruction ) );
}

/*-------------------------------------------------------------------------*/

CUDPSocket::CUDPSocket( bool blocking )
    : CSocket()             ,
      _blocking( blocking ) ,
      m_port( 0 )           ,
      _data( NULL )         ,
      m_buffer()            ,
      m_pulseGenerator( &CORE::CCoreGlobal::Instance()->GetPulseGenerator() )
{GUCEF_TRACE;

    RegisterEvents();

    _data = new TUDPSockData;
    memset( _data, 0, sizeof( TUDPSockData ) );

    m_buffer.SetBufferSize( 1024 );

    SubscribeTo( m_pulseGenerator                              ,
                 CORE::CPulseGenerator::PulseEvent             ,
                 &TEventCallback( this, &CUDPSocket::OnPulse ) );
    SubscribeTo( m_pulseGenerator                                                  ,
                 CORE::CPulseGenerator::DestructionEvent                           ,
                 &TEventCallback( this, &CUDPSocket::OnPulseGeneratorDestruction ) );
}

/*-------------------------------------------------------------------------*/

CUDPSocket::~CUDPSocket()
{GUCEF_TRACE;

    Close( true );

    delete _data;
    _data = NULL;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::RegisterEvents( void )
{GUCEF_TRACE;

    UDPSocketErrorEvent.Initialize();
    UDPSocketClosedEvent.Initialize();
    UDPSocketOpenedEvent.Initialize();
    UDPPacketRecievedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::SendPacketTo( const CIPAddress& dest ,
                          const void* data       ,
                          UInt16 datasize        )
{GUCEF_TRACE;

    if ( !_data->sockid )
    {
            Open();
    }

    struct sockaddr_in remote;
    memset( &remote, 0, sizeof( remote ) ); // should this be CLEAR_ADDR( &remote ); ?
    remote.sin_addr.s_addr = dest.GetAddress();
    remote.sin_port = dest.GetPort();
    remote.sin_family = AF_INET;
    return sendto( _data->sockid             ,
                   (const char*)data         ,
                   datasize                  ,
                   0                         ,
                   (struct sockaddr*)&remote ,
                   sizeof(remote)            );
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::SendPacketTo( const CORE::CString& dnsname ,
                          UInt16 port                  ,
                          const void* data             ,
                          UInt16 datasize              )
{GUCEF_TRACE;

    CIPAddress dest;
    if ( ConvertToIPAddress( dnsname, port, dest ) )
    {
            return SendPacketTo( dest, data, datasize );
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::OnPulse( CORE::CNotifier* notifier                 ,
                     const CORE::CEvent& eventid               ,
                     CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( !_blocking )
    {
        CIPAddress src;
        while ( IsIncomingDataQueued() )
        {
            Recieve( src  ,
                     NULL ,
                     0    );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::OnPulseGeneratorDestruction( CORE::CNotifier* notifier                 ,
                                         const CORE::CEvent& eventid               ,
                                         CORE::CICloneable* eventdata /* = NULL */ )

{GUCEF_TRACE;

    if ( notifier == m_pulseGenerator )
    {
        m_pulseGenerator = NULL;
    }
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::IsIncomingDataQueued( void ) const
{GUCEF_TRACE;

    TIMEVAL time = { 0, 0 };
    fd_set sockset;
    FD_ZERO( &sockset );
    FD_SET( _data->sockid, &sockset );

    if ( select( 2, &sockset, NULL, NULL, &time ) > 0 )
    {
            return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

UInt32
CUDPSocket::GetRecievedDataBufferSize( void ) const
{GUCEF_TRACE;

    return m_buffer.GetBufferSize();
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::SetRecievedDataBufferSize( const UInt32 newBufferSize )
{GUCEF_TRACE;

    m_buffer.SetBufferSize( newBufferSize );
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::Recieve( CIPAddress& src ,
                     void* destbuf   ,
                     UInt16 bufsize  )
{GUCEF_TRACE;

    struct sockaddr_in remote;
    socklen_t structsize( sizeof( remote ) );
    int sockError;
    int retval = dvsocket_recvfrom( _data->sockid                  ,
                                    (char*)m_buffer.GetBufferPtr() ,
                                    m_buffer.GetBufferSize()       ,
                                    0                              ,
                                    (struct sockaddr*)&remote      ,
                                    &structsize                    ,
                                    &sockError                     );
    if ( retval < 0 )
    {
            return -1;
    }
    if ( retval == 0 )
    {
            Close( true );
            return 0;
    }

    // Set the actual usefull bytes of data in the buffer
    // to match the number of retrieved bytes
    m_buffer.SetDataSize( retval );

    // Copy the data directly to our user
    src.SetPort( remote.sin_port );
    src.SetAddress( remote.sin_addr.s_addr );

    if ( NULL != destbuf )
    {
        UInt32  copyLimit = bufsize;
        if ( m_buffer.GetBufferSize() < copyLimit )
        {
            copyLimit = m_buffer.GetBufferSize();
        }
        memcpy( destbuf, m_buffer.GetBufferPtr(), copyLimit );
    }

    // Send an event notifying people that the data was recieved
    struct SUDPPacketRecievedEventData infoStruct;
    infoStruct.sourceAddress.SetPort( remote.sin_port );
    infoStruct.sourceAddress.SetAddress( remote.sin_addr.s_addr );
    infoStruct.dataBuffer = CORE::TLinkedCloneableBuffer( &m_buffer );

    UDPPacketRecievedEventData eventData( infoStruct );
    NotifyObservers( UDPPacketRecievedEvent, &eventData );

    return retval;
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::Recieve( void* destbuf  ,
                     UInt16 bufsize )
{GUCEF_TRACE;

    CIPAddress src;
    return Recieve( src     ,
                    destbuf ,
                    bufsize );
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Open( void )
{GUCEF_TRACE;

    Close( true );

    int errorCode;
    if ( dvsocket_bind( _data->sockid                            ,
                        (struct sockaddr *) &_data->localaddress ,
                        sizeof(struct sockaddr_in)               ,
                        &errorCode                               ) == 0 )
    {
        NotifyObservers( UDPSocketOpenedEvent );

        // We will now be requiring periodic updates to poll for data
        m_pulseGenerator->RequestPeriodicPulses( this, PULSEUPDATEINTERVAL );

        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Open( UInt16 port )
{GUCEF_TRACE;

    return Open( CORE::CString(), port );
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Open( const CORE::CString& localaddr ,
                  UInt16 port                    )
{GUCEF_TRACE;

    Close( true );

    int errorCode;
    _data->sockid = dvsocket_socket( AF_INET     ,
                                     SOCK_DGRAM  ,
                                     IPPROTO_UDP ,
                                     &errorCode  );
    if ( _data->sockid != INVALID_SOCKET ) return false;

    // Set the desired blocking mode
    if ( !SetBlockingMode( _data->sockid, _blocking ) )
    {
        return false;
    }

    _data->localaddress.sin_family = AF_INET;
    _data->localaddress.sin_port = htons( port );
    if ( localaddr.IsNULLOrEmpty() )
    {
        _data->localaddress.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        _data->localaddress.sin_addr.s_addr = inet_addr( localaddr.C_String() );
    }

    if ( dvsocket_bind( _data->sockid                            ,
                        (struct sockaddr *) &_data->localaddress ,
                        sizeof(struct sockaddr_in)               ,
                        &errorCode                               ) == 0 )
    {
        m_port = port;
        NotifyObservers( UDPSocketOpenedEvent );

        // We will now be requiring periodic updates to poll for data
        m_pulseGenerator->RequestPeriodicPulses( this, PULSEUPDATEINTERVAL );

        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::Close( bool force )
{GUCEF_TRACE;

    if ( IsActive() )
    {
        int error;
        force ? dvsocket_closesocket( _data->sockid, &error ) : shutdown( _data->sockid, 1 );

        // We now no longer require periodic updates to poll for data
        m_pulseGenerator->RequestStopOfPeriodicUpdates( this );

        NotifyObservers( UDPSocketClosedEvent );
    }
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::IsBlocking( void ) const
{GUCEF_TRACE;

    return _blocking;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::IsActive( void ) const
{GUCEF_TRACE;

    return _data->sockid > 0;
}

/*-------------------------------------------------------------------------*/

UInt16
CUDPSocket::GetPort( void ) const
{GUCEF_TRACE;

    return m_port;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
