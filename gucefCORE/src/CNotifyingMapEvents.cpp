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

#include "CNotifyingMapEvents.h"

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

const CEvent CNotifyingMapEvents::ItemErasedEvent = "GUCEF::CORE::CNotifyingMapEvents::ItemErasedEvent";
const CEvent CNotifyingMapEvents::ItemAddedEvent = "GUCEF::CORE::CNotifyingMapEvents::ItemAddedEvent";
const CEvent CNotifyingMapEvents::ItemModifiedEvent = "GUCEF::CORE::CNotifyingMapEvents::ItemModifiedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CNotifyingMapEvents::CNotifyingMapEvents( void )
{GUCEF_TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CNotifyingMapEvents::CNotifyingMapEvents( const CNotifyingMapEvents& src )
{GUCEF_TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CNotifyingMapEvents::~CNotifyingMapEvents()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CNotifyingMapEvents&
CNotifyingMapEvents::operator=( const CNotifyingMapEvents& src )
{GUCEF_TRACE;
    
    if ( &src != this )
    {
        
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CNotifyingMapEvents::RegisterEvents( void )
{GUCEF_TRACE;

    ItemErasedEvent.Initialize();
    ItemAddedEvent.Initialize();
    ItemModifiedEvent.Initialize();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/