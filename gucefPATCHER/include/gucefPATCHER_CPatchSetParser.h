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

class CPatchSetParserEventHandler;

/*-------------------------------------------------------------------------*/

class EXPORT_CPP CPatchSetParser
{
    public:
    
    struct SSourceInfo
    {
        CORE::CString Tag;
        CORE::CString URL;
        CORE::CString Alternative;
        CORE::CString Codec;
        CORE::CString CodecParams;
    };
    typedef struct SSourceInfo TSourceInfo;

    typedef std::map< CORE::CString, TSourceInfo > TSourceList;
        
    public:
    
    CPatchSetParser( void );
    
    CPatchSetParser( const CPatchSetParser& src );
    
    ~CPatchSetParser();
    
    CPatchSetParser& operator=( const CPatchSetParser& src );
    
    /**
     *  High-level parser, extracts the source list and starts
     *  calling the events handlers for each appropriote event
     */
    void 
    ProcessPatchList( const CORE::CDataNode& patchList          ,
                      CPatchSetParserEventHandler& eventHandler );

    /**
     *  Allows you to parse a source list from the given data tree
     */
    void
    ParseSourceList( const CORE::CDataNode& rootNode ,
                     TSourceList& sourceList         );
    
    /**
     *  Merge capability. If so desired you can subtract the old from the 
     *  new patchset local list.
     *  It simply copies all the diffrences into the diffList 
     */
    void 
    SubtractOldFromNewLocalList( const CORE::CDataNode& oldLocalList ,
                                 const CORE::CDataNode& newLocalList ,
                                 CORE::CDataNode& diffList           );
    
    private:
    
    void 
    ParseSourceList( TSourceList& sourceList ,
                     CORE::CDataNode* n      );
                          
    void
    ProcessPatchListImp( const CORE::CDataNode& rootNode           ,
                         const CORE::CString localPath             ,
                         CPatchSetParserEventHandler& eventHandler );

    void 
    ProcessPatchList( const TSourceList& sourceList ,
                      CORE::CDataNode* n            );
                           
    const CORE::CDataNode*
    FindPathInLocalListImp( const CORE::CDataNode& currentNode ,
                            const CORE::CString& path          );

    const CORE::CDataNode*
    FindPathInLocalList( const CORE::CDataNode& rootNode ,
                         const CORE::CString& path       );
                         
    void
    SubtractOldFromNewLocalListImp( const CORE::CDataNode& oldLocalListRoot ,
                                    const CORE::CDataNode& newLocalListNode ,
                                    const CORE::CString& localPath          ,
                                    CORE::CDataNode& diffList               );
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

- 06-05-2005 :
        - Initial version

-----------------------------------------------------------------------------*/