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

#ifndef GUCEF_GUI_CFORMBACKEND_H
#define GUCEF_GUI_CFORMBACKEND_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_CTFACTORYBASE_H
#include "CTFactoryBase.h"
#define GUCEF_CORE_CTFACTORYBASE_H
#endif /* GUCEF_CORE_CTFACTORYBASE_H ? */

#ifndef GUCEF_GUI_CWIDGET_H
#include "gucefGUI_CWidget.h"
#define GUCEF_GUI_CWIDGET_H
#endif /* GUCEF_GUI_CWIDGET_H ? */

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

/**
 *  Abstract base class used by the form class to abstract from the backend
 *  This allows you to descend from the CForm class as you wish without having
 *  to create a matching backend implementation for every form type.
 */
class GUCEF_GUI_EXPORT_CPP CFormBackend
{
    public:
    
    typedef std::vector< CWidget* > TWidgetVector;
    
    CFormBackend( void );
    
    virtual ~CFormBackend();

    virtual bool LoadLayout( CORE::CIOAccess& layoutStorage ) = 0;
    
    virtual bool SaveLayout( CORE::CIOAccess& layoutStorage ) = 0;
    
    virtual CWidget* GetRootWidget( void ) = 0;
    
    virtual const CWidget* GetRootWidget( void ) const = 0;
    
    virtual CWidget* GetWidget( const CString& widgetName ) = 0;
    
    virtual void GetWidgetVector( const CString& widgetName   ,
                                  TWidgetVector& widgetVector ) = 0;
                                  
    private:
    
    CFormBackend( const CFormBackend& src );
    CFormBackend& operator=( const CFormBackend& other );
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTFactoryBase< CFormBackend > CFormBackendFactory;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUI_CFORMBACKEND_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/
