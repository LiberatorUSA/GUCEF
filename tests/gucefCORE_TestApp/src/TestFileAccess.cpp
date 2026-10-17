/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#include <string.h>
#include <stdio.h>

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CMFILEACCESS_H
#include "CMFileAccess.h"
#define GUCEF_CORE_CMFILEACCESS_H
#endif /* GUCEF_CORE_CMFILEACCESS_H ? */

#ifndef GUCEF_CORE_CSUBIOACCESS_H
#include "gucefCORE_CSubIOAccess.h"
#define GUCEF_CORE_CSUBIOACCESS_H
#endif /* GUCEF_CORE_CSUBIOACCESS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <io.h>
    #include <fcntl.h>
    #include <sys/stat.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestFileAccess.h"

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static int
OpenDescriptorForReading( const CORE::CString& path )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    return _open( path.C_String(), _O_RDONLY | _O_BINARY );
    #else
    return open( path.C_String(), O_RDONLY );
    #endif
}

/*-------------------------------------------------------------------------*/

static void
CloseDescriptor( int fd )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    _close( fd );
    #else
    close( fd );
    #endif
}

/*-------------------------------------------------------------------------*/

void
PerformFileAccessTests( void )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "COMMENCING FileAccess TESTS" );

    GUCEF_TESTFW_SUITE_SCOPE( "FileAccess" );

    const char* payload = "0123456789abcdefghijklmnopqrstuvwxyz";
    const UInt32 payloadSize = (UInt32) strlen( payload );

    CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_FileAccess" );
    CORE::CreateDirs( dir );
    CORE::CString filePath = CORE::CombinePath( dir, "payload.bin" );
    {
        CORE::CFileAccess writer( filePath, "wb" );
        writer.Write( payload, 1, payloadSize );
    }

    GUCEF_TESTFW_TESTCASE( "Test: A file is read through a file descriptor that the access object owns" )
        try
        {
            int fd = OpenDescriptorForReading( filePath );
            ASSERT_TRUE( fd >= 0 );

            CORE::CFileAccess access;
            ASSERT_TRUE( access.OpenFromFileDescriptor( fd, "rb", true ) );
            ASSERT_TRUE( access.Opened() );
            ASSERT_TRUE( access.IsReadable() );
            ASSERT_FALSE( access.IsWriteable() );
            ASSERT_TRUE( access.IsValid() );
            ASSERT_TRUE( access.GetSize() == payloadSize );

            char buffer[ 64 ];
            memset( buffer, 0, sizeof( buffer ) );
            ASSERT_TRUE( access.Read( buffer, 1, 10 ) == 10 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload, 10 ) );
            ASSERT_TRUE( access.Tell() == 10 );

            // size queries must not disturb the position
            ASSERT_TRUE( access.GetSize() == payloadSize );
            ASSERT_TRUE( access.Tell() == 10 );

            ASSERT_TRUE( 0 == access.Setpos( 30 ) );
            ASSERT_TRUE( access.Read( buffer, 1, 6 ) == 6 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload + 30, 6 ) );
            ASSERT_TRUE( access.Tell() == payloadSize );

            ASSERT_TRUE( 0 == access.Seek( -5, SEEK_END ) );
            ASSERT_TRUE( access.Read( buffer, 1, 5 ) == 5 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload + payloadSize - 5, 5 ) );

            // No path means no independent clone
            ASSERT_TRUE( GUCEF_NULL == access.Clone() );

            access.Close();
            ASSERT_FALSE( access.Opened() );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: A descriptor that is borrowed stays open after the access object closes" )
        try
        {
            int fd = OpenDescriptorForReading( filePath );
            ASSERT_TRUE( fd >= 0 );
            {
                CORE::CFileAccess access;
                ASSERT_TRUE( access.OpenFromFileDescriptor( fd, "rb", false ) );
                ASSERT_TRUE( access.GetSize() == payloadSize );
                char c = 0;
                ASSERT_TRUE( access.Read( &c, 1, 1 ) == 1 );
                ASSERT_TRUE( c == payload[ 0 ] );
            }

            // The original descriptor must still be usable
            #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
            ASSERT_TRUE( _lseek( fd, 0, SEEK_SET ) >= 0 );
            #else
            ASSERT_TRUE( lseek( fd, 0, SEEK_SET ) >= 0 );
            #endif
            CloseDescriptor( fd );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: Opening an invalid descriptor fails and leaves the access object closed" )
        try
        {
            CORE::CFileAccess access;
            ASSERT_FALSE( access.OpenFromFileDescriptor( -1, "rb", true ) );
            ASSERT_FALSE( access.Opened() );
            ASSERT_FALSE( access.IsValid() );
            ASSERT_TRUE( access.GetSize() == 0 );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: A named file keeps working with the 64 bit capable positioning" )
        try
        {
            CORE::CFileAccess access( filePath, "rb" );
            ASSERT_TRUE( access.Opened() );
            ASSERT_TRUE( access.GetSize() == payloadSize );
            ASSERT_TRUE( 0 == access.Setpos( 20 ) );
            ASSERT_TRUE( access.Tell() == 20 );
            char c = 0;
            ASSERT_TRUE( access.Read( &c, 1, 1 ) == 1 );
            ASSERT_TRUE( c == payload[ 20 ] );
            ASSERT_TRUE( 0 == access.Seek( 2, SEEK_CUR ) );
            ASSERT_TRUE( access.Tell() == 23 );
            CORE::CICloneable* clone = access.Clone();
            ASSERT_FALSE( GUCEF_NULL == clone );
            delete clone;
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: A memory block is read through the memory access classes" )
        try
        {
            CORE::CMFileAccess memAccess( payload, payloadSize );
            ASSERT_TRUE( memAccess.GetSize() == payloadSize );
            char buffer[ 16 ];
            ASSERT_TRUE( 0 == memAccess.Setpos( 10 ) );
            ASSERT_TRUE( memAccess.Read( buffer, 1, 8 ) == 8 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload + 10, 8 ) );
            ASSERT_TRUE( memAccess.Tell() == 18 );

            // owning memory: a dynamic buffer that lives and dies with the access object
            CORE::CDynamicBuffer* owned = GUCEF_NEW CORE::CDynamicBuffer();
            owned->CopyFrom( payloadSize, payload );
            CORE::CDynamicBufferAccess ownedAccess( owned, true );
            ASSERT_TRUE( ownedAccess.GetSize() == payloadSize );
            ASSERT_TRUE( ownedAccess.Read( buffer, 1, 4 ) == 4 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload, 4 ) );

            // non owning view on external memory
            CORE::CDynamicBufferAccess linkedAccess;
            linkedAccess.LinkTo( payload, payloadSize );
            ASSERT_TRUE( linkedAccess.GetSize() == payloadSize );
            ASSERT_TRUE( linkedAccess.Read( buffer, 1, 3 ) == 3 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload, 3 ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: A window on another IO access reads only its own range" )
        try
        {
            CORE::IOAccessPtr parent( GUCEF_NEW CORE::CMFileAccess( payload, payloadSize ) );
            CORE::CSubIOAccess window( parent, 10, 8 );
            ASSERT_TRUE( window.Opened() );
            ASSERT_TRUE( window.IsValid() );
            ASSERT_TRUE( window.IsReadable() );
            ASSERT_FALSE( window.IsWriteable() );
            ASSERT_TRUE( window.GetSize() == 8 );
            ASSERT_TRUE( window.GetOffset() == 10 );
            ASSERT_TRUE( window.Tell() == 0 );

            char buffer[ 32 ];
            memset( buffer, 0, sizeof( buffer ) );
            ASSERT_TRUE( window.Read( buffer, 1, 4 ) == 4 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload + 10, 4 ) );
            ASSERT_TRUE( window.Tell() == 4 );
            ASSERT_FALSE( window.Eof() );

            // asking for more than is left returns what is left, never data outside of the window
            ASSERT_TRUE( window.Read( buffer, 1, 20 ) == 4 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload + 14, 4 ) );
            ASSERT_TRUE( window.Eof() );
            ASSERT_TRUE( window.Read( buffer, 1, 1 ) == 0 );

            // partial elements are not returned
            ASSERT_TRUE( 0 == window.Setpos( 5 ) );
            ASSERT_TRUE( window.Read( buffer, 2, 5 ) == 1 );
            ASSERT_TRUE( window.Tell() == 7 );

            // seeking
            ASSERT_TRUE( 0 == window.Seek( 2, SEEK_SET ) );
            ASSERT_TRUE( window.GetChar() == payload[ 12 ] );
            ASSERT_TRUE( 0 == window.Seek( 2, SEEK_CUR ) );
            ASSERT_TRUE( window.GetChar() == payload[ 15 ] );
            ASSERT_TRUE( 0 == window.Seek( -1, SEEK_END ) );
            ASSERT_TRUE( window.GetChar() == payload[ 17 ] );
            ASSERT_TRUE( 0 != window.Seek( 100, SEEK_SET ) );
            ASSERT_TRUE( 0 != window.Seek( -100, SEEK_CUR ) );
            ASSERT_TRUE( 0 != window.Setpos( 9 ) );
            ASSERT_TRUE( window.Tell() == 8 ); // failed seeks leave the position alone

            ASSERT_TRUE( window.Write( buffer, 1, 1 ) == 0 );

            window.Close();
            ASSERT_FALSE( window.Opened() );
            ASSERT_FALSE( window.IsValid() );
            ASSERT_TRUE( window.Read( buffer, 1, 1 ) == 0 );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: Windows sharing one parent keep their own position and nest" )
        try
        {
            CORE::IOAccessPtr parent( GUCEF_NEW CORE::CMFileAccess( payload, payloadSize ) );
            CORE::CSubIOAccess a( parent, 0, 10 );
            CORE::CSubIOAccess b( parent, 10, 10 );
            char x = 0, y = 0;
            ASSERT_TRUE( a.Read( &x, 1, 1 ) == 1 );
            ASSERT_TRUE( b.Read( &y, 1, 1 ) == 1 );
            ASSERT_TRUE( x == payload[ 0 ] );
            ASSERT_TRUE( y == payload[ 10 ] );
            ASSERT_TRUE( a.Read( &x, 1, 1 ) == 1 );
            ASSERT_TRUE( b.Read( &y, 1, 1 ) == 1 );
            ASSERT_TRUE( x == payload[ 1 ] );
            ASSERT_TRUE( y == payload[ 11 ] );

            // a window in a window
            CORE::IOAccessPtr outer( GUCEF_NEW CORE::CSubIOAccess( parent, 5, 20 ) );
            CORE::CSubIOAccess inner( outer, 5, 5 );
            char buffer[ 8 ];
            ASSERT_TRUE( inner.Read( buffer, 1, 5 ) == 5 );
            ASSERT_TRUE( 0 == memcmp( buffer, payload + 10, 5 ) );

            // a clone has its own position
            CORE::CICloneable* cloneBase = a.Clone();
            ASSERT_FALSE( GUCEF_NULL == cloneBase );
            CORE::CSubIOAccess* clone = static_cast< CORE::CSubIOAccess* >( cloneBase );
            ASSERT_TRUE( clone->Tell() == 0 );
            ASSERT_TRUE( clone->GetSize() == 10 );
            delete clone;
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: A window that does not fit its parent is refused" )
        try
        {
            CORE::IOAccessPtr parent( GUCEF_NEW CORE::CMFileAccess( payload, payloadSize ) );
            CORE::CSubIOAccess window;
            ASSERT_FALSE( window.Load( parent, payloadSize - 2, 10 ) );
            ASSERT_FALSE( window.Load( parent, payloadSize + 1, 0 ) );
            ASSERT_FALSE( window.Load( parent, 0xFFFFFFFFFFFFFFF0ULL, 0x20 ) ); // would overflow when added
            ASSERT_FALSE( window.Load( CORE::IOAccessPtr(), 0, 1 ) );
            ASSERT_FALSE( window.Opened() );
            ASSERT_TRUE( window.Load( parent, payloadSize - 2, 2 ) ); // exactly up to the end is fine
            ASSERT_TRUE( window.Load( parent, payloadSize, 0 ) );     // empty window at the end is fine
            ASSERT_TRUE( window.Eof() );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    CORE::DeleteFile( filePath );
}

/*-------------------------------------------------------------------------*/
