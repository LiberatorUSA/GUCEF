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

#ifndef GUCEF_INPUT_CKEYMODSTATECHANGEDEVENTDATA_H
#include "gucefINPUT_CKeyModStateChangedEventData.h"
#define GUCEF_INPUT_CKEYMODSTATECHANGEDEVENTDATA_H
#endif /* GUCEF_INPUT_CKEYMODSTATECHANGEDEVENTDATA_H ? */

#include "gucefINPUT_CKeyboard.h"

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

const CORE::CEvent CKeyboard::KeyStateChangedEvent = "GUCEF::INPUT::CKeyboard::KeyStateChangedEvent";
const CORE::CEvent CKeyboard::KeyModStateChangedEvent = "GUCEF::INPUT::CKeyboard::KeyModStateChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CKeyboard::RegisterEvents( void )
{GUCEF_TRACE;

    KeyStateChangedEvent.Initialize();
    KeyModStateChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CKeyboard::CKeyboard( void )
    : CORE::CNotifier() ,
      m_keyState()      ,
      m_keyModStates()
{GUCEF_TRACE;
    
    RegisterEvents();
    ResetKeyboardState();
}

/*-------------------------------------------------------------------------*/
                        
CKeyboard::~CKeyboard()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CKeyboard::IsKeyDown( const KeyCode key ) const
{GUCEF_TRACE;

    TKeyStates::const_iterator i = m_keyState.find( key );
    return (*i).second;
}

/*-------------------------------------------------------------------------*/
    
bool
CKeyboard::IsKeyModifierActive( const KeyModifier keyMod ) const
{GUCEF_TRACE;

    return ( m_keyModStates & keyMod ) > 0;
}

/*-------------------------------------------------------------------------*/
    
const CKeyboard::TKeyStates&
CKeyboard::GetKeyStates( void ) const
{GUCEF_TRACE;

    return m_keyState;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CKeyboard::GetKeyModStates( void ) const
{GUCEF_TRACE;

    return m_keyModStates;
}

/*-------------------------------------------------------------------------*/

void
CKeyboard::SetKeyState( const KeyCode key       , 
                        const bool pressedState )
{GUCEF_TRACE;

    m_keyState[ key ] = pressedState;
    CKeyStateChangedEventData eventData( key, pressedState, m_keyModStates );
    NotifyObservers( KeyStateChangedEvent, &eventData );
}

/*-------------------------------------------------------------------------*/

void
CKeyboard::SetKeyModState( const KeyModifier keyMod , 
                           const bool pressedState  )
{GUCEF_TRACE;

    m_keyModStates = m_keyModStates | keyMod;
    CKeyModStateChangedEventData eventData( keyMod, pressedState );
    NotifyObservers( KeyStateChangedEvent, &eventData );
}

/*-------------------------------------------------------------------------*/
                         
void
CKeyboard::ResetKeyboardState( void )
{GUCEF_TRACE;

    for ( int i=0; i<KEYCODE_MAX; ++i )
    {
        m_keyState[ (KeyCode) i ] = false;
    }
    
    m_keyModStates = 0;
}

/*-------------------------------------------------------------------------*/

const CString&
CKeyboard::GetType( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::INPUT::CKeyboard";
    return typeName;
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
