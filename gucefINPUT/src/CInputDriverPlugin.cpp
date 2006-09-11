/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef DVOSWRAP_H
#include "dvoswrap.h"            /* OS wrapping functionality */
#define DVOSWRAP_H
#endif /* DVOSWRAP_H ? */

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

typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Init )                   ( void** plugdata, const char*** args ) PLUGIN_CALLSPEC_SUFFIX;
typedef void ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Shutdown )                 ( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Name )              ( const void* plugdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Copyright )         ( const void* plugdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef const CORE::TVersion* ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Version ) ( const void* plugdata ) PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_Update )      ( void* plugdata, void* contextdata ) PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_CreateContext )  ( void* plugdata, void** contextdata, const char*** args, const TInputCallbacks* callbacks ) PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_DestroyContext ) ( void* plugdata, void* contextdata ) PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetMousePos ) ( void* plugdata, void* contextdata, UInt32* xpos, UInt32* ypos ) PLUGIN_CALLSPEC_SUFFIX;
typedef UInt8* ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetKeyBoardKeyStates ) ( void* plugdata, void* contextdata ) PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetMouseButtonPressedState ) ( void* plugdata, void* contextdata, const UInt32 buttonindex ) PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetKeyboardKeyPressedState ) ( void* plugdata, void* contextdata, const UInt32 keyindex ) PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetDeviceBoolState ) ( void* plugdata, void* contextdata, const UInt32 deviceid, const UInt32 stateindex ) PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( PLUGIN_CALLSPEC_PREFIX *TINPUTDRIVERPLUGFPTR_GetDeviceVarState ) ( void* plugdata, void* contextdata, const UInt32 deviceid, const UInt32 stateindex, Float32* varstate ) PLUGIN_CALLSPEC_SUFFIX;

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
        {TRACE;
        }                
        
        virtual ~CPluginInputContext()
        {TRACE;
        }
        
        CIInputHandler* DoGetMutableHandler( void )
        {TRACE;
                return GetMutableHandler();        
        }
        
        void* m_contextdata;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void PLUGIN_CALLSPEC_PREFIX
onMouseButtonDown( void* userData           , 
                   const UInt32 buttonindex ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnMouseButtonDown( *context    ,
                                                                   buttonindex );
        }                
}

/*-------------------------------------------------------------------------*/

void PLUGIN_CALLSPEC_PREFIX
onMouseButtonUp( void* userData           , 
                 const UInt32 buttonindex ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnMouseButtonUp( *context    ,
                                                                 buttonindex );
        }
}

/*-------------------------------------------------------------------------*/

void PLUGIN_CALLSPEC_PREFIX
onMouseMove( void* userData     , 
             const Int32 xPos   ,
             const Int32 yPos   ,
             const Int32 xDelta ,
             const Int32 yDelta ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
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

void PLUGIN_CALLSPEC_PREFIX
onMouseVarChanged( void* userData         , 
                   const UInt32 varIndex  ,
                   const Int32 value      ,
                   const Int32 valueDelta ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
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
        
void PLUGIN_CALLSPEC_PREFIX
onKeyboardKeyDown( void* userData           , 
                   const UInt32 keyindex    ,
                   const UInt32 keyModState ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnKeyboardKeyDown( *context    ,
                                                                   keyindex    ,
                                                                   keyModState );
        }
}
        
/*-------------------------------------------------------------------------*/

void PLUGIN_CALLSPEC_PREFIX
onKeyboardKeyUp( void* userData           , 
                 const UInt32 keyindex    ,
                 const UInt32 keyModState ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnKeyboardKeyUp( *context    ,  
                                                                 keyindex    ,
                                                                 keyModState );
        }
}
        
/*-------------------------------------------------------------------------*/
        
void PLUGIN_CALLSPEC_PREFIX
onDeviceBooleanOff( void* userData          , 
                    const UInt32 deviceid   , 
                    const UInt32 stateindex ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnDeviceBooleanOn( *context   ,
                                                                   deviceid   ,
                                                                   stateindex );
        }
}
        
/*-------------------------------------------------------------------------*/
        
