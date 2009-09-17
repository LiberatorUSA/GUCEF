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

#ifndef CSERVERPORTEXTENDER_H
#define CSERVERPORTEXTENDER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#endif /* GUCEF_COMCORE_CTCPCLIENTSOCKET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using GUCEF;
using GUCEF::COMCORE;
using GUCEF::CORE;
using GUCEF::MT;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CServerPortExtender : public CObserver
{
    public:
    
    CServerPortExtender( void );
    
    bool ListenForClientsOnPort( UInt16 port );
    
    bool ListenForReversedControlClientOnPort( UInt16 port );
    
    bool ListenForReversedClientsOnPort( UInt16 port );

    protected:
    
    typedef CTEventHandlerFunctor< CServerPortExtender > TEventCallback;
    
    private:
    
    CServerPortExtender( const CServerPortExtender& src );

    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );
    
    virtual void OnServerSocketNotify( CNotifier* notifier           ,
                                       const CEvent& eventid         ,
                                       CICloneable* eventdata = NULL );

    virtual void OnReversedServerSocketNotify( CNotifier* notifier           ,
                                               const CEvent& eventid         ,
                                               CICloneable* eventdata = NULL );

    virtual void OnReversedServerControlSocketNotify( CNotifier* notifier           ,
                                                      const CEvent& eventid         ,
                                                      CICloneable* eventdata = NULL );

    virtual void OnRSControlClientConnected( CNotifier* notifier           ,
                                             const CEvent& eventid         ,
                                             CICloneable* eventdata = NULL );

    virtual void OnRSControlClientDisconnected( CNotifier* notifier           ,
                                                const CEvent& eventid         ,
                                                CICloneable* eventdata = NULL );

    virtual void OnRSControlClientDataSent( CNotifier* notifier           ,
                                            const CEvent& eventid         ,
                                            CICloneable* eventdata = NULL );

    virtual void OnRSControlClientDataRecieved( CNotifier* notifier           ,
                                                const CEvent& eventid         ,
                                                CICloneable* eventdata = NULL );
                                         
    virtual void OnRSControlClientSocketError( CNotifier* notifier           ,
                                               const CEvent& eventid         ,
                                               CICloneable* eventdata = NULL );

    virtual void OnRSClientConnected( CNotifier* notifier           ,
                                      const CEvent& eventid         ,
                                      CICloneable* eventdata = NULL );

    virtual void OnRSClientDisconnected( CNotifier* notifier           ,
                                         const CEvent& eventid         ,
                                         CICloneable* eventdata = NULL );

    virtual void OnRSClientDataSent( CNotifier* notifier           ,
                                     const CEvent& eventid         ,
                                     CICloneable* eventdata = NULL );

    virtual void OnRSClientDataRecieved( CNotifier* notifier           ,
                                         const CEvent& eventid         ,
                                         CICloneable* eventdata = NULL );
                                         
    virtual void OnRSClientSocketError( CNotifier* notifier           ,
                                        const CEvent& eventid         ,
                                        CICloneable* eventdata = NULL );
    
    private:
    
    CTCPServerSocket m_reversedServerControlSocket;
    CTCPServerSocket m_reversedServerSocket;
    CTCPServerSocket m_serverSocket;
    UInt16 m_serverPort;
    UInt16 m_reversedServerPort;
    UInt16 m_reversedServerControlPort;
};