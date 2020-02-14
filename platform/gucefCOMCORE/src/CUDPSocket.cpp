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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

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
    struct timeval timeout;         /* timeout for blocking operations */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CUDPSocket::CUDPSocket( CORE::CPulseGenerator& pulseGenerator ,
                        bool blocking                         )
    : CSocket()                           
    , m_autoReopenOnError( false )        
    , _blocking( blocking )               
    , _data( NULL )                       
    , m_hostAddress()                     
    , m_buffer()                          
    , m_pulseGenerator( &pulseGenerator )
    , m_maxUpdatesPerCycle( 0 )
    , m_allowMulticastLoopback( false )
    , m_multicastTTL( 8 )
    , m_allowBroadcast( false )
    , m_bytesReceived( 0 )
    , m_bytesTransmitted( 0 )
    , m_nrOfDataReceivedEvents( 0 )
{GUCEF_TRACE;

    RegisterEvents();

    _data = new TUDPSockData;
    memset( _data, 0, sizeof( TUDPSockData ) );

    // Set a default buffer size large enough to handle Jumbo frames
    m_buffer.SetBufferSize( 9000 );

    TEventCallback callback( this, &CUDPSocket::OnPulse );
    SubscribeTo( m_pulseGenerator                  ,
                 CORE::CPulseGenerator::PulseEvent ,
                 callback                          );
    TEventCallback callback2( this, &CUDPSocket::OnPulseGeneratorDestruction );
    SubscribeTo( m_pulseGenerator                        ,
                 CORE::CPulseGenerator::DestructionEvent ,
                 callback2                               );
}

/*-------------------------------------------------------------------------*/

