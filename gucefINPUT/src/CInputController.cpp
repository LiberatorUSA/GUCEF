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

#include <assert.h>

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */ 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_INPUT_CINPUTDRIVER_H
#include "gucefINPUT_CInputDriver.h"
#define GUCEF_INPUT_CINPUTDRIVER_H
#endif /* GUCEF_INPUT_CINPUTDRIVER_H ? */

#ifndef CINPUTDRIVERPLUGIN_H
#include "CInputDriverPlugin.h"
#define CINPUTDRIVERPLUGIN_H
#endif /* CINPUTDRIVERPLUGIN_H ? */

#include "CInputController.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CInputController::InputDriverLoadedEvent = "GUCEF::INPUT::InputDriverLoadedEvent";
const CORE::CEvent CInputController::InputDriverUnloadedEvent = "GUCEF::INPUT::InputDriverUnloadedEvent";
CInputController* CInputController::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_IMPLEMENT_MSGEXCEPTION( CInputController, EInvalidIndex );

/*-------------------------------------------------------------------------*/

CInputController::CInputController( void )
        : m_driverisplugin( false ) ,
          m_driver( NULL )          ,
          m_keyboard( NULL )        ,
          m_mouseVector()
          
          #ifdef GUCEF_MSWIN_BUILD
          ,
          m_hinstance(0UL)
          #endif
{GUCEF_TRACE;

    RegisterEvents();
    
    m_keyboard = new CKeyboard( this );
    
    m_mouseVector.reserve( 2 );
    m_mouseVector.push_back( new CMouse( 0 ) );
    m_mouseVector.push_back( new CMouse( 1 ) );
}

/*-------------------------------------------------------------------------*/

CInputController::~CInputController()
{GUCEF_TRACE;

    delete m_keyboard;
    m_keyboard = NULL;
}

/*-------------------------------------------------------------------------*/

