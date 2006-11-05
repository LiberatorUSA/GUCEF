/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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
{TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CNotifyingMapEvents::CNotifyingMapEvents( const CNotifyingMapEvents& src )
{TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CNotifyingMapEvents::~CNotifyingMapEvents()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CNotifyingMapEvents&
CNotifyingMapEvents::operator=( const CNotifyingMapEvents& src )
{TRACE;
    
    if ( &src != this )
    {
        
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CNotifyingMapEvents::RegisterEvents( void )
{TRACE;

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