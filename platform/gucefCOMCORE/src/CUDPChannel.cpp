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

#include <assert.h>

#ifndef GUCEF_COMCORE_CUDPMASTERSOCKET_H
#include "CUDPMasterSocket.h"
#define GUCEF_COMCORE_CUDPMASTERSOCKET_H
#endif /* GUCEF_COMCORE_CUDPMASTERSOCKET_H ? */

#include "CUDPChannel.h"

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

const CORE::CEvent CUDPChannel::UDPPacketReceivedEvent = "GUCEF::COMCORE::CUDPChannel::UDPPacketReceivedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CUDPChannel::RegisterEvents( void )
{GUCEF_TRACE;
    
    UDPPacketReceivedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CUDPChannel::CUDPChannel( CUDPMasterSocket& parentSocket ,
                          const CIPAddress& remoteAddr   )
    : m_remotePeer( remoteAddr )      ,
      m_parentSocket( &parentSocket )
{GUCEF_TRACE;

    assert( m_parentSocket != NULL );
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/
    
CUDPChannel::~CUDPChannel()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CIPAddress&
CUDPChannel::GetRemoteAddress( void ) const
{GUCEF_TRACE;

    return m_remotePeer;
}

/*-------------------------------------------------------------------------*/

CUDPMasterSocket&
CUDPChannel::GetParentSocket( void ) const
{GUCEF_TRACE;

    return *m_parentSocket;
}

/*-------------------------------------------------------------------------*/

Int32
CUDPChannel::SendPacket( const void* data , 
                         UInt16 datasize  )
{GUCEF_TRACE;

    // Delegate to the socket while adding our peer address
    return m_parentSocket->SendPacketTo( m_remotePeer ,
                                         data         ,
                                         datasize     );
}

/*-------------------------------------------------------------------------*/
    
bool
CUDPChannel::DoNotifyObservers( const CORE::CEvent& eventid               ,
                                CORE::CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;
    
    return NotifyObservers( eventid   ,
                            eventData );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/