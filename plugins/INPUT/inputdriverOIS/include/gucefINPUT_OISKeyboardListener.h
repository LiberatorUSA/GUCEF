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

#ifndef GUCEF_INPUT_OISKEYBOARDLISTENER_H
#define GUCEF_INPUT_OISKEYBOARDLISTENER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef OIS_Keyboard_H
#include "OISKeyboard.h"
#define OIS_Keyboard_H
#endif /* OIS_Keyboard_H ? */

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

class GUCEF_INPUT_PLUGIN_PRIVATE_CPP CKeyboardListener : public OIS::KeyListener
{
    public:

    CKeyboardListener( const TInputCallbacks& callbacks );

    virtual ~CKeyboardListener();

    virtual bool keyPressed( const OIS::KeyEvent &arg );

    virtual bool keyReleased( const OIS::KeyEvent &arg );

    private:

    CKeyboardListener( void );
    CKeyboardListener( const CKeyboardListener& src );
    CKeyboardListener& operator=( const CKeyboardListener& src );

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

#endif /* GUCEF_INPUT_OISKEYBOARDLISTENER_H ? */

/*--------------------------------------------------------------------------//
//                                                                          //
//      Info & Changes                                                      //
//                                                                          //
//--------------------------------------------------------------------------//

- 29-11-2007 :
        - Initial version

----------------------------------------------------------------------------*/
