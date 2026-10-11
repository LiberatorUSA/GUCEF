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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

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

    CORE::CLogStreamScope::FlushLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ALL File Utils TESTS COMPLETED" );
}

/*-------------------------------------------------------------------------*/
