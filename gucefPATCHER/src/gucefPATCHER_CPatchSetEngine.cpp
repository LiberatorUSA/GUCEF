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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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
    : CORE::CForwardingNotifier()                     ,
      CPatchSetDirEngineEvents()                      ,
      CPatchSetFileEngineEvents()                     ,
      CPatchSetEngineEvents()                         ,
      m_patchSetDirEngine( new CPatchSetDirEngine() ) ,
      m_patchSet( NULL )                              ,
      m_dirIndex( 0 )                                 ,
      m_isActive( false )                             ,
      m_stopSignalGiven( false )                      ,
      m_localRoot()
      
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CPatchSetEngine::CPatchSetEngine( CORE::CPulseGenerator& pulseGenerator )
    : CORE::CForwardingNotifier()                                     ,
      CPatchSetDirEngineEvents()                                      ,
      CPatchSetFileEngineEvents()                                     ,
      CPatchSetEngineEvents()                                         ,
      m_patchSetDirEngine( new CPatchSetDirEngine( pulseGenerator ) ) ,
      m_patchSet( NULL )                                              ,
      m_dirIndex( 0 )                                                 ,
      m_isActive( false )                                             ,
      m_stopSignalGiven( false )                                      ,
      m_localRoot()
      
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

void
CPatchSetEngine::Initialize( void )
{GUCEF_TRACE;

    assert( m_patchSetDirEngine != NULL );
    
    // Forward events from the dir engines
    AddForwardingForEvent( SubDirProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( SubDirProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    
    // Forward file engine events
    AddForwardingForEvent( FileListProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileSizeMismatchEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileIsOKEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileHashMismatchEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileNotFoundEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileReplacedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileRetrievalStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileRetrievalCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileStorageErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileListProcessingCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileListProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );    
    
    SubscribeTo( m_patchSetDirEngine );    
}

/*-------------------------------------------------------------------------*/

CPatchSetEngine::~CPatchSetEngine()
{GUCEF_TRACE;

    Stop();
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetEngine::Start( const TPatchSet& patchSet            ,
                        const CORE::CString& localRoot       ,
                        const CORE::CString& tempStorageRoot )
{GUCEF_TRACE;

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
{GUCEF_TRACE;
    
    m_stopSignalGiven = true;
    
    m_patchSetDirEngine->Stop();
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchSetEngine::IsActive( void ) const
{GUCEF_TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/

void
CPatchSetEngine::OnNotify( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventid               ,
                           CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

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