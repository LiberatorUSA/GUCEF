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

#ifndef GUCEF_MYGUI_CFORMBACKENDIMP_H
#define GUCEF_MYGUI_CFORMBACKENDIMP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#include <MyGUI.h>

#ifndef GUCEF_GUI_CFORMBACKEND_H
#include "gucefGUI_CFormBackend.h"
#define GUCEF_GUI_CFORMBACKEND_H
#endif /* GUCEF_GUI_CFORMBACKEND_H ? */

#ifndef GUCEF_MYGUI_CWIDGETIMP_H
#include "guceMyGUI_CWidgetImp.h"
#define GUCEF_MYGUI_CWIDGETIMP_H
#endif /* GUCEF_MYGUI_CWIDGETIMP_H ? */

#ifndef GUCEF_MYGUI_MACROS_H
#include "guceMyGUI_macros.h"     /* often used guceMYGUI macros */
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

/**
 *  Class used by the form class to abstract from the backend.
 *  This allows you to descend from the CForm class as you wish without having
 *  to create a matching backend implementation for every form type.
 */
class GUCEF_MYGUI_EXPORT_CPP CFormBackendImp : public GUCEF::GUI::CFormBackend
{
    public:

    typedef std::map< CString, GUCEF::GUI::CWidget* > TWidgetMap;

    CFormBackendImp( void );

    virtual ~CFormBackendImp();

    virtual bool LoadLayout( const GUCEF::CORE::CString& layoutStoragePath );

    virtual bool SaveLayout( const GUCEF::CORE::CString& layoutStoragePath );

    virtual bool LoadLayout( GUCEF::CORE::CIOAccess& layoutStorage );

    virtual bool SaveLayout( GUCEF::CORE::CIOAccess& layoutStorage );

    virtual GUCEF::GUI::CWidget* GetRootWidget( void );

    virtual const GUCEF::GUI::CWidget* GetRootWidget( void ) const;

    virtual GUCEF::GUI::CWidget* GetWidget( const CString& widgetName );

    virtual void GetWidgetVector( const CString& widgetName   ,
                                  TWidgetVector& widgetVector );

    private:

    CFormBackendImp( const CFormBackendImp& src );
    CFormBackendImp& operator=( const CFormBackendImp& other );

    GUCEF::GUI::CWidget*
    CreateAndHookWrapperForWindow( MyGUI::Widget* window );

    void
    WrapAndHookChildWindows( MyGUI::Widget* window );

    private:

    TWidgetMap m_widgetMap;
    GUI::CWidget* m_rootWindow;
    CString m_widgetNamePrefix;
    CString m_resourceGroupName;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MYGUI_CFORMBACKENDIMP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/
