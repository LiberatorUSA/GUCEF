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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "ArchiveDiffLib.h"

/*-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace ARCHIVEDIFF {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define VERSION_NUMBER  1.1

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
InitializeFileStatus( TFileStatus& fileStatus )
{GUCEF_TRACE;

    fileStatus.resourceState = RESOURCESTATE_UNKNOWN;
    
    fileStatus.templateArchiveInfo.name.Clear();
    fileStatus.templateArchiveInfo.hash.Clear();
    fileStatus.templateArchiveInfo.sizeInBytes = 0;
    fileStatus.templateArchiveInfo.fileLocations.clear();
    
    fileStatus.mainSvnArchiveInfo.name.Clear();
    fileStatus.mainSvnArchiveInfo.hash.Clear();
    fileStatus.mainSvnArchiveInfo.sizeInBytes = 0;
    fileStatus.mainSvnArchiveInfo.fileLocations.clear();
}

/*---------------------------------------------------------------------------*/

CORE::CString
CreatePathToUnchangedFileDiff( const CORE::CString& dir )
{GUCEF_TRACE;
    
    CORE::CString path = dir;
    CORE::AppendToPath( path, "UnchangedFiles.xml" );
    return path;
}

/*---------------------------------------------------------------------------*/

CORE::CString
CreatePathToChangedFileDiff( const CORE::CString& dir )
{GUCEF_TRACE;
    
    CORE::CString path = dir;
    CORE::AppendToPath( path, "ChangedFiles.xml" );
    return path;
}

/*---------------------------------------------------------------------------*/

CORE::CString
CreatePathToAddedFileDiff( const CORE::CString& dir )
{GUCEF_TRACE;
    
    CORE::CString path = dir;
    CORE::AppendToPath( path, "AddedFiles.xml" );
    return path;
}

/*---------------------------------------------------------------------------*/

CORE::CString
CreatePathToMissingFileDiff( const CORE::CString& dir )
{GUCEF_TRACE;
    
    CORE::CString path = dir;
    CORE::AppendToPath( path, "MissingFiles.xml" );
    return path;

}

/*---------------------------------------------------------------------------*/

CORE::CDStoreCodecRegistry::TDStoreCodecPtr
GetXmlDStoreCodec( void )
{GUCEF_TRACE;

    static CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr;
    if ( codecPtr.IsNULL() )
    {
        CORE::CDStoreCodecRegistry* registry = CORE::CDStoreCodecRegistry::Instance();
        if ( !registry->TryLookup( "XML", codecPtr, false ) )
        {
            // No codec is registered to handle XML, try and load a plugin for it

            CORE::CPluginManager::TPluginPtr codecPlugin =

                #ifdef GUCEF_CORE_DEBUG_MODE
                CORE::CDStoreCodecPluginManager::Instance()->LoadPlugin( "$MODULEDIR$/dstorepluginPARSIFALXML_d" );
                #else
                CORE::CDStoreCodecPluginManager::Instance()->LoadPlugin( "$MODULEDIR$/dstorepluginPARSIFALXML" );
                #endif

            if ( NULL != codecPlugin )
            {
                // Now try and get the codec again
                registry->TryLookup( "XML", codecPtr, false );
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Request for data storage codec for xml file, succesfully loaded plugin to handle request" );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Request for data storage codec for xml file but no plugin for it could be loaded!" );
                CORE::ShowErrorMessage( "Missing codec support",
                                        "Request for data storage codec for xml file but no plugin for it could be loaded!" );
            }
        }
    }
    return codecPtr;
}

/*---------------------------------------------------------------------------*/

