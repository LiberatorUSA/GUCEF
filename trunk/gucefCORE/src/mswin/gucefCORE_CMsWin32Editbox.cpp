/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
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

#include "gucefCORE_CMsWin32Editbox.h"

#ifdef GUCEF_MSWIN_BUILD

#ifndef GUCEF_CORE_MSWINUTILS_H
#include "gucefCORE_mswinutils.h"
#define GUCEF_CORE_MSWINUTILS_H
#endif /* GUCEF_CORE_MSWINUTILS_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CEvent CMsWin32Editbox::EnterPressedEvent = "GUCEF::CORE::CMsWin32Editbox::EnterPressedEvent";
const CEvent CMsWin32Editbox::TextChangedEvent = "GUCEF::CORE::CMsWin32Editbox::TextChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CMsWin32Editbox::RegisterEvents( void )
{GUCEF_TRACE;

    EnterPressedEvent.Initialize();
    TextChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CMsWin32Editbox::CMsWin32Editbox( void )
    : CMsWin32Window()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CMsWin32Editbox::~CMsWin32Editbox()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CMsWin32Editbox::Clear( void )
{
    SetWindowText( GetHwnd(), "" );
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Editbox::SetText( const CString& text )
{
    SetWindowText( GetHwnd(), text.C_String() );
}

/*-------------------------------------------------------------------------*/
    
CString
CMsWin32Editbox::GetText( void ) const
{
    int len = GetWindowTextLength( GetHwnd() );
    if( len > 0 )
    {
        char* buf = (char*) GlobalAlloc( GPTR, len + 1 );
        GetWindowText( GetHwnd(), buf, len + 1);

        CString returnValue( buf, len );

        GlobalFree( (HANDLE)buf );
        
        return returnValue;
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Editbox::AppendLine( const CString& line )
{GUCEF_TRACE;

    CString currentText = GetText();
    if ( currentText.Length() > 0 )
    {
        char lastChar = currentText[ currentText.Length()-1 ];
        if ( lastChar == '\n' || lastChar == '\r' )
        {
            currentText += line;
        }
        else
        {
            currentText += "\r\n" + line;
        }
        SetText( currentText );
    }
    else
    {
        SetText( line );
    }
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Editbox::AppendLines( TStringVector& lines )
{GUCEF_TRACE;

    TStringVector::iterator i = lines.begin();
    while ( i != lines.end() )
    {
        AppendLine( (*i) );
        ++i;
    }    
}

/*-------------------------------------------------------------------------*/

LRESULT
CMsWin32Editbox::WindowProc( const HWND hWnd     ,
                             const UINT nMsg     ,
                             const WPARAM wParam ,
                             const LPARAM lParam )
{GUCEF_TRACE;

    switch( nMsg )
    {
        case WM_KEYDOWN:
        {
            if( VK_RETURN == wParam )
            {
                NotifyObservers( EnterPressedEvent );
            }
        }
        //case WM_COMMAND:
        //{
        //    // Check if this is a text changed notification
        //    if ( HIWORD( wParam ) == EN_CHANGE )
        //    {
        //        NotifyObservers( TextChangedEvent );
        //        break;
        //    }
        //}
        default:
        {
            break;
        }
    }
    
    return CMsWin32Window::WindowProc( hWnd   ,
                                       nMsg   ,
                                       wParam ,
                                       lParam );
}

/*-------------------------------------------------------------------------*/

bool
CMsWin32Editbox::EditboxCreate( CMsWin32Window& parent           ,
                                const CString& windowTitle       ,
                                const bool createAsMultilineEdit ,
                                const Int32 xPosition            ,
                                const Int32 yPosition            ,
                                const UInt32 width               ,
                                const UInt32 height              ,
                                const bool readOnly              )
{GUCEF_TRACE;

    if ( 0 != GetHwnd() )
    {
        DestroyWindow( GetHwnd() );
        SetHwnd( 0 );
    }

    DWORD dwStyle = 0;
    if ( readOnly )
    {
        dwStyle |= ES_READONLY;
    }
    
    if ( createAsMultilineEdit )
    {        
        dwStyle |= WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_WANTRETURN|ES_AUTOHSCROLL|ES_AUTOVSCROLL;
        
        SetHwnd( CreateWindow( "edit", 
                               windowTitle.C_String(),
                               dwStyle,
                               xPosition, yPosition, (int)width, (int)height, 
                               parent.GetHwnd(), (HMENU)this, GetCurrentModuleHandle(), (LPVOID)this ) );
    }
    else
    {        
        dwStyle |= WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL|ES_AUTOVSCROLL;
        
        SetHwnd( CreateWindow( "edit", 
                               windowTitle.C_String(),
                               dwStyle,
                               xPosition, yPosition, (int)width, (int)height, 
                               parent.GetHwnd(), (HMENU)this, GetCurrentModuleHandle(), (LPVOID)this ) );
    }

    if ( GetHwnd() != 0 )
    {
        HookWndProc();
        return true;
    }            
    return false;
}

/*-------------------------------------------------------------------------*/

const CString&
CMsWin32Editbox::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::CORE::CMsWin32Editbox";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */
