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
