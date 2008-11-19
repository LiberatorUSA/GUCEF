/*
 *  DuplicateFileFinder: Tool for locating duplicate files
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <windows.h>
#undef min
#undef max

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_VFS_H
#include "gucefVFS.h"
#define GUCEF_VFS_H
#endif /* GUCEF_VFS_H ? */

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
          
#ifdef GUCEF_MSWIN_BUILD

void cls( void )
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
    {GUCEF_TRACE;
    
        AllocConsole();
        
        /* reopen stdin handle as console window input */
        freopen( "CONIN$", "rb", stdin );
        
        /* reopen stout handle as console window output */
        freopen( "CONOUT$", "wb", stdout );
        
        /* reopen stderr handle as console window output */
        freopen( "CONOUT$", "wb", stderr );        
    }
    
    ~CMSWin32ConsoleWindow()
    {GUCEF_TRACE;
    
        FreeConsole();
    }
};

#endif /* GUCEF_MSWIN_BUILD ? */

/*---------------------------------------------------------------------------*/

void
PrintHeader( void )
{GUCEF_TRACE;

    cls();
    printf( "*****************************************\n" );
    printf( "*                                       *\n" );
    printf( "*  Duplicate File Finder                *\n" );
    printf( "*                                       *\n" );
    printf( "*    Copyright (C) Dinand Vanvelzen     *\n");
    printf( "*    2002 - 2007. All rights reserved.  *\n");
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
    printf( "  Arguments should be passed in the form \"key=value\"\n" );
    printf( "    'RootDirPath'     : A root directory you wish to include in the search\n" );
    printf( "    'OutputPath'      : The directory where you want the output to be written\n" );
    printf( "    'HashCheck'       : optional parameter: explicitly set whether to perform\n" );
    printf( "                        a hash check on the files, this is a lengthy operation\n" );    
    printf( "    'Filter'          : optional parameter: Filter mask that can be used to\n" );
    printf( "                        exclude files\n" );    
    printf( "    'PluginDir'       : optional parameter: path to a dir where plugins can be\n" );
    printf( "                        found\n" );
    printf( "    'ListCodec'       : optional parameter: the codec to use for the output\n" );
    printf( "                        patch set file, the default codec is 'xml'\n" );
}

/*---------------------------------------------------------------------------*/

void
ParseParams( const int argc                        , 
             char* argv[]                          ,
             GUCEF::CORE::CValueList& keyValueList )
{GUCEF_TRACE;
    
    keyValueList.DeleteAll();
    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        for ( int i=1; i<argc; ++i )
        {
            argString += ( ' ' + argv[ i ] );
        }
        
        keyValueList.SetMultiple( argString, '"' );
    }
}

/*-------------------------------------------------------------------------*/

void
BuildDuplicateList( GUCEF::CORE::CDataNode& dupList        ,
                    const GUCEF::CORE::CDataNode& fullList )
{GUCEF_TRACE;

    // @TODO @MAKEME
}

