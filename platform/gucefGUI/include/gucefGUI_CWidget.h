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

#ifndef GUCEF_GUI_CWIDGET_H
#define GUCEF_GUI_CWIDGET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_GUI_CUVECTOR_H
#include "gucefGUI_CUVector.h"
#define GUCEF_GUI_CUVECTOR_H
#endif /* GUCEF_GUI_CUVECTOR_H ? */

#ifndef GUCEF_GUI_CUDIM_H
#include "gucefGUI_CUDim.h"
#define GUCEF_GUI_CUDIM_H
#endif /* GUCEF_GUI_CUDIM_H ? */

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

class GUCEF_GUI_PUBLIC_CPP CWidget : public CORE::CObservingNotifier
{
    public:
    
    static const CORE::CEvent PropertiesLoadedEvent;
    static const CORE::CEvent PropertiesSavedEvent;
    static const CORE::CEvent PropertyChangedEvent;
    
    static const CORE::CEvent MouseMoveEvent;
    static const CORE::CEvent MouseDownEvent;
    static const CORE::CEvent MouseUpEvent;
    static const CORE::CEvent MouseClickEvent;
    static const CORE::CEvent MouseDoubleClickEvent;
    static const CORE::CEvent MouseTripleClickEvent;
    static const CORE::CEvent MouseFocusReceivedEvent;
    static const CORE::CEvent MouseFocusLostEvent;
        
    static const CORE::CEvent KeyDownEvent;
    static const CORE::CEvent KeyUpEvent;
    static const CORE::CEvent KeyboardFocusReceivedEvent;
    static const CORE::CEvent KeyboardFocusLostEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    typedef std::set< CWidget* > TWidgetSet;
    
    CWidget( void );
    
    CWidget( const CString& widgetTypeName );
    
    virtual ~CWidget();
    
    virtual bool SetVisibility( const bool isVisible );
    
    virtual bool IsVisible( void ) const;
    
    virtual bool ShowModal( void );
    
    virtual bool SetEnabled( const bool isEnabled );
     
    virtual bool IsEnabled( void ) const;
    
    virtual bool SetPosition( const CUVector& newPos );
    
    virtual bool GetPosition( CUVector& pos );
    
    virtual bool SetSize( const CUVector& newSize );
    
    virtual bool GetSize( CUVector& size ) const;
    
    virtual bool SetWidth( const CUDim& newWidth );
    
    virtual bool GetWidth( CUDim& width );
    
    virtual bool SetHeight( const CUDim& newHeight );
    
    virtual bool GetHeight( CUDim& width );    
    
    virtual bool SetParentWidget( CWidget* parentWidget );
    
    virtual CWidget* GetParentWidget( void );
    
    virtual CWidget* GetChildWidget( const CString& widgetName );
    
    virtual bool GetChildWidgetSet( TWidgetSet& childSet );
    
    virtual CString GetName( void ) const;
    
    virtual bool SetPropertyValue( const CString& widgetProperty ,
                                   const CString& propertyValue  );

    virtual bool GetPropertyValue( const CString& widgetProperty ,
                                   CString& propertyValue        ) const;

    virtual bool SetProperties( const CORE::CValueList& list );

    virtual bool GetProperties( CORE::CValueList& list ) const;
    
    virtual bool HasProperty( const CString& widgetProperty ) const;
    
    virtual bool LoadProperties( CORE::CIOAccess& propertyStorage );
    
    virtual bool SaveProperties( CORE::CIOAccess& propertyStorage ) const;
    
    virtual void* GetImplementationPtr( void ) const;

    virtual void SetImplementationPtr( void* impPtr );
    
    virtual const CString& GetType( void ) const;

    virtual const CString& GetClassTypeName( void ) const;

    private:

    CWidget( const CWidget& src );
    CWidget& operator=( const CWidget& src );
    
    private:
    
    CString m_widgetTypeName;
    void* m_impPtr;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUI_CWIDGET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/
