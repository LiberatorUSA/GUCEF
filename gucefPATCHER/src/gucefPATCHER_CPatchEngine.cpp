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

#ifndef GUCEF_CORE_CMFILEACCESS_H
#include "CMFileAccess.h"
#define GUCEF_CORE_CMFILEACCESS_H
#endif /* GUCEF_CORE_CMFILEACCESS_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_PATCHER_CPATCHLISTENGINE_H
#include "gucefPATCHER_CPatchListEngine.h"
#define GUCEF_PATCHER_CPATCHLISTENGINE_H
#endif /* GUCEF_PATCHER_CPATCHLISTENGINE_H ? */

#include "gucefPATCHER_CPatchEngine.h"	/* definition of this class */

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

const CORE::CEvent CPatchEngine::PatchProcessStartedEvent = "GUCEF::PATCHER::CPatchEngine::PatchProcessStartedEvent";
const CORE::CEvent CPatchEngine::PatchProcessCompletedEvent = "GUCEF::PATCHER::CPatchEngine::PatchProcessCompletedEvent";
const CORE::CEvent CPatchEngine::PatchProcessAbortedEvent = "GUCEF::PATCHER::CPatchEngine::PatchProcessAbortedEvent";
const CORE::CEvent CPatchEngine::PatchProcessFailedEvent = "GUCEF::PATCHER::CPatchEngine::PatchProcessFailedEvent";
const CORE::CEvent CPatchEngine::PatchListDataReceivedEvent = "GUCEF::PATCHER::CPatchEngine::PatchListDataReceivedEvent";
const CORE::CEvent CPatchEngine::PatchListRetrievalStartedEvent = "GUCEF::PATCHER::CPatchEngine::PatchListRetrievalStartedEvent";
const CORE::CEvent CPatchEngine::PatchListRetrievalCompletedEvent = "GUCEF::PATCHER::CPatchEngine::PatchListRetrievalCompletedEvent";
const CORE::CEvent CPatchEngine::PatchListRetrievalFailedEvent = "GUCEF::PATCHER::CPatchEngine::PatchListRetrievalFailedEvent";
const CORE::CEvent CPatchEngine::PatchListRetrievalAbortedEvent = "GUCEF::PATCHER::CPatchEngine::PatchListRetrievalAbortedEvent";
const CORE::CEvent CPatchEngine::PatchListDecodingFailedEvent = "GUCEF::PATCHER::CPatchEngine::PatchListDecodingFailedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchEngine::CPatchEngine( void )
    : CORE::CObservingNotifier()                  ,
      CORE::CIConfigurable()                      ,
      CPatchSetDirEngineEvents()                  ,
      CPatchSetFileEngineEvents()                 ,
      CPatchSetEngineEvents()                     ,
      CPatchListEngineEvents()                    ,
      m_patchListEngine( new CPatchListEngine() ) ,
      m_url()                                     ,
      m_listDataBuffer()                          ,
      m_isActive( false )                         ,
      m_stopSignalGiven( false )                  ,
      m_localRoot()                               ,
      m_tempStorageRoot()                         ,
      m_patchListURL()                            ,
      m_patchListCodec()
      
