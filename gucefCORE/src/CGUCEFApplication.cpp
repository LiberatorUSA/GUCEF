/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "dvoswrap.h"           /* needed for GUCEFPrecisionTimerInit() ect. */
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

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

#ifndef GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#include "CGUCEFAppSubSystem.h"
#define GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#endif /* GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H ? */

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef GUCEF_CORE_CIGUCEFAPPLICATIONDRIVER_H
#include "CIGUCEFApplicationDriver.h"
#define GUCEF_CORE_CIGUCEFAPPLICATIONDRIVER_H
#endif /* GUCEF_CORE_CIGUCEFAPPLICATIONDRIVER_H ? */

#ifndef GUCEF_CORE_COBSERVERPUMP_H
#include "CObserverPump.h"
#define GUCEF_CORE_COBSERVERPUMP_H
#endif /* GUCEF_CORE_COBSERVERPUMP_H ? */

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

CGUCEFApplication* CGUCEFApplication::_instance = NULL;
MT::CMutex CGUCEFApplication::m_mutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUCEFApplication*
CGUCEFApplication::Instance( void )
{TRACE;
        m_mutex.Lock();
        if ( !_instance )
        {                
                 /*
                  *     Instantiate the GUCEF application class
                  */
                _instance = new CGUCEFApplication();
                CHECKMEM( _instance, sizeof( CGUCEFApplication ) );
                
                /*
                 *  We now register some sub-systems on purpose here.
                 *  These are normally always registered but cannot auto-register
                 *  because they are part of the construction of classes used in the
                 *  CGUCEFApplication as well. This would cause a endless recursive loop.
                 *  This is solved by doing it manually here
                 */
                CObserverPump::Instance()->RegisterSubSystem();
                RegisterEvents();                 
        }
        m_mutex.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CGUCEFApplication::Deinstance( void )
{TRACE;
        m_mutex.Lock();
        CHECKMEM( _instance, sizeof( CGUCEFApplication ) );
        delete _instance;
        _instance = NULL;
        m_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

CGUCEFApplication::CGUCEFApplication( void )
        : CIConfigurable( true )                      ,
          _active( false )                            ,
          _initialized( false )                       ,
          m_maxdeltaticksfordelay( 0 )                ,
          m_delaytime( 10 )                           ,
          m_minimalUpdateDelta( GUCEFCORE_UINT32MAX ) ,
          m_requiresPeriodicUpdates( false )          ,
          m_inNeedOfAnUpdate( false )
{TRACE;

        /*
         *      Initialize high-resolution timing
         */
        GUCEFPrecisionTimerInit();
                                            
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
        CStringList args;
        sysconsole->RegisterCmd( "GUCEF\\CORE\\CGUCEFApplication" ,
                                 "Stop"                           ,
                                 args                             ,
                                 this                             );
        sysconsole->RegisterCmd( "GUCEF\\CORE\\CGUCEFApplication" ,
                                 "GetApplicationDir"              ,
                                 args                             ,
                                 this                             );                                                  
}

/*-------------------------------------------------------------------------*/

CGUCEFApplication::~CGUCEFApplication()
{TRACE;
        GUCEFPrecisionTimerShutdown();
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
int
CGUCEFApplication::Main( HINSTANCE hinstance     ,
                         LPSTR lpcmdline         ,
                         int ncmdshow            ,
                         bool run                )
{
        GUCEF_BEGIN;
        
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
                 *      Set the given values as enviornment vars
                 */
                #pragma warning( disable: 4311 ) 
                char intstr[ 10 ];
                sprintf( intstr, "%d", (INTPTR)hinstance );  
                GUCEFSetEnv( "HINSTANCE", intstr );                 
                
                char apppath[ MAX_PATH ]; 
                if ( GetModuleFileName( hinstance, apppath, MAX_PATH ) )
                {
                        _appdir = apppath;
                }                        
        }                

        /*
         *      We now know we have an instance of this singleton and can begin
         *      our main() code. We will send the application initialization
         *      event to all event clients. The following code segment is a
         *      special case since it may be followed by the main application
         *      loop which would keep anything staticly allocated here in memory
         *      Thus we turn the following into a compound statement.
         */
        {
                struct SAppInitEventData data;
                
                data.hinstance = hinstance;
                data.lpcmdline = lpcmdline;
                data.ncmdshow = ncmdshow;

                data.argc = 0;
                data.argv = NULL;
                
                TAppInitEventData cloneableData( data );                
                NotifyObservers( AppInitEvent, &cloneableData );
        }

        _initialized = true;
        m_mutex.Unlock();
        if ( run )
        {
                Run();
        }
        return 0;      
}
#endif

/*-------------------------------------------------------------------------*/

int 
CGUCEFApplication::main( int argc    ,
                         char** argv ,
                         bool run    )
{TRACE;
        
        /*
         *      ensure that we have an instance of this class
         */
        m_mutex.Lock(); 
        if ( !_instance )
        {
                int retval = Instance()->main( argc ,
                                               argv ,
                                               run  );                                               
                return retval;
        }
        
        {
                /*
                 *      Set the given values as environment vars
                 */
                char intstr[ 10 ];
                sprintf( intstr, "%d", argc );  
                GUCEFSetEnv( "argc", intstr );
                GUCEFSetEnv( "argv", (char*)argv );                 
        }

        /*
         *      We now know we have an instance of this singleton and can begin
         *      our main() code. We will send the application initialization
         *      event to all event clients. The following code segment is a
         *      special case since it may be followed by the main application
         *      loop which would keep anything staticly allocated here in memory
         *      Thus we turn the following into a compound statement.
         */
        {
                struct SAppInitEventData data;
                
                data.argc = argc;
                data.argv = argv;
                
                TAppInitEventData cloneableData( data );
                NotifyObservers( AppInitEvent, &cloneableData );
        }

        _initialized = true;
        m_mutex.Unlock();
        if ( run )
        {
                Run();
        }
        return 0;      
}                        

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::RegisterEvents( void )
{TRACE;
    
    AppInitEvent.Initialize();
    AppShutdownEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::Update( void )
{TRACE;

    LockData();
    
    UInt32 tickCount( GUCEFGetTickCount() );
    UInt32 deltaTicks( tickCount - m_appTickCount );
    
    TSubSystemList::iterator i = m_subSysList.begin();
    while ( i != m_subSysList.end() )
    {
        (*i)->OnUpdate( tickCount  ,
                        deltaTicks );
        ++i;
    }
    m_appTickCount = tickCount;
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::SetApplicationDriver( CIGUCEFApplicationDriver* appDriver )
{TRACE;

    m_appDriver = appDriver;
}

/*-------------------------------------------------------------------------*/
        
CIGUCEFApplicationDriver*
CGUCEFApplication::GetApplicationDriver( void ) const
{TRACE;
    
    return m_appDriver;
}

/*-------------------------------------------------------------------------*/

bool
CGUCEFApplication::GetRequiresPeriodicUpdate( void ) const
{TRACE;

    return m_requiresPeriodicUpdates;
}

/*-------------------------------------------------------------------------*/

UInt32
CGUCEFApplication::GetMinimalReqUpdateResolution( void ) const
{TRACE;

    return m_minimalUpdateDelta;
}

/*-------------------------------------------------------------------------*/

UInt32
CGUCEFApplication::GetLastUpdateTickCount( void ) const
{TRACE;

    return m_appTickCount;    
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::Run( void )
{TRACE;

        _active = true;
        
        while ( _active )
        {
                Update();                                
        }
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::Stop( void )
{TRACE;
    if ( _active )
    {
        _active = false;
        NotifyObservers( AppShutdownEvent );
    }
}

/*-------------------------------------------------------------------------*/

void 
CGUCEFApplication::SetPluginDir( const CString& plugindir )
{TRACE;
        CPluginControl* pc = CPluginControl::Instance();
        pc->SetPluginDir( plugindir );
        pc->LoadAll();
}

/*-------------------------------------------------------------------------*/
                       
CString 
CGUCEFApplication::GetPluginDir( void ) const
{TRACE;
        return CPluginControl::Instance()->GetPluginDir();
}

/*-------------------------------------------------------------------------*/
        
CString 
CGUCEFApplication::GetApplicationDir( void ) const
{TRACE;
        return _appdir;
}

/*-------------------------------------------------------------------------*/

bool 
CGUCEFApplication::SaveConfig( CDataNode& tree )
{TRACE;
        CDataNode* n = tree.Structure( "GUCEF%CORE%CGUCEFApplication" ,
                                       '%'                            );                     
        n->SetAttribute( "appdir" ,
                         _appdir  );                        
        return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGUCEFApplication::LoadConfig( const CDataNode& tree )
{TRACE;
        return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGUCEFApplication::OnSysConsoleCommand( const CString& path     ,
                                        const CString& command  ,
                                        const CStringList& args ,
                                        CStringList& resultdata )
{TRACE;
        if ( command == "Stop" )
        {
                Stop();
                return true;        
        }
        else
        if ( command == "GetApplicationDir" )
        {
                resultdata.Append( GetApplicationDir() );
                return true;
        }
        return false;
}

/*-------------------------------------------------------------------------*/

void 
CGUCEFApplication::OnUpdate( UInt32 tickcount  ,
                             UInt32 deltaticks )
{TRACE;
        
        #ifdef GUCEF_MSWIN_BUILD
    	MSG  msg;
        while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {                
                TranslateMessage( &msg );
                DispatchMessage( &msg );
        }
        #endif /* WIN32_BUILD ? */        
        
        if ( deltaticks <= m_maxdeltaticksfordelay )
        {
                /*
                 *      Not much action atm, so we should
                 *      avoid hogging the CPU 
                 */
                GUCEFPrecisionDelay( m_delaytime );                
        }
}

/*-------------------------------------------------------------------------*/                             

void 
CGUCEFApplication::SetCycleDelay( UInt32 maxdeltaticks ,
                                  UInt32 delay         )
{TRACE;
    m_maxdeltaticksfordelay = maxdeltaticks;
    m_delaytime = delay;
}

/*-------------------------------------------------------------------------*/

void 
CGUCEFApplication::GetCycleDelay( UInt32& maxdeltaticks ,
                                  UInt32& delay         ) const
{TRACE;
    maxdeltaticks = m_maxdeltaticksfordelay;
    delay = m_delaytime;
}                                  

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::LockData( void )
{TRACE;

    m_mutex.Lock();
}

/*-------------------------------------------------------------------------*/
        
void
CGUCEFApplication::UnlockData( void )
{TRACE;

    m_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::RegisterSubSystem( CGUCEFAppSubSystem* subSystem )
{TRACE;

    assert( NULL != subSystem );
    m_subSysList.insert( subSystem );    
    Subscribe( &subSystem->AsObserver() );
}

/*-------------------------------------------------------------------------*/
        
void
CGUCEFApplication::UnregisterSubSystem( CGUCEFAppSubSystem* subSystem )
{TRACE;

    assert( NULL != subSystem );
    m_subSysList.erase( subSystem );    
    Unsubscribe( &subSystem->AsObserver() );
}

/*-------------------------------------------------------------------------*/
        
void
CGUCEFApplication::RefreshMinimalSubSysInterval( void )
{TRACE;
    
    LockData();
    
    UInt32 temp( 0 );
    
    TSubSystemList::iterator i = m_subSysList.begin();
    while ( i != m_subSysList.end() )
    {
        temp = (*i)->GetDesiredUpdateInterval();
        if ( temp < m_minimalUpdateDelta )
        {
            m_minimalUpdateDelta = temp;
        }
        ++i;
    }

    // request new update minimum from driver    
    if ( NULL != m_appDriver )
    {
        m_appDriver->OnRequestNewMinimalUpdateFreq( m_minimalUpdateDelta );
    }
    UnlockData();    
}

/*-------------------------------------------------------------------------*/
        
void
CGUCEFApplication::RefreshPeriodicUpdateRequirement( void )
{TRACE;
    
    LockData();
    
    TSubSystemList::iterator i = m_subSysList.begin();
    while ( i != m_subSysList.end() )
    {
        if ( (*i)->ArePeriodicUpdatesRequired() )
        {
            if ( !m_requiresPeriodicUpdates )
            {
                m_requiresPeriodicUpdates = true;
                                    
                // notify driver of change                
                if ( NULL != m_appDriver )
                {
                    m_appDriver->OnSwitchUpdateMethod( true );
                }
            }
            UnlockData();
            return;
        }
        ++i;
    }
    
    if ( m_requiresPeriodicUpdates )
    {
        // This is not true anymore
        m_requiresPeriodicUpdates = false;
        
        // notify driver of change                
        if ( NULL != m_appDriver )
        {
            m_appDriver->OnSwitchUpdateMethod( false );
        }        
    }

    UnlockData();
}

/*-------------------------------------------------------------------------*/
        
void
CGUCEFApplication::DoRequestSubSysUpdate( void )
{TRACE;
    
    LockData();
    
    if ( !m_inNeedOfAnUpdate )
    {
        m_inNeedOfAnUpdate = true;
        UnlockData();
        
        // ask driver for update
        if ( NULL != m_appDriver )
        {
            m_appDriver->OnRequestNewUpdateCycle();
        }        
        return;
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CGUCEFApplication::OnPumpedNotify( CNotifier* notifier                 ,
                                   const CEvent& eventid               ,
                                   CICloneable* eventdata /* = NULL */ )
{TRACE;
    // nothing to do here atm
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
