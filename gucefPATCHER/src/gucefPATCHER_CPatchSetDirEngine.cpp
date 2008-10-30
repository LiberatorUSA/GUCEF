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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetDirEngine::CPatchSetDirEngine( void )
    : CORE::CForwardingNotifier() ,
      CPatchSetFileEngineEvents() ,   
      m_curSubDirIndex( 0 )       ,
      m_subDirPatchEngine( NULL ) ,
      m_filePatchEngine( NULL )   ,    
      m_dir()                     ,
      m_isActive( false )         ,
      m_stopSignalGiven( false )  ,
      m_localRoot()               ,
      m_tempStorageRoot()         ,
      m_pulseGenerator( NULL )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CPatchSetDirEngine::CPatchSetDirEngine( CORE::CPulseGenerator& pulseGenerator )
    : CORE::CForwardingNotifier()         ,
      CPatchSetFileEngineEvents()         ,   
      m_curSubDirIndex( 0 )               ,
      m_subDirPatchEngine( NULL )         ,
      m_filePatchEngine( NULL )           ,    
      m_dir()                             ,
      m_isActive( false )                 ,
      m_stopSignalGiven( false )          ,
      m_localRoot()                       ,
      m_tempStorageRoot()                 ,
      m_pulseGenerator( &pulseGenerator )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

void
CPatchSetDirEngine::Initialize( void )
{
    // Forward events from child sub-dir engines
    AddForwardingForEvent( SubDirProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( SubDirProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    
    // Forward file engine events
    AddForwardingForEvent( FileListProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileIsOKEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileSizeMismatchEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileHashMismatchEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileNotFoundEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( LocalFileReplacedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileRetrievalStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileRetrievalCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileStorageErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileListProcessingCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( FileListProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );
}

/*-------------------------------------------------------------------------*/

CPatchSetDirEngine::~CPatchSetDirEngine()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPatchSetDirEngine::ProcessFilesInDir( void )
{GUCEF_TRACE;

    if ( m_dir.files.size() > 0 )
    {
        // We will be needing an engine for our file processing
        if ( m_filePatchEngine == NULL )
        {
            if ( NULL != m_pulseGenerator )
            {
                m_filePatchEngine = new CPatchSetFileEngine( *m_pulseGenerator );
            }
            else
            {
                m_filePatchEngine = new CPatchSetFileEngine();
            }
            SubscribeTo( m_filePatchEngine );
        }
        
        // process the files in this directory
        return m_filePatchEngine->Start( m_dir.files       ,
                                         m_localRoot       ,
                                         m_tempStorageRoot );
    }
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetDirEngine::ProcessCurSubDir( void )
{GUCEF_TRACE;

    if ( m_curSubDirIndex < m_dir.subDirs.size() )
    {
        NotifyObservers( SubDirProcessingStartedEvent );
        
        // We will be needing an engine for our sub-dir processing
        if ( m_subDirPatchEngine == NULL )
        {
            if ( NULL != m_pulseGenerator )
            {
                m_subDirPatchEngine = new CPatchSetDirEngine( *m_pulseGenerator );
            }
            else
            {
                m_subDirPatchEngine = new CPatchSetDirEngine();
            }
            SubscribeTo( m_subDirPatchEngine ); 
        }
        
        // Create the correct path strings for this sub-sir
        const TDirEntry* subDir = &m_dir.subDirs[ m_curSubDirIndex ];
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
{GUCEF_TRACE;

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
            m_dir = startingDir;
            m_localRoot = localRoot;
            m_tempStorageRoot = tempStorageRoot;
            m_curSubDirIndex = 0;
            
            if ( m_dir.files.size() > 0 )
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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

    return m_isActive;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetDirEngine::ProcessNextSubDir( void )
{GUCEF_TRACE;

    // Move on to the next sub-dir (if any exists)
    if ( m_curSubDirIndex+1 < m_dir.subDirs.size() )
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
{GUCEF_TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Call base-class version
        CORE::CForwardingNotifier::OnNotify( notifier  ,
                                             eventid   ,
                                             eventdata );
        
        if ( ( m_filePatchEngine != NULL ) && ( notifier == m_filePatchEngine ) )
        {
            if ( eventid == CPatchSetFileEngine::FileListProcessingCompleteEvent )
            {
                if ( m_curSubDirIndex < m_dir.subDirs.size() )
                {
                    // The files in this dir have been processed, we can move on to the
                    // sub-directories. The first sub-dir is the current sub-dir.
                    ProcessCurSubDir();
                }
                else
                {
                    // If we get here then we have worked trough our list of sub-dirs
                    // Since this operation comes after file handling we can now tell
                    // everyone we are finished
                    m_stopSignalGiven = false;
                    m_isActive = false;
                    NotifyObservers( DirProcessingCompletedEvent );
                }
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