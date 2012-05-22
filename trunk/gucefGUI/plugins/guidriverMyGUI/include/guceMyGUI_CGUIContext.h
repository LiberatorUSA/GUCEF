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

#ifndef GUCEF_MYGUI_CGUICONTEXT_H
#define GUCEF_MYGUI_CGUICONTEXT_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_CIGUICONTEXT_H
#include "gucefGUI_CIGUIContext.h"
#define GUCEF_GUI_CIGUICONTEXT_H
#endif /* GUCEF_GUI_CIGUICONTEXT_H ? */

#ifndef GUCEF_MYGUI_MACROS_H
#include "guceMyGUI_macros.h"     /* often used guceMYGUIOGRE macros */
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

class CGUIDriver;

/*-------------------------------------------------------------------------*/

/**
 *  Implementation of the GUI context for the MyGUI&Ogre combo
 */
class GUCEF_MYGUI_EXPORT_CPP CGUIContext : public GUI::CIGUIContext
{
    public:    
    
    CGUIContext( CGUIDriver& myGuiDriver );
    
    virtual ~CGUIContext();
    
    virtual GUCEF::GUI::CWidget* CreateWidget( const CString& widgetName );
    
    virtual void DestroyWidget( GUI::CWidget* widget );
    
    virtual GUCEF::GUI::CForm* CreateForm( const CString& formName );
    
    virtual void DestroyForm( GUI::CForm* form );   

    virtual TStringSet GetAvailableFormTypes( void );
    
    virtual TStringSet GetAvailableWidgetTypes( void );
    
    virtual GUCEF::GUI::CFormBackend* CreateFormBackend( void );
    
    virtual void DestroyFormBackend( GUI::CFormBackend* formBackend );
    
    virtual GUCEF::GUI::CGUIDriver* GetDriver( void );
    
    virtual TWidgetSet GetOwnedWidgets( void );
    
    virtual TFormSet GetOwnedForms( void );
    
    private:
    
    CGUIContext( const CGUIContext& src );            
    CGUIContext& operator=( const CGUIContext& src );
    
    private:

    CGUIDriver* m_driver;
    TWidgetSet m_widgetSet;
    TFormSet m_formSet;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace MYGUI */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MYGUI_CGUICONTEXT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-04-2007 :
        - Initial implementation

---------------------------------------------------------------------------*/
