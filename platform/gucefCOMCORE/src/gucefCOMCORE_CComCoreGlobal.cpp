/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

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

#ifndef GUCEF_COMCORE_CDISCOVERYMANAGER_H
#include "gucefCOMCORE_CDiscoveryManager.h"
#define GUCEF_COMCORE_CDISCOVERYMANAGER_H
#endif /* GUCEF_COMCORE_CDISCOVERYMANAGER_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#include "gucefCOMCORE_CPubSubClientFactory.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H ? */

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

MT::CMutex CComCoreGlobal::g_dataLock;
CComCoreGlobal* CComCoreGlobal::g_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CComCoreGlobal::CComCoreGlobal( void )
    : m_com( GUCEF_NULL )
    , m_discoveryManager( GUCEF_NULL )
    , m_pubsubClientFactory( GUCEF_NULL )
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
    m_discoveryManager = new CDiscoveryManager();
    m_pubsubClientFactory = new CPubSubClientFactory();
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
CComCoreGlobal::Instance()
{GUCEF_TRACE;

    if ( GUCEF_NULL == g_instance )
    {
        MT::CScopeMutex lock( g_dataLock );
        if ( GUCEF_NULL == g_instance )
        {
            g_instance = new CComCoreGlobal();
            g_instance->Initialize();
        }
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
{GUCEF_TRACE;

    return *m_com;
}

/*-------------------------------------------------------------------------*/

CDiscoveryManager&
CComCoreGlobal::GetDiscoveryManager( void )
{GUCEF_TRACE;

    return *m_discoveryManager;
}

/*-------------------------------------------------------------------------*/

CPubSubClientFactory& 
CComCoreGlobal::GetPubSubClientFactory( void )
{GUCEF_TRACE;

    return *m_pubsubClientFactory;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
