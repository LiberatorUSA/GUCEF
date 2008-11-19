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
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CIURLEvents::CIURLEvents( const CIURLEvents& src )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CIURLEvents::~CIURLEvents()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

void
CIURLEvents::RegisterEvents( void )
{GUCEF_TRACE;

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