/*-------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
#ifdef GUCEF_MSWIN_BUILD

int __stdcall
WinMain( HINSTANCE hinstance     ,
         HINSTANCE hprevinstance ,
         LPSTR lpcmdline         ,
         int ncmdshow            )
{

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
               
    #ifdef GUCEF_CORE_DEBUG_MODE
    //GUCEF::CORE::GUCEF_LogStackToStdOut();
    //GUCEF::CORE::GUCEF_SetStackLogging( 1 );
    #endif /* GUCEF_CORE_DEBUG_MODE ? */

    try 
    {                               
        GUCEF::CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
        GUCEF::CORE::AppendToPath( logFilename, "DuplicateFileFinder_Log.txt" );
        GUCEF::CORE::CFileAccess logFileAccess( logFilename, "w" );
        
        GUCEF::CORE::CStdLogger logger( logFileAccess );
        GUCEF::CORE::CLogManager::Instance()->AddLogger( &logger );
        
        #ifdef GUCEF_MSWIN_BUILD
        GUCEF::CORE::CMSWinConsoleLogger consoleOut;
        GUCEF::CORE::CLogManager::Instance()->AddLogger( &consoleOut );
        #endif /* GUCEF_MSWIN_BUILD ? */

        #ifdef GUCEF_MSWIN_BUILD
        CMSWin32ConsoleWindow console;
        #endif

	    if ( 0 == argc )
	    {
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
    	                 
            if ( !argList.HasKey( "RootDirPath" ) ||
                 !argList.HasKey( "OutputPath"  )  )
            {
                printf( "ERROR: Not enough parameters where provided\n\n" );
                PrintHelp();
                getchar();
                return 1;
            }
           
            // Obtain the application arguments
            GUCEF::CORE::CString rootDirPath = argList[ "RootDirPath" ];
            GUCEF::CORE::CString outputPath = argList[ "OutputPath" ];
            GUCEF::CORE::CString listCodec = argList[ "ListCodec" ];
            GUCEF::CORE::CString pluginDir = argList[ "PluginDir" ];
            GUCEF::CORE::CString hashCheckStr = argList[ "HashCheck" ];
            GUCEF::CORE::CString filter = argList[ "Filter" ]; 

            // Prepare some vars
            GUCEF::CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr;
            GUCEF::VFS::CVFS* vfs = GUCEF::VFS::CVFS::Instance();
            bool hashCheck = false;
            
            if ( hashCheckStr.Length() != 0 )
            {
                // Override our default which is false
                hashCheck = GUCEF::CORE::StringToBool( hashCheckStr );
            }
            if ( listCodec.Length() == 0 )
            {
                // Use our codec default which is xml
                listCodec = "xml";
            }
            if ( pluginDir.Length() == 0 )
            {
                // Use our codec default plugin path
                pluginDir = GUCEF::CORE::RelativePath( "$MODULEDIR$\\plugins" );
            }
            
            // Attempt to obtain the code
            codecPtr = GUCEF::CORE::CDStoreCodecRegistry::Instance()->Lookup( listCodec );
            if ( codecPtr == NULL )
            {
                // No point in proceeding if we don't have a valid codec with which we can
                // store the result
                return 0;
            }

            // Load all plugins, this allows us to support multiple codec's
            // with our minimal console interface
            GUCEF::CORE::CPluginControl* pluginControl = GUCEF::CORE::CPluginControl::Instance();
            pluginControl->SetPluginDir( pluginDir );
            pluginControl->LoadAll();

            // Now we ask the VFS to build the list for us.
            // This operation can take a while
            GUCEF::CORE::CDataNode fileListRoot;
            vfs->GetList( fileListRoot ,
                          ""           ,
                          true         ,
                          filter       ,
                          hashCheck    );

            // Store the full list to file
            GUCEF::CORE::CString fullListFilePath( outputPath );
            GUCEF::CORE::AppendToPath( fullListFilePath, "FullDirAndFileIndex." );
            fullListFilePath += listCodec;
            GUCEF::CORE::CFileAccess fullListFile( fullListFilePath );
            
            codecPtr->StoreDataTree( &fileListRoot, &fullListFile );
                                     
            // Now we have all the data we need to start looking for duplicate 
            // files with ease
            GUCEF::CORE::CDataNode duplicateList;
            BuildDuplicateList( duplicateList ,
                                fileListRoot  );
                                
            // Store the duplicate list to file
            GUCEF::CORE::CString duplicateListFilePath( outputPath );
            GUCEF::CORE::AppendToPath( duplicateListFilePath, "DuplicateFileList." );
            duplicateListFilePath += listCodec;
            GUCEF::CORE::CFileAccess duplicateListFile( fullListFilePath );
            
            codecPtr->StoreDataTree( &duplicateList, &duplicateListFile );
                                            
            return 1;
        }
        return 0;
    }
    catch ( GUCEF::CORE::CException& e )
    {
        #ifdef GUCEF_CORE_DEBUG_MODE
        GUCEF::CORE::GUCEF_PrintCallstack();
        GUCEF::CORE::GUCEF_DumpCallstack( "DuplicateFileFinder_callstack.txt" );
        #endif /* GUCEF_CORE_DEBUG_MODE ? */
            
        GUCEF::CORE::CString msg( "GUCEF exception during program execution\n\n" );
        msg += "file: " + GUCEF::CORE::CString( e.GetOriginFile() ) + "\n" + "line: " + GUCEF::CORE::Int32ToString( e.GetOriginLineNr() );
        
        GUCEF::CORE::ShowErrorMessage( "Unhandled GUCEF exception" ,
                                       msg.C_String()              );                  
    }
    catch( std::bad_alloc& e )
    {
        #ifdef GUCEF_CORE_DEBUG_MODE
        GUCEF::CORE::GUCEF_PrintCallstack();
        GUCEF::CORE::GUCEF_DumpCallstack( "DuplicateFileFinder_callstack.txt" );
        #endif /* GUCEF_CORE_DEBUG_MODE ? */
        
        GUCEF::CORE::CString msg( "Memory allocation failure during program execution\n\n" + GUCEF::CORE::CString( e.what() ) );
        GUCEF::CORE::ShowErrorMessage( "Unhandled STL bad_alloc exception" ,
                                       msg.C_String()                      );    
    }
    catch( std::exception& e )
    {
        #ifdef GUCEF_CORE_DEBUG_MODE
        GUCEF::CORE::GUCEF_PrintCallstack();
        GUCEF::CORE::GUCEF_DumpCallstack( "DuplicateFileFinder_callstack.txt" );
        #endif /* GUCEF_CORE_DEBUG_MODE ? */
        
        GUCEF::CORE::CString msg( "STL exception during program execution\n\n" + GUCEF::CORE::CString( e.what() ) );
        GUCEF::CORE::ShowErrorMessage( "Unhandled STL exception" ,
                                       msg.C_String()            );        
    }
    catch ( ... )
    {
        #ifdef GUCEF_CORE_DEBUG_MODE
        GUCEF::CORE::GUCEF_PrintCallstack();
        GUCEF::CORE::GUCEF_DumpCallstack( "DuplicateFileFinder_callstack.txt" );
        #endif /* GUCEF_CORE_DEBUG_MODE ? */
        
        GUCEF::CORE::ShowErrorMessage( "Unknown exception"                                                                 ,
                                       "Unhandled exception during program execution, the application will now terminate"  );                                                         
    }
    return 1;                                                                                                                              
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 10-12-2006
        - Dinand: Added this comment section

---------------------------------------------------------------------------*/