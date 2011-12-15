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

#include <Rocket/Core.h>

#ifndef GUCEF_INPUT_CINPUTCONTEXT_H
#include "CInputContext.h"
#define GUCEF_INPUT_CINPUTCONTEXT_H
#endif /* GUCEF_INPUT_CINPUTCONTEXT_H ? */

#ifndef GUCEF_GUI_CWINDOWCONTEXT_H
#include "gucefGUI_CWindowContext.h"
#define GUCEF_GUI_CWINDOWCONTEXT_H
#endif /* GUCEF_GUI_CWINDOWCONTEXT_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_GUI_CIGUICONTEXT_H
#include "gucefGUI_CIGUIContext.h"
#define GUCEF_GUI_CIGUICONTEXT_H
#endif /* GUCEF_GUI_CIGUICONTEXT_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CINPUTADAPTER_H
#include "guidriverRocket_CInputAdapter.h"
#define GUCEF_GUIDRIVERROCKET_CINPUTADAPTER_H
#endif /* GUCEF_GUIDRIVERROCKET_CINPUTADAPTER_H ? */

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


class GUCEF_GUIDRIVERROCKET_PUBLIC_CPP CGUIContext : public CORE::CObserver   ,
                                                     public GUI::CIGUIContext
{    
    public:

    CGUIContext( GUI::CGUIDriver* guiDriver           ,
                 Rocket::Core::Context* context       ,
                 GUI::TWindowContextPtr windowContext ,
                 INPUT::CInputContext* inputContext   );

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

    virtual const CORE::CString& GetClassTypeName( void ) const;

    Rocket::Core::Context* GetRocketContext( void );

    GUI::TWindowContextPtr GetWindowContext( void );

    INPUT::CInputContext* GetInputContext( void );
    
    protected:
   
    virtual void OnNotify( CORE::CNotifier* notifier          ,
                           const CORE::CEvent& eventID        ,
                           CORE::CICloneable* evenData = NULL );

    private:

    CGUIContext( void );                    /**< not implemented */
    CGUIContext( const CGUIContext& src );  /**< not implemented */

    private:

    GUI::CGUIDriver* m_guiDriver;
    Rocket::Core::Context* m_rocketContext;
    GUI::TWindowContextPtr m_windowContext;
    INPUT::CInputContext* m_inputContext;
    CInputAdapter m_inputAdapter;
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
