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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

#include <fstream>

#include "gucefCOMCORE_CLinuxNetworkInterface.h"

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_COMCORE_CCOM_H
#include "CCom.h"
#define GUCEF_COMCORE_CCOM_H
#endif /* GUCEF_COMCORE_CCOM_H ? */

#ifndef GUCEF_COMCORE_CCOMCOREGLOBAL_H
#include "gucefCOMCORE_CComCoreGlobal.h"
#define GUCEF_COMCORE_CCOMCOREGLOBAL_H
#endif /* GUCEF_COMCORE_CCOMCOREGLOBAL_H ? */

#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/rtnetlink.h>

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CString
GetDefaultLinuxNetworkGateway( void )
{GUCEF_TRACE;

    int sock = ::socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if ( sock < 0 )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GetDefaultLinuxNetworkGateway: socket failed" );
        return CString::Empty;
    }

    struct {
        struct nlmsghdr nlmsg;
        struct rtmsg rtmsg;
        char buf[8192];
    } request;

    memset(&request, 0, sizeof(request));
    request.nlmsg.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    request.nlmsg.nlmsg_type = RTM_GETROUTE;
    request.nlmsg.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    request.nlmsg.nlmsg_seq = 1;
    request.nlmsg.nlmsg_pid = getpid();
    request.rtmsg.rtm_family = AF_INET;

    if ( ::send( sock, &request, request.nlmsg.nlmsg_len, 0) < 0 )
    {
        close( sock );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GetDefaultLinuxNetworkGateway: socket send failed" );
        return CString::Empty;
    }

    char buffer[8192];
    int len = ::recv( sock, buffer, sizeof(buffer), 0 );
    if ( len < 0 )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GetDefaultLinuxNetworkGateway: socket recv failed" );
        ::close( sock );
        return CString::Empty;
    }

    struct nlmsghdr *nlh = (struct nlmsghdr *)buffer;
    for (; NLMSG_OK(nlh, len); nlh = NLMSG_NEXT(nlh, len))
    {
        if (nlh->nlmsg_type == NLMSG_DONE)
        {
            break;
        }
        if (nlh->nlmsg_type == NLMSG_ERROR)
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GetDefaultLinuxNetworkGateway: Error in netlink message" );
            ::close( sock );
            return CString::Empty;
        }

        struct rtmsg* rtm = (struct rtmsg *)NLMSG_DATA(nlh);
        if (rtm->rtm_table != RT_TABLE_MAIN)
        {
            continue;
        }

        struct rtattr* rta = (struct rtattr *)RTM_RTA(rtm);
        int rta_len = RTM_PAYLOAD(nlh);
        for (; RTA_OK(rta, rta_len); rta = RTA_NEXT(rta, rta_len))
        {
            if (rta->rta_type == RTA_GATEWAY)
            {
                char gateway[INET_ADDRSTRLEN];
                ::inet_ntop(AF_INET, RTA_DATA(rta), gateway, sizeof(gateway));
                ::close(sock);
                return CString(gateway);
            }
        }
    }

    ::close(sock);
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GetDefaultLinuxNetworkGateway: No default gateway found" );
    return CString::Empty;
}

/*-------------------------------------------------------------------------*/

CORE::CString
GetDhcpLeaseTimestampFromSystemdJournal( const CORE::CString& interfaceName )
{GUCEF_TRACE;

    char buffer[128];
    std::string result;
    std::string command = "journalctl -u systemd-networkd -u NetworkManager --no-pager | grep 'DHCPv4 lease acquired' | grep '" + interfaceName.STL_String() + "'";
    FILE* pipe = ::popen( command.c_str(), "r" );
    if ( GUCEF_NULL == pipe )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GetDhcpLeaseTimestampFromSystemdJournal: popen() failed" );
        return CString::Empty;
    }

    try
    {
        while ( fgets(buffer, sizeof(buffer), pipe) != GUCEF_NULL )
        {
            result += buffer;
        }
    }
    catch (...)
    {
        ::pclose( pipe );
        return CString::Empty;
    }
    ::pclose( pipe );

    if ( result.empty() )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GetDhcpLeaseTimestampFromSystemdJournal: No DHCP lease timestamp found" );
        return CString::Empty;
    }

    // Extract the timestamp from the log entry
    std::string::size_type pos = result.find(" ");
    if ( pos != std::string::npos )
    {
        return result.substr(0, pos);
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GetDhcpLeaseTimestampFromSystemdJournal: No DHCP lease timestamp found" );
    return CString::Empty;
}