CUDPSocket::CUDPSocket( bool blocking )
    : CSocket()            
    , m_autoReopenOnError( false )
    , _blocking( blocking )
    , _data( NULL )        
    , m_hostAddress()      
    , m_buffer()           
    , m_pulseGenerator( &CORE::CCoreGlobal::Instance()->GetPulseGenerator() )
    , m_maxUpdatesPerCycle( 0 )
    , m_allowMulticastLoopback( false )
    , m_multicastTTL( 8 )
    , m_allowBroadcast( false )
    , m_bytesReceived( 0 )
    , m_bytesTransmitted( 0 )
    , m_nrOfDataReceivedEvents( 0 )
{GUCEF_TRACE;

    RegisterEvents();

    _data = new TUDPSockData;
    memset( _data, 0, sizeof( TUDPSockData ) );

    // Set a default buffer size large enough to handle Jumbo frames
    m_buffer.SetBufferSize( 9000 );

    TEventCallback callback( this, &CUDPSocket::OnPulse );
    SubscribeTo( m_pulseGenerator                  ,
                 CORE::CPulseGenerator::PulseEvent ,
                 callback                          );
    TEventCallback callback2( this, &CUDPSocket::OnPulseGeneratorDestruction );
    SubscribeTo( m_pulseGenerator                        ,
                 CORE::CPulseGenerator::DestructionEvent ,
                 callback2                               );
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
    return const_cast< const CUDPSocket* >( this )->SendPacketTo( dest     ,
                                                                  data     ,
                                                                  datasize );
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::SendPacketTo( const CIPAddress& dest ,
                          const void* data       ,
                          UInt16 datasize        ) const
{GUCEF_TRACE;

    struct sockaddr_in remote;
    memset( &remote, 0, sizeof( remote ) );
    remote.sin_addr.s_addr = dest.GetAddress();
    remote.sin_port = dest.GetPort();
    remote.sin_family = AF_INET;
    
    Int32 returnValue = sendto( _data->sockid             ,
                                (const char*)data         ,
                                datasize                  ,
                                0                         ,
                                (struct sockaddr*)&remote ,
                                sizeof(remote)            );
    
    if ( returnValue >= 0 )
        m_bytesTransmitted += returnValue;
    else
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket:SendPacketTo: Failed to send " + CORE::UInt16ToString( datasize ) + " bytes to " + dest.AddressAndPortAsString() );

    return returnValue;
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

bool
CUDPSocket::IsIncomingDataQueued( void )
{GUCEF_TRACE;

    return Update( false );
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Update( bool performRead )
{GUCEF_TRACE;

    if ( !_blocking && _data->sockid > 0 )
    {
        fd_set readfds;      /* Setup the read variable for the select function */
        fd_set exceptfds;    /* Setup the except variable for the select function */

        FD_ZERO( &readfds );
        FD_ZERO( &exceptfds );

        LockData();

        FD_SET( _data->sockid, &readfds );
        FD_SET( _data->sockid, &exceptfds );

        int errorCode = 0;
        if ( dvsocket_select( _data->sockid+1   ,
                              &readfds          ,
                              NULL              , // We don't care about socket writes here
                              &exceptfds        ,
                              &_data->timeout   ,
                              &errorCode        ) != SOCKET_ERROR )
        {
            /* something happened on the socket */

            if ( FD_ISSET( _data->sockid, &exceptfds ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CUDPSocket(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( errorCode ) );

                int lastError = errorCode;
                dvsocket_closesocket( _data->sockid, &errorCode );
                _data->sockid = 0;

                TSocketErrorEventData eData( lastError );
                if ( !NotifyObservers( UDPSocketErrorEvent, &eData ) ) return false;

                UnlockData();

                if ( m_autoReopenOnError )
                    Open();

                return false;
            }
            else
            if ( FD_ISSET( _data->sockid, &readfds ) )
            {
                /* data can be read from the socket */
                if ( performRead )
                {
                    CIPAddress sender;
                    bool receiveSuccess = 0 < Recieve( sender, NULL, 0);
                    UnlockData();
                    return receiveSuccess;
                }
                return true;
            }

            UnlockData();
        }
        else
        {
            /* select call failed */
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CUDPSocket(" + CORE::PointerToString( this ) + "): Socket error occured (select call failed): " + CORE::Int32ToString( errorCode ) );

            TSocketErrorEventData eData( errorCode );
            NotifyObservers( UDPSocketErrorEvent, &eData );

            UnlockData();
            
            if ( m_autoReopenOnError )
                Open();
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::SetMaxUpdatesPerCycle( UInt32 maxUpdates )
{GUCEF_TRACE;

    m_maxUpdatesPerCycle = maxUpdates;
}

/*-------------------------------------------------------------------------*/


UInt32
CUDPSocket::GetMaxUpdatesPerCycle( void ) const
{GUCEF_TRACE;

    return m_maxUpdatesPerCycle;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::OnPulse( CORE::CNotifier* notifier                 ,
                     const CORE::CEvent& eventid               ,
                     CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( m_maxUpdatesPerCycle > 0 )
    {
        UInt32 cycleCount = 0;
        while( ( cycleCount < m_maxUpdatesPerCycle ) && Update( true ) ) 
        {
            ++cycleCount;
        }
    }
    else
    {
        while( Update( true ) );
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

void
CUDPSocket::SetAllowBroadcast( bool allowBroadcast )
{GUCEF_TRACE;

    m_allowBroadcast = allowBroadcast;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::GetAllowBroadcast( void ) const
{GUCEF_TRACE;

    return m_allowBroadcast;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::SetAllowMulticastLoopback( bool allowLoopback )
{GUCEF_TRACE;

    m_allowMulticastLoopback = allowLoopback;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::GetAllowMulticastLoopback( void ) const
{GUCEF_TRACE;

    return m_allowMulticastLoopback;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::SetMulticastTTL( Int32 ttl )
{GUCEF_TRACE;

    m_multicastTTL = ttl;
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::GetMulticastTTL( void ) const
{GUCEF_TRACE;

    return m_multicastTTL;
}

/*-------------------------------------------------------------------------*/

UInt32
CUDPSocket::GetNrOfDataReceivedEvents( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        UInt32 nrOfDataReceivedEvents = m_nrOfDataReceivedEvents;
        m_nrOfDataReceivedEvents = 0;
        return nrOfDataReceivedEvents;
    }
    else
        return m_nrOfDataReceivedEvents;
}

/*-------------------------------------------------------------------------*/

UInt32
CUDPSocket::GetBytesReceived( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        UInt32 bytesReceived = m_bytesReceived;
        m_bytesReceived = 0;
        return bytesReceived;
    }
    else
        return m_bytesReceived;
}

/*-------------------------------------------------------------------------*/

UInt32 
CUDPSocket::GetBytesTransmitted( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        UInt32 bytesTransmitted = m_bytesTransmitted;
        m_bytesTransmitted = 0;
        return bytesTransmitted;
    }
    else
        return m_bytesTransmitted;
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::Recieve( void )
{GUCEF_TRACE;
    
    return Recieve( nullptr, 0 );
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::Recieve( CIPAddress& src ,
                     void* destbuf   ,
                     UInt16 bufsize  )
{GUCEF_TRACE;

    struct sockaddr_in remote;
    socklen_t structsize( sizeof( remote ) );
    int sockError = 0;
    int retval = dvsocket_recvfrom( _data->sockid                  ,
                                    (char*)m_buffer.GetBufferPtr() ,
                                    m_buffer.GetBufferSize()       ,
                                    0                              ,
                                    (struct sockaddr*)&remote      ,
                                    &structsize                    ,
                                    &sockError                     );
    if ( retval < 0 )
    {       
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UDPSocket: Failure trying to receive data on socket " + m_hostAddress.AddressAndPortAsString() + ". Error code: " + CORE::Int32ToString( sockError ) );
        
        TSocketErrorEventData eData( sockError );
        NotifyObservers( UDPSocketErrorEvent, &eData );        
        return -1;
    }
    if ( retval == 0 )
    {
        Close( false );
        return 0;
    }
                                                                                             
    // Set the actual usefull bytes of data in the buffer
    // to match the number of retrieved bytes
    m_buffer.SetDataSize( retval );
    m_bytesReceived += retval;

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
    UDPPacketRecievedEventData eventData;
    TUDPPacketRecievedEventData& infoStruct = eventData.GetData();
    infoStruct.sourceAddress.SetPort( remote.sin_port );
    infoStruct.sourceAddress.SetAddress( remote.sin_addr.s_addr );
    infoStruct.dataBuffer.LinkTo( &m_buffer );
    
    ++m_nrOfDataReceivedEvents;
    
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

    return Open( m_hostAddress );
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Open( UInt16 port )
{GUCEF_TRACE;

    CIPAddress address( INADDR_ANY, htons( port ) );
    return Open( address );
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Open( const CORE::CString& localaddr ,
                  UInt16 port                    )
{GUCEF_TRACE;

    CHostAddress address( localaddr, port );
    bool success = Open( address );
    m_hostAddress = address;
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Open( const CIPAddress& localaddr )
{GUCEF_TRACE;

    Close( false );

    m_hostAddress = localaddr; 

    //create a UDP socket
    int errorCode = 0;
    if ( ( _data->sockid = dvsocket_socket( AF_INET     , 
                                            SOCK_DGRAM  , 
                                            IPPROTO_UDP , 
                                            &errorCode  ) ) == INVALID_SOCKET )
    {
        TSocketErrorEventData eData( errorCode );
        if ( !NotifyObservers( UDPSocketErrorEvent, &eData ) ) return false;
        
        if ( m_autoReopenOnError )
            m_pulseGenerator->RequestPeriodicPulses( this, PULSEUPDATEINTERVAL );
        
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UDPSocket: Failed to open socket at " + m_hostAddress.AddressAndPortAsString() + ". Error code: " + CORE::Int32ToString( errorCode ) );
        return false;
    }

    // Set the desired blocking mode
    if ( !SetBlockingMode( _data->sockid, _blocking ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UDPSocket: Failed to set blocking mode \"" + CORE::BoolToString( _blocking ) + "\" on socket" );
        
        if ( m_autoReopenOnError )
            m_pulseGenerator->RequestPeriodicPulses( this, PULSEUPDATEINTERVAL );

        return false;
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "UDPSocket: Successfully set blocking mode \"" + CORE::BoolToString( _blocking ) + "\" on socket" );

    int allowAddressReuse = 1;
    if ( 0 > dvsocket_setsockopt( _data->sockid, SOL_SOCKET, SO_REUSEADDR, (const char*) &allowAddressReuse, sizeof(int), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to set address reuse mode \"" + CORE::BoolToString( allowAddressReuse != 0 ) 
            + "\" on socket. Error code: " + CORE::UInt32ToString( errorCode ) );
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "UDPSocket: Successfully set address reuse mode \"" + CORE::BoolToString( allowAddressReuse != 0 ) + "\" on socket" );
                
    #ifdef SO_REUSEPORT
    int allowPortReuse = 1;
    if ( 0 > dvsocket_setsockopt( _data->sockid, SOL_SOCKET, SO_REUSEPORT, (const char*) &allowPortReuse, sizeof(int), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to port reuse mode \"" + CORE::BoolToString( allowPortReuse != 0 ) 
            + "\" on socket. Error code: " + CORE::UInt32ToString( errorCode ) );
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "UDPSocket: Successfully set port reuse mode \"" + CORE::BoolToString( allowPortReuse != 0 ) + "\" on socket" );
    #endif

    Int32 loopch = m_allowMulticastLoopback ? (Int32)1 : (Int32)0;
    if ( 0 > dvsocket_setsockopt( _data->sockid, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*) &loopch, sizeof(loopch), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to set multicast loopback (dis)allowed mode \"" + CORE::BoolToString( loopch != 0 ) 
            + "\" on socket. Error code: " + CORE::UInt32ToString( errorCode ) );
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "UDPSocket: Successfully set multicast loopback mode \"" + CORE::BoolToString( loopch != 0 ) + "\" on socket" );
                
    if ( 0 > dvsocket_setsockopt( _data->sockid, IPPROTO_IP, IP_MULTICAST_TTL, (const char*) &m_multicastTTL, sizeof(m_multicastTTL), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to set multicast TTL to " + CORE::Int32ToString( m_multicastTTL ) 
            + "\" on socket. Error code: " + CORE::UInt32ToString( errorCode ) );
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "UDPSocket: Successfully set multicast TTL to \"" + CORE::Int32ToString( m_multicastTTL ) + "\" on socket" );

    // This option is needed on the socket in order to be able to receive broadcast messages
    // If not set the socket will not receive broadcast messages in the local network.
    Int32 broadcastOption = m_allowBroadcast ? 1 : 0;
    if ( 0 > dvsocket_setsockopt( _data->sockid, SOL_SOCKET, SO_BROADCAST, (const char*) &broadcastOption, sizeof(broadcastOption), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to set broadcast option to " + CORE::BoolToString( m_allowBroadcast ) 
            + "\" on socket. Error code: " + CORE::UInt32ToString( errorCode ) );
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "UDPSocket: Successfully set broadcast option to \"" + CORE::BoolToString( m_allowBroadcast ) + "\" on socket" );

    _data->localaddress.sin_family = AF_INET;
    _data->localaddress.sin_port = m_hostAddress.GetPort();
    _data->localaddress.sin_addr.s_addr = m_hostAddress.GetAddress();

    if ( dvsocket_bind( _data->sockid                            ,
                        (struct sockaddr *) &_data->localaddress ,
                        sizeof(struct sockaddr_in)               ,
                        &errorCode                               ) == 0 )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Successfully bound and opened socket at " + m_hostAddress.AddressAndPortAsString() 
            + " aka " + CORE::UInt32ToString( m_hostAddress.GetAddress() ) + ":" + CORE::UInt16ToString( m_hostAddress.GetPort() ) + " in network format" );
        if ( !NotifyObservers( UDPSocketOpenedEvent ) ) return true;

        // We will now be requiring periodic updates to poll for data
        m_pulseGenerator->RequestPeriodicPulses( this, PULSEUPDATEINTERVAL );

        return true;
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to bind to " + m_hostAddress.AddressAndPortAsString() + ". Error code: " + CORE::Int32ToString( errorCode ) );
        TSocketErrorEventData eData( errorCode );
        if ( !NotifyObservers( UDPSocketErrorEvent, &eData ) ) return false;

        if ( m_autoReopenOnError )
            m_pulseGenerator->RequestPeriodicPulses( this, PULSEUPDATEINTERVAL );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Join( const CIPAddress& multicastGroup , 
                  const CIPAddress& srcAddr        )
{GUCEF_TRACE;

    struct ip_mreq_source imr; 
    memset( &imr, 0, sizeof( imr ) );
    imr.imr_multiaddr.S_un.S_addr = multicastGroup.GetAddress();
    imr.imr_sourceaddr.S_un.S_addr = srcAddr.GetAddress();
    imr.imr_interface.S_un.S_addr = m_hostAddress.GetAddress();

    int errorCode = 0;
    if ( 0 > dvsocket_setsockopt( _data->sockid, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char*) &imr, sizeof(imr), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to join multicast group " + multicastGroup.AddressAsString() 
            + ", using interface " + m_hostAddress.AddressAsString() + ", for data coming from " + srcAddr.AddressAsString()
            + ". Error code: " + CORE::UInt32ToString( errorCode ) )
        return false;    
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Joined multicast group " + multicastGroup.AddressAsString() 
        + ", using interface " + m_hostAddress.AddressAsString() + ", for data coming from " + srcAddr.AddressAsString() );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Join( const CIPAddress& multicastGroup )
{GUCEF_TRACE;

    struct ip_mreq_source imr; 
    memset( &imr, 0, sizeof( imr ) );
    imr.imr_multiaddr.S_un.S_addr = multicastGroup.GetAddress();
    imr.imr_interface.S_un.S_addr = m_hostAddress.GetAddress();
    
    int errorCode = 0;
    if ( 0 > dvsocket_setsockopt( _data->sockid, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &imr, sizeof(imr), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to join multicast group " + multicastGroup.AddressAsString() 
            + ", using interface " + m_hostAddress.AddressAsString() 
            + " aka " + CORE::UInt32ToString( m_hostAddress.GetAddress() ) + ":" + CORE::UInt16ToString( m_hostAddress.GetPort() ) + " in network format"
            + ", for all data on the multicast group except explicit blocks"
            + ". Error code: " + CORE::UInt32ToString( errorCode ) )
        return false;    
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Joined multicast group " + multicastGroup.AddressAsString() 
        + ", using interface " + m_hostAddress.AddressAsString() + ", for all data on the multicast group except explicit blocks" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Leave( const CIPAddress& multicastGroup , 
                   const CIPAddress& srcAddr        )
{GUCEF_TRACE;

    struct ip_mreq_source imr; 
    memset( &imr, 0, sizeof( imr ) );
    imr.imr_multiaddr.S_un.S_addr  = multicastGroup.GetAddress();
    imr.imr_sourceaddr.S_un.S_addr = srcAddr.GetAddress();
    imr.imr_interface.S_un.S_addr  = m_hostAddress.GetAddress();
    
    int errorCode = 0;
    if ( 0 > dvsocket_setsockopt( _data->sockid, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (char*) &imr, sizeof(imr), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to leave multicast group " + multicastGroup.AddressAsString() 
            + ", using interface " + m_hostAddress.AddressAsString() + ", for data coming data from " + srcAddr.AddressAsString()
            + ". Error code: " + CORE::UInt32ToString( errorCode ) );
        return false;    
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Left multicast group " + multicastGroup.AddressAsString() 
        + ", using interface " + m_hostAddress.AddressAsString() + ", for data coming from " + srcAddr.AddressAsString() );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Leave( const CIPAddress& multicastGroup )
{GUCEF_TRACE;

    struct ip_mreq_source imr; 
    memset( &imr, 0, sizeof( imr ) );
    imr.imr_multiaddr.S_un.S_addr  = multicastGroup.GetAddress();
    imr.imr_interface.S_un.S_addr  = m_hostAddress.GetAddress();
    
    int errorCode = 0;
    if ( 0 > dvsocket_setsockopt( _data->sockid, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*) &imr, sizeof(imr), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to leave multicast group " + multicastGroup.AddressAsString() 
            + ", using interface " + m_hostAddress.AddressAsString() + ", for all data on the multicast group" 
            + ". Error code: " + CORE::UInt32ToString( errorCode ) );
        return false;    
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Left multicast group " + multicastGroup.AddressAsString() 
        + ", using interface " + m_hostAddress.AddressAsString() + ", for all data on the multicast group" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Block( const CIPAddress& multicastGroup , 
                   const CIPAddress& srcAddr        )
{GUCEF_TRACE;

    struct ip_mreq_source imr; 
    memset( &imr, 0, sizeof( imr ) );
    imr.imr_multiaddr.S_un.S_addr  = multicastGroup.GetAddress();
    imr.imr_sourceaddr.S_un.S_addr = srcAddr.GetAddress();
    imr.imr_interface.S_un.S_addr  = m_hostAddress.GetAddress();
    
    int errorCode = 0;
    if ( 0 > dvsocket_setsockopt( _data->sockid, IPPROTO_IP, IP_BLOCK_SOURCE, (char*) &imr, sizeof(imr), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to block data for multicast group " + multicastGroup.AddressAsString() 
            + ", using interface " + m_hostAddress.AddressAsString() + ", for data coming data from " + srcAddr.AddressAsString() 
            + ". Error code: " + CORE::UInt32ToString( errorCode ) );
        return false;    
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Blocking data for multicast group " + multicastGroup.AddressAsString() 
        + ", using interface " + m_hostAddress.AddressAsString() + ", for data coming from " + srcAddr.AddressAsString() );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::Unblock( const CIPAddress& multicastGroup , 
                     const CIPAddress& srcAddr        )
{GUCEF_TRACE;

    struct ip_mreq_source imr; 
    memset( &imr, 0, sizeof( imr ) );
    imr.imr_multiaddr.S_un.S_addr  = multicastGroup.GetAddress();
    imr.imr_sourceaddr.S_un.S_addr = srcAddr.GetAddress();
    imr.imr_interface.S_un.S_addr  = m_hostAddress.GetAddress();

    int errorCode = 0;
    if ( 0 > dvsocket_setsockopt( _data->sockid, IPPROTO_IP, IP_UNBLOCK_SOURCE, (char*) &imr, sizeof(imr), &errorCode ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed to unblock data for multicast group " + multicastGroup.AddressAsString() 
            + ", using interface " + m_hostAddress.AddressAsString() + ", for data coming data from " + srcAddr.AddressAsString() 
            + ". Error code: " + CORE::UInt32ToString( errorCode ) );
        return false;    
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Unblocking data for multicast group " + multicastGroup.AddressAsString() 
        + ", using interface " + m_hostAddress.AddressAsString() + ", for data coming from " + srcAddr.AddressAsString() );
    return true;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::Close( bool shutdownOnly )
{GUCEF_TRACE;

    if ( IsActive() )
    {
        if ( !shutdownOnly )
        {
            // A socket close will trigger a shutdown sequence if one hasnt occured yet and will also free up the related
            // system resources
            int errorCode = 0;
            if ( 0 == dvsocket_closesocket( _data->sockid, &errorCode ) )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Closed socket at " + m_hostAddress.AddressAndPortAsString() );

                // We now no longer require periodic updates to poll for data
                if ( !m_autoReopenOnError )
                    m_pulseGenerator->RequestStopOfPeriodicUpdates( this );

                NotifyObservers( UDPSocketClosedEvent );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: Failed close socket at " + m_hostAddress.AddressAndPortAsString() );
                
                TSocketErrorEventData eData( errorCode );
                NotifyObservers( UDPSocketErrorEvent, &eData );
            }
        }
        else
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UDPSocket: shutting down socket at " + m_hostAddress.AddressAndPortAsString() );
            shutdown( _data->sockid, 1 );
        }        
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

    return m_hostAddress.GetPortInHostByteOrder();
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::SetAutoReOpenOnError( bool autoReOpen )
{GUCEF_TRACE;

    m_autoReopenOnError = autoReOpen;
}

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::GetAutoReOpenOnError( void ) const
{GUCEF_TRACE;

    return m_autoReopenOnError;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
