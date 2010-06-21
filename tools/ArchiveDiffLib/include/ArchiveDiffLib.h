/*
 *  ArchiveDiffLib: library with some archive diff util functions
 *
 *  Copyright (C) 2002 - 2010.  Dinand Vanvelzen
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

#ifndef ARCHIVEDIFFLIB_H
#define ARCHIVEDIFFLIB_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#include "CDStoreCodecPluginManager.h"
#define GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETGENERATOR_H
#include "gucefPATCHER_CPatchSetGenerator.h"
#define GUCEF_PATCHER_CPATCHSETGENERATOR_H
#endif /* GUCEF_PATCHER_CPATCHSETGENERATOR_H ? */

#ifndef ARCHIVEDIFFLIB_MACROS_H
#include "ArchiveDiffLib_macros.h"
#define ARCHIVEDIFFLIB_MACROS_H
#endif /* ARCHIVEDIFFLIB_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace ARCHIVEDIFF {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum EResourceState
{
    RESOURCESTATE_FILE_UNCHANGED           ,
    RESOURCESTATE_FILE_UNCHANGED_BUT_MOVED ,
    RESOURCESTATE_FILE_CHANGED             ,
    RESOURCESTATE_FILE_MISSING_IN_TEMPLATE ,
    RESOURCESTATE_FILE_MISSING_IN_MAIN     ,
    
    RESOURCESTATE_UNKNOWN
};
typedef enum EResourceState TResourceState;

/*-------------------------------------------------------------------------*/

struct SFileStatus
{
    TResourceState resourceState;    
    GUCEF::PATCHER::CPatchSetParser::TFileEntry templateArchiveInfo;
    GUCEF::PATCHER::CPatchSetParser::TFileEntry mainSvnArchiveInfo;
};
typedef struct SFileStatus TFileStatus;
typedef std::vector< TFileStatus > TFileStatusVector;

/*-------------------------------------------------------------------------*/

typedef std::set< const GUCEF::PATCHER::CPatchSetParser::TFileEntry* > TConstFileEntrySet;
typedef std::set< GUCEF::PATCHER::CPatchSetParser::TFileEntry* > TFileEntrySet;
typedef std::map< GUCEF::CORE::CString, TFileEntrySet > TFileEntryPtrMap;

/*-------------------------------------------------------------------------*/

struct SSortedFileEntryMaps
{
    TFileEntryPtrMap hashMap;
    TFileEntryPtrMap nameMap;
};
typedef struct SSortedFileEntryMaps TSortedFileEntryMaps;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void ARCHIVEDIFFLIB_PUBLIC_CPP
InitializeFileStatus( TFileStatus& fileStatus );

/*-------------------------------------------------------------------------*/

GUCEF::CORE::CDStoreCodecRegistry::TDStoreCodecPtr ARCHIVEDIFFLIB_PUBLIC_CPP
GetXmlDStoreCodec( void );

/*-------------------------------------------------------------------------*/

bool ARCHIVEDIFFLIB_PUBLIC_CPP
BuildSortedFileEntryMaps( GUCEF::PATCHER::CPatchSetParser::TPatchSet& patchSet ,
                          TSortedFileEntryMaps& sortedFileMaps                 );

/*-------------------------------------------------------------------------*/

bool ARCHIVEDIFFLIB_PUBLIC_CPP
LoadPatchSet( const GUCEF::CORE::CString& filePath                 ,
              GUCEF::PATCHER::CPatchSetParser::TPatchSet& patchSet );
              
/*-------------------------------------------------------------------------*/
              
bool ARCHIVEDIFFLIB_PUBLIC_CPP
LoadXmlFile( const GUCEF::CORE::CString& filePath ,
             GUCEF::CORE::CDataNode& dataTree     );

/*-------------------------------------------------------------------------*/
             
bool ARCHIVEDIFFLIB_PUBLIC_CPP
SaveXmlFile( const GUCEF::CORE::CString& filePath   ,
             const GUCEF::CORE::CDataNode& dataTree );
              
/*-------------------------------------------------------------------------*/

bool ARCHIVEDIFFLIB_PUBLIC_CPP
PerformArchiveDiff( const TFileEntryPtrMap& templatePatchsetSortedMap    ,
                    const TFileEntryPtrMap& mainArchivePatchsetSortedMap ,
                    TFileStatusVector& fileStatusList                    );

/*-------------------------------------------------------------------------*/

bool ARCHIVEDIFFLIB_PUBLIC_CPP
PerformArchiveDiff( GUCEF::PATCHER::CPatchSetParser::TPatchSet& templatePatchset    ,
                    GUCEF::PATCHER::CPatchSetParser::TPatchSet& mainArchivePatchset ,
                    TFileStatusVector& fileStatusList                               );

/*-------------------------------------------------------------------------*/

bool ARCHIVEDIFFLIB_PUBLIC_CPP
LoadFileStatusList( const GUCEF::CORE::CString& filePath ,
                    TFileStatusVector& fileStatusList    );

/*-------------------------------------------------------------------------*/

bool ARCHIVEDIFFLIB_PUBLIC_CPP
SerializeFileStatus( const TFileStatus& fileStatus      ,
                     GUCEF::CORE::CDataNode& parentNode );

/*-------------------------------------------------------------------------*/
                    
/**
 *  Saves the status list given to the file indicated by filePath
 *  Note that if maxEntriesPerFile is greater then -1 it will count as 
 *  a limit on the number of entries allowed in a file. If the file status
 *  entries do not fit in a single files multiple files will be created holding
 *  up to 'maxEntriesPerFile' entries.
 */
bool ARCHIVEDIFFLIB_PUBLIC_CPP
SaveFileStatusList( const GUCEF::CORE::CString& filePath       ,
                    const TFileStatusVector& fileStatusList    ,
                    const GUCEF::CORE::Int32 maxEntriesPerFile );

/*-------------------------------------------------------------------------*/

GUCEF::CORE::CString ARCHIVEDIFFLIB_PUBLIC_CPP
GetPathForFile( const GUCEF::PATCHER::CPatchSetParser::TFileEntry& fileEntry );

/*-------------------------------------------------------------------------*/

TResourceState ARCHIVEDIFFLIB_PUBLIC_CPP
StringToResourceState( const GUCEF::CORE::CString& state );
                    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace ARCHIVEDIFF */

/*-------------------------------------------------------------------------*/

#endif /* ARCHIVEDIFFLIB_H ? */
