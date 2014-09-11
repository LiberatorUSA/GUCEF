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

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"       /* library build defines & macros */
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

#include <winsock2.h>                  /* windows networking API */

#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

#include "dvwinsock.h"                 /* wrapper for global winsock API */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace COMCORE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static UInt32 _wsinit = 0;

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

static WSADATA wsadata;

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

#define LastSocketError WSAGetLastError()

#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

#define LastSocketError errno
#define closesocket close

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

/*-------------------------------------------------------------------------*/

UInt32
IsWinsockInitialized( void )
{
    return _wsinit;
}

/*-------------------------------------------------------------------------*/

/**
 *      Initializes winsock if winsock has not yet been initialized
 *
 *      @param desiredversion the winsock version you require
 *      @return returns wheter the initialization succeeded
 */
void
InitWinsock( UInt16 desiredversion )
{
    if ( _wsinit == 0 )
    {
        WSAStartup( desiredversion ,
                    &wsadata       );
        _wsinit = 1;
    }
}

/*-------------------------------------------------------------------------*/

void
ShutdownWinsock( void )
{
    if ( _wsinit == 1 )
    {
        WSACleanup();
        _wsinit = 0;
    }
}

/*-------------------------------------------------------------------------*/

const void*
GetWinsockData( void )
{
    if ( _wsinit > 0 )
    {
        return &wsadata;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM_MSWIN ? */

/*-------------------------------------------------------------------------*/

int
dvsocket_bind( SOCKET s                    ,
               const struct sockaddr* type ,
               int namelen                 ,
               int* error                  )
{
    int retval;
    if ( ( retval = bind( s       ,
                          type    ,
                          namelen ) ) == -1 )
    {
            *error = LastSocketError;
            return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

SOCKET
dvsocket_socket( int af       ,
                 int type     ,
                 int protocol ,
                 int* error   )
{
    SOCKET sock;
    if ( ( sock = socket( af       ,
                          type     ,
                          protocol ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return sock;
    }
    *error = 0;
    return sock;
}

/*-------------------------------------------------------------------------*/

int
dvsocket_select( int nfds                      ,
                 fd_set* readfds               ,
                 fd_set* writefds              ,
                 fd_set* exceptfds             ,
                 const struct timeval* timeout ,
                 int* error                    )
{
    /* timeval differs via constness between winsock and linux bsd sockets */
    int retval;
    if ( ( retval = select( nfds                      ,
                            readfds                   ,
                            writefds                  ,
                            exceptfds                 ,
                            (struct timeval*) timeout ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

int
dvsocket_listen( SOCKET s    ,
                 int backlog ,
                 int* error  )
{
    int retval;
    if ( ( retval = listen( s       ,
                            backlog ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

SOCKET
dvsocket_accept( SOCKET s              ,
                 struct sockaddr* addr ,
                 int* addrlen          ,
                 int* error            )
{
    SOCKET sock;
    if ( ( sock = accept( s       ,
                          addr    ,
                          addrlen ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return sock;
    }
    *error = 0;
    return sock;
}

/*-------------------------------------------------------------------------*/

int
dvsocket_send( SOCKET s        ,
               const void* buf ,
               int len         ,
               int flags       ,
               int* error      )
{
    int retval;
    if ( ( retval = send( s       ,
                          buf     ,
                          len     ,
                          flags   ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

int
dvsocket_recv( SOCKET s   ,
               void* buf  ,
               int len    ,
               int flags  ,
               int* error )
{
    int retval;
    if ( ( retval = recv( s       ,
                          buf     ,
                          len     ,
                          flags   ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

int
dvsocket_recvfrom( SOCKET s                  ,
                   void* buf                 ,
                   int len                   ,
                   int flags                 ,
                   struct sockaddr* fromaddr ,
                   socklen_t* fromlen        ,
                   int* error                )
{
    int retval;
    if ( ( retval = recvfrom( s        ,
                              buf      ,
                              len      ,
                              flags    ,
                              fromaddr ,
                              fromlen  ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

void
dvsocket_inet_ntoa( struct in_addr in ,
                char* ip          )
{
    char* retip;
    retip = inet_ntoa( in );
    strcpy( ip, retip );
}

/*-------------------------------------------------------------------------*/

LPHOSTENT
dvsocket_gethostbyname( const char* name ,
                        int* error       )
{
    LPHOSTENT retval;
    retval = gethostbyname( name );
    if ( retval == NULL )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

LPHOSTENT
dvsocket_gethostbyaddr( const char* addr ,
                        int len          ,
                        int type         ,
                        int* error       )
{
    LPHOSTENT retval;
    retval = gethostbyaddr( addr ,
                            len  ,
                            type );
    if ( retval == NULL )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

int
dvsocket_connect( SOCKET s                    ,
                  const struct sockaddr* addr ,
                  int namelen                 ,
                  int* error                  )
{
    int retval;
    if ( ( retval = connect( s        ,
                             addr     ,
                             namelen  ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------*/

int
dvsocket_closesocket( SOCKET s   ,
                      int* error )
{
    int retval;
    if ( ( retval = closesocket( s ) ) == SOCKET_ERROR )
    {
        *error = LastSocketError;
        return retval;
    }
    *error = 0;
    return retval;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace COMCORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/
