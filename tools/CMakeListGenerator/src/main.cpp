/*
 *  CMakeListGenerator: Tool to generate CMakeList.txt with headers and sources
 *  Copyright (C) 2002 - 2009.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifdef GUCEF_MSWIN_BUILD
#include <windows.h>
#endif /* GUCEF_MSWIN_BUILD ? */


/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*---------------------------------------------------------------------------*/

void
PopulateFileListFromDir( const CORE::CString& path              , 
                         std::vector< CORE::CString >& fileList )
{GUCEF_TRACE;

    CORE::SDI_Data* sdiData = CORE::DI_First_Dir_Entry( path.C_String() );
    if ( NULL != sdiData )
    {
        do
        {
            if ( 0 != DI_Is_It_A_File( sdiData ) )
            {
                fileList.push_back( DI_Name( sdiData ) );
            }
        }
        while ( 0 != DI_Next_Dir_Entry( sdiData ) );
        DI_Cleanup( sdiData );
    }
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetCMakeListsFileHeader( void )
{GUCEF_TRACE;

    return
    "#-------------------------------------------------------------------\n"
    "# This file is part of the CMake build system for GUCEF\n"
    "#     (Galaxy Unlimited Framework)\n"
    "# For the latest info, see http://www.VanvelzenSoftware.com/\n"
    "#\n"
    "# The contents of this file are placed in the public domain. Feel\n"
    "# free to make use of it in any way you like.\n"
    "#-------------------------------------------------------------------\n\n";
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileSection( const CORE::CString& sectionContent       , 
                               const CORE::CString& filePrefix           , 
                               const std::vector< CORE::CString >& files )
{GUCEF_TRACE;

    CORE::CString newSectionContent = sectionContent;
    std::vector< CORE::CString >::const_iterator i = files.begin();
    while ( i != files.end() )
    {
        newSectionContent += "  " + filePrefix + (*i) + "\n";
        ++i;
    }
    newSectionContent += ")\n";
    return newSectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileIncludeSection( const std::vector< CORE::CString >& includeFiles )
{
    CORE::CString sectionContent = "set(HEADER_FILES \n";
    return GenerateCMakeListsFileSection( sectionContent, "include/", includeFiles );
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileSrcSection( const std::vector< CORE::CString >& includeFiles )
{
    CORE::CString sectionContent = "set(SOURCE_FILES \n";
    return GenerateCMakeListsFileSection( sectionContent, "src/", includeFiles );
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFile( const CORE::CString& projectName                 ,
                        const std::vector< CORE::CString >& includeFiles ,
                        const std::vector< CORE::CString >& srcFiles     ,
                        const CORE::CString& fileSuffix                  )
{GUCEF_TRACE;

    // Set file header comment section
    CORE::CString fileContent = GetCMakeListsFileHeader();
    
    // Set project name comment section
    fileContent += "# Configure " + projectName + "\n\n";
    
    // Add all the include files
    fileContent += GenerateCMakeListsFileIncludeSection( includeFiles );
    
    // Add all the source files
    fileContent += GenerateCMakeListsFileSrcSection( srcFiles );
    
    if ( fileSuffix.Length() > 0 )
    {
        fileContent += fileSuffix;
    }
    else
    {
        // add example section
        fileContent += "# TODO: the following is an example section, you have to complete it\n";
        fileContent += "include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)\n";
        fileContent += "add_definitions(-DTIXML_USE_STL)\n";
        fileContent += "add_executable(" + projectName + " ${HEADER_FILES} ${SOURCE_FILES})\n";
        fileContent += "target_link_libraries(" + projectName + " ${OGRE_LIBRARIES})\n";
        fileContent += "ogre_config_tool(" + projectName + ")\n";
    }
    return fileContent;
}

/*---------------------------------------------------------------------------*/

int
main( int argc , char* argv[] )
{GUCEF_TRACE;

    CORE::CString projectDir = CORE::RelativePath( "$CURWORKDIR$" );    
    CORE::CString projectName = CORE::LastSubDir( projectDir ); 
    
    CORE::CString includeDir = projectDir;
    CORE::AppendToPath( includeDir, "include" );
    CORE::CString srcDir = projectDir;
    CORE::AppendToPath( srcDir, "src" );
    
    std::vector< CORE::CString > includeFiles;
    PopulateFileListFromDir( includeDir, includeFiles );
    
    std::vector< CORE::CString > srcFiles;
    PopulateFileListFromDir( srcDir, srcFiles );
    
    CORE::CString fileSuffix;
    CORE::LoadTextFileAsString( "CMakeListsSuffix.txt", fileSuffix );
    
    CORE::CString fileContent = GenerateCMakeListsFile( projectName, includeFiles, srcFiles, fileSuffix );
    
    fptr = fopen( "CMakeLists.txt", "wb" );
    fwrite( fileContent.C_String(), fileContent.Length(), 1, fptr );
    fclose( fptr );
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

    return main( argc, argv );
}

#endif

/*---------------------------------------------------------------------------*/
