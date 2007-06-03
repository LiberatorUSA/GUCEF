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

#ifndef GUCEF_COMCORE_CUDPCHANNEL_H
#define GUCEF_COMCORE_CUDPCHANNEL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CUDPMasterSocket;

/*-------------------------------------------------------------------------*/

/**
 *  Aggregate component of the CUDPMasterSocket.
 *  Provides a medium to which observers can subscribe and receive
 *  per peer notification of received data.
 */
class GUCEF_COMCORE_EXPORT_CPP CUDPChannel : public CORE::CNotifier
{
    public:

    static const CORE::CEvent UDPPacketReceivedEvent;   
    
    static void RegisterEvents( void );
    
    typedef CORE::TLinkedCloneableBuffer UDPPacketReceivedEventData;
    
    public:  
    
    const CIPAddress& GetRemoteAddress( void ) const;
    
    CUDPMasterSocket& GetParentSocket( void ) const;
    
    /**
     *      Attempts to send a UDP packet to the peer identified by the channel.
     *      Note that the data you send must fit within the payload
     *      space of a UDP packet.
     *
     *      @param data pointer to the data you wish to send
     *      @param datasize size in bytes of the data block pointed to by data
     *      @return the actual number of bytes that where sent. -1 indicates an error.
     */
    Int32 SendPacket( const void* data , 
                      UInt16 datasize  );

    private:
    friend class CUDPMasterSocket; 
        
    CUDPChannel( CUDPMasterSocket& parentSocket ,
                 const CIPAddress& remoteAddr   );
    
    virtual ~CUDPChannel();
    
    bool DoNotifyObservers( const CORE::CEvent& eventid         ,
                            CORE::CICloneable* eventData = NULL );
    
    private:
    
    CUDPChannel( const CUDPChannel& src );            /**< not implemented, not possible */
    CUDPChannel& operator=( const CUDPChannel& src ); /**< not implemented, not possible */
    
    private:
    
    CIPAddress m_remotePeer;
    CUDPMasterSocket* m_parentSocket;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CUDPCHANNEL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-03-2007 :
        - Initial version

-----------------------------------------------------------------------------*/