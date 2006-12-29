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
class EXPORT_CPP CPatchListParser
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