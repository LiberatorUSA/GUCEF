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

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"     /* global plugin manager control center */
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#include "CNotificationIDRegistry.h"

#include "CPluginManager.h"    /* definition of the class implemented here */

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

const CEvent CPluginManager::PluginLoadedEvent = "GUCEF::CORE::CPluginManager::PluginLoadedEvent";
const CEvent CPluginManager::PluginUnloadedEvent = "GUCEF::CORE::CPluginManager::PluginUnloadedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Generic interface for plugin managers
 */
CPluginManager::CPluginManager( void )
    : CObservingNotifier()
{TRACE;

    RegisterEvents();       
}

/*-------------------------------------------------------------------------*/

CPluginManager::CPluginManager( const CPluginManager& src )
    : CObservingNotifier( src )
{TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/
        
CPluginManager::~CPluginManager()
{TRACE;
    
    CPluginControl::Instance()->Unregister( this );
}

/*-------------------------------------------------------------------------*/

CString
CPluginManager::GetPluginDir( void ) const
{TRACE;
    
    return CPluginControl::Instance()->GetPluginDir();
}

/*-------------------------------------------------------------------------*/

void 
CPluginManager::OnSetPluginDir( const CString& path )
{TRACE;
      
}

/*-------------------------------------------------------------------------*/

void
CPluginManager::RegisterEvents( void )
{TRACE;
        
    PluginLoadedEvent.Initialize();
    PluginUnloadedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void
CPluginManager::OnNotify( CNotifier* notifier                 ,
                          const UInt32 eventid                ,
                          CICloneable* eventdata /* = NULL */ )
{TRACE;

    /* dummy to avoid mandatory implementation in the descending class */
}
        
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
