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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex g_datalock;
void* g_dblModule = GUCEF_NULL;

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

    return 1;
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_close( void* deviceContext ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    return;
}

/*---------------------------------------------------------------------------*/

int32_t GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_nic_id_to_str( const char** outStr , 
                              int64_t nicId       , 
                              char flag           ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    return 1;
}

/*---------------------------------------------------------------------------*/

int32_t GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_translate_to_product( int64_t nicIndex        , 
                                     char* productIdPartA    , 
                                     int32_t* productIdPartB ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    return 1;
}

/*---------------------------------------------------------------------------*/

int32_t GUCEF_PLUGIN_CALLSPEC_PREFIX
dblwrapper_mal_ioctrl( void* deviceContext ,
                       int32_t infoTypeId  ,
                       void* inAndOutData  ,
                       char propertyId     ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

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
