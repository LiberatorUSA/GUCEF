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

#ifndef GUCEF_PATCHER_CPATCHSETPARSERREVENTS_H
#define GUCEF_PATCHER_CPATCHSETPARSERREVENTS_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#include "gucefPATCHER_CPatchListParser.h"
#include "gucefPATCHER_CPatchSetParser.h"

#ifndef GUCEF_PATCHER_MACROS_H
#include "gucefPATCHER_macros.h"
#define GUCEF_PATCHER_MACROS_H
#endif /* GUCEF_PATCHER_MACROS_H ? */

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

class EXPORT_CPP CPatcher : public CORE::CObservingNotifier
{
    public:

    typedef CPatchListParser::TPatchSetLocation TPatchSetLocation;
    typedef CPatchListParser::TPatchSetLocations TPatchSetLocations;
    typedef CPatchListParser::TPatchList TPatchList;
    typedef CPatchSetParser::TPatchSet TFileLocation;
    typedef CPatchSetParser::TPatchSet TFileEntry;
    typedef CPatchSetParser::TPatchSet TPatchSet;
    
    bool ProcessPatchList( const TPatchList& patchList    ,
                           const CORE::CString& localRoot );
    
    bool ProcessPatchSet( const TPatchSet& patchSet      ,
                          const CORE::CString& localRoot );
    
    private:
    CORE::CURLDataRetriever m_urlDataRetriever;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHSETPARSERREVENTS_H ? */