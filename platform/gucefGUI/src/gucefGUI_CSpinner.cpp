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

#include "gucefGUI_CSpinner.h"

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

const CORE::CEvent CSpinner::ValueChangedEvent = "GUCEF::GUI::CSpinner::ValueChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CSpinner::RegisterEvents( void )
{GUCEF_TRACE;

    ValueChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CSpinner::CSpinner( void )
    : CWidget( "Spinner" )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CSpinner::~CSpinner()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CSpinner::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::GUI::CSpinner";
    return typeName;
}

/*-------------------------------------------------------------------------*/

bool
CSpinner::SetValueUInt32( const UInt32 value )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CSpinner::SetValueInt32( const Int32 value )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CSpinner::SetValueFloat32( const Float32 value )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CSpinner::GetValueUInt32( void ) const
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/
    
Int32
CSpinner::GetValueInt32( void ) const
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/
    
Float32
CSpinner::GetValueFloat32( void ) const
{GUCEF_TRACE;

    return 0.0f;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
