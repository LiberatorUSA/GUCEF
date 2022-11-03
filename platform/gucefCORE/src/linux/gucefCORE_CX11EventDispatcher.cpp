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

#include "gucefCORE_CX11EventDispatcher.h"

#ifdef GUCEF_LINUX_BUILD

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

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

MT::CMutex CX11EventDispatcher::g_mutex;
CX11EventDispatcher* CX11EventDispatcher::g_instance = NULL;

const CEvent CX11EventDispatcher::X11Event = "GUCEF::CORE::CX11EventDispatcher::X11Event";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CX11EventDispatcher::RegisterEvents( void )
{GUCEF_TRACE;

    X11Event.Initialize();
}

/*-------------------------------------------------------------------------*/

CX11EventDispatcher::CX11EventDispatcher( void )
    : m_windowObserverMap(),
      m_display( NULL )
{GUCEF_TRACE;

    PulseGeneratorPtr pulseGenerator = CCoreGlobal::Instance()->GetPulseGenerator();
    SubscribeTo( pulseGenerator.GetPointerAlways(), CPulseGenerator::PulseEvent );
    pulseGenerator->RequestPeriodicPulses( this, 10 );

    // Open the connection to the X client
    m_display = ::XOpenDisplay( NULL );
    if ( NULL == m_display )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CX11Window::WindowCreate(): Failed to open display" );
    }
}

/*-------------------------------------------------------------------------*/

CX11EventDispatcher::~CX11EventDispatcher()
{GUCEF_TRACE;

    m_windowObserverMap.clear();
}

/*-------------------------------------------------------------------------*/