void
LocateFileEntriesUsingName( const PATCHER::CPatchSetParser::TFileEntry& patchsetFileEntry ,
                            const CORE::CString& relMainArchivePath                       ,
                            PATCHER::CPatchSetParser::TFileEntryList& fileList            ,
                            const CORE::CString& name                                     )
{GUCEF_TRACE;

    if ( patchsetFileEntry.name.Equals( name, false ) )
    {        
        CORE::CString relFilePath = relMainArchivePath;
        CORE::AppendToPath( relFilePath, patchsetFileEntry.name );
        
        PATCHER::CPatchSetParser::TFileEntry entry = patchsetFileEntry;
        PATCHER::CPatchSetParser::TFileLocation diskLocation;
        diskLocation.codec = "FILE"; //<- so we can differentiate from the original location item
        diskLocation.URL = relFilePath;
        entry.fileLocations.push_back( diskLocation );
        
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Found file entry with the same name at " + relFilePath );
        
        fileList.push_back( entry );
    }
}

/*---------------------------------------------------------------------------*/

void
LocateFileEntriesUsingName( const PATCHER::CPatchSetParser::TDirEntry& patchsetDirEntry ,
                            const CORE::CString& relMainArchivePath                     ,
                            PATCHER::CPatchSetParser::TFileEntryList& fileList          ,
                            const CORE::CString& name                                   )
{GUCEF_TRACE;

    PATCHER::CPatchSetParser::TFileEntryList::const_iterator i = patchsetDirEntry.files.begin();
    while ( i != patchsetDirEntry.files.end() )       
    {
        LocateFileEntriesUsingName( (*i)               ,
                                    relMainArchivePath ,
                                    fileList           ,
                                    name               );
        
        ++i;
    }
    
    PATCHER::CPatchSetParser::TPatchSet::const_iterator n = patchsetDirEntry.subDirs.begin(); 
    while ( n != patchsetDirEntry.subDirs.end() )        
    {
        CORE::CString subDirPath = relMainArchivePath;
        CORE::AppendToPath( subDirPath, (*n).name );
        
        LocateFileEntriesUsingName( (*n)       ,
                                    subDirPath ,
                                    fileList   ,
                                    name       );
        
        ++n;
    }
}

/*---------------------------------------------------------------------------*/