void PLUGIN_CALLSPEC_PREFIX
onDeviceBooleanOn( void* userData          , 
                   const UInt32 deviceid   , 
                   const UInt32 stateindex ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
        CPluginInputContext* context( static_cast<CPluginInputContext*>( userData ) );
        if ( NULL != context->DoGetMutableHandler() )
        {
                context->DoGetMutableHandler()->OnDeviceBooleanOff( *context   ,
                                                                    deviceid   ,
                                                                    stateindex );
        }
}
        
/*-------------------------------------------------------------------------*/
        
void PLUGIN_CALLSPEC_PREFIX
onDeviceVarChanged( void* userData          , 
                    const UInt32 deviceid   , 
                    const UInt32 stateindex , 
                    const Float32 value     ) PLUGIN_CALLSPEC_SUFFIX
{TRACE;
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
{TRACE;    

}

/*-------------------------------------------------------------------------*/

CInputDriverPlugin::CInputDriverPlugin( const CInputDriverPlugin& src )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CInputDriverPlugin::~CInputDriverPlugin()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CInputDriverPlugin&
CInputDriverPlugin::operator=( const CInputDriverPlugin& src )
{TRACE;
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
{TRACE;
        m_sohandle = CORE::LoadModuleDynamicly( filename.C_String() );
        if ( !m_sohandle ) return false;               
        
        /*
         *      Link up generic module utilities
         */
        m_fptable[ INPUTDRIVERPLUG_INIT ] = CORE::GetFunctionAddress( m_sohandle             ,
                                                                      "INPUTDRIVERPLUG_Init" ,
                                                                      2*PTRSIZE              );
        m_fptable[ INPUTDRIVERPLUG_SHUTDOWN ] = CORE::GetFunctionAddress( m_sohandle                 ,
                                                                          "INPUTDRIVERPLUG_Shutdown" ,
                                                                          1*PTRSIZE                  );
        m_fptable[ INPUTDRIVERPLUG_NAME ] = CORE::GetFunctionAddress( m_sohandle             ,
                                                                      "INPUTDRIVERPLUG_Name" ,
                                                                      1*PTRSIZE              );
        m_fptable[ INPUTDRIVERPLUG_COPYRIGHT ] = CORE::GetFunctionAddress( m_sohandle                  ,
                                                                           "INPUTDRIVERPLUG_Copyright" ,
                                                                           1*PTRSIZE                   );
        m_fptable[ INPUTDRIVERPLUG_VERSION ] = CORE::GetFunctionAddress( m_sohandle                ,
                                                                         "INPUTDRIVERPLUG_Version" ,
                                                                         1*PTRSIZE                 );


        /*
         *      Link up driver utilities
         */

                                                          
        m_fptable[ INPUTDRIVERPLUG_UPDATE ] = CORE::GetFunctionAddress( m_sohandle               ,
                                                                        "INPUTDRIVERPLUG_Update" ,
                                                                        2*PTRSIZE                );
        m_fptable[ INPUTDRIVERPLUG_CREATECONTEXT ] = CORE::GetFunctionAddress( m_sohandle                      ,
                                                                               "INPUTDRIVERPLUG_CreateContext" ,
                                                                               4*PTRSIZE                       );
        m_fptable[ INPUTDRIVERPLUG_DESTROYCONTEXT ] = CORE::GetFunctionAddress( m_sohandle                       ,
                                                                                "INPUTDRIVERPLUG_DestroyContext" ,
                                                                                2*PTRSIZE                        );


        m_fptable[ INPUTDRIVERPLUG_GETMOUSEPOS ] = CORE::GetFunctionAddress( m_sohandle               ,
                                                                             "INPUTDRIVERPLUG_GetMousePos" ,
                                                                             4*PTRSIZE                     );
        m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYSTATES ] = CORE::GetFunctionAddress( m_sohandle                             ,
                                                                                      "INPUTDRIVERPLUG_GetKeyBoardKeyStates" ,
                                                                                      2*PTRSIZE                              );
        m_fptable[ INPUTDRIVERPLUG_GETMOUSEBUTTONPRESSEDSTATE ] = CORE::GetFunctionAddress( m_sohandle                                   ,
                                                                                            "INPUTDRIVERPLUG_GetMouseButtonPressedState" ,
                                                                                            2*PTRSIZE + 4                                );
        m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYPRESSEDSTATE ] = CORE::GetFunctionAddress( m_sohandle                                   ,
                                                                                            "INPUTDRIVERPLUG_GetKeyboardKeyPressedState" ,
                                                                                            2*PTRSIZE + 4                                );
        m_fptable[ INPUTDRIVERPLUG_GETDEVICEBOOLSTATE ] = CORE::GetFunctionAddress( m_sohandle                           ,
                                                                                    "INPUTDRIVERPLUG_GetDeviceBoolState" ,
                                                                                    2*PTRSIZE + 8                        );
        m_fptable[ INPUTDRIVERPLUG_GETDEVICEVARSTATE ] = CORE::GetFunctionAddress( m_sohandle                          ,
                                                                                   "INPUTDRIVERPLUG_GetDeviceVarState" ,
                                                                                   2*PTRSIZE + 8                       );
                                                                                                                       
        
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
                
                DEBUGOUTPUT( "Invalid codec module" );
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
{TRACE;
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
{TRACE;
        ( (TINPUTDRIVERPLUGFPTR_GetMousePos) m_fptable[ INPUTDRIVERPLUG_GETMOUSEPOS ] )( m_plugdata                                                  , 
                                                                                         static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                         xpos                                                        ,
                                                                                         ypos                                                        );
}

/*-------------------------------------------------------------------------*/
        
const UInt8* 
CInputDriverPlugin::GetKeyboardKeyStates( CInputContext* context )
{TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetKeyBoardKeyStates) m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYSTATES ] )( &m_plugdata                                                 , 
                                                                                                                  static_cast<CPluginInputContext*>( context )->m_contextdata );
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputDriverPlugin::GetMouseButtonPressedState( CInputContext* context   ,
                                                const UInt32 buttonindex )
{TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetMouseButtonPressedState) m_fptable[ INPUTDRIVERPLUG_GETMOUSEBUTTONPRESSEDSTATE ] )( m_plugdata                                                  ,
                                                                                                                              static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                                                              buttonindex                                                 ) > 0;
}        

