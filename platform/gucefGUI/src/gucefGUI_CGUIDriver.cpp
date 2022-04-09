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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefGUI_CGUIDriver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CGUIDriver::WidgetTypeRegisteredEvent = "GUCEF::GUI::WidgetTypeRegisteredEvent";
const CORE::CEvent CGUIDriver::WidgetTypeUnregisteredEvent = "GUCEF::GUI::WidgetTypeUnregisteredEvent";    
const CORE::CEvent CGUIDriver::FormTypeRegisteredEvent = "GUCEF::GUI::FormTypeRegisteredEvent";
const CORE::CEvent CGUIDriver::FormTypeUnregisteredEvent = "GUCEF::GUI::FormTypeUnregisteredEvent";
const CORE::CEvent CGUIDriver::FontTypeRegisteredEvent = "GUCEF::GUI::FontTypeRegisteredEvent";
const CORE::CEvent CGUIDriver::FontTypeUnregisteredEvent = "GUCEF::GUI::FontTypeUnregisteredEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CGUIDriver::RegisterEvents( void )
{GUCEF_TRACE;

    WidgetTypeRegisteredEvent.Initialize();
    WidgetTypeUnregisteredEvent.Initialize();
    FormTypeRegisteredEvent.Initialize();
    FormTypeUnregisteredEvent.Initialize();
    FontTypeRegisteredEvent.Initialize();
    FontTypeUnregisteredEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CGUIDriver::CGUIDriver()
    : CObservingNotifier()                    ,
      CORE::CGloballyConfigurable()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CGUIDriver::CGUIDriver( const CGUIDriver& src )
    : CObservingNotifier( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CGUIDriver::~CGUIDriver()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CGUIDriver::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::GUI::CGUIDriver";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
