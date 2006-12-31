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

#ifndef GUCEF_PATCHER_CPATCHSETDIRENGINE_H
#define GUCEF_PATCHER_CPATCHSETDIRENGINE_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETPARSER_H
#include "gucefPATCHER_CPatchSetParser.h"
#define GUCEF_PATCHER_CPATCHSETPARSER_H
#endif /* GUCEF_PATCHER_CPATCHSETPARSER_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
#include "gucefPATCHER_CPatchSetFileEngineEvents.h"
#define GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
#endif /* GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H
#include "gucefPATCHER_CPatchSetDirEngineEvents.h"
#define GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H
#endif /* GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H ? */

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

class CPatchSetFileEngine;

/*-------------------------------------------------------------------------*/

/**
 *  Engine for patching a directory tree and all the files it contains.
 *  The tree is mirrored to the one starting at the given local root.
 */
class GUCEFPATCHER_EXPORT_CPP CPatchSetDirEngine : public CORE::CObservingNotifier  ,
                                                   public CPatchSetFileEngineEvents ,
                                                   public CPatchSetDirEngineEvents
{
    public:

    typedef CPatchSetParser::TFileLocation TFileLocation;
    typedef CPatchSetParser::TFileEntry TFileEntry;
    typedef CPatchSetParser::TDirEntry TDirEntry;
    
    public:
    
    CPatchSetDirEngine( void );
    
    virtual ~CPatchSetDirEngine();
    
    public:
    
    bool Start( const TDirEntry& startingDir         ,
                const CORE::CString& localRoot       ,
                const CORE::CString& tempStorageRoot );
    
    void Stop( void );
    
    bool IsActive( void ) const;

    protected:
        
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );    
    
    private:
    
    bool ProcessCurSubDir( void );
    bool ProcessNextSubDir( void );
    bool ProcessFilesInDir( void );
    
    private:
        
    UInt32 m_curSubDirIndex;
    CPatchSetDirEngine* m_subDirPatchEngine;
    CPatchSetFileEngine* m_filePatchEngine;
    
    TDirEntry m_dir;
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

#endif /* GUCEF_PATCHER_CPATCHSETDIRENGINE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/