/*
 *  guidriverRocket: GUI backend using Rocket
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_GUIDRIVERROCKET_CINPUTADAPTER_H
#define GUCEF_GUIDRIVERROCKET_CINPUTADAPTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <Rocket/Core.h>

#ifndef GUCEF_INPUT_CINPUTCONTROLLER_H
#include "CInputController.h"
#define GUCEF_INPUT_CINPUTCONTROLLER_H
#endif /* GUCEF_INPUT_CINPUTCONTROLLER_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_MACROS_H
#include "guidriverRocket_macros.h"
#define GUCEF_GUIDRIVERROCKET_MACROS_H
#endif /* GUCEF_GUIDRIVERROCKET_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERROCKET {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class which routes input events to Rocket
 */
class GUCEF_GUIDRIVERROCKET_PUBLIC_CPP CInputAdapter : public CORE::CObserver
{
    public:

	CInputAdapter();

	virtual ~CInputAdapter();

    void SetRocketContext( Rocket::Core::Context* rocketContext );

    Rocket::Core::Context* GetRocketContext( void );

    void StartListningForInputEvents( void );

    void StopListningForInputEvents( void );

    void SetInputContext( INPUT::CInputContext* inputContext );

    INPUT::CInputContext* GetInputContext( void ) const;

    protected:

    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );

    private:

    void OnMouseEvent( INPUT::CMouse* mouse         ,
                       const CORE::CEvent& eventid  ,
                       CORE::CICloneable* eventdata );

    void OnKeyboardEvent( INPUT::CKeyboard* mouse      ,
                          const CORE::CEvent& eventid  ,
                          CORE::CICloneable* eventdata );

    static int MapKeyModifierState( const INPUT::UInt32 keyModifierState );

    static Rocket::Core::Input::KeyIdentifier MapKeyIdentifier( const INPUT::KeyCode keyCode );
                           
    private:

    INPUT::CInputContext* m_inputContext;
    Rocket::Core::Context* m_rocketContext;
    int m_lastKeyModifierState;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERROCKET */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUIDRIVERROCKET_CINPUTADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
