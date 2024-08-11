/*
 *  GucefArchiver: Tool to manipulate archives
 *
 *  Copyright (C) 1998 - 2024.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#include "GucefArchiver.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GucefArchiver::GucefArchiver( void )
    : CORE::CObservingNotifier()
    , m_console( GUCEF_NULL )
    , m_params()
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

GucefArchiver::~GucefArchiver()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
GucefArchiver::OnAppStarted( CORE::CNotifier* notifier    ,
                             const CORE::CEvent& eventId  ,
                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    if ( vfs.IsInitialized() )
        PerformWork();
}

/*-------------------------------------------------------------------------*/

void
GucefArchiver::OnVfsInitializationCompleted( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

    PerformWork();
}

/*-------------------------------------------------------------------------*/

void 
GucefArchiver::RegisterEventHandlers( void )
{GUCEF_TRACE;

    CORE::CGUCEFApplication& app = CORE::CCoreGlobal::Instance()->GetApplication();
    
    TEventCallback callback( this, &GucefArchiver::OnAppStarted );
    SubscribeTo( &app                                     ,
                 CORE::CGUCEFApplication::FirstCycleEvent ,
                 callback                                 );

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    TEventCallback callback2( this, &GucefArchiver::OnVfsInitializationCompleted );
    SubscribeTo( &vfs                                       ,
                 VFS::CVFS::VfsInitializationCompletedEvent ,
                 callback2                                  );

}

/*-------------------------------------------------------------------------*/

void 
GucefArchiver::SetConsoleAccess( CORE::CPlatformNativeConsoleLogger& console )
{GUCEF_TRACE;

    m_console = &console;
}

/*-------------------------------------------------------------------------*/

void 
GucefArchiver::SetParams( const CORE::CValueList& keyValueList )
{GUCEF_TRACE;

    m_params = keyValueList;
}

/*-------------------------------------------------------------------------*/

bool 
GucefArchiver::PerformWork( void )
{GUCEF_TRACE;

    CORE::CString outputDir = m_params.GetValueAlways( "outputDir" );

    CORE::CString archivePath = m_params.GetValueAlways( "archive" );
    archivePath = archivePath.Trim( true ).Trim( false );
    if ( !archivePath.IsNULLOrEmpty() )
    {
        CORE::CString archiveType = m_params.GetValueAlways( "archiveType" );
        if ( archiveType.IsNULLOrEmpty() )
        {
            archiveType = CORE::ExtractFileExtention( archivePath ).Lowercase();
        }
        archiveType = archiveType.Trim( true ).Trim( false );

        bool listArchiveContents = CORE::StringToBool( m_params.GetValueAlways( "list", "true" ) );
        bool extractArchiveContents = CORE::StringToBool( m_params.GetValueAlways( "extract", "false" ) );
        bool overwrite = CORE::StringToBool( m_params.GetValueAlways( "overwrite", "false" ) );

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
                        if ( GUCEF_NULL != m_console )
                        {
                            m_console->GetLogger()->SetFormatAsConsoleUI( true );
                            GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, "***** BEGIN ARCHIVE CONTENTS *****" );
                            CORE::UInt32 n=1;
                            VFS::CVFS::TStringVector::iterator i = archiveFileContent.begin();
                            while ( i != archiveFileContent.end() )
                            {
                                GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, CORE::UInt32ToString( n ) + ": " + (*i) );
                                ++i; ++n;
                            }
                            GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, "***** END ARCHIVE CONTENTS *****" );
                            m_console->GetLogger()->SetFormatAsConsoleUI( false );
                        }
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
                                        CORE::UInt64 fileSize = fileAccess->GetSize();
                                        CORE::UInt64 bytesExtracted = extractedFile.Write( *fileAccess );
                                        extractedFile.Close();

                                        if ( fileSize == bytesExtracted )
                                        {
                                            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Extracted " + CORE::ToString( bytesExtracted ) + " bytes into file: " + extractedFilePath );
                                        }
                                        else
                                        {
                                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Extracted " + CORE::ToString( bytesExtracted ) + " bytes instead of " +
                                                        CORE::ToString( fileSize ) + " expected bytes into file: " + extractedFilePath + ". The file is probably corrupt" );
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

    CORE::CCoreGlobal::Instance()->GetApplication().Stop();
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
