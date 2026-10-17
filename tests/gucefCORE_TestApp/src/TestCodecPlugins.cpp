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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CORECODECTYPES_H
#include "gucefCORE_CoreCodecTypes.h"
#define GUCEF_CORE_CORECODECTYPES_H
#endif /* GUCEF_CORE_CORECODECTYPES_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestCodecPlugins.h"

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------*/

/**
 *  Buffer access which accepts at most a given number of bytes per write call,
 *  used to verify that codecs handle partial writes correctly
 */
class CPartialWriteBufferAccess : public CORE::CDynamicBufferAccess
{
    public:

    CPartialWriteBufferAccess( CORE::CDynamicBuffer& buffer, UInt32 maxBytesPerWrite )
        : CORE::CDynamicBufferAccess( buffer )
        , m_maxBytesPerWrite( maxBytesPerWrite )
        , m_partialWriteCount( 0 )
    {}

    virtual UInt32 Write( const void* srcdata ,
                          UInt32 esize        ,
                          UInt32 elements     ) GUCEF_VIRTUAL_OVERRIDE
    {
        if ( 1 == esize && elements > m_maxBytesPerWrite )
        {
            ++m_partialWriteCount;
            elements = m_maxBytesPerWrite;
        }
        return CORE::CDynamicBufferAccess::Write( srcdata, esize, elements );
    }

    UInt32 GetPartialWriteCount( void ) const { return m_partialWriteCount; }

    private:

    UInt32 m_maxBytesPerWrite;
    UInt32 m_partialWriteCount;
};

/*-------------------------------------------------------------------------*/

static bool
EnsureCodecPluginIsLoaded( const CORE::CString& pluginName )
{GUCEF_TRACE;

    // Normally this would be managed via config however to keep the test self contained we do it programmatically
    #ifdef GUCEF_DEBUG_MODE
    CORE::CString firstPluginPath = "$MODULEDIR$/" + pluginName + "_d";
    CORE::CString secondPluginPath = "$MODULEDIR$/" + pluginName;
    #else
    CORE::CString firstPluginPath = "$MODULEDIR$/" + pluginName;
    CORE::CString secondPluginPath = "$MODULEDIR$/" + pluginName + "_d";
    #endif

    CORE::CPluginControl& pluginControl = CORE::CCoreGlobal::Instance()->GetPluginControl();
    if ( pluginControl.AddPluginFromDir( firstPluginPath, "test", true ) )
        return true;
    return pluginControl.AddPluginFromDir( secondPluginPath, "test", true );
}

/*-------------------------------------------------------------------------*/

static bool
GetCodec( const CORE::CString& familyName                 ,
          const CORE::CString& codecName                  ,
          CORE::CCodecRegistry::TICodecPtr& codec         )
{GUCEF_TRACE;

    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();
    CORE::CCodecRegistry::TCodecFamilyRegistryPtr codecFamilyRegistry;
    if ( codecRegistry.TryLookup( familyName, codecFamilyRegistry, false ) && codecFamilyRegistry )
        return codecFamilyRegistry->TryLookup( codecName, codec, false ) && codec;
    return false;
}

/*-------------------------------------------------------------------------*/

static void
CreateCompressibleTestData( CORE::CDynamicBuffer& data, UInt32 size )
{GUCEF_TRACE;

    // Mix of repetitive and pseudo random content so the data compresses but not trivially
    data.SetDataSize( size );
    UInt8* bytes = data.AsTypePtr< UInt8 >();
    UInt32 state = 12345;
    for ( UInt32 i=0; i<size; ++i )
    {
        state = state * 1103515245 + 12345;
        bytes[ i ] = ( 0 == ( i / 4096 ) % 2 ) ? (UInt8) ( 'a' + ( i % 26 ) ) : (UInt8) ( state >> 16 );
    }
}

/*-------------------------------------------------------------------------*/

static bool
BuffersAreEqual( const CORE::CDynamicBuffer& a, const CORE::CDynamicBuffer& b )
{GUCEF_TRACE;

    if ( a.GetDataSize() != b.GetDataSize() )
        return false;
    if ( 0 == a.GetDataSize() )
        return true;
    return 0 == memcmp( a.GetConstBufferPtr(), b.GetConstBufferPtr(), a.GetDataSize() );
}

/*-------------------------------------------------------------------------*/

static void
PerformCompressionRoundTripTest( CORE::CCodecRegistry::TICodecPtr& codec ,
                                 const CORE::CDynamicBuffer& original    ,
                                 UInt32 maxBytesPerWrite                 )
{GUCEF_TRACE;

    CORE::CDynamicBuffer inputBuffer( original );
    CORE::CDynamicBufferAccess input( inputBuffer );

    CORE::CDynamicBuffer compressed;
    CPartialWriteBufferAccess compressedAccess( compressed, maxBytesPerWrite );
    ASSERT_TRUE( codec->Encode( input, compressedAccess ) );
    ASSERT_TRUE( compressed.GetDataSize() > 0 );
    ASSERT_TRUE( compressed.GetDataSize() < original.GetDataSize() );

    compressedAccess.Setpos( 0 );
    CORE::CDynamicBuffer decompressed;
    CPartialWriteBufferAccess decompressedAccess( decompressed, maxBytesPerWrite );
    ASSERT_TRUE( codec->Decode( compressedAccess, decompressedAccess ) );
    ASSERT_TRUE( BuffersAreEqual( original, decompressed ) );

    if ( maxBytesPerWrite < original.GetDataSize() )
    {
        // Make sure the test actually exercised partial writes
        ASSERT_TRUE( decompressedAccess.GetPartialWriteCount() > 0 );
    }
}

