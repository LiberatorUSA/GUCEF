/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef GUCEF_PATCHER_CPATCHSETPARSER_H
#define GUCEF_PATCHER_CPATCHSETPARSER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include <vector>
#include "CDataNode.h"
#include "CDVString.h"

#include "gucefPATCHER_macros.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/ 

class GUCEFPATCHER_EXPORT_CPP CPatchSetParser
{
    public:
    
    struct SFileLocation
    {
        CORE::CString URL;          /**< URL where the file can be retrieved */
        CORE::CString codec;        /**< codec that should be used on the file once it's retrieved (if any) */
        CORE::CString codecParams;  /**< parameters for the codec that should be used on the file once it's retrieved (if any) */
    };
    typedef struct SFileLocation TFileLocation;
    typedef std::vector< TFileLocation > TFileLocations;
    
    struct SFileEntry
    {
        CORE::CString name;
        UInt32 sizeInBytes;
        CORE::CString hash;
        TFileLocations fileLocations;
    };
    typedef struct SFileEntry TFileEntry;
    
    struct SDirEntry
    {
        CORE::CString name;
        UInt32 sizeInBytes;
        CORE::CString hash;
        std::vector< TFileEntry > files;
        std::vector< struct SDirEntry > subDirs;
    };
    typedef struct SDirEntry TDirEntry;

    typedef std::vector< TDirEntry > TPatchSet;
    
    public:
    
    CPatchSetParser( void );
    
    ~CPatchSetParser();

    bool ParsePatchSet( const CORE::CDataNode& patchSetData ,
                        TPatchSet& patchSet                 ) const;
    
    bool ParsePatchSet( const TPatchSet& patchSetData ,
                        CORE::CDataNode& patchSet     ) const;

    private:
    
    CPatchSetParser( const CPatchSetParser& src );
    CPatchSetParser& operator=( const CPatchSetParser& src );
    
    bool ParseAndWalkDirTree( const TDirEntry& patchSetDir   ,
                              CORE::CDataNode& parentDirNode ) const;

    bool ParseTopLevelDir( const CORE::CDataNode& patchSetDirNode ,
                           TPatchSet& patchSet                    ) const;

    bool ParseAndWalkDirTree( const CORE::CDataNode& patchSetDirNode ,
                              TDirEntry& parentDir                   ) const;
                   
    bool ValidateAndParseFileEntry( const CORE::CDataNode& patchSetFileNode ,
                                    TFileEntry& fileEntry                   ) const;

    bool ValidateAndParseFileLocEntries( const CORE::CDataNode& patchSetFileNode ,
                                         TFileEntry& fileEntry                   ) const;

    bool ValidateAndParseDirEntry( const CORE::CDataNode& patchSetDirNode ,
                                   TDirEntry& dirEntry                    ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHSETPARSER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-12-2006 :
        - rewrote this class. It is now simply a parser of a data tree into
          a more concrete data structure. The data structure is validated
          while it is parsed.
- 06-05-2005 :
        - Initial version

-----------------------------------------------------------------------------*/