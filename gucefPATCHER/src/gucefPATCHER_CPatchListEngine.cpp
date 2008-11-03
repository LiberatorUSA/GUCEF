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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CMFILEACCESS_H
#include "CMFileAccess.h"
#define GUCEF_CORE_CMFILEACCESS_H
#endif /* GUCEF_CORE_CMFILEACCESS_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETENGINE_H
#include "gucefPATCHER_CPatchSetEngine.h"
#define GUCEF_PATCHER_CPATCHSETENGINE_H
#endif /* GUCEF_PATCHER_CPATCHSETENGINE_H ? */

#include "gucefPATCHER_CPatchListEngine.h"	/* definition of this class */

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

CPatchListEngine::CPatchListEngine( CORE::CPulseGenerator& pulseGenerator )
    : CORE::CForwardingNotifier()                               ,
      CPatchSetDirEngineEvents()                                ,
      CPatchSetFileEngineEvents()                               ,
      CPatchSetEngineEvents()                                   ,
      CPatchListEngineEvents()                                  ,
      m_patchSetEngine( new CPatchSetEngine( pulseGenerator ) ) ,
      m_url()                                                   ,
      m_setDataBuffer()                                         ,
      m_patchList()                                             ,
      m_setIndex( 0 )                                           ,
      m_setLocIndex( 0 )                                        ,
      m_isActive( false )                                       ,
      m_stopSignalGiven( false )                                ,
      m_localRoot()                                             ,
      m_tempStorageRoot()
      
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CPatchListEngine::CPatchListEngine( void )
    : CORE::CForwardingNotifier()               ,
      CPatchSetDirEngineEvents()                ,
      CPatchSetFileEngineEvents()               ,
      CPatchSetEngineEvents()                   ,
      CPatchListEngineEvents()                  ,
      m_patchSetEngine( new CPatchSetEngine() ) ,
      m_url()                                   ,
      m_setDataBuffer()                         ,
      m_patchList()                             ,
      m_setIndex( 0 )                           ,
      m_setLocIndex( 0 )                        ,
      m_isActive( false )                       ,
      m_stopSignalGiven( false )                ,
      m_localRoot()                             ,
      m_tempStorageRoot()
      
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

void
CPatchListEngine::Initialize( void )
{GUCEF_TRACE;
    
    assert( m_patchSetEngine != NULL );
  
    // Forward events from the set engine
    AddForwardingForEvent( PatchSetProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetProcessingFailedEvent, EVENTORIGINFILTER_TRANSFER );
    
    // Forward events from the dir engines
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

    SubscribeTo( &m_url );
    
    // Subscribe to all events from the patch set engine
    // This should allow us to receive all events that we wish to forward
    SubscribeTo( m_patchSetEngine );    
}

/*-------------------------------------------------------------------------*/

CPatchListEngine::~CPatchListEngine()
{GUCEF_TRACE;

    Stop();
    
    delete m_patchSetEngine;
    m_patchSetEngine = NULL;
}

/*-------------------------------------------------------------------------*/

bool
CPatchListEngine::GetCurrentPatchSetLocation( TPatchSetLocation** location )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchListEngine: Obtaining the current patch set location" );
    
    // Get the current set
    TPatchList::iterator i = m_patchList.begin();
    for ( UInt32 n=0; n<m_setIndex; ++n ) { ++i; }

    // Get the current set location 
    TPatchSetLocations& patchSetLocations = (*i).second;
    if ( m_setLocIndex < patchSetLocations.size() )
    {
        // Assign the location
        *location = &( patchSetLocations[ m_setLocIndex ] );
        return true;
    }
    
    return false; 
}

/*-------------------------------------------------------------------------*/

