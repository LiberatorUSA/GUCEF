/*
 *  guidriverMyGUI: glue module for the MyGUI GUI backend
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_MYGUI_CMYGUIINPUTADAPTER_H
#define GUCEF_MYGUI_CMYGUIINPUTADAPTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <MyGUI.h>

#ifndef GUCEF_CORE_COBSERVER_H
#include "CObserver.h"
#define GUCEF_CORE_COBSERVER_H
#endif /* GUCEF_CORE_COBSERVER_H ? */

#ifndef GUCEF_INPUT_CINPUTCONTROLLER_H
#include "CInputController.h"
#define GUCEF_INPUT_CINPUTCONTROLLER_H
#endif /* GUCEF_INPUT_CINPUTCONTROLLER_H ? */

#ifndef GUCEF_MYGUI_MACROS_H
#include "guceMyGUI_macros.h"      /* often used macros */
#define GUCEF_MYGUI_MACROS_H
#endif /* GUCEF_MYGUI_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MYGUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_MYGUI_EXPORT_CPP CMyGUIInputAdapter : public GUCEF::CORE::CObserver
{
    public:

    CMyGUIInputAdapter( void );

    virtual ~CMyGUIInputAdapter();

    const CString& GetClassTypeName( void ) const;

    void StartListningForInputEvents( void );

    void StopListningForInputEvents( void );

    void SetInputContext( INPUT::CInputContext* inputContext );

    INPUT::CInputContext* GetInputContext( void ) const;

    protected:

    virtual void OnNotify( GUCEF::CORE::CNotifier* notifier                 ,
                           const GUCEF::CORE::CEvent& eventid               ,
                           GUCEF::CORE::CICloneable* eventdata /* = NULL */ );

    private:

    CMyGUIInputAdapter( const CMyGUIInputAdapter& src );
    CMyGUIInputAdapter& operator=( const CMyGUIInputAdapter& src );

    MyGUI::MouseButton ConvertMouseButtonIndex( const UInt32 buttonIndex );

    private:

    INPUT::CInputContext* m_inputContext;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIOGRE */
}; /* namespace GUCE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MYGUI_CMYGUIINPUTADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/
