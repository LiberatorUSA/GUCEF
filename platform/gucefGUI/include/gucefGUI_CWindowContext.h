/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
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

#ifndef GUCEF_GUI_CWINDOWCONTEXT_H
#define GUCEF_GUI_CWINDOWCONTEXT_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

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

class GUCEF_GUI_PUBLIC_CPP CWindowContext : public CORE::CObservingNotifier
{        
    public:
    
    static const GUCEF::CORE::CEvent WindowContextRedrawEvent;
    static const GUCEF::CORE::CEvent WindowContextSizeEvent;
    static const GUCEF::CORE::CEvent WindowContextActivateEvent;

    typedef GUCEF::CORE::TCloneableBool WindowContextActivateEventData;
    
    static void RegisterEvents( void );
    
    public:
    
    CWindowContext( void );

    virtual ~CWindowContext();

    virtual UInt32 GetWidth( void ) const = 0;

    virtual UInt32 GetHeight( void ) const = 0;
    
    virtual UInt32 GetID( void ) const = 0;
    
    virtual bool IsActive( void ) const = 0;
    
    virtual CString GetName( void ) const = 0;

    virtual void SetGuiContext( TGuiContextPtr& context ) = 0;
    
    virtual TGuiContextPtr GetGuiContext( void ) = 0;

    virtual CString GetProperty( const CString& propertyName ) const = 0;
    
    private:
    
    CWindowContext( const CWindowContext& src );            /**< private because: must be unique */ 
    CWindowContext& operator=( const CWindowContext& src ); /**< private because: must be unique */
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CWindowContext, MT::CMutex > TWindowContextPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUI */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CWINDOWCONTEXT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2005 :
        - Initial version

---------------------------------------------------------------------------*/