/*-------------------------------------------------------------------------*/

bool 
CInputDriverPlugin::GetKeyboardKeyPressedState( CInputContext* context ,
                                                const UInt32 keyindex  )
{TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetKeyboardKeyPressedState) m_fptable[ INPUTDRIVERPLUG_GETKEYBOARDKEYPRESSEDSTATE ] )( m_plugdata                                                  ,
                                                                                                                              static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                                                              keyindex                                                    ) > 0;
}

/*-------------------------------------------------------------------------*/                                                
        
bool 
CInputDriverPlugin::GetDeviceBooleanState( CInputContext* context  ,
                                           const UInt32 deviceid   ,
                                           const UInt32 stateindex )
{TRACE;
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
{TRACE;
        return ( (TINPUTDRIVERPLUGFPTR_GetDeviceVarState) m_fptable[ INPUTDRIVERPLUG_GETDEVICEVARSTATE ] )( m_plugdata                                                  ,
                                                                                                            static_cast<CPluginInputContext*>( context )->m_contextdata ,
                                                                                                            deviceid                                                    ,
                                                                                                            stateindex                                                  ,
                                                                                                            &varstate                                                   ) > 0;
}                                            

/*-------------------------------------------------------------------------*/

bool 
CInputDriverPlugin::OnUpdate( const UInt32 tickcount  ,
                              const UInt32 deltaticks ,
                              CInputContext* context  )
{TRACE;
        
        return ( (TINPUTDRIVERPLUGFPTR_Update) m_fptable[ INPUTDRIVERPLUG_UPDATE ] )( m_plugdata, static_cast<CPluginInputContext*>( context )->m_contextdata ) > 0;
}

/*-------------------------------------------------------------------------*/

char***
CInputDriverPlugin::CreateArgMatrix( const CORE::CValueList& params )
{TRACE;
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
{TRACE;
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
{TRACE;       
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
{TRACE;
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
