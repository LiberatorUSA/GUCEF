/*
 *  ReferenceUpdater: Little tool to help keep binary references up to date
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

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

/*-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define VERSION_NUMBER  1.0

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SFileEntry
{
    CORE::CString filename;
    CORE::CString filedir;
};
typedef struct SFileEntry TFileEntry;

typedef std::vector< TFileEntry > TFileEntryVector;

/*-------------------------------------------------------------------------*/

struct SMatchEntry
{
    TFileEntry source;
    TFileEntryVector destinations;
};
typedef struct SMatchEntry TMatchEntry;

typedef std::vector< TMatchEntry > TMatchEntryVector;

/*-------------------------------------------------------------------------*/

typedef std::set< CORE::CString > TStringSet;
typedef std::vector< CORE::CString > TStringVector;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
BuildFileList( const CORE::CString& srcDir    ,
               TFileEntryVector& files        ,
               const TStringSet* fileTypes    ,
               const TStringSet* dirsToIgnore )
{GUCEF_TRACE;

    struct CORE::SDI_Data* dirEntry = CORE::DI_First_Dir_Entry( srcDir.C_String() );
    if ( dirEntry != NULL )    
    {
        do 
        {
            CORE::CString entryName = CORE::DI_Name( dirEntry );
            if ( ( entryName != "." )  &&
                 ( entryName != ".." )  )
            {
                // Check if this is a dir we have to ignore
                if ( NULL != dirsToIgnore )
                {
                    if ( dirsToIgnore->find( entryName.Lowercase() ) != dirsToIgnore->end() )
                    {
                        // do not process further
                        break;
                    }
                }
                
                if ( 0 != CORE::DI_Is_It_A_File( dirEntry ) )
                {
                    if ( NULL != fileTypes )
                    {
                        CORE::CString fileExt = CORE::Extract_File_Ext( entryName.C_String() );
                        fileExt = fileExt.Lowercase();
                        if ( fileTypes->find( fileExt ) == fileTypes->end() )
                        {
                            // skip this entry as instructed, not a matching filetype
                            continue;
                        }
                    }
                    
                    TFileEntry fileEntry;
                    fileEntry.filedir = srcDir;
                    fileEntry.filename = entryName;
                    
                    files.push_back( fileEntry );
                }
                else
                {
                    // We found a sub-dir, process it
                    CORE::CString subDirPath = srcDir;
                    CORE::AppendToPath( subDirPath, entryName );
                    
                    BuildFileList( subDirPath   ,
                                   files        ,
                                   fileTypes    ,
                                   dirsToIgnore );
                }
            }            
        } 
        while ( CORE::DI_Next_Dir_Entry( dirEntry ) != 0 );
        
        // clean up our toys
        CORE::DI_Cleanup( dirEntry );
    }
}

/*-------------------------------------------------------------------------*/

const TStringSet&
GetIncludeFileTypes( void )
{GUCEF_TRACE;

    static TStringSet fileTypes;
    if ( fileTypes.empty() )
    {
        fileTypes.insert( "h" );
        fileTypes.insert( "hpp" );
    }
    return fileTypes;
}

/*-------------------------------------------------------------------------*/

const TStringSet&
GetBinaryFileTypes( void )
{GUCEF_TRACE;

    static TStringSet fileTypes;
    if ( fileTypes.empty() )
    {
        fileTypes.insert( "pdb" );
        fileTypes.insert( "dll" );
        fileTypes.insert( "so" );
        fileTypes.insert( "manifest" );
    }
    return fileTypes;
}

/*-------------------------------------------------------------------------*/

