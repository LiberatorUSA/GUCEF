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
{TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPatchListEngineEvents::~CPatchListEngineEvents()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchListEngineEvents::RegisterEvents( void )
{TRACE;

    PatchListProcessingStartedEvent.Initialize();
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