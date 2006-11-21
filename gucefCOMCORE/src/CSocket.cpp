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

#ifndef CCOM_H
#include "CCom.h"		  /* network manager */
#define CCOM_H
#endif /* CCOM_H ? */

#ifdef MSWIN_BUILD
  #define FD_SETSIZE 1      /* should set the size of the FD set struct to 1 for VC */
  #include <winsock2.h>
#else
 #ifdef LINUX_BUILD
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
         *      recieving update events.
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
                struct hostent* retval = NULL;                              
                retval = gethostbyname( destaddrstr.C_String() );
                if ( retval )
                {
                        resolvedDest.netaddr = inet_addr( retval->h_addr_list[ 0 ] ); // <- werkt niet ??
                        resolvedDest.port = htons( destport );
                        return true;
                }
                return false;                   
        }
}

/*-------------------------------------------------------------------------*/

bool 
CSocket::ConvertFromIPAddress( const TIPAddress& src     ,
                               CORE::CString& srcaddrstr ,
                               UInt16& srcport           )
{TRACE;                   
             // @todo MAKEME
        GUCEF_ASSERT_ALWAYS;

        if ( src.netaddr == INADDR_ANY )
        {
                srcaddrstr = "localhost";       
        }
        return true;        
}  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/