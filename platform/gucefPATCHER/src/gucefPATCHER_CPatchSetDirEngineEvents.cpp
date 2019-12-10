/*
 *  gucefPATCHER: GUCEF RAD module providing a patch delivery system
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

#include "gucefPATCHER_CPatchSetDirEngineEvents.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CPatchSetDirEngineEvents::DirProcessingStartedEvent = "GUCEF::PATCHER::CPatchSetDirEngineEvents::DirProcessingStartedEvent";
const CORE::CEvent CPatchSetDirEngineEvents::DirProcessingProgressEvent = "GUCEF::PATCHER::CPatchSetDirEngineEvents::DirProcessingProgressEvent";
const CORE::CEvent CPatchSetDirEngineEvents::SubDirProcessingStartedEvent = "GUCEF::PATCHER::CPatchSetDirEngineEvents::SubDirProcessingStartedEvent";
const CORE::CEvent CPatchSetDirEngineEvents::SubDirProcessingCompletedEvent = "GUCEF::PATCHER::CPatchSetDirEngineEvents::SubDirProcessingCompletedEvent";
const CORE::CEvent CPatchSetDirEngineEvents::DirProcessingCompletedEvent = "GUCEF::PATCHER::CPatchSetDirEngineEvents::DirProcessingCompletedEvent";
const CORE::CEvent CPatchSetDirEngineEvents::DirProcessingAbortedEvent = "GUCEF::PATCHER::CPatchSetDirEngineEvents::DirProcessingAbortedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetDirEngineEvents::CPatchSetDirEngineEvents( void )
{GUCEF_TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPatchSetDirEngineEvents::~CPatchSetDirEngineEvents()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchSetDirEngineEvents::RegisterEvents( void )
{GUCEF_TRACE;

    DirProcessingStartedEvent.Initialize();
    DirProcessingProgressEvent.Initialize();
    SubDirProcessingStartedEvent.Initialize();
    SubDirProcessingCompletedEvent.Initialize();
    DirProcessingCompletedEvent.Initialize();
    DirProcessingAbortedEvent.Initialize();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/