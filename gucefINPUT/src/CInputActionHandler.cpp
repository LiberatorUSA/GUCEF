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

#include "CInputActionHandler.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputActionHandler::CInputActionHandler( void )
{
}

/*-------------------------------------------------------------------------*/
        
CInputActionHandler::CInputActionHandler( const CInputActionHandler& src )
        : m_actionmap( src.m_actionmap )
{
}

/*-------------------------------------------------------------------------*/
        
CInputActionHandler::~CInputActionHandler()
{
}

/*-------------------------------------------------------------------------*/
        
CInputActionHandler& 
CInputActionHandler::operator=( const CInputActionHandler& src )
{
        if ( this != &src )
        {
                m_actionmap = src.m_actionmap;
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CInputActionHandler::SetActionMap( const CInputActionMap& actionmap )
{
        m_actionmap = actionmap;
}

/*-------------------------------------------------------------------------*/
        
const CInputActionMap& 
CInputActionHandler::GetActionMap( void ) const
{
        return m_actionmap;
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputActionHandler::LoadActionMapConfig( const CORE::CDataNode& treenode )
{
        return m_actionmap.LoadConfig( treenode );
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputActionHandler::SaveActionMapConfig( CORE::CDataNode& treenode )
{
        return m_actionmap.SaveConfig( treenode );
}

/*-------------------------------------------------------------------------*/
        
CInputActionMap& 
CInputActionHandler::GetMutableActionMap( void )
{
        return m_actionmap;
}

/*-------------------------------------------------------------------------*/

void 
CInputActionHandler::OnMouseButtonDown( const UInt32 buttonindex )
{
        OnMappedMouseButtonDown( m_actionmap.GetMouseButtonDownMap( buttonindex ) ,
                                 buttonindex                                      );
}

/*-------------------------------------------------------------------------*/
        
void 
CInputActionHandler::OnMouseButtonUp( const UInt32 buttonindex )
{
        OnMappedMouseButtonUp( m_actionmap.GetMouseButtonUpMap( buttonindex ) ,
                               buttonindex                                    );
}

/*-------------------------------------------------------------------------*/

void 
CInputActionHandler::OnKeyboardKeyDown( const UInt32 keyindex )
{
        OnMappedKeyboardKeyDown( m_actionmap.GetKeyboardKeyDownMap( keyindex ) ,
                                 keyindex                                      );
}

/*-------------------------------------------------------------------------*/
        
void 
CInputActionHandler::OnKeyboardKeyUp( const UInt32 keyindex )
{
        OnMappedKeyboardKeyUp( m_actionmap.GetKeyboardKeyUpMap( keyindex ) ,
                               keyindex                                    );
}
 
/*-------------------------------------------------------------------------*/ 
        
void 
CInputActionHandler::OnDeviceBooleanOn( const UInt32 deviceid   ,
                                        const UInt32 stateindex )
{
        OnMappedDeviceBooleanOn( m_actionmap.GetDeviceBooleanOnMap( deviceid   ,
                                                                    stateindex ) ,
                                 deviceid                                        ,
                                 stateindex                                      );
}

/*-------------------------------------------------------------------------*/

void 
CInputActionHandler::OnDeviceBooleanOff( const UInt32 deviceid   ,
                                         const UInt32 stateindex )
{
        OnMappedDeviceBooleanOff( m_actionmap.GetDeviceBooleanOffMap( deviceid   ,
                                                                      stateindex ) ,
                                  deviceid                                         ,
                                  stateindex                                       );
}                                         

/*-------------------------------------------------------------------------*/
                                         
void 
CInputActionHandler::OnDeviceVariableChanged( const UInt32 deviceid   ,
                                              const UInt32 stateindex ,
                                              const Float32 value     )
{
        OnMappedDeviceVariableChanged( m_actionmap.GetDeviceVariableChangedMap( deviceid   ,
                                                                                stateindex ) ,
                                       deviceid                                              ,
                                       stateindex                                            ,
                                       value                                                 );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
