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

class CSimpleTCPClient : public CORE::CObservingNotifier
{
        public:
        
        CSimpleTCPClient( const CORE::CString& remoteaddr  ,
                          CORE::UInt16 port                ,
                          const CORE::CString& information )
                : _clientsock( false ),
                  CObservingNotifier()
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
            else if ( eventid == CTCPClientSocket::ConnectedEvent )
            {
                _clientsock.Send(Information);
            }
            else if ( eventid == CTCPClientSocket::DataRecievedEvent )
            {
                int buffersize = ((CTCPClientSocket::TDataRecievedEventData*)eventdata)->GetData().GetDataSize();
                char *buffer = new char[buffersize + 1];
                memcpy(buffer, ((CTCPClientSocket::TDataRecievedEventData*)eventdata)->GetData().GetConstBufferPtr(), buffersize);
                buffer[buffersize] = '\0';

                printf("%s", buffer);
            }
            else if ( eventid == CTCPClientSocket::SocketErrorEvent )
            {
                if ( NULL != eventdata )
                {
                    CTCPClientSocket::TSocketErrorEventData* errorData = static_cast< CTCPClientSocket::TSocketErrorEventData* >( eventdata );                     
                    printf( "%s - Error code: %d\r\n", eventid.GetName(), errorData->GetData() );
                }
                else
                {
                    printf( "%s - No error data found\r\n", eventid.GetName() );
                }
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