/*-------------------------------------------------------------------------*/

struct EtcNetworkInterface
{
    CString iface;
    CString address;
    CString netmask;
    CString gateway;
};
typedef std::map< CString, struct EtcNetworkInterface >    TStringToNetworkInterface;

/*-------------------------------------------------------------------------*/

//  Debian-based systems:
//     The traditional location is /etc/network/interfaces.
void
ParseEtcNetworkInterfacesFile( const CString& filePath                      ,
                               std::vector<EtcNetworkInterface>& interfaces )
{GUCEF_TRACE;

    std::ifstream file( filePath.C_String() );
    if ( !file.is_open() )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "ParseEtcNetworkInterfacesFile: Unable to open file " + filePath );
        return;
    }

    std::string line;
    EtcNetworkInterface currentInterface;
    bool inInterfaceBlock = false;

    while (std::getline(file, line))
    {
        if (line.find("iface") == 0)
        {
            if (inInterfaceBlock)
            {
                interfaces.push_back(currentInterface);
                currentInterface = EtcNetworkInterface();
            }
            inInterfaceBlock = true;
            currentInterface.iface = line;
        }
        else
        if (line.find("address") == 0)
        {
            currentInterface.address = line;
        }
        else
        if (line.find("netmask") == 0)
        {
            currentInterface.netmask = line;
        }
        else
        if (line.find("gateway") == 0)
        {
            currentInterface.gateway = line;
        }
    }

    if (inInterfaceBlock)
    {
        interfaces.push_back(currentInterface);
    }

    file.close();
}

/*-------------------------------------------------------------------------*/

