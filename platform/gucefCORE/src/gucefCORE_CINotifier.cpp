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

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#include "gucefCORE_CINotifier.h"

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

const CEvent CINotifier::SubscribeEvent = "GUCEF::CORE::CINotifier::SubscribeEvent";
const CEvent CINotifier::UnsubscribeEvent = "GUCEF::CORE::CINotifier::UnsubscribeEvent";
const CEvent CINotifier::ModifyEvent = "GUCEF::CORE::CINotifier::ModifyEvent";
const CEvent CINotifier::DestructionEvent = "GUCEF::CORE::CINotifier::DestructionEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CINotifier::CINotifier( bool registerStdEvents )
    : MT::CILockable()
    , CITypeNamed() 
{GUCEF_TRACE;

    if ( registerStdEvents )
        RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CINotifier::CINotifier( const CINotifier& src )
    : MT::CILockable( src )
    , CITypeNamed( src ) 
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CINotifier&
CINotifier::operator=( const CINotifier& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        MT::CILockable::operator=( src );
        CITypeNamed::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CINotifier::~CINotifier()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CINotifier::RegisterEvents( void )
{GUCEF_TRACE;

    ModifyEvent.Initialize();
    DestructionEvent.Initialize();
    SubscribeEvent.Initialize();
    UnsubscribeEvent.Initialize();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
