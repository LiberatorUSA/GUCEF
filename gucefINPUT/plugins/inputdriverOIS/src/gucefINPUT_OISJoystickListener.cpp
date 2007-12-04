/*
 *  inputdriverOIS: GUCEF plugin module proving an input driver backend
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#include "gucefINPUT_OISJoystickListener.h"
 
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

CJoyStickListener::CJoyStickListener( const TInputCallbacks& callbacks )
    : OIS::JoyStickListener()  ,
      m_callbacks( callbacks )
{

}

/*-------------------------------------------------------------------------*/
    
CJoyStickListener::~CJoyStickListener()
{

}

/*-------------------------------------------------------------------------*/

bool
CJoyStickListener::buttonPressed( const OIS::JoyStickEvent& arg , 
                                  int button                    )
{
    m_callbacks.onDeviceBooleanOn( m_callbacks.userData ,
                                   0                    ,
                                   button               );
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
CJoyStickListener::buttonReleased( const OIS::JoyStickEvent& arg , 
                                   int button                    )
{
    m_callbacks.onDeviceBooleanOff( m_callbacks.userData ,
                                    arg.device->getID()  ,
                                    button               );
    return true;
}
    
/*-------------------------------------------------------------------------*/

bool
CJoyStickListener::axisMoved( const OIS::JoyStickEvent& arg , 
                              int axis                      )
{
    m_callbacks.onDeviceVarChanged( m_callbacks.userData        ,
                                    arg.device->getID()         ,
                                    axis                        ,
                                    arg.state.mAxes[ axis ].abs );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CJoyStickListener::sliderMoved( const OIS::JoyStickEvent& arg , 
                                int id                        )
{
    m_callbacks.onDeviceVarChanged( m_callbacks.userData         ,
                                    arg.device->getID()          ,
                                    arg.state.mAxes.size() + id  ,
                                    arg.state.mSliders[ id ].abX );
    return true;
}
    
/*-------------------------------------------------------------------------*/

bool
CJoyStickListener::povMoved( const OIS::JoyStickEvent& arg , 
                             int id                        )
{
    m_callbacks.onDeviceVarChanged( m_callbacks.userData            ,
                                    arg.device->getID()             ,
                                    arg.state.mAxes.size() + 4 + id ,
                                    arg.state.mPOV[ id ].direction  );
    return true;
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