CInputController* 
CInputController::Instance( void )
{GUCEF_TRACE;

    if ( !m_instance )
    {
        m_instance = new CInputController();
    }
    return m_instance;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputController::Deinstance( void )
{GUCEF_TRACE;
    
    delete m_instance;
    m_instance = NULL;
}

/*-------------------------------------------------------------------------*/
        
CInputContext* 
CInputController::CreateContext( const CORE::CValueList& params )
{GUCEF_TRACE;
    if ( m_driver )
    {
        #ifdef GUCEF_MSWIN_BUILD
        
        CORE::CValueList extraparams( params );
        CORE::CString hinststr;
        hinststr.SetInt( m_hinstance );
        extraparams.Set( "HINSTANCE", hinststr );
        
        CInputContext* context = m_driver->CreateContext( extraparams );
        
        #else
        
        CInputContext* context = m_driver->CreateContext( params );
        
        #endif
        
        if ( context )
        {
            GUCEF_SYSTEM_LOG( 0, "Created input context" );
            
            context->SetID( m_contextlist.AddEntry( context ) );
            return context;
        }
    }
    
    GUCEF_ERROR_LOG( 0, "Failed to created input context" );
    
    return NULL;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputController::DestroyContext( CInputContext* context )
{GUCEF_TRACE;
        if ( m_driver )
        {
                m_contextlist.RemoveEntry( context->GetID() );
                m_driver->DeleteContext( context );
                
                GUCEF_SYSTEM_LOG( 0, "Destroyed input context" );
        }
        
        GUCEF_ERROR_LOG( 0, "Attempting to destroy an input context without an input driver" );
        assert( 0 );
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CInputController::GetContextCount( void ) const
{GUCEF_TRACE;
        return m_contextlist.GetCount();
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputController::SetDriver( CInputDriver* driver )
{GUCEF_TRACE;
        if ( 0 == m_contextlist.GetCount() )
        {
                UnloadDriverModule();
                
                m_driver = driver;
                m_driverisplugin = false;
                
                GUCEF_SYSTEM_LOG( 0, "Input driver has been set" );
                
                NotifyObservers( InputDriverLoadedEvent );
                return true;
        }
        
        GUCEF_ERROR_LOG( 0, "Failed to set input driver because there are input context objects remaining" );
        return false;
}
        
/*-------------------------------------------------------------------------*/

const CInputDriver* 
CInputController::GetDriver( void ) const
{GUCEF_TRACE;
        return m_driver;
}
                
/*-------------------------------------------------------------------------*/

bool 
CInputController::LoadDriverModule( const CORE::CString& filename  ,
                                    const CORE::CValueList& params )
{GUCEF_TRACE;
        CORE::CString path = CORE::RelativePath( filename );
        
        GUCEF_SYSTEM_LOG( 0, "Attempting to load an input driver from module: \"" + filename + "\"" );
        
        CInputDriverPlugin* plugin = new CInputDriverPlugin();
        if ( plugin->LoadModule( path, params ) )
        {
                GUCEF_SYSTEM_LOG( 0, "Successfully loaded an input driver from module: \"" + filename + "\"" );
                
                if ( SetDriver( plugin ) )
                {
                        m_driverisplugin = true;
                        NotifyObservers( InputDriverLoadedEvent );
                        return true;
                }
        }
        delete plugin;
        return false;
}
        
/*-------------------------------------------------------------------------*/

void 
CInputController::UnloadDriverModule( void )
{GUCEF_TRACE;
        if ( m_driverisplugin && m_driver )
        {
                CInputDriverPlugin* plugin = static_cast<CInputDriverPlugin*>( m_driver );
                plugin->UnloadModule();
                
                m_driver = NULL;
                delete plugin;
                
                GUCEF_SYSTEM_LOG( 0, "Input driver has unloaded" );
                
                NotifyObservers( InputDriverUnloadedEvent );
        }
}

/*-------------------------------------------------------------------------*/

void 
CInputController::OnUpdate( const UInt64 tickcount               ,
                            const Float64 updateDeltaInMilliSecs )
{GUCEF_TRACE;
        for ( UInt32 i=0; i<m_contextlist.GetCount(); ++i )
        {
                #ifdef DEBUG_MODE

                if ( !m_driver->OnUpdate( tickcount                                         ,
                                          updateDeltaInMilliSecs                            ,
                                          static_cast<CInputContext*>( m_contextlist[ i ] ) ) )
                {
                    GUCEF_ERROR_LOG( 0, "Failed to perform an update cycle on the input driver" );
                }                                          
                
                #else
                
                m_driver->OnUpdate( tickcount                                         ,
                                    updateDeltaInMilliSecs                            ,
                                    static_cast<CInputContext*>( m_contextlist[ i ] ) );

                #endif                                    
        }                                                                
}

/*-------------------------------------------------------------------------*/

void
CInputController::OnNotify( CORE::CNotifier* notifier                 ,
                            const CORE::CEvent& eventid               ,
                            CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;
        #ifdef GUCEF_MSWIN_BUILD
        if ( eventid == CORE::CGUCEFApplication::AppInitEvent )
        {
                CORE::CGUCEFApplication::TAppInitEventData* initData = static_cast< CORE::CGUCEFApplication::TAppInitEventData* >( eventdata );
                #pragma warning( disable: 4311 ) // pointer truncation warning
                m_hinstance = reinterpret_cast<UInt32>( initData->GetData().hinstance );
        }
        #endif /* GUCEF_MSWIN_BUILD ? */
}

/*-------------------------------------------------------------------------*/

void
CInputController::RegisterEvents( void )
{GUCEF_TRACE;
    
    InputDriverLoadedEvent.Initialize();
    InputDriverUnloadedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void
CInputController::SetMouseButtonState( const UInt32 deviceIndex ,
                                       const UInt32 buttonIndex , 
                                       const bool pressedState  )
{GUCEF_TRACE;

    if ( m_mouseVector.size() > deviceIndex )
    {
        m_mouseVector[ deviceIndex ]->SetButtonState( buttonIndex  ,
                                                      pressedState );
    }
}

/*-------------------------------------------------------------------------*/

void
CInputController::SetMousePos( const UInt32 deviceIndex ,
                               const UInt32 xPos        ,
                               const UInt32 yPos        ,
                               const Int32 xDelta       ,
                               const Int32 yDelta       )
{GUCEF_TRACE;

    if ( m_mouseVector.size() > deviceIndex )
    {
        m_mouseVector[ deviceIndex ]->SetMousePos( xPos  ,
                                                   yPos );
    }
}

/*-------------------------------------------------------------------------*/
                         
void
CInputController::ResetMouseStates( const UInt32 deviceIndex )
{GUCEF_TRACE;

    if ( m_mouseVector.size() > deviceIndex )
    {
        m_mouseVector[ deviceIndex ]->ResetMouseStates();
    }
}

/*-------------------------------------------------------------------------*/

void
CInputController::SetKeyboardKeyState( const KeyCode keyCode ,
                                       const bool keyPressed )
{GUCEF_TRACE;

    m_keyboard->SetKeyState( keyCode    ,
                             keyPressed );
}

/*-------------------------------------------------------------------------*/

CKeyboard&
CInputController::GetKeyboard( void ) const
{GUCEF_TRACE;
    
    return *m_keyboard;
}

/*-------------------------------------------------------------------------*/
    
CMouse&
CInputController::GetMouse( const UInt32 index /* = 0 */ ) const
{GUCEF_TRACE;

    if ( m_mouseVector.size() > index )
    {
        return *(m_mouseVector[ index ]);
    }
    GUCEF_EMSGTHROW( EInvalidIndex, "CInputController::GetMouse(): Invalid device index given" );
}

/*-------------------------------------------------------------------------*/

UInt32
CInputController::GetMouseCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_mouseVector.size();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
