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

#include "gucefGUI_CWindow.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CWindow::CWindow( void )
    : CPanel( "Window" )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

CWindow::~CWindow()
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

CWidget*
CWindow::Create( void )
{GUCEF_TRACE;

    return GUCEF_NEW CWindow();
}

/*-------------------------------------------------------------------------*/

bool
CWindow::SetTitleBarText( const CString& title )
{GUCEF_TRACE;

    return SetProperty( "Title", title );
}

/*-------------------------------------------------------------------------*/

bool
CWindow::GetTitleBarText( CString& title ) const
{GUCEF_TRACE;

    title = GetProperty( "Title" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CWindow::SetTitleBarVisible( const bool isVisible )
{GUCEF_TRACE;

    return SetBoolProperty( "TitleVisible", isVisible );
}

/*-------------------------------------------------------------------------*/

bool
CWindow::GetTitleBarVisible( bool& isVisible ) const
{GUCEF_TRACE;

    isVisible = GetBoolProperty( "TitleVisible" );
    return true;
}

/*-------------------------------------------------------------------------*/

const CString&
CWindow::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::GUI::CWindow";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
