/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef DVOSWRAP_H
#include "dvoswrap.h"            /* OS wrapping functionality */
#define DVOSWRAP_H
#endif /* DVOSWRAP_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef INPUTDRIVERPLUGINSTRUCTS_H
#include "inputdriverpluginstructs.h"   /* plugin API structs */
#define INPUTDRIVERPLUGINSTRUCTS_H
#endif /* INPUTDRIVERPLUGINSTRUCTS_H ? */

#ifndef CIINPUTHANDLER_H
#include "CIInputHandler.h"
#define CIINPUTHANDLER_H
#endif /* CIINPUTHANDLER_H ? */

#include "CInputDriverPlugin.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum
{
        INPUTDRIVERPLUG_INIT            = 0        ,
        INPUTDRIVERPLUG_SHUTDOWN                   ,
        INPUTDRIVERPLUG_NAME                       ,
        INPUTDRIVERPLUG_COPYRIGHT                  ,
        INPUTDRIVERPLUG_VERSION                    ,
        
        INPUTDRIVERPLUG_UPDATE                     ,
        
        INPUTDRIVERPLUG_CREATECONTEXT              ,
        INPUTDRIVERPLUG_DESTROYCONTEXT             ,
        
        INPUTDRIVERPLUG_GETMOUSEPOS                ,
        INPUTDRIVERPLUG_GETKEYBOARDKEYSTATES       ,
        INPUTDRIVERPLUG_GETMOUSEBUTTONPRESSEDSTATE ,
        INPUTDRIVERPLUG_GETKEYBOARDKEYPRESSEDSTATE ,
        INPUTDRIVERPLUG_GETDEVICEBOOLSTATE         ,
        INPUTDRIVERPLUG_GETDEVICEVARSTATE          ,
        
        INPUTDRIVERPLUG_LASTPTR
};

/*-------------------------------------------------------------------------*/

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Init )                   ( void** plugdata, const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Shutdown )                 ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Name )              ( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Copyright )         ( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const CORE::TVersion* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Version ) ( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Update )      ( void* plugdata, void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_CreateContext )  ( void* plugdata, void** contextdata, const char*** args, const TInputCallbacks* callbacks ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_DestroyContext ) ( void* plugdata, void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetMousePos ) ( void* plugdata, void* contextdata, UInt32* xpos, UInt32* ypos ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt8* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetKeyBoardKeyStates ) ( void* plugdata, void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetMouseButtonPressedState ) ( void* plugdata, void* contextdata, const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetKeyboardKeyPressedState ) ( void* plugdata, void* contextdata, const UInt32 keyindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetDeviceBoolState ) ( void* plugdata, void* contextdata, const UInt32 deviceid, const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetDeviceVarState ) ( void* plugdata, void* contextdata, const UInt32 deviceid, const UInt32 stateindex, Float32* varstate ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CPluginInputContext : public CInputContext
{
        public:
        CPluginInputContext( const CORE::CValueList& params ,
                             CIInputHandler* handler        ,
                             void* data = NULL              )
         : CInputContext( params, handler ) ,
           m_contextdata( data )
        {GUCEF_TRACE;
        }                
        
        virtual ~CPluginInputContext()
        {GUCEF_TRACE;
        }
        
        CIInputHandler* DoGetMutableHandler( void )
        {GUCEF_TRACE;
                return GetMutableHandler();        
        }
        
        void* m_contextdata;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
onMouseButtonDown( void* userData           , 
                   const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnMouseButtonDown( *context    ,
                                                                   buttonindex );
        }                
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
onMouseButtonUp( void* userData           , 
                 const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnMouseButtonUp( *context    ,
                                                                 buttonindex );
        }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
onMouseMove( void* userData     , 
             const Int32 xPos   ,
             const Int32 yPos   ,
             const Int32 xDelta ,
             const Int32 yDelta ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnMouseMove( *context ,
                                                             xPos     ,
                                                             yPos     ,
                                                             xDelta   ,
                                                             yDelta   );
        }        
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
onMouseVarChanged( void* userData         , 
                   const UInt32 varIndex  ,
                   const Int32 value      ,
                   const Int32 valueDelta ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnMouseVariableChanged( *context   ,
                                                                        varIndex   ,
                                                                        value      ,
                                                                        valueDelta );
        }        
}        

/*-------------------------------------------------------------------------*/
        
void GUCEF_PLUGIN_CALLSPEC_PREFIX
onKeyboardKeyDown( void* userData           , 
                   const UInt32 keyindex    ,
                   const UInt32 keyModState ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnKeyboardKeyDown( *context    ,
                                                                   keyindex    ,
                                                                   keyModState );
        }
}
        
/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
onKeyboardKeyUp( void* userData           , 
                 const UInt32 keyindex    ,
                 const UInt32 keyModState ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnKeyboardKeyUp( *context    ,  
                                                                 keyindex    ,
                                                                 keyModState );
        }
}
        
/*-------------------------------------------------------------------------*/
        
void GUCEF_PLUGIN_CALLSPEC_PREFIX
onDeviceBooleanOff( void* userData          , 
                    const UInt32 deviceid   , 
                    const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnDeviceBooleanOn( *context   ,
                                                                   deviceid   ,
                                                                   stateindex );
        }
}
        
/*-------------------------------------------------------------------------*/
        
void GUCEF_PLUGIN_CALLSPEC_PREFIX
onDeviceBooleanOn( void* userData          , 
                   const UInt32 deviceid   , 
                   const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnDeviceBooleanOff( *context   ,
                                                                    deviceid   ,
                                                                    stateindex );
        }
}
        
/*-------------------------------------------------------------------------*/
        
void GUCEF_PLUGIN_CALLSPEC_PREFIX
onDeviceVarChanged( void* userData          , 
                    const UInt32 deviceid   , 
                    const UInt32 stateindex , 
                    const Float32 value     ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnDeviceVariableChanged( *context   ,
                                                                         deviceid   ,
                                                                         stateindex ,
                                                                         value      );
        }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputDriverPlugin::CInputDriverPlugin( void )
        : m_sohandle( NULL )
{GUCEF_TRACE;    

}

/*-------------------------------------------------------------------------*/

CInputDriverPlugin::CInputDriverPlugin( const CInputDriverPlugin& src )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CInputDriverPlugin::~CInputDriverPlugin()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CInputDriverPlugin&
CInputDriverPlugin::operator=( const CInputDriverPlugin& src )
{GUCEF_TRACE;
        return *this;
}

/*-------------------------------------------------------------------------*/

const CORE::TVersion*
CInputDriverPlugin::GetVersion( void )
{
        return ( (TINPUTDRIVERPLUGFPTR_Version) m_fptable[ INPUTDRIVERPLUG_VERSION ] )( &m_plugdata );
}

/*-------------------------------------------------------------------------*/

