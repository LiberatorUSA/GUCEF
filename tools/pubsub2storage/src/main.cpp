/*
 *  pubsub2storage: service which transfers between pubsub and storage
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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#ifndef GUCEF_CORE_CROLLINGFILEACCESS_H
#include "gucefCORE_CRollingFileAccess.h"
#define GUCEF_CORE_CROLLINGFILEACCESS_H
#endif /* GUCEF_CORE_CROLLINGFILEACCESS_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

#ifndef GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H
#include "gucefCORE_CPlatformNativeConsoleWindow.h"
#define GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H
#endif /* GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CCOMCOREGLOBAL_H
#include "gucefCOMCORE_CComCoreGlobal.h"
#define GUCEF_COMCORE_CCOMCOREGLOBAL_H
#endif /* GUCEF_COMCORE_CCOMCOREGLOBAL_H ? */

#ifndef GUCEF_COM_CCOMGLOBAL_H
#include "gucefCOM_CComGlobal.h"
#define GUCEF_COM_CCOMGLOBAL_H
#endif /* GUCEF_COM_CCOMGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_WEB_CWEBGLOBAL_H
#include "gucefWEB_CWebGlobal.h"
#define GUCEF_WEB_CWEBGLOBAL_H
#endif /* GUCEF_WEB_CWEBGLOBAL_H ? */

#include "pubsub2storage.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
LoadConfig( const CORE::CString& configPath            ,
            CORE::CValueList& keyValueList             ,
            CORE::CDataNode* loadedConfig = GUCEF_NULL )
{GUCEF_TRACE;

    #ifdef GUCEF_DEBUG_MODE
    const CORE::CString configFile = "pubsub2storage_d.ini";
    #else
    const CORE::CString configFile = "pubsub2storage.ini";
    #endif

    CORE::CString configFilePath;
    bool foundViaParam = false;
    if ( !configPath.IsNULLOrEmpty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configPath );
        foundViaParam = CORE::FileExists( configPath );
        configFilePath = configPath;
    }

    if ( !foundViaParam )
    {
        configFilePath = CORE::CombinePath( "$CURWORKDIR$", configFile );
        configFilePath = CORE::RelativePath( configFilePath );

        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
        if ( !CORE::FileExists( configFilePath ) )
        {
            configFilePath = CORE::CombinePath( "$MODULEDIR$", configFile );
            configFilePath = CORE::RelativePath( configFilePath );

            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
            if ( !FileExists( configFilePath ) )
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Unable to locate any config file, will rely on params" );
                return false;
            }
        }
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Located config file @ " + configFilePath );

    keyValueList.SetConfigNamespace( "Main/AppArgs" );
    keyValueList.SetUseGlobalConfig( true );
    keyValueList.SetAllowDuplicates( false );
    keyValueList.SetAllowMultipleValues( true );

    CORE::CConfigStore& configStore = CORE::CCoreGlobal::Instance()->GetConfigStore();
    configStore.SetConfigFile( configFilePath );
    return configStore.LoadConfig( loadedConfig );
}

/*-------------------------------------------------------------------------*/

void
ParseParams( const int argc                 ,
             char* argv[]                   ,
             CORE::CValueList& keyValueList )
{GUCEF_TRACE;

    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        // Combine the argument strings back into a single string because we don't want to use
        // a space as the seperator
        for ( int i=1; i<argc; ++i )
        {
            argString += ' ' + CORE::CString( argv[ i ] );
        }

        // Parse the param list based on the ' symbol
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Application parameters: " + argString );
        keyValueList.SetMultiple( argString, '*' );
    }
}

/*-------------------------------------------------------------------------*/

void
GucefAppSignalHandler( int signal )
{GUCEF_TRACE;
    
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "GucefAppSignalHandler: Received signal " + CORE::ToString( signal ) );
    ::GUCEF::CORE::CCoreGlobal::Instance()->GetApplication().Stop();
}

