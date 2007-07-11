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

#define ERRORHERE { printf( "Test failed @ %s(%d)\n", __FILE__, __LINE__ ); DebugBreak(); }

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
    CORE::GUCEF_LogStackToStdOut();
    CORE::GUCEF_SetStackLogging( 1 );
    #endif /* GUCEF_VFS_DEBUG_MODE ? */
    
    try
    {
        // See if we can manage to get access to the VFS
        VFS::CVFS* vfs = VFS::CVFS::Instance();
        if ( vfs == NULL )
        {
            ERRORHERE;
        }
        
        // We can use the module directory since it should hold the at least 1 file
        // which is the test app itself
        vfs->AddRoot( "$MODULEDIR$" );

        // We will use the root directory itself and ask for a list with no recursive
        // dir iteration and no filter.
        VFS::CVFS::TStringList fileList;
        vfs->GetList( fileList, "", false, "" );
                     fileList.pop_back();
        if ( fileList.size() == 0 )
        {
            // We should have at least 1 file in that dir
            ERRORHERE;
        }

        // We will attempt to load a number of files in sequence
        VFS::UInt32 maxFiles = (VFS::UInt32) fileList.size() > 5 ? 5 : (VFS::UInt32) fileList.size();
        std::vector< VFS::CVFS::CVFSHandlePtr > fileHandles;
        for ( VFS::UInt32 i=0; i<maxFiles; ++i )
        {
            // Try to load the file
            VFS::UInt32 errorCode = 0;
            VFS::CVFS::CVFSHandlePtr filePtr = vfs->GetFile( fileList[ i ] ,
                                                             errorCode     ,
                                                             "rb"          ,
                                                             false         );
            // check if the file was loaded
            if ( filePtr == NULL )
            {
                // We failed to load a file even though we know it is there
                // make sure the file is not being written to because that would invalidate this test
                ERRORHERE;
            }
            
            printf( "Loaded file %s\n", filePtr->GetFilename().C_String() );
            fileHandles.push_back( filePtr );
        }
        
        // Now we try to unload the files we loaded in sequence
        while ( !fileHandles.empty() )
        {
            // unloading should work automatically when we get rid of our
            // file handles
            fileHandles.pop_back();
        }
    }
    catch ( ... )
    {
        printf( "ERROR unhandled exception during test\n" );
        #ifdef GUCEF_VFS_DEBUG_MODE
        CORE::GUCEF_PrintCallstack();
        #endif /* GUCEF_VFS_DEBUG_MODE ? */
        ERRORHERE;
    } 
}

/*-------------------------------------------------------------------------*/