void
ParseEtcNetworkInterfacesFile( const CString& filePath               ,
                               TStringToNetworkInterface& interfaces )
{GUCEF_TRACE;

    std::vector<EtcNetworkInterface> interfacesVec;
    ParseEtcNetworkInterfacesFile( filePath, interfacesVec );

    if ( !interfacesVec.empty() )
    {
        std::vector<EtcNetworkInterface>::iterator i = interfacesVec.begin();
        while ( i != interfacesVec.end() )
        {
            const EtcNetworkInterface& nic = (*i);
            interfaces[ nic.iface ] = nic;
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CLinuxNetworkInterface::EnumNetworkAdapters( TINetworkInterfacePtrVector& interfaces )
{GUCEF_TRACE;   

    TStringToNetworkInterface etcInterfaces;
    ParseEtcNetworkInterfacesFile( "/etc/network/interfaces", etcInterfaces );

    struct ifaddrs* ifaddr = GUCEF_NULL;
    struct ifaddrs* ifa = GUCEF_NULL;    

    // Get the list of network interfaces
    if ( ::getifaddrs( &ifaddr ) == -1)
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: getifaddrs failed" );
        return false;
    }

    // Iterate through the list of interfaces
    typedef std::map< CString, CLinuxNetworkInterfacePtr > TNicMap;
    TNicMap nicMap;
    for ( ifa = ifaddr; ifa != GUCEF_NULL; ifa = ifa->ifa_next )
    {
        if ( ifa->ifa_addr == GUCEF_NULL)
        {
            continue;
        }

        int family = ifa->ifa_addr->sa_family;

        // Check if the interface is an Ethernet interface
        if ( family == AF_INET || family == AF_INET6 )
        {
            if ( ifa->ifa_flags & IFF_LOOPBACK )
            {
                continue; // Skip loopback interfaces
            }

            // Get the IP address
            bool getnameinfoSuccess = true;
            CORE::CString host;
            host.Reserve( (UInt32) NI_MAXHOST );
            int s = 0;
            do
            {
                s = ::getnameinfo( ifa->ifa_addr,
                                   (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                         sizeof(struct sockaddr_in6),
                                   (char*)host.C_String(),
                                   host.ByteSize(),
                                   GUCEF_NULL,
                                   0,
                                   NI_NUMERICHOST  );
                if ( s == 0 )
                {
                    getnameinfoSuccess = true;

                    // this function provides no way to know how many bytes were written.
                    // only that it wont overflow the input buffer.
                    // As such we need to consider the entire buffer as valid data
                    host.DetermineLength();
                }
                else
                {
                    getnameinfoSuccess = false;
                    if ( s == EAI_OVERFLOW )
                    {
                        // just try again with a larger buffer
                        if ( GUCEF_NULL == host.Reserve( host.ByteSize() * 2 ) )
                            break; // out of memory ?
                    }
                    else
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: getnameinfo failed with error code " + CORE::ToString( s ) );
                        s = 0;
                        break;
                    }
                }
            }
            while ( s != 0 );

            if ( !getnameinfoSuccess )
                continue; // no joy, try a different entry if any

            // the same nic id can be encountered multiple times in the list
            // for example once for IPv4 and once for IPv6
            // as such we need to check if we already created a nic with the given id
            CLinuxNetworkInterfacePtr nic;
            TNicMap::iterator n = nicMap.find( host );
            if ( n == nicMap.end() )
            {
                nic = CLinuxNetworkInterfacePtr( GUCEF_NEW CLinuxNetworkInterface() );
                nicMap[ host ] = nic;

                nic->m_name = ifa->ifa_name;
                nic->m_nicIndex = ::if_nametoindex( ifa->ifa_name );

                // Linux typically only has 1 global network gateway
                nic->m_defGateway.SetAddress( GetDefaultLinuxNetworkGateway() );

                CString dhcpLeaseStr = GetDhcpLeaseTimestampFromSystemdJournal( nic->m_name );
                if ( !dhcpLeaseStr.IsNULLOrEmpty() )
                {
                    int fg=0; // @TODO
                }

                TStringToNetworkInterface::iterator m = etcInterfaces.find( nic->m_name );
                if ( m != etcInterfaces.end() )
                {
                    const EtcNetworkInterface& etcNic = (*m).second;

                    CIPv4Address nicGateway;
                    if ( nicGateway.SetAddress( etcNic.gateway ) )
                    {
                        nic->m_gatewayList.push_back( nicGateway );
                    }
                }
            }
            if ( nic.IsNULL() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "LinuxNetworkInterface:EnumNetworkAdapters: No nic object" );
                continue;
            }

            // Now start filling in information for the NIC

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: found interface \"" +
                nic->m_name + "\" host " + host + " with index " + CORE::ToString( nic->m_nicIndex ) );

            int fd = ::socket( family, SOCK_DGRAM, 0 );
            if ( family == AF_INET )
            {
                CIPv4Address ipv4;
                if ( ipv4.SetAddress( host ) )
                {
                    nic->m_curIpAddr.ip = ipv4;

                    // Get additional IPv4 information via ioctl()

                    if ( fd != -1)
                    {
                        struct ifreq ifr;
                        ::strncpy( ifr.ifr_name, nic->m_name.C_String(), GUCEF_SMALLEST( IFNAMSIZ-1, nic->m_name.ByteSize() ) );
                        ifr.ifr_name[ IFNAMSIZ-1 ] = '\0';

                        int ioctlResult = ::ioctl( fd, SIOCGIFNETMASK, &ifr );
                        if ( ioctlResult != -1 )
                        {
                            struct sockaddr_in* netmask = (struct sockaddr_in*) &ifr.ifr_netmask;
                            char netmask_str[ INET_ADDRSTRLEN ];
                            ::inet_ntop( AF_INET, &netmask->sin_addr, netmask_str, INET_ADDRSTRLEN );

                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: Subnet Mask: " + CString( netmask_str ) );

                            // Calculate the subnet
                            struct in_addr ip_addr, subnet_mask, subnet;
                            ip_addr.s_addr = ipv4.GetAddressInHostByteOrder();

                            ::inet_pton( AF_INET, netmask_str, &subnet_mask );
                            subnet.s_addr = ip_addr.s_addr & subnet_mask.s_addr;

                            char subnet_str[INET_ADDRSTRLEN];
                            ::inet_ntop( AF_INET, &subnet, subnet_str, INET_ADDRSTRLEN );

                            CIPv4Address subnetIpv4;
                            subnetIpv4.SetAddress( subnet.s_addr );

                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: Subnet: " + ToString( subnetIpv4 ) );
                            nic->m_curIpAddr.subnet = subnetIpv4;
                        }
                        else
                        {
                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: ioctl has error getting SIOCGIFNETMASK: " + CString( errno ) );
                        }
                    }
                }
                else
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: Failed to parse IPv4 of interface \"" + nic->m_name + "\" host " + host );
                }

            }
            else
            {
                CIPv6Address ipv6;
                //if ( ipv6 )
                    // @TODO
            }

            // Get additional information via ioctl()

            struct ifreq ifr;
            ::strncpy( ifr.ifr_name, nic->m_name.C_String(), GUCEF_SMALLEST( IFNAMSIZ-1, nic->m_name.ByteSize() ) );
            ifr.ifr_name[ IFNAMSIZ-1 ] = '\0';

            int ioctlResult = ::ioctl( fd, SIOCGIFHWADDR, &ifr );
            if ( ioctlResult != -1 )
            {
                unsigned char* mac = reinterpret_cast< unsigned char* >( ifr.ifr_hwaddr.sa_data );

                char macAddress[ 24 ];
                sprintf( macAddress, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );
                CString macAddressStr = macAddress;

                nic->m_macAddrs.insert( macAddressStr );
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: MAC: " + macAddressStr );
            }
            else
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface:EnumNetworkAdapters: ioctl has error getting SIOCGIFHWADDR: " + CString( errno ) );
            }

            ::close( fd );

            interfaces.push_back( nic.StaticCast< CINetworkInterface >() );

        }
    }

    // Free the memory allocated by getifaddrs
    ::freeifaddrs( ifaddr );
    ifaddr = GUCEF_NULL;






   /*
    CORE::CString content;
    if ( LoadTextFileAsString( "/proc/net/dev", content, true, "\n" ) )
    {
        CORE::CString::StringVector lines = content.ParseElements( '\n', false );
        CORE::CString::StringVector::iterator i = lines.begin();

        // Skip the first two lines (headers)
        ++i; ++i;

        while ( i != lines.end() )
        {
            const CORE::CString& line = (*i);
            CORE::CString interfaceName;

            Int32 colonIndex = line.HasChar( ':', true );
            if ( colonIndex > 0 )
            {
                interfaceName = line.SubstrToIndex( colonIndex, true ).Trim( true );
                if ( !interfaceName.IsNULLOrEmpty() )
                {
                    CLinuxNetworkInterfacePtr nic = CLinuxNetworkInterfacePtr( GUCEF_NEW CLinuxNetworkInterface() );
                    nic->m_name = interfaceName;

                    interfaces.push_back( nic.StaticCast< CINetworkInterface >() );
                }
            }
            ++i;
        }
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "LinuxNetworkInterface: Failed to obtain info from /proc/net/dev" );
    return false;
}
   /*
        std::istringstream iss(line);
        std::string interface;
        iss >> interface;

        // Remove the colon at the end of the interface name
        if (interface.back() == ':') {
            interface.pop_back();
        }

        if (interface == iface_name) {
            unsigned long rx_bytes, rx_packets, rx_errs, rx_drop, rx_fifo, rx_frame, rx_compressed, rx_multicast;
            unsigned long tx_bytes, tx_packets, tx_errs, tx_drop, tx_fifo, tx_colls, tx_carrier, tx_compressed;

            iss >> rx_bytes >> rx_packets >> rx_errs >> rx_drop >> rx_fifo >> rx_frame >> rx_compressed >> rx_multicast
                >> tx_bytes >> tx_packets >> tx_errs >> tx_drop >> tx_fifo >> tx_colls >> tx_carrier >> tx_compressed;

            std::cout << "Statistics for interface: " << iface_name << std::endl;
            std::cout << std::setw(15) << "RX Bytes: " << rx_bytes << std::endl;
            std::cout << std::setw(15) << "RX Packets: " << rx_packets << std::endl;
            std::cout << std::setw(15) << "RX Errors: " << rx_errs << std::endl;
            std::cout << std::setw(15) << "RX Dropped: " << rx_drop << std::endl;
            std::cout << std::setw(15) << "TX Bytes: " << tx_bytes << std::endl;
            std::cout << std::setw(15) << "TX Packets: " << tx_packets << std::endl;
            std::cout << std::setw(15) << "TX Errors: " << tx_errs << std::endl;
            std::cout << std::setw(15) << "TX Dropped: " << tx_drop << std::endl;

            return;
        }
    }

    std::cerr << "Interface " << iface_name << " not found." << std::endl;
}


*/
	return true;
}

