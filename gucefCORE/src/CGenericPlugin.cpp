/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#include <string.h>

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_GENERICPLUGINAPI_H
#include "gucefCORE_GenericPluginAPI.h"
#define GUCEF_CORE_GENERICPLUGINAPI_H
#endif /* GUCEF_CORE_GENERICPLUGINAPI_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ?  */

#include "CGenericPlugin.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

enum TGenericPluginFuncPtrType
{
    GPLUGINFUNCPTR_LOAD        ,
    GPLUGINFUNCPTR_UNLOAD      ,
    GPLUGINFUNCPTR_VERSION     ,
    GPLUGINFUNCPTR_COPYRIGHT   ,
    GPLUGINFUNCPTR_DESCRIPTION ,

    GPLUGINFUNCPTR_COUNT
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CGenericPlugin::CGenericPlugin( void )
    : CIPlugin()             ,
      m_moduleHandle( NULL ) ,
      m_modulePath()         ,
      m_params()             ,
      m_metaData()
{GUCEF_TRACE;

    memset( m_funcPointers, 0, GPLUGINFUNCPTR_COUNT );
}

/*-------------------------------------------------------------------------*/

CGenericPlugin::~CGenericPlugin()
{GUCEF_TRACE;

    Unlink();
}

/*-------------------------------------------------------------------------*/

void
CGenericPlugin::SetPluginParams( const CValueList& pluginParams )
{GUCEF_TRACE;

    m_params = pluginParams; 
}

/*-------------------------------------------------------------------------*/

const CValueList&
CGenericPlugin::GetPluginParams( void ) const
{GUCEF_TRACE;

    return m_params;
}

/*-------------------------------------------------------------------------*/

TPluginMetaDataPtr
CGenericPlugin::GetMetaData( void )
{GUCEF_TRACE;

    return m_metaData;
}

/*-------------------------------------------------------------------------*/

void*
CGenericPlugin::GetModulePointer( void )
{GUCEF_TRACE;

    return m_moduleHandle;
}

/*-------------------------------------------------------------------------*/

bool
CGenericPlugin::Link( void* modulePtr                   ,
                      TPluginMetaDataPtr pluginMetaData )
{GUCEF_TRACE;

    if ( IsLoaded() ) return false;
    
    m_moduleHandle = modulePtr;
    if ( NULL != m_moduleHandle )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GenericPlugin: Linking API using module pointer: " + PointerToString( modulePtr ) );
            
        // We have now successfully loaded the module itself
        // We will now try to lookup the function pointers
        m_funcPointers[ GPLUGINFUNCPTR_LOAD ] = GetFunctionAddress( m_moduleHandle                ,
                                                                    "GUCEFPlugin_Load"            ,
                                                                    sizeof(UInt32)+sizeof(char**) ).funcPtr;
        m_funcPointers[ GPLUGINFUNCPTR_UNLOAD ] = GetFunctionAddress( m_moduleHandle       ,
                                                                      "GUCEFPlugin_Unload" ,
                                                                      0                    ).funcPtr;
        m_funcPointers[ GPLUGINFUNCPTR_VERSION ] = GetFunctionAddress( m_moduleHandle           ,
                                                                       "GUCEFPlugin_GetVersion" ,
                                                                       sizeof( TVersion* )      ).funcPtr;
        m_funcPointers[ GPLUGINFUNCPTR_DESCRIPTION ] = GetFunctionAddress( m_moduleHandle               ,
                                                                           "GUCEFPlugin_GetDescription" ,
                                                                           0                            ).funcPtr;

        m_funcPointers[ GPLUGINFUNCPTR_COPYRIGHT ] = GetFunctionAddress( m_moduleHandle             ,
                                                                         "GUCEFPlugin_GetCopyright" ,
                                                                         0                          ).funcPtr;

        // Verify that all function pointers are loaded correctly
        if ( ( NULL != m_funcPointers[ GPLUGINFUNCPTR_LOAD ] )        &&
             ( NULL != m_funcPointers[ GPLUGINFUNCPTR_UNLOAD ] )      &&
             ( NULL != m_funcPointers[ GPLUGINFUNCPTR_VERSION ] )     &&
             ( NULL != m_funcPointers[ GPLUGINFUNCPTR_DESCRIPTION ] ) &&
             ( NULL != m_funcPointers[ GPLUGINFUNCPTR_COPYRIGHT ] )    )
        {
            // Make the module parameter list
            const char** argv = NULL;
            UInt32 argc = m_params.GetCount();
            if ( argc > 0 )
            {
                argv = new const char*[ argc ];
                for ( UInt32 i=0; i<argc; ++i )
                {
                    CString keyValuePair = m_params.GetPair( i );
                    argv[ i ] = new char[ keyValuePair.Length()+1 ];
                    memcpy( (void*)argv[ i ], keyValuePair.C_String(), keyValuePair.Length()+1 );
                } 
            }
                
            // Call the module's Load()
            Int32 loadStatus = reinterpret_cast< TGUCEFGENERICPLUGFPTR_Load >( m_funcPointers[ GPLUGINFUNCPTR_LOAD ] )( argc, argv );
                
            // Cleanup the module parameter list
            if ( argc > 0 )
            {
                for ( UInt32 i=0; i<argc; ++i )
                {
                    delete[] argv[ i ];
                }
                delete[] argv;
                argv = NULL;
            }
                
            // Check whether the load was successfull
            if ( loadStatus > 0 )
            {
                // We have loaded & linked our plugin module
                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GenericPlugin: Successfully loaded module and invoked Load() which returned status " + 
                        Int32ToString( loadStatus  ) + " using module: " + PointerToString( modulePtr ) );

                // Copy the given metadata and update it with info from the actual module
                m_metaData = new CPluginMetaData( *pluginMetaData );                 
                m_metaData->SetDescription( GetDescription() );
                m_metaData->SetCopyright( GetCopyright() );
                m_metaData->SetVersion( GetVersion() );

                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "GenericPlugin: Failed load module because Load returned an error state (" + 
                                                        Int32ToString( loadStatus ) + ") with module: " + PointerToString( modulePtr ) );

                // Call the module's Unload()
                reinterpret_cast< TGUCEFGENERICPLUGFPTR_Unload >( m_funcPointers[ GPLUGINFUNCPTR_UNLOAD ] )();
            }
                
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "CGenericPlugin: Failed to locate the required exports in module: " + PointerToString( modulePtr ) );
        }

        // We failed to link the functions :(
        memset( m_funcPointers, 0, GPLUGINFUNCPTR_COUNT );
        m_moduleHandle = NULL;
        return false;
    }

    // Unloading->Loading must be done explicitly
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CGenericPlugin::Unlink( void )
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        // Call the module's Unload()
        reinterpret_cast< TGUCEFGENERICPLUGFPTR_Unload >( m_funcPointers[ GPLUGINFUNCPTR_UNLOAD ] )();

        // Cleanup recources
        memset( m_funcPointers, 0, GPLUGINFUNCPTR_COUNT );
        m_moduleHandle = NULL;
        m_metaData = NULL;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGenericPlugin::IsLoaded( void ) const
{GUCEF_TRACE;

    return NULL != m_moduleHandle;
}

/*-------------------------------------------------------------------------*/

CString
CGenericPlugin::GetDescription( void ) const
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        return reinterpret_cast< TGUCEFGENERICPLUGFPTR_GetDescription >( m_funcPointers[ GPLUGINFUNCPTR_DESCRIPTION ] )();
    }

    return CString();
}

/*-------------------------------------------------------------------------*/

CString
CGenericPlugin::GetCopyright( void ) const
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        return reinterpret_cast< TGUCEFGENERICPLUGFPTR_GetCopyright >( m_funcPointers[ GPLUGINFUNCPTR_COPYRIGHT ] )();
    }

    return CString();
}

/*-------------------------------------------------------------------------*/

TVersion
CGenericPlugin::GetVersion( void ) const
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        TVersion versionInfo;
        reinterpret_cast< TGUCEFGENERICPLUGFPTR_GetVersion >( m_funcPointers[ GPLUGINFUNCPTR_VERSION ] )( &versionInfo );

        return versionInfo;
    }

    TVersion unknownVersion = { -1, -1, -1, -1 };
    return unknownVersion;
}


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
