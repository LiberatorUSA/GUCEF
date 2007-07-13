/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */ 
#define DVCPPSTRINGUTILS_H
#endif /* DVCPPSTRINGUTILS_H ? */

#ifndef CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define CGUCEFAPPLICATION_H
#endif /* CGUCEFAPPLICATION_H ? */

#ifndef CIINPUTDRIVER_H
#include "CIInputDriver.h"
#define CIINPUTDRIVER_H
#endif /* CIINPUTDRIVER_H ? */

#ifndef CINPUTDRIVERPLUGIN_H
#include "CInputDriverPlugin.h"
#define CINPUTDRIVERPLUGIN_H
#endif /* CINPUTDRIVERPLUGIN_H ? */

#include "CInputController.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CInputController::InputDriverLoadedEvent = "GUCEF::INPUT::InputDriverLoadedEvent";
const CORE::CEvent CInputController::InputDriverUnloadedEvent = "GUCEF::INPUT::InputDriverUnloadedEvent";
CInputController* CInputController::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputController::CInputController( void )
        : m_driverisplugin( false ) ,
          m_driver( NULL )
          
          #ifdef GUCEF_MSWIN_BUILD
          ,
          m_hinstance(0UL)
          #endif
{TRACE;

        RegisterEvents();
        CORE::CGUCEFApplication::Instance();    
}

/*-------------------------------------------------------------------------*/

CInputController::CInputController( const CInputController& src )
{TRACE;
        assert( 0 );
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CInputController::~CInputController()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CInputController&
CInputController::operator=( const CInputController& src )
{TRACE;
        assert( 0 );
        /* dummy, do not use */
        
        return *this;
}

/*-------------------------------------------------------------------------*/

CInputController* 
CInputController::Instance( void )
{TRACE;
        if ( !m_instance )
        {
                m_instance = new CInputController();
        }
        return m_instance;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputController::Deinstance( void )
{TRACE;
        delete m_instance;
        m_instance = NULL;
}

/*-------------------------------------------------------------------------*/
        
CInputContext* 
CInputController::CreateContext( const CORE::CValueList& params       ,
                                 CIInputHandler* handler /* = NULL */ )
{TRACE;
        if ( m_driver )
        {
                #ifdef GUCEF_MSWIN_BUILD
                
                CORE::CValueList extraparams( params );
                CORE::CString hinststr;
                hinststr.SetInt( m_hinstance );
                extraparams.Set( "HINSTANCE", hinststr );
                
                CInputContext* context = m_driver->CreateContext( extraparams, handler );
                
                #else
                
                CInputContext* context = m_driver->CreateContext( params, handler );
                
                #endif
                
                if ( context )
                {
                        context->SetID( m_contextlist.AddEntry( context ) );
                        return context;
                }
        }
        return NULL;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputController::DestroyContext( CInputContext* context )
{TRACE;
        if ( m_driver )
        {
                m_contextlist.RemoveEntry( context->GetID() );
                
                m_driver->DeleteContext( context );
        }
        assert( 0 );
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CInputController::GetContextCount( void ) const
{TRACE;
        return m_contextlist.GetCount();
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputController::SetDriver( CIInputDriver* driver )
{TRACE;
        if ( 0 == m_contextlist.GetCount() )
        {
                UnloadDriverModule();
                
                m_driver = driver;
                m_driverisplugin = false;
                NotifyObservers( InputDriverLoadedEvent );
                return true;
        }
        return false;
}
        
/*-------------------------------------------------------------------------*/

const CIInputDriver* 
CInputController::GetDriver( void ) const
{TRACE;
        return m_driver;
}
                
/*-------------------------------------------------------------------------*/

bool 
CInputController::LoadDriverModule( const CORE::CString& filename  ,
                                    const CORE::CValueList& params )
{TRACE;
        CORE::CString path = CORE::RelativePath( filename );
        
        CInputDriverPlugin* plugin = new CInputDriverPlugin();
        if ( plugin->LoadModule( path, params ) )
        {
                if ( SetDriver( plugin ) )
                {
                        m_driverisplugin = true;
                        NotifyObservers( InputDriverLoadedEvent );
                        return true;
                }
        }
        delete plugin;
        return false;
}
        
/*-------------------------------------------------------------------------*/

void 
CInputController::UnloadDriverModule( void )
{TRACE;
        if ( m_driverisplugin && m_driver )
        {
                CInputDriverPlugin* plugin = static_cast<CInputDriverPlugin*>( m_driver );
                plugin->UnloadModule();
                
                m_driver = NULL;
                delete plugin;
                
                NotifyObservers( InputDriverUnloadedEvent );
        }
}

/*-------------------------------------------------------------------------*/

void 
CInputController::OnUpdate( const UInt64 tickcount               ,
                            const Float64 updateDeltaInMilliSecs )
{TRACE;
        for ( UInt32 i=0; i<m_contextlist.GetCount(); ++i )
        {
                #ifdef DEBUG_MODE

                if ( !m_driver->OnUpdate( tickcount                                         ,
                                          updateDeltaInMilliSecs                            ,
                                          static_cast<CInputContext*>( m_contextlist[ i ] ) ) )
                {
                        DEBUGOUTPUTsi( "CInputController::OnUpdate() Failed on context ", i );
                }                                          
                
                #else
                
                m_driver->OnUpdate( tickcount                                         ,
                                    updateDeltaInMilliSecs                            ,
                                    static_cast<CInputContext*>( m_contextlist[ i ] ) );

                #endif                                    
        }                                                                
}

/*-------------------------------------------------------------------------*/

void
CInputController::OnNotify( CORE::CNotifier* notifier                 ,
                            const CORE::CEvent& eventid               ,
                            CORE::CICloneable* eventdata /* = NULL */ )
{TRACE;
        #ifdef GUCEF_MSWIN_BUILD
        if ( eventid == CORE::CGUCEFApplication::AppInitEvent )
        {
                CORE::CGUCEFApplication::TAppInitEventData* initData = static_cast< CORE::CGUCEFApplication::TAppInitEventData* >( eventdata );
                #pragma warning( disable: 4311 ) // pointer truncation warning
                m_hinstance = reinterpret_cast<UInt32>( initData->GetData().hinstance );
        }
        #endif /* GUCEF_MSWIN_BUILD ? */
}

/*-------------------------------------------------------------------------*/

void
CInputController::RegisterEvents( void )
{TRACE;
    
    InputDriverLoadedEvent.Initialize();
    InputDriverUnloadedEvent.Initialize();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
