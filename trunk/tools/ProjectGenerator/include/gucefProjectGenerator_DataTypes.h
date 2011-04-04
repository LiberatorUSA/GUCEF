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
    MODULETYPE_UNDEFINED                ,    // <- this is the initialization value
    
    MODULETYPE_EXECUTABLE               ,
    MODULETYPE_SHARED_LIBRARY           ,    // <- shared library (.dll on win32, .so on linux/android)
    MODULETYPE_STATIC_LIBRARY           ,    // <- static library (.lib on win32, .a on linux/android)
    MODULETYPE_HEADER_INCLUDE_LOCATION  ,    // <- location where headers can be placed for reference by other modules
    
    MODULETYPE_UNKNOWN             // <- to be used when initialized BUT we cannot determine the module type
};
typedef enum EModuleType TModuleType;

/*---------------------------------------------------------------------------*/

/**
 *  Structure where all linker related information should be stored
 */
struct SLinkerSettings
{
    TStringVector linkedLibraries;               // list of all libraries the module links against
};
typedef struct SLinkerSettings TLinkerSettings;

/*---------------------------------------------------------------------------*/

/**
 *  Structure where all compiler related information should be stored
 */
struct SCompilerSettings
{
    TStringVector languagesUsed;                // list of all programming languages used within this module
};
typedef struct SCompilerSettings TCompilerSettings;

/*---------------------------------------------------------------------------*/

/**
 *  Structure where all preprocessor related information should be stored
 */
struct SPreprocessorSettings
{
    TStringVector defines;                       // list of all precompiler definitions for this module
};
typedef struct SPreprocessorSettings TPreprocessorSettings;

/*---------------------------------------------------------------------------*/

struct SModuleInfo
{
    CORE::CString name;                          // the name of the module
    TModuleType moduleType;                      // The type of module we are dealing with
    
    TStringVector dependencies;                  // list of module names of all modules this module depends on
    TStringSet dependencyIncludeDirs;            // include directories needed for the headers of the dependencies, paths only no files
       
    TStringVectorMap includeDirs;                // include directories of this module's own headers
    TStringVectorMap sourceDirs;                 // source directories of this module's own source

    int buildOrder;                              // order number of this module in the build queue

    TLinkerSettings linkerSettings;              // all linker related settings for this module
    TCompilerSettings compilerSettings;          // all compiler related settings for this module
    TPreprocessorSettings preprocessorSettings;  // all preprocessor related settings for this module
};
typedef struct SModuleInfo TModuleInfo;

/*---------------------------------------------------------------------------*/

typedef std::vector< TModuleInfo > TModuleInfoVector;
typedef std::map< CORE::CString, TModuleInfoVector > TModuleInfoVectorMap;
typedef std::map< CORE::CString, TModuleInfo > TModuleInfoMap;
typedef std::vector< TModuleInfo* > TModuleInfoPtrVector;

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
typedef std::vector< TModuleInfoEntry* > TModuleInfoEntryPtrVector;
typedef std::map< int, TModuleInfoEntry* > TModuleInfoEntryPrioMap;

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

TModuleInfo*
FindModuleInfoForPlatform( TModuleInfoEntry& moduleInfoEntry ,
                           const CORE::CString& platform     ,
                           bool createNewIfNoneExists        );

/*-------------------------------------------------------------------------*/

const CORE::CString*
GetModuleName( const TModuleInfoEntry& moduleInfoEntry ,
               const CORE::CString& targetPlatform     ,
               const TModuleInfo** moduleInfo = NULL   );
               
/*-------------------------------------------------------------------------*/

// The name to use in config files etc can't always be multiple names or defined
// per platform. For that we have this function which looks at the different names
// available across the platforms and find the general consensus name which is the
// best guess name that could be used to label this module generally without specifying 
// a platform
CORE::CString
GetConsensusModuleName( const TModuleInfoEntry& moduleInfoEntry );

/*-------------------------------------------------------------------------*/

// Tries to get the properly merged module name first for the given platform
// if this does not yield a module name it will use the general consensus
// module name instead
CORE::CString
GetModuleNameAlways( const TModuleInfoEntry& moduleInfoEntry ,
                     const CORE::CString& targetPlatform     );

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

/*-------------------------------------------------------------------------*/

void
GetModuleDependencies( TModuleInfoEntry& moduleInfoEntry   ,
                       const CORE::CString& targetPlatform ,
                       TStringVector& dependencies         );
                       

/*-------------------------------------------------------------------------*/

TModuleType
GetModuleType( const TModuleInfoEntry& moduleInfoEntry ,
               const CORE::CString& targetPlatform     );
                      
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
