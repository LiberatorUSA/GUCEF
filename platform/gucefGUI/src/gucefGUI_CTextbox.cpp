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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_GUI_CGUICONTEXT_H
#include "gucefGUI_CGUIContext.h"
#define GUCEF_GUI_CGUICONTEXT_H
#endif /* GUCEF_GUI_CGUICONTEXT_H ? */

#ifndef GUCEF_GUI_UTILS_H
#include "gucefGUI_Utils.h"
#define GUCEF_GUI_UTILS_H
#endif /* GUCEF_GUI_UTILS_H ? */

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/


namespace {

/** The height of one row: the RowHeight property, else the line height of the font */
Int32
RowHeightOf( const CWidget& widget )
{
    Int32 rowHeight = widget.GetIntProperty( "RowHeight" );
    if ( rowHeight > 0 )
        return rowHeight;
    if ( GUCEF_NULL != widget.GetContext() )
        return widget.GetContext()->GetLineHeight( widget.GetProperty( "Font" ) );
    return 16;
}

}

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

CWidget*
CTextbox::Create( void )
{GUCEF_TRACE;

    return GUCEF_NEW CTextbox();
}

/*-------------------------------------------------------------------------*/

UInt32
CTextbox::GetLineCount( void )
{GUCEF_TRACE;

    return (UInt32) GetText().size();
}

/*-------------------------------------------------------------------------*/

bool
CTextbox::Clear( void )
{GUCEF_TRACE;

    SetIntProperty( "ScrollPosition", 0 );
    return SetProperty( "Text", CString::Empty );
}

/*-------------------------------------------------------------------------*/

bool
CTextbox::GetLineAtIndex( const UInt32 lineIndex ,
                          CString& lineText      ) const
{GUCEF_TRACE;

    TStringVector lines = GetText();
    if ( lineIndex >= (UInt32) lines.size() )
        return false;
    lineText = lines[ lineIndex ];
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTextbox::InsertLineAtIndex( const UInt32 lineIndex ,
                             const CString& text    )
{GUCEF_TRACE;

    TStringVector lines = GetText();
    if ( lineIndex > (UInt32) lines.size() )
        return false;
    lines.insert( lines.begin() + lineIndex, text );
    return SetProperty( "Text", JoinStringList( lines ) );
}

/*-------------------------------------------------------------------------*/

bool
CTextbox::AppendLine( const CString& text )
{GUCEF_TRACE;

    CString current = GetProperty( "Text" );
    return SetProperty( "Text", current.IsNULLOrEmpty() ? text : current + '\n' + text );
}

/*-------------------------------------------------------------------------*/

CString
CTextbox::GetTextAsString( void ) const
{GUCEF_TRACE;

    return GetProperty( "Text" );
}

/*-------------------------------------------------------------------------*/

CTextbox::TStringVector
CTextbox::GetText( void ) const
{GUCEF_TRACE;

    return SplitStringList( GetProperty( "Text" ) );
}

/*-------------------------------------------------------------------------*/

CString
CTextbox::GetSelectedText( void ) const
{GUCEF_TRACE;

    return CString::Empty;
}

/*-------------------------------------------------------------------------*/

CTextbox::TStringVector
CTextbox::GetDisplayLines( void ) const
{GUCEF_TRACE;

    CString text = GetProperty( "Text" );
    CGUIContext* context = GetContext();
    if ( GUCEF_NULL != context )
    {
        TBitmapFontPtr font = context->GetBitmapFont( GetProperty( "Font" ) );
        if ( !font.IsNULL() )
        {
            CBitmapFont::TStringVector broken = font->BreakLines( text, GetBoolProperty( "WordWrap" ) ? GetRect().width : 0 );
            return TStringVector( broken.begin(), broken.end() );
        }
    }
    return SplitStringList( text );
}

/*-------------------------------------------------------------------------*/

Int32
CTextbox::GetVisibleLineCount( void ) const
{GUCEF_TRACE;

    Int32 rowHeight = RowHeightOf( *this );
    return rowHeight > 0 ? GetRect().height / rowHeight : 0;
}

/*-------------------------------------------------------------------------*/

void
CTextbox::OnPropertyChanged( const CString& name, const CString& oldValue, const CString& newValue )
{GUCEF_TRACE;

    if ( name == "Text" )
        NotifyObservers( TextChangedEvent );
}

/*-------------------------------------------------------------------------*/

bool
CTextbox::OnDefaultAction( const CORE::CEvent& eventId, CRoutedEventData& eventData )
{GUCEF_TRACE;

    if ( MouseWheelEvent != eventId )
        return false;
    CWheelEventData* wheel = static_cast< CWheelEventData* >( &eventData );
    Int32 maxScroll = (Int32) GetDisplayLines().size() - GetVisibleLineCount();
    if ( maxScroll < 0 )
        maxScroll = 0;
    Int32 scroll = GetIntProperty( "ScrollPosition" ) - wheel->delta;
    if ( scroll < 0 ) scroll = 0;
    if ( scroll > maxScroll ) scroll = maxScroll;
    ChangeValueByUser( "ScrollPosition", CORE::Int32ToString( scroll ) );
    return true;
}

/*-------------------------------------------------------------------------*/

const CString&
CTextbox::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::GUI::CTextbox";
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
