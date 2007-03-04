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

#include "gucefPATCHER_CPatchSetEngineEvents.h"

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

const CORE::CEvent CPatchSetEngineEvents::PatchSetProcessingStartedEvent = "GUCEF::PATCHER::CPatchSetEngineEvents::PatchSetProcessingStartedEvent";
const CORE::CEvent CPatchSetEngineEvents::PatchSetProcessingCompletedEvent = "GUCEF::PATCHER::CPatchSetEngineEvents::PatchSetProcessingCompletedEvent";
const CORE::CEvent CPatchSetEngineEvents::PatchSetProcessingAbortedEvent = "GUCEF::PATCHER::CPatchSetEngineEvents::PatchSetProcessingAbortedEvent";
const CORE::CEvent CPatchSetEngineEvents::PatchSetProcessingFailedEvent = "GUCEF::PATCHER::CPatchSetEngineEvents::PatchSetProcessingFailedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetEngineEvents::CPatchSetEngineEvents( void )
{TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPatchSetEngineEvents::~CPatchSetEngineEvents()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchSetEngineEvents::RegisterEvents( void )
{TRACE;

    PatchSetProcessingStartedEvent.Initialize();
    PatchSetProcessingCompletedEvent.Initialize();
    PatchSetProcessingAbortedEvent.Initialize();
    PatchSetProcessingFailedEvent.Initialize();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/