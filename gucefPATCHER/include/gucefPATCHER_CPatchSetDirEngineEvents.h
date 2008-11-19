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

#ifndef GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H
#define GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H
 
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

#ifndef GUCEF_PATCHER_CPATCHSETPARSER_H
#include "gucefPATCHER_CPatchSetParser.h"
#define GUCEF_PATCHER_CPATCHSETPARSER_H
#endif /* GUCEF_PATCHER_CPATCHSETPARSER_H ? */

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

class GUCEFPATCHER_EXPORT_CPP CPatchSetDirEngineEvents
{
    public:
    
    static const CORE::CEvent DirProcessingStartedEvent;
    static const CORE::CEvent DirProcessingProgressEvent;
    static const CORE::CEvent SubDirProcessingStartedEvent;
    static const CORE::CEvent SubDirProcessingCompletedEvent;
    static const CORE::CEvent DirProcessingCompletedEvent;
    static const CORE::CEvent DirProcessingAbortedEvent;
    
    static void RegisterEvents( void );

    typedef CPatchSetParser::TFileLocation TFileLocation;
    typedef CPatchSetParser::TFileEntry TFileEntry;
    typedef CPatchSetParser::TDirEntry TDirEntry;

    /**
     *  Storage structure for patch-set-dir-engine events that provides access to status information
     *  We use a single data type for all event data for all events listed in this event interface.
     *  We cannot include references to data since that would cause problems in the very likely threading use-case
     */
    struct SPatchSetDirEngineEventDataStorage
    {
        CString dirName;
        CString dirHash;
        UInt64 dirSizeInBytes;           /**< total size in bytes of the directory and all it's sub-directories */
        UInt64 processedDataSizeInBytes; /**< total amount of data processed for the directory and all it's sub-directories */
    };
    typedef struct SPatchSetDirEngineEventDataStorage TPatchSetDirEngineEventDataStorage;
    
    /**
     *  Cloneable wrapper for the event data.
     *  See struct SPatchSetDirEngineEventDataStorage for information.     
     */
    typedef CORE::CTCloneableObj< TPatchSetDirEngineEventDataStorage > TPatchSetDirEngineEventData;
    
    protected:
    
    CPatchSetDirEngineEvents( void );
    virtual ~CPatchSetDirEngineEvents();
    
    private:
    CPatchSetDirEngineEvents( const CPatchSetDirEngineEvents& src );            /**< not implemented */
    CPatchSetDirEngineEvents& operator=( const CPatchSetDirEngineEvents& src ); /**< not implemented */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/