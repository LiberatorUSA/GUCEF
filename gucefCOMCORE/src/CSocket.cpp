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

#ifndef CCOM_H
#include "CCom.h"		  /* network manager */
#define CCOM_H
#endif /* CCOM_H ? */

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/