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

#ifndef GUCEF_PATCHER_CPATCHSETENGINE_H
#define GUCEF_PATCHER_CPATCHSETENGINE_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CDVString.h"
#include "CURLDataRetriever.h"

#include "gucefPATCHER_CPatchSetParser.h"
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
class EXPORT_CPP CPatchSetEngine : public CORE::CObservingNotifier
{
    public:

    typedef CPatchSetParser::TFileLocation TFileLocation;
    typedef CPatchSetParser::TFileEntry TFileEntry;
    typedef CPatchSetParser::TDirEntry TDirEntry;
    typedef CPatchSetParser::TPatchSet TPatchSet;
    
    public:
    
    bool Start( const TPatchSet& pathSet ,
                CORE::CString& localRoot );
    
    void Stop( void );
    
    bool IsActive( void ) const;
    
    private:
    
    TPatchSet* m_patchSet;
    CPatchSetDirEngine* m_patchSetDirEngine;
    bool m_isActive;
    bool m_stopSignalGiven;
    CORE::CString m_localRoot;
    CORE::CString m_localPath;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHSETENGINE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/