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

#ifndef GUCEF_PATCHER_CPATCHSETFILEENGINE_H
#define GUCEF_PATCHER_CPATCHSETFILEENGINE_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CDVString.h"
#include "CURLDataRetriever.h"
#include "CFileAccess.h"

#include "gucefPATCHER_CPatchSetFileEngineEvents.h"
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
 *  Engine for patching a list of files given.
 *  The files are compared with the ones in the local root that is given.
 */
class GUCEFPATCHER_EXPORT_CPP CPatchSetFileEngine : public CORE::CObservingNotifier  ,
                                                    public CPatchSetFileEngineEvents
{
    public:

    typedef CPatchSetParser::TFileLocation TFileLocation;
    typedef CPatchSetParser::TFileEntry TFileEntry;
    typedef std::vector< TFileEntry > TFileList;
  
    public:
    
    CPatchSetFileEngine( void );
    virtual ~CPatchSetFileEngine();
    
    public:
    
    bool Start( const TFileList& fileList            ,
                const CORE::CString& localRoot       ,
                const CORE::CString& tempStorageRoot );
    
    void Stop( void );
    
    bool IsActive( void ) const;
    
    protected:
        
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );

    private:
    
    bool ProcessCurrentFile( void );
    bool ProceedToNextFile( void );
    bool TryNextFileLocation( void );
    
    private:
    
    CPatchSetFileEngine( const CPatchSetFileEngine& src );            /**< not implemented */
    CPatchSetFileEngine& operator=( const CPatchSetFileEngine& src ); /**< not implemented */
    
    private:
    
    UInt32 m_curFileIndex;
    UInt32 m_curFileLocIndex;
    TFileList m_fileList;
    
    CORE::CFileAccess* m_fileAccess;
    CORE::CURLDataRetriever m_dataRetriever;
    
    bool m_isActive;
    bool m_stopSignalGiven;
    CORE::CString m_localRoot;
    CORE::CString m_tempStorageRoot;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHSETFILEENGINE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/