/*-------------------------------------------------------------------------*/

static void
PerformZLibCodecTests( void )
{GUCEF_TRACE;

    if ( !EnsureCodecPluginIsLoaded( "codecspluginZLIB" ) )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "ZLib codec plugin TESTS skipped: codecspluginZLIB is not available" );
        return;
    }

    static const char* compressionCodecs[] = { "deflate", "zlib", "gzip" };

    // Large enough to need multiple internal buffer cycles of the codec
    CORE::CDynamicBuffer original;
    CreateCompressibleTestData( original, 1024 * 1024 + 123 );

    GUCEF_TESTFW_TESTCASE( "Test: ZLib compression codecs round trip" )
        try
        {
            for ( UInt32 i=0; i<3; ++i )
            {
                CORE::CCodecRegistry::TICodecPtr codec;
                ASSERT_TRUE( GetCodec( CORE::CoreCodecTypes::CompressionCodec, compressionCodecs[ i ], codec ) );
                if ( !codec.IsNULL() )
                    PerformCompressionRoundTripTest( codec, original, GUCEF_UINT32MAX );
            }
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: after a partial write the codec continued with the wrong output buffer position
    // which corrupted the output, and the end of stream flush wrote more bytes than were produced
    GUCEF_TESTFW_TESTCASE( "Test: ZLib compression codecs handle partial writes" )
        try
        {
            for ( UInt32 i=0; i<3; ++i )
            {
                CORE::CCodecRegistry::TICodecPtr codec;
                ASSERT_TRUE( GetCodec( CORE::CoreCodecTypes::CompressionCodec, compressionCodecs[ i ], codec ) );
                if ( !codec.IsNULL() )
                    PerformCompressionRoundTripTest( codec, original, 1000 );
            }
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: ZLib codec handles empty input" )
        try
        {
            CORE::CCodecRegistry::TICodecPtr codec;
            ASSERT_TRUE( GetCodec( CORE::CoreCodecTypes::CompressionCodec, "gzip", codec ) );
            if ( !codec.IsNULL() )
            {
                CORE::CDynamicBuffer emptyBuffer;
                CORE::CDynamicBufferAccess input( emptyBuffer );
                CORE::CDynamicBuffer compressed;
                CORE::CDynamicBufferAccess compressedAccess( compressed );
                ASSERT_TRUE( codec->Encode( input, compressedAccess ) );
                ASSERT_TRUE( compressed.GetDataSize() > 0 );

                compressedAccess.Setpos( 0 );
                CORE::CDynamicBuffer decompressed;
                CORE::CDynamicBufferAccess decompressedAccess( decompressed );
                ASSERT_TRUE( codec->Decode( compressedAccess, decompressedAccess ) );
                ASSERT_TRUE( 0 == decompressed.GetDataSize() );
            }
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: ZLib codec rejects truncated and corrupt input" )
        try
        {
            CORE::CCodecRegistry::TICodecPtr codec;
            ASSERT_TRUE( GetCodec( CORE::CoreCodecTypes::CompressionCodec, "gzip", codec ) );
            if ( !codec.IsNULL() )
            {
                CORE::CDynamicBuffer inputBuffer( original );
                CORE::CDynamicBufferAccess input( inputBuffer );
                CORE::CDynamicBuffer compressed;
                CORE::CDynamicBufferAccess compressedAccess( compressed );
                ASSERT_TRUE( codec->Encode( input, compressedAccess ) );

                // truncated: the stream ends before the end of the compressed data
                CORE::CDynamicBuffer truncated;
                truncated.CopyFrom( compressed.GetDataSize() / 2, compressed.GetConstBufferPtr() );
                CORE::CDynamicBufferAccess truncatedAccess( truncated );
                CORE::CDynamicBuffer output;
                CORE::CDynamicBufferAccess outputAccess( output );
                ASSERT_FALSE( codec->Decode( truncatedAccess, outputAccess ) );

                // corrupt: not compressed data at all
                CORE::CDynamicBuffer corrupt;
                corrupt.CopyFrom( 64, "this is definitely not a valid gzip or zlib stream, not at all..." );
                CORE::CDynamicBufferAccess corruptAccess( corrupt );
                CORE::CDynamicBuffer output2;
                CORE::CDynamicBufferAccess outputAccess2( output2 );
                ASSERT_FALSE( codec->Decode( corruptAccess, outputAccess2 ) );
            }
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // Regression: the checksum codecs reported failure because the result of writing
    // the 4 byte checksum was compared against the wrong element count
    GUCEF_TESTFW_TESTCASE( "Test: ZLib checksum codecs known answers" )
        try
        {
            static const char* checkInput = "123456789";
            static const char* checksumCodecs[] = { "crc-32", "adler-32" };
            static const UInt32 expectedChecksums[] = { 0xCBF43926, 0x091E01DE };

            for ( UInt32 i=0; i<2; ++i )
            {
                CORE::CCodecRegistry::TICodecPtr codec;
                ASSERT_TRUE( GetCodec( CORE::CoreCodecTypes::ChecksumCodec, checksumCodecs[ i ], codec ) );
                if ( !codec.IsNULL() )
                {
                    CORE::CDynamicBuffer inputBuffer;
                    inputBuffer.CopyFrom( (UInt32) strlen( checkInput ), checkInput );
                    CORE::CDynamicBufferAccess input( inputBuffer );
                    CORE::CDynamicBuffer checksum;
                    CORE::CDynamicBufferAccess checksumAccess( checksum );
                    ASSERT_TRUE( codec->Encode( input, checksumAccess ) );
                    ASSERT_TRUE( 4 == checksum.GetDataSize() );
                    if ( 4 == checksum.GetDataSize() )
                        ASSERT_TRUE( expectedChecksums[ i ] == checksum.AsConstType< UInt32 >() );
                }
            }
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/

static void
PerformHashCodecTests( void )
{GUCEF_TRACE;

    if ( !EnsureCodecPluginIsLoaded( "codecspluginSTBRUMMEHASH" ) )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Hash codec plugin TESTS skipped: codecspluginSTBRUMMEHASH is not available" );
        return;
    }

    struct SHashVector { const char* codecName; const char* expectedHex; };
    // Known answers for the input "abc"
    static const SHashVector vectors[] = {
        { "md5"    , "900150983cd24fb0d6963f7d28e17f72" },
        { "sha1"   , "a9993e364706816aba3e25717850c26c9cd0d89d" },
        { "sha256" , "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" },
        { "sha3"   , "3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431532" },
        { "keccak" , "4e03657aea45a94fc7d47ba826c8d667c0d1e6e33a64a036ec44f58fa12d6c45" }
    };
    static const UInt32 vectorCount = (UInt32) ( sizeof( vectors ) / sizeof( vectors[ 0 ] ) );

    GUCEF_TESTFW_TESTCASE( "Test: Hash codecs known answers as hex and raw digests" )
        try
        {
            for ( UInt32 i=0; i<vectorCount; ++i )
            {
                CORE::CString hexName = vectors[ i ].codecName;
                CORE::CString rawName = hexName + "-raw";
                CORE::CString expectedHex = vectors[ i ].expectedHex;

                CORE::CCodecRegistry::TICodecPtr hexCodec;
                CORE::CCodecRegistry::TICodecPtr rawCodec;
                ASSERT_TRUE( GetCodec( CORE::CoreCodecTypes::CryptographicHashCodec, hexName, hexCodec ) );
                ASSERT_TRUE( GetCodec( CORE::CoreCodecTypes::CryptographicHashCodec, rawName, rawCodec ) );
                if ( hexCodec.IsNULL() || rawCodec.IsNULL() )
                    continue;

                CORE::CDynamicBuffer inputBuffer;
                inputBuffer.CopyFrom( 3, "abc" );
                CORE::CDynamicBufferAccess input( inputBuffer );
                CORE::CDynamicBuffer hexResult;
                CORE::CDynamicBufferAccess hexResultAccess( hexResult );
                ASSERT_TRUE( hexCodec->Encode( input, hexResultAccess ) );
                ASSERT_TRUE( expectedHex.Length() == hexResult.GetDataSize() );
                ASSERT_TRUE( CORE::CString( hexResult.AsConstTypePtr< char >(), hexResult.GetDataSize() ) == expectedHex );

                input.Setpos( 0 );
                CORE::CDynamicBuffer rawResult;
                CORE::CDynamicBufferAccess rawResultAccess( rawResult );
                ASSERT_TRUE( rawCodec->Encode( input, rawResultAccess ) );
                ASSERT_TRUE( expectedHex.Length() / 2 == rawResult.GetDataSize() );

                // compare the raw bytes against the hex representation
                static const char* hexDigits = "0123456789abcdef";
                CORE::CString rawAsHex;
                const UInt8* rawBytes = rawResult.AsConstTypePtr< UInt8 >();
                for ( UInt32 n=0; n<rawResult.GetDataSize(); ++n )
                {
                    rawAsHex += CORE::CString( hexDigits + ( rawBytes[ n ] >> 4 ), 1 );
                    rawAsHex += CORE::CString( hexDigits + ( rawBytes[ n ] & 0xF ), 1 );
                }
                ASSERT_TRUE( rawAsHex == expectedHex );
            }
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/

void
PerformCodecPluginTests( void )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "COMMENCING Codec Plugin TESTS" );

    GUCEF_TESTFW_SUITE_SCOPE( "CodecPlugins" );

    PerformZLibCodecTests();
    PerformHashCodecTests();

    CORE::CLogStreamScope::FlushLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ALL Codec Plugin TESTS COMPLETED" );
}

/*-------------------------------------------------------------------------*/
