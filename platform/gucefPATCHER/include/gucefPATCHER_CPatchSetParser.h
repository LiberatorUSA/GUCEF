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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
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

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

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
        UInt64 sizeInBytes;
        CORE::CString hash;
        TFileLocations fileLocations;
    };
    typedef struct SFileEntry TFileEntry;
    typedef std::vector< TFileEntry > TFileEntryList;
    
    struct SDirEntry
    {
        CORE::CString name;
        UInt64 sizeInBytes;
        CORE::CString hash;
        TFileEntryList files;
        std::vector< struct SDirEntry > subDirs;
    };
    typedef struct SDirEntry TDirEntry;
    typedef std::vector< TDirEntry >  TDirEntryList;
    typedef TDirEntryList TPatchSet;
    
    public:
    
    CPatchSetParser( void );
    
    ~CPatchSetParser();

    bool ParsePatchSet( const CORE::CDataNode& patchSetData ,
                        TPatchSet& patchSet                 ) const;
    
    bool ParsePatchSet( const TPatchSet& patchSetData ,
                        CORE::CDataNode& patchSet     ) const;

    bool ValidateAndParseFileEntry( const CORE::CDataNode& patchSetFileNode ,
                                    TFileEntry& fileEntry                   ,
                                    bool mustHaveLocationEntries = true     ) const;

    bool ValidateAndParseFileLocEntries( const CORE::CDataNode& patchSetFileNode ,
                                         TFileEntry& fileEntry                   ,
                                         bool mustHaveLocationEntries = true     ) const;

    bool ValidateAndParseDirEntry( const CORE::CDataNode& patchSetDirNode ,
                                   TDirEntry& dirEntry                    ) const;

    void SerializeFileEntry( const TFileEntry& fileEntry ,
                             CORE::CDataNode& parentNode ) const;

    private:
    
    CPatchSetParser( const CPatchSetParser& src );
    CPatchSetParser& operator=( const CPatchSetParser& src );
    
    bool ParseAndWalkDirTree( const TDirEntry& patchSetDir   ,
                              CORE::CDataNode& parentDirNode ) const;

    bool ParseTopLevelDir( const CORE::CDataNode& patchSetDirNode ,
                           TPatchSet& patchSet                    ) const;

    bool ParseAndWalkDirTree( const CORE::CDataNode& patchSetDirNode ,
                              TDirEntry& parentDir                   ) const;
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