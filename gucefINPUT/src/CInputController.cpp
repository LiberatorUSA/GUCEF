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
const CORE::CEvent CInputController::MouseAttachedEvent = "GUCEF::INPUT::MouseAttachedEvent";
const CORE::CEvent CInputController::MouseDetachedEvent = "GUCEF::INPUT::MouseDetachedEvent";
const CORE::CEvent CInputController::KeyboardAttachedEvent = "GUCEF::INPUT::KeyboardAttachedEvent";
const CORE::CEvent CInputController::KeyboardDetachedEvent = "GUCEF::INPUT::KeyboardDetachedEvent";
const CORE::CEvent CInputController::JoystickAttachedEvent = "GUCEF::INPUT::JoystickAttachedEvent";
const CORE::CEvent CInputController::JoystickDetachedEvent = "GUCEF::INPUT::JoystickDetachedEvent";
CInputController* CInputController::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_IMPLEMENT_MSGEXCEPTION( CInputController, EInvalidIndex );

/*-------------------------------------------------------------------------*/

CInputController::CInputController( void )
        : CGUCEFAppSubSystem( true ) ,
          m_driverisplugin( false )  ,
          m_driver( NULL )           ,
          m_keyboardMap()            ,
          m_mouseMap()
          
          #ifdef GUCEF_MSWIN_BUILD
          ,
          m_hinstance(0UL)
          #endif
{GUCEF_TRACE;

    RegisterEvents();
    SetPeriodicUpdateRequirement( true );
    RequestUpdateInterval( 5 );
}

/*-------------------------------------------------------------------------*/

CInputController::~CInputController()
{GUCEF_TRACE;

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
            
            m_contextSet.insert( context );
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
        m_contextSet.erase( context );
        m_driver->DeleteContext( context );
        
        GUCEF_SYSTEM_LOG( 0, "Destroyed input context" );
        return;
    }
    
    GUCEF_ERROR_LOG( 0, "Attempting to destroy an input context without an input driver" );
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CInputController::GetContextCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_contextSet.size();
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputController::SetDriver( CInputDriver* driver )
{GUCEF_TRACE;

    if ( 0 == GetContextCount() )
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
        
        GUCEF_ERROR_LOG( 0, "Failed to load an input driver from module: \"" + filename + "\"" );
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

    TContextSet::iterator i = m_contextSet.begin();
    while( i != m_contextSet.end() )
    {
        #ifdef GUCEF_INPUT_DEBUG_MODE

        if ( !m_driver->OnUpdate( tickcount              ,
                                  updateDeltaInMilliSecs ,
                                  (*i)                   ) )
        {
            GUCEF_ERROR_LOG( 0, "Failed to perform an update cycle on the input driver" );
        }                                          
        
        #else
        
        m_driver->OnUpdate( tickcount              ,
                            updateDeltaInMilliSecs ,
                            (*i)                   );

        #endif
        
        ++i;                                    
    }                                                                
}

/*-------------------------------------------------------------------------*/

void
CInputController::OnNotify( CORE::CNotifier* notifier                 ,
                            const CORE::CEvent& eventid               ,
                            CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;
        
    if ( eventid == CORE::CGUCEFApplication::AppInitEvent )
    {
        #ifdef GUCEF_MSWIN_BUILD
        CORE::CGUCEFApplication::TAppInitEventData* initData = static_cast< CORE::CGUCEFApplication::TAppInitEventData* >( eventdata );
        #pragma warning( disable: 4311 ) // pointer truncation warning
        m_hinstance = reinterpret_cast<UInt32>( initData->GetData().hinstance );
        #endif /* GUCEF_MSWIN_BUILD ? */
        
        RequestUpdate();
    }
}

/*-------------------------------------------------------------------------*/

void
CInputController::RegisterEvents( void )
{GUCEF_TRACE;
    
    InputDriverLoadedEvent.Initialize();
    InputDriverUnloadedEvent.Initialize();
    MouseAttachedEvent.Initialize();
    MouseDetachedEvent.Initialize();
    KeyboardAttachedEvent.Initialize();
    KeyboardDetachedEvent.Initialize();
    JoystickAttachedEvent.Initialize();
    JoystickDetachedEvent.Initialize();    
}

/*-------------------------------------------------------------------------*/

void
CInputController::SetMouseButtonState( const Int32 deviceID     ,
                                       const UInt32 buttonIndex , 
                                       const bool pressedState  )
{GUCEF_TRACE;

    TMouseMap::iterator i = m_mouseMap.find( deviceID );
    if ( i != m_mouseMap.end() )
    {
        (*i).second->SetButtonState( buttonIndex  ,
                                     pressedState );
    }
    else
    {
        GUCEF_ERROR_LOG( 0, "Invalid input device ID given: " + CORE::Int32ToString( deviceID ) );
    }    
}

/*-------------------------------------------------------------------------*/

void
CInputController::SetMousePos( const Int32 deviceID ,
                               const UInt32 xPos    ,
                               const UInt32 yPos    ,
                               const Int32 xDelta   ,
                               const Int32 yDelta   )
{GUCEF_TRACE;

    TMouseMap::iterator i = m_mouseMap.find( deviceID );
    if ( i != m_mouseMap.end() )
    {
        (*i).second->SetMousePos( xPos  ,
                                  yPos );
    }
    else
    {
        GUCEF_ERROR_LOG( 0, "Invalid input device ID given: " + CORE::Int32ToString( deviceID ) );
    }    
}

/*-------------------------------------------------------------------------*/
                         
