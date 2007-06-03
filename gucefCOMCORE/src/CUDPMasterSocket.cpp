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

#include "CUDPMasterSocket.h"

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

const CORE::CEvent CUDPMasterSocket::UDPMasterSocketErrorEvent = "GUCEF::COMCORE::CUDPMasterSocket::UDPMasterSocketErrorEvent";
const CORE::CEvent CUDPMasterSocket::UDPMasterSocketClosedEvent = "GUCEF::COMCORE::CUDPMasterSocket::UDPMasterSocketClosedEvent";
const CORE::CEvent CUDPMasterSocket::UDPMasterSocketOpenedEvent = "GUCEF::COMCORE::CUDPMasterSocket::UDPMasterSocketOpenedEvent";
const CORE::CEvent CUDPMasterSocket::UDPChannelCreatedEvent = "GUCEF::COMCORE::CUDPMasterSocket::UDPChannelCreatedEvent";
const CORE::CEvent CUDPMasterSocket::UDPChannelToBeDestroyedEvent = "GUCEF::COMCORE::CUDPMasterSocket::UDPChannelToBeDestroyedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CUDPMasterSocket::RegisterEvents( void )
{GUCEF_TRACE;

    UDPMasterSocketErrorEvent.Initialize();
    UDPMasterSocketClosedEvent.Initialize();
    UDPMasterSocketOpenedEvent.Initialize();
    UDPChannelCreatedEvent.Initialize();
    UDPChannelToBeDestroyedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/
    
CUDPMasterSocket::CUDPMasterSocket( bool blocking )
    : m_udpSocket( blocking )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/
    
CUDPMasterSocket::~CUDPMasterSocket()
{GUCEF_TRACE;

    Close( true );
}

/*-------------------------------------------------------------------------*/
    
bool
CUDPMasterSocket::IsActive( void ) const
{GUCEF_TRACE;

    return m_udpSocket.IsActive();
}

/*-------------------------------------------------------------------------*/
    
bool
CUDPMasterSocket::Open( void )
{GUCEF_TRACE;

    return m_udpSocket.Open();
}

/*-------------------------------------------------------------------------*/
      
bool
CUDPMasterSocket::Open( UInt16 port )
{GUCEF_TRACE;

    return m_udpSocket.Open( port );
}

/*-------------------------------------------------------------------------*/
    
bool
CUDPMasterSocket::Open( const CORE::CString& localaddr ,
                        UInt16 port                    )
{GUCEF_TRACE;

    return m_udpSocket.Open( localaddr ,
                             port      );
}

/*-------------------------------------------------------------------------*/
               
UInt16
CUDPMasterSocket::GetPort( void ) const
{GUCEF_TRACE;

    return m_udpSocket.GetPort();
}

/*-------------------------------------------------------------------------*/
    
void
CUDPMasterSocket::Close( bool force /* = false */ )
{GUCEF_TRACE;

    return m_udpSocket.Close( force );
}

/*-------------------------------------------------------------------------*/
                           
bool
CUDPMasterSocket::IsBlocking( void ) const
{GUCEF_TRACE;

    return m_udpSocket.IsBlocking();
}

/*-------------------------------------------------------------------------*/    

CUDPChannel*
CUDPMasterSocket::GetChannelForAddress( const CIPAddress& remoteAddr )
{GUCEF_TRACE;

    TChannelMap::iterator i = m_channelMap.find( remoteAddr );
    if ( i != m_channelMap.end() )
    {
        return (*i).second;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

CUDPChannel*
CUDPMasterSocket::ReserveChannelForAddress( const CIPAddress& remoteAddr )
{GUCEF_TRACE;

    // Check for an existing channel for the given address
    CUDPChannel* channel = GetChannelForAddress( remoteAddr );
    if ( NULL == channel )
    {
        // Reserve a new channel for the given address
        channel = new CUDPChannel( *this, remoteAddr );
        m_channelMap.insert( std::pair< CIPAddress, CUDPChannel* >( remoteAddr, channel ) );
        
        UDPChannelCreatedEventData eData( channel );
        NotifyObservers( UDPChannelCreatedEvent, &eData );
    }
    return channel;
}

/*-------------------------------------------------------------------------*/

bool
CUDPMasterSocket::DestroyChannelForAddress( const CIPAddress& remoteAddr )
{GUCEF_TRACE;

    TChannelMap::iterator i = m_channelMap.find( remoteAddr );
    if ( i != m_channelMap.end() )
    {
        UDPChannelToBeDestroyedEventData eData( (*i).second );
        NotifyObservers( UDPChannelToBeDestroyedEvent, &eData );
        
        delete ((*i).second);
        m_channelMap.erase( i );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CUDPMasterSocket::DestroyAllChannels( void )
{GUCEF_TRACE;

    TChannelMap::iterator i = m_channelMap.begin();
    while ( i != m_channelMap.end() )
    {
        UDPChannelToBeDestroyedEventData eData( (*i).second );
        NotifyObservers( UDPChannelToBeDestroyedEvent, &eData );
        
        delete (*i).second;
        (*i).second = NULL;
        ++i;
    }
    m_channelMap.clear();
}

/*-------------------------------------------------------------------------*/

void
CUDPMasterSocket::OnNotify( CORE::CNotifier* notifier                 ,
                            const CORE::CEvent& eventid               ,
                            CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    // Mandatory: call the base class implementation
    CObservingNotifier::OnNotify( notifier  ,
                                  eventid   ,
                                  eventdata );
    
    // We only accept messages from our own UDP socket
    if ( notifier == &m_udpSocket )
    {
        if ( eventid == CUDPSocket::UDPPacketRecievedEvent )
        {
            // convert eventdata into something usable
            CUDPSocket::UDPPacketRecievedEventData* eData = static_cast< CUDPSocket::UDPPacketRecievedEventData* >( eventdata );
            const CUDPSocket::SUDPPacketRecievedEventData& data = eData->GetData();
            
            // Obtain a channel for the given peer address
            CUDPChannel* channel = ReserveChannelForAddress( data.sourceAddress );
            
            // Force the channel to notify to its clients
            // Because the data buffer link will only live inside this scope it is safe to
            // link to the already linked buffer
            CORE::TLinkedCloneableBuffer buffer( &data.dataBuffer.GetData() );
            channel->DoNotifyObservers( CUDPChannel::UDPPacketReceivedEvent ,
                                        &buffer                             );
        }
        else
        if ( eventid == CUDPSocket::UDPSocketErrorEvent )
        {
            NotifyObservers( UDPMasterSocketErrorEvent, eventdata );
        }
        else
        if ( eventid == CUDPSocket::UDPSocketClosedEvent )
        {
            NotifyObservers( UDPMasterSocketClosedEvent, eventdata );
        }
        else
        if ( eventid == CUDPSocket::UDPSocketOpenedEvent )
        {
            NotifyObservers( UDPMasterSocketOpenedEvent, eventdata );
        }                
    }
}

/*-------------------------------------------------------------------------*/

Int32
CUDPMasterSocket::SendPacketTo( const CORE::CString& DNSName ,
                                UInt16 port                  ,
                                const void* data             , 
                                UInt16 dataSize              )
{GUCEF_TRACE;

    return m_udpSocket.SendPacketTo( DNSName  ,
                                     port     ,
                                     data     ,
                                     dataSize );    
}

/*-------------------------------------------------------------------------*/

Int32
CUDPMasterSocket::SendPacketTo( const CIPAddress& dest ,
                                const void* data       , 
                                UInt16 datasize        )
{GUCEF_TRACE;

    return m_udpSocket.SendPacketTo( dest     ,
                                     data     ,
                                     datasize );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/