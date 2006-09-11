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

#ifndef CDRNCONTROLLER_H
#include "CDRNController.h"
#define CDRNCONTROLLER_H
#endif /* CDRNCONTROLLER_H ? */

#include "CDRNControllerPHUDPPeerComHandler.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDRNControllerPHUDPPeerComHandler::CDRNControllerPHUDPPeerComHandler( void )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNControllerPHUDPPeerComHandler::CDRNControllerPHUDPPeerComHandler( CDRNController* controller )
        : m_controller( controller )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNControllerPHUDPPeerComHandler::CDRNControllerPHUDPPeerComHandler( const CDRNControllerPHUDPPeerComHandler& src )
{TRACE;
        
}

/*-------------------------------------------------------------------------*/

CDRNControllerPHUDPPeerComHandler::~CDRNControllerPHUDPPeerComHandler()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNControllerPHUDPPeerComHandler& 
CDRNControllerPHUDPPeerComHandler::operator=( const CDRNControllerPHUDPPeerComHandler& src )
{TRACE;
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CDRNControllerPHUDPPeerComHandler::OnPHUDPSocketEvent( CPHUDPSocket& socket                ,
                                                       CPHUDPSocket::TUDPSocketEvent event )
{TRACE;
}                                                       

/*-------------------------------------------------------------------------*/
                                         
void 
CDRNControllerPHUDPPeerComHandler::OnPacketRecieved( CPHUDPSocket& socket                ,
                                                     const CPHUDPSocket::TIPAddress& src ,
                                                     const void* data                    ,
                                                     const UInt32 datasize               ,
                                                     const UInt16 packettype             ,
                                                     const UInt32 packetnumber           )
{TRACE;
        m_controller->OnUDPPeerPacketRecieved( socket       ,
                                               src          ,
                                               data         ,
                                               datasize     ,
                                               packettype   ,
                                               packetnumber );
}                                                     

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
