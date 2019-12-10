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

#ifndef GUCEF_COMCORE_CTCPCONNECTION_H
#define GUCEF_COMCORE_CTCPCONNECTION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_COMCORE_CSOCKET_H
#include "CSocket.h"
#define GUCEF_COMCORE_CSOCKET_H
#endif /* GUCEF_COMCORE_CSOCKET_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

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

/**
 *  Base class for TCP based socket connections
 */
class GUCEF_COMCORE_EXPORT_CPP CTCPConnection : public CSocket
{
    public:

    static const CORE::CEvent ConnectedEvent;
    static const CORE::CEvent DisconnectedEvent;
    static const CORE::CEvent DataRecievedEvent;
    static const CORE::CEvent DataSentEvent;
    static const CORE::CEvent SocketErrorEvent;

    typedef CORE::TLinkedCloneableBuffer TDataRecievedEventData;
    typedef CORE::TLinkedCloneableBuffer TDataSentEventData;
    typedef CORE::TCloneableInt32        TSocketErrorEventData;
    
    static void RegisterEvents( void );
    
    public:
    
    CTCPConnection( void );

    virtual ~CTCPConnection();
    
    virtual CIPAddress GetRemoteIP( void ) const = 0;
    
    virtual const CORE::CString& GetRemoteHostName( void ) const = 0;
    
    virtual UInt16 GetRemoteTCPPort( void ) const = 0;
    
    virtual bool IsActive( void ) const = 0;
    
    virtual void Close( void ) = 0;
    
    virtual bool Send( const void* dataSource ,
                       const UInt32 dataSize  ) = 0;

    bool SendString( const CString& data );

    /**
     *  Allows you to set whether to use data coalescing on sends.
     *  This is commonly refered to as the Nagle algorithm
     */
    virtual bool SetUseTcpSendCoalescing( bool coaleseData ) = 0;

    virtual bool GetUseTcpSendCoalescing( void ) const = 0;

    virtual const CORE::CString& GetClassTypeName( void ) const;
 
    private:
    
    CTCPConnection( const CTCPConnection& src );            /**< connections are unique and cannot be copied */
    CTCPConnection& operator=( const CTCPConnection& src ); /**< connections are unique and cannot be copied */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CTCPCONNECTION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
