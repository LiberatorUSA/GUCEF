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

#ifndef CIPHUDPSOCKETEVENTHANDLER_H
#define CIPHUDPSOCKETEVENTHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CPHUDPSOCKET_H
#include "CPHUDPSocket.h"
#define CPHUDPSOCKET_H
#endif /* CPHUDPSOCKET_H ? */

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

class EXPORT_CPP CIPHUDPSocketEventHandler
{                      
        public:
        
        virtual ~CIPHUDPSocketEventHandler();
        
        protected:
        friend class CPHUDPSocket;                
        
        virtual void OnPHUDPSocketEvent( CPHUDPSocket& socket                ,
                                         CPHUDPSocket::TUDPSocketEvent event ) = 0;
                                         
        virtual void OnPacketRecieved( CPHUDPSocket& socket                ,
                                       const CPHUDPSocket::TIPAddress& src ,
                                       const void* data                    ,
                                       const UInt32 datasize               ,
                                       const UInt16 packettype             ,
                                       const UInt32 packetnumber           ) = 0;
                                                                                                                      
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CIPHUDPSOCKETEVENTHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-07-2005 :
        - Moved to GUCEF::COM
        - Renamed to CIPHUDPSocketEventHandler
- 28-05-2005 :
        - Initial version

-----------------------------------------------------------------------------*/
