/*
 *  GucefArchiver: Tool to manipulate archives
 *  Copyright (C) 2002 - 2015.  Dinand Vanvelzen
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

#include <stdio.h>

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CGLOBALCONFIGVALUELIST_H
#include "gucefCORE_CGlobalConfigValueList.h"
#define GUCEF_CORE_CGLOBALCONFIGVALUELIST_H
#endif /* GUCEF_CORE_CGLOBALCONFIGVALUELIST_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

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

CORE::CString
LookForConfigFile( const CORE::CString& configFile )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFile );
    if ( !CORE::FileExists( configFile ) )
    {
        CORE::CString configFilePath = CORE::CombinePath( "$CURWORKDIR$", configFile );
        configFilePath = CORE::RelativePath( configFilePath );

        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
        if ( !CORE::FileExists( configFilePath ) )
        {
            configFilePath = CORE::CombinePath( "$MODULEDIR$", configFile );
            configFilePath = CORE::RelativePath( configFilePath );

            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
            if ( !FileExists( configFilePath ) )
            {
                configFilePath = CORE::CombinePath( "$TEMPDIR$", configFile );
                configFilePath = CORE::RelativePath( configFilePath );

                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
                if ( !FileExists( configFilePath ) )
                {
                    return CORE::CString::Empty;
                }
            }
        }

        return configFilePath;
    }

    return configFile;
}

/*-------------------------------------------------------------------------*/

bool
LoadConfig( const CORE::CString& bootstrapConfigPath   ,
            const CORE::CString& configPath            ,
            CORE::CValueList& keyValueList             ,
            CORE::CDataNode* loadedConfig = GUCEF_NULL )
{GUCEF_TRACE;

    #ifdef GUCEF_DEBUG_MODE
    const CORE::CString bootstrapConfigFile = "GucefArchiver_bootstrap_d.ini";
    const CORE::CString configFile = "GucefArchiver_d.ini";
    #else
    const CORE::CString bootstrapConfigFile = "GucefArchiver_bootstrap.ini";
    const CORE::CString configFile = "GucefArchiver.ini";
    #endif

    CORE::CConfigStore& configStore = CORE::CCoreGlobal::Instance()->GetConfigStore();

    CORE::CString bootstrapConfigFilePath = LookForConfigFile( bootstrapConfigFile );
    CORE::CString configFilePath = LookForConfigFile( configFile );

    if ( !bootstrapConfigFilePath.IsNULLOrEmpty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Located bootstrap config file @ " + bootstrapConfigFilePath );
        configStore.SetBootstrapConfigFile( bootstrapConfigFilePath );
    }
    if ( !configFilePath.IsNULLOrEmpty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Located config file @ " + configFilePath );
        configStore.SetConfigFile( configFilePath );
    }
    if ( bootstrapConfigFilePath.IsNULLOrEmpty() && configFilePath.IsNULLOrEmpty() )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Unable to locate any config file, will rely on params only" );
    }

    CORE::CGlobalConfigValueList globalCfg;
    globalCfg.SetConfigNamespace( "Main/AppArgs" );
    globalCfg.SetAllowDuplicates( false );
    globalCfg.SetAllowMultipleValues( true );

    bool loadSuccess = configStore.LoadConfig( loadedConfig );

    keyValueList = globalCfg;
    return loadSuccess;
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
        // a space as the deliminator
        for ( int i=1; i<argc; ++i )
        {
            argString += ' ' + CORE::CString( argv[ i ] );
        }

        // Parse the param list based on the ' symbol
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Application parameters: " + argString );
        keyValueList.SetMultiple( argString, '\'' );
    }
}

