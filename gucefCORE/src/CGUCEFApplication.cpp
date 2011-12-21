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

#include <assert.h>

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"   /* Additional utilities for manipulating string class objects */
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_TSPRINTING_H
#include "tsprinting.h"         /* centralized console output */
#define GUCEF_CORE_TSPRINTING_H
#endif /* GUCEF_CORE_TSPRINTING_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"     /* centralized plugin control */
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"          /* node for data storage */
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CSYSCONSOLE_H
#include "CSysConsole.h"
#define GUCEF_CORE_CSYSCONSOLE_H
#endif /* GUCEF_CORE_CSYSCONSOLE_H ? */

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#include "CGUCEFApplication.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

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

const CEvent CGUCEFApplication::AppInitEvent = "GUCEF::CORE::CGUCEFApplication::AppInitEvent";
const CEvent CGUCEFApplication::AppShutdownEvent = "GUCEF::CORE::CGUCEFApplication::AppShutdownEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUCEFApplication::CGUCEFApplication( void )
    : CIConfigurable( true )                                 ,
      _initialized( false )                                  ,
      m_shutdownRequested( false )                           ,
      m_pulseGenerator()                                     ,
      m_busyWaitPulseDriver()
{GUCEF_TRACE;

    RegisterEvents();

    /*
     *      Set an initial app dir just in case we have trouble getting one
     *      at a later stage
     */
    _appdir = RelativePath( "$MODULEDIR$" );

    /*
     *      Initialize our plugin control center
     */
    CPluginControl::Instance();

    /*
     *      Register some functionality at the system console
     */
    CSysConsole* sysconsole = CSysConsole::Instance();
    std::vector< CString > args;
    sysconsole->RegisterCmd( "GUCEF\\CORE\\CGUCEFApplication" ,
                             "Stop"                           ,
                             args                             ,
                             this                             );
    sysconsole->RegisterCmd( "GUCEF\\CORE\\CGUCEFApplication" ,
                             "GetApplicationDir"              ,
                             args                             ,
                             this                             );
    sysconsole->RegisterAlias( "exit"                           ,
                               "GUCEF\\CORE\\CGUCEFApplication" ,
                               "Stop"                           );

    // Set the busy wait pulse driver as the default driver for the
    // pulse generator
    m_pulseGenerator.SetPulseGeneratorDriver( &m_busyWaitPulseDriver );
}

/*-------------------------------------------------------------------------*/

