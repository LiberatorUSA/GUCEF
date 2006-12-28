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

#ifndef GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
#define GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CEvent.h"
#include "gucefPATCHER_macros.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class EXPORT_CPP CPatchSetFileEngineEvents
{
    public:
    
    static const CORE::CEvent FileListProcessingStartedEvent;
    static const CORE::CEvent LocalFileSizeMismatchEvent;
    static const CORE::CEvent LocalFileHashMismatchEvent;
    static const CORE::CEvent LocalFileNotFoundEvent;
    static const CORE::CEvent FileRetrievalStartedEvent;
    static const CORE::CEvent FileRetrievalCompleteEvent;
    static const CORE::CEvent FileRetrievalErrorEvent;
    static const CORE::CEvent FileStorageErrorEvent;
    static const CORE::CEvent FileListProcessingCompleteEvent;
    static const CORE::CEvent FileListProcessingAbortedEvent;
    
    static void RegisterEvents( void );
    
    protected:
    
    CPatchSetFileEngineEvents( void );
    virtual ~CPatchSetFileEngineEvents();
    
    private:
    CPatchSetFileEngineEvents( const CPatchSetFileEngineEvents& src );            /**< not implemented */
    CPatchSetFileEngineEvents& operator=( const CPatchSetFileEngineEvents& src ); /**< not implemented */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/