/*
 *  ProjectGenerator: Tool to generate module/project files
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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
 
#ifndef GUCEF_PROJECTGENERATOR_DATATYPES_H
#define GUCEF_PROJECTGENERATOR_DATATYPES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>
#include <map>
#include <vector>

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PROJECTGENERATOR {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef std::set< CORE::CString > TStringSet;
typedef std::map< CORE::CString, CORE::CString > TStringMap;
typedef std::map< CORE::CString, TStringSet > TStringSetMap;
typedef std::vector< CORE::CString > TStringVector;
typedef std::map< CORE::CString, TStringVector > TStringVectorMap;
typedef std::map< CORE::CString, TStringVectorMap > TStringVectorMapMap;

/*---------------------------------------------------------------------------*/

enum EModuleType
{
    MODULETYPE_UNDEFINED      ,    // <- this is the initialization value
    
    MODULETYPE_EXECUTABLE     ,
    MODULETYPE_SHARED_LIBRARY ,
    MODULETYPE_STATIC_LIBRARY ,
    
    MODULETYPE_UNKNOWN             // <- to be used when initialized BUT we cannot determine the module type
};
typedef enum EModuleType TModuleType;

/*---------------------------------------------------------------------------*/

struct SModuleInfo
{
    TStringVector dependencies;                  // list of module names of all modules this module depends on
    TStringVector linkedLibraries;               // list of all libraries the module links against

    TStringVectorMap includeDirs;                // include directories of this module's own headers
    TStringVectorMap sourceDirs;                 // source directories of this module's own source

    TStringSet dependencyIncludeDirs;            // include directories needed for the headers of the dependencies, paths only no files

    CORE::CString name;                          // the name of the module
    TModuleType moduleType;                      // The type of module we are dealing with

    int buildOrder;                              // order number of this module in the build queue
};
typedef struct SModuleInfo TModuleInfo;

/*---------------------------------------------------------------------------*/

typedef std::vector< TModuleInfo > TModuleInfoVector;
typedef std::map< CORE::CString, TModuleInfoVector > TModuleInfoVectorMap;
typedef std::map< CORE::CString, TModuleInfo > TModuleInfoMap;
typedef std::vector< TModuleInfo* > TModuleInfoPtrVector;
typedef std::map< int, TModuleInfo* > TModulePrioMap;

/*---------------------------------------------------------------------------*/

struct SModuleInfoEntry
{
    TModuleInfoMap  modulesPerPlatform;     // ModuleInfo per platform
    CORE::CString   rootDir;                // the absolute path to the root of this module's directory tree
};
typedef struct SModuleInfoEntry TModuleInfoEntry;

/*---------------------------------------------------------------------------*/

typedef std::vector< TModuleInfoEntry > TModuleInfoEntryVector;
typedef std::pair< const TModuleInfoEntry*, const TModuleInfo* > TModuleInfoEntryPair;
typedef std::vector< TModuleInfoEntryPair > TModuleInfoEntryPairVector;

/*---------------------------------------------------------------------------*/

struct SDirProcessingInstructions
{
    TStringVectorMap dirExcludeList;        // list of directories that are to be excluded, maps a list of per platform  
    TStringVectorMap dirIncludeList;        // list of directories that are to be included, maps a list of per platform

    TStringVectorMap fileExcludeList;       // list of files that are to be excluded, maps a list of per platform
    TStringVectorMap fileIncludeList;       // list of files that are to be included, maps a list of per platform

    CORE::CDataNode processingInstructions; // All unparsed processing instruction data
};
typedef struct SDirProcessingInstructions TDirProcessingInstructions;

/*---------------------------------------------------------------------------*/

typedef std::map< CORE::CString, TDirProcessingInstructions > TDirProcessingInstructionsMap;

/*---------------------------------------------------------------------------*/

struct SProjectInfo
{
    CORE::CString projectName;                               // Name of the overall project
    TModuleInfoEntryVector modules;                          // All generated module information
    TDirProcessingInstructionsMap dirProcessingInstructions; // All loaded processing instructions mapped per path
    TStringVector globalDirExcludeList;                      // Dirs that should never be included in processing regardless of path
};
typedef struct SProjectInfo TProjectInfo;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
InitializeModuleInfoEntry( TModuleInfoEntry& moduleEntry );

/*-------------------------------------------------------------------------*/

void
InitializeModuleInfo( TModuleInfo& moduleInfo );


/*-------------------------------------------------------------------------*/

const TModuleInfo*
FindModuleInfoForPlatform( const TModuleInfoEntry& moduleInfoEntry ,
                           const CORE::CString& platform           );

/*-------------------------------------------------------------------------*/

void
MergeModuleInfo( TModuleInfo& targetModuleInfo          ,
                 const TModuleInfo& moduleInfoToMergeIn );

/*-------------------------------------------------------------------------*/

bool
MergeModuleInfo( const TModuleInfoEntry& moduleInfo     ,
                 const CORE::CString& targetPlatform    ,
                 TModuleInfo& mergedModuleInfo          );

/*-------------------------------------------------------------------------*/

bool
MergeAllModuleInfoForPlatform( const TModuleInfoEntryVector& allInfo  ,
                               const CORE::CString& platform          ,
                               TModuleInfoVector& allMergedInfo       ,
                               TModuleInfoEntryPairVector& mergeLinks );

/*-------------------------------------------------------------------------*/

CORE::CString
ModuleTypeToString( const TModuleType moduleType );

/*-------------------------------------------------------------------------*/

TModuleType
StringToModuleType( const CORE::CString moduleTypeStr );

/*-------------------------------------------------------------------------*/

bool
SerializeModuleInfo( const TModuleInfoEntry& moduleInfo  ,
                     const CORE::CString& outputFilepath );

/*-------------------------------------------------------------------------*/

bool
DeserializeModuleInfo( TModuleInfoEntry& moduleInfo       ,
                       const CORE::CString& inputFilepath );


/*-------------------------------------------------------------------------*/

bool
SerializeProjectInfo( const TProjectInfo& projectInfo ,
                      CORE::CDataNode& rootNodeToBe   );

/*-------------------------------------------------------------------------*/

bool
SerializeProjectInfo( const TProjectInfo& projectInfo     ,
                      const CORE::CString& outputFilepath );
                      
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGENERATOR */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PROJECTGENERATOR_DATATYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
