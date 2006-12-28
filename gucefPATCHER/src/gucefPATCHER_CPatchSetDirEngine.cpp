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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETFILEENGINE_H
#include "gucefPATCHER_CPatchSetFileEngine.h"
#define GUCEF_PATCHER_CPATCHSETFILEENGINE_H
#endif /* GUCEF_PATCHER_CPATCHSETFILEENGINE_H ? */

#include "gucefPATCHER_CPatchSetDirEngine.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CPatchSetDirEngine::DirProcessingStartedEvent = "GUCEF::PATCHER::CPatchSetDirEngine::DirProcessingStartedEvent";
const CORE::CEvent CPatchSetDirEngine::SubDirProcessingStartedEvent = "GUCEF::PATCHER::CPatchSetDirEngine::SubDirProcessingStartedEvent";
const CORE::CEvent CPatchSetDirEngine::SubDirProcessingCompletedEvent = "GUCEF::PATCHER::CPatchSetDirEngine::SubDirProcessingCompletedEvent";
const CORE::CEvent CPatchSetDirEngine::DirProcessingCompletedEvent = "GUCEF::PATCHER::CPatchSetDirEngine::DirProcessingCompletedEvent";
const CORE::CEvent CPatchSetDirEngine::DirProcessingAbortedEvent = "GUCEF::PATCHER::CPatchSetDirEngine::DirProcessingAbortedEvent";
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetDirEngine::CPatchSetDirEngine( void )
    : CORE::CObservingNotifier()  ,
      CPatchSetFileEngineEvents() ,   
      m_curSubDirIndex( 0 )       ,
      m_subDirPatchEngine( NULL ) ,
      m_filePatchEngine( NULL )   ,    
      m_dir( NULL )               ,
      m_isActive( false )         ,
      m_stopSignalGiven( false )  ,
      m_localRoot()               ,
      m_tempStorageRoot()
{TRACE;

    RegisterEvents();
    
    // Forward events from child sub-dir engines
    AddEventForwarding( SubDirProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( SubDirProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    
    // Forward file engine events
    AddEventForwarding( FileListProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( LocalFileSizeMismatchEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( LocalFileHashMismatchEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( LocalFileNotFoundEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileRetrievalStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileRetrievalCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileStorageErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileListProcessingCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileListProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );
}

/*-------------------------------------------------------------------------*/

CPatchSetDirEngine::~CPatchSetDirEngine()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchSetDirEngine::RegisterEvents( void )
{TRACE;

    DirProcessingStartedEvent.Initialize();
    SubDirProcessingStartedEvent.Initialize();
    SubDirProcessingCompletedEvent.Initialize();
    DirProcessingCompletedEvent.Initialize();
    DirProcessingAbortedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetDirEngine::ProcessFilesInDir( void )
{TRACE;

    if ( m_dir->files.size() > 0 )
    {
        // We will be needing an engine for our file processing
        if ( m_filePatchEngine == NULL )
        {
            m_filePatchEngine = new CPatchSetFileEngine();
            SubscribeTo( m_filePatchEngine );
        }
        
        // process the files in this directory
        return m_filePatchEngine->Start( m_dir->files      ,
                                         m_localRoot       ,
                                         m_tempStorageRoot );
    }
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetDirEngine::ProcessCurSubDir( void )
{TRACE;

    if ( m_curSubDirIndex <= m_dir->subDirs.size() )
    {
        NotifyObservers( SubDirProcessingStartedEvent );
        
        // We will be needing an engine for our sub-dir processing
        if ( m_subDirPatchEngine == NULL )
        {
            m_subDirPatchEngine = new CPatchSetDirEngine();
            SubscribeTo( m_subDirPatchEngine ); 
        }
        
        // Create the correct path strings for this sub-sir
        const TDirEntry* subDir = &m_dir->subDirs[ m_curSubDirIndex ];
        CORE::CString subDirPath = m_localRoot;
        CORE::AppendToPath( subDirPath, subDir->name );
        CORE::CString subTmpDirPath = m_tempStorageRoot;
        CORE::AppendToPath( subTmpDirPath, subDir->name );
        
        // make sure the sub-sir exists locally
        CORE::Create_Directory( subDirPath.C_String() );
        
        // process the sub-dir
        return m_subDirPatchEngine->Start( *subDir       ,
                                           subDirPath    ,
                                           subTmpDirPath );
    }
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetDirEngine::Start( const TDirEntry& startingDir         ,
                           const CORE::CString& localRoot       ,
                           const CORE::CString& tempStorageRoot )
{TRACE;

    assert( &startingDir != NULL );
    
    // The user should explicitly stop first if we are already busy
    if ( !IsActive() )
    {
        // parameter sanity check
        if ( localRoot.Length() > 0 )
        {
            m_isActive = true;
            m_stopSignalGiven = false;
            
            NotifyObservers( DirProcessingStartedEvent );
            
            // Copy and link params for later use
            m_dir = &startingDir;
            m_localRoot = localRoot;
            m_tempStorageRoot = tempStorageRoot;
            m_curSubDirIndex = 0;
            
            if ( m_dir->files.size() > 0 )
            {
                return ProcessFilesInDir();
            }
            else
            {
                NotifyObservers( SubDirProcessingStartedEvent );
                return ProcessCurSubDir();
            }
        }
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchSetDirEngine::Stop( void )
{TRACE;

    if ( m_isActive )
    {
        m_stopSignalGiven = true;
        
        if ( m_filePatchEngine != NULL )
        {
            if ( m_filePatchEngine->IsActive() )
            {
                m_filePatchEngine->Stop();
            }
        }
        if ( m_subDirPatchEngine != NULL )
        {
            if ( m_subDirPatchEngine->IsActive() )
            {
                m_subDirPatchEngine->Stop();
            }
        }
    }    
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetDirEngine::IsActive( void ) const
{TRACE;

    return m_isActive;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetDirEngine::ProcessNextSubDir( void )
{TRACE;

    // Move on to the next sub-dir (if any exists)
    if ( m_curSubDirIndex+1 < m_dir->subDirs.size() )
    {
        ++m_curSubDirIndex;
        return ProcessCurSubDir();
    }
    
    // If we get here then we have worked trough our list of sub-dirs
    // Since this operation comes after file handling we can now tell
    // everyone we are finished
    m_stopSignalGiven = false;
    m_isActive = false;
    NotifyObservers( DirProcessingCompletedEvent );
    
    return true;
}

/*-------------------------------------------------------------------------*/

void
CPatchSetDirEngine::OnNotify( CORE::CNotifier* notifier                 ,
                              const CORE::CEvent& eventid               ,
                              CORE::CICloneable* eventdata /* = NULL */ )
{TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Call base-class version
        CORE::CObservingNotifier::OnNotify( notifier  ,
                                            eventid   ,
                                            eventdata );
        
        if ( ( m_filePatchEngine != NULL ) && ( notifier == m_filePatchEngine ) )
        {
            if ( eventid == CPatchSetFileEngine::FileListProcessingCompleteEvent )
            {
                // The files in this dir have been processed, we can move on to the
                // sub-directories. The first sub-dir is the current sub-dir.
                ProcessCurSubDir();
                return;
            }
        }
        else
        if ( ( m_subDirPatchEngine != NULL ) && ( notifier == m_subDirPatchEngine ) )
        {
            if ( eventid == DirProcessingCompletedEvent )
            {
                NotifyObservers( SubDirProcessingCompletedEvent );
                
                // Move on to the next sub-dir (if any exists)
                ProcessNextSubDir();
                return;
            }
        }
    }
    else
    {
        m_stopSignalGiven = false;
        m_isActive = false;
        NotifyObservers( DirProcessingAbortedEvent );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/