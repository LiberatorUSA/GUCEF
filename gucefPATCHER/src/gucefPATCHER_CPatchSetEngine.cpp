/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2003.  All rights reserved.
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

#include <assert.h>

#ifndef GUCEF_PATCHER_CPATCHSETDIRENGINE_H
#include "gucefPATCHER_CPatchSetDirEngine.h"
#define GUCEF_PATCHER_CPATCHSETDIRENGINE_H
#endif /* GUCEF_PATCHER_CPATCHSETDIRENGINE_H ? */

#include "gucefPATCHER_CPatchSetEngine.h"	/* definition of this class */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetEngine::CPatchSetEngine( void )
    : CORE::CObservingNotifier()                      ,
      CPatchSetDirEngineEvents()                      ,
      CPatchSetFileEngineEvents()                     ,
      CPatchSetEngineEvents()                         ,
      m_patchSetDirEngine( new CPatchSetDirEngine() ) ,
      m_patchSet( NULL )                              ,
      m_dirIndex( 0 )                                 ,
      m_isActive( false )                             ,
      m_stopSignalGiven( false )                      ,
      m_localRoot()
      
{TRACE;

    assert( m_patchSetDirEngine != NULL );
    
    // Forward events from the dir engines
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
    
    SubscribeTo( m_patchSetDirEngine );
}

/*-------------------------------------------------------------------------*/

CPatchSetEngine::~CPatchSetEngine()
{TRACE;

    Stop();
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetEngine::Start( const TPatchSet& patchSet            ,
                        const CORE::CString& localRoot       ,
                        const CORE::CString& tempStorageRoot )
{TRACE;

    assert( &patchSet != NULL );
    
    // The user should explicitly stop first if we are already busy
    if ( !IsActive() )
    {
        // parameter sanity check
        if ( ( patchSet.size() > 0 )     &&
             ( localRoot.Length() > 0 )  )
        {
            m_isActive = true;
            m_stopSignalGiven = false;
            
            m_patchSet = patchSet;

            m_localRoot = localRoot;
            m_tempStorageRoot = tempStorageRoot;
            m_dirIndex = 0;
            
            NotifyObservers( PatchSetProcessingStartedEvent );            
            m_patchSetDirEngine->Start( m_patchSet[ m_dirIndex ] ,
                                        m_localRoot              ,
                                        m_tempStorageRoot        );
            
            return true;
        }
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchSetEngine::Stop( void )
{TRACE;
    
    m_stopSignalGiven = true;
    
    m_patchSetDirEngine->Stop();
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchSetEngine::IsActive( void ) const
{TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/

void
CPatchSetEngine::OnNotify( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventid               ,
                           CORE::CICloneable* eventdata /* = NULL */ )
{TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Call base-class version
        CORE::CObservingNotifier::OnNotify( notifier  ,
                                            eventid   ,
                                            eventdata );
        
        if ( notifier == m_patchSetDirEngine )
        {
            if ( eventid == DirProcessingCompletedEvent )
            {
                NotifyObservers( SubDirProcessingCompletedEvent );
                
                // Move on to the next sub-dir (if any exists)
                if ( m_dirIndex+1 < m_patchSet.size() )
                {
                    ++m_dirIndex;
                    
                    NotifyObservers( SubDirProcessingStartedEvent );
                    m_patchSetDirEngine->Start( m_patchSet[ m_dirIndex ] ,
                                                m_localRoot              ,
                                                m_tempStorageRoot        );
                }
                else
                {
                    NotifyObservers( PatchSetProcessingCompletedEvent );
                }
                return;
            }
            else
            if ( eventid == DirProcessingAbortedEvent )
            {
                NotifyObservers( PatchSetProcessingAbortedEvent );
                return;
            }
        }
    }
    else
    {
        m_stopSignalGiven = false;
        m_isActive = false;
        NotifyObservers( PatchSetProcessingAbortedEvent );
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