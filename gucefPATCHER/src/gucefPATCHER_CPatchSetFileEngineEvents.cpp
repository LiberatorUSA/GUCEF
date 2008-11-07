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

#include "gucefPATCHER_CPatchSetFileEngineEvents.h"

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

const CORE::CEvent CPatchSetFileEngineEvents::FileListProcessingStartedEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileListProcessingStartedEvent";
const CORE::CEvent CPatchSetFileEngineEvents::LocalFileIsOKEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::LocalFileIsOKEvent";
const CORE::CEvent CPatchSetFileEngineEvents::LocalFileSizeMismatchEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::LocalFileSizeMismatchEvent";
const CORE::CEvent CPatchSetFileEngineEvents::LocalFileHashMismatchEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::LocalFileHashMismatchEvent";
const CORE::CEvent CPatchSetFileEngineEvents::LocalFileNotFoundEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::LocalFileNotFoundEvent";
const CORE::CEvent CPatchSetFileEngineEvents::LocalFileReplacedEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::LocalFileReplacedEvent";
const CORE::CEvent CPatchSetFileEngineEvents::LocalFileReplacementFailure = "GUCEF::PATCHER::CPatchSetFileEngineEvents::LocalFileReplacementFailure";
const CORE::CEvent CPatchSetFileEngineEvents::FileRetrievalStartedEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileRetrievalStartedEvent";
const CORE::CEvent CPatchSetFileEngineEvents::FileRetrievalProgressEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileRetrievalProgressEvent";
const CORE::CEvent CPatchSetFileEngineEvents::FileRetrievalCompleteEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileRetrievalCompleteEvent";
const CORE::CEvent CPatchSetFileEngineEvents::FileRetrievalErrorEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileRetrievalErrorEvent";
const CORE::CEvent CPatchSetFileEngineEvents::FileStorageErrorEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileStorageErrorEvent";
const CORE::CEvent CPatchSetFileEngineEvents::FileListProcessingCompleteEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileListProcessingCompleteEvent";
const CORE::CEvent CPatchSetFileEngineEvents::FileListProcessingAbortedEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileListProcessingAbortedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetFileEngineEvents::CPatchSetFileEngineEvents( void )
{GUCEF_TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPatchSetFileEngineEvents::~CPatchSetFileEngineEvents()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchSetFileEngineEvents::RegisterEvents( void )
{GUCEF_TRACE;

    FileListProcessingStartedEvent.Initialize();
    LocalFileIsOKEvent.Initialize();
    LocalFileSizeMismatchEvent.Initialize();
    LocalFileHashMismatchEvent.Initialize();
    LocalFileNotFoundEvent.Initialize();
    LocalFileReplacedEvent.Initialize();
    LocalFileReplacementFailure.Initialize();
    FileRetrievalStartedEvent.Initialize();
    FileRetrievalProgressEvent.Initialize();
    FileRetrievalCompleteEvent.Initialize();
    FileRetrievalErrorEvent.Initialize();
    FileStorageErrorEvent.Initialize();
    FileListProcessingCompleteEvent.Initialize();
    FileListProcessingAbortedEvent.Initialize();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/