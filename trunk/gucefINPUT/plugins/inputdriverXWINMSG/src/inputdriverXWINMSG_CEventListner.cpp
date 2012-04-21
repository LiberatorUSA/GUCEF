/*
 *  inputdriverXWINMSG: GUCEF input driver module for X11
 *  Copyright (C) 2002 - 2012.  Dinand Vanvelzen
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

#include "inputdriverXWINMSG_CEventListner.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {
namespace XWINMSG {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventListner::CEventListner( ::Window window                  ,
                              const TInputCallbacks* callbacks )
    : CObserver()              ,
      m_window( window )       ,
      m_callbacks( callbacks ) ,
      m_mouseX( 0 )            ,
      m_mouseY( 0 )
{GUCEF_TRACE;

    CORE::CX11EventDispatcher::Instance()->SubscribeOnBehalfOfWindow( *this, window );
}

/*--------------------------------------------------------------------------*/

CEventListner::~CEventListner()
{
    CORE::CX11EventDispatcher::Instance()->UnsubscribeOnBehalfOfWindow( *this, m_window );
}

/*--------------------------------------------------------------------------*/

::Window
CEventListner::GetWindow( void ) const
{
    return m_window;
}

/*--------------------------------------------------------------------------*/

const TInputCallbacks*
CEventListner::GetCallbacks( void ) const
{
    return m_callbacks;
}

/*


typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseButtonDown )  ( void* userData, const Int32 deviceID, const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseButtonUp )    ( void* userData, const Int32 deviceID, const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseMove )        ( void* userData, const Int32 deviceID, const Int32 xPos, const Int32 yPos, const Int32 xDelta, const Int32 yDelta ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseVarChanged )  ( void* userData, const Int32 deviceID, const UInt32 varIndex, const Int32 value, const Int32 valueDelta ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnKeyboardKeyDown )  ( void* userData, const Int32 deviceID, const KeyCode keyCode, const UInt32 keyModStates ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnKeyboardKeyUp )    ( void* userData, const Int32 deviceID, const KeyCode keyCode, const UInt32 keyModStates ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnDeviceBooleanOn )  ( void* userData, const Int32 deviceID, const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnDeviceBooleanOff ) ( void* userData, const Int32 deviceID, const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnDeviceVarChanged ) ( void* userData, const Int32 deviceID, const UInt32 stateindex, const Float32 value ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseAttached ) ( void* userData, const Int32 deviceID ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnKeyboardAttached ) ( void* userData, const Int32 deviceID ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnDeviceAttached ) ( void* userData, const Int32 deviceID ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseDetached ) ( void* userData, const Int32 deviceID ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnKeyboardDetached ) ( void* userData, const Int32 deviceID ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnDeviceDetached ) ( void* userData, const Int32 deviceID ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

*/
/*--------------------------------------------------------------------------*/

void
CEventListner::OnX11Event( ::XEvent event )
{
    // We only care about input related events
    switch ( event.type )
    {
        case KeyRelease:
        {
            m_callbacks->onKeyboardKeyUp( 0, 0, (GUCEF::INPUT::KeyCode) event.xkey.keycode, 0 );
            return;
        }
        case KeyPress:
        {
            m_callbacks->onKeyboardKeyDown( 0, 0, (GUCEF::INPUT::KeyCode) event.xkey.keycode, 0 );
            return;
        }
        case ButtonPress:
        {
            m_callbacks->onMouseButtonDown( 0, 0, event.xbutton.button );
            return;
        }
        case ButtonRelease:
        {
            m_callbacks->onMouseButtonUp( 0, 0, event.xbutton.button );
            return;
        }
        case MotionNotify:
        {
            Int32 deltaX = event.xmotion.x - m_mouseX;
            Int32 deltaY = event.xmotion.y - m_mouseY;

            m_mouseX = event.xmotion.x;
            m_mouseY = event.xmotion.y;

            m_callbacks->onMouseMove( 0, 0, m_mouseX, m_mouseY, deltaX, deltaY );
            return;
        }
    }
}

//
//typedef struct {
//	int type;		/* of event */
//	unsigned long serial;	/* # of last request processed by server */
//	Bool send_event;	/* true if this came from a SendEvent request */
//	Display *display;	/* Display the event was read from */
//	Window window;	        /* "event" window it is reported relative to */
//	Window root;	        /* root window that the event occurred on */
//	Window subwindow;	/* child window */
//	Time time;		/* milliseconds */
//	int x, y;		/* pointer x, y coordinates in event window */
//	int x_root, y_root;	/* coordinates relative to root */
//	unsigned int state;	/* key or button mask */
//	unsigned int button;	/* detail */
//	Bool same_screen;	/* same screen flag */
//} XButtonEvent;
//typedef XButtonEvent XButtonPressedEvent;
//typedef XButtonEvent XButtonReleasedEvent;
//
//typedef struct {
//	int type;		/* of event */
//	unsigned long serial;	/* # of last request processed by server */
//	Bool send_event;	/* true if this came from a SendEvent request */
//	Display *display;	/* Display the event was read from */
//	Window window;	        /* "event" window reported relative to */
//	Window root;	        /* root window that the event occurred on */
//	Window subwindow;	/* child window */
//	Time time;		/* milliseconds */
//	int x, y;		/* pointer x, y coordinates in event window */
//	int x_root, y_root;	/* coordinates relative to root */
//	unsigned int state;	/* key or button mask */
//	char is_hint;		/* detail */
//	Bool same_screen;	/* same screen flag */
//} XMotionEvent;
//typedef XMotionEvent XPointerMovedEvent;

/*--------------------------------------------------------------------------*/

void
CEventListner::OnNotify( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventid  ,
                         CORE::CICloneable* eventdata )
{
    if ( CORE::CX11EventDispatcher::X11Event == eventid )
    {
        CORE::CX11EventDispatcher::TX11EventData* x11data = (CORE::CX11EventDispatcher::TX11EventData*) eventdata;
        OnX11Event( x11data->GetData() );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace XWINMSG */
}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
