/*
 *  SVNMagicMerge: Little tool to help merge 2 convoluted archives
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

/*-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define VERSION_NUMBER  1.1

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
    RESOURCESTATE_FILE_ADDED               ,
    RESOURCESTATE_FILE_MISSING             ,
    
    RESOURCESTATE_UNKNOWN
};
typedef enum EResourceState TResourceState;

/*-------------------------------------------------------------------------*/

struct SFileStatus
{
    TResourceState resourceState;
    
    PATCHER::CPatchSetParser::TFileEntry templateArchiveInfo;
    CORE::CString templateArchiveFilePath;
    PATCHER::CPatchSetParser::TFileEntry mainSvnArchiveInfo;
    CORE::CString mainSvnArchivePath;
};
typedef struct SFileStatus TFileStatus;

/*-------------------------------------------------------------------------*/

typedef std::vector< TFileStatus > TFileStatusVector;

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
        
        if ( ( templatePatchsetFileEntry.hash == fileEntry.hash )               &&
             ( templatePatchsetFileEntry.sizeInBytes == fileEntry.sizeInBytes )  )
        {
            // File content is identical
            CORE::CString relPathForMainArchiveFile = GetPathForFile( fileEntry );
            if ( templateFilePath.Equals( relPathForMainArchiveFile, false ) )
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

                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determined that file \"" + templateFilePath + "\" was unchanged but moved to \"" +  + "\"" );
                
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
            
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determined that file \"" + templateFilePath + "\" was changed at location \"" +  + "\"" );
            
            fileStatusList.push_back( fileStatus );
        }
        
        ++i;
    }
    
    if ( 0 == filesWithSameName.size() )
    {
        // Unable to locate such a file in the other archive
        TFileStatus fileStatus;
        InitializeFileStatus( fileStatus );
        fileStatus.resourceState = RESOURCESTATE_FILE_ADDED;
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
LoadPatchSet( const CORE::CString& filePath  ,
              CORE::CDataNode& patchSet      )
{GUCEF_TRACE;

    if ( CORE::FileExists( filePath ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully located file: " + filePath );
        
        CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = GetXmlDStoreCodec();
        if ( !codecPtr.IsNULL() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully located codec: " + codecPtr->GetTypeName() );
            
            if ( codecPtr->BuildDataTree( &patchSet ,
                                          filePath  ) )
            {
                return true;
            }
        }
    }
    return false;
}

/*---------------------------------------------------------------------------*/

bool
LoadPatchSet( const CORE::CString& filePath                 ,
              PATCHER::CPatchSetParser::TPatchSet& patchSet )
{GUCEF_TRACE;

    CORE::CDataNode patchSetTree;
    if ( LoadPatchSet( filePath     ,
                       patchSetTree ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded the raw patch set data from file, parsing data into strongly typed data structures" );
        
        PATCHER::CPatchSetParser parser;
        return parser.ParsePatchSet( patchSetTree ,
                                     patchSet     );
    }
    return false;
}


/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

void 
cls( void )
{GUCEF_TRACE;

    COORD coordScreen = { 0, 0 }; /* here's where we'll home the cursor */
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize; /* number of character cells in the current buffer */

    /* get the output console handle */
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    
    /* get the number of character cells in the current buffer */
    GetConsoleScreenBufferInfo(hConsole, &csbi);    
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    
    /* fill the entire screen with blanks */
    FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
      dwConSize, coordScreen, &cCharsWritten);
      
    /* get the current text attribute */
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    /* now set the buffer's attributes accordingly */
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
      dwConSize, coordScreen, &cCharsWritten);
      
    /* put the cursor at (0, 0) */
    SetConsoleCursorPosition(hConsole, coordScreen);
    return;
}

/*---------------------------------------------------------------------------*/

class CMSWin32ConsoleWindow
{
    public:
    
    CMSWin32ConsoleWindow( void )
    {
        AllocConsole();
        
        /* reopen stdin handle as console window input */
        freopen( "CONIN$", "rb", stdin );
        
        /* reopen stout handle as console window output */
        freopen( "CONOUT$", "wb", stdout );
        
        /* reopen stderr handle as console window output */
        freopen( "CONOUT$", "wb", stderr );        
    }
    
    ~CMSWin32ConsoleWindow()
    {
        FreeConsole();
    }
};

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------*/

void
ParseParams( const int argc                        , 
             char* argv[]                          ,
             GUCEF::CORE::CValueList& keyValueList )
{GUCEF_TRACE;
    
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Parsing parameters" );
    
    keyValueList.DeleteAll();
    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        for ( int i=1; i<argc; ++i )
        {
            argString += ( ' ' + argv[ i ] );
        }
        
        keyValueList.SetMultiple( argString, '\'' );
    }
}

