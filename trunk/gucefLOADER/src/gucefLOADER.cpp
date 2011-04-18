/*
 *  gucefLOADER: GUCEF module which loads up the GUCEF platform
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
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

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefLOADER.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
FindAnyParamKey( char** argv    ,
                 int argc       ,
                 int startIndex ,
                 int* keyIndex  )
{
    int i=startIndex;
    
    *keyIndex = -1;
    for ( i; i<argc; ++i )
    {
        if ( argv[ i ][ 0 ] == '-' )
        {
            *keyIndex = i;
            return;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
FindParamKey( char** argv        ,
              int argc           ,
              int startIndex     ,
              const char* keyStr ,
              int* keyIndex      )
{
    int curKeyIndex = -1;    
    do
    {
        FindAnyParamKey( argv         ,
                         argc         ,
                         startIndex   ,
                         &curKeyIndex );

        if ( curKeyIndex > -1 )
        {
            if ( 0 == strcmp( argv[ curKeyIndex ], keyStr ) )
            {
                *keyIndex = curKeyIndex;
                return; 
            }
        }
        startIndex = curKeyIndex+1;
    }
    while ( curKeyIndex  > -1 );    
}

/*-------------------------------------------------------------------------*/

void
FindParam( const char* paramKey ,
           int* paramStartIndex ,
           int* paramEndIndex   ,
           char** argv          ,
           int argc             )
{
    int keyIndex = -1;
    
    // Init return values
    *paramStartIndex = -1;
    *paramEndIndex = -1;
    
    // First try to locate the requested key
    FindParamKey( argv, argc, 0, paramKey, &keyIndex );
    if ( keyIndex > -1 )
    {
        // Found it, now find the end delimeter which is either another key
        // or the end of the string list
        *paramStartIndex = keyIndex;
        keyIndex = -1;        
        
        FindAnyParamKey( argv       , 
                         argc       ,
                         keyIndex+1 ,
                         &keyIndex  );
                         
        if ( keyIndex > -1 )
        {
            // end delimiter is another key
            *paramEndIndex = keyIndex;
        }
        else
        {
            // end delimiter is the end of the string set
            *paramEndIndex = argc-1;
        }
    }
}

/*-------------------------------------------------------------------------*/

CORE::CString
GetModuleRootPath( char** argv ,
                   int argc    )
{
    CORE::CString moduleRoot;
    int paramStartIndex = -1;
    int paramEndIndex = -1;
    
    FindParam( "-moduleRoot"    ,
               &paramStartIndex ,
               &paramEndIndex   ,
               argv             ,
               argc             );
               
    if ( ( paramStartIndex > -1 && paramEndIndex > -1 ) &&
         ( paramStartIndex != paramEndIndex > -1 )       ) 
    {
        if ( paramStartIndex+1 < argc-1 )
        {
            moduleRoot = CORE::RelativePath( argv+paramStartIndex+1 );
            return;
        }
    }

    if ( moduleRoot.IsNULLOrEmpty() )
    {
        moduleRoot = CORE::RelativePath( "$MODULEDIR$" );
    }
    
    return moduleRoot;
}

/*-------------------------------------------------------------------------*/

void
ParseListOfExtraModulestoLoad( char** argv        ,
                               int argc           ,
                               char*** moduleList ,
                               int* moduleCount   )
{
    int paramStartIndex = -1;
    int paramEndIndex = -1;
    
    FindParam( "-gucefModules",
               &paramStartIndex ,
               &paramEndIndex   ,
               argv             ,
               argc             );
               
    if ( ( paramStartIndex > -1 && paramEndIndex > -1 ) &&
         ( paramStartIndex != paramEndIndex > -1 )       ) 
    {
        if ( paramStartIndex+1 < argc-1 )
        {
            *moduleList = argv+paramStartIndex+1;
            *moduleCount = (paramEndIndex-paramStartIndex)+1;
            return;
        }
    }

    *moduleList = NULL;
    *moduleCount = 0;
}

/*-------------------------------------------------------------------------*/

void
GetHighestVersionAvailableFromDir( const CORE::CString rootDir ,
                                   long& patchVersion          ,
                                   long& releaseVersion        )
{

    
}

/*-------------------------------------------------------------------------*/

int
LoadGucefPlatformImp( unsigned long mayorVersion   ,
                      unsigned long minorVersion   ,
                      long patchVersion            ,
                      long releaseVersion          ,
                      char** argv                  ,
                      int argc                     )
{
    // get a list of optional gucef modules that should be loaded
    char** moduleList = NULL;
    int moduleCount = 0;
    ParseListOfExtraModulestoLoad( argv         , 
                                   argc         ,
                                   &moduleList  ,
                                   &moduleCount );
                                   
    // get the root path to where we should load modules from
    CORE::CString moduleRoot = GetModuleRootPath( argv, argc );
    
    // adjust path for desired version
    char versionDir[ 41 ];
    sprintf( versionDir, "%d.%d", mayorVersion, minorVersion );
    CORE::AppendToPath( moduleRoot, versionDir );
    
    if ( patchVersion < 0 || releaseVersion < 0 )
    {
        // We will have to search for the latest version available
        GetHighestVersionAvailableFromDir( moduleRoot, patchVersion, releaseVersion );
        if ( patchVersion < 0 || releaseVersion < 0 )
        {
            // Unable to find any versions in this dir
            return -1;
        }
    }

    // Simply append with the specific version requested
    sprintf( versionDir, "%d.%d", patchVersion, releaseVersion );
    CORE::AppendToPath( moduleRoot, versionDir );
    
    /* check if all modules are present */
    CORE::CString filePath;
    
    
    
    /* load the modules into memory */
    
    return -1;
}

/*-------------------------------------------------------------------------*/

int
LoadGucefPlatform( unsigned long mayorVersion   ,
                   unsigned long minorVersion   ,
                   char** argv                  ,
                   int argc                     )
{

    return LoadGucefPlatformImp( mayorVersion ,
                                 minorVersion ,
                                 -1           ,
                                 -1           ,
                                 argv         ,
                                 argc         );
}


/*-------------------------------------------------------------------------*/

int
LoadGucefPlatformEx( unsigned long mayorVersion   ,
                     unsigned long minorVersion   ,
                     unsigned long patchVersion   ,
                     unsigned long releaseVersion ,
                     char** argv                  ,
                     int argc                     )
{

    return LoadGucefPlatformImp( mayorVersion          ,
                                 minorVersion          ,
                                 (long) patchVersion   ,
                                 (long) releaseVersion ,
                                 argv                  ,
                                 argc                  );
}

/*-------------------------------------------------------------------------*/
