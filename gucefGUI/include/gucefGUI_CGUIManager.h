/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_GUI_CGUIMANAGER_H
#define GUCEF_GUI_CGUIMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_CWIDGETFACTORY_H
#include "gucefGUI_CWidgetFactory.h"
#define GUCEF_GUI_CWIDGETFACTORY_H
#endif /* GUCEF_GUI_CWIDGETFACTORY_H ? */

#ifndef GUCEF_GUI_CFORMFACTORY_H
#include "gucefGUI_CFormFactory.h"
#define GUCEF_GUI_CFORMFACTORY_H
#endif /* GUCEF_GUI_CFORMFACTORY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_GUI_EXPORT_CPP CGUIManager
{
    public:
    
    static CGUIManager* Instance( void );
    
    CWidgetFactory& GetWidgetFactory( void );
    
    CFormFactory& GetFormFactory( void );
    
    void SetFormBackendFactory( CFormBackendFactory* backendFactory );

    CFormBackendFactory* GetFormBackendFactory( void );
    
    private:
    friend class CGUCEFGUIModule;    
    
    static void Deinstance( void );
    
    private:
    
    CGUIManager( void );
    CGUIManager( const CGUIManager& src );
    ~CGUIManager();
    CGUIManager& operator=( const CGUIManager& src );
    
    private:
    
    CWidgetFactory m_widgetFactory;
    CFormFactory m_formFactory;
    CFormBackendFactory* m_formBackendFactory;
    
    static CGUIManager* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUI_CGUIMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/