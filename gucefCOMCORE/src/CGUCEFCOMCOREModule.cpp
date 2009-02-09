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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef CCOM_H
#include "CCom.h"      /* header for the main communication manager */
#define CCOM_H
#endif /* CCOM_H ? */

#ifndef CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define CTCPCLIENTSOCKET_H
#endif /* CTCPCLIENTSOCKET_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CPING_H
#include "CPing.h"
#define GUCEF_COMCORE_CPING_H
#endif /* GUCEF_COMCORE_CPING_H ? */

#ifndef GUCEF_COMCORE_CUDPMASTERSOCKET_H
#include "CUDPMasterSocket.h"
#define GUCEF_COMCORE_CUDPMASTERSOCKET_H
#endif /* GUCEF_COMCORE_CUDPMASTERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CUDPCHANNEL_H
#include "CUDPChannel.h"
#define GUCEF_COMCORE_CUDPCHANNEL_H
#endif /* GUCEF_COMCORE_CUDPCHANNEL_H ? */

#ifndef GUCEF_COMCORE_CPINGTASKCONSUMER_H
#include "gucefCOMCORE_CPingTaskConsumer.h"
#define GUCEF_COMCORE_CPINGTASKCONSUMER_H
#endif /* GUCEF_COMCORE_CPINGTASKCONSUMER_H ? */

#include "CGUCEFCOMCOREModule.h"  /* definition of the class implemented here */

#ifdef GUCEF_MSWIN_BUILD
  #ifndef DVWINSOCK_H
  #include "dvwinsock.h"
  #define DVWINSOCK_H
  #endif /* DVWINSOCK_H */
#endif

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
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CORE::CTFactory< CORE::CTaskConsumer, CPingTaskConsumer > TPingTaskConsumerFactory;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

bool 
CGUCEFCOMCOREModule::Load( void )
{
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOMCORE Module loaded" );
    
    #ifdef GUCEF_MSWIN_BUILD
    InitWinsock( 1 );
    #endif
    
    /* simply instantiate our com manager when the module is loaded */
    //CCom::Instance();
    
    CTCPConnection::RegisterEvents();
    CTCPServerSocket::RegisterEvents();
    CTCPClientSocket::RegisterEvents();
    CUDPSocket::RegisterEvents();
    CPing::RegisterEvents();
    CUDPMasterSocket::RegisterEvents();
    CUDPChannel::RegisterEvents();
    
    // Make the task manager capable of handling ping tasks    
    CORE::CTaskManager::Instance()->RegisterTaskConsumerFactory( CPingTaskConsumer::GetTypeString() ,
                                                                 new TPingTaskConsumerFactory()     );
    
    return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFCOMCOREModule::Unload( void )
{
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOMCORE Module unloading" );
        
        CORE::CTaskManager::Instance()->UnregisterTaskConsumerFactory( CPingTaskConsumer::GetTypeString() );
        
        CCom::Deinstance();
        
        #ifdef GUCEF_MSWIN_BUILD
        ShutdownWinsock();
        #endif
                
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
