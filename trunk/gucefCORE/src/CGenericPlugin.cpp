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

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "dvoswrap.h"
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
    : m_moduleHandle( NULL ) ,
      m_modulePath()
{GUCEF_TRACE;
    
    memset( m_funcPointers, 0, GPLUGINFUNCPTR_COUNT );
}

/*-------------------------------------------------------------------------*/

CGenericPlugin::~CGenericPlugin()
{GUCEF_TRACE;

    Unload();
}

/*-------------------------------------------------------------------------*/

bool
CGenericPlugin::Load( const CString& pluginPath )
{GUCEF_TRACE;
    
    if ( !IsLoaded() )
    {
        m_moduleHandle = LoadModuleDynamicly( pluginPath.C_String() );
        if ( NULL != m_moduleHandle )
        {
            // We have now successfully loaded the module itself
            // We will now try to lookup the function pointers
            m_funcPointers[ GPLUGINFUNCPTR_LOAD ] = GetFunctionAddress( m_moduleHandle     ,
                                                                        "GUCEFPlugin_Load" ,
                                                                        0                  );
            m_funcPointers[ GPLUGINFUNCPTR_UNLOAD ] = GetFunctionAddress( m_moduleHandle       ,
                                                                          "GUCEFPlugin_Unload" ,
                                                                          0                    );            
            m_funcPointers[ GPLUGINFUNCPTR_VERSION ] = GetFunctionAddress( m_moduleHandle           ,
                                                                           "GUCEFPlugin_GetVersion" ,
                                                                           sizeof( TVersion* )      );            
            m_funcPointers[ GPLUGINFUNCPTR_DESCRIPTION ] = GetFunctionAddress( m_moduleHandle               ,
                                                                               "GUCEFPlugin_GetDescription" ,
                                                                               0                            );            

            m_funcPointers[ GPLUGINFUNCPTR_COPYRIGHT ] = GetFunctionAddress( m_moduleHandle           ,
                                                                             "GUCEFPlugin_GetCopyright" ,
                                                                             0                        );

            // Verify that all function pointers are loaded correctly
            if ( ( NULL != m_funcPointers[ GPLUGINFUNCPTR_LOAD ] )        &&
                 ( NULL != m_funcPointers[ GPLUGINFUNCPTR_UNLOAD ] )      &&
                 ( NULL != m_funcPointers[ GPLUGINFUNCPTR_VERSION ] )     &&
                 ( NULL != m_funcPointers[ GPLUGINFUNCPTR_DESCRIPTION ] ) &&
                 ( NULL != m_funcPointers[ GPLUGINFUNCPTR_COPYRIGHT ] )    )
            {
                // Call the module's Load()
                static_cast< TGUCEFGENERICPLUGFPTR_Load >( m_funcPointers[ GPLUGINFUNCPTR_LOAD ] )();
                
                // We have loaded & linked our plugin module
                return true;
            }
                        
            // We failed to link the functions :(
            memset( m_funcPointers, 0, GPLUGINFUNCPTR_COUNT );
            UnloadModuleDynamicly( m_moduleHandle );
            m_moduleHandle = NULL;
            return false;
        }
    }
    
    // Unloading->Loading must be done explicitly
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CGenericPlugin::Unload( void )
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        // Call the module's Unload()
        static_cast< TGUCEFGENERICPLUGFPTR_Unload >( m_funcPointers[ GPLUGINFUNCPTR_UNLOAD ] )();

        // Cleanup recources
        memset( m_funcPointers, 0, GPLUGINFUNCPTR_COUNT );
        UnloadModuleDynamicly( m_moduleHandle );
        m_moduleHandle = NULL;
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
        return static_cast< TGUCEFGENERICPLUGFPTR_GetDescription >( m_funcPointers[ GPLUGINFUNCPTR_DESCRIPTION ] )();
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "CGenericPlugin::GetDescription(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CString
CGenericPlugin::GetCopyright( void ) const
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        return static_cast< TGUCEFGENERICPLUGFPTR_GetCopyright >( m_funcPointers[ GPLUGINFUNCPTR_COPYRIGHT ] )();
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "CGenericPlugin::GetCopyright(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
TVersion
CGenericPlugin::GetVersion( void ) const
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        TVersion versionInfo;
        static_cast< TGUCEFGENERICPLUGFPTR_GetVersion >( m_funcPointers[ GPLUGINFUNCPTR_VERSION ] )( &versionInfo );
        
        return versionInfo;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "CGenericPlugin::GetVersion(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CString
CGenericPlugin::GetModulePath( void ) const
{GUCEF_TRACE;
    
    if ( IsLoaded() )
    {
        return m_modulePath;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "CGenericPlugin::GetModulePath(): No module is loaded" );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/