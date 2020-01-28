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

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#define GUCEF_COMCORE_CHOSTADDRESS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CIPADDRESS_H
#include "CIPAddress.h"      /* macros and build config for the COMCORE library */
#define GUCEF_COMCORE_CIPADDRESS_H
#endif /* GUCEF_COMCORE_CIPADDRESS_H ? */

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
 *  Class representing an internet address with host name in string form
 *  plus the IPv4 address with a port number included.
 *  Note that this class uses network byte order as its default.
 */
class GUCEF_COMCORE_EXPORT_CPP CHostAddress : public CIPAddress
{
    public:
    
    CHostAddress( void );
    
    CHostAddress( const CHostAddress& src );

    /**
     *  The port value is expected to be in host byte order
     */
    CHostAddress( const CORE::CString& hostname ,
                  const UInt16 port             );

    /**
     *  The host (DNS) and port together in a single string
     *  takes the forms:
     *      <DNS>:<Port>
     *      <IP>:<Port>
     */
    CHostAddress( const CORE::CString& hostAndPort );
                    
    CHostAddress( const CIPAddress& ipAddress   ,
                  const CORE::CString& hostname );

    virtual ~CHostAddress();
    
    bool SetHostname( const CORE::CString& hostName );

    const CORE::CString& GetHostname( void ) const;

    /**
     *  Refreshes the DNS resolution of the hostname
     */
    virtual bool Refresh( void );

    /**
     *  Address and port as a string with the network to host
     *  conversion applied
     */
    virtual CORE::CString HostnameAndPortAsString( void ) const;
    
    CHostAddress& operator=( const CHostAddress& src );

    CHostAddress& operator=( const CIPAddress& src );
    
    bool operator==( const CHostAddress& other ) const;
    
    bool operator!=( const CHostAddress& other ) const;
    
    /**
     *  This operator is only implemented to facilitate ordering
     *  in STL containers. The return value has no real meaning
     *  except that of a binary data compare.
     */
    bool operator<( const CHostAddress& other ) const;
    
    protected:
    
    virtual void OnChange( const bool addressChanged ,
                           const bool portChanged    );
    
    private:
    
    CORE::CString m_hostname;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-04-2007 :
        - Initial version

-----------------------------------------------------------------------------*/