{TRACE;

    assert( m_patchListEngine != NULL );
    
    RegisterEvents();    
  
    // Forward events from the list engine
    AddEventForwarding( PatchListProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchListProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchListProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchListProcessingFailedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetRetrievalStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetRetrievalCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetRetrievalFailedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetRetrievalAbortedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PatchSetDecodingFailedEvent, EVENTORIGINFILTER_TRANSFER );

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
    AddEventForwarding( FileRetrievalStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileRetrievalCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileStorageErrorEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileListProcessingCompleteEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( FileListProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );    
    
    SubscribeTo( &m_url );
    
    // Subscribe to all events from the patch list engine
    // This should allow us to receive all events that we wish to forward
    SubscribeTo( m_patchListEngine );
}

/*-------------------------------------------------------------------------*/

CPatchEngine::~CPatchEngine()
{TRACE;

    Stop();
    
    delete m_patchListEngine;
    m_patchListEngine = NULL;
}

/*-------------------------------------------------------------------------*/

void
CPatchEngine::RegisterEvents( void )
{TRACE;
    
    PatchProcessStartedEvent.Initialize();
    PatchProcessCompletedEvent.Initialize();
    PatchProcessAbortedEvent.Initialize();
    PatchProcessFailedEvent.Initialize();
    PatchListDataReceivedEvent.Initialize();
    PatchListRetrievalStartedEvent.Initialize();
    PatchListRetrievalCompletedEvent.Initialize();
    PatchListRetrievalFailedEvent.Initialize();
    PatchListRetrievalAbortedEvent.Initialize();
    PatchListDecodingFailedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

bool
CPatchEngine::SaveConfig( CORE::CDataNode& tree )
{TRACE;
    
    return false;
}

/*-------------------------------------------------------------------------*/
                                   
bool
CPatchEngine::LoadConfig( const CORE::CDataNode& treeroot )
{TRACE;

    const CORE::CDataNode* infoNode = treeroot.Find( "CPatchEngine" );
    if ( infoNode != NULL )
    {
        // First we obtain the mandatory atributes
        const CORE::CDataNode::TNodeAtt* att = infoNode->GetAttribute( "LocalRootDir" );
        if ( att == NULL ) return false;
        SetLocalRootDir( att->value );
        
        att = infoNode->GetAttribute( "TempStorageRoot" );
        if ( att == NULL ) return false;
        SetLocalTempStorageRootDir( att->value );
        
        att = infoNode->GetAttribute( "PatchListURL" );
        if ( att == NULL ) return false;
        SetPatchListURL( att->value );

        att = infoNode->GetAttribute( "PatchListCodec" );
        if ( att == NULL ) return false;
        SetPatchListCodec( att->value );
        
        // Try and find some optional engine trigger events
        const CORE::CDataNode* childNode = NULL;
        CORE::CDataNode::const_iterator i = infoNode->ConstBegin();
        while ( i != infoNode->ConstEnd() )
        {
            childNode = (*i);
            if ( childNode->GetName() == "EngineStartTrigger" )
            {
                att = infoNode->GetAttribute( "Event" );
                if ( att != NULL )
                {
                    AddEngineStartTriggerEvent( att->value );
                }
            }
            else
            if ( childNode->GetName() == "EngineStopTrigger" )
            {
                att = infoNode->GetAttribute( "Event" );
                if ( att != NULL )
                {
                    AddEngineStopTriggerEvent( att->value );
                }
            }            
            ++i;
        }
        return true;
    }  
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::AddEngineStartTriggerEvent( const CORE::CEvent& triggerEvent )
{TRACE;

    if ( !m_isActive )
    {
        m_startTriggers.insert( triggerEvent );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::RemoveEngineStartTriggerEvent( const CORE::CEvent& triggerEvent )
{TRACE;

    if ( !m_isActive )
    {
        m_startTriggers.erase( triggerEvent );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::AddEngineStopTriggerEvent( const CORE::CEvent& triggerEvent )
{TRACE;

    if ( !m_isActive )
    {
        m_stopTriggers.insert( triggerEvent );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::RemoveEngineStopTriggerEvent( const CORE::CEvent& triggerEvent )
{TRACE;

    if ( !m_isActive )
    {
        m_stopTriggers.erase( triggerEvent );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchEngine::SetLocalRootDir( const CORE::CString& localRoot )
{TRACE;

    if ( !m_isActive )
    {
        m_localRoot = localRoot;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CPatchEngine::GetLocalRootDir( void ) const
{TRACE;

    return m_localRoot;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::SetLocalTempStorageRootDir( const CORE::CString& tempStorageRoot )
{TRACE;

    if ( !m_isActive )
    {
        m_tempStorageRoot = tempStorageRoot;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CPatchEngine::GetLocalTempStorageRootDir( void ) const
{TRACE;

    return m_tempStorageRoot;
}
    
/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::SetPatchListURL( const CORE::CString& patchListURL )
{TRACE;

    if ( !m_isActive )
    {
        m_patchListURL = patchListURL;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CPatchEngine::GetPatchListURL( void ) const
{TRACE;

    return m_patchListURL;
}

/*-------------------------------------------------------------------------*/

bool
CPatchEngine::SetPatchListCodec( const CORE::CString& patchListCodec )
{TRACE;

    if ( !m_isActive )
    {
        m_patchListCodec = patchListCodec;
        return true;
    }
    return false;
}
    
/*-------------------------------------------------------------------------*/

const CORE::CString&
CPatchEngine::GetPatchListCodec( void ) const
{TRACE;

    return m_patchListCodec;
}

/*-------------------------------------------------------------------------*/

bool
CPatchEngine::Start( void )
{TRACE;

    // The user should explicitly stop first if we are already busy
    if ( !m_isActive )
    {
        // configuration sanity check
        if ( ( m_patchListURL.Length() > 0 )   &&
             ( m_patchListCodec.Length() > 0 ) &&
             ( m_localRoot.Length() > 0 )       )
        {
            m_isActive = true;
            m_stopSignalGiven = false;
            m_listDataBuffer.Clear();
            
            NotifyObservers( PatchProcessStartedEvent );
            
            // We must obtain the patch list before we can use it,..
            // Set the URL for the list location        
            m_url.SetURL( m_patchListURL );
        
            // Now we try and obtain it
            NotifyObservers( PatchListRetrievalStartedEvent );
            if ( !m_url.Activate() )
            {
                NotifyObservers( PatchListRetrievalFailedEvent );
                NotifyObservers( PatchProcessFailedEvent );
                return false;
            }
            
            // Now we wait
            return true;                 
        }
    }
    
    NotifyObservers( PatchProcessFailedEvent );
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchEngine::Stop( void )
{TRACE;
    
    if ( !m_stopSignalGiven && m_isActive )
    {
        m_stopSignalGiven = true;
        
        m_url.Deactivate();
        m_patchListEngine->Stop();
    }
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::IsActive( void ) const
{TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/

bool
CPatchEngine::ProcessRecievedPatchList( void )
{TRACE;

    // Now we must process the raw patch set data to turn it into something we can use
    // Get the required codec for the current raw patch set data type
    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = CORE::CDStoreCodecRegistry::Instance()->Lookup( m_patchListCodec );
    if ( NULL != codecPtr )
    {
        // Prepare vars for the decoding process
        CORE::CDataNode rootNode;
        CORE::CMFileAccess dataAccess( m_listDataBuffer.GetConstBufferPtr() , 
                                       m_listDataBuffer.GetDataSize()       );
                                       
        // decode the data in our buffer into a data tree
        if ( codecPtr->BuildDataTree( &rootNode   ,
                                      &dataAccess ) )
        {
            // Now we have to parse the data tree into something more familiar
            CPatchListParser::TPatchList patchList;
            CPatchListParser listParser;                        
            listParser.ParsePatchList( rootNode  ,
                                       patchList );
                                       
            // Now that the raw data has been processed into a real patch list we can commence
            // with the patching process for this patch list
            return m_patchListEngine->Start( patchList         ,
                                             m_localRoot       ,
                                             m_tempStorageRoot );
        }
    }
    
    // If we get here then we failed to decode the raw data into a patch set
    NotifyObservers( PatchListDecodingFailedEvent );
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchEngine::OnNotify( CORE::CNotifier* notifier                 ,
                        const CORE::CEvent& eventid               ,
                        CORE::CICloneable* eventdata /* = NULL */ )
{TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Call base-class version
        CORE::CObservingNotifier::OnNotify( notifier  ,
                                            eventid   ,
                                            eventdata );
        
        if ( notifier == m_patchListEngine )
        {
            if ( eventid == PatchListProcessingCompletedEvent )
            {
                // YIHA,.. We are finished
                m_stopSignalGiven = false;
                m_isActive = false;
                NotifyObservers( PatchProcessCompletedEvent );
            }
            else
            if ( eventid == PatchListProcessingFailedEvent )
            {
                // Cascade failure
                m_stopSignalGiven = false;
                m_isActive = false;                
                NotifyObservers( PatchProcessFailedEvent );
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
                m_listDataBuffer.Append( buffer.GetConstBufferPtr() , 
                                         buffer.GetDataSize()       );
                                        
                NotifyObservers( PatchListDataReceivedEvent );
            }
            else
            if ( eventid == CORE::CURL::URLAllDataRecievedEvent )
            {
                // The retrieval process of the patch list has been completed
                NotifyObservers( PatchListRetrievalCompletedEvent );
                
                // Now we must process the received patch list
                if ( !ProcessRecievedPatchList() )
                {
                    NotifyObservers( PatchProcessFailedEvent );
                }
            }
            else
            if ( eventid == CORE::CURL::URLDeactivateEvent )
            {
                // Someone has called Stop() while we where busy with our data retrieval
                m_stopSignalGiven = false;
                m_isActive = false;
                NotifyObservers( PatchProcessAbortedEvent );            
            }
            else
            if ( eventid == CORE::CURL::URLDataRetrievalErrorEvent )
            {
                // We failed to obtain the patch list data using the URL
                 m_stopSignalGiven = false;
                 m_isActive = false;
                 NotifyObservers( PatchListRetrievalFailedEvent );
                 NotifyObservers( PatchProcessFailedEvent );
            }
        }
    }
    else
    {
        if ( m_stopSignalGiven && m_isActive )
        {
            m_stopSignalGiven = false;
            m_isActive = false;
            NotifyObservers( PatchProcessAbortedEvent );
        }
        else
        if ( ( eventid != CORE::CNotifier::DestructionEvent ) &&
             ( eventid != CORE::CNotifier::UnsubscribeEvent ) &&
             ( eventid != CORE::CNotifier::SubscribeEvent   )  )
        {
            if ( m_startTriggers.end() != m_startTriggers.find( eventid ) )
            {
                Start();
            }
            else
            if ( m_stopTriggers.end() != m_stopTriggers.find( eventid ) )
            {
                Stop();
            }             
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