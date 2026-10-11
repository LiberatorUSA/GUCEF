/*
 *  codecspluginSTBRUMMEHASH: Codec plugin for RC32/MD5/SHA1/SHA3/SHA256/Keccak
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

#include "codecspluginSTBRUMMEHASH.h"     // function prototypes for this plugin

#include "crc32.h"
#include "sha1.h"
#include "sha3.h"
#include "sha256.h"
#include "keccak.h"
#include "md5.h"

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
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CODECSPLUGIN {
namespace STBRUMMEHASH {

using namespace GUCEF::CORE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define CODECSPLUGIN_STBRUMMEHASH_MAYOR_VERSION       0
#define CODECSPLUGIN_STBRUMMEHASH_MINOR_VERSION       1
#define CODECSPLUGIN_STBRUMMEHASH_PATCH_VERSION       0
#define CODECSPLUGIN_STBRUMMEHASH_RELEASE_VERSION     0

#define TEMP_READ_HASH_BUFFER_SIZE                (1024)

/*---------------------------------------------------------------------------*/

// The "-raw" variants output the binary digest instead of its hex string representation
static const char* supportedCryptoHashTypes[] = {
    "md5", "sha1", "sha3", "sha256", "keccak",
    "md5-raw", "sha1-raw", "sha3-raw", "sha256-raw", "keccak-raw"
};

static const char* supportedHashTypes[] = {
    "crc32"
};

static const Int32 cryptoHashCodecCount = (Int32) ( sizeof( supportedCryptoHashTypes ) / sizeof(char*) );
static const Int32 hashCodecCount = (Int32) ( sizeof( supportedHashTypes ) / sizeof(char*) );

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Converts a hex string as produced by the hash library into the binary bytes it represents
 */
static bool
HexStringToBytes( const std::string& hexStr, std::string& bytes )
{
    if ( 0 != ( hexStr.size() % 2 ) )
        return false;

    bytes.clear();
    bytes.reserve( hexStr.size() / 2 );
    for ( size_t i=0; i<hexStr.size(); i+=2 )
    {
        int byteValue = 0;
        for ( size_t n=i; n<i+2; ++n )
        {
            char c = hexStr[ n ];
            int nibble = 0;
            if ( c >= '0' && c <= '9' )
                nibble = c - '0';
            else
            if ( c >= 'a' && c <= 'f' )
                nibble = c - 'a' + 10;
            else
            if ( c >= 'A' && c <= 'F' )
                nibble = c - 'A' + 10;
            else
                return false;
            byteValue = ( byteValue << 4 ) | nibble;
        }
        bytes.push_back( (char) byteValue );
    }
    return true;
}

/*---------------------------------------------------------------------------*/

/**
 *  Hashes all of the input and writes the hash to the output either as a hex string or as the raw binary digest
 *  Note that the hash classes have a base class in the dependent library but its commented out hence the template
 */
template < class THashAlgo >
UInt32
GenerateHash( TIOAccess* input  ,
              TIOAccess* output ,
              bool rawDigest    )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_HASH_BUFFER_SIZE ];

    THashAlgo hashAlgo;

    /* hash the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_HASH_BUFFER_SIZE );
        if ( 0 == inputBytesRead )
            break;
        hashAlgo.add( tempInputBuffer, inputBytesRead );
    }

    std::string resultHash = hashAlgo.getHash();
    if ( rawDigest )
    {
        std::string hexHash = resultHash;
        if ( !HexStringToBytes( hexHash, resultHash ) )
            return 0;
    }

    if ( 1 == output->write( output, resultHash.c_str(), (UInt32) resultHash.size(), 1 ) )
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
    return "codecspluginSTBRUMMEHASH: Codec plugin for RC32/MD5/SHA1/SHA3/SHA256/Keccak";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "Plugin Copyright (C) Dinand Vanvelzen. 1998 - 2022.  Licensed under the Apache License, Version 2.0. STBRUMME's Hash Library dependency is licensed under ZLIB\0";
}

/*---------------------------------------------------------------------------*/

TVersion GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TVersion version;

    version.major = CODECSPLUGIN_STBRUMMEHASH_MAYOR_VERSION;
    version.minor = CODECSPLUGIN_STBRUMMEHASH_MINOR_VERSION;
    version.patch = CODECSPLUGIN_STBRUMMEHASH_PATCH_VERSION;
    version.release = CODECSPLUGIN_STBRUMMEHASH_RELEASE_VERSION;

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
    if ( 0 == strcmp( "CryptographicHashCodec", familyType ) )
    {
        if ( 0 == strcmp( "md5", codecType ) )
            return GenerateHash< MD5 >( input, output, false );
        if ( 0 == strcmp( "sha1", codecType ) )
            return GenerateHash< SHA1 >( input, output, false );
        if ( 0 == strcmp( "sha3", codecType ) )
            return GenerateHash< SHA3 >( input, output, false );
        if ( 0 == strcmp( "sha256", codecType ) )
            return GenerateHash< SHA256 >( input, output, false );
        if ( 0 == strcmp( "keccak", codecType ) )
            return GenerateHash< Keccak >( input, output, false );
        if ( 0 == strcmp( "md5-raw", codecType ) )
            return GenerateHash< MD5 >( input, output, true );
        if ( 0 == strcmp( "sha1-raw", codecType ) )
            return GenerateHash< SHA1 >( input, output, true );
        if ( 0 == strcmp( "sha3-raw", codecType ) )
            return GenerateHash< SHA3 >( input, output, true );
        if ( 0 == strcmp( "sha256-raw", codecType ) )
            return GenerateHash< SHA256 >( input, output, true );
        if ( 0 == strcmp( "keccak-raw", codecType ) )
            return GenerateHash< Keccak >( input, output, true );
    }
    if ( 0 == strcmp( "ChecksumCodec", familyType ) )
    {
        if ( 0 == strcmp( "crc32", codecType ) )
            return GenerateHash< CRC32 >( input, output, false );
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
    // A crypto hash is a one-way hash, no such thing as a decode (we hope)
    // Note that MD5 has known collisions
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

    if ( codecIndex < cryptoHashCodecCount )
    {
        *codecLink = (TCodecPluginLink*) malloc( sizeof( TCodecPluginLink ) );
        if ( GUCEF_NULL == *codecLink )
            return 0;

        (*codecLink)->codecData = GUCEF_NULL;
        (*codecLink)->codecFamily = "CryptographicHashCodec";
        (*codecLink)->codecType = supportedCryptoHashTypes[ codecIndex ];
        (*codecLink)->encode = CODECPLUGIN_Encode;
        (*codecLink)->decode = CODECPLUGIN_Decode;
        return 1;
    }
    codecIndex -= cryptoHashCodecCount;

    if ( codecIndex < hashCodecCount )
    {
        *codecLink = (TCodecPluginLink*) malloc( sizeof( TCodecPluginLink ) );
        if ( GUCEF_NULL == *codecLink )
            return 0;

        (*codecLink)->codecData = GUCEF_NULL;
        (*codecLink)->codecFamily = "ChecksumCodec";
        (*codecLink)->codecType = supportedHashTypes[ codecIndex ];
        (*codecLink)->encode = CODECPLUGIN_Encode;
        (*codecLink)->decode = CODECPLUGIN_Decode;
        return 1;
    }

    // There is no codec with the given index
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

    if ( (*codecIndex) < cryptoHashCodecCount+hashCodecCount )
    {
        return 1;
    }

    /* There are no more codecs */
    return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace STBRUMMEHASH */
}; /* namespace CODECSPLUGIN */
}; /* namespace GUCEF */

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