bool 
CInputDriverPlugin::LoadModule( const CORE::CString& filename  ,
                                const CORE::CValueList& params )
{GUCEF_TRACE;
        m_sohandle = CORE::LoadModuleDynamicly( filename.C_String() );
        if ( !m_sohandle ) return false;               
        
        /*
         *      Link up generic module utilities
         */
        m_fptable[ INPUTDRIVERPLUG_INIT ] = CORE::GetFunctionAddress( m_sohandle             ,
                                                                      "INPUTDRIVERPLUG_Init" ,
                                                                      2*sizeof(void*)              );
        m_fptable[ INPUTDRIVERPLUG_SHUTDOWN ] = CORE::GetFunctionAddress( m_sohandle                 ,
                                                                          "INPUTDRIVERPLUG_Shutdown" ,
                                                                          1*sizeof(void*)                  );
        m_fptable[ INPUTDRIVERPLUG_NAME ] = CORE::GetFunctionAddress( m_sohandle             ,
                                                                      "INPUTDRIVERPLUG_Name" ,
                                                                      1*sizeof(void*)              );
        m_fptable[ INPUTDRIVERPLUG_COPYRIGHT ] = CORE::GetFunctionAddress( m_sohandle                  ,
                                                                           "INPUTDRIVERPLUG_Copyright" ,
                                                                           1*sizeof(void*)                   );
        m_fptable[ INPUTDRIVERPLUG_VERSION ] = CORE::GetFunctionAddress( m_sohandle                ,
                                                                         "INPUTDRIVERPLUG_Version" ,
                                                                         1*sizeof(void*)                 );


        /*
         *      Link up driver utilities
         */

                                                          
        m_fptable[ INPUTDRIVERPLUG_UPDATE ] = CORE::GetFunctionAddress( m_sohandle               ,
                                                                        "INPUTDRIVERPLUG_Update" ,
                                                                        2*sizeof(void*)                );
        m_fptable[ INPUTDRIVERPLUG_CREATECONTEXT ] = CORE::GetFunctionAddress( m_sohandle                      ,
                                                                               "INPUTDRIVERPLUG_CreateContext" ,
                                                                               4*sizeof(void*)                       );
        m_fptable[ INPUTDRIVERPLUG_DESTROYCONTEXT ] = CORE::GetFunctionAddress( m_sohandle                       ,
                                                                                "INPUTDRIVERPLUG_DestroyContext" ,
                                                                                2*sizeof(void*)                        );


        m_fptable[ INPUTDRIVERPLUG_GETMOUSEPOS ] = CORE::GetFunctionAddress( m_sohandle               ,
                                                                             "INPUTDRIVERPLUG_GetMousePos" ,
                                                                             4*sizeof(void*)                     );
        m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYSTATES ] = CORE::GetFunctionAddress( m_sohandle                             ,
                                                                                      "INPUTDRIVERPLUG_GetKeyBoardKeyStates" ,
                                                                                      2*sizeof(void*)                              );
        m_fptable[ INPUTDRIVERPLUG_GETMOUSEBUTTONPRESSEDSTATE ] = CORE::GetFunctionAddress( m_sohandle                                   ,
                                                                                            "INPUTDRIVERPLUG_GetMouseButtonPressedState" ,
                                                                                            2*sizeof(void*) + 4                                );
        m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYPRESSEDSTATE ] = CORE::GetFunctionAddress( m_sohandle                                   ,
                                                                                            "INPUTDRIVERPLUG_GetKeyboardKeyPressedState" ,
                                                                                            2*sizeof(void*) + 4                                );
        m_fptable[ INPUTDRIVERPLUG_GETDEVICEBOOLSTATE ] = CORE::GetFunctionAddress( m_sohandle                           ,
                                                                                    "INPUTDRIVERPLUG_GetDeviceBoolState" ,
                                                                                    2*sizeof(void*) + 8                        );
        m_fptable[ INPUTDRIVERPLUG_GETDEVICEVARSTATE ] = CORE::GetFunctionAddress( m_sohandle                          ,
                                                                                   "INPUTDRIVERPLUG_GetDeviceVarState" ,
                                                                                   2*sizeof(void*) + 8                       );
                                                                                                                       
        
        if ( ( !m_fptable[ INPUTDRIVERPLUG_INIT ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_SHUTDOWN ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_NAME ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_COPYRIGHT ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_VERSION ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_UPDATE ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_CREATECONTEXT ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_DESTROYCONTEXT ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_GETMOUSEPOS ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYSTATES ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_GETMOUSEBUTTONPRESSEDSTATE ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYPRESSEDSTATE ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_GETDEVICEBOOLSTATE ] ) ||
             ( !m_fptable[ INPUTDRIVERPLUG_GETDEVICEVARSTATE ] ) )
        {
                CORE::UnloadModuleDynamicly( m_sohandle );
                memset( m_fptable, NULL, sizeof(void*) * INPUTDRIVERPLUG_LASTPTR );
                m_sohandle = NULL;
                
                GUCEF_DEBUG_LOG( "Invalid codec module" );
                return false;        
        }
        
        /*
         *      Intialize the plugin module
         */
        char*** argmatrix = CreateArgMatrix( params ); 
        ( (TINPUTDRIVERPLUGFPTR_Init) m_fptable[ INPUTDRIVERPLUG_INIT ] )( &m_plugdata, const_cast< const char*** >( argmatrix ) );
        DestroyArgMatrix( argmatrix );
        return true;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputDriverPlugin::UnloadModule( void )
{GUCEF_TRACE;
        ( (TINPUTDRIVERPLUGFPTR_Shutdown) m_fptable[ INPUTDRIVERPLUG_SHUTDOWN ] )( m_plugdata );
        
        CORE::UnloadModuleDynamicly( m_sohandle );
        memset( m_fptable, NULL, sizeof(void*) * INPUTDRIVERPLUG_LASTPTR );
        m_sohandle = NULL;
}