void
BuildSourceFileList( const CORE::CString& srcDir    ,
                     TMatchEntryVector& matches     ,
                     const TStringSet* fileTypes    ,
                     const TStringSet* dirsToIgnore )
{GUCEF_TRACE;

    // get a list of all the files
    TFileEntryVector files;
    BuildFileList( srcDir, files, fileTypes, dirsToIgnore );
    
    // add entry for each file
    TFileEntryVector::iterator i = files.begin();
    while ( i != files.end() ) 
    {
        TMatchEntry matchEntry;
        matchEntry.source = (*i);        
        matches.push_back( matchEntry );        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
FindMatchesForFile( const TFileEntryVector& totalSet ,
                    const CORE::CString& filename    ,
                    TFileEntryVector& matches        )
{GUCEF_TRACE;

    TFileEntryVector::const_iterator i = totalSet.begin();
    while ( i != totalSet.end() ) 
    {
        const TFileEntry& fileEntry = (*i);        
        if ( fileEntry.filename == filename )
        {
            matches.push_back( fileEntry );
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
FindMatchesForSourceFiles( TMatchEntryVector& matches     ,
                           const CORE::CString& destDir   ,
                           const TStringSet* fileTypes    ,
                           const TStringSet* dirsToIgnore )
{GUCEF_TRACE;

    // get a list of all destination files
    TFileEntryVector destFiles;
    BuildFileList( destDir, destFiles, fileTypes, dirsToIgnore );
    
    // Find destination matches for each source file
    TMatchEntryVector::iterator i = matches.begin();
    while ( i != matches.end() ) 
    {
        TMatchEntry& matchEntry = (*i);
        
        FindMatchesForFile( destFiles                  ,
                            matchEntry.source.filename ,
                            matchEntry.destinations    );       
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
LocateFileMatches( const CORE::CString& srcDir    ,
                   const CORE::CString& destDir   , 
                   TMatchEntryVector& matches     ,
                   const TStringSet* fileTypes    ,
                   const TStringSet* dirsToIgnore )
{GUCEF_TRACE;

    // first build a list of source files
    BuildSourceFileList( srcDir, matches, fileTypes, dirsToIgnore );
    
    // match source files to destination files
    FindMatchesForSourceFiles( matches, destDir, fileTypes, dirsToIgnore );
}

/*-------------------------------------------------------------------------*/

void
CopyOverMatchedFiles( TMatchEntryVector& matches )
{GUCEF_TRACE;

    TMatchEntryVector::iterator i = matches.begin();
    while ( i != matches.end() )
    {
        TMatchEntry& matchEntry = (*i);
        
        CORE::CString sourceFilePath = matchEntry.source.filedir;
        CORE::AppendToPath( sourceFilePath, matchEntry.source.filename );
        
        TFileEntryVector& destinations = matchEntry.destinations;
        TFileEntryVector::iterator n = destinations.begin(); 
        while ( n != destinations.end() )
        {
            TFileEntry& destEntry = (*n);
            
            CORE::CString destFilePath = destEntry.filedir;
            CORE::AppendToPath( destFilePath, destEntry.filename );
            
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Preparing to copy file from \"" + sourceFilePath + "\" to \"" + destFilePath + "\"" );            
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Deleting destination file \"" + destFilePath + "\"" ); 
            if ( 0 != CORE::Delete_File( destFilePath.C_String() ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully deleted destination file \"" + destFilePath + "\"" );
                
                if ( 0 != CORE::Copy_File( destFilePath.C_String()   , 
                                           sourceFilePath.C_String() ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully copied file from \"" + sourceFilePath + "\" to \"" + destFilePath + "\"" );
                }
                else
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Failed to copy file from \"" + sourceFilePath + "\" to \"" + destFilePath + "\"" );
                }
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Failed to delete destination file \"" + destFilePath + "\"" );
            }
            
            ++n;
        }        
        ++i;
    }
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

/*-------------------------------------------------------------------------*/

void
PrintHeader( void )
{GUCEF_TRACE;

    cls();
    printf( "*****************************************\n" );
    printf( "*                                       *\n" );
    printf( "*  Reference Updater                    *\n" );
    printf( "*                                       *\n" );
    printf( "*****************************************\n" );
    printf( "\n" );
    printf( " - Tool Version %f\n" , VERSION_NUMBER );    
}

/*-------------------------------------------------------------------------*/

void
PrintHelp( void )
{GUCEF_TRACE;

    printf( "\n" );
    printf( " - Program arguments:\n" );
    printf( "  Arguments should be passed in the form \'key=value\'\n" );
    printf( "    'SrcIncludeDir' : path to the source dir with the includes\n" );
    printf( "    'DstIncludeDir' : path to the destination dir with the includes\n" );
    printf( "    'SrcBinDir'     : path to the source dir with the binaries\n" );
    printf( "    'DstBinDir'     : path to the destination dir with the binaries\n" );
    printf( "    'DirsToIgnore'  : optional param of ; seperated directory names\n" );
    printf( "                      of directories that should be ignored\n" );
}

/*-------------------------------------------------------------------------*/

TStringSet
VectorToSet( const TStringVector& src )
{GUCEF_TRACE;

    TStringSet set;
    TStringVector::const_iterator i = src.begin();
    while ( i != src.end() )
    {
        set.insert( (*i) );
        ++i;
    }
    return set;
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
    CORE::AppendToPath( logFilename, "ReferenceUpdater_Log.txt" );
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
	                 
        if ( !argList.HasKey( "SrcIncludeDir" )  ||
             !argList.HasKey( "DestIncludeDir" ) ||
             !argList.HasKey( "SrcBinDir" )      ||
             !argList.HasKey( "DstBinDir" )       )
        {
            printf( "ERROR: Not enough parameters where provided\n\n" );
            PrintHelp();
            getchar();
            return 1;
        }
        
        CORE::CString srcIncludeDir = argList.GetValueAlways( "SrcIncludeDir" );
        CORE::CString destIncludeDir = argList.GetValueAlways( "DestIncludeDir" );
        CORE::CString srcBinDir = argList.GetValueAlways( "SrcBinDir" );
        CORE::CString destBinDir = argList.GetValueAlways( "DestBinDir" );
        TStringSet dirsToIgnore = VectorToSet( argList.GetValueAlways( "DirsToIgnore" ).ParseElements( ';', false ) );
        
        
        // match the includes
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Locating file matches for the include files" );
        TMatchEntryVector includeMatches;
        LocateFileMatches( srcIncludeDir, destIncludeDir, includeMatches, &GetIncludeFileTypes(), &dirsToIgnore );
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Matched " + CORE::UInt32ToString( includeMatches.size() ) + " include files" );
        
        // match the binaries
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Locating file matches for the binary files" );
        TMatchEntryVector binaryMatches;
        LocateFileMatches( srcIncludeDir, destIncludeDir, binaryMatches, &GetBinaryFileTypes(), &dirsToIgnore );
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Matched " + CORE::UInt32ToString( binaryMatches.size() ) + " binary files" );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Commencing copy of include files,..." );
        CopyOverMatchedFiles( includeMatches );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Commencing copy of binary files,..." );
        CopyOverMatchedFiles( binaryMatches );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/
