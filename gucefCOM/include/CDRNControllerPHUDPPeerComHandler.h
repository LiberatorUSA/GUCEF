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

#ifndef CDRNCONTROLLERPHUDPPEERCOMHANDLER_H
#define CDRNCONTROLLERPHUDPPEERCOMHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CIPHUDPSOCKETEVENTHANDLER_H
#include "CIPHUDPSocketEventHandler.h"
#define CIPHUDPSOCKETEVENTHANDLER_H
#endif /* CIPHUDPSOCKETEVENTHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CDRNController;

/*-------------------------------------------------------------------------*/

/**
 *      Internally used class
 *      Acts as a proxy to the DRN Controller
 */
class CDRNControllerPHUDPPeerComHandler : public CIPHUDPSocketEventHandler
{
        public:
        
        CDRNControllerPHUDPPeerComHandler( CDRNController* controller );
        
        ~CDRNControllerPHUDPPeerComHandler();
        
        virtual void OnPHUDPSocketEvent( CPHUDPSocket& socket                ,
                                         CPHUDPSocket::TUDPSocketEvent event );
                                         
        virtual void OnPacketRecieved( CPHUDPSocket& socket                ,
                                       const CPHUDPSocket::TIPAddress& src ,
                                       const void* data                    ,
                                       const UInt32 datasize               ,
                                       const UInt16 packettype             ,
                                       const UInt32 packetnumber           );
        
        private:
        CDRNControllerPHUDPPeerComHandler( void );
        CDRNControllerPHUDPPeerComHandler( const CDRNControllerPHUDPPeerComHandler& src );
        CDRNControllerPHUDPPeerComHandler& operator=( const CDRNControllerPHUDPPeerComHandler& src );
        
        CDRNController* m_controller;                              
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CDRNCONTROLLERPHUDPPEERCOMHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-07-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
