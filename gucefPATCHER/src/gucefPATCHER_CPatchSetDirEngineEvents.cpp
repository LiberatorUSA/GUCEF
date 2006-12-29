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
{TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPatchSetDirEngineEvents::~CPatchSetDirEngineEvents()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchSetDirEngineEvents::RegisterEvents( void )
{TRACE;

    DirProcessingStartedEvent.Initialize();
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