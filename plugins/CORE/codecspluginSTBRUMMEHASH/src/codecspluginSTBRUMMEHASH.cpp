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

static const char* supportedCryptoHashTypes[] = {
    "md5", "sha1", "sha3", "sha256", "keccak"
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

//  Note that the crypto hash classes have a base class in the dependent library but its commented out
//  if that is implemented later the various functions below can be cleaned up into 1

/*---------------------------------------------------------------------------*/

UInt32
GenerateKeccakCryptoHash( TIOAccess* input  ,
                          TIOAccess* output )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_HASH_BUFFER_SIZE ];

    Keccak cryptohashAlgo;

    /* crypto hash the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_HASH_BUFFER_SIZE );
        cryptohashAlgo.add( tempInputBuffer, inputBytesRead );
    }

    std::string resultHash = cryptohashAlgo.getHash();

    if ( 1 == output->write( output, resultHash.c_str(), (UInt32) resultHash.size(), 1 ) )
        return 1;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32
GenerateSHA256CryptoHash( TIOAccess* input  ,
                          TIOAccess* output )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_HASH_BUFFER_SIZE ];

    SHA256 cryptohashAlgo;

    /* crypto hash the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_HASH_BUFFER_SIZE );
        cryptohashAlgo.add( tempInputBuffer, inputBytesRead );
    }

    std::string resultHash = cryptohashAlgo.getHash();

    if ( 1 == output->write( output, resultHash.c_str(), (UInt32) resultHash.size(), 1 ) )
        return 1;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32
GenerateSHA3CryptoHash( TIOAccess* input  ,
                        TIOAccess* output )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_HASH_BUFFER_SIZE ];

    SHA3 cryptohashAlgo;

    /* crypto hash the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_HASH_BUFFER_SIZE );
        cryptohashAlgo.add( tempInputBuffer, inputBytesRead );
    }

    std::string resultHash = cryptohashAlgo.getHash();

    if ( 1 == output->write( output, resultHash.c_str(), (UInt32) resultHash.size(), 1 ) )
        return 1;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32
GenerateSHA1CryptoHash( TIOAccess* input  ,
                        TIOAccess* output )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_HASH_BUFFER_SIZE ];

    SHA1 cryptohashAlgo;

    /* crypto hash the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_HASH_BUFFER_SIZE );
        cryptohashAlgo.add( tempInputBuffer, inputBytesRead );
    }

    std::string resultHash = cryptohashAlgo.getHash();

    if ( 1 == output->write( output, resultHash.c_str(), (UInt32) resultHash.size(), 1 ) )
        return 1;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32
GenerateMD5CryptoHash( TIOAccess* input  ,
                       TIOAccess* output )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_HASH_BUFFER_SIZE ];

    MD5 cryptohashAlgo;

    /* crypto hash the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_HASH_BUFFER_SIZE );
        cryptohashAlgo.add( tempInputBuffer, inputBytesRead );
    }

    std::string resultHash = cryptohashAlgo.getHash();

    if ( 1 == output->write( output, resultHash.c_str(), (UInt32) resultHash.size(), 1 ) )
        return 1;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32
GenerateCRC32Hash( TIOAccess* input  ,
                   TIOAccess* output )
{
    UInt32 inputBytesRead = 0;
    UInt8 tempInputBuffer[ TEMP_READ_HASH_BUFFER_SIZE ];

    CRC32 hashAlgo;

    /* crypto hash the input in chunks */
    while ( 0 == input->eof( input ) )
    {
        inputBytesRead = input->read( input, tempInputBuffer, 1, TEMP_READ_HASH_BUFFER_SIZE );
        hashAlgo.add( tempInputBuffer, inputBytesRead );
    }

    std::string resultHash = hashAlgo.getHash();

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
            return GenerateMD5CryptoHash( input, output );
        if ( 0 == strcmp( "sha1", codecType ) )
            return GenerateSHA1CryptoHash( input, output );
        if ( 0 == strcmp( "sha3", codecType ) )
            return GenerateSHA3CryptoHash( input, output );
        if ( 0 == strcmp( "sha256", codecType ) )
            return GenerateSHA256CryptoHash( input, output );
        if ( 0 == strcmp( "keccak", codecType ) )
            return GenerateKeccakCryptoHash( input, output );
    }
    if ( 0 == strcmp( "ChecksumCodec", familyType ) )
    {
        if ( 0 == strcmp( "crc32", codecType ) )
            return GenerateCRC32Hash( input, output );
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