/*-------------------------------------------------------------------------*/

CLinuxNetworkInterface::CLinuxNetworkInterface( void )
    : CINetworkInterface()
    , m_name()
    , m_desc()
    , m_macAddrs()
    , m_priWins()
    , m_secWins()
    , m_defGateway()
    , m_dhcpAddr()
    , m_curIpAddr()
    , m_nicIndex( 0 )
    , m_adapterType( 0 )
    , m_dhcpUsed( false )
    , m_winsUsed( false )
    , m_dnsAddresses()
    , m_ipAddresses()
    , m_gatewayList()
    , m_leaseObtained( 0 )
    , m_leaseExpires( 0 )
	, m_dataLock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CLinuxNetworkInterface::~CLinuxNetworkInterface()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CLinuxNetworkInterface::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::COMCORE::CLinuxNetworkInterface";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CLinuxNetworkInterface::GetCommunicationInterfaceType( void ) const
{GUCEF_TRACE;

    return "network";
}

/*-------------------------------------------------------------------------*/

CString
CLinuxNetworkInterface::GetAdapterName( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_name;
}

/*-------------------------------------------------------------------------*/

CString
CLinuxNetworkInterface::GetAdapterDescription( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_desc;
}

/*-------------------------------------------------------------------------*/

UInt32
CLinuxNetworkInterface::GetNrOfDnsAddresses( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return (UInt32) m_dnsAddresses.size();
}

/*-------------------------------------------------------------------------*/

bool
CLinuxNetworkInterface::GetDnsAddresses( THostAddressVector& dnsAddresses )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	THostAddressVector::const_iterator i = m_dnsAddresses.begin();
	while ( i != m_dnsAddresses.end() )
	{
		dnsAddresses.push_back( (*i) );
		++i;
	}
	return true;
}

/*-------------------------------------------------------------------------*/

UInt32
CLinuxNetworkInterface::GetNrOfIPAddresses( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return (UInt32) m_ipAddresses.size();
}

/*-------------------------------------------------------------------------*/

bool
CLinuxNetworkInterface::GetIPInfo( TIPInfoVector& ipInfo, bool includeUninitialized ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	TIPInfoVector::const_iterator i = m_ipAddresses.begin();
	while ( i != m_ipAddresses.end() )
	{
		if ( includeUninitialized || 0 != (*i).ip.GetAddress() )
			ipInfo.push_back( (*i) );
		++i;
	}
	return true;
}

/*-------------------------------------------------------------------------*/

bool
CLinuxNetworkInterface::IsDhcpUsed( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_dhcpUsed;
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime
CLinuxNetworkInterface::GetDhcpLeaseObtainedTime( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	CORE::CDateTime result;
	result.FromUnixEpochBasedTicksInMillisecs( m_leaseObtained * 1000 );
	return result;
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime
CLinuxNetworkInterface::GetDhcpLeaseExpirationTime( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	CORE::CDateTime result;
	result.FromUnixEpochBasedTicksInMillisecs( m_leaseExpires * 1000 );
	return result;
}

/*-------------------------------------------------------------------------*/

bool
CLinuxNetworkInterface::ReleaseAddress( void )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

bool
CLinuxNetworkInterface::RenewAddress( void )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

bool
CLinuxNetworkInterface::IsWinsUsed( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_winsUsed;
}

/*-------------------------------------------------------------------------*/

CIPv4Address
CLinuxNetworkInterface::GetPrimaryWinsServer( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_priWins;
}

/*-------------------------------------------------------------------------*/

CIPv4Address
CLinuxNetworkInterface::GetSecondaryWinsServer( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_secWins;
}

/*-------------------------------------------------------------------------*/

UInt32
CLinuxNetworkInterface::GetOsAdapterIndex( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_nicIndex;
}

/*-------------------------------------------------------------------------*/

bool
CLinuxNetworkInterface::GetMetrics( CNetworkInterfaceMetrics& metrics ) const
{GUCEF_TRACE;
     /*
	if ( GUCEF_NULL != COMCORE::GetIfEntry2 )
	{
		MT::CObjectScopeLock lock( this );

		// Make sure the struct is zeroed out
		COMCORE::MIB_IF_ROW2 interfaceInfo;
		SecureZeroMemory((PVOID) &interfaceInfo, sizeof(COMCORE::MIB_IF_ROW2) );
		interfaceInfo.InterfaceIndex = m_nicIndex;

		lock.EarlyUnlock();

		DWORD queryResult = COMCORE::GetIfEntry2( &interfaceInfo );
		if ( queryResult == NO_ERROR )
		{
			metrics.inboundOctets = interfaceInfo.InOctets;
			metrics.hasInboundOctets = true;
			metrics.inboundErroredPackets = interfaceInfo.InErrors;
			metrics.hasInboundErroredPackets = true;
			metrics.inboundDiscardedPackets = interfaceInfo.InDiscards;
			metrics.hasInboundDiscardedPackets = true;
			metrics.inboundUnknownProtocolPackets = interfaceInfo.InUnknownProtos;
			metrics.hasInboundUnknownProtocolPackets = true;
			metrics.inboundUnicastPackets = interfaceInfo.InUcastPkts;
			metrics.hasInboundUnicastPackets = true;
			metrics.outboundErroredPackets = interfaceInfo.OutErrors;
			metrics.hasOutboundErroredPackets = true;
			metrics.outboundDiscardedPackets = interfaceInfo.OutDiscards;
			metrics.hasOutboundDiscardedPackets = true;
			metrics.outboundOctets = interfaceInfo.OutOctets;
			metrics.hasOutboundOctets = true;
			metrics.outboundUnicastPackets = interfaceInfo.OutUcastPkts;
			metrics.hasOutboundUnicastPackets = true;

			metrics.transmitLinkSpeedBitsPerSec = interfaceInfo.TransmitLinkSpeed;
			metrics.hasTransmitLinkSpeedBitsPerSec = true;
			metrics.receiveLinkSpeedBitsPerSec = interfaceInfo.ReceiveLinkSpeed;
			metrics.hasReceiveLinkSpeedBitsPerSec = true;
			metrics.outboundMulticastOctets = interfaceInfo.OutMulticastOctets;
			metrics.hasOutboundMulticastOctets = true;
			metrics.inboundMulticastOctets = interfaceInfo.InMulticastOctets;
			metrics.hasInboundMulticastOctets = true;
			metrics.outboundBroadcastOctets = interfaceInfo.OutBroadcastOctets;
			metrics.hasOutboundBroadcastOctets = true;
			metrics.inboundBroadcastOctets = interfaceInfo.InBroadcastOctets;
			metrics.hasInboundBroadcastOctets = true;
			metrics.outboundNonUnicastPackets = interfaceInfo.OutNUcastPkts;
			metrics.hasOutboundNonUnicastPackets = true;
			metrics.inboundNonUnicastPackets = interfaceInfo.InNUcastPkts;
			metrics.hasInboundNonUnicastPackets = true;

			return true;
		}
		else
		{
			GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CLinuxNetworkInterface: Failed to obtain adapter stats using Win32 GetIfEntry2()" );
		}
	}
	else
	{
		MT::CObjectScopeLock lock( this );

		// Make sure the struct is zeroed out
		::MIB_IFROW interfaceInfo;
		SecureZeroMemory((PVOID) &interfaceInfo, sizeof(::MIB_IFROW) );
		interfaceInfo.dwIndex = m_nicIndex;

		lock.EarlyUnlock();

		DWORD queryResult = ::GetIfEntry( &interfaceInfo );
		if ( queryResult == NO_ERROR )
		{
			metrics.inboundOctets = interfaceInfo.dwInOctets;
			metrics.hasInboundOctets = true;
			metrics.inboundErroredPackets = interfaceInfo.dwInErrors;
			metrics.hasInboundErroredPackets = true;
			metrics.inboundDiscardedPackets = interfaceInfo.dwInDiscards;
			metrics.hasInboundDiscardedPackets = true;
			metrics.inboundUnknownProtocolPackets = interfaceInfo.dwInUnknownProtos;
			metrics.hasInboundUnknownProtocolPackets = true;
			metrics.inboundUnicastPackets = interfaceInfo.dwInUcastPkts;
			metrics.hasInboundUnicastPackets = true;
			metrics.outboundErroredPackets = interfaceInfo.dwOutErrors;
			metrics.hasOutboundErroredPackets = true;
			metrics.outboundDiscardedPackets = interfaceInfo.dwOutDiscards;
			metrics.hasOutboundDiscardedPackets = true;
			metrics.outboundOctets = interfaceInfo.dwOutOctets;
			metrics.hasOutboundOctets = true;
			metrics.outboundUnicastPackets = interfaceInfo.dwOutUcastPkts;
			metrics.hasOutboundUnicastPackets = true;

			return true;
		}
		else
		{
			GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CLinuxNetworkInterface: Failed to obtain adapter stats using Win32 GetIfEntry()" );
		}
	}   */
	return false;
}

/*-------------------------------------------------------------------------*/

const MT::CILockable*
CLinuxNetworkInterface::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CLinuxNetworkInterface::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_dataLock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CLinuxNetworkInterface::Unlock( void ) const
{GUCEF_TRACE;

    return m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */
