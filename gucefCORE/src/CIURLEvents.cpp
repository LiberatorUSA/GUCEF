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

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#include "CIURLEvents.h"

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

const CEvent CIURLEvents::URLActivateEvent = "GUCEF::CORE::CIURLEvents::URLActivateEvent";
const CEvent CIURLEvents::URLDeactivateEvent = "GUCEF::CORE::CIURLEvents::URLDeactivateEvent";
const CEvent CIURLEvents::URLDataRecievedEvent = "GUCEF::CORE::CIURLEvents::URLDataRecievedEvent";
const CEvent CIURLEvents::URLAllDataRecievedEvent = "GUCEF::CORE::CIURLEvents::URLAllDataRecievedEvent";
const CEvent CIURLEvents::URLDataRetrievalErrorEvent = "GUCEF::CORE::CIURLEvents::URLDataRetrievalErrorEvent";


/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CIURLEvents::CIURLEvents( void )
{TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CIURLEvents::CIURLEvents( const CIURLEvents& src )
{TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CIURLEvents::~CIURLEvents()
{TRACE;
}

/*-------------------------------------------------------------------------*/

void
CIURLEvents::RegisterEvents( void )
{TRACE;

    URLActivateEvent.Initialize();
    URLDeactivateEvent.Initialize();
    URLDataRecievedEvent.Initialize();
    URLAllDataRecievedEvent.Initialize();
    URLDataRetrievalErrorEvent.Initialize();
}
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/