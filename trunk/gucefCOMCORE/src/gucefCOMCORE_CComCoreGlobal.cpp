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

#ifndef GUCEF_COMCORE_DVSOCKET_H
#include "dvwinsock.h"
#define GUCEF_COMCORE_DVSOCKET_H
#endif /* GUCEF_COMCORE_DVSOCKET_H */

#ifndef GUCEF_COMCORE_CPINGTASKCONSUMER_H
#include "gucefCOMCORE_CPingTaskConsumer.h"
#define GUCEF_COMCORE_CPINGTASKCONSUMER_H
#endif /* GUCEF_COMCORE_CPINGTASKCONSUMER_H ? */

#include "gucefCOMCORE_CComCoreGlobal.h"  /* definition of the class implemented here */

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CComCoreGlobal* CComCoreGlobal::g_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CComCoreGlobal::CComCoreGlobal( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CComCoreGlobal::Initialize( void )
{GUCEF_TRACE;

    CORE::CCoreGlobal::Instance();

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOMCORE Global systems initializing" );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    InitWinsock( 1 );
    #endif

    CTCPConnection::RegisterEvents();
    CTCPServerSocket::RegisterEvents();
    CTCPClientSocket::RegisterEvents();
    CUDPSocket::RegisterEvents();
    CPing::RegisterEvents();
    CPingTaskConsumer::RegisterEvents();
    CUDPMasterSocket::RegisterEvents();
    CUDPChannel::RegisterEvents();

    // Make the task manager capable of handling ping tasks
    CORE::CCoreGlobal::Instance()->GetTaskManager().RegisterTaskConsumerFactory( CPingTaskConsumer::GetTypeString() ,
                                                                                 new TPingTaskConsumerFactory()     );

    m_com = new CCom();
}

/*-------------------------------------------------------------------------*/

CComCoreGlobal::~CComCoreGlobal()
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOMCORE Global systems shutting down" );

    delete m_com;
    m_com = NULL;

    CORE::CCoreGlobal::Instance()->GetTaskManager().UnregisterTaskConsumerFactory( CPingTaskConsumer::GetTypeString() );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    ShutdownWinsock();
    #endif
}

/*-------------------------------------------------------------------------*/

CComCoreGlobal*
CComCoreGlobal::Instance( void )
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        g_instance = new CComCoreGlobal();
        g_instance->Initialize();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CComCoreGlobal::Deinstance( void )
{GUCEF_TRACE;

    delete g_instance;
    g_instance = NULL;
}

/*-------------------------------------------------------------------------*/

CCom&
CComCoreGlobal::GetCom( void )
{
    return *m_com;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
