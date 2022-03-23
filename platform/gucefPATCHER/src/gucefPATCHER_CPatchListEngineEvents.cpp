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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#include "gucefPATCHER_CPatchListEngineEvents.h"

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

const CORE::CEvent CPatchListEngineEvents::PatchListProcessingStartedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchListProcessingStartedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchListProcessingProgressEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchListProcessingProgressEvent";
const CORE::CEvent CPatchListEngineEvents::PatchListProcessingCompletedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchListProcessingCompletedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchListProcessingAbortedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchListProcessingAbortedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchListProcessingFailedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchListProcessingFailedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchSetDataRecievedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchSetDataRecievedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchSetRetrievalStartedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchSetRetrievalStartedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchSetRetrievalCompletedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchSetRetrievalCompletedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchSetRetrievalFailedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchSetRetrievalFailedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchSetRetrievalAbortedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchSetRetrievalAbortedEvent";
const CORE::CEvent CPatchListEngineEvents::PatchSetDecodingFailedEvent = "GUCEF::PATCHER::CPatchListEngineEvents::PatchSetDecodingFailedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchListEngineEvents::CPatchListEngineEvents( void )
{GUCEF_TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPatchListEngineEvents::~CPatchListEngineEvents()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchListEngineEvents::RegisterEvents( void )
{GUCEF_TRACE;

    PatchListProcessingStartedEvent.Initialize();
    PatchListProcessingProgressEvent.Initialize();
    PatchListProcessingCompletedEvent.Initialize();
    PatchListProcessingAbortedEvent.Initialize();
    PatchListProcessingFailedEvent.Initialize();
    PatchSetDataRecievedEvent.Initialize();
    PatchSetRetrievalStartedEvent.Initialize();
    PatchSetRetrievalCompletedEvent.Initialize();
    PatchSetRetrievalFailedEvent.Initialize();
    PatchSetRetrievalAbortedEvent.Initialize();
    PatchSetDecodingFailedEvent.Initialize();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/