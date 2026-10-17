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

#ifndef GUCEF_GUI_CLISTBOX_H
#include "gucefGUI_CListBox.h"
#define GUCEF_GUI_CLISTBOX_H
#endif /* GUCEF_GUI_CLISTBOX_H ? */

#include "gucefGUI_CCombobox.h"

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

const CORE::CEvent CCombobox::TextChangedEvent = "GUCEF::GUI::CCombobox::TextChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CCombobox::RegisterEvents( void )
{GUCEF_TRACE;

    TextChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CCombobox::CCombobox( void )
    : CWidget( "Combobox" )
    , m_list( GUCEF_NULL )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CCombobox::~CCombobox()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_list && GUCEF_NULL != GetContext() )
        GetContext()->DestroyWidget( m_list );
    m_list = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CWidget*
CCombobox::Create( void )
{GUCEF_TRACE;

    return GUCEF_NEW CCombobox();
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::SetText( const CString& text )
{GUCEF_TRACE;

    return SetProperty( "Text", text );
}

/*-------------------------------------------------------------------------*/

CString
CCombobox::GetText( void ) const
{GUCEF_TRACE;

    return GetProperty( "Text" );
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::SetListItems( const TStringVector& items )
{GUCEF_TRACE;

    return SetStringListProperty( "Items", items );
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::GetListItems( TStringVector& items ) const
{GUCEF_TRACE;

    items = GetStringListProperty( "Items" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::ClearList( void )
{GUCEF_TRACE;

    SetIntProperty( "SelectedIndex", -1 );
    return SetProperty( "Items", CString::Empty );
}

/*-------------------------------------------------------------------------*/

Int32
CCombobox::GetSelectedIndex( void ) const
{GUCEF_TRACE;

    return GetIntProperty( "SelectedIndex" );
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::SetSelectedIndex( Int32 index )
{GUCEF_TRACE;

    return SetIntProperty( "SelectedIndex", index );
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::SetExpanded( bool expanded )
{GUCEF_TRACE;

    CGUIContext* context = GetContext();
    if ( GUCEF_NULL == context )
        return false;

    if ( !expanded )
    {
        if ( GUCEF_NULL != m_list && context->GetPopup() == m_list )
            context->ClosePopup();
        return SetBoolProperty( "Expanded", false );
    }

    if ( GUCEF_NULL == m_list )
    {
        m_list = static_cast< CListbox* >( context->CreateWidget( "Listbox", GetName() + "#List", GUCEF_NULL ) );
        if ( GUCEF_NULL == m_list )
            return false;
        SubscribeTo( m_list, CWidget::ValueChangedEvent );
        SubscribeTo( m_list, CWidget::ItemActivatedEvent );
        SubscribeTo( context, CGUIContext::FormHiddenEvent );
    }
    TStringVector items = GetStringListProperty( "Items" );
    Int32 rows = GetIntProperty( "DropDownRows" );
    if ( rows <= 0 || rows > (Int32) items.size() )
        rows = (Int32) items.size();
    m_list->SetItems( items );
    m_list->SetProperty( "Font", GetProperty( "Font" ) );
    m_list->SetProperty( "TextColour", GetProperty( "TextColour" ) );
    m_list->SetProperty( "BackgroundColour", GetProperty( "BackgroundColour" ) );
    m_list->SetSelectedIndex( GetSelectedIndex() );
    m_list->SetSize( CUVector( CUDim( 0.0f, GetRect().width ), CUDim( 0.0f, rows * m_list->GetRowHeight() ) ) );
    context->ShowPopup( m_list, GetRect().left, GetRect().Bottom() );
    return SetBoolProperty( "Expanded", true );
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::GetNaturalSize( Int32& width, Int32& height ) const
{GUCEF_TRACE;

    width = height = 0;
    if ( GUCEF_NULL == GetContext() )
        return false;
    TStringVector items = GetStringListProperty( "Items" );
    for ( UInt32 i=0; i<(UInt32)items.size(); ++i )
    {
        Int32 w = 0, h = 0;
        GetContext()->MeasureText( GetProperty( "Font" ), items[ i ], 0, w, h );
        if ( w > width ) width = w;
    }
    width += 24;
    height = GetContext()->GetLineHeight( GetProperty( "Font" ) ) + 4;
    return true;
}

/*-------------------------------------------------------------------------*/

void
CCombobox::OnPropertyChanged( const CString& name, const CString& oldValue, const CString& newValue )
{GUCEF_TRACE;

    if ( name == "SelectedIndex" )
    {
        TStringVector items = GetStringListProperty( "Items" );
        Int32 index = CORE::StringToInt32( newValue, -1 );
        SetProperty( "Text", index >= 0 && index < (Int32) items.size() ? items[ index ] : CString::Empty );
    }
    else
    if ( name == "Text" )
        NotifyObservers( TextChangedEvent );
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::OnDefaultAction( const CORE::CEvent& eventId, CRoutedEventData& eventData )
{GUCEF_TRACE;

    if ( ActivatedEvent == eventId || ( MouseClickEvent == eventId && MOUSEBUTTON_LEFT == static_cast< CPointerEventData* >( &eventData )->button ) )
    {
        SetExpanded( !GetBoolProperty( "Expanded" ) );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CCombobox::OnNotify( CORE::CNotifier* notifier                 ,
                     const CORE::CEvent& eventId               ,
                     CORE::CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_list && notifier == m_list )
    {
        if ( CWidget::ValueChangedEvent == eventId || CWidget::ItemActivatedEvent == eventId )
        {
            Int32 picked = m_list->GetSelectedIndex();
            SetExpanded( false );
            ChangeValueByUser( "SelectedIndex", CORE::Int32ToString( picked ) );
        }
        return;
    }
    if ( CGUIContext::FormHiddenEvent == eventId && GUCEF_NULL != m_list && GUCEF_NULL != GetContext() && GetContext()->GetPopup() != m_list )
        SetBoolProperty( "Expanded", false );
    CWidget::OnNotify( notifier, eventId, eventData );
}

/*-------------------------------------------------------------------------*/

const CString&
CCombobox::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::GUI::CCombobox";
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