CX11EventDispatcher*
CX11EventDispatcher::Instance( void )
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        g_mutex.Lock();
        if ( NULL == g_instance )
        {
            g_instance = new CX11EventDispatcher();
        }
        g_mutex.Unlock();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::Deinstance( void )
{GUCEF_TRACE;

    g_mutex.Lock();
    delete g_instance;
    g_instance = NULL;
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::SubscribeOnBehalfOfWindow( CObserver& observer ,
                                                ::Window window     )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Subscribing observer (" +
             CORE::PointerToString( &observer ) + ") to X11 events on behalf of window " +
             CORE::UInt32ToString( window ) );

    // First create the actual subscription
    Subscribe( &observer );

    // Now update our local map which we use to filter who to send event to
    g_mutex.Lock();
    m_windowObserverMap[ window ].insert( &observer );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::UnsubscribeOnBehalfOfWindow( CObserver& observer ,
                                                  ::Window window     )
{GUCEF_TRACE;

    g_mutex.Lock();
    TWindowObserverMap::iterator i = m_windowObserverMap.find( window );
    if ( i != m_windowObserverMap.end() )
    {
        (*i).second.erase( &observer );
        if ( (*i).second.empty() )
        {
            m_windowObserverMap.erase( i );
        }
    }
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::OnObserverDestruction( CObserver* observer )
{GUCEF_TRACE;

    g_mutex.Lock();

    TWindowObserverMap::iterator i = m_windowObserverMap.begin();
    while ( i != m_windowObserverMap.end() )
    {
        TObserverSet& observerSet = (*i).second;
        TObserverSet::iterator n = observerSet.begin();
        while ( n != observerSet.end() )
        {
            if ( (*n) == observer )
            {
                observerSet.erase( n );
                if ( observerSet.empty() )
                {
                    m_windowObserverMap.erase( i );
                }
                break;
            }
            ++n;
        }
        ++i;
    }

    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::LockData( void ) const
{GUCEF_TRACE;

    g_mutex.Lock();
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::UnlockData( void ) const
{GUCEF_TRACE;

    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::DispatchEventToWindow( ::XEvent& event ,
                                            ::Window window )
{GUCEF_TRACE;

    TWindowObserverMap::iterator i = m_windowObserverMap.find( window );
    if ( i != m_windowObserverMap.end() )
    {
        TX11EventData eventData( event );
        TObserverSet& observerSet = (*i).second;
        TObserverSet::iterator n = observerSet.begin();
        while ( n != observerSet.end() )
        {
            NotifySpecificObserver( *(*n)      ,
                                    X11Event   ,
                                    &eventData );
            ++n;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::ProcessEvent( ::XEvent& event )
{GUCEF_TRACE;

    switch( event.type )
    {
        case KeyRelease :
        case KeyPress :
        {
            DispatchEventToWindow( event, event.xkey.window );
            break;
        }
        case ButtonRelease :
        case ButtonPress :
        {
            DispatchEventToWindow( event, event.xbutton.window );
            break;
        }
        case MotionNotify :
        {
            DispatchEventToWindow( event, event.xmotion.window );
            break;
        }
        case ResizeRequest :
        {
            DispatchEventToWindow( event, event.xresizerequest.window );
            break;
        }
        case EnterNotify :
        case LeaveNotify :
        {
            DispatchEventToWindow( event, event.xcrossing.window );
            break;
        }
        case FocusIn :
        case FocusOut :
        {
            DispatchEventToWindow( event, event.xfocus.window );
            break;
        }
        case Expose :
        {
            DispatchEventToWindow( event, event.xexpose.window );
            break;
        }
        case GraphicsExpose :
        {
            //DispatchEventToWindow( event, event.xgraphicsexpose.window );
            break;
        }
        case NoExpose :
        {
            //DispatchEventToWindow( event, event.xnoexpose.window );
            break;
        }
        case VisibilityNotify :
        {
            DispatchEventToWindow( event, event.xvisibility.window );
            break;
        }
        case CreateNotify :
        {
            DispatchEventToWindow( event, event.xcreatewindow.window );
            break;
        }
        case DestroyNotify :
        {
            DispatchEventToWindow( event, event.xdestroywindow.window );
            break;
        }
        case UnmapNotify :
        {
            DispatchEventToWindow( event, event.xunmap.window );
            break;
        }
        case MapNotify :
        {
            DispatchEventToWindow( event, event.xmap.window );
            break;
        }
        case MapRequest :
        {
            DispatchEventToWindow( event, event.xmaprequest.window );
            break;
        }
        case ReparentNotify :
        {
            DispatchEventToWindow( event, event.xreparent.window );
            break;
        }
        case ConfigureNotify :
        {
            DispatchEventToWindow( event, event.xconfigure.window );
            break;
        }
        case GravityNotify :
        {
            DispatchEventToWindow( event, event.xgravity.window );
            break;
        }
        case ConfigureRequest :
        {
            DispatchEventToWindow( event, event.xconfigurerequest.window );
            break;
        }
        case CirculateNotify :
        {
            DispatchEventToWindow( event, event.xcirculate.window );
            break;
        }
        case CirculateRequest :
        {
            DispatchEventToWindow( event, event.xcirculaterequest.window );
            break;
        }
        case PropertyNotify :
        {
            DispatchEventToWindow( event, event.xproperty.window );
            break;
        }
        case SelectionClear :
        {
            DispatchEventToWindow( event, event.xselectionclear.window );
            break;
        }
        case SelectionRequest :
        {
            DispatchEventToWindow( event, event.xselectionrequest.owner );
            break;
        }
        case SelectionNotify :
        {
            DispatchEventToWindow( event, event.xselection.requestor );
            break;
        }
        case ColormapNotify :
        {
            DispatchEventToWindow( event, event.xcolormap.window );
            break;
        }
        case ClientMessage :
        {
            DispatchEventToWindow( event, event.xclient.window );
            break;
        }
        case MappingNotify :
        {
            DispatchEventToWindow( event, event.xmapping.window );
            break;
        }
        //case ::ErrorNotify :
        //{
            //DispatchEventToWindow( event, event.xerror.window );
            //break;
        //}
        case KeymapNotify :
        {
            DispatchEventToWindow( event, event.xkeymap.window );
            break;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CX11EventDispatcher::OnNotify( CNotifier* notifier    ,
                               const CEvent& eventid  ,
                               CICloneable* eventdata )
{GUCEF_TRACE;

    if ( eventid == CPulseGenerator::PulseEvent )
    {
        XSync( m_display, False );

        ::XEvent event;
        while ( 0 != ::XEventsQueued( m_display, QueuedAlready ) )
        {
            ::XNextEvent( m_display, &event );
            ProcessEvent( event );
        }
    }
}

/*-------------------------------------------------------------------------*/

::Display*
CX11EventDispatcher::GetDisplay( void ) const
{GUCEF_TRACE;

    return m_display;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_LINUX_BUILD ? */
