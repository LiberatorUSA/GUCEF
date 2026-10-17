/*
 *  codecspluginZLIB: Codec plugin for zlib/gzip/deflate/crc32/adler32
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

#include <stdlib.h>               /* for memory utils */
#include <string.h>               /* for memset */

#include "zlib.h"

#define GUCEF_DONT_TYPEDEF_BYTE
#include "codecspluginZLIB.h"     /* function prototypes for this plugin */


/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* make sure no structure packing is enabled, this allows us to use the structures directly */
#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif /* WIN32 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define CODECSPLUGIN_ZLIB_MAYOR_VERSION       0
#define CODECSPLUGIN_ZLIB_MINOR_VERSION       1
#define CODECSPLUGIN_ZLIB_PATCH_VERSION       0
#define CODECSPLUGIN_ZLIB_RELEASE_VERSION     0

#define TEMP_READ_CHECKSUM_BUFFER_SIZE                (1024)
#define TEMP_READ_BUFFER_SIZE                         (128 * 1024)
#define TEMP_INPROCESS_BUFFER_SIZE                    (128 * 1024)

/*---------------------------------------------------------------------------*/

static char* supportedCompressionTypes[] = {
    "zlib", "deflate", "inflate", "gzip"
};
static const Int32 compressionCodecCount = (Int32) ( sizeof( supportedCompressionTypes ) / sizeof(char*) );

static char* supportedChecksumTypes[] = {
    "crc-32", "adler-32"
};
static const Int32 checksumCodecCount = (Int32) ( sizeof( supportedChecksumTypes ) / sizeof(char*) );

static const int WINDOW_BITS_RAW_DEFLATE        =   -15;
static const int WINDOW_BITS_ZLIB_WRAPPER       =   15;
static const int WINDOW_BITS_GZIP_WRAPPER       =   31;
static const int WINDOW_BITS_GZIP_OR_ZLIB_AUTO  =   47;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Writes all of the given bytes to the output, also when the output only accepts part of the data per write call
 *  Returns 1 on success and 0 if the output stops accepting data
 */
