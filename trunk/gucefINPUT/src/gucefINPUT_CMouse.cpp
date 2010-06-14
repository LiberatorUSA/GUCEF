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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */ 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefINPUT_CMouse.h"

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

const CORE::CEvent CMouse::MouseButtonEvent = "GUCEF::INPUT::CMouse::MouseButtonEvent";
const CORE::CEvent CMouse::MouseMovedEvent = "GUCEF::INPUT::CMouse::MouseMovedEvent";
const CORE::CEvent CMouse::MouseClickedEvent = "GUCEF::INPUT::CMouse::MouseClickedEvent"; 
const CORE::CEvent CMouse::MouseDoubleClickedEvent = "GUCEF::INPUT::CMouse::MouseDoubleClickedEvent";
const CORE::CEvent CMouse::MouseTrippleClickedEvent = "GUCEF::INPUT::CMouse::MouseTrippleClickedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CMouse::RegisterEvents( void )
{GUCEF_TRACE;

    MouseButtonEvent.Initialize();
    MouseMovedEvent.Initialize();
    MouseClickedEvent.Initialize();
    MouseDoubleClickedEvent.Initialize();
    MouseTrippleClickedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

const CString&
CMouse::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::INPUT::CMouse";
    return typeName;
}

/*-------------------------------------------------------------------------*/

bool
CMouse::IsButtonPressed( const UInt32 buttonIndex ) const
{GUCEF_TRACE;

    if ( m_buttonStates.size() > buttonIndex )
    {
        return m_buttonStates[ buttonIndex ];
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CMouse::GetPrevMousePos( UInt32& xPos ,
                         UInt32& yPos ) const
{GUCEF_TRACE;

    xPos = m_prevXPos;
    yPos = m_prevYPos;
}

/*-------------------------------------------------------------------------*/

void
CMouse::GetMousePos( UInt32& xPos ,
                     UInt32& yPos ) const
{GUCEF_TRACE;

    xPos = m_xPos;
    yPos = m_yPos;
}

/*-------------------------------------------------------------------------*/

const CMouse::TButtonStates&
CMouse::GetButtonStates( void ) const
{GUCEF_TRACE;

    return m_buttonStates;
}

/*-------------------------------------------------------------------------*/

CMouse::CMouse( const UInt32 deviceID )
    : CORE::CNotifier()      ,
      m_xPos( 0 )            ,
      m_yPos( 0 )            ,
      m_prevXPos( 0 )        ,
      m_prevYPos( 0 )        ,
      m_buttonStates()       ,
      m_deviceID( deviceID )
{GUCEF_TRACE;

    RegisterEvents();
    ResetMouseStates();
}

/*-------------------------------------------------------------------------*/
    
CMouse::~CMouse()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
void
CMouse::SetButtonState( const UInt32 buttonIndex , 
                        const bool pressedState  )
{GUCEF_TRACE;

    m_buttonStates[ buttonIndex ] = pressedState;
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Setting state of button " + CORE::Int32ToString( buttonIndex ) + " to " + CORE::BoolToString( pressedState ) + " on mouse " + CORE::Int32ToString( m_deviceID ) );
    
    CMouseButtonEventData eData( m_deviceID, buttonIndex, pressedState, m_xPos, m_yPos );
    NotifyObservers( MouseButtonEvent, &eData );
}

/*-------------------------------------------------------------------------*/

void
CMouse::SetMousePos( const UInt32 xPos ,
                     const UInt32 yPos )
{GUCEF_TRACE;

    m_prevXPos = m_xPos;
    m_prevYPos = m_yPos;

    m_xPos = xPos;
    m_yPos = yPos;
    
    CMouseMovedEventData eData( m_deviceID, m_xPos, m_yPos, m_prevXPos, m_prevYPos );
    NotifyObservers( MouseMovedEvent, &eData );
}

/*-------------------------------------------------------------------------*/                         

void
CMouse::ResetMouseStates( void )
{GUCEF_TRACE;

    UInt32 buttonCount = m_buttonStates.size() > 8 ? (UInt32) m_buttonStates.size() : 8;
    m_buttonStates.resize( 8 );
    for ( UInt32 i=0; i<buttonCount; ++i )
    {
        m_buttonStates[ i ] = false;
    }
    
    m_xPos = 0;
    m_yPos = 0;
}

/*-------------------------------------------------------------------------*/

const CString&
CMouse::GetType( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::INPUT::CMouse";
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