/*-------------------------------------------------------------------------*/

void
CInputDriverPlugin::GetMousePos( CInputContext* context ,
                                 UInt32* xpos           ,
                                 UInt32* ypos           )
{GUCEF_TRACE;
        ( (TINPUTDRIVERPLUGFPTR_GetMousePos) m_fptable[ INPUTDRIVERPLUG_GETMOUSEPOS ] )( m_plugdata                                                  , 
                                                                                         static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                         xpos                                                        ,
                                                                                         ypos                                                        );
}

/*-------------------------------------------------------------------------*/
        
const UInt8* 
CInputDriverPlugin::GetKeyboardKeyStates( CInputContext* context )
{GUCEF_TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetKeyBoardKeyStates) m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYSTATES ] )( &m_plugdata                                                 , 
                                                                                                                  static_cast<CPluginInputContext*>( context )->m_contextdata );
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputDriverPlugin::GetMouseButtonPressedState( CInputContext* context   ,
                                                const UInt32 buttonindex )
{GUCEF_TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetMouseButtonPressedState) m_fptable[ INPUTDRIVERPLUG_GETMOUSEBUTTONPRESSEDSTATE ] )( m_plugdata                                                  ,
                                                                                                                              static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                                                              buttonindex                                                 ) > 0;
}        

/*-------------------------------------------------------------------------*/

bool 
CInputDriverPlugin::GetKeyboardKeyPressedState( CInputContext* context ,
                                                const UInt32 keyindex  )
{GUCEF_TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetKeyboardKeyPressedState) m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYPRESSEDSTATE ] )( m_plugdata                                                  ,
                                                                                                                              static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                                                              keyindex                                                    ) > 0;
}

/*-------------------------------------------------------------------------*/                                                
        
bool 
CInputDriverPlugin::GetDeviceBooleanState( CInputContext* context  ,
                                           const UInt32 deviceid   ,
                                           const UInt32 stateindex )
{GUCEF_TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetDeviceBoolState) m_fptable[ INPUTDRIVERPLUG_GETDEVICEBOOLSTATE ] )( m_plugdata                                                  ,
                                                                                                              static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                                              deviceid                                                    ,
                                                                                                              stateindex                                                  ) > 0;
}                                           
        
/*-------------------------------------------------------------------------*/

bool 
CInputDriverPlugin::GetDeviceVariableState( CInputContext* context  ,
                                            const UInt32 deviceid   ,
                                            const UInt32 stateindex ,
                                            Float32& varstate       )
{GUCEF_TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetDeviceVarState) m_fptable[ INPUTDRIVERPLUG_GETDEVICEVARSTATE ] )( m_plugdata                                                  ,
                                                                                                            static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                                            deviceid                                                    ,
                                                                                                            stateindex                                                  ,
                                                                                                            &varstate                                                   ) > 0;
}                                            

