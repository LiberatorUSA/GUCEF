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

#ifdef GUCEF_MSWIN_BUILD
  #include <windows.h>
#else
  #include <assert.h>
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MSWIN_BUILD
  #define DebugBreak() assert( 0 )
#endif

#define ERRORHERE { GUCEF_ERROR_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "Test failed @ " + CORE::CString( __FILE__ ) + CORE::UInt32ToString( __LINE__ ) ); DebugBreak(); }

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
{  
    #ifdef GUCEF_VFS_DEBUG_MODE
    //MEMAN_LogStackToStdOut();
    //MEMAN_SetStackLogging( 1 );
    #endif /* GUCEF_VFS_DEBUG_MODE ? */
    
    try
    {
        // See if we can manage to get access to the VFS
        VFS::CVFS& vfs = GUCEF::VFS::CVfsGlobal::Instance()->GetVfs();
        
        // We can use the module directory since it should hold the at least 1 file
        // which is the test app itself
        vfs.AddRoot( "$MODULEDIR$", "Test", true );

        // We will use the root directory itself and ask for a list with no recursive
        // dir iteration and no filter.
        VFS::CVFS::TStringVector fileList;
        vfs.GetFileList( fileList, "", false, "" );

        if ( fileList.size() == 0 )
        {
            // We should have at least 1 file in that dir
            ERRORHERE;
        }

        // We will attempt to load a number of files in sequence
        VFS::UInt32 maxFiles = (VFS::UInt32) fileList.size() > 5 ? 5 : (VFS::UInt32) fileList.size();
        std::vector< VFS::TBasicVfsResourcePtr > fileHandles;
        VFS::CVFS::TStringVector::iterator n = fileList.begin();
        for ( VFS::UInt32 i=0; i<maxFiles; ++i )
        {
            // Try to load the file
            VFS::UInt32 errorCode = 0;
            VFS::TBasicVfsResourcePtr filePtr = vfs.GetFile( (*n)  ,
                                                            "rb"  ,
                                                            false );
            // check if the file was loaded
            if ( filePtr.IsNULL() )
            {
                // We failed to load a file even though we know it is there
                // make sure the file is not being written to because that would invalidate this test
                ERRORHERE;
            }
            
            printf( "Loaded file %s\n", filePtr->GetFilename().C_String() );
            fileHandles.push_back( filePtr );
            ++n;
        }
        
        // Now we try to unload the files we loaded in sequence
        while ( !fileHandles.empty() )
        {
            // unloading should work automatically when we get rid of our
            // file handles
            fileHandles.pop_back();
        }
        
        // Create a new unique filename
        CORE::UInt32 i=0;
        CORE::CString newFilename;
        do
        {
            newFilename = "DummyVFSTestFile" + CORE::UInt32ToString( i ) + ".tmp";
            ++i;
        }    
        while ( vfs.FileExists( newFilename ) );
        
        VFS::TBasicVfsResourcePtr filePtr = vfs.GetFile( newFilename ,
                                                         "wb"        ,
                                                         true        );
    
        if ( filePtr.IsNULL() )
        {
            // We verified that the filename was unique, the operation should have suceeded
            ERRORHERE;
        }
        
        // Get access to the file
        CORE::IOAccessPtr access = filePtr->GetAccess();
        access->Open();
        
        // Create a buffer with test values
        char buffer[ 1024 ]; 
        CORE::UInt8 testValue = 0;
        for ( CORE::UInt32 n=0; n<1024; ++n )
        {
            buffer[ n ] = testValue;
            ++testValue;
        }
        
        // Write the test data
        for ( CORE::UInt32 n=0; n<1024; ++n )
        {
            if ( 1024 != access->Write( buffer, 1, 1024 ) )
            {
                // Failed to write data block to file
                ERRORHERE;
            }
        }
        access->Close();
        
        // Make sure there is no outstanding reference to our test file
        filePtr = NULL;
        
        // Try to load the file we created
        filePtr = vfs.GetFile( newFilename ,
                               "rb"        ,
                               false       );

        if ( NULL == filePtr )
        {
            // We created the file, we should be able to load it
            ERRORHERE;
        }
        
        access = filePtr->GetAccess();
        access->Open();
        
        char readBuffer[ 1024 ];
        for ( CORE::UInt32 n=0; n<1024; ++n )
        {
            if ( 1024 != access->Read( readBuffer, 1, 1024 ) )
            {
                // Failed to read full data block from file
                ERRORHERE;
            }
            
            if ( memcmp( buffer, readBuffer, 1024 ) != 0 )
            {
                // The data found does not match what we expected
                ERRORHERE;            
            }
        }
        
        GUCEF_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "Finished all load unload tests" );
        
    }
    catch ( ... )
    {
        GUCEF_ERROR_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "unhandled exception during test" );
        #ifdef GUCEF_VFS_DEBUG_MODE
        //GUCEF::MEMAN_PrintCallstack();
        #endif /* GUCEF_VFS_DEBUG_MODE ? */
        ERRORHERE;
    } 
}

/*-------------------------------------------------------------------------*/
