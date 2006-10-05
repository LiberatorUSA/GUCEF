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
#endif GUCEF_CORE_CFILEURLHANDLER_H

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

#include "CGUCEFCOREModule.h"  /* definition of the class implemented here */

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

CGUCEFCOREModule::CGUCEFCOREModule( void )
{
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOREModule::CGUCEFCOREModule( const CGUCEFCOREModule& src )
{
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOREModule::~CGUCEFCOREModule()
{
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOREModule&
CGUCEFCOREModule::operator=( const CGUCEFCOREModule& src )
{
        /* dummy, should never be used */
        return *this;
}
        
/*-------------------------------------------------------------------------*/        

bool 
CGUCEFCOREModule::Load( void )
{
        /*
         *      Verry important: Initialize the memory manager before anything else !!!!!
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
        #ifdef DEBUG_MODE        
        tssetcoutfile( "GUCEFLog.txt" );
        tsusecoutfile( 1 );
        #endif
        
        /*
         *      Instantiate all the singletons
         */
        CNotificationIDRegistry::Instance();        
        CPluginControl::Instance();         
        CDStoreCodecRegistry::Instance();
        CDStoreCodecPluginManager::Instance();
        CConfigStore::Instance();
        CURLHandlerRegistry::Instance();
        CGUCEFApplication::Instance();
        CSysConsole::Instance();
        CGenericPluginManager::Instance();
        
        /*
         *      Register events for objects that can be created dynamicly
         */
        CNotifier::RegisterEvents();
        CPluginManager::RegisterEvents();
        CIURLEvents::RegisterEvents();
        
        /*
         *      Register some default codecs/handlers 
         */
        CURLHandlerRegistry::Instance()->Register( "file", new CFileURLHandler() );
        
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFCOREModule::Unload( void )
{         
        DEBUGOUTPUT( "CGUCEFCOREModule::Unload( void )" );
               
        /*
         *      cleanup all singletons
         *      Take care to deinstance them in the correct order !!!
         */
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
         *      Verry important: Shutdown the memory manager last !!!!!
         */
        #ifdef ADD_MEMORY_MANAGER 
        MEMMAN_Shutdown();
        #endif
                
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
