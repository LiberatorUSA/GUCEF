/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#include "gucefCORE_CCoreGlobal.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CCoreGlobal* CCoreGlobal::g_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CCoreGlobal*
CCoreGlobal::Instance()
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        return new CCoreGlobal();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CCoreGlobal::Deinstance( void )
{GUCEF_TRACE;

    delete g_instance;
    g_instance = NULL;
}

/*-------------------------------------------------------------------------*/

CCoreGlobal::CCoreGlobal( void )
    : m_taskManager( NULL )                ,
      m_urlHandlerRegistry( NULL )         ,
      m_dstoreCodecRegistry( NULL )        ,
      m_exclusiveActivationManager( NULL ) ,
      m_application( NULL )                ,
      m_logManager( NULL )                 ,
      m_application( NULL )                ,
      m_dstoreCodecPluginManager( NULL )   ,
      m_genericPluginManager( NULL )       , 
      m_pluginControl( NULL )              ,
      m_sysConsole( NULL )                 ,
      m_notificationIdRegistry( NULL )     ,
      m_stdCodecPluginManager( NULL )
{GUCEF_TRACE;

    // it is important to initialize the call stack tracer at an early stage
    GUCEF_InitCallstackUtility();
    
    /*
     *      Very important: Initialize the memory manager before anything else !!!!!
     */
    #ifdef ADD_MEMORY_MANAGER        
    MEMMAN_SetLogFile( "GUCEFMemoryLog.txt" );
    MEMMAN_SetExhaustiveTesting( 0 ); 
    MEMMAN_SetPaddingSize( 0 );
    MEMMAN_Initialize();       
    #endif 
    
    /*
     *      Initialize centralized output
     */
    tspinit();
    #ifdef GUCEF_CORE_DEBUG_MODE        
    tssetcoutfile( "GUCEFLog.txt" );
    tsusecoutfile( 1 );
    #endif

    /*
     *  Instantiate all the singletons
     *  We start with the log manager so that it is possible to log everything from that point on
     *  if a logger is registered at an early stage      
     */
    m_logManager = new CLogManager();
    m_notificationIdRegistry = new CNotificationIDRegistry();

    /*
     *  Make sure all events are registered from the start
     */
    CNotifier::RegisterEvents();
    CPulseGenerator::RegisterEvents();
    CStreamerEvents::RegisterEvents();
    CTimer::RegisterEvents();
    CPluginControl::RegisterEvents();
    CIURLEvents::RegisterEvents();
    CNotifyingMapEvents::RegisterEvents();
    CGUCEFApplication::RegisterEvents();
    CTaskDelegator::RegisterEvents();
    CTaskConsumer::RegisterEvents();
    CTaskManager::RegisterEvents();        

    /*
     *  Instantiate the rest of the singletons
     */
    m_configStore = new CConfigStore();
    m_taskManager = new CTaskManager();
    m_pluginControl = new CPluginControl();
    m_dstoreCodecRegistry = new CDStoreCodecRegistry();
    m_dstoreCodecPluginManager = new CDStoreCodecPluginManager();
    m_genericPluginManager = new CGenericPluginManager();
    m_stdCodecPluginManager = new CStdCodecPluginManager();
    m_urlHandlerRegistry = new CURLHandlerRegistry();
    m_sysConsole = new CSysConsole();
    m_application = new CGUCEFApplication();

    #ifdef GUCEF_MSWIN_BUILD
    CWndMsgHookNotifier::RegisterEvents();
    #endif /* GUCEF_MSWIN_BUILD ? */ 
    
    /*
     *      Register some default codecs/handlers 
     */
    m_urlHandlerRegistry->Register( "file", new CFileURLHandler() );
    m_dstoreCodecRegistry->Register( "ini", new CIniDataStoreCodec() );
    
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "gucefCORE Global systems initialized" );

}

/*-------------------------------------------------------------------------*/