static int
WriteAllToOutput( TIOAccess* output    ,
                  const UInt8* data    ,
                  UInt32 dataSize      )
{
    while ( dataSize > 0 )
    {
        UInt32 bytesWritten = output->write( output, data, 1, dataSize );
        if ( 0 == bytesWritten || bytesWritten > dataSize )
            return 0;
        data += bytesWritten;
        dataSize -= bytesWritten;
    }
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32
ZLibCompress( TIOAccess* input  ,
              TIOAccess* output ,
              int windowBits    )
{
    int zlibResult = Z_OK;
    int flushMode = Z_NO_FLUSH;
    UInt32 success = 1;
    UInt8* tempInputBuffer = (UInt8*) malloc( TEMP_READ_BUFFER_SIZE );
    UInt8* tempOutputBuffer = (UInt8*) malloc( TEMP_INPROCESS_BUFFER_SIZE );
    z_stream strm;

    if ( GUCEF_NULL == tempInputBuffer || GUCEF_NULL == tempOutputBuffer )
    {
        free( tempInputBuffer );
        free( tempOutputBuffer );
        return 0;
    }

    memset( &strm, 0, sizeof( strm ) );
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    zlibResult = deflateInit2( &strm, Z_BEST_COMPRESSION, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY );
    if ( Z_OK != zlibResult )
    {
        free( tempInputBuffer );
        free( tempOutputBuffer );
        return 0;
    }

    /* read blocks of data from the input, encode them and write them to the output */
    /* once the input is exhausted we finish the stream which also writes any trailer info */
    do
    {
        strm.avail_in = input->read( input, tempInputBuffer, 1, TEMP_READ_BUFFER_SIZE );
        strm.next_in = tempInputBuffer;
        flushMode = ( 0 == strm.avail_in ) ? Z_FINISH : Z_NO_FLUSH;

        /* keep deflating until deflate no longer fills up the entire output buffer */
        do
        {
            strm.next_out = tempOutputBuffer;
            strm.avail_out = TEMP_INPROCESS_BUFFER_SIZE;

            zlibResult = deflate( &strm, flushMode );
            if ( Z_STREAM_ERROR == zlibResult )
            {
                success = 0;
                break;
            }

            if ( 0 == WriteAllToOutput( output, tempOutputBuffer, TEMP_INPROCESS_BUFFER_SIZE - strm.avail_out ) )
            {
                success = 0;
                break;
            }
        }
        while ( 0 == strm.avail_out );
    }
    while ( 1 == success && Z_FINISH != flushMode );

    if ( 1 == success && Z_STREAM_END != zlibResult )
        success = 0;

    deflateEnd( &strm );
    free( tempInputBuffer );
    free( tempOutputBuffer );

    return success;
}

/*---------------------------------------------------------------------------*/

UInt32
ZLibDecompress( TIOAccess* input  ,
                TIOAccess* output ,
                int windowBits    )
{
    int zlibResult = Z_OK;
    UInt32 success = 1;
    UInt8* tempInputBuffer = (UInt8*) malloc( TEMP_READ_BUFFER_SIZE );
    UInt8* tempOutputBuffer = (UInt8*) malloc( TEMP_INPROCESS_BUFFER_SIZE );
    z_stream strm;

    if ( GUCEF_NULL == tempInputBuffer || GUCEF_NULL == tempOutputBuffer )
    {
        free( tempInputBuffer );
        free( tempOutputBuffer );
        return 0;
    }

    memset( &strm, 0, sizeof( strm ) );
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;

    zlibResult = inflateInit2( &strm, windowBits );
    if ( Z_OK != zlibResult )
    {
        free( tempInputBuffer );
        free( tempOutputBuffer );
        return 0;
    }

    /* read blocks of data from the input, decode them and write them to the output until the end of the stream */
    do
    {
        strm.avail_in = input->read( input, tempInputBuffer, 1, TEMP_READ_BUFFER_SIZE );
        strm.next_in = tempInputBuffer;
        if ( 0 == strm.avail_in )
        {
            /* the input ended before the end of the compressed stream */
            break;
        }

        /* keep inflating until inflate no longer fills up the entire output buffer */
        do
        {
            strm.next_out = tempOutputBuffer;
            strm.avail_out = TEMP_INPROCESS_BUFFER_SIZE;

            zlibResult = inflate( &strm, Z_NO_FLUSH );
            if ( Z_STREAM_ERROR == zlibResult || Z_NEED_DICT == zlibResult ||
                 Z_DATA_ERROR == zlibResult || Z_MEM_ERROR == zlibResult )
            {
                success = 0;
                break;
            }

            if ( 0 == WriteAllToOutput( output, tempOutputBuffer, TEMP_INPROCESS_BUFFER_SIZE - strm.avail_out ) )
            {
                success = 0;
                break;
            }
        }
        while ( 0 == strm.avail_out );
    }
    while ( 1 == success && Z_STREAM_END != zlibResult );

    if ( 1 == success && Z_STREAM_END != zlibResult )
        success = 0;

    inflateEnd( &strm );
    free( tempInputBuffer );
    free( tempOutputBuffer );

    return success;
}

/*---------------------------------------------------------------------------*/

UInt32
GenerateCRC32Checksum( TIOAccess* input  ,
                       TIOAccess* output )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_CHECKSUM_BUFFER_SIZE ];

    /* init the checksum */
    UInt32 finalChecksum = 0;
    uLong checksum = crc32( 0L, Z_NULL, 0 );

    /* checksum the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_CHECKSUM_BUFFER_SIZE );
        if ( 0 == inputBytesRead )
            break;
        checksum = crc32( checksum, tempInputBuffer, inputBytesRead );
    }

    /* produce the result checksum */
    finalChecksum = (UInt32) checksum;
    if ( 1 == output->write( output, &finalChecksum, sizeof(finalChecksum), 1 ) )
        return 1;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32
