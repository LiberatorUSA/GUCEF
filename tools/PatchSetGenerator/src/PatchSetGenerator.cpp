/*
 *  PatchSetGenerator: Tool shell using the gucefPATCHER module
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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifdef GUCEF_MSWIN_BUILD
#include <windows.h>
#endif /* GUCEF_MSWIN_BUILD ? */

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

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETGENERATOR_H
#include "gucefPATCHER_CPatchSetGenerator.h"
#define GUCEF_PATCHER_CPATCHSETGENERATOR_H
#endif /* GUCEF_PATCHER_CPATCHSETGENERATOR_H ? */

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

#endif /* GUCEF_MSWIN_BUILD ? */

/*---------------------------------------------------------------------------*/

void
PrintHeader( void )
{GUCEF_TRACE;

    cls();
    printf( "*****************************************\n" );
    printf( "*                                       *\n" );
    printf( "*  Patch Set Generator                  *\n" );
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
    printf( "    'RootDirPath'     : the root directory of the patch set\n" );
    printf( "    'PatchSetOutFile' : the output patch set file\n" );
    printf( "    'URLRoot'         : the root of the URL to which the paths will be appended\n" );
    printf( "    'PluginDir'       : optional parameter: path to a dir where plugins can be\n" );
    printf( "                        found\n" );
    printf( "    'PatchSetOutCodec': optional parameter: the codec to use for the output\n" );
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

/*---------------------------------------------------------------------------*/

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
	                 
        if ( !argList.HasKey( "RootDirPath" )     ||
             !argList.HasKey( "PatchSetOutFile" ) ||
             !argList.HasKey( "URLRoot" )          )
        {
            printf( "ERROR: Not enough parameters where provided\n\n" );
            PrintHelp();
            getchar();
            return 1;
        }
               
        GUCEF::CORE::CString rootDirPath = argList[ "RootDirPath" ];
        GUCEF::CORE::CString patchSetOutFile = argList[ "PatchSetOutFile" ];
        GUCEF::CORE::CString URLRoot = argList[ "URLRoot" ];
        GUCEF::CORE::CString patchSetOutCodec = argList[ "PatchSetOutCodec" ];
        GUCEF::CORE::CString pluginDir = argList[ "PluginDir" ];
        
        if ( patchSetOutCodec.Length() == 0 )
        {
            // Use our codec default which is xml
            patchSetOutCodec = "xml";
        }
        if ( pluginDir.Length() == 0 )
        {
            // Use our codec default plugin path
            pluginDir = GUCEF::CORE::RelativePath( "$MODULEDIR$\\plugins" );
        }

        // Load all plugins, this allows us to support multiple codec's
        // with our minimal console interface
        GUCEF::CORE::CPluginControl* pluginControl = GUCEF::CORE::CPluginControl::Instance();
        pluginControl->SetPluginDir( pluginDir );
        pluginControl->LoadAll();

        // Validate in advance that we have a codec loaded of the desired type
        // This will save us some work if it is not available
        if ( GUCEF::CORE::CDStoreCodecRegistry::Instance()->IsRegistered( patchSetOutCodec ) )
        {
            GUCEF::CORE::CFileAccess fileAccess( patchSetOutFile, "wb" );
            if ( fileAccess.Opened() && fileAccess.IsValid() )
            {        
                GUCEF::PATCHER::CPatchSetGenerator psGenerator;
                if ( psGenerator.GeneratePatchSet( rootDirPath      ,
                                                   URLRoot          ,
                                                   patchSetOutCodec ,
                                                   fileAccess       ) )
                {
                    printf( "SUCCESS: The patch set generation process has been successfully completed\n" );
                    return 0;
                }
                else
                {
                    printf( "ERROR: An error occurred during the patch set generation process\n" );
                    getchar();
                    return 1;
                }
            }
            else
            {
                printf( "ERROR: failed to open output file for writing\n" );
                getchar();
                return 1;        
            }
        }
        else
        {
            printf( "ERROR: no data storage codec is available of the given type\n" );
            getchar();
            return 1;         
        }
	}
	
	return 0;
}

/*---------------------------------------------------------------------------*/