void
LocateFileEntriesUsingName( const PATCHER::CPatchSetParser::TPatchSet& patchSet ,
                            const CORE::CString& relMainArchivePath             ,
                            PATCHER::CPatchSetParser::TFileEntryList& fileList  ,
                            const CORE::CString& name                           )
{GUCEF_TRACE;

    PATCHER::CPatchSetParser::TPatchSet::const_iterator i = patchSet.begin();
    while ( i != patchSet.end() )        
    {
        CORE::CString subDirPath = relMainArchivePath;
        CORE::AppendToPath( subDirPath, (*i).name );

        LocateFileEntriesUsingName( (*i)       ,
                                    subDirPath ,
                                    fileList   ,
                                    name       );
        
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetPathForFile( const PATCHER::CPatchSetParser::TFileEntry& fileEntry )
{GUCEF_TRACE;

    PATCHER::CPatchSetParser::TFileLocations::const_iterator i = fileEntry.fileLocations.begin();
    while ( i != fileEntry.fileLocations.end() )
    {
        const PATCHER::CPatchSetParser::TFileLocation& location = (*i);
        if ( "FILE" == location.codec )
        {
            return location.URL;
        }
        ++i;
    }
    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

bool
PerformArchiveDiff( const PATCHER::CPatchSetParser::TFileEntry& templatePatchsetFileEntry ,
                    const CORE::CString& relTemplatePatchsetDirPath                       ,
                    const PATCHER::CPatchSetParser::TPatchSet& mainArchivePatchset        ,
                    TFileStatusVector& fileStatusList                                     )
{GUCEF_TRACE;

    CORE::CString templateFilePath = relTemplatePatchsetDirPath;
    CORE::AppendToPath( templateFilePath, templatePatchsetFileEntry.name );
    CORE::Int32 firstSlashIndex = templateFilePath.HasChar( '\\' );
    CORE::CString templateFilePathMinuxRoot;
    if ( firstSlashIndex < 0 )
    {
        templateFilePathMinuxRoot = templateFilePath;
    }
    else
    {
        templateFilePathMinuxRoot = templateFilePath.CutChars( firstSlashIndex+1, true ); 
    }
       
    // First locate all files in the main archive that have the same name as
    // our template archive file
    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Commencing search for files with the same name as template file: " + templateFilePath );
    PATCHER::CPatchSetParser::TFileEntryList filesWithSameName;
    LocateFileEntriesUsingName( mainArchivePatchset            , 
                                CORE::CString()                ,
                                filesWithSameName              ,
                                templatePatchsetFileEntry.name );

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Found " + CORE::UInt32ToString( filesWithSameName.size() ) + " files with the same name as our template file in the main archive" );                                
    
    // Now check the resource state for each of those files
    PATCHER::CPatchSetParser::TFileEntryList::iterator i = filesWithSameName.begin();
    while ( i != filesWithSameName.end() )
    {        
        PATCHER::CPatchSetParser::TFileEntry& fileEntry = (*i);

        CORE::CString relPathForMainArchiveFile = GetPathForFile( fileEntry );
        firstSlashIndex = relPathForMainArchiveFile.HasChar( '\\' );
        CORE::CString relPathForMainArchiveFileMinusRoot;
        if ( firstSlashIndex < 0 )
        {
            relPathForMainArchiveFileMinusRoot = relPathForMainArchiveFile;
        }
        else
        {
            relPathForMainArchiveFileMinusRoot = relPathForMainArchiveFile.CutChars( firstSlashIndex+1, true ); 
        }
        
        if ( ( templatePatchsetFileEntry.hash == fileEntry.hash )               &&
             ( templatePatchsetFileEntry.sizeInBytes == fileEntry.sizeInBytes )  )
        {
            // File content is identical
            if ( templateFilePathMinuxRoot.Equals( relPathForMainArchiveFileMinusRoot, false ) )
            {                
                TFileStatus fileStatus;
                InitializeFileStatus( fileStatus );
                fileStatus.resourceState = RESOURCESTATE_FILE_UNCHANGED;
                fileStatus.templateArchiveInfo = templatePatchsetFileEntry;
                fileStatus.mainSvnArchiveInfo = fileEntry;
                
                PATCHER::CPatchSetParser::TFileLocation templateLocation;
                templateLocation.codec = "FILE";
                templateLocation.URL = templateFilePath;
                fileStatus.templateArchiveInfo.fileLocations.push_back( templateLocation );
                
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determined that file \"" + templateFilePath + "\" was unchanged" );
                
                fileStatusList.push_back( fileStatus );
            }
            else
            {
                TFileStatus fileStatus;
                InitializeFileStatus( fileStatus );
                fileStatus.resourceState = RESOURCESTATE_FILE_UNCHANGED_BUT_MOVED;
                fileStatus.templateArchiveInfo = templatePatchsetFileEntry;
                fileStatus.mainSvnArchiveInfo = fileEntry;
                
                PATCHER::CPatchSetParser::TFileLocation templateLocation;
                templateLocation.codec = "FILE";
                templateLocation.URL = templateFilePath;
                fileStatus.templateArchiveInfo.fileLocations.push_back( templateLocation );

                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determined that file \"" + templateFilePath + "\" was unchanged but moved to \"" + relPathForMainArchiveFile + "\"" );
                
                fileStatusList.push_back( fileStatus );
            }
        }
        else
        {
            // File content is different
            TFileStatus fileStatus;
            InitializeFileStatus( fileStatus );
            fileStatus.resourceState = RESOURCESTATE_FILE_CHANGED;
            fileStatus.templateArchiveInfo = templatePatchsetFileEntry;
            fileStatus.mainSvnArchiveInfo = fileEntry;

            PATCHER::CPatchSetParser::TFileLocation templateLocation;
            templateLocation.codec = "FILE";
            templateLocation.URL = templateFilePath;
            fileStatus.templateArchiveInfo.fileLocations.push_back( templateLocation );
            
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determined that file \"" + templateFilePath + "\" was changed at location \"" + relPathForMainArchiveFile + "\"" );
            
            fileStatusList.push_back( fileStatus );
        }
        
        ++i;
    }
    
    if ( 0 == filesWithSameName.size() )
    {
        // Unable to locate such a file in the other archive
        TFileStatus fileStatus;
        InitializeFileStatus( fileStatus );
        fileStatus.resourceState = RESOURCESTATE_FILE_MISSING_IN_MAIN;
        fileStatus.templateArchiveInfo = templatePatchsetFileEntry;

        PATCHER::CPatchSetParser::TFileLocation templateLocation;
        templateLocation.codec = "FILE";
        templateLocation.URL = templateFilePath;
        fileStatus.templateArchiveInfo.fileLocations.push_back( templateLocation );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determined that file \"" + templateFilePath + "\" was added in the template archive" );
        
        fileStatusList.push_back( fileStatus );
    }
        
    return true;
}