bool
CPatchListEngine::ObtainCurrentPatchSet( void )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchListEngine: Obtaining the current patch set" );
    
    TPatchSetLocation* setLocation = NULL;
    if ( GetCurrentPatchSetLocation( &setLocation ) )
    {
        // Make sure the buffer is cleared
        m_setDataBuffer.Clear();
        
        // Set the URL for the current set location        
        m_url.SetURL( setLocation->URL );
        
        // Now we try and obtain it
        NotifyObservers( PatchSetRetrievalStartedEvent );
        if ( !m_url.Activate() )
        {
            NotifyObservers( PatchSetRetrievalFailedEvent );
            return false;
        }
        
        return true;
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchListEngine::Start( const TPatchList& patchList          ,
                         const CORE::CString& localRoot       ,
                         const CORE::CString& tempStorageRoot )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchListEngine: Starting,.." );
    
    assert( &patchList != NULL );
    
    // The user should explicitly stop first if we are already busy
    if ( !IsActive() )
    {
        // parameter sanity check
        if ( ( patchList.size() > 0 )   &&
             ( localRoot.Length() > 0 )  )
        {
            m_isActive = true;
            m_stopSignalGiven = false;
            
            m_patchList = patchList;

            m_localRoot = CORE::RelativePath( localRoot );
            m_tempStorageRoot = CORE::RelativePath( tempStorageRoot );
            m_setIndex = 0;
            m_setLocIndex = 0;
            
            NotifyObservers( PatchListProcessingStartedEvent );
            
            // We must obtain the patch set before we can use it
            if ( !ObtainCurrentPatchSet() )
            {
                NotifyObservers( PatchListProcessingFailedEvent );
                return false;
            }
            
            // Now we wait
            return true;
        }
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchListEngine::Stop( void )
{GUCEF_TRACE;
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchListEngine: Stopping,.." );
    
    if ( !m_stopSignalGiven && m_isActive )
    {
        m_stopSignalGiven = true;
        
        m_url.Deactivate();
        m_patchSetEngine->Stop();
    }
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchListEngine::IsActive( void ) const
{GUCEF_TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/

bool
CPatchListEngine::ProcessRecievedPatchSet( void )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchListEngine: Processing the received patch set" );
    
    if ( m_setDataBuffer.GetDataSize() > 0 )
    {
        // Now we must process the raw patch set data to turn it into something we can use
        TPatchSetLocation* setLocation = NULL;
        if ( GetCurrentPatchSetLocation( &setLocation ) )
        {
            // Get the required codec for the current raw patch set data type
            CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = CORE::CDStoreCodecRegistry::Instance()->Lookup( setLocation->codec );
            if ( NULL != codecPtr )
            {                
                // Prepare vars for the decoding process
                CORE::CDataNode rootNode;
                CORE::CMFileAccess dataAccess( m_setDataBuffer.GetConstBufferPtr() , 
                                               m_setDataBuffer.GetDataSize()       );
                                               
                // decode the data in our buffer into a data tree
                if ( codecPtr->BuildDataTree( &rootNode   ,
                                              &dataAccess ) )
                {
                    // Make sure the buffer is cleared
                    m_setDataBuffer.Clear();
                    
                    // Now we have to parse the data tree into something more familiar
                    TPatchSet patchSet;
                    CPatchSetParser setParser;                        
                    if ( setParser.ParsePatchSet( rootNode ,
                                                  patchSet ) )
                    {
                        // Now that the raw data has been processed into a real patch list we can commence
                        // with the patching process for this patch set
                        return m_patchSetEngine->Start( patchSet          ,
                                                        m_localRoot       ,
                                                        m_tempStorageRoot );
                    }
                }
            }
        }
    }
    
    // If we get here then we failed to decode the raw data into a patch set
    NotifyObservers( PatchSetDecodingFailedEvent );
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchListEngine::OnNotify( CORE::CNotifier* notifier                 ,
                            const CORE::CEvent& eventid               ,
                            CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Call base-class version
        CORE::CForwardingNotifier::OnNotify( notifier  ,
                                             eventid   ,
                                             eventdata );
        
        if ( notifier == m_patchSetEngine )
        {
            if ( eventid == PatchSetProcessingCompletedEvent )
            {
                // W00t,.. we finished processing an entire patch set
                if ( m_setIndex+1 < m_patchList.size() )
                {
                    ++m_setIndex;
                    m_setLocIndex = 0;
                    ObtainCurrentPatchSet();
                }
                else
                {
                    // YIHA,.. We are finished
                    m_stopSignalGiven = false;
                    m_isActive = false;
                    NotifyObservers( PatchListProcessingCompletedEvent );                    
                }
            }
            else
            if ( eventid == PatchSetProcessingFailedEvent )
            {
                // Cascade failure
                m_stopSignalGiven = false;
                m_isActive = false;                
                NotifyObservers( PatchListProcessingFailedEvent );
            }
        }
        else
        if ( notifier == &m_url )
        {
            if ( eventid == CORE::CURL::URLDataRecievedEvent )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchListEngine: Patch set data received" );
                
                // Translate event data
                const CORE::CDynamicBuffer& buffer = ( static_cast< CORE::CURL::TURLDataRecievedEventData* >( eventdata ) )->GetData();
                
                // Append the data to our buffer
                m_setDataBuffer.Append( buffer.GetConstBufferPtr() , 
                                        buffer.GetDataSize()       );
                                        
                NotifyObservers( PatchSetDataRecievedEvent );
            }
            else
            if ( eventid == CORE::CURL::URLAllDataRecievedEvent )
            {
                // The retrieval process of the patch set has been completed
                NotifyObservers( PatchSetRetrievalCompletedEvent );
                
                // Now we must process the received patch set
                ProcessRecievedPatchSet();
            }
            else
            if ( eventid == CORE::CURL::URLDeactivateEvent )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchListEngine: Aborting patch list processing" );
                
                // Someone has called Stop() while we where busy with our data retrieval
                m_stopSignalGiven = false;
                m_isActive = false;
                NotifyObservers( PatchListProcessingAbortedEvent );            
            }
            else
            if ( eventid == CORE::CURL::URLDataRetrievalErrorEvent )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchListEngine: Patch set data retrieval error" );
                
                // We failed to obtain the patch set data using the current location URL
                // Perhaps we can use another location
                ++m_setLocIndex;
                if ( !ObtainCurrentPatchSet() )
                {
                    // switching to the next location (if there is one) didn't help.
                    m_stopSignalGiven = false;
                    m_isActive = false;
                    NotifyObservers( PatchSetRetrievalFailedEvent );
                    NotifyObservers( PatchListProcessingFailedEvent );
                } 
            }
        }
    }
    else
    {
        if ( m_isActive )
        {
            m_stopSignalGiven = false;
            m_isActive = false;
            NotifyObservers( PatchListProcessingAbortedEvent );
        }
    }
}

/*-------------------------------------------------------------------------*/

const CString&
CPatchListEngine::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::PATCHER::CPatchListEngine";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/