CCoreGlobal::~CCoreGlobal()
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Shutting down gucefCORE global systems" );
           
    /*
     *      cleanup all singletons
     *      Take care to deinstance them in the correct order !!!
     */
    delete m_taskManager;
    m_taskManager = NULL;
    delete m_urlHandlerRegistry;
    m_urlHandlerRegistry = NULL;
    delete m_dstoreCodecRegistry;
    m_dstoreCodecRegistry = NULL;
    delete m_exclusiveActivationManager;
    m_exclusiveActivationManager = NULL;
    delete m_application;
    m_application = NULL;
    delete m_logManager;
    m_logManager = NULL;
    delete m_dstoreCodecPluginManager;
    m_dstoreCodecPluginManager = NULL;
    delete m_genericPluginManager;
    m_genericPluginManager = NULL;
    delete m_pluginControl;
    m_pluginControl = NULL;
    delete m_sysConsole;
    m_sysConsole = NULL;
    delete m_notificationIdRegistry;
    m_notificationIdRegistry = NULL;
    delete m_stdCodecPluginManager;
    m_stdCodecPluginManager = NULL;
    
    /*
     *      Shutdown centralized output last
     */
    tspshutdown();
          
    /*
     *      Very important: Shutdown the memory manager last !!!!!
     */
    #ifdef ADD_MEMORY_MANAGER 
    MEMMAN_Shutdown();
    #endif
    
    // it important to shutdown the call stack tracer as the last
    GUCEF_ShutdowntCallstackUtility();
}

/*-------------------------------------------------------------------------*/

CConfigStore&
CCoreGlobal::GetConfigStore( void )
{GUCEF_TRACE;

    return *m_configStore;
}

/*-------------------------------------------------------------------------*/

CGUCEFApplication&
CCoreGlobal::GetApplication( void )
{GUCEF_TRACE;

    return *m_application;
}

/*-------------------------------------------------------------------------*/
    
CPulseGenerator&
CCoreGlobal::GetPulseGenerator( void )
{GUCEF_TRACE;

    return m_application->GetPulseGenerator();
}

/*-------------------------------------------------------------------------*/

CTaskManager&
CCoreGlobal::GetTaskManager( void )
{GUCEF_TRACE;

    return *m_taskManager;
}

/*-------------------------------------------------------------------------*/

CLogManager&
CCoreGlobal::GetLogManager( void )
{GUCEF_TRACE;

    return *m_logManager;
}

/*-------------------------------------------------------------------------*/

CPluginControl&
CCoreGlobal::GetPluginControl( void )
{GUCEF_TRACE;

    return *m_pluginControl;
}

/*-------------------------------------------------------------------------*/

CSysConsole&
CCoreGlobal::GetSysConsole( void )
{GUCEF_TRACE;
    
    return *m_sysConsole;
}

/*-------------------------------------------------------------------------*/

CNotificationIDRegistry&
CCoreGlobal::GetNotificationIDRegistry( void )
{GUCEF_TRACE;

    return *m_notificationIdRegistry;
}

/*-------------------------------------------------------------------------*/

CURLHandlerRegistry&
CCoreGlobal::GetUrlHandlerRegistry( void )
{GUCEF_TRACE;

    return *m_urlHandlerRegistry;
}

/*-------------------------------------------------------------------------*/

CDStoreCodecRegistry&
CCoreGlobal::GetDStoreCodecRegistry( void )
{GUCEF_TRACE;

    return *m_dstoreCodecRegistry;
}

/*-------------------------------------------------------------------------*/

CDStoreCodecPluginManager&
CCoreGlobal::GetDStoreCodecPluginManager( void )
{GUCEF_TRACE;

    return *m_dstoreCodecPluginManager;
}

/*-------------------------------------------------------------------------*/

CGenericPluginManager&
CCoreGlobal::GetGenericPluginManager( void )
{GUCEF_TRACE;

    return *m_genericPluginManager;
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager&
CCoreGlobal::GetStdCodecPluginManager( void )
{GUCEF_TRACE;

    return *m_stdCodecPluginManager;
}

/*-------------------------------------------------------------------------*/

CExclusiveActivationManager&
CCoreGlobal::GetExclusiveActivationManager( void )
{GUCEF_TRACE;

    return *m_exclusiveActivationManager;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/
