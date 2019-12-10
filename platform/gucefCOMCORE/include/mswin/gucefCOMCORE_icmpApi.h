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

/*
 *  Wrapper for the MS Windows ICMP API that allows dynamic linking with ICMP
 *  functions preventing getting tied to a specific version of the MS Windows O/S
 */
 
#ifndef GUCEF_COMCORE_ICMPAPI_H
#define GUCEF_COMCORE_ICMPAPI_H

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

typedef struct ip_option_information {
  UCHAR Ttl;
  UCHAR Tos;
  UCHAR Flags;
  UCHAR OptionsSize;
  PUCHAR OptionsData;
} IP_OPTION_INFORMATION, 
 *PIP_OPTION_INFORMATION;
 
/*-------------------------------------------------------------------------*/
 
typedef struct {
  union {
    struct {
      UCHAR s_b1,s_b2,s_b3,s_b4;
    } S_un_b;
    struct {
      USHORT s_w1,s_w2;
    } S_un_w;
    ULONG S_addr;
  } S_un;
} IPAddr;

/*-------------------------------------------------------------------------*/

typedef struct icmp_echo_reply {
  IPAddr Address;
  ULONG Status;
  ULONG RoundTripTime;
  USHORT DataSize;
  USHORT Reserved;
  PVOID Data;
  IP_OPTION_INFORMATION Options;
} ICMP_ECHO_REPLY, 
 *PICMP_ECHO_REPLY;

/*-------------------------------------------------------------------------*/

typedef struct _IO_STATUS_BLOCK {
    union {
        LONG Status;
        PVOID Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

/*-------------------------------------------------------------------------*/

/* For Vista and later: */
typedef
VOID 
(*PIO_APC_ROUTINE_NEW) (
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
    );
    
/* Older then Vista: */
typedef
VOID 
(*PIO_APC_ROUTINE_OLD) (
    IN PVOID ApcContext
    );

/*-------------------------------------------------------------------------*/

typedef HANDLE ( WINAPI *TIcmpCreateFilePtr )( VOID );
typedef BOOL ( WINAPI *TIcmpCloseHandlePtr )( HANDLE IcmpHandle );
typedef DWORD ( WINAPI *TIcmpParseRepliesPtr )( LPVOID ReplyBuffer, DWORD ReplySize );
typedef DWORD ( WINAPI *TIcmpSendEchoPtr )( HANDLE IcmpHandle, IPAddr DestinationAddress, LPVOID RequestData, WORD RequestSize, PIP_OPTION_INFORMATION RequestOptions, LPVOID ReplyBuffer, DWORD ReplySize, DWORD Timeout );
typedef DWORD ( WINAPI *TIcmpSendEcho2VistaPtr ) ( HANDLE IcmpHandle, HANDLE Event, PIO_APC_ROUTINE_NEW ApcRoutine, PVOID ApcContext, IPAddr DestinationAddress, LPVOID RequestData, WORD RequestSize, PIP_OPTION_INFORMATION RequestOptions, LPVOID ReplyBuffer, DWORD ReplySize, DWORD Timeout );
typedef DWORD ( WINAPI *TIcmpSendEcho2Ptr ) ( HANDLE IcmpHandle, HANDLE Event, PIO_APC_ROUTINE_OLD ApcRoutine, PVOID ApcContext, IPAddr DestinationAddress, LPVOID RequestData, WORD RequestSize, PIP_OPTION_INFORMATION RequestOptions, LPVOID ReplyBuffer, DWORD ReplySize, DWORD Timeout );

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  icmp_echo_reply.Status is one of the following values
 */

#define IP_SUCCESS                  11000
#define IP_BUF_TOO_SMALL            11001
#define IP_DEST_NET_UNREACHABLE     11002
#define IP_DEST_HOST_UNREACHABLE    11003
#define IP_DEST_PROT_UNREACHABLE    11004
#define IP_DEST_PORT_UNREACHABLE    11005
#define IP_NO_RESOURCES             11006
#define IP_BAD_OPTION               11007
#define IP_HW_ERROR                 11008
#define IP_PACKET_TOO_BIG           11009
#define IP_REQ_TIMED_OUT            11010
#define IP_BAD_REQ                  11011
#define IP_BAD_ROUTE                11012
#define IP_TTL_EXPIRED_TRANSIT      11013
#define IP_TTL_EXPIRED_REASSEM      11014
#define IP_PARAM_PROBLEM            11015
#define IP_SOURCE_QUENCH            11016
#define IP_OPTION_TOO_BIG           11017
#define IP_BAD_DESTINATION          11018
#define IP_DEST_UNREACHABLE         11019
#define IP_TIME_EXCEEDED            11020
#define IP_BAD_HEADER               11021
#define IP_UNRECOGNIZED_NEXT_HEADER 11022
#define IP_ICMP_ERROR               11023
#define IP_DEST_SCOPE_MISMATCH      11024

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
Int32 LinkICMP( void );

/*-------------------------------------------------------------------------*/

GUCEF_COMCORE_EXPORT_C
void UnlinkICMP( void );

/*-------------------------------------------------------------------------*/

/*
 *  ICMP function pointers
 */
GUCEF_COMCORE_EXPORT_C extern TIcmpCreateFilePtr IcmpCreateFile;
GUCEF_COMCORE_EXPORT_C extern TIcmpCloseHandlePtr IcmpCloseHandle;
GUCEF_COMCORE_EXPORT_C extern TIcmpParseRepliesPtr IcmpParseReplies;
GUCEF_COMCORE_EXPORT_C extern TIcmpSendEchoPtr IcmpSendEcho;
GUCEF_COMCORE_EXPORT_C extern TIcmpSendEcho2VistaPtr IcmpSendEcho2Vista;
GUCEF_COMCORE_EXPORT_C extern TIcmpSendEcho2Ptr IcmpSendEcho2;

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

#endif /* GUCEFCOMCORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-06-2007 :
       - Dinand: Added this section

---------------------------------------------------------------------------*/