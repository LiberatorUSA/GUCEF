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

#include <string.h>               /* string utils */

#include "CSocket.h"              /* header for this class */

#ifndef DVSTRUTILS_H
#include "dvstrutils.h"
#define DVSTRUTILS_H
#endif /* DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"              /* we want the log manager for its defines */
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

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
                             CIPAddress& resolvedDest         )
{TRACE;

    if ( CORE::Check_If_IP( destaddrstr.C_String() ) )
    {
        resolvedDest.SetAddress( inet_addr( destaddrstr.C_String() ) );
        if ( resolvedDest.GetAddress() == INADDR_NONE ) return false;                
        resolvedDest.SetPort( destport );
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
            Int32 netaddr = inet_addr( addrStr );
            if ( netaddr >= 0 ) 
            {   
                resolvedDest.SetAddress( netaddr );
                resolvedDest.SetPort( destport );             
                return true;
            }            
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
CSocket::ConvertFromIPAddress( const CIPAddress& src     ,
                               CORE::CString& srcaddrstr ,
                               UInt16& srcport           )
{TRACE;                   

    in_addr addrStruct;
    addrStruct.S_un.S_addr = src.GetAddress();
    const char* addrStr = inet_ntoa( addrStruct );
    if ( addrStr != NULL )
    {
        srcaddrstr = addrStr;
        srcport = ntohs( src.GetPort() );
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