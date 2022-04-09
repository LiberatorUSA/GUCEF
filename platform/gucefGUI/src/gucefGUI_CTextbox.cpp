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

#include "gucefGUI_CTextbox.h"

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

const CORE::CEvent CTextbox::SelectionChangedEvent = "GUCEF::GUI::CTextbox::SelectionChangedEvent";
const CORE::CEvent CTextbox::TextChangedEvent = "GUCEF::GUI::CTextbox::TextChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CTextbox::RegisterEvents( void )
{GUCEF_TRACE;

    SelectionChangedEvent.Initialize();
    TextChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CTextbox::CTextbox( void )
    : CWidget( "Textbox" )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CTextbox::~CTextbox()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CTextbox::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::GUI::CTextbox";
    return typeName;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CTextbox::GetLineCount( void )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/
    
bool
CTextbox::Clear( void )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CTextbox::GetLineAtIndex( const UInt32 lineIndex ,
                          CString& lineText      ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CTextbox::InsertLineAtIndex( const UInt32 lineIndex ,
                             const CString& text    )
{GUCEF_TRACE;


    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CTextbox::AppendLine( const CString& text )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
CString
CTextbox::GetTextAsString( void ) const
{GUCEF_TRACE;

    return CString();
}

/*-------------------------------------------------------------------------*/
    
CTextbox::TStringVector
CTextbox::GetText( void ) const
{GUCEF_TRACE;

    return TStringVector();
}

/*-------------------------------------------------------------------------*/

CString
CTextbox::GetSelectedText( void ) const
{GUCEF_TRACE;

    return CString();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