/*---------------------------------------------------------------------------*/

bool
PerformArchiveDiff( const PATCHER::CPatchSetParser::TDirEntry& templatePatchsetDirEntry ,
                    const CORE::CString& relTemplatePatchsetDirPath                     ,
                    const PATCHER::CPatchSetParser::TPatchSet& mainArchivePatchset      ,
                    TFileStatusVector& fileStatusList                                   )
{GUCEF_TRACE;

    // process all files in this dir
    PATCHER::CPatchSetParser::TFileEntryList::const_iterator i = templatePatchsetDirEntry.files.begin();
    while ( i != templatePatchsetDirEntry.files.end() )
    {
        PerformArchiveDiff( (*i)                          ,
                            relTemplatePatchsetDirPath    ,
                            mainArchivePatchset           ,
                            fileStatusList                );
        
        ++i;
    }

    PATCHER::CPatchSetParser::TDirEntryList::const_iterator n = templatePatchsetDirEntry.subDirs.begin();
    while ( n != templatePatchsetDirEntry.subDirs.end() )        
    {
        // create relative path to sub-dir
        CORE::CString fullPathToSubDir = relTemplatePatchsetDirPath;
        CORE::AppendToPath( fullPathToSubDir, (*n).name );
        
        // do the same for the sub-dir
        PerformArchiveDiff( (*n)                ,
                            fullPathToSubDir    ,
                            mainArchivePatchset ,
                            fileStatusList      );
        
        ++n;
    }

    return true;
}

/*---------------------------------------------------------------------------*/

bool
PerformArchiveDiff( const PATCHER::CPatchSetParser::TPatchSet& templatePatchset    ,
                    const PATCHER::CPatchSetParser::TPatchSet& mainArchivePatchset ,
                    TFileStatusVector& fileStatusList                              )
{GUCEF_TRACE;

    PATCHER::CPatchSetParser::TPatchSet::const_iterator i = templatePatchset.begin();
    while ( i != templatePatchset.end() )        
    {
        PerformArchiveDiff( (*i)                ,
                            (*i).name           ,
                            mainArchivePatchset ,
                            fileStatusList      );
        
        ++i;
    }
    return true;
}

/*---------------------------------------------------------------------------*/

