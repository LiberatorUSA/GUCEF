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

#ifndef GUCEF_PATCHER_CPATCHLISTPARSER_H
#define GUCEF_PATCHER_CPATCHLISTPARSER_H
 
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

/**
 *  Parser that parses a data node tree for the information required to
 *  build a patch list. Any and all corrupt entries will be dropped.
 */
class GUCEFPATCHER_EXPORT_CPP CPatchListParser
{
    public:
    
    struct SPatchSetLocation
    {
        CORE::CString URL;          /**< URL where the patch set can be retrieved */
        CORE::CString codec;        /**< codec that should be used on the raw patch set data once it's retrieved */
    };
    typedef struct SPatchSetLocation TPatchSetLocation;
    typedef std::vector< TPatchSetLocation > TPatchSetLocations;
    typedef std::map< CORE::CString, TPatchSetLocations > TPatchList;
    
    CPatchListParser( void );    
       
    virtual ~CPatchListParser();
    
    void ParsePatchList( const CORE::CDataNode& patchListData ,
                         TPatchList& patchList                ) const;
   
    private:
    
    CPatchListParser( const CPatchListParser& src ); 
    CPatchListParser& operator=( const CPatchListParser& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHLISTPARSER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 25-12-2006 :
        - Dinand: rewrote this class, it's far simpler now. Simply parses the 
          given patch list data tree for information and stores it into a  
          structure that is easy to use.
- 07-10-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/