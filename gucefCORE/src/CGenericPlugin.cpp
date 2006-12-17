/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
{TRACE;
    
    memset( m_funcPointers, 0, GPLUGINFUNCPTR_COUNT );
}

/*-------------------------------------------------------------------------*/

CGenericPlugin::~CGenericPlugin()
{TRACE;

    Unload();
}

/*-------------------------------------------------------------------------*/

bool
CGenericPlugin::Load( const CString& pluginPath )
{TRACE;
    
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
                                                                           sizeof( TVersion )       );            
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
{TRACE;

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
{TRACE;
    
    return NULL != m_moduleHandle;
}

/*-------------------------------------------------------------------------*/
    
CString
CGenericPlugin::GetDescription( void ) const
{TRACE;

    if ( IsLoaded() )
    {
        return static_cast< TGUCEFGENERICPLUGFPTR_GetDescription >( m_funcPointers[ GPLUGINFUNCPTR_DESCRIPTION ] )();
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "CGenericPlugin::GetDescription(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CString
CGenericPlugin::GetCopyright( void ) const
{TRACE;

    if ( IsLoaded() )
    {
        return static_cast< TGUCEFGENERICPLUGFPTR_GetCopyright >( m_funcPointers[ GPLUGINFUNCPTR_COPYRIGHT ] )();
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "CGenericPlugin::GetCopyright(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
TVersion
CGenericPlugin::GetVersion( void ) const
{TRACE;

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
{TRACE;
    
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