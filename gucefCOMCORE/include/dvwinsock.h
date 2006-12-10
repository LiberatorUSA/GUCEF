/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */
 
#ifndef DVWINSOCK_H
#define DVWINSOCK_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"       /* library build defines & macros */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

#ifdef GUCEF_MSWIN_BUILD

//#ifndef SOCKETERRORS_H
//#include "socketerrors.h"              /* network API independant socket errors */
//#define SOCKETERRORS_H
//#endif /* SOCKETERRORS_H ? */

#include <winsock.h>                   /* windows networking API */

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ Name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*--------------------------------------------------------------------------*/

/**
 *      Checks wheter winsock has already been initialized.
 *
 *      @return boolean indicating true (>0) or false (0)
 */
UInt32 
IsWinsockInitialized( void );

/*-------------------------------------------------------------------------*/

/**
 *      Returns the global winsock 
 *
 *      @return returns the global winsock data structure information
 */
const WSADATA*
GetWinsockData( void );

/*-------------------------------------------------------------------------*/

/**
 *      Initializes winsock if winsock has not yet been initialized
 *      Should be called before each instance of use for winsock.
 *      ie for each socket. Each call increaments a counter that is 
 *      decremented with a call to ShutdownWinsock().
 *
 *      @param desiredversion the winsock version you require
 */
void 
InitWinsock( UInt16 desiredversion );

/*-------------------------------------------------------------------------*/

void
ShutdownWinsock( void );

/*-------------------------------------------------------------------------*/

/*
 *      The following are threadsafe replacements for winsock functions
 */

/*-------------------------------------------------------------------------*/

int
WSTS_bind( SOCKET s                    ,
           const struct sockaddr* type ,
           int namelen                 ,
           int* error                  );


/*-------------------------------------------------------------------------*/

SOCKET
WSTS_socket( int af       ,
             int type     ,
             int protocol ,
             int* error   );

/*-------------------------------------------------------------------------*/
             
int 
WSTS_select( int nfds                      ,
             fd_set* readfds               ,
             fd_set* writefds              ,
             fd_set* exceptfds             ,
             const struct timeval* timeout ,
             int* error                    );               
             
 /*-------------------------------------------------------------------------*/
             
int
WSTS_listen( SOCKET s    ,
             int backlog ,
             int* error  ); 
             
/*-------------------------------------------------------------------------*/              
             
SOCKET
WSTS_accept( SOCKET s              ,
             struct sockaddr* addr ,
             int* addrlen          ,
             int* error            );
             
/*-------------------------------------------------------------------------*/             
             
int
WSTS_connect( SOCKET s                    ,
              const struct sockaddr* addr ,
              int namelen                 ,
              int* error                  );
      
/*-------------------------------------------------------------------------*/      
              
int 
WSTS_send( SOCKET s        ,
           const void* buf ,
           int len         ,
           int flags       ,
           int* error      );
           
/*-------------------------------------------------------------------------*/
           
int 
WSTS_recv( SOCKET s   ,
           void* buf  ,
           int len    ,
           int flags  ,
           int* error );                                     
             
/*-------------------------------------------------------------------------*/                                                           

void
WSTS_inet_ntoa( struct in_addr in ,
                char* ip          );
              
/*-------------------------------------------------------------------------*/              
                
LPHOSTENT               
WSTS_gethostbyname( const char* name ,
                    int* error       );

/*-------------------------------------------------------------------------*/

LPHOSTENT 
WSTS_gethostbyaddr( const char* addr ,
                    int len          ,
                    int type         ,
                    int* error       );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace COMCORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */

/*--------------------------------------------------------------------------*/

#endif /* DVWINSOCK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 05-02-2004 :
        - Initial implementation.

-----------------------------------------------------------------------------*/
 