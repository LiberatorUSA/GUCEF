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

#include "CTCPServerSocketInterface.h"

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

CTCPServerSocketInterface::CTCPServerSocketInterface( void )
{TRACE;

}

/*-------------------------------------------------------------------------*/        

CTCPServerSocketInterface::CTCPServerSocketInterface( const CTCPServerSocketInterface& src )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CTCPServerSocketInterface&
CTCPServerSocketInterface::operator=( const CTCPServerSocketInterface& src )
{TRACE;
    if ( &src != this )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CTCPServerSocketInterface::OnClientRead( CTCPServerSocket& socket     ,
                                         CTCPServerConnection& client ,
                                         UInt32 connection            ,
                                         const char *data             ,
                                         UInt16 size                  ,
                                         UInt16 &keepbytes            )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void
CTCPServerSocketInterface::OnClientWrite( CTCPServerSocket& socket     ,
                                          CTCPServerConnection& client ,
                                          UInt32 connection            ,
                                          const char *data             ,
                                          UInt32 size                  )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void
CTCPServerSocketInterface::OnClientConnected( CTCPServerSocket& socket        ,
                                              CTCPServerConnection& clientcon ,
                                              UInt32 connection               )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void
CTCPServerSocketInterface::OnClientDisconnect( CTCPServerSocket& socket     ,
                                               CTCPServerConnection& client ,
                                               UInt32 connection            ,
                                               bool clientclose             )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CTCPServerSocketInterface::OnClientError( CTCPServerSocket& socket     ,
                                          CTCPServerConnection& client ,
                                          UInt32 connection            ,
                                          TSocketError *errorcode      )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void
CTCPServerSocketInterface::OnOpen( CTCPServerSocket &socket )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CTCPServerSocketInterface::OnClose( CTCPServerSocket &socket )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CTCPServerSocketInterface::OnMaxConnectionsChanged( CTCPServerSocket &socket ,
                                                    UInt32 old_max           ,
                                                    UInt32 new_max           )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void
CTCPServerSocketInterface::OnError( CTCPServerSocket &socket ,
                                    TSocketError errorcode   )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CTCPServerSocketInterface::~CTCPServerSocketInterface()
{TRACE;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