/*---------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "This tool was compiled on: " __DATE__ " @ " __TIME__ );

    CORE::CCoreGlobal::Instance();
    VFS::CVfsGlobal::Instance();

    // Check for config param first
    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );
    CORE::CString bootstrapConfigPathParam = keyValueList.GetValueAlways( "BootstrapConfigPath" );
    CORE::CString configPathParam = keyValueList.GetValueAlways( "configPath" );
    keyValueList.Clear();

    // Load settings from a config file (if any) and then override with params (if any)
    CORE::CDataNode loadedConfig;
    LoadConfig( bootstrapConfigPathParam, configPathParam, keyValueList, &loadedConfig );
    ParseParams( argc, argv, keyValueList );

    CORE::Int32 minLogLevel = CORE::LOGLEVEL_BELOW_NORMAL;
    CORE::CString valueStr = keyValueList.GetValueAlways( "minimalLogLevel" );
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

    CORE::CString logFilename = CORE::CombinePath( outputDir, "GucefArchiver_Log.txt" );

    keyValueList.Set( "logfile", logFilename );

    CORE::CFileAccess logFileAccess( logFilename, "w" );
    CORE::CStdLogger logger( logFileAccess );
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &logger );

    CORE::CPlatformNativeConsoleLogger console;
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( console.GetLogger() );

    CORE::CCoreGlobal::Instance()->GetLogManager().FlushBootstrapLogEntriesToLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Flushed to log @ " + logFilename );

    if ( argc > 0 )
    {
        CORE::CString archivePath = keyValueList.GetValueAlways( "archive" );
        archivePath = archivePath.Trim( true ).Trim( false );
        if ( !archivePath.IsNULLOrEmpty() )
        {
            CORE::CString archiveType = keyValueList.GetValueAlways( "archiveType" );
            if ( archiveType.IsNULLOrEmpty() )
            {
                archiveType = CORE::ExtractFileExtention( archivePath ).Lowercase();
            }
            archiveType = archiveType.Trim( true ).Trim( false );

            bool listArchiveContents = CORE::StringToBool( keyValueList.GetValueAlways( "list", "true" ) );
            bool extractArchiveContents = CORE::StringToBool( keyValueList.GetValueAlways( "extract", "false" ) );
            bool overwrite = CORE::StringToBool( keyValueList.GetValueAlways( "overwrite", "false" ) );

            CORE::CString archiveFilename = CORE::ExtractFilename( archivePath );
            CORE::CString archiveDir = CORE::StripFilename( archivePath );

            VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

            if ( archiveDir.IsNULLOrEmpty() )
            {
                archiveDir = CORE::RelativePath( "$CURWORKDIR$" );
            }

            // Add the archive dir as a VFS root named root
            vfs.AddRoot( archiveDir, "root", false, false );

            VFS::CVFS::TStringVector archiveFileNames;
            if ( -1 < archiveFilename.HasChar( '*' ) )
            {
                // Use the filename as a filter
                vfs.GetFileList( archiveFileNames, CORE::CString::Empty, false, false, archiveFilename );
            }
            else
            {
                // directly use the filename
                archiveFileNames.push_back( archiveFilename );
            }

            // Batch extract each of the archives
            VFS::CVFS::TStringVector::iterator n = archiveFileNames.begin();
            while ( n != archiveFileNames.end() )
            {
                archiveFilename = (*n);

                if ( vfs.FileExists( archiveFilename ) )
                {
                    CORE::CString mount = '[' + archiveFilename + ']';

                    // Mount the archive
                    if ( vfs.MountArchive( mount, archiveFilename, archiveType, mount, false, false ) )
                    {
                        if ( listArchiveContents )
                        {
                            // Get the contents of the archive
                            VFS::CVFS::TStringVector archiveFileContent;
                            vfs.GetFileList( archiveFileContent, mount, true, true, CORE::CString::Empty );

                            // Write the contents to the log which includes the console
                            console.GetLogger()->SetFormatAsConsoleUI( true );
                            GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, "***** BEGIN ARCHIVE CONTENTS *****" );
                            CORE::UInt32 n=1;
                            VFS::CVFS::TStringVector::iterator i = archiveFileContent.begin();
                            while ( i != archiveFileContent.end() )
                            {
                                GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, CORE::UInt32ToString( n ) + ": " + (*i) );
                                ++i; ++n;
                            }
                            GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, "***** END ARCHIVE CONTENTS *****" );
                            console.GetLogger()->SetFormatAsConsoleUI( false );
                        }

                        if ( extractArchiveContents )
                        {
                            // Get the contents of the archive
                            VFS::CVFS::TStringVector archiveFileContent;
                            vfs.GetFileList( archiveFileContent, mount, true, true, CORE::CString::Empty );

                            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "archive contains " + CORE::ToString( archiveFileContent.size() ) + " entries" );

                            // prepare the output folder
                            CORE::CString archiveOutputDir = archiveFilename.ReplaceChar( '.', '_' );
                            archiveOutputDir = CORE::CombinePath( outputDir, archiveOutputDir );
                            if ( CORE::CreateDirs( archiveOutputDir ) )
                            {
                                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Output path " + archiveOutputDir + " is available for extraction" );

                                // Obtain access to each of the resources in the archive and extract it
                                VFS::CVFS::TStringVector::iterator i = archiveFileContent.begin();
                                while ( i != archiveFileContent.end() )
                                {
                                    CORE::CString filename = CORE::ExtractFilename( (*i) );
                                    CORE::CString vfsFilePath = CORE::CombinePath( mount, filename );

                                    CORE::CString extractedFilePath = filename;
                                    extractedFilePath = CORE::CombinePath( archiveOutputDir, extractedFilePath );

                                    if ( !overwrite )
                                    {
                                        // Check if destination file already exists
                                        if ( CORE::FileExists( extractedFilePath ) )
                                        {
                                            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Skipping extraction of file since the destination path already exists and 'overwrite=false'. Path: " + extractedFilePath );
                                            ++i;
                                            continue;
                                        }
                                    }

                                    VFS::TBasicVfsResourcePtr file = vfs.GetFile( vfsFilePath, "rb", false );
                                    if ( !file.IsNULL() )
                                    {
                                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Obtained access to resource: " + (*i) );

                                        CORE::CFileAccess extractedFile;
                                        if ( extractedFile.Open( extractedFilePath, "wb" ) )
                                        {
                                            CORE::IOAccessPtr fileAccess = file->GetAccess();
                                            CORE::UInt32 fileSize = fileAccess->GetSize();
                                            CORE::UInt32 bytesExtracted = extractedFile.Write( *fileAccess );
                                            extractedFile.Close();

                                            if ( fileSize == bytesExtracted )
                                            {
                                                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Extracted " + CORE::UInt32ToString( bytesExtracted ) + " bytes into file: " + extractedFilePath );
                                            }
                                            else
                                            {
                                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Extracted " + CORE::UInt32ToString( bytesExtracted ) + " bytes instead of " +
                                                         CORE::UInt32ToString( fileSize ) + " expected bytes into file: " + extractedFilePath + ". The file is probably corrupt" );
                                            }
                                        }
                                        else
                                        {
                                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to open target file for writing: " + extractedFilePath );
                                        }
                                    }
                                    else
                                    {
                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to obtain access to resource: " + filename );
                                    }
                                    ++i;
                                }
                            }
                        }

                        if ( !vfs.UnmountArchiveByName( mount ) )
                        {
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to unmount archive" );
                            break;
                        }
                    }
                    else
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Failed to mount archive" );
                    }
                }
                else
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Archive file does not exist, cannot proceed" );
                }

                ++n;
            }
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Missing minimal param \"archive\" thus no idea which archive to use" );
        }
    }

	GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Wrote log file to: " + logFilename );

    CORE::CCoreGlobal::Instance()->GetLogManager().ClearLoggers();

    return 0;
}
GUCEF_OSMAIN_END

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 25-08-2015 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
