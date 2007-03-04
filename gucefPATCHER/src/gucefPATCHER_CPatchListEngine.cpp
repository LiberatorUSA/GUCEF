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

CPatchListEngine::CPatchListEngine( void )
    : CORE::CObservingNotifier()                ,
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
      
{TRACE;

    assert( m_patchSetEngine != NULL );
  
    // Forward events from the set engine
    AddEventForwarding( PatchSetProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetProcessingFailedEvent, EVENTORIGINFILTER_TRANSFER );
    
    // Forward events from the dir engines
    AddEventForwarding( SubDirProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( SubDirProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    
    // Forward file engine events
    AddEventForwarding( FileListProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( LocalFileSizeMismatchEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( LocalFileHashMismatchEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( LocalFileNotFoundEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( LocalFileReplacedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileRetrievalStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileRetrievalCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileStorageErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileListProcessingCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileListProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );    

    SubscribeTo( &m_url );
    
    // Subscribe to all events from the patch set engine
    // This should allow us to receive all events that we wish to forward
    SubscribeTo( m_patchSetEngine );
}

/*-------------------------------------------------------------------------*/

CPatchListEngine::~CPatchListEngine()
{TRACE;

    Stop();
    
    delete m_patchSetEngine;
    m_patchSetEngine = NULL;
}

/*-------------------------------------------------------------------------*/

bool
CPatchListEngine::GetCurrentPatchSetLocation( TPatchSetLocation** location )
{TRACE;

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
{TRACE;

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
{TRACE;

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
{TRACE;
    
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
{TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/

bool
CPatchListEngine::ProcessRecievedPatchSet( void )
{TRACE;

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
{TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Call base-class version
        CORE::CObservingNotifier::OnNotify( notifier  ,
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
                // Someone has called Stop() while we where busy with our data retrieval
                m_stopSignalGiven = false;
                m_isActive = false;
                NotifyObservers( PatchListProcessingAbortedEvent );            
            }
            else
            if ( eventid == CORE::CURL::URLDataRetrievalErrorEvent )
            {
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/