/*-------------------------------------------------------------------------*/

bool 
CInputDriverPlugin::OnUpdate( const UInt64 tickcount               ,
                              const Float64 updateDeltaInMilliSecs ,
                              CInputContext* context               )
{GUCEF_TRACE;
        
        return ( (TINPUTDRIVERPLUGFPTR_Update) m_fptable[ INPUTDRIVERPLUG_UPDATE ] )( m_plugdata, static_cast<CPluginInputContext*>( context )->m_contextdata ) > 0;
}

/*-------------------------------------------------------------------------*/

char***
CInputDriverPlugin::CreateArgMatrix( const CORE::CValueList& params )
{GUCEF_TRACE;
        char*** argmatrix = new char**[ params.GetCount()+1 ];
        CORE::CString value, key;
        for ( UInt32 i=0; i<params.GetCount(); ++i )
        {
                value = params.GetValue( i );
                key = params.GetKey( i );
                
                argmatrix[ i ] = new char*[ 2 ];
                argmatrix[ i ][ 0 ] = new char[ key.Length()+1 ];
                memcpy( argmatrix[ i ][ 0 ], key.C_String(), key.Length()+1 );  
                argmatrix[ i ][ 1 ] = new char[ value.Length()+1 ];
                memcpy( argmatrix[ i ][ 1 ], value.C_String(), value.Length()+1 );
        }
        argmatrix[ params.GetCount() ] = NULL;
        
        return argmatrix;
}

/*-------------------------------------------------------------------------*/

void 
CInputDriverPlugin::DestroyArgMatrix( char*** argmatrix )
{GUCEF_TRACE;
        for ( UInt32 i=0; argmatrix[ i ]>NULL; ++i )
        {
                delete [](argmatrix[ i ][ 0 ]);                
                delete [](argmatrix[ i ][ 1 ]);
                delete [](argmatrix[ i ]);
        }
        delete []argmatrix;
}

/*-------------------------------------------------------------------------*/

CInputContext* 
CInputDriverPlugin::CreateContext( const CORE::CValueList& params       ,
                                   CIInputHandler* handler /* = NULL */ )
{GUCEF_TRACE;       
        CPluginInputContext* context( new CPluginInputContext( params, handler ) ); 
        
        TInputCallbacks callbacks;
        callbacks.onMouseButtonDown  = onMouseButtonDown;
        callbacks.onMouseButtonUp    = onMouseButtonUp;
        callbacks.onMouseMove        = onMouseMove;
        callbacks.onMouseVarChanged  = onMouseVarChanged;
        callbacks.onKeyboardKeyDown  = onKeyboardKeyDown;
        callbacks.onKeyboardKeyUp    = onKeyboardKeyUp;
        callbacks.onDeviceBooleanOff = onDeviceBooleanOff;
        callbacks.onDeviceBooleanOn  = onDeviceBooleanOn;
        callbacks.onDeviceVarChanged = onDeviceVarChanged;
        callbacks.userData = context;
        
        char*** argmatrix = CreateArgMatrix( params );
        if ( ( (TINPUTDRIVERPLUGFPTR_CreateContext) m_fptable[ INPUTDRIVERPLUG_CREATECONTEXT ] )( m_plugdata, &context->m_contextdata, const_cast<const char***>( argmatrix ), &callbacks ) )
        {        
                DestroyArgMatrix( argmatrix );          
                return context;
        }
        delete context;
        DestroyArgMatrix( argmatrix );
        return NULL;                
}

/*-------------------------------------------------------------------------*/
        
void 
CInputDriverPlugin::DeleteContext( CInputContext* context )
{GUCEF_TRACE;
        ( (TINPUTDRIVERPLUGFPTR_DestroyContext) m_fptable[ INPUTDRIVERPLUG_DESTROYCONTEXT ] )( m_plugdata, static_cast<CPluginInputContext*>( context )->m_contextdata );
        delete static_cast<CPluginInputContext*>( context );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
