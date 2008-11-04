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

#ifndef GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
#define GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
 
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

class GUCEFPATCHER_EXPORT_CPP CPatchSetFileEngineEvents
{
    public:
    
    static const CORE::CEvent FileListProcessingStartedEvent;
    static const CORE::CEvent LocalFileIsOKEvent;
    static const CORE::CEvent LocalFileSizeMismatchEvent;
    static const CORE::CEvent LocalFileHashMismatchEvent;
    static const CORE::CEvent LocalFileNotFoundEvent;
    static const CORE::CEvent LocalFileReplacedEvent;
    static const CORE::CEvent FileRetrievalStartedEvent;
    static const CORE::CEvent FileRetrievalCompleteEvent;
    static const CORE::CEvent FileRetrievalErrorEvent;
    static const CORE::CEvent FileStorageErrorEvent;
    static const CORE::CEvent FileListProcessingCompleteEvent;
    static const CORE::CEvent FileListProcessingAbortedEvent;
    
    static void RegisterEvents( void );
    
    typedef CPatchSetParser::TFileLocation TFileLocation;
    typedef CPatchSetParser::TFileEntry TFileEntry;
    typedef std::vector< TFileEntry > TFileList;
    
    /**
     *  Storage structure for patch-set-file-engine events that provides access to status information
     *  We use a single data type for all event data for all events listed in this event interface.
     *  We cannot include references to data since that would cause problems in the very likely threading use-case
     */
    struct SPatchSetFileEngineEventDataStorage
    {
        TFileEntry currentFileEntry;         /**< current file that is being processed */
        UInt32 currentRemoteLocationIndex;   /**< index of the current remote location of the current file that is being processed */
        CString localRoot;                   /**< local root directory where the local copy will be placed/patched */
        CString tempStorageRoot;             /**< local root directory used for temporary resource storage */
        UInt64 totalBytesReceived;           /**< total number of bytes received so far */
    };
    typedef struct SPatchSetFileEngineEventDataStorage TPatchSetFileEngineEventDataStorage;
    
    /**
     *  Cloneable wrapper for the event data.
     *  See struct SPatchSetFileEngineEventDataStorage for information.     
     */
    typedef CORE::CTCloneableObj< TPatchSetFileEngineEventDataStorage > TPatchSetFileEngineEventData;
    
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