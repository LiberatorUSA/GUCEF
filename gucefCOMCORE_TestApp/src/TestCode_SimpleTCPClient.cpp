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

#ifndef GUCEFCORE_H
#include "gucefCORE.h"          /* GUCEF CORE library API */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefCOMCORE.h"       /* GUCEF COMCORE library API */
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;
using namespace GUCEF::MT;
using namespace GUCEF::CORE;
using namespace GUCEF::COMCORE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CSimpleTCPClient : public CGUCEFAppSubSystem
{
        public:
        
        CSimpleTCPClient( const CORE::CString& remoteaddr  ,
                        CORE::UInt16 port                ,
                        const CORE::CString& information )
                : _clientsock( false ),
                  CGUCEFAppSubSystem(true)
        {
            RemoteAddr  = remoteaddr;
            Port        = port;
            Information = information;
        }
        
        protected:
                   
        virtual void OnNotify( CORE::CNotifier* notifier           ,
                               const CORE::CEvent& eventid         ,
                               CORE::CICloneable* eventdata = NULL )
        {
            if ( eventid == CGUCEFApplication::AppInitEvent )
            {
                printf("%s\n", eventid.GetName());
                _clientsock.Subscribe( &AsObserver() );
                _clientsock.ConnectTo( RemoteAddr,
                                       Port      );
            }
            else if ( eventid == _clientsock.ConnectedEvent )
            {
                system("pause");

                _clientsock.Send(Information);
            }
            else
            {
                printf("%s - UNHANDLED\r\n", eventid.GetName());
            }
        }

        private:
        CTCPClientSocket _clientsock;
        CORE::CString RemoteAddr;
        CORE::UInt16  Port;
        CORE::CString Information;
};


/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
SetupSimpleTCPClient( const CORE::CString& remoteaddr  ,
                     CORE::UInt16 port                 ,
                      const CORE::CString& information )
{
    CSimpleTCPClient *tcpclient = new CSimpleTCPClient(remoteaddr, port, information);
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 10-12-2006 : Logan
        - Initial version

-----------------------------------------------------------------------------*/