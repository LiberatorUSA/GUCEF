/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

    /**
     *  The host (DNS) and port together in a single string
     *  takes the forms:
     *      <DNS>:<Port>
     *      <IP>:<Port>
     */
    bool SetHostnameAndPort( const CORE::CString& hostAndPort );

    bool SetHostnameAndPort( const CORE::CString& host, UInt16 portInHostOrder );

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