void
CInputController::ResetMouseStates( const Int32 deviceID )
{GUCEF_TRACE;

    TMouseMap::iterator i = m_mouseMap.find( deviceID );
    if ( i != m_mouseMap.end() )
    {
        (*i).second->ResetMouseStates();
    }
    else
    {
        GUCEF_ERROR_LOG( 0, "Invalid input device ID given: " + CORE::Int32ToString( deviceID ) );
    }    
}

/*-------------------------------------------------------------------------*/

void
CInputController::SetKeyboardKeyState( const Int32 deviceID  ,
                                       const KeyCode keyCode ,
                                       const bool keyPressed )
{GUCEF_TRACE;

    TKeyboardMap::iterator i = m_keyboardMap.find( deviceID ); 
    if ( i != m_keyboardMap.end() )
    {
        (*i).second->SetKeyState( keyCode    ,
                                  keyPressed );
    }
    else
    {
        GUCEF_ERROR_LOG( 0, "Invalid input device ID given: " + CORE::Int32ToString( deviceID ) );
    }
}

/*-------------------------------------------------------------------------*/

CKeyboard&
CInputController::GetKeyboard( const Int32 deviceID )
{GUCEF_TRACE;
    
    TKeyboardMap::iterator i = m_keyboardMap.find( deviceID ); 
    if ( i != m_keyboardMap.end() )
    {
        return *(*i).second;
    }

    GUCEF_EMSGTHROW( EInvalidIndex, "CInputController::GetKeyboard(): Invalid device ID given" );
}

/*-------------------------------------------------------------------------*/

UInt32
CInputController::GetKeyboardCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_keyboardMap.size();
}

/*-------------------------------------------------------------------------*/
    
const CInputController::TKeyboardMap&
CInputController::GetKeyboardMap( void ) const
{GUCEF_TRACE;

    return m_keyboardMap;
}

/*-------------------------------------------------------------------------*/
    
CMouse&
CInputController::GetMouse( const Int32 deviceID )
{GUCEF_TRACE;

    TMouseMap::iterator i = m_mouseMap.find( deviceID ); 
    if ( i != m_mouseMap.end() )
    {
        return *(*i).second;
    }
    GUCEF_EMSGTHROW( EInvalidIndex, "CInputController::GetMouse(): Invalid device ID given" );
}

/*-------------------------------------------------------------------------*/

UInt32
CInputController::GetMouseCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_mouseMap.size();
}

/*-------------------------------------------------------------------------*/
    
const CInputController::TMouseMap&
CInputController::GetMouseMap( void ) const
{GUCEF_TRACE;

    return m_mouseMap;
}

/*-------------------------------------------------------------------------*/

void
CInputController::AddMouse( const Int32 deviceID )
{GUCEF_TRACE;

    TMouseMap::iterator i = m_mouseMap.find( deviceID ); 
    if ( i == m_mouseMap.end() )
    {
        CMouse* mouse = new CMouse( deviceID );
        m_mouseMap.insert( std::pair< UInt32, CMouse* >( deviceID, mouse ) );
        
        GUCEF_SYSTEM_LOG( 0, "Mouse input device added with device ID " + CORE::Int32ToString( deviceID ) );
        NotifyObservers( MouseAttachedEvent, &TMouseAttachedEventData( deviceID ) );
    }
}

/*-------------------------------------------------------------------------*/
    
void
CInputController::RemoveMouse( const Int32 deviceID )
{GUCEF_TRACE;

    TMouseMap::iterator i = m_mouseMap.find( deviceID ); 
    if ( i != m_mouseMap.end() )
    {
        CMouse* mouse = (*i).second;
        delete mouse;
        m_mouseMap.erase( i );
        
        GUCEF_SYSTEM_LOG( 0, "Mouse input device removed with device ID " + CORE::Int32ToString( deviceID ) );
        NotifyObservers( MouseDetachedEvent, &TMouseDetachedEventData( deviceID ) );
    }
}

/*-------------------------------------------------------------------------*/
    
void
CInputController::AddKeyboard( const Int32 deviceID )
{GUCEF_TRACE;

    TKeyboardMap::iterator i = m_keyboardMap.find( deviceID ); 
    if ( i == m_keyboardMap.end() )
    {
        CKeyboard* keyboard = new CKeyboard( deviceID, this );
        m_keyboardMap.insert( std::pair< UInt32, CKeyboard* >( deviceID, keyboard ) );
        
        GUCEF_SYSTEM_LOG( 0, "Keyboard input device added with device ID " + CORE::Int32ToString( deviceID ) );
        NotifyObservers( KeyboardAttachedEvent, &TKeyboardAttachedEventData( deviceID ) );
    }
}

/*-------------------------------------------------------------------------*/
    
void
CInputController::RemoveKeyboard( const Int32 deviceID )
{GUCEF_TRACE;

    TKeyboardMap::iterator i = m_keyboardMap.find( deviceID ); 
    if ( i != m_keyboardMap.end() )
    {
        CKeyboard* keyboard = (*i).second;
        delete keyboard;
        m_keyboardMap.erase( i );
        
        GUCEF_SYSTEM_LOG( 0, "Keyboard input device removed with device ID " + CORE::Int32ToString( deviceID ) );
        NotifyObservers( KeyboardDetachedEvent, &TKeyboardDetachedEventData( deviceID ) );
    }
}

/*-------------------------------------------------------------------------*/
    
void
CInputController::AddDevice( const Int32 deviceID )
{GUCEF_TRACE;

    // @TODO
}

/*-------------------------------------------------------------------------*/
    
void
CInputController::RemoveDevice( const Int32 deviceID )
{GUCEF_TRACE;

    // @TODO
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
