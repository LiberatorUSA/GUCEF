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

#ifndef GUCEF_GUIDRIVERROCKET_CGUICONTEXT_H
#define GUCEF_GUIDRIVERROCKET_CGUICONTEXT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_CIGUICONTEXT_H
#include "gucefGUI_CIGUIContext.h"
#define GUCEF_GUI_CIGUICONTEXT_H
#endif /* GUCEF_GUI_CIGUICONTEXT_H ? */

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


class GUCEF_GUIDRIVERROCKET_PUBLIC_CPP CGUIContext : public GUI::CIGUIContext
{    
    public:

    CGUIContext( GUI::CGUIDriver* guiDriver );

    virtual ~CGUIContext();
    
    virtual GUI::CWidget* CreateWidget( const GUI::CString& widgetName );
    
    virtual void DestroyWidget( GUI::CWidget* widget );
    
    virtual GUI::CForm* CreateForm( const GUI::CString& formName );
    
    virtual void DestroyForm( GUI::CForm* form );   

    virtual TStringSet GetAvailableFormTypes( void );
    
    virtual TStringSet GetAvailableWidgetTypes( void );
    
    virtual GUI::CFormBackend* CreateFormBackend( void );
    
    virtual void DestroyFormBackend( GUI::CFormBackend* formBackend );
    
    virtual GUI::CGUIDriver* GetDriver( void );
    
    virtual TWidgetSet GetOwnedWidgets( void );
    
    virtual TFormSet GetOwnedForms( void );
    
    protected:
    
    CGUIContext( void );
    
    CGUIContext( const CGUIContext& src );    

    private:

    GUI::CGUIDriver* m_guiDriver;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERROCKET */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUIDRIVERROCKET_CGUICONTEXT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
