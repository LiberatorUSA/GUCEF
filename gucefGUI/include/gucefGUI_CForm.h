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

#ifndef GUCEF_GUI_CFORM_H
#define GUCEF_GUI_CFORM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_GUI_CWIDGET_H
#include "gucefGUI_CWidget.h"
#define GUCEF_GUI_CWIDGET_H
#endif /* GUCEF_GUI_CWIDGET_H ? */

#ifndef GUCEF_GUI_CFORMBACKEND_H
#include "gucefGUI_CFormBackend.h"
#define GUCEF_GUI_CFORMBACKEND_H
#endif /* GUCEF_GUI_CFORMBACKEND_H ? */

#ifndef GUCEF_GUI_CIGUICONTEXT_H
#include "gucefGUI_CIGUIContext.h"
#define GUCEF_GUI_CIGUICONTEXT_H
#endif /* GUCEF_GUI_CIGUICONTEXT_H ? */

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

class GUCEF_GUI_PUBLIC_CPP CForm : public CORE::CObservingNotifier
{
    public:    
    
    static const CORE::CEvent LayoutLoadedEvent;
    static const CORE::CEvent LayoutSavedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    CForm( void );
    
    CForm( CIGUIContext* context );
    
    virtual ~CForm();
    
    virtual bool LoadLayout( CORE::CIOAccess& layoutStorage );
    
    virtual bool SaveLayout( CORE::CIOAccess& layoutStorage );
    
    void SetContext( TGuiContextPtr& context );
    
    TGuiContextPtr GetContext( void );
    
    virtual bool SetParent( CForm* parentForm );
    
    virtual bool SetParent( CWidget* parentWidget );
    
    virtual CWidget* GetParent( void );

    virtual const CWidget* GetRootWidget( void ) const;
    
    virtual CWidget* GetRootWidget( void );
    
    virtual CWidget* GetWidget( const CString& widgetName );

    virtual bool IsVisible( void ) const;
    
    virtual bool SetVisibility( const bool isVisible );
    
    virtual bool Show( void );
    
    virtual bool ShowModal( void );
    
    virtual bool Hide( void );

    virtual const CString& GetClassTypeName( void ) const;
    
    virtual const CString& GetType( void ) const;
    
    protected:
    
    virtual void OnPreLayoutLoad( void );
    
    virtual void OnPostLayoutLoad( void );
    
    private:
    
    CForm( const CForm& src );
    CForm& operator=( const CForm& src );
    
    private:
    
    CFormBackend* m_backend;
    CWidget* m_parentWidget;
    TGuiContextPtr m_context;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUI_CFORM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/