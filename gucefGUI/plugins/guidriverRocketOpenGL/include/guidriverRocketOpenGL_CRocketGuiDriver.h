/*
 *  guidriverRocketOpenGL: module providing a GUI backend using Rocket and OpenGL
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

#ifndef GUCEF_GUIDRIVERROCKETGL_CROCKETGUIDRIVER_H
#define GUCEF_GUIDRIVERROCKETGL_CROCKETGUIDRIVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_GUI_CGUIDRIVER_H
#include "gucefGUI_CGUIDriver.h"
#define GUCEF_GUI_CGUIDRIVER_H
#endif /* GUCEF_GUI_CGUIDRIVER_H ? */

#ifndef GUCEF_GUIDRIVERROCKETGL_CROCKETRENDERINTERFACEOPENGL_H
#include "guidriverRocketOpenGL_CRocketRenderInterfaceOpenGL.h"
#define GUCEF_GUIDRIVERROCKETGL_CROCKETRENDERINTERFACEOPENGL_H
#endif /* GUCEF_GUIDRIVERROCKETGL_CROCKETRENDERINTERFACEOPENGL_H ? */

#ifndef GUCEF_GUIDRIVERROCKETGL_MACROS_H
#include "guidriverRocketOpenGL_macros.h"
#define GUCEF_GUIDRIVERROCKETGL_MACROS_H
#endif /* GUCEF_GUIDRIVERROCKETGL_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERROCKETGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUIDRIVERROCKETGL_PUBLIC_CPP CRocketGuiDriver : public GUI::CGUIDriver
{    
    public:

    CRocketGuiDriver( void );

    virtual ~CRocketGuiDriver();
    
    virtual GUI::TGuiContextPtr CreateGUIContext();
    
    virtual TGUIContextSet GetContextList( void );
    
    virtual GUI::UInt32 GetContextCount( void );
    
    virtual GUI::CString GetDriverName( void );
    
    virtual TStringSet GetAvailableFormTypes( void );
    
    virtual TStringSet GetAvailableWidgetTypes( void );
    
    virtual const GUI::CString& GetClassTypeName( void ) const;
    
    virtual GUI::CString GetDriverProperty( const GUI::CString& propertyName ) const;
    
    private:
    
    CRocketGuiDriver( const CRocketGuiDriver& src );

    private:

    CRocketRenderInterfaceOpenGL m_rocketRenderer;
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERROCKETGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUIDRIVERROCKETGL_CROCKETGUIDRIVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2010 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/