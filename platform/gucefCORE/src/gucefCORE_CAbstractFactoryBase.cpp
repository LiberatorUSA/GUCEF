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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "gucefCORE_CAbstractFactoryBase.h"

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

const CEvent CAbstractFactoryBase::ConcreteFactoryRegisteredEvent = "GUCEF::CORE::CAbstractFactoryBase::ConcreteFactoryRegisteredEvent";
const CEvent CAbstractFactoryBase::ConcreteFactoryUnregisteredEvent = "GUCEF::CORE::CAbstractFactoryBase::ConcreteFactoryUnregisteredEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
    
void
CAbstractFactoryBase::RegisterEvents( void )
{GUCEF_TRACE;

    ConcreteFactoryRegisteredEvent.Initialize();
    ConcreteFactoryUnregisteredEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CAbstractFactoryBase::CAbstractFactoryBase( bool useEventing )
    : CNotifier( useEventing )
    , m_useEventing( useEventing )
{GUCEF_TRACE;

    if ( useEventing )
        RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CAbstractFactoryBase::~CAbstractFactoryBase()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