bool
SaveXmlFile( const CORE::CString& filePath   ,
             const CORE::CDataNode& dataTree )
{GUCEF_TRACE;

    if ( CORE::FileExists( filePath ) )
    {
        if ( 0 != CORE::Delete_File( filePath.C_String() ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "A file already exists and it could not be deleted: " + filePath );
            return false;
        }
    }
    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = GetXmlDStoreCodec();
    if ( !codecPtr.IsNULL() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully located codec: " + codecPtr->GetTypeName() );
        
        if ( codecPtr->StoreDataTree( &dataTree ,
                                      filePath  ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully stored data tree to file using codec " + codecPtr->GetTypeName() + " at location " + filePath );
            return true;
        }
    }

    return false;
}

/*---------------------------------------------------------------------------*/

bool
LoadXmlFile( const CORE::CString& filePath  ,
             CORE::CDataNode& dataTree      )
{GUCEF_TRACE;

    if ( CORE::FileExists( filePath ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully located file: " + filePath );
        
        CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = GetXmlDStoreCodec();
        if ( !codecPtr.IsNULL() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully located codec: " + codecPtr->GetTypeName() );
            
            if ( codecPtr->BuildDataTree( &dataTree ,
                                          filePath  ) )
            {
                return true;
            }
        }
    }
    return false;
}

/*---------------------------------------------------------------------------*/

CORE::UInt32
GetFileCount( const PATCHER::CPatchSetParser::TDirEntry& patchDir )
{GUCEF_TRACE;

    CORE::UInt32 count = patchDir.files.size();
    PATCHER::CPatchSetParser::TDirEntryList::const_iterator i = patchDir.subDirs.begin();
    while ( i != patchDir.subDirs.end() )
    {
        count += GetFileCount( (*i) );
        ++i;
    }
    return count;
}

/*---------------------------------------------------------------------------*/

CORE::UInt32
GetFileCount( const PATCHER::CPatchSetParser::TPatchSet& patchSet )
{GUCEF_TRACE;

    CORE::UInt32 count = 0;
    PATCHER::CPatchSetParser::TPatchSet::const_iterator i = patchSet.begin();
    while ( i != patchSet.end() )
    {
        count += GetFileCount( (*i) );
        ++i;
    }
    return count;    
}

/*---------------------------------------------------------------------------*/

bool
LoadPatchSet( const CORE::CString& filePath                 ,
              PATCHER::CPatchSetParser::TPatchSet& patchSet )
{GUCEF_TRACE;

    CORE::CDataNode patchSetTree;
    if ( LoadXmlFile( filePath     ,
                      patchSetTree ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded the xml data from file totalling " + 
                                          CORE::UInt32ToString( patchSetTree.GetNrOfTreeNodes() ) + 
                                          " nodes, parsing data into strongly typed data structures" );
        
        PATCHER::CPatchSetParser parser;
        if ( parser.ParsePatchSet( patchSetTree ,
                                   patchSet     ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully parsed the data tree into " +
                                              CORE::UInt32ToString( GetFileCount( patchSet ) ) + 
                                              " patch set entries" );
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const char*
ResourceStateToString( TResourceState state )
{GUCEF_TRACE;
    
    switch ( state )
    {
        case RESOURCESTATE_FILE_UNCHANGED : return "Unchanged";
        case RESOURCESTATE_FILE_UNCHANGED_BUT_MOVED : return "UnchangedButMoved";
        case RESOURCESTATE_FILE_CHANGED : return "Changed";
        case RESOURCESTATE_FILE_MISSING_IN_MAIN : return "MissingInMain";
        case RESOURCESTATE_FILE_MISSING_IN_TEMPLATE : return "MissingInTemplate";
        default:
        {
            return "Unknown";
        }
    }
}

/*-------------------------------------------------------------------------*/

TResourceState
StringToResourceState( const CORE::CString& state )
{GUCEF_TRACE;

    if ( state.Equals( "Unchanged", false ) ) return RESOURCESTATE_FILE_UNCHANGED;
    if ( state.Equals( "UnchangedButMoved", false ) ) return RESOURCESTATE_FILE_UNCHANGED_BUT_MOVED;
    if ( state.Equals( "Changed", false ) ) return RESOURCESTATE_FILE_CHANGED;
    if ( state.Equals( "MissingInMain", false ) ) return RESOURCESTATE_FILE_MISSING_IN_MAIN;
    if ( state.Equals( "MissingInTemplate", false ) ) return RESOURCESTATE_FILE_MISSING_IN_TEMPLATE;
    if ( state.Equals( "Unknown", false ) ) return RESOURCESTATE_UNKNOWN;

    return RESOURCESTATE_UNKNOWN;
}

/*-------------------------------------------------------------------------*/

void
SerializeFileStatus( const TFileStatus& fileStatus ,
                     CORE::CDataNode& parentNode   )
{GUCEF_TRACE;

    static PATCHER::CPatchSetParser parser;
    
    CORE::CDataNode fileStatusNode;
    fileStatusNode.SetName( "FileStatus" );
    fileStatusNode.SetAttribute( "State", ResourceStateToString( fileStatus.resourceState ) );
    
    CORE::CDataNode templateFileInfo;
    templateFileInfo.SetName( "TemplateInfo" );    
    parser.SerializeFileEntry( fileStatus.templateArchiveInfo ,
                               templateFileInfo               );
    fileStatusNode.AddChild( templateFileInfo );
    templateFileInfo.Clear();
    
    CORE::CDataNode mainFileInfo;
    mainFileInfo.SetName( "MainInfo" );    
    parser.SerializeFileEntry( fileStatus.mainSvnArchiveInfo ,
                               mainFileInfo                  );
    fileStatusNode.AddChild( mainFileInfo );
    mainFileInfo.Clear();
}

/*-------------------------------------------------------------------------*/

bool
DeserializeFileInfo( const CORE::CDataNode& fileStatusNode           ,
                     const CORE::CString& name                       ,
                     PATCHER::CPatchSetParser::TFileEntry& fileEntry )
{GUCEF_TRACE;

    static PATCHER::CPatchSetParser parser;    
    const CORE::CDataNode* infoNodeParent = fileStatusNode.FindChild( name );
    if ( NULL != infoNodeParent )
    {
        const CORE::CDataNode* infoNode = infoNodeParent->GetFirstChild();
        if ( NULL != infoNode )
        {
            if ( parser.ValidateAndParseFileEntry( *infoNode ,
                                                   fileEntry ) )
            {
                return true;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to locate child node with file info" );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to locate child node with name " + name );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
DeserializeFileStatus( const CORE::CDataNode& fileStatusNode ,
                       TFileStatus& fileStatus               )
{GUCEF_TRACE;

    if ( fileStatusNode.GetName().Equals( "FileStatus", false ) )
    {
        // This is an actual file status entry
        fileStatus.resourceState = StringToResourceState( fileStatusNode.GetAttributeValue( "State" ) );
        
        if ( DeserializeFileInfo( fileStatusNode                 ,
                                  "TemplateInfo"                 ,
                                  fileStatus.templateArchiveInfo ) )
        {
            if ( DeserializeFileInfo( fileStatusNode                ,
                                      "MainInfo"                    ,
                                      fileStatus.mainSvnArchiveInfo ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully deserialized an entire file status object for: " + fileStatus.templateArchiveInfo.name );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to deserialize the main file info" );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to deserialize the template file info" );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "The given node in the XML document is not a file status node" );
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
SaveFileStatusList( const CORE::CString& filePath           ,
                    const TFileStatusVector& fileStatusList )
{GUCEF_TRACE;

    CORE::CDataNode fileStatusTree;
    fileStatusTree.SetName( "FileStatusList" );
    fileStatusTree.SetAttribute( "ItemCount", CORE::UInt32ToString( fileStatusList.size() ) );
    
    TFileStatusVector::const_iterator i = fileStatusList.begin();
    while ( i != fileStatusList.end() )
    {
        const TFileStatus& fileStatus = (*i);
        SerializeFileStatus( fileStatus, fileStatusTree );
        ++i;
    }
    
    if ( SaveXmlFile( filePath       ,
                      fileStatusTree ) )
    {   
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully saved the generated data tree to file " + filePath );
        return true;
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to save the generated data tree to file " + filePath );
    return false;    
}

/*-------------------------------------------------------------------------*/

bool
LoadFileStatusList( const CORE::CString& filePath     ,
                    TFileStatusVector& fileStatusList )
{GUCEF_TRACE;

    CORE::CDataNode fileStatusTree;
    if ( LoadXmlFile( filePath       ,
                      fileStatusTree ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded the xml data from file, parsing data into strongly typed data structures" );

        const CORE::CDataNode* rootNode = fileStatusTree.Find( "FileStatusList" );
        if ( NULL != rootNode )
        {
            CORE::CDataNode::const_iterator i = rootNode->ConstBegin();
            while ( i != rootNode->ConstEnd() )
            {
                const CORE::CDataNode* childNode = (*i);
                TFileStatus fileStatus;
                if ( DeserializeFileStatus( *childNode ,
                                            fileStatus ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully deserialized a file status entry" );
                    fileStatusList.push_back( fileStatus );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to deserialize a file status entry" );
                }
                ++i;
            } 
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to locate file status list root node in XML document" );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace ARCHIVEDIFF */

/*-------------------------------------------------------------------------*/
