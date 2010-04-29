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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* module config macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifdef GUCEF_MSWIN_BUILD

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "gucefCORE_CMsWin32ConsoleWindow.h"

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

//const CEvent CWndMsgHookNotifier::WindowActivationEvent = "GUCEF::CORE::CWndMsgHookNotifier::WindowActivationEvent";
//const CEvent CWndMsgHookNotifier::WindowSizeEvent = "GUCEF::CORE::CWndMsgHookNotifier::WindowSizeEvent";
//const CEvent CWndMsgHookNotifier::WindowDestroyEvent = "GUCEF::CORE::CWndMsgHookNotifier::WindowDestroyEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

//void
//CWndMsgHookNotifier::RegisterEvents( void )
//{GUCEF_TRACE;
//
//    WindowActivationEvent.Initialize();
//    WindowSizeEvent.Initialize();
//    WindowDestroyEvent.Initialize();
//}

/*-------------------------------------------------------------------------*/

CMsWin32ConsoleWindow::CMsWin32ConsoleWindow( void )
    : CMsWin32Window() ,
      m_inputbox()     ,
      m_outputbox()
{GUCEF_TRACE;

  //  RegisterEvents();
  
    SubscribeTo( &m_inputbox );
    SubscribeTo( &m_outputbox );
}

/*-------------------------------------------------------------------------*/

CMsWin32ConsoleWindow::~CMsWin32ConsoleWindow()
{GUCEF_TRACE;

    m_inputbox.WindowDestroy();
    m_outputbox.WindowDestroy();
}

/*-------------------------------------------------------------------------*/

void
CMsWin32ConsoleWindow::AppendLine( const CString& line )
{GUCEF_TRACE;

    m_outputbox.AppendLine( GetPath() + ">" + line ); 
}

/*-------------------------------------------------------------------------*/

void
CMsWin32ConsoleWindow::AppendLines( const TStringVector& lines )
{GUCEF_TRACE;

    TStringVector::const_iterator i = lines.begin();
    while ( i != lines.end() )
    {
        AppendLine( (*i) );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CMsWin32ConsoleWindow::OnNotify( CNotifier* notifier    ,
                                 const CEvent& eventid  ,
                                 CICloneable* eventdata )
{GUCEF_TRACE;

    if ( &m_inputbox == notifier )
    {
        if ( CMsWin32Editbox::EnterPressedEvent == eventid )
        {
            CString inputText = m_inputbox.GetText();
            AppendLine( inputText );
            
            TStringVector resultdata;
            if ( ProcessUserInput( inputText  ,
                                   resultdata ) )
            {
                AppendLines( resultdata );
            }
        }
    }
    else
    if ( &m_outputbox == notifier )
    {
    }
    else
    {
        CMsWin32Window::OnNotify( notifier  ,
                                  eventid   ,
                                  eventdata );
    }
}

/*-------------------------------------------------------------------------*/

bool
CMsWin32ConsoleWindow::ConsoleWindowCreate( void )
{GUCEF_TRACE;
                 
    if ( RegisterWindowClass( GetClassTypeName() ) )
    {
        if ( WindowCreate( GetClassTypeName() ,
                           "System Console"   ,
                           CW_USEDEFAULT      ,
                           CW_USEDEFAULT      ,
                           420                , 
                           360                ,
                           0                  ) )
        {
            if ( m_outputbox.EditboxCreate( *this     ,
                                            CString() ,
                                            true      ,
                                            0         ,
                                            0         ,
                                            400       ,
                                            280       ,
                                            true      ) )
            {
                if ( m_inputbox.EditboxCreate( *this     ,
                                               CString() ,
                                               false     ,
                                               0         ,
                                               300       ,
                                               400       ,
                                               60        ,
                                               false     ) )
                { 
                    m_outputbox.Show();
                    m_inputbox.Show();
                    return true;                                 
                }
            }
                                                                                            
        }        
    }    
    return false;
}

/*-------------------------------------------------------------------------*/

const CString&
CMsWin32ConsoleWindow::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::CORE::CMsWin32ConsoleWindow";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

LRESULT
CMsWin32ConsoleWindow::WindowProc( const HWND hWnd     ,
                                   const UINT nMsg     ,
                                   const WPARAM wParam ,
                                   const LPARAM lParam )
{GUCEF_TRACE;

    return CMsWin32Window::WindowProc( hWnd, nMsg, wParam, lParam );
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
