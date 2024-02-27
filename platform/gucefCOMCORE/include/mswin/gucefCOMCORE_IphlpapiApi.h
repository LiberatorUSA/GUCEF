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

/*
 *  Wrapper for the MS Windows IPHLP API that allows dynamic linking with IPHLP
 *  functions preventing getting tied to a specific version of the MS Windows O/S
 */
                          
#ifndef GUCEF_COMCORE_IPHLPAPI_H
#define GUCEF_COMCORE_IPHLPAPI_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"       /* build defines */
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace COMCORE {
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

//
// Physical Medium Type definitions. Used with OID_GEN_PHYSICAL_MEDIUM.
//
typedef enum _NDIS_PHYSICAL_MEDIUM
{
    NdisPhysicalMediumUnspecified,
    NdisPhysicalMediumWirelessLan,
    NdisPhysicalMediumCableModem,
    NdisPhysicalMediumPhoneLine,
    NdisPhysicalMediumPowerLine,
    NdisPhysicalMediumDSL,      // includes ADSL and UADSL (G.Lite)
    NdisPhysicalMediumFibreChannel,
    NdisPhysicalMedium1394,
    NdisPhysicalMediumWirelessWan,
    NdisPhysicalMediumNative802_11,
    NdisPhysicalMediumBluetooth,
    NdisPhysicalMediumInfiniband,
    NdisPhysicalMediumWiMax,
    NdisPhysicalMediumUWB,
    NdisPhysicalMedium802_3,
    NdisPhysicalMedium802_5,
    NdisPhysicalMediumIrda,
    NdisPhysicalMediumWiredWAN,
    NdisPhysicalMediumWiredCoWan,
    NdisPhysicalMediumOther,
    NdisPhysicalMediumNative802_15_4,
    NdisPhysicalMediumMax       // Not a real physical type, defined as an upper-bound
} NDIS_PHYSICAL_MEDIUM, *PNDIS_PHYSICAL_MEDIUM;

/*-------------------------------------------------------------------------*/

//
// Medium the Ndis Driver is running on (OID_GEN_MEDIA_SUPPORTED/ OID_GEN_MEDIA_IN_USE).
//
typedef enum _NDIS_MEDIUM
{
    NdisMedium802_3,
    NdisMedium802_5,
    NdisMediumFddi,
    NdisMediumWan,
    NdisMediumLocalTalk,
    NdisMediumDix,              // defined for convenience, not a real medium
    NdisMediumArcnetRaw,
    NdisMediumArcnet878_2,
    NdisMediumAtm,
    NdisMediumWirelessWan,
    NdisMediumIrda,
    NdisMediumBpc,
    NdisMediumCoWan,
    NdisMedium1394,
    NdisMediumInfiniBand,
#if ((NTDDI_VERSION >= NTDDI_VISTA) || NDIS_SUPPORT_NDIS6)
    NdisMediumTunnel,
    NdisMediumNative802_11,
    NdisMediumLoopback,
#endif // (NTDDI_VERSION >= NTDDI_VISTA)

#if (NTDDI_VERSION >= NTDDI_WIN7)
    NdisMediumWiMAX,
    NdisMediumIP,
#endif

    NdisMediumMax               // Not a real medium, defined as an upper-bound
} NDIS_MEDIUM, *PNDIS_MEDIUM;

/*-------------------------------------------------------------------------*/

//
// Types of tunnels (sub-type of IF_TYPE when IF_TYPE is IF_TYPE_TUNNEL).
// See http://www.iana.org/assignments/ianaiftype-mib.
//
typedef enum {
    TUNNEL_TYPE_NONE = 0,
    TUNNEL_TYPE_OTHER = 1,
    TUNNEL_TYPE_DIRECT = 2,
    TUNNEL_TYPE_6TO4 = 11,
    TUNNEL_TYPE_ISATAP = 13,
    TUNNEL_TYPE_TEREDO = 14,
    TUNNEL_TYPE_IPHTTPS = 15,
} TUNNEL_TYPE, *PTUNNEL_TYPE;

/*-------------------------------------------------------------------------*/

//
// OperStatus values from RFC 2863
//
typedef enum {
    IfOperStatusUp = 1,
    IfOperStatusDown,
    IfOperStatusTesting,
    IfOperStatusUnknown,
    IfOperStatusDormant,
    IfOperStatusNotPresent,
    IfOperStatusLowerLayerDown
} IF_OPER_STATUS;

/*-------------------------------------------------------------------------*/

typedef enum _NET_IF_ADMIN_STATUS   // ifAdminStatus
{
    NET_IF_ADMIN_STATUS_UP = 1,
    NET_IF_ADMIN_STATUS_DOWN = 2,
    NET_IF_ADMIN_STATUS_TESTING = 3
} NET_IF_ADMIN_STATUS, *PNET_IF_ADMIN_STATUS;

/*-------------------------------------------------------------------------*/

typedef enum _NET_IF_MEDIA_CONNECT_STATE
{
    MediaConnectStateUnknown,
    MediaConnectStateConnected,
    MediaConnectStateDisconnected
} NET_IF_MEDIA_CONNECT_STATE, *PNET_IF_MEDIA_CONNECT_STATE;

/*-------------------------------------------------------------------------*/

//
// Define datalink interface access types.
//
typedef enum _NET_IF_ACCESS_TYPE
{
    NET_IF_ACCESS_LOOPBACK = 1,
    NET_IF_ACCESS_BROADCAST = 2,
    NET_IF_ACCESS_POINT_TO_POINT = 3,
    NET_IF_ACCESS_POINT_TO_MULTI_POINT = 4,
    NET_IF_ACCESS_MAXIMUM = 5
} NET_IF_ACCESS_TYPE, *PNET_IF_ACCESS_TYPE;

/*-------------------------------------------------------------------------*/

//
// Define datalink interface direction types.
//
typedef enum _NET_IF_DIRECTION_TYPE
{
    NET_IF_DIRECTION_SENDRECEIVE,
    NET_IF_DIRECTION_SENDONLY,
    NET_IF_DIRECTION_RECEIVEONLY,
    NET_IF_DIRECTION_MAXIMUM
} NET_IF_DIRECTION_TYPE, *PNET_IF_DIRECTION_TYPE;

/*-------------------------------------------------------------------------*/

typedef enum _NET_IF_CONNECTION_TYPE
{
   NET_IF_CONNECTION_DEDICATED = 1,
   NET_IF_CONNECTION_PASSIVE = 2,
   NET_IF_CONNECTION_DEMAND = 3,
   NET_IF_CONNECTION_MAXIMUM = 4
} NET_IF_CONNECTION_TYPE, *PNET_IF_CONNECTION_TYPE;

/*-------------------------------------------------------------------------*/

//
// maximum string size in -wchar- units
//
#define IF_MAX_STRING_SIZE 256

#define IF_MAX_PHYS_ADDRESS_LENGTH 32

/*-------------------------------------------------------------------------*/

typedef ULONG NET_IFINDEX, *PNET_IFINDEX;       // Interface Index (ifIndex)
typedef UINT16 NET_IFTYPE, *PNET_IFTYPE;        // Interface Type (IANA ifType)

typedef ULONG IFTYPE;

//
// Define NetworkGUID type:
//
typedef GUID NET_IF_NETWORK_GUID, *PNET_IF_NETWORK_GUID;

/*-------------------------------------------------------------------------*/

#pragma warning(push)
#pragma warning(disable:4214) // bit field types other than int
typedef union _NET_LUID_LH
{
    ULONG64     Value;
    struct
    {
        ULONG64     Reserved:24;
        ULONG64     NetLuidIndex:24;
        ULONG64     IfType:16;                  // equal to IANA IF type
    }Info;
} NET_LUID_LH, *PNET_LUID_LH;
#pragma warning(pop)

/*-------------------------------------------------------------------------*/

//
// Need to make this visible on all platforms (for the purpose of IF_LUID).
//
typedef NET_LUID_LH NET_LUID;
typedef NET_LUID* PNET_LUID;

/*-------------------------------------------------------------------------*/

typedef struct _MIB_IF_ROW2 {
    //
    // Key structure.  Sorted by preference.
    //
    NET_LUID InterfaceLuid;
    NET_IFINDEX InterfaceIndex;

    //
    // Read-Only fields.
    //
    GUID InterfaceGuid;
    WCHAR Alias[IF_MAX_STRING_SIZE + 1];
    WCHAR Description[IF_MAX_STRING_SIZE + 1];
    ULONG PhysicalAddressLength;
    UCHAR PhysicalAddress[IF_MAX_PHYS_ADDRESS_LENGTH];
    UCHAR PermanentPhysicalAddress[IF_MAX_PHYS_ADDRESS_LENGTH];

    ULONG Mtu;
    IFTYPE Type;                // Interface Type.
    TUNNEL_TYPE TunnelType;     // Tunnel Type, if Type = IF_TUNNEL.
    NDIS_MEDIUM MediaType;
    NDIS_PHYSICAL_MEDIUM PhysicalMediumType;
    NET_IF_ACCESS_TYPE AccessType;
    NET_IF_DIRECTION_TYPE DirectionType;
    struct {
        BOOLEAN HardwareInterface : 1;
        BOOLEAN FilterInterface : 1;
        BOOLEAN ConnectorPresent : 1;
        BOOLEAN NotAuthenticated : 1;
        BOOLEAN NotMediaConnected : 1;
        BOOLEAN Paused : 1;
        BOOLEAN LowPower : 1;
        BOOLEAN EndPointInterface : 1;
    } InterfaceAndOperStatusFlags;

    IF_OPER_STATUS OperStatus;
    NET_IF_ADMIN_STATUS AdminStatus;
    NET_IF_MEDIA_CONNECT_STATE MediaConnectState;
    NET_IF_NETWORK_GUID NetworkGuid;
    NET_IF_CONNECTION_TYPE ConnectionType;

    //
    // Statistics.
    //
    ULONG64 TransmitLinkSpeed;
    ULONG64 ReceiveLinkSpeed;

    ULONG64 InOctets;
    ULONG64 InUcastPkts;
    ULONG64 InNUcastPkts;
    ULONG64 InDiscards;
    ULONG64 InErrors;
    ULONG64 InUnknownProtos;
    ULONG64 InUcastOctets;
    ULONG64 InMulticastOctets;
    ULONG64 InBroadcastOctets;
    ULONG64 OutOctets;
    ULONG64 OutUcastPkts;           /* The number of octets of data transmitted without errors in unicast packets through this interface. */
    ULONG64 OutNUcastPkts;
    ULONG64 OutDiscards;
    ULONG64 OutErrors;
    ULONG64 OutUcastOctets;
    ULONG64 OutMulticastOctets;     /* The number of octets of data transmitted without errors in multicast packets through this interface. */
    ULONG64 OutBroadcastOctets;     /* The number of octets of data transmitted without errors in broadcast packets through this interface. */
    ULONG64 OutQLen;                /* The transmit queue length. This field is not currently used. */
} MIB_IF_ROW2, *PMIB_IF_ROW2;

/*-------------------------------------------------------------------------*/

typedef DWORD ( WINAPI *TGetIfEntry2Ptr )( PMIB_IF_ROW2 Row );

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*-------------------------------------------------------------------------*/

GUCEF_COMCORE_EXPORT_C 
Int32 LinkLPHLPAPI( void );

/*-------------------------------------------------------------------------*/

GUCEF_COMCORE_EXPORT_C
void UnlinkLPHLPAPI( void );

/*-------------------------------------------------------------------------*/

/*
 *  Iphlpapi function pointers
 */
GUCEF_COMCORE_EXPORT_C extern TGetIfEntry2Ptr GetIfEntry2;

#ifdef __cplusplus

GUCEF_COMCORE_EXPORT_CPP extern MT::CMutex win32ApiLinkerMutex;

#endif /* __cplusplus */

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
} /* namespace COMCORE ? */
} /* namespace GUCEF ? */
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_IPHLPAPI_H ? */
