/*
 *  gucefPATCHER: GUCEF RAD module providing a patch delivery system
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

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#include "dvstrutils.h"

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#include "gucefPATCHER_CPatchSetGenerator.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetGenerator::CPatchSetGenerator( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPatchSetGenerator::~CPatchSetGenerator()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetGenerator::GeneratePatchSet( const CORE::CString& localRoot ,
                                      const CORE::CString& URLRoot   ,
                                      TPatchSet& patchSet            ,
                                      const TStringSet* dirsToIgnore ) const
{GUCEF_TRACE;

    CORE::CString subDir = CORE::LastSubDir( localRoot );
    
    if ( NULL != dirsToIgnore )
    {       
        if ( dirsToIgnore->find( subDir ) != dirsToIgnore->end() )
        {
            // the current subDir is a dir we are instructed to ignore
            return false;
        }
    }
    
    CORE::CString URLRootDir = URLRoot + '/' + subDir;
    
    TDirEntry dirEntry;
    dirEntry.name = subDir;
    if ( GeneratePatchSet( localRoot  ,
                           URLRootDir ,
                           dirEntry   ) )
    {
        patchSet.push_back( dirEntry );
        return true;
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetGenerator::GeneratePatchSet( const CORE::CString& localRoot ,
                                      const CORE::CString& URLRoot   ,
                                      TDirEntry& currentDir          ,
                                      const TStringSet* dirsToIgnore ) const
{GUCEF_TRACE;

    currentDir.sizeInBytes = 0;
    currentDir.hash = currentDir.name;
    
    struct CORE::SDI_Data* dirEntry = CORE::DI_First_Dir_Entry( localRoot.C_String() );
    if ( dirEntry != NULL )    
    {
        do 
        {
            CORE::CString entryName = CORE::DI_Name( dirEntry );
            if ( ( entryName != "." )  &&
                 ( entryName != ".." )  )
            {
                if ( 0 == CORE::DI_Is_It_A_File( dirEntry ) )
                {
                    // We found a sub-dir
                    TDirEntry subDirs;
                    subDirs.name = entryName;
                    
                    CORE::CString subDirPath = localRoot;
                    CORE::AppendToPath( subDirPath, subDirs.name );
                    CORE::CString URLRootPlusDir = URLRoot + '/' + subDirs.name;
                    
                    if ( GeneratePatchSet( subDirPath     ,
                                           URLRootPlusDir ,
                                           subDirs        ,
                                           dirsToIgnore   ) )
                    {
                        currentDir.sizeInBytes += subDirs.sizeInBytes;
                        currentDir.hash += subDirs.hash;
                        currentDir.subDirs.push_back( subDirs );
                    }               
                }
                else
                {
                    // We found a file in this dir
                    TFileEntry fileEntry;
                    fileEntry.name = entryName;
                    fileEntry.sizeInBytes = CORE::DI_Size( dirEntry );
                    CORE::CString filePath = localRoot;
                    CORE::AppendToPath( filePath, fileEntry.name );
                    CORE::CFileAccess fileAccess( filePath, "rb" );
                    fileAccess.Open();
                    
                    UInt8 md5Digest[ 16 ];
                    if ( 0 == CORE::md5frommfile( fileAccess.CStyleAccess() ,
                                                  md5Digest                 ) )
                    {
                        // clean up our toys
                        CORE::DI_Cleanup( dirEntry );
                        return false;
                    }
                    
                    fileEntry.hash = CORE::MD5ToString( md5Digest );
                    
                    currentDir.hash += fileEntry.hash;
                    currentDir.sizeInBytes += fileEntry.sizeInBytes;
                    
                    TFileLocation location;
                    location.URL = URLRoot + '/' + fileEntry.name;
                    fileEntry.fileLocations.push_back( location );                
                    currentDir.files.push_back( fileEntry );
                }
            }            
        } 
        while ( CORE::DI_Next_Dir_Entry( dirEntry ) != 0 );
        
        // clean up our toys
        CORE::DI_Cleanup( dirEntry );
        
        currentDir.hash = CORE::StringToMD5String( currentDir.hash );
        
        return true;
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetGenerator::GeneratePatchSet( const CORE::CString& localRoot ,
                                      const CORE::CString& URLRoot   ,
                                      CORE::CDataNode& patchSet      ,
                                      const TStringSet* dirsToIgnore ) const
{GUCEF_TRACE;

    TPatchSet patchSetData;
    if ( GeneratePatchSet( localRoot    ,
                           URLRoot      ,
                           patchSetData ,
                           dirsToIgnore ) )
    {
        CPatchSetParser parser;
        return parser.ParsePatchSet( patchSetData ,
                                     patchSet     );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetGenerator::GeneratePatchSet( const CORE::CString& localRoot    ,
                                      const CORE::CString& URLRoot      ,
                                      const CORE::CString& storageCodec ,
                                      CORE::CIOAccess& patchSetStorage  ,
                                      const TStringSet* dirsToIgnore    ) const
{GUCEF_TRACE;

    CORE::CDataNode patchSet;
    if ( GeneratePatchSet( localRoot    ,
                           URLRoot      ,
                           patchSet     ,
                           dirsToIgnore ) )
    {
        // Get the required codec for the patch set storage conversion
        CORE::CDStoreCodecRegistry* codecRegistry = CORE::CDStoreCodecRegistry::Instance();
        if ( codecRegistry->IsRegistered( storageCodec ) )
        { 
            CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = codecRegistry->Lookup( storageCodec );
            if ( NULL != codecPtr )
            {
                // decode the data in our buffer into a data tree
                return codecPtr->StoreDataTree( &patchSet        ,
                                                &patchSetStorage );
            }
        }
    }
    
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/