/*-------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
//GUCEF_OSSERVICEMAIN_BEGIN( "pubsub2storage" )
{GUCEF_TRACE;

    int returnValue = -100;
    try
    {

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "This service was compiled on: " + CORE::CDateTime::CompileDateTime( __DATE__, __TIME__ ).ToIso8601DateTimeString( true, true ) );

    CORE::CCoreGlobal::Instance();
    COMCORE::CComCoreGlobal::Instance();
    COM::CComGlobal::Instance();
    VFS::CVfsGlobal::Instance();
    WEB::CWebGlobal::Instance();

    // Check for config param first
    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );
    CORE::CString configPathParam = keyValueList.GetValueAlways( "ConfigPath" );
    keyValueList.Clear();

    // Load settings from a config file (if any) and then override with params (if any)
    CORE::CDataNode* globalConfig = new CORE::CDataNode();
    LoadConfig( configPathParam, keyValueList, globalConfig );
    ParseParams( argc, argv, keyValueList );

    CORE::Int32 minLogLevel = CORE::LOGLEVEL_BELOW_NORMAL;
    CORE::CString valueStr = keyValueList.GetValueAlways( "MinimalLogLevel" );
    if ( !valueStr.IsNULLOrEmpty() )
    {
        minLogLevel = CORE::StringToInt32( valueStr );
        CORE::CCoreGlobal::Instance()->GetLogManager().SetMinLogLevel( minLogLevel );
    }

    CORE::CString outputDir = CORE::RelativePath( keyValueList.GetValueAlways( "outputDir" ) );
    if ( outputDir.IsNULLOrEmpty() )
    {
        outputDir = CORE::RelativePath( "$CURWORKDIR$" );
    }
    CORE::CreateDirs( outputDir );

    CORE::CString logFilename = CORE::CombinePath( outputDir, "pubsub2storage_log.txt" );

    keyValueList.Set( "logfile", logFilename );

    CORE::CRollingFileAccess logFileAccess( logFilename, "w" );
    logFileAccess.SetMaxRolloverFilesBeforeDeletion( 10 );
    CORE::CStdLogger logger( logFileAccess );
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &logger );

    CORE::CPlatformNativeConsoleLogger console;
    if ( GUCEF_APP_TYPE == GUCEF_APP_TYPE_CONSOLE )
        CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( console.GetLogger() );
           
    CORE::CCoreGlobal::Instance()->GetLogManager().FlushBootstrapLogEntriesToLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Flushed to log @ " + logFilename );

    GUCEF_OSMAIN_SIGNAL_HANDLER( GucefAppSignalHandler );

    PubSub2Storage pubSub2Storage;
    if ( !pubSub2Storage.LoadConfig( *globalConfig ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "pubsub2storage: Exiting because LoadConfig of global config failed" );
        delete globalConfig;
        return -1;
    }
    delete globalConfig;
    globalConfig = GUCEF_NULL;
    
    if ( !pubSub2Storage.LoadConfig( keyValueList ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "pubsub2storage: Exiting because LoadConfig failed" );
        return -1;
    }
    
    if ( !pubSub2Storage.Start() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "pubsub2storage: Failed to Start()" );
        return -2;
    }

    auto& pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();
    pulseGenerator.RequestPulseInterval( 25 );
    pulseGenerator.RequestPulsesPerImmediatePulseRequest( 25 );
    
    auto& app = CORE::CCoreGlobal::Instance()->GetApplication();
    returnValue = app.main( argc, argv, true );
    pubSub2Storage.SetStandbyMode( true );

    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "pubsub2storage: Main thread stl exeption: " + CORE::CString( e.what() ) );
        throw e;
    }
    #ifndef GUCEF_DEBUG_MODE
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "pubsub2storage: Main thread unknown exeption occured. Caught in catch-all" );
        throw;
    }
    #endif

    return returnValue;
}
GUCEF_OSMAIN_END

/*---------------------------------------------------------------------------*/
