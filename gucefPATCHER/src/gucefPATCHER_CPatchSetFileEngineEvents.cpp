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
const CORE::CEvent CPatchSetFileEngineEvents::FileRetrievalStartedEvent = "GUCEF::PATCHER::CPatchSetFileEngineEvents::FileRetrievalStartedEvent";
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
{TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPatchSetFileEngineEvents::~CPatchSetFileEngineEvents()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchSetFileEngineEvents::RegisterEvents( void )
{TRACE;

    FileListProcessingStartedEvent.Initialize();
    LocalFileIsOKEvent.Initialize();
    LocalFileSizeMismatchEvent.Initialize();
    LocalFileHashMismatchEvent.Initialize();
    LocalFileNotFoundEvent.Initialize();
    LocalFileReplacedEvent.Initialize();
    FileRetrievalStartedEvent.Initialize();
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