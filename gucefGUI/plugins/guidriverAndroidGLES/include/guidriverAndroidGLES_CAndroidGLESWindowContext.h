/*
 *  guidriverAndroidGLES: module implementing GLES based window management for Android
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

#ifndef GUIDRIVERANDROIDGLES_CANDROIDGLESWINDOWCONTEXT_H
#define GUIDRIVERANDROIDGLES_CANDROIDGLESWINDOWCONTEXT_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <EGL/egl.h>
#include <GLES/gl.h>

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERANDROIDGLES {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_GUI_PUBLIC_CPP CAndroidGLESWindowContext : public CWindowContext
{        
    public:
    
    CAndroidGLESWindowContext( void );
    
    virtual ~CAndroidGLESWindowContext();
    
    virtual void SetGuiContext( TGuiContextPtr& context );
    
    virtual TGuiContextPtr GetGuiContext( void );
    
    virtual GUI::UInt32 GetID( void ) const;
    
    virtual bool IsActive( void ) const;
    
    virtual GUI::CString GetName( void ) const;

    bool Initialize( const GUI::CVideoSettings& videoSettings );
    
    private:
    
    CAndroidGLESWindowContext( const CAndroidGLESWindowContext& src );            /**< private because: must be unique */ 
    CAndroidGLESWindowContext& operator=( const CAndroidGLESWindowContext& src ); /**< private because: must be unique */

    private:

    TGuiContextPtr m_guiContext;
    GUI::UInt32 m_id;
    GUI::CString m_name;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CAndroidGLESWindowContext >  TAndroidGLESWindowContextPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUIDRIVERANDROIDGLES */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUIDRIVERANDROIDGLES_CANDROIDGLESWINDOWCONTEXT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2005 :
        - Initial version

---------------------------------------------------------------------------*/
