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

#ifndef ARCHIVEDIFFLIB_H
#include "ArchiveDiffLib.h"
#define ARCHIVEDIFFLIB_H
#endif /* ARCHIVEDIFFLIB_H ? */

/*-------------------------------------------------------------------------*/

using namespace GUCEF;
using namespace ARCHIVEDIFF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define VERSION_NUMBER  1.0

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

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
    printf( "    'ArchiveDiffFile'  : path to a archive diff file\n" );
    printf( "    'ArchiveRootPath'  : path to root of the archive the diff file applies too\n" );
    printf( "    'InfoOutputDir'    : optional param: path to dir where you want to tool output to be stored\n" );
    printf( "    'Plugins'          : optional param: comma seperated list of plugins to load\n" );
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
	                 
        if ( !argList.HasKey( "ArchiveDiffFile" ) ||
             !argList.HasKey( "ArchiveRootPath" ) )
        {
            printf( "ERROR: Not enough parameters where provided\n\n" );
            PrintHelp();
            getchar();
            return 1;
        }
        
        CORE::CString archiveDiffFile = argList[ "ArchiveDiffFile" ];
        CORE::CString archiveRootPath = argList[ "ArchiveRootPath" ];
        CORE::CString pluginsListStr = argList.GetValueAlways( "Plugins" );
        CORE::CString infoOutputDir = argList.GetValueAlways( "InfoOutputDir" );
        if ( 0 == infoOutputDir.Length() )
        {
            infoOutputDir = CORE::RelativePath( "$MODULEDIR$" );
        }
        
        TFileStatusVector fileStatusList;
        if ( LoadFileStatusList( archiveDiffFile , 
                                 fileStatusList  ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded the file status list" );
            
            // do svn stuff
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load and parse the index for the template archive" );
        }
        
    }
	return 0;
}

/*-------------------------------------------------------------------------*/
