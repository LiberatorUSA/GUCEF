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

#include <string.h>               /* string utils */

#include "CSocket.h"              /* header for this class */

#ifndef DVSTRUTILS_H
#include "dvstrutils.h"
#define DVSTRUTILS_H
#endif /* DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef CCOM_H
#include "CCom.h"		  /* network manager */
#define CCOM_H
#endif /* CCOM_H ? */

#ifdef GUCEF_MSWIN_BUILD
  #define FD_SETSIZE 1      /* should set the size of the FD set struct to 1 for VC */
  #include <winsock2.h>
  #include <Ws2tcpip.h>
  #include <Wspiapi.h>
#else
 #ifdef GUCEF_LINUX_BUILD
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
 #endif
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CSocket::CSocket( void )
{TRACE;

        /*
         *      Register the socket object so that it's capable of 
         *      receiving update events.
         */
        CCom::Instance()->RegisterSocketObject( this );
}

/*-------------------------------------------------------------------------*/

CSocket::CSocket( const CSocket& src )
{TRACE;

        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CSocket::~CSocket()
{TRACE;

        /*
         *      Unregister the socket object so that it's removed
         *      from the update event notification list 
         */
        CCom::Instance()->UnregisterSocketObject( this );
}

/*-------------------------------------------------------------------------*/

CSocket&
CSocket::operator=( const CSocket& src )
{TRACE;
       
        /* dummy, do not use */ 
        return *this;       
}

/*-------------------------------------------------------------------------*/

void 
CSocket::SetSocketID( UInt32 sid )
{TRACE;
        _sid = sid; 
}

/*-------------------------------------------------------------------------*/

UInt32 
CSocket::GetSocketID( void ) const
{TRACE;

        return _sid;
}

/*-------------------------------------------------------------------------*/

bool 
CSocket::ConvertToIPAddress( const CORE::CString& destaddrstr ,
                             const UInt16 destport            ,  
                             TIPAddress& resolvedDest         )
{TRACE;

    if ( CORE::Check_If_IP( destaddrstr.C_String() ) )
    {
        resolvedDest.netaddr = inet_addr( destaddrstr.C_String() );                
        if ( resolvedDest.netaddr == INADDR_NONE ) return false;                
        resolvedDest.port = htons( destport );
        return true;
    }
    else
    {              
        #if 1

        struct hostent* retval = gethostbyname( destaddrstr.C_String() );        
        if ( retval != NULL )
        {
            GUCEF_DEBUG_LOG( 1, CORE::CString( "CSocket::ConvertToIPAddress(): gethostbyname(): full name: " ) + retval->h_name );
            char* addrStr = inet_ntoa( *( struct in_addr*)( retval->h_addr_list[0] ) );
            resolvedDest.netaddr = inet_addr( addrStr );
            resolvedDest.port = htons( destport );             
            return true;            
        }
        return false;
        
        #else
        
        struct addrinfo* info = NULL;
        CORE::CString portString( CORE::Int32ToString( destport ) );
        int retval = getaddrinfo( destaddrstr.C_String() ,
                                  portString.C_String()  ,
                                  NULL                   ,
                                  &info                  );
        if ( retval == 0 )
        {
            struct in_addr* addr = (struct in_addr*)info->ai_addr;
            DEBUGOUTPUTssss( "CSocket::ConvertToIPAddress(): resolved DNS name ", destaddrstr.C_String(), " to ", inet_ntoa( *addr ) );
            resolvedDest.netaddr = inet_addr( inet_ntoa( *addr ) ); // <- does this actually work ???
            resolvedDest.port = htons( destport );
            return true;
        }
        #ifdef GUCEF_COMCORE_DEBUG_MODE
        DEBUGOUTPUTsi( "CSocket::ConvertToIPAddress(): gethostbyname() failed with code ", retval );
        #endif
        return false;
        
        #endif
    }
}

/*-------------------------------------------------------------------------*/

bool 
CSocket::ConvertFromIPAddress( const TIPAddress& src     ,
                               CORE::CString& srcaddrstr ,
                               UInt16& srcport           )
{TRACE;                   

    in_addr addrStruct;
    addrStruct.S_un.S_addr = src.netaddr;
    const char* addrStr = inet_ntoa( addrStruct );
    if ( addrStr != NULL )
    {
        srcaddrstr = addrStr;
        srcport = ntohs( src.port );
        return true;
    }
    return false;        
}  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/