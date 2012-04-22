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

#ifndef GUCEF_INPUT_OISJOYSTICKLISTENER_H
#define GUCEF_INPUT_OISJOYSTICKLISTENER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef OIS_Joystick_H
#include "OISJoyStick.h"
#define OIS_Joystick_H
#endif /* OIS_Joystick_H ? */

#ifndef GUCEF_INPUT_INPUTDRIVERPLUGINSTRUCTS_H
#include "inputdriverpluginstructs.h"           /* plugin API structures */
#define GUCEF_INPUT_INPUTDRIVERPLUGINSTRUCTS_H
#endif /* GUCEF_INPUT_INPUTDRIVERPLUGINSTRUCTS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_INPUT_PLUGIN_PRIVATE_CPP CJoyStickListener : public OIS::JoyStickListener
{
    public:

    CJoyStickListener( const TInputCallbacks& callbacks );

    virtual ~CJoyStickListener();

    virtual bool buttonPressed( const OIS::JoyStickEvent& arg, int button );

    virtual bool buttonReleased( const OIS::JoyStickEvent& arg, int button );

    virtual bool axisMoved( const OIS::JoyStickEvent& arg, int axis );

    virtual bool sliderMoved( const OIS::JoyStickEvent& arg, int id );

    virtual bool povMoved( const OIS::JoyStickEvent& arg, int id );

    private:

    CJoyStickListener( void );
    CJoyStickListener( const CJoyStickListener& src );
    CJoyStickListener& operator=( const CJoyStickListener& src );

    private:

    TInputCallbacks m_callbacks;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_INPUT_OISJOYSTICKLISTENER_H ? */

/*--------------------------------------------------------------------------//
//                                                                          //
//      Info & Changes                                                      //
//                                                                          //
//--------------------------------------------------------------------------//

- 29-11-2007 :
        - Initial version

----------------------------------------------------------------------------*/