GenerateAdler32Checksum( TIOAccess* input  ,
                         TIOAccess* output )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_CHECKSUM_BUFFER_SIZE ];

    /* init the checksum */
    UInt32 finalChecksum = 0;
    uLong checksum = adler32( 0L, Z_NULL, 0 );

    /* checksum the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_CHECKSUM_BUFFER_SIZE );
        if ( 0 == inputBytesRead )
            break;
        checksum = adler32( checksum, tempInputBuffer, inputBytesRead );
    }

    /* produce the result checksum */
    finalChecksum = (UInt32) checksum;
    if ( 1 == output->write( output, &finalChecksum, sizeof(finalChecksum), 1 ) )
        return 1;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Init( void** plugdata   ,
                  const int argc    ,
                  const char** argv ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{

    *plugdata = GUCEF_NULL;
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{

    return 1;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Description( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "compressioncodecZLIB: zlib/deflate/gzip (de)compression and crc32/adler32 checksum codecs support using zlib\0";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "Plugin Copyright (C) Dinand Vanvelzen. 1998 - 2020.  Licensed under the Apache License, Version 2.0\0";
}

/*---------------------------------------------------------------------------*/

TVersion GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TVersion version;

    version.major = CODECSPLUGIN_ZLIB_MAYOR_VERSION;
    version.minor = CODECSPLUGIN_ZLIB_MINOR_VERSION;
    version.patch = CODECSPLUGIN_ZLIB_PATCH_VERSION;
    version.release = CODECSPLUGIN_ZLIB_RELEASE_VERSION;

    return version;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Encode( void* plugdata         ,
                    void* codecData        ,
                    const char* familyType ,
                    const char* codecType  ,
                    TIOAccess* input       ,
                    TIOAccess* output      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    if ( 0 == strcmp( "CompressionCodec", familyType ) )
    {
        if ( 0 == strcmp( "deflate", codecType ) )
            return ZLibCompress( input, output, WINDOW_BITS_RAW_DEFLATE );
        if ( 0 == strcmp( "zlib", codecType ) )
            return ZLibCompress( input, output, WINDOW_BITS_ZLIB_WRAPPER );
        if ( 0 == strcmp( "gzip", codecType ) )
            return ZLibCompress( input, output, WINDOW_BITS_GZIP_WRAPPER );
    }
    if ( 0 == strcmp( "ChecksumCodec", familyType ) )
    {
        if ( 0 == strcmp( "crc-32", codecType ) )
            return GenerateCRC32Checksum( input, output );
        if ( 0 == strcmp( "adler-32", codecType ) )
            return GenerateAdler32Checksum( input, output );
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Decode( void* pluginData       ,
                    void* codecData        ,
                    const char* familyType ,
                    const char* codecType  ,
                    TIOAccess* input       ,
                    TIOAccess* output      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    if ( 0 == strcmp( "CompressionCodec", familyType ) )
    {
        if ( 0 == strcmp( "zlib", codecType ) || 0 == strcmp( "gzip", codecType ) )
            return ZLibDecompress( input, output, WINDOW_BITS_GZIP_OR_ZLIB_AUTO );

        /* the algo name is "deflate" but what we are doing here is "inflate", as such we will accept both terms for the same for now */
        if ( 0 == strcmp( "deflate", codecType ) || 0 == strcmp( "inflate", codecType ) )
            return ZLibDecompress( input, output, WINDOW_BITS_RAW_DEFLATE );
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_GetCodecSetBegin( void* plugdata  ,
                              void** iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    *iterator = malloc( sizeof( Int32 ) );
    if ( GUCEF_NULL != *iterator )
        *( (Int32*) *iterator ) = 0;
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_GetCodecLink( void* plugdata               ,
                          void* iterator               ,
                          TCodecPluginLink** codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    Int32 codecIndex = *( (Int32*) iterator );

    if ( codecIndex < compressionCodecCount )
    {
        *codecLink = (TCodecPluginLink*) malloc( sizeof( TCodecPluginLink ) );
        if ( GUCEF_NULL == *codecLink )
            return 0;

        (*codecLink)->codecData = GUCEF_NULL;
        (*codecLink)->codecFamily = "CompressionCodec";
        (*codecLink)->codecType = supportedCompressionTypes[ codecIndex ];
        (*codecLink)->encode = CODECPLUGIN_Encode;
        (*codecLink)->decode = CODECPLUGIN_Decode;
        return 1;
    }
    codecIndex -= compressionCodecCount;

    if ( codecIndex < checksumCodecCount )
    {
        *codecLink = (TCodecPluginLink*) malloc( sizeof( TCodecPluginLink ) );
        if ( GUCEF_NULL == *codecLink )
            return 0;

        (*codecLink)->codecData = GUCEF_NULL;
        (*codecLink)->codecFamily = "ChecksumCodec";
        (*codecLink)->codecType = supportedChecksumTypes[ codecIndex ];
        (*codecLink)->encode = CODECPLUGIN_Encode;
        (*codecLink)->decode = CODECPLUGIN_Decode;
        return 1;
    }

    /* There is no codec with the given index */
    *codecLink = GUCEF_NULL;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_FreeCodecLink( void* plugdata              ,
                           TCodecPluginLink* codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    free( codecLink );
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_FreeCodecIterator( void* plugdata ,
                               void* iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    Int32* codecIndex = (Int32*) iterator;
    free( codecIndex );
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_GetCodecSetNextItem( void* plugdata ,
                                 void* iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    Int32* codecIndex = ( (Int32*) iterator );
    ++(*codecIndex);

    if ( (*codecIndex) < compressionCodecCount+checksumCodecCount )
    {
        return 1;
    }

    /* There are no more codecs */
    return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* restore structure packing */
#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(pop)
#else
#pragma pack()
#endif /* WIN32 */

/*-------------------------------------------------------------------------*/
