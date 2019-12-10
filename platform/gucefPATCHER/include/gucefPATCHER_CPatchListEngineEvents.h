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

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

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

class GUCEFPATCHER_EXPORT_CPP CPatchListEngineEvents
{
    public:
    
    static const CORE::CEvent PatchListProcessingStartedEvent;
    static const CORE::CEvent PatchListProcessingProgressEvent;
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
    
    /**
     *  Storage structure for patch-list-engine events that provides access to status information
     *  We use a single data type for all event data for all events listed in this event interface.
     *  We cannot include references to data since that would cause problems in the very likely threading use-case
     */
    struct SPatchListEngineEventDataStorage
    {
        UInt64 totalDataSizeInBytes;       /**< total size of all data in all patch sets */
        UInt64 processedDataSizeInBytes;   /**< total amount of processed data for all patch sets */
    };
    typedef struct SPatchListEngineEventDataStorage TPatchListEngineEventDataStorage;
    
    /**
     *  Cloneable wrapper for the event data.
     *  See struct TPatchSetEngineEventDataStorage for information.     
     */
    typedef CORE::CTCloneableObj< TPatchListEngineEventDataStorage > TPatchListEngineEventData;    
    
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