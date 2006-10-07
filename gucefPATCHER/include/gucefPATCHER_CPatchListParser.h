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
#include "CURL.h"
#include "CObservingNotifier.h"

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

class EXPORT_CPP CPatchListParser : public CORE::CObservingNotifier
{
    public:
    
    static const CORE::CString PatchListRetrievalBeginEvent;
    static const CORE::CString PatchListRetrievalEndEvent;
    static const CORE::CString PatchListRetrievalErrorEvent;
    static const CORE::CString PatchListEntryEvent;
    static const CORE::CString PatchSetEntryEvent;
    static const CORE::CString PatchSetRetrievalBeginEvent;
    static const CORE::CString PatchSetRetrievalEndEvent;
    static const CORE::CString PatchSetRetrievalErrorEvent;
    static const CORE::CString ParserErrorEvent;
        
    public:
    
    CPatchListParser( void );    
       
    virtual ~CPatchListParser();
    
    void ProcessPatchList( const CORE::CDataNode& patchList );
    
    bool ProcessPatchList( const CORE::CString& patchListURL );
    
    bool IsActive( void ) const;
    
    void Stop( void );

    CORE::CEvent GetPatchListRetrievalBeginEventID( void ) const;
    CORE::CEvent GetPatchListRetrievalEndEventID( void ) const;
    CORE::CEvent GetPatchListRetrievalErrorEventID( void ) const;
    CORE::CEvent GetPatchListEntryEventID( void ) const;
    CORE::CEvent GetPatchSetEntryEventID( void ) const;
    CORE::CEvent GetPatchSetRetrievalBeginEventID( void ) const;
    CORE::CEvent GetPatchSetRetrievalEndEventID( void ) const;
    CORE::CEvent GetPatchSetRetrievalErrorEventID( void ) const;
    CORE::CEvent GetParserErrorEventID( void ) const;

    protected:
    
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );    
    
    private:
    
    CPatchListParser( const CPatchListParser& src ); 
    CPatchListParser& operator=( const CPatchListParser& src );
    
    private:
    
    CORE::CEvent m_patchListRetrievalBeginEventID;
    CORE::CEvent m_patchListRetrievalEndEventID;
    CORE::CEvent m_patchListRetrievalErrorEventID;
    CORE::CEvent m_patchListEntryEventID;
    CORE::CEvent m_patchSetEntryEventID;
    CORE::CEvent m_patchSetRetrievalBeginEventID;
    CORE::CEvent m_patchSetRetrievalEndEventID;
    CORE::CEvent m_patchSetRetrievalErrorEventID;
    CORE::CEvent m_parserErrorEventID;
    CORE::CURL m_url;
    bool m_isActive;
    bool m_retrievingList;
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

- 07-10-2006 :
        - Initial version

-----------------------------------------------------------------------------*/