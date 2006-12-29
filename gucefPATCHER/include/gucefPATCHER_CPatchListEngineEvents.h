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

#ifndef GUCEF_PATCHER_CPATCHLISTENGINEEVENTS_H
#define GUCEF_PATCHER_CPATCHLISTENGINEEVENTS_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

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

class EXPORT_CPP CPatchListEngineEvents
{
    public:
    
    static const CORE::CEvent PatchListProcessingStartedEvent;
    static const CORE::CEvent PatchListProcessingCompletedEvent;
    static const CORE::CEvent PatchListProcessingAbortedEvent;
    static const CORE::CEvent PatchListProcessingFailedEvent;
    static const CORE::CEvent PatchSetDataRecievedEvent;
    static const CORE::CEvent PatchSetRetrievalStartedEvent;
    static const CORE::CEvent PatchSetRetrievalCompletedEvent;
    static const CORE::CEvent PatchSetRetrievalFailedEvent;
    static const CORE::CEvent PatchSetRetrievalAbortedEvent;
    static const CORE::CEvent PatchSetDecodingFailedEvent;
    
    static void RegisterEvents( void );
    
    protected:
    
    CPatchListEngineEvents( void );
    virtual ~CPatchListEngineEvents();
    
    private:
    CPatchListEngineEvents( const CPatchListEngineEvents& src );            /**< not implemented */
    CPatchListEngineEvents& operator=( const CPatchListEngineEvents& src ); /**< not implemented */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHLISTENGINEEVENTS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/