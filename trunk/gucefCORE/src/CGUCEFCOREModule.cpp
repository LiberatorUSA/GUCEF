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

#ifndef GUCEF_CORE_CALLSTACK_H
#include "callstack.h"
#define GUCEF_CORE_CALLSTACK_H
#endif /* GUCEF_CORE_CALLSTACK_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"               /* centralized configuration management */
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"             /* centralized plugin management */
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"          /* main application class */
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"       /* registry for data storage codecs */
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#include "CDStoreCodecPluginManager.h"  /* plugin manager for data storage codecs */
#define GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#include "CURLHandlerRegistry.h"        /* Registry for URL handlers */
#define GUCEF_CORE_CURLHANDLERREGISTRY_H
#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */

#ifndef GUCEF_CORE_CSYSCONSOLE_H
#include "CSysConsole.h"
#define GUCEF_CORE_CSYSCONSOLE_H
#endif /* GUCEF_CORE_CSYSCONSOLE_H ? */

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef GUCEF_CORE_CFILEURLHANDLER_H
#include "CFileURLHandler.h"
#define GUCEF_CORE_CFILEURLHANDLER_H
#endif /* GUCEF_CORE_CFILEURLHANDLER_H ? */

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_CGENERICPLUGINMANAGER_H
#include "CGenericPluginManager.h"
#define GUCEF_CORE_CGENERICPLUGINMANAGER_H
#endif /* GUCEF_CORE_CGENERICPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CIURLEVENTS_H
#include "CURL.h"
#define GUCEF_CORE_CIURLEVENTS_H
#endif /* GUCEF_CORE_CIURLEVENTS_H ? */

#ifndef GUCEF_CORE_CNOTIFYINGMAPEVENTS_H
#include "CNotifyingMapEvents.h"
#define GUCEF_CORE_CNOTIFYINGMAPEVENTS_H
#endif /* GUCEF_CORE_CNOTIFYINGMAPEVENTS_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CSTDCODECPLUGINMANAGER_H
#include "CStdCodecPluginManager.h"
#define GUCEF_CORE_CSTDCODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CSTDCODECPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_CORE_CSTREAMEREVENTS_H
#include "CStreamerEvents.h"
#define GUCEF_CORE_CSTREAMEREVENTS_H
#endif /* GUCEF_CORE_CSTREAMEREVENTS_H ? */

#ifndef GUCEF_CORE_CTASKDELEGATOR_H
#include "gucefCORE_CTaskDelegator.h"
#define GUCEF_CORE_CTASKDELEGATOR_H
#endif /* GUCEF_CORE_CTASKDELEGATOR_H ? */

#ifndef GUCEF_CORE_CTASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CTASKCONSUMER_H
#endif /* GUCEF_CORE_CTASKCONSUMER_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_TSPRINTING_H
#include "tsprinting.h"
#define GUCEF_CORE_TSPRINTING_H
#endif /* GUCEF_CORE_TSPRINTING_H ? */

#include "CGUCEFCOREModule.h"  /* definition of the class implemented here */

#ifdef GUCEF_MSWIN_BUILD
  #ifndef GUCEF_CORE_CWNDMSGHOOKNOTIFIER_H
  #include "CWndMsgHookNotifier.h"
  #define GUCEF_CORE_CWNDMSGHOOKNOTIFIER_H
  #endif /* GUCEF_CORE_CWNDMSGHOOKNOTIFIER_H ? */
#endif /* GUCEF_MSWIN_BUILD ? */  

#ifdef ADD_MEMORY_MANAGER
  #ifndef GUCEF_CORE_MEMORYMANAGER_H
  #include "MemoryManager.h"    /* memory manager API */
  #define GUCEF_CORE_MEMORYMANAGER_H
  #endif /* GUCEF_CORE_MEMORYMANAGER_H ? */
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

bool 
CGUCEFCOREModule::Load( void )
{
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
        CLogManager::Instance();
        CNotificationIDRegistry::Instance();

        /*
         *  Make sure all events are registered from the start
         */
        CNotifier::RegisterEvents();
        CPulseGenerator::RegisterEvents();
        CStreamerEvents::RegisterEvents();
        CTimer::RegisterEvents();
        CPluginManager::RegisterEvents();
        CIURLEvents::RegisterEvents();
        CNotifyingMapEvents::RegisterEvents();
        CGUCEFApplication::RegisterEvents();
        CTaskDelegator::RegisterEvents();
        CTaskConsumer::RegisterEvents();
        CTaskManager::RegisterEvents();        

        /*
         *  Instantiate the rest of the singletons
         */
        CPluginControl::Instance();         
        CDStoreCodecRegistry::Instance();
        CDStoreCodecPluginManager::Instance();
        CConfigStore::Instance();
        CTaskManager::Instance();
        CURLHandlerRegistry::Instance();
        CGUCEFApplication::Instance();
        CSysConsole::Instance();
        CGenericPluginManager::Instance();
        CStdCodecPluginManager::Instance();
        


        #ifdef GUCEF_MSWIN_BUILD
        CWndMsgHookNotifier::RegisterEvents();
        #endif /* GUCEF_MSWIN_BUILD ? */ 
        
        /*
         *      Register some default codecs/handlers 
         */
        CURLHandlerRegistry::Instance()->Register( "file", new CFileURLHandler() );
        
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "gucefCORE module initialized" );
        
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFCOREModule::Unload( void )
{         
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Unloading gucefCORE module" );
               
        /*
         *      cleanup all singletons
         *      Take care to deinstance them in the correct order !!!
         */
        CTaskManager::Deinstance();
        CStdCodecPluginManager::Deinstance();
        CGenericPluginManager::Deinstance(); 
        CSysConsole::Deinstance(); 
        CGUCEFApplication::Deinstance();
        CURLHandlerRegistry::Deinstance();
        CConfigStore::Deinstance();
        CDStoreCodecPluginManager::Deinstance();
        CDStoreCodecRegistry::Deinstance();
        CPluginControl::Deinstance();
        CNotificationIDRegistry::Deinstance();
        
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
                        
        return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
