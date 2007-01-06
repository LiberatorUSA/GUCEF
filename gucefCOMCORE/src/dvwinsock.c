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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEFMT_H
#include "gucefMT.h"                   /* platform threading utils */
#define GUCEFMT_H
#endif /* GUCEFMT_H ? */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"       /* library build defines & macros */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"          /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */ 

#ifdef GUCEF_MSWIN_BUILD

#include "dvwinsock.h"                 /* wrapper for global winsock API */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

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
static struct SMutex* _globallock = NULL;
static WSADATA wsadata;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Checks wheter winsock has already been initialized.
 *
 *      @return boolean indicating true (>0) or false (0)
 */
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
        _globallock = MutexCreate();
                
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
        MutexDestroy( _globallock );
        _wsinit = 0;                
    }                
}

/*-------------------------------------------------------------------------*/

/**
 *      Returns the global winsock 
 *
 *      @return returns the global winsock data structure information
 */
const WSADATA*
GetWinsockData( void )
{
        MutexLock( _globallock );        
        if ( _wsinit > 0 )
        {
                MutexUnlock( _globallock );
                return &wsadata;        
        }
        MutexUnlock( _globallock );
        return NULL;      
}                

/*-------------------------------------------------------------------------*/

int
WSTS_bind( SOCKET s                    ,
           const struct sockaddr* type ,
           int namelen                 ,
           int* error                  )
{
        int retval;
        MutexLock( _globallock );                
        if ( ( retval = bind( s       , 
                              type    ,
                              namelen ) ) == -1 )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return retval;
        }                                
        MutexUnlock( _globallock );
        *error = 0; 
        return retval;                         
}

/*-------------------------------------------------------------------------*/

SOCKET
WSTS_socket( int af       ,
             int type     ,
             int protocol ,
             int* error   )
{
        SOCKET sock;
        MutexLock( _globallock );         
        if ( ( sock = socket( af       , 
                              type     ,
                              protocol ) ) == SOCKET_ERROR )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return sock;
        }                                
        MutexUnlock( _globallock );
        *error = 0;
        return sock;       
}

/*-------------------------------------------------------------------------*/

int 
WSTS_select( int nfds                      ,
             fd_set* readfds               ,
             fd_set* writefds              ,
             fd_set* exceptfds             ,
             const struct timeval* timeout ,
             int* error                    )
{
        int retval;
        MutexLock( _globallock );         
        if ( ( retval = select( nfds      , 
                                readfds   ,
                                writefds  ,
                                exceptfds ,
                                timeout   ) ) == SOCKET_ERROR )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return retval;
        }                                
        MutexUnlock( _globallock );
        *error = 0;
        return retval;
}              

/*-------------------------------------------------------------------------*/   

int
WSTS_listen( SOCKET s    ,
             int backlog ,
             int* error  )
{
        int retval;
        MutexLock( _globallock );         
        if ( ( retval = listen( s       , 
                                backlog ) ) == SOCKET_ERROR )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return retval;
        }                                
        MutexUnlock( _globallock );
        *error = 0;
        return retval;         
}                                 
           
/*-------------------------------------------------------------------------*/

SOCKET
WSTS_accept( SOCKET s              ,
             struct sockaddr* addr ,
             int* addrlen          ,
             int* error            )
{
        SOCKET sock;
        MutexLock( _globallock );         
        if ( ( sock = accept( s       , 
                              addr    ,
                              addrlen ) ) == SOCKET_ERROR )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return sock;
        }                                
        MutexUnlock( _globallock );
        *error = 0;
        return sock;             
}

/*-------------------------------------------------------------------------*/                         

int 
WSTS_send( SOCKET s        ,
           const void* buf ,
           int len         ,
           int flags       ,
           int* error      )
{
        int retval;
        MutexLock( _globallock );         
        if ( ( retval = send( s       , 
                              buf     ,
                              len     ,
                              flags   ) ) == SOCKET_ERROR )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return retval;
        }                                
        MutexUnlock( _globallock );
        *error = 0;
        return retval;  
}

/*-------------------------------------------------------------------------*/

int 
WSTS_recv( SOCKET s   ,
           void* buf  ,
           int len    ,
           int flags  ,
           int* error )
{
        int retval;
        MutexLock( _globallock );         
        if ( ( retval = recv( s       , 
                              buf     ,
                              len     ,
                              flags   ) ) == SOCKET_ERROR )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return retval;
        }                                
        MutexUnlock( _globallock );
        *error = 0;
        return retval;        
}                      

/*-------------------------------------------------------------------------*/

void
WSTS_inet_ntoa( struct in_addr in ,
                char* ip          )
{
        char* retip;
        MutexLock( _globallock );
        retip = inet_ntoa( in );
        strcpy( ip, retip );
        MutexUnlock( _globallock );              
}

/*-------------------------------------------------------------------------*/

LPHOSTENT               
WSTS_gethostbyname( const char* name ,
                    int* error       )
{
        LPHOSTENT retval;
        MutexLock( _globallock );
        retval = gethostbyname( name );
        if ( retval == NULL )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return retval;                
        }        
        MutexUnlock( _globallock );         
        *error = 0;
        return retval;
}

/*-------------------------------------------------------------------------*/

LPHOSTENT 
WSTS_gethostbyaddr( const char* addr ,
                    int len          ,
                    int type         ,
                    int* error       )
{
        LPHOSTENT retval;
        MutexLock( _globallock );
        retval = gethostbyaddr( addr ,
                                len  ,
                                type );
        if ( retval == NULL )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return retval;                
        }        
        MutexUnlock( _globallock );         
        *error = 0;
        return retval;
}                   

/*-------------------------------------------------------------------------*/

int
WSTS_connect( SOCKET s                    ,
              const struct sockaddr* addr ,
              int namelen                 ,
              int* error                  )
{
        int retval;
        MutexLock( _globallock );         
        if ( ( retval = connect( s        , 
                                 addr     ,
                                 namelen  ) ) == SOCKET_ERROR )
        {
                *error = WSAGetLastError();
                MutexUnlock( _globallock ); 
                return retval;
        }                                
        MutexUnlock( _globallock );
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

#endif /* GUCEF_MSWIN_BUILD ? */
 