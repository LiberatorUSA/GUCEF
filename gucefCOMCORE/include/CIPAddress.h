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

#ifndef GUCEF_COMCORE_CIPADDRESS_H
#define GUCEF_COMCORE_CIPADDRESS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* macros and build config for the COMCORE library */
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
 *  Class representing an IPv4 address with a port number included.
 *  Note that this class uses network byte order as its default.
 */
class GUCEF_COMCORE_EXPORT_CPP CIPAddress
{
    public:
    
    CIPAddress( void );
    
    CIPAddress( const CIPAddress& src );

    /**
     *  The values are expected to be in network byte order 
     */
    CIPAddress( const UInt32 address ,
                const UInt16 port    );

    /**
     *  The port value is expected to be in host byte order
     */
    CIPAddress( const CORE::CString& address ,
                const UInt16 port            );
                    
    virtual ~CIPAddress();
    
    void SetPort( const UInt16 port );
    
    /**
     *  Port number in network byte order
     */
    UInt16 GetPort( void ) const;
    
    void SetPortInHostByteOrder( const UInt16 port );

    /**
     *  Port number in host byte order
     */
    UInt16 GetPortInHostByteOrder( void ) const;
    
    void SetAddress( const UInt32 address );
    
    /**
     *  Address in network byte order
     */
    UInt32 GetAddress( void ) const;
    
    void SetAddressInHostByteOrder( const UInt32 address );
    
    /**
     *  Address in host byte order
     */
    UInt32 GetAddressInHostByteOrder( void ) const;
    
    /**
     *  Address as a string with the network to host
     *  conversion applied
     */
    CORE::CString AddressAsString( void ) const;
    
    /**
     *  Port as a string with the network to host
     *  conversion applied
     */
    CORE::CString PortAsString( void ) const;
    
    /**
     *  Address and port as a string with the network to host
     *  conversion applied
     */
    CORE::CString AddressAndPortAsString( void ) const;
    
    CIPAddress& operator=( const CIPAddress& src );
    
    bool operator==( const CIPAddress& other ) const;
    
    bool operator!=( const CIPAddress& other ) const;
    
    /**
     *  This operator is only implemented to facility ordering
     *  in STL containers. The return value has no real meaning
     *  except that of a binary data compare.
     */
    bool operator<( const CIPAddress& other ) const;
    
    private:
    
    bool ResolveDNS( const CORE::CString& address ,
                     const UInt16 port            );
    
    private:
    
    UInt16 m_port;
    UInt32 m_address;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CIPADDRESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-03-2007 :
        - Initial version, this used to be a struct

-----------------------------------------------------------------------------*/
