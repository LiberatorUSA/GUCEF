/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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

bool
CPatchSetGenerator::GeneratePatchSet( const CORE::CString& localRoot ,
                                      const CORE::CString& URLRoot   ,
                                      TPatchSet& patchSet            ) const
{TRACE;

    CORE::CString rootDir;
    CORE::CString subDir = CORE::LastSubDir( localRoot );
    rootDir.Set( localRoot.C_String(), subDir.Length() );
    CORE::CString URLRootDir = URLRoot + '/' + subDir;
    
    TDirEntry dirEntry;
    dirEntry.name = subDir;
    if ( GeneratePatchSet( rootDir    ,
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
                                      TDirEntry& currentDir          ) const
{TRACE;

    currentDir.sizeInBytes = 0;
    currentDir.hash = currentDir.name;
    
    struct CORE::SDI_Data* dirEntry = CORE::DI_First_Dir_Entry( localRoot.C_String() );
    if ( dirEntry != NULL )    
    {
        do 
        {
            if ( 0 == CORE::DI_Is_It_A_File( dirEntry ) )
            {
                // We found a sub-sir
                TDirEntry subDirs;
                subDirs.name = CORE::DI_Name( dirEntry );
                
                CORE::CString subDirPath = localRoot;
                CORE::AppendToPath( subDirPath, subDirs.name );
                CORE::CString URLRootPlusDir = URLRoot + '/' + subDirs.name;
                
                if ( GeneratePatchSet( subDirPath     ,
                                       URLRootPlusDir ,
                                       subDirs        ) )
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
                fileEntry.name = CORE::DI_Name( dirEntry );
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
            
        } while ( CORE::DI_Next_Dir_Entry( dirEntry ) != 0 );
        
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
                                      CORE::CDataNode& patchSet      ) const
{TRACE;

    TPatchSet patchSetData;
    if ( GeneratePatchSet( localRoot    ,
                           URLRoot      ,
                           patchSetData ) )
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
                                      CORE::CIOAccess& patchSetStorage  ) const
{TRACE;

    CORE::CDataNode patchSet;
    if ( GeneratePatchSet( localRoot ,
                           URLRoot   ,
                           patchSet  ) )
    {
        // Get the required codec for the patch set storage conversion
        CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = CORE::CDStoreCodecRegistry::Instance()->Lookup( storageCodec );
        if ( NULL != codecPtr )
        {
            // decode the data in our buffer into a data tree
            return codecPtr->StoreDataTree( &patchSet        ,
                                            &patchSetStorage );
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