CGUCEFApplication::~CGUCEFApplication()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPulseGenerator&
CGUCEFApplication::GetPulseGenerator( void )
{GUCEF_TRACE;

    return m_pulseGenerator;
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
int
CGUCEFApplication::Main( HINSTANCE hinstance     ,
                         LPSTR lpcmdline         ,
                         int ncmdshow            ,
                         bool run                )
{GUCEF_TRACE;

    /*
     *      ensure that we have an instance of this class
     */
    m_mutex.Lock();
    if ( !_instance )
    {
        int retval = Instance()->Main( hinstance     ,
                                       lpcmdline     ,
                                       ncmdshow      ,
                                       run           );
        m_mutex.Unlock();
        return retval;
    }

    /*
     *      Set the application dir
     *      This is pure MSWIN code and is not portable !!!
     */
    {
        /*
         *      Set the given values as environment vars
         */
        #pragma warning( disable: 4311 )
        char intstr[ 10 ];
        sprintf( intstr, "%d", (UInt32)hinstance );
        GUCEFSetEnv( "HINSTANCE", intstr );

        char apppath[ MAX_PATH ];

        if ( GetModuleFileNameA( hinstance, apppath, MAX_PATH ) )
        {
                _appdir = apppath;
        }
    }

    struct SAppInitEventData data;

    // Copy the MSWIN params
    data.hinstance = hinstance;
    data.lpcmdline = lpcmdline;
    data.ncmdshow = ncmdshow;

    // Parse the MSWIN param into the old style param list
    std::vector< CString > argList;
    if ( NULL != lpcmdline )
    {
        argList = CString( lpcmdline ).ParseElements( ' ' );
    }

    data.argc = (int)argList.size();
    if ( data.argc > 0 )
    {
        data.argv = new char*[ data.argc ];
        for ( Int32 i=0; i<data.argc; ++i )
        {
            data.argv[ i ] = const_cast< char* >( argList[ i ].C_String() );
        }
    }
    else
    {
        data.argv = NULL;
    }

    // Set shutdown to false before we emit the init event so that observers can
    // request a shutdown if something went wrong
    m_shutdownRequested = false;

    // Dispatch the initialization event
    TAppInitEventData cloneableData( data );
    if ( !NotifyObservers( AppInitEvent, &cloneableData ) ) return 0;

    // Check if we need to abort because a shutdown was requested
    if ( !m_shutdownRequested )
    {
        _initialized = true;

        if ( run )
        {
            m_pulseGenerator.AllowPeriodicPulses();
            m_mutex.Unlock();

            m_pulseGenerator.RequestPeriodicPulses();

            // Unless the pulse drive is overridden we will now start the loop
            if ( m_pulseGenerator.GetPulseGeneratorDriver() == &m_busyWaitPulseDriver )
            {
                m_busyWaitPulseDriver.Run( m_pulseGenerator );
            }
        }
        else
        {
            m_mutex.Unlock();
        }
    }
    else
    {
        m_mutex.Unlock();
    }

    delete []data.argv;
    return 0;
}
#endif

/*-------------------------------------------------------------------------*/

int
CGUCEFApplication::main( int argc    ,
                         char** argv ,
                         bool run    )
{GUCEF_TRACE;

    /*
     *      ensure that we have an instance of this class
     */
    m_mutex.Lock();

    /*
     *      Set the given values as environment vars
     */
    if ( argc > 0 )
    {
        char intstr[ 10 ];
        sprintf( intstr, "%d", argc );
        GUCEFSetEnv( "argc", intstr );
        GUCEFSetEnv( "argv", (char*)argv );
    }

    _initialized = false;

    // Set shutdown to false before we emit the init event so that observers can
    // request a shutdown if something went wrong
    m_shutdownRequested = false;

    /*
     *      We now know we have an instance of this singleton and can begin
     *      our main() code. We will send the application initialization
     *      event to all event clients. The following code segment is a
     *      special case since it may be followed by the main application
     *      loop which would keep anything statically allocated here in memory
     *      Thus we turn the following into a compound statement.
     */
    {
        struct SAppInitEventData data;

        data.argc = argc;
        data.argv = argv;

        TAppInitEventData cloneableData( data );
        if ( !NotifyObservers( AppInitEvent, &cloneableData ) ) return 0;
    }

    // Check if we need to abort because a shutdown was requested
    if ( !m_shutdownRequested )
    {
        _initialized = true;

        if ( run )
        {
            m_pulseGenerator.AllowPeriodicPulses();
            m_mutex.Unlock();

            m_pulseGenerator.RequestPeriodicPulses();

            // Unless the pulse drive is overridden we will now start the loop
            if ( m_pulseGenerator.GetPulseGeneratorDriver() == &m_busyWaitPulseDriver )
            {
                m_busyWaitPulseDriver.Run( m_pulseGenerator );
            }
        }
        else
        {
            m_mutex.Unlock();
        }
    }
    else
    {
        m_mutex.Unlock();
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::RegisterEvents( void )
{GUCEF_TRACE;

    AppInitEvent.Initialize();
    AppShutdownEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::Update( void )
{GUCEF_TRACE;

    m_pulseGenerator.RequestPulse();
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::Stop( void )
{GUCEF_TRACE;

    LockData();

    if ( !m_shutdownRequested )
    {
        m_shutdownRequested = true;
        m_pulseGenerator.ForceStopOfPeriodicPulses();
        if ( !NotifyObservers( AppShutdownEvent ) ) return;
    }

    UnlockData();
}

/*-------------------------------------------------------------------------*/

CString
CGUCEFApplication::GetApplicationDir( void ) const
{GUCEF_TRACE;
        return _appdir;
}

/*-------------------------------------------------------------------------*/

bool
CGUCEFApplication::SaveConfig( CDataNode& tree )
{GUCEF_TRACE;

    LockData();

    CDataNode* n = tree.Structure( "GUCEF%CORE%CGUCEFApplication" ,
                                   '%'                            );
    n->SetAttribute( "appdir" ,
                     _appdir  );

    UnlockData();
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUCEFApplication::LoadConfig( const CDataNode& tree )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "CGUCEFApplication: Loading config" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUCEFApplication::OnSysConsoleCommand( const CString& path                ,
                                        const CString& command             ,
                                        const std::vector< CString >& args ,
                                        std::vector< CString >& resultdata )
{GUCEF_TRACE;

    LockData();

    if ( command == "Stop" )
    {
            Stop();
            UnlockData();
            return true;
    }
    else
    if ( command == "GetApplicationDir" )
    {
            resultdata.push_back( GetApplicationDir() );
            UnlockData();
            return true;
    }
    UnlockData();
    return false;
}

/*-------------------------------------------------------------------------*/
  /*
void
CGUCEFApplication::OnUpdate( const UInt64 tickcount               ,
                             const Float64 updateDeltaInMilliSecs )
{GUCEF_TRACE;

        #ifdef GUCEF_MSWIN_BUILD
    	MSG  msg;
        while( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
                ::TranslateMessage( &msg );
                ::DispatchMessage( &msg );
        }
        #endif /* GUCEF_MSWIN_BUILD ? *//*

        if ( updateDeltaInMilliSecs <= m_minimalCycleDeltaInMilliSecs )
        {
                /*
                 *      Not much action ATM, so we should
                 *      avoid hogging the system resources
                 *
                MT::PrecisionDelay( m_cycleDelayInMilliSecs );
        }
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::LockData( void )
{GUCEF_TRACE;

    m_mutex.Lock();
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::UnlockData( void )
{GUCEF_TRACE;

    m_mutex.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
