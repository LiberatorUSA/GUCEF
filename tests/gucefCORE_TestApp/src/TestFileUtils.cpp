/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
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

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
  #include <sys/stat.h>
  #include <unistd.h>
#endif

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#include "TestFileUtils.h"

using namespace GUCEF;

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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
TestRelativePathFunction( void )
{
    try
    {
        // test that the $CURWORKDIR$ macro gets converted
        CORE::CString relPath = CORE::RelativePath( "$CURWORKDIR$/test.txt", true );
        ASSERT_TRUE( !relPath.IsNULLOrEmpty() );
        ASSERT_TRUE( !relPath.StartsWith( "$CURWORKDIR$" ) );
        ASSERT_TRUE( relPath.EndsWith( "test.txt" ) );
        CORE::CString::StringVector elements = relPath.ParseElements( GUCEF_DIRSEPCHAR );
        ASSERT_TRUE( elements.size() > 1 );

        // test that the $MODULEDIR$ macro gets converted
        relPath = CORE::RelativePath( "$MODULEDIR$/test.txt", true );
        ASSERT_TRUE( !relPath.IsNULLOrEmpty() );
        ASSERT_TRUE( !relPath.StartsWith( "$MODULEDIR$" ) );
        ASSERT_TRUE( relPath.EndsWith( "test.txt" ) );
        elements = relPath.ParseElements( GUCEF_DIRSEPCHAR );
        ASSERT_TRUE( elements.size() > 1 );

        // test that the function converts paths to use the O/S native dir separator
        relPath = CORE::RelativePath( "root/dir1/dir2/dir3/test.txt", true );
        ASSERT_TRUE( !relPath.IsNULLOrEmpty() );
        ASSERT_TRUE( relPath == CORE::CString( "root/dir1/dir2/dir3/test.txt" ).ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
        elements = relPath.ParseElements( GUCEF_DIRSEPCHAR );
        ASSERT_TRUE( elements.size() == 5 );

        // test that the function simplifies paths to reduce 'up dir' segments aka ../ if feasible
        relPath = CORE::RelativePath( "root/dir1/../dir3/../test.txt", true );
        ASSERT_TRUE( !relPath.IsNULLOrEmpty() );
        ASSERT_TRUE( relPath == CORE::CString( "root/test.txt" ).ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
        elements = relPath.ParseElements( GUCEF_DIRSEPCHAR );
        ASSERT_TRUE( elements.size() == 2 );
    }
    catch (...)
    {
        ERRORHERE;
    }
}
/*-------------------------------------------------------------------------*/

void
PerformFileUtilsTests( void )
{
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "COMMENCING File Utils TESTS" );
    
    GUCEF_TESTFW_SUITE_SCOPE( "FileUtils" );

    GUCEF_TESTFW_TESTCASE( "Test: RelativePath function" )
        TestRelativePathFunction();
    GUCEF_TESTFW_TESTCASE_END

    // Regression: StripFilename() returned the input as-is when there was no directory part
    // which caused CFileAccess to create a directory named after the file
    GUCEF_TESTFW_TESTCASE( "Test: StripFilename edge cases" )
        try
        {
            ASSERT_TRUE( CORE::StripFilename( "file.txt" ).IsNULLOrEmpty() );
            CORE::CString dirSep( GUCEF_DIRSEPCHAR );
            ASSERT_TRUE( CORE::StripFilename( dirSep + "file.txt" ) == dirSep );
            ASSERT_TRUE( CORE::StripFilename( "dir" + dirSep + "file.txt" ) == "dir" );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: on Linux CreateDirs() failed for absolute paths and looped forever on relative paths with multiple segments
    GUCEF_TESTFW_TESTCASE( "Test: CreateDirs with absolute and multi segment paths" )
        try
        {
            CORE::CString nestedDir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_CreateDirs/a/b/c" );
            ASSERT_TRUE( CORE::CreateDirs( nestedDir ) );
            ASSERT_TRUE( CORE::DirExists( nestedDir ) );
            // creating it again must also succeed
            ASSERT_TRUE( CORE::CreateDirs( nestedDir ) );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: TempDir() had no implementation outside of Windows
    GUCEF_TESTFW_TESTCASE( "Test: TempDir returns an existing directory" )
        try
        {
            CORE::CString tempDir = CORE::TempDir();
            ASSERT_TRUE( !tempDir.IsNULLOrEmpty() );
            ASSERT_TRUE( CORE::DirExists( tempDir ) );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: on Linux CFileSystemIterator changed the process wide working dir and resolved entries
    // relative to it, so concurrent iterators (other threads or interleaved use) misclassified entries
    GUCEF_TESTFW_TESTCASE( "Test: CFileSystemIterator does not depend on the working dir" )
        try
        {
            CORE::CString rootDir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_FsIterator" );
            CORE::CString dirA = CORE::CombinePath( rootDir, "A" );
            CORE::CString dirB = CORE::CombinePath( rootDir, "B" );
            ASSERT_TRUE( CORE::CreateDirs( CORE::CombinePath( dirA, "subDirA" ) ) );
            ASSERT_TRUE( CORE::CreateDirs( dirB ) );
            ASSERT_TRUE( CORE::WriteStringAsTextFile( CORE::CombinePath( dirA, "fileA1.txt" ), "A1" ) );
            ASSERT_TRUE( CORE::WriteStringAsTextFile( CORE::CombinePath( dirA, "fileA2.txt" ), "A2" ) );
            ASSERT_TRUE( CORE::WriteStringAsTextFile( CORE::CombinePath( dirB, "fileB1.txt" ), "B1" ) );

            CORE::CString cwdBefore = CORE::CurrentWorkingDir();

            // Interleave two iterators on different dirs, entries of A must still be classified correctly
            CORE::CFileSystemIterator itA;
            CORE::CFileSystemIterator itB;
            ASSERT_TRUE( itA.FindFirst( dirA ) );
            ASSERT_TRUE( itB.FindFirst( dirB ) );

            UInt32 filesInA = 0;
            UInt32 dirsInA = 0;
            do
            {
                CORE::CString name = itA.GetResourceName();
                ASSERT_TRUE( itA.GetResourcePath() == CORE::CombinePath( dirA, name ) );
                if ( itA.IsAFile() )
                {
                    ASSERT_TRUE( name.StartsWith( "fileA" ) );
                    ++filesInA;
                }
                else
                if ( itA.IsADirectory() )
                {
                    ASSERT_TRUE( name == "subDirA" );
                    ++dirsInA;
                }
                itB.FindNext();
            }
            while ( itA.FindNext() );
            itA.FindClose();
            itB.FindClose();

            ASSERT_TRUE( 2 == filesInA );
            ASSERT_TRUE( 1 == dirsInA );
            ASSERT_TRUE( cwdBefore == CORE::CurrentWorkingDir() );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: CFileAccess::Sync flushes to storage" )
        try
        {
            CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_Durable" );
            ASSERT_TRUE( CORE::CreateDirs( dir ) );
            CORE::CString path = CORE::CombinePath( dir, "synced.txt" );

            CORE::CFileAccess unopened;
            ASSERT_FALSE( unopened.Sync() );

            {
                CORE::CFileAccess file( path, "wb" );
                ASSERT_TRUE( file.IsValid() );
                ASSERT_TRUE( 5 == file.Write( "hello", 1, 5 ) );
                ASSERT_TRUE( file.Sync() );

                // the data must be readable by others while the file is still open
                CORE::CString content;
                ASSERT_TRUE( CORE::LoadTextFileAsString( path, content, false ) );
                ASSERT_TRUE( content == "hello" );
            }
            CORE::DeleteFile( path );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: WriteFileAtomically creates and replaces files without leftovers" )
        try
        {
            CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_Atomic" );
            ASSERT_TRUE( CORE::CreateDirs( dir ) );
            CORE::CString path = CORE::CombinePath( dir, "target.txt" );
            if ( CORE::FileExists( path ) )
                CORE::DeleteFile( path );

            // new file
            ASSERT_TRUE( CORE::WriteFileAtomically( path, "first version", 13 ) );
            CORE::CString content;
            ASSERT_TRUE( CORE::LoadTextFileAsString( path, content, false ) );
            ASSERT_TRUE( content == "first version" );

            // replace with a different size
            CORE::CDynamicBuffer buffer;
            buffer.CopyFrom( 6, "second" );
            ASSERT_TRUE( CORE::WriteFileAtomically( path, buffer ) );
            ASSERT_TRUE( CORE::LoadTextFileAsString( path, content, false ) );
            ASSERT_TRUE( content == "second" );

            // empty content
            ASSERT_TRUE( CORE::WriteFileAtomically( path, GUCEF_NULL, 0 ) );
            ASSERT_TRUE( CORE::LoadTextFileAsString( path, content, false ) );
            ASSERT_TRUE( content.IsNULLOrEmpty() );

            // only the target may remain in the directory
            UInt32 fileCount = 0;
            CORE::CFileSystemIterator it;
            if ( it.FindFirst( dir ) )
            {
                do
                {
                    ++fileCount;
                    ASSERT_TRUE( it.GetResourceName() == "target.txt" );
                }
                while ( it.FindNext() );
                it.FindClose();
            }
            ASSERT_TRUE( 1 == fileCount );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: WriteFileAtomically leaves the original intact when the replace fails" )
        try
        {
            CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_AtomicFail" );
            ASSERT_TRUE( CORE::CreateDirs( dir ) );

            // Case 1: the target is a directory so moving the temp file over it fails after the temp file was written
            CORE::CString dirTarget = CORE::CombinePath( dir, "isadir" );
            ASSERT_TRUE( CORE::CreateDirs( dirTarget ) );
            ASSERT_FALSE( CORE::WriteFileAtomically( dirTarget, "data", 4 ) );
            ASSERT_TRUE( CORE::DirExists( dirTarget ) );

            #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

            // Case 2: the temp file cannot be written at all. Not applicable when running as root
            if ( 0 != geteuid() )
            {
                CORE::CString lockedDir = CORE::CombinePath( dir, "locked" );
                ASSERT_TRUE( CORE::CreateDirs( lockedDir ) );
                CORE::CString lockedFile = CORE::CombinePath( lockedDir, "original.txt" );
                ASSERT_TRUE( CORE::WriteStringAsTextFile( lockedFile, "original" ) );
                ::chmod( lockedDir.C_String(), 0555 );

                bool replaced = CORE::WriteFileAtomically( lockedFile, "replacement", 11 );
                ::chmod( lockedDir.C_String(), 0755 );

                ASSERT_FALSE( replaced );
                CORE::CString content;
                ASSERT_TRUE( CORE::LoadTextFileAsString( lockedFile, content, false ) );
                ASSERT_TRUE( content == "original" );
            }

            #endif

            // no temp files may be left behind in the dir
            UInt32 entryCount = 0;
            CORE::CFileSystemIterator it;
            if ( it.FindFirst( dir ) )
            {
                do
                {
                    ++entryCount;
                    ASSERT_FALSE( it.GetResourceName().HasSubstr( ".tmp" ) >= 0 );
                }
                while ( it.FindNext() );
                it.FindClose();
            }
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: on Linux MoveFile ignored the overwrite flag and always replaced an existing target
    GUCEF_TESTFW_TESTCASE( "Test: MoveFile honors the overwrite flag" )
        try
        {
            CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_MoveFile" );
            ASSERT_TRUE( CORE::CreateDirs( dir ) );
            CORE::CString source = CORE::CombinePath( dir, "source.txt" );
            CORE::CString target = CORE::CombinePath( dir, "target.txt" );
            ASSERT_TRUE( CORE::WriteStringAsTextFile( source, "source" ) );
            ASSERT_TRUE( CORE::WriteStringAsTextFile( target, "target" ) );

            ASSERT_FALSE( CORE::MoveFile( source, target, false ) );
            CORE::CString content;
            ASSERT_TRUE( CORE::LoadTextFileAsString( target, content, false ) );
            ASSERT_TRUE( content == "target" );
            ASSERT_TRUE( CORE::FileExists( source ) );

            ASSERT_TRUE( CORE::MoveFile( source, target, true ) );
            ASSERT_TRUE( CORE::LoadTextFileAsString( target, content, false ) );
            ASSERT_TRUE( content == "source" );
            ASSERT_FALSE( CORE::FileExists( source ) );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: on Linux FileExists also returned true for directories, unlike on Windows
    GUCEF_TESTFW_TESTCASE( "Test: FileExists is only true for files and DirExists only for directories" )
        try
        {
            CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_FileExists" );
            ASSERT_TRUE( CORE::CreateDirs( dir ) );
            CORE::CString file = CORE::CombinePath( dir, "file.txt" );
            ASSERT_TRUE( CORE::WriteStringAsTextFile( file, "content" ) );

            ASSERT_TRUE( CORE::FileExists( file ) );
            ASSERT_FALSE( CORE::DirExists( file ) );
            ASSERT_FALSE( CORE::FileExists( dir ) );
            ASSERT_TRUE( CORE::DirExists( dir ) );
            ASSERT_FALSE( CORE::FileExists( CORE::CombinePath( dir, "nothing.txt" ) ) );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    CORE::CLogStreamScope::FlushLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ALL File Utils TESTS COMPLETED" );
}

/*-------------------------------------------------------------------------*/
