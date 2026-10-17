/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"          /* GUCEF CORE library API */
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_VFS_H
#include "gucefVFS.h"
#define GUCEF_VFS_H
#endif /* GUCEF_VFS_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestCode_VFSFileLoadUnloadTest.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

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

void
PerformVFSFileLoadUnloadTest( void )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "COMMENCING VFS file load/unload TESTS" );

    GUCEF_TESTFW_SUITE_SCOPE( "VFSFileLoadUnload" );

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

    GUCEF_TESTFW_TESTCASE( "Test: Files of a mounted directory can be listed, loaded and released" )
        try
        {
            // The module directory holds at least the test app itself
            ASSERT_TRUE( vfs.AddRoot( "$MODULEDIR$", "LoadUnloadModuleDir", false, false, "moduledir" ) );

            VFS::CVFS::TStringVector fileList;
            ASSERT_TRUE( vfs.GetFileList( fileList, "moduledir", false, true, CORE::CString::EmptyStringSet ) );
            ASSERT_FALSE( fileList.empty() );

            // Load a number of files at the same time
            UInt32 maxFiles = (UInt32) fileList.size() > 5 ? 5 : (UInt32) fileList.size();
            std::vector< VFS::TBasicVfsResourcePtr > fileHandles;
            VFS::CVFS::TStringVector::iterator n = fileList.begin();
            for ( UInt32 i=0; i<maxFiles; ++i, ++n )
            {
                VFS::TBasicVfsResourcePtr filePtr = vfs.GetFile( (*n), "rb", false );
                ASSERT_FALSE( filePtr.IsNULL() );
                if ( !filePtr.IsNULL() )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Loaded file " + filePtr->GetFilename() );
                    fileHandles.push_back( filePtr );
                }
            }
            ASSERT_TRUE( maxFiles == fileHandles.size() );

            // Releasing the handles unloads the files
            while ( !fileHandles.empty() )
                fileHandles.pop_back();

            ASSERT_TRUE( vfs.UnmountArchiveByName( "LoadUnloadModuleDir" ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: A file written through the VFS reads back identically" )
        try
        {
            CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefVFS_TestApp_LoadUnload" );
            ASSERT_TRUE( CORE::CreateDirs( dir ) );
            ASSERT_TRUE( vfs.AddRoot( dir, "LoadUnloadTempDir", true, false, "tempdir" ) );

            CORE::CString newFilename = "tempdir/DummyVFSTestFile.tmp";
            vfs.DeleteFile( newFilename, true );
            ASSERT_FALSE( vfs.FileExists( newFilename ) );

            // Create a buffer with test values
            char buffer[ 1024 ];
            UInt8 testValue = 0;
            for ( UInt32 n=0; n<1024; ++n )
                buffer[ n ] = (char) testValue++;

            VFS::TBasicVfsResourcePtr filePtr = vfs.GetFile( newFilename, "wb", true );
            ASSERT_FALSE( filePtr.IsNULL() );
            if ( !filePtr.IsNULL() )
            {
                CORE::IOAccessPtr access = filePtr->GetAccess();
                ASSERT_FALSE( access.IsNULL() );
                if ( !access.IsNULL() )
                {
                    access->Open();
                    bool allWritten = true;
                    for ( UInt32 n=0; n<1024 && allWritten; ++n )
                        allWritten = 1024 == access->Write( buffer, 1, 1024 );
                    ASSERT_TRUE( allWritten );
                    access->Close();
                }
            }

            // Make sure there is no outstanding reference to our test file and load it again
            filePtr = VFS::TBasicVfsResourcePtr();
            filePtr = vfs.GetFile( newFilename, "rb", false );
            ASSERT_FALSE( filePtr.IsNULL() );
            if ( !filePtr.IsNULL() )
            {
                CORE::IOAccessPtr access = filePtr->GetAccess();
                ASSERT_FALSE( access.IsNULL() );
                if ( !access.IsNULL() )
                {
                    access->Open();
                    char readBuffer[ 1024 ];
                    bool allRead = true;
                    bool allMatch = true;
                    for ( UInt32 n=0; n<1024 && allRead && allMatch; ++n )
                    {
                        allRead = 1024 == access->Read( readBuffer, 1, 1024 );
                        allMatch = 0 == memcmp( buffer, readBuffer, 1024 );
                    }
                    ASSERT_TRUE( allRead );
                    ASSERT_TRUE( allMatch );
                    access->Close();
                }
            }

            filePtr = VFS::TBasicVfsResourcePtr();
            ASSERT_TRUE( vfs.DeleteFile( newFilename, false ) );
            ASSERT_TRUE( vfs.UnmountArchiveByName( "LoadUnloadTempDir" ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    CORE::CLogStreamScope::FlushLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ALL VFS file load/unload TESTS COMPLETED" );
}

/*-------------------------------------------------------------------------*/
