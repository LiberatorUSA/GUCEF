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

#ifndef GUCEF_INPUT_CINPUTCONTROLLER_H
#include "CInputController.h"
#define GUCEF_INPUT_CINPUTCONTROLLER_H
#endif /* GUCEF_INPUT_CINPUTCONTROLLER_H ? */

#include "gucefINPUT_CInputObserverSwitch.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputObserverSwitch::CInputObserverSwitch( void )
{GUCEF_TRACE;

    CInputController::Instance()->Subscribe( &AsObserver() );
}

/*-------------------------------------------------------------------------*/
    
CInputObserverSwitch::CInputObserverSwitch( const CInputObserverSwitch& src )
{GUCEF_TRACE;

    CInputController::Instance()->Subscribe( &AsObserver() );
}

/*-------------------------------------------------------------------------*/
    
CInputObserverSwitch::~CInputObserverSwitch()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CInputObserverSwitch&
CInputObserverSwitch::operator=( const CInputObserverSwitch& src )
{GUCEF_TRACE;

    return *this;
}
    
/*-------------------------------------------------------------------------*/
    
const CString&
CInputObserverSwitch::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString className = "GUCEF::INPUT::CInputObserverSwitch";
    return className;
}
    
/*-------------------------------------------------------------------------*/
    
void
CInputObserverSwitch::OnNotify( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventid  ,
                                CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    CObserverSwitch::OnNotify( notifier  ,
                               eventid   ,
                               eventdata );
    
    if ( CInputController::MouseAttachedEvent == eventid )
    {
        // Get access to the mouse object
        CInputController* controller = CInputController::Instance();
        Int32 deviceID = (*static_cast< CORE::TCloneableInt32* >( eventdata )).GetData();
        CMouse& mouse = controller->GetMouse( deviceID );
        
        // Subscribe to all mouse events
        mouse.Subscribe( &AsObserver() ); 
    }
    else
    if ( CInputController::KeyboardAttachedEvent == eventid )
    {
        // Get access to the keyboard object
        CInputController* controller = CInputController::Instance();
        Int32 deviceID = (*static_cast< CORE::TCloneableInt32* >( eventdata )).GetData();
        CKeyboard& keyboard = controller->GetKeyboard( deviceID );
        
        // Subscribe to all keyboard events
        keyboard.Subscribe( &AsObserver() );    
    }
    else            
    if ( CInputController::JoystickAttachedEvent == eventid )
    {        /*
        // Get access to the joystick object
        CInputController* controller = CInputController::Instance();
        Int32 deviceID = (*static_cast< CORE::TCloneableInt32* >( eventdata )).GetData();
        CJoystick& joystick = controller->GetJoystick( deviceID );
        
        // Subscribe to all joystick events
        joystick.Subscribe( &AsObserver() );  */  
    }    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
