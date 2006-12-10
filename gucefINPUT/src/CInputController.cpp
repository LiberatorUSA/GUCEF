/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef CEVENTTYPEREGISTRY_H
#include "CEventTypeRegistry.h"
#define CEVENTTYPEREGISTRY_H
#endif /* CEVENTTYPEREGISTRY_H ? */

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
CInputController::OnUpdate( UInt32 tickcount  ,
                            UInt32 deltaticks )
{TRACE;
        for ( UInt32 i=0; i<m_contextlist.GetCount(); ++i )
        {
                #ifdef DEBUG_MODE

                if ( !m_driver->OnUpdate( tickcount                                         ,
                                          deltaticks                                        ,
                                          static_cast<CInputContext*>( m_contextlist[ i ] ) ) )
                {
                        DEBUGOUTPUTsi( "CInputController::OnUpdate() Failed on context ", i );
                }                                          
                
                #else
                
                m_driver->OnUpdate( tickcount                                         ,
                                    deltaticks                                        ,
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
