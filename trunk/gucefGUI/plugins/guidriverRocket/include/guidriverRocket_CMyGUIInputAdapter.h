/*
 *  guceMyGUIOgre: glue module for the MyGUI+Ogre GUI backend
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

#ifndef GUCE_MYGUIOGRE_CMYGUIINPUTADAPTER_H
#define GUCE_MYGUIOGRE_CMYGUIINPUTADAPTER_H

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

#ifndef GUCE_MYGUIOGRE_MACROS_H
#include "guceMyGUIOgre_macros.h"      /* often used macros */
#define GUCE_MYGUIOGRE_MACROS_H
#endif /* GUCE_MYGUIOGRE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCE {
namespace MYGUIOGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCE_MYGUIOGRE_EXPORT_CPP CMyGUIInputAdapter : public GUCEF::CORE::CObserver
{
    public:
    
    CMyGUIInputAdapter( MyGUI::Gui* guiSystem );

    virtual ~CMyGUIInputAdapter();
    
    const CString& GetClassTypeName( void ) const;

    protected:
    
    virtual void OnNotify( GUCEF::CORE::CNotifier* notifier                 ,
                           const GUCEF::CORE::CEvent& eventid               ,
                           GUCEF::CORE::CICloneable* eventdata /* = NULL */ );
    
    private:
    
    CMyGUIInputAdapter( void );
    CMyGUIInputAdapter( const CMyGUIInputAdapter& src );    
    CMyGUIInputAdapter& operator=( const CMyGUIInputAdapter& src );

    MyGUI::MouseButton ConvertMouseButtonIdex( const UInt32 buttonIndex );
    
    private:
    
    MyGUI::Gui* m_guiSystem;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIOGRE */
}; /* namespace GUCE */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCE_MYGUIOGRE_CMYGUIINPUTADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/