/*
 *  guidriverXWinGL: module implementing GL based window management for X11
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

#ifndef GUIDRIVERXWINGL_CXWINGLWINDOWCONTEXT_H
#define GUIDRIVERXWINGL_CXWINGLWINDOWCONTEXT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CX11WINDOW_H
#include "gucefCORE_CX11Window.h"
#define GUCEF_CORE_CX11WINDOW_H
#endif /* GUCEF_CORE_CX11WINDOW_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_GUI_CWINDOWCONTEXT_H
#include "gucefGUI_CWindowContext.h"
#define GUCEF_GUI_CWINDOWCONTEXT_H
#endif /* GUCEF_GUI_CWINDOWCONTEXT_H ? */

#ifndef GUCEF_GUI_CVIDEOSETTINGS_H
#include "gucefGUI_CVideoSettings.h"
#define GUCEF_GUI_CVIDEOSETTINGS_H
#endif /* GUCEF_GUI_CVIDEOSETTINGS_H ? */

#ifndef GUIDRIVERXWINGL_MACROS_H
#include "guidriverXWinGL_macros.h"
#define GUIDRIVERXWINGL_MACROS_H
#endif /* GUIDRIVERXWINGL_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERXWINGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUIDRIVERXWINGL_PUBLIC_CPP CXWinGLWindowContext : public GUI::CWindowContext
{
    public:

    CXWinGLWindowContext( void );

    virtual ~CXWinGLWindowContext();

    virtual void SetGuiContext( GUI::TGuiContextPtr& context );

    virtual GUI::TGuiContextPtr GetGuiContext( void );

    virtual GUI::UInt32 GetWidth( void ) const;

    virtual GUI::UInt32 GetHeight( void ) const;

    virtual GUI::UInt32 GetID( void ) const;

    virtual bool IsActive( void ) const;

    virtual GUI::CString GetName( void ) const;

    virtual GUI::CString GetProperty( const GUI::CString& propertyName ) const;

    bool Initialize( const GUI::CString& title                ,
                     const GUI::CVideoSettings& videoSettings );

    void Shutdown( void );

    virtual const CORE::CString& GetClassTypeName( void ) const;

    protected:

    virtual void OnNotify( CORE::CNotifier* notifier          ,
                           const CORE::CEvent& eventID        ,
                           CORE::CICloneable* evenData = NULL );

    private:

    CXWinGLWindowContext( const CXWinGLWindowContext& src );            /**< private because: must be unique */
    CXWinGLWindowContext& operator=( const CXWinGLWindowContext& src ); /**< private because: must be unique */

    private:

    GUI::TGuiContextPtr m_guiContext;
    GUI::UInt32 m_id;
    GUI::CString m_name;
    CORE::CX11Window m_window;
    //HGLRC m_renderContext;
    //HDC m_deviceContext;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CXWinGLWindowContext >  TXWinGLWindowContextPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUIDRIVERXWINGL */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUIDRIVERXWINGL_CXWINGLWINDOWCONTEXT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2005 :
        - Initial version

---------------------------------------------------------------------------*/