/*---------------------------------------------------------------------------*/

void
PrintHeader( void )
{GUCEF_TRACE;

    cls();
    printf( "*****************************************\n" );
    printf( "*                                       *\n" );
    printf( "*  SVN Magic Merge                      *\n" );
    printf( "*                                       *\n" );
    printf( "*****************************************\n" );
    printf( "\n" );
    printf( " - Tool Version %f\n" , VERSION_NUMBER );    
}

/*---------------------------------------------------------------------------*/

void
PrintHelp( void )
{GUCEF_TRACE;

    printf( "\n" );
    printf( " - Program arguments:\n" );
    printf( "  Arguments should be passed in the form \'key=value\'\n" );
    printf( "    'TemplateArchiveIndex'  : path to patch set containing the index of the template archive\n" );
    printf( "    'TemplateArchivePath'   : path on disk to the template archive\n" );
    printf( "    'MainArchiveIndex'      : path to patch set containing the index of the main SVN archive\n" );
    printf( "    'MainArchivePath'       : path on disk to the main SVN archive\n" );
    printf( "    'Plugins'               : optional parameter: comman seperated list of plugins to load\n" );
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

int __stdcall
WinMain( HINSTANCE hinstance     ,
         HINSTANCE hprevinstance ,
         LPSTR lpcmdline         ,
         int ncmdshow            )
{GUCEF_TRACE;

    int argc = 0;
    char** argv = &lpcmdline;
    if ( lpcmdline != NULL )
    {
        if ( *lpcmdline != '\0' )
        {
            argc = 1;
        }
    }
    
#else

int
main( int argc , char* argv[] )
{GUCEF_TRACE;

#endif

    CORE::CString logFilename = CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( logFilename, "SVNMagicMerge_Log.txt" );
    CORE::CFileAccess logFileAccess( logFilename, "w" );

    CORE::CStdLogger logger( logFileAccess );
    CORE::CLogManager::Instance()->AddLogger( &logger );

    #ifdef GUCEF_MSWIN_BUILD
    CORE::CMSWinConsoleLogger consoleOut;
    CORE::CLogManager::Instance()->AddLogger( &consoleOut );
    #endif /* GUCEF_MSWIN_BUILD ? */
    
    CORE::CLogManager::Instance()->FlushBootstrapLogEntriesToLogs();

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "This tool was compiled on: " __DATE__ " @ " __TIME__ );

	if ( 0 == argc )
	{
	    #ifdef GUCEF_MSWIN_BUILD
	    CMSWin32ConsoleWindow console;
	    #endif
	    
	    PrintHeader();
	    PrintHelp();
	    getchar();
	}
	else
	{
	    GUCEF::CORE::CValueList argList;
	    ParseParams( argc    ,
	                 argv    ,
	                 argList ); 
	                 
        if ( !argList.HasKey( "TemplateArchiveIndex" ) ||
             !argList.HasKey( "TemplateArchivePath" )  ||
             !argList.HasKey( "MainArchiveIndex" )     ||
             !argList.HasKey( "MainArchivePath" )       )
        {
            printf( "ERROR: Not enough parameters where provided\n\n" );
            PrintHelp();
            getchar();
            return 1;
        }
        
        CORE::CString templateArchiveIndex = argList[ "TemplateArchiveIndex" ];
        CORE::CString templateArchivePath = argList[ "TemplateArchivePath" ];
        CORE::CString mainArchiveIndex = argList[ "MainArchiveIndex" ];
        CORE::CString mainArchivePath = argList[ "MainArchivePath" ];
        CORE::CString pluginsListStr = argList.GetValueAlways( "Plugins" );
        CORE::CString diffOutputDir = argList.GetValueAlways( "DiffOutputDir" );
        if ( 0 == diffOutputDir.Length() )
        {
            diffOutputDir = CORE::RelativePath( "$MODULEDIR$" );
        }
        
        PATCHER::CPatchSetParser::TPatchSet templatePatchset;
        PATCHER::CPatchSetParser::TPatchSet mainArchivePatchset;
        
        if ( LoadPatchSet( templateArchiveIndex , 
                           templatePatchset     ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded and parsed the index for the template archive" );
            
            if ( LoadPatchSet( mainArchiveIndex    , 
                               mainArchivePatchset ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded and parsed the index for the main SVN archive" );
                
                TFileStatusVector fileStatusList;
                if ( PerformArchiveDiff( templatePatchset    ,
                                         mainArchivePatchset ,
                                         fileStatusList      ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully completed determination of differences between the two archives" );    
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load and parse the index for the main SVN archive" );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load and parse the index for the template archive" );
        }
        
    }
	return 0;
}

/*-------------------------------------------------------------------------*/
