/*
 *  comcorepluginDBL: Generic GUCEF COMCORE plugin for providing access to the DBL network driver
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

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#include "comcorepluginDBL_driverapi.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCOREPLUGIN {
namespace DBL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

enum TDblDriverFuncPtrType
{
    DBLDRIVERFUNCPTR_MAL_OPEN                 ,
    DBLDRIVERFUNCPTR_MAL_CLOSE                ,
    DBLDRIVERFUNCPTR_MAL_NIC_ID_TO_STR        ,
    DBLDRIVERFUNCPTR_MAL_TRANSLATE_TO_PRODUCT ,
    DBLDRIVERFUNCPTR_MAL_IOCTL                ,

    DBLDRIVERFUNCPTR_COUNT
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef int32_t ( GUCEF_CALLSPEC_C_PREFIX *TDBLDRIVERPTR_mal_open ) ( uint32_t productIdPartB, uint32_t v2, void** deviceContext, int32_t optionFlags, int32_t productIdVar2 ) GUCEF_CALLSPEC_C_SUFFIX;
typedef void ( GUCEF_CALLSPEC_C_PREFIX *TDBLDRIVERPTR_mal_close ) ( void* deviceContext ) GUCEF_CALLSPEC_C_SUFFIX;
typedef int32_t ( GUCEF_CALLSPEC_C_PREFIX *TDBLDRIVERPTR_mal_nic_id_to_str ) ( const char** outStr, int64_t nicId, char flag ) GUCEF_CALLSPEC_C_SUFFIX;
typedef int32_t ( GUCEF_CALLSPEC_C_PREFIX *TDBLDRIVERPTR_mal_translate_to_product ) ( int64_t nicIndex, char* productIdPartA, int32_t* productIdPartB ) GUCEF_CALLSPEC_C_SUFFIX;
typedef int32_t ( GUCEF_CALLSPEC_C_PREFIX *TDBLDRIVERPTR_mal_ioctl ) ( void* deviceContext, int32_t infoTypeId, void* inAndOutData, char propertyId ) GUCEF_CALLSPEC_C_SUFFIX;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex g_datalock;
void* g_dblModule = GUCEF_NULL;
CORE::TDefaultFuncPtr g_funcPointers[ DBLDRIVERFUNCPTR_COUNT ];

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

int GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_init( char failureIsOk ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBL API wrapper: Attempting to init DBL wrapper API" );

    MT::CScopeMutex lock( g_datalock );

    if ( GUCEF_NULL == g_dblModule )
    {
        memset( g_funcPointers, 0, sizeof( g_funcPointers ) );
        
        // the DBL driver module should be available system wide if its installed
        g_dblModule = CORE::LoadModuleDynamicly( "DBL" );
        if ( GUCEF_NULL == g_dblModule )
        {
            if ( 0 != failureIsOk )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBL API wrapper: Failed to load DBL module access" );
            }
            else
            {
                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBL API wrapper: The DBL module is not available" );
            }
            return 0;
        }

        // We have now successfully loaded the module itself
        // We will now try to lookup the function pointers
        g_funcPointers[ DBLDRIVERFUNCPTR_MAL_OPEN ] = CORE::GetFunctionAddress( g_dblModule  ,
                                                                                "mal_open"   ,
                                                                                sizeof(uint32_t)+sizeof(uint32_t)+sizeof(void**)+sizeof(int32_t)+sizeof(int32_t)  ).funcPtr;
        g_funcPointers[ DBLDRIVERFUNCPTR_MAL_CLOSE ] = CORE::GetFunctionAddress( g_dblModule ,
                                                                                 "mal_close" ,
                                                                                 sizeof(void*)  ).funcPtr;
        g_funcPointers[ DBLDRIVERFUNCPTR_MAL_NIC_ID_TO_STR ] = CORE::GetFunctionAddress( g_dblModule         ,
                                                                                         "mal_nic_id_to_str" ,
                                                                                         sizeof(const char**)+sizeof(int64_t)+sizeof(char)  ).funcPtr;
        g_funcPointers[ DBLDRIVERFUNCPTR_MAL_TRANSLATE_TO_PRODUCT ] = CORE::GetFunctionAddress( g_dblModule                ,
                                                                                                "mal_translate_to_product" ,
                                                                                                sizeof(int64_t)+sizeof(char*)+sizeof(int32_t*)  ).funcPtr;
        g_funcPointers[ DBLDRIVERFUNCPTR_MAL_IOCTL ] = CORE::GetFunctionAddress( g_dblModule  ,
                                                                                 "mal_ioctl" ,
                                                                                 sizeof(void*)+sizeof(int32_t)+sizeof(void*)+sizeof(char)  ).funcPtr;

        // Verify that all function pointers are loaded correctly
        if ( ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_OPEN ] )                 &&
             ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_CLOSE ] )                &&
             ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_NIC_ID_TO_STR ] )        &&
             ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_TRANSLATE_TO_PRODUCT ] ) &&
             ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_IOCTL ] )                 )
        {
            // We now have the addresses for all the DBL driver manmagement functions we support
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBL API wrapper: The DBL module mal functions have been resolved and linked" );
        }
        else
        {
            if ( 0 != failureIsOk )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBL API wrapper: Failed to resolve and link DBL module mal functions" );
            }
            else
            {
                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBL API wrapper: Cannot resolve and link DBL module mal functions" );
            }
            return 0;
        }
    }
    else
    {
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBL API wrapper: The DBL module is already loaded" );
    }

    return 1;
}

/*---------------------------------------------------------------------------*/

int GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_shutdown( void ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    MT::CScopeMutex lock( g_datalock );

    if ( GUCEF_NULL != g_dblModule )
    {
        CORE::UnloadModuleDynamicly( g_dblModule );
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBL API wrapper: The DBL module has been unloaded" );
    }
    else
    {
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBL API wrapper: The DBL module is not loaded" );
    }

    return 1;
}

/*---------------------------------------------------------------------------*/

int32_t GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_open( uint32_t productIdPartB , 
                     uint32_t v2             , 
                     void** deviceContext    , 
                     int32_t optionFlags     , 
                     int32_t productIdVar2   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_OPEN ] )
    {
        return reinterpret_cast< TDBLDRIVERPTR_mal_open >( g_funcPointers[ DBLDRIVERFUNCPTR_MAL_OPEN ] )( productIdPartB ,
                                                                                                          v2             ,
                                                                                                          deviceContext  ,
                                                                                                          optionFlags    ,
                                                                                                          productIdVar2  );
    }
    return 1;
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_close( void* deviceContext ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_CLOSE ] )
    {
        reinterpret_cast< TDBLDRIVERPTR_mal_close >( g_funcPointers[ DBLDRIVERFUNCPTR_MAL_CLOSE ] )( deviceContext );
    }
}

/*---------------------------------------------------------------------------*/

int32_t GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_nic_id_to_str( const char** outStr , 
                              int64_t nicId       , 
                              char flag           ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_NIC_ID_TO_STR ] )
    {
        return reinterpret_cast< TDBLDRIVERPTR_mal_nic_id_to_str >( g_funcPointers[ DBLDRIVERFUNCPTR_MAL_NIC_ID_TO_STR ] )( outStr ,
                                                                                                                            nicId  ,
                                                                                                                            flag   );
    }
    return 1;
}

/*---------------------------------------------------------------------------*/

int32_t GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_translate_to_product( int64_t nicIndex        , 
                                     char* productIdPartA    , 
                                     int32_t* productIdPartB ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_TRANSLATE_TO_PRODUCT ] )
    {
        return reinterpret_cast< TDBLDRIVERPTR_mal_translate_to_product >( g_funcPointers[ DBLDRIVERFUNCPTR_MAL_TRANSLATE_TO_PRODUCT ] )( nicIndex       ,
                                                                                                                                          productIdPartA ,
                                                                                                                                          productIdPartB );
    }
    return 1;
}

/*---------------------------------------------------------------------------*/

int32_t GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_ioctl( void* deviceContext ,
                      int32_t infoTypeId  ,
                      void* inAndOutData  ,
                      char propertyId     ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != g_funcPointers[ DBLDRIVERFUNCPTR_MAL_IOCTL ] )
    {
        return reinterpret_cast< TDBLDRIVERPTR_mal_ioctl >( g_funcPointers[ DBLDRIVERFUNCPTR_MAL_IOCTL ] )( deviceContext ,
                                                                                                            infoTypeId    ,
                                                                                                            inAndOutData  ,
                                                                                                            propertyId    );
    }
    return 1;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DBL */
}; /* namespace COMCOREPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
