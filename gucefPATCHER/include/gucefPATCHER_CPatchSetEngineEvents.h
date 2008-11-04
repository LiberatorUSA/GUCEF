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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef GUCEF_PATCHER_CPATCHSETENGINEEVENTS_H
#define GUCEF_PATCHER_CPATCHSETENGINEEVENTS_H
 
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

class GUCEFPATCHER_EXPORT_CPP CPatchSetEngineEvents
{
    public:
    
    static const CORE::CEvent PatchSetProcessingStartedEvent;
    static const CORE::CEvent PatchSetProcessingProgressEvent;
    static const CORE::CEvent PatchSetProcessingCompletedEvent;
    static const CORE::CEvent PatchSetProcessingAbortedEvent;
    static const CORE::CEvent PatchSetProcessingFailedEvent;
    
    static void RegisterEvents( void );
    
    /**
     *  Storage structure for patch-set-engine events that provides access to status information
     *  We use a single data type for all event data for all events listed in this event interface.
     *  We cannot include references to data since that would cause problems in the very likely threading use-case
     */
    struct SPatchSetEngineEventDataStorage
    {
        UInt64 totalDataSizeInBytes;
        UInt64 processedDataSizeInBytes;
    };
    typedef struct SPatchSetEngineEventDataStorage TPatchSetEngineEventDataStorage;    

    /**
     *  Cloneable wrapper for the event data.
     *  See struct TPatchSetEngineEventDataStorage for information.     
     */
    typedef CORE::CTCloneableObj< TPatchSetEngineEventDataStorage > TPatchSetEngineEventData;
    
    protected:
    
    CPatchSetEngineEvents( void );
    virtual ~CPatchSetEngineEvents();
    
    private:
    CPatchSetEngineEvents( const CPatchSetEngineEvents& src );            /**< not implemented */
    CPatchSetEngineEvents& operator=( const CPatchSetEngineEvents& src ); /**< not implemented */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHSETENGINEEVENTS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/