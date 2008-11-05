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

CPatchEngine::CPatchEngine( CORE::CPulseGenerator& pulseGenerator )
    : CORE::CForwardingNotifier()                                 ,
      CORE::CIConfigurable()                                      ,
      CPatchSetDirEngineEvents()                                  ,
      CPatchSetFileEngineEvents()                                 ,
      CPatchSetEngineEvents()                                     ,
      CPatchListEngineEvents()                                    ,
      m_patchListEngine( new CPatchListEngine( pulseGenerator ) ) ,
      m_url()                                                     ,
      m_listDataBuffer()                                          ,
      m_isActive( false )                                         ,
      m_stopSignalGiven( false )                                  ,
      m_localRoot()                                               ,
      m_tempStorageRoot()                                         ,
      m_patchListURL()                                            ,
      m_patchListCodec()
      
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CPatchEngine::CPatchEngine( void )
    : CORE::CForwardingNotifier()                 ,
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
      
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

void
CPatchEngine::Initialize( void )
{GUCEF_TRACE;

    assert( m_patchListEngine != NULL );
    
    RegisterEvents();    
  
    // Forward events from the list engine
    AddForwardingForEvent( PatchListProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchListProcessingProgressEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchListProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchListProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchListProcessingFailedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetRetrievalStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetRetrievalCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetRetrievalFailedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetRetrievalAbortedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetDecodingFailedEvent, EVENTORIGINFILTER_TRANSFER );

    // Forward events from the set engine
    AddForwardingForEvent( PatchSetProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetProcessingProgressEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetProcessingAbortedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PatchSetProcessingFailedEvent, EVENTORIGINFILTER_TRANSFER );
    
    // Forward events from the dir engines
    AddForwardingForEvent( DirProcessingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( DirProcessingCompletedEvent, EVENTORIGINFILTER_TRANSFER );
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
    
    // Subscribe to all events from the patch list engine
    // This should allow us to receive all events that we wish to forward
    SubscribeTo( m_patchListEngine );
}

/*-------------------------------------------------------------------------*/

CPatchEngine::~CPatchEngine()
{GUCEF_TRACE;

    Stop();
    
    delete m_patchListEngine;
    m_patchListEngine = NULL;
}

/*-------------------------------------------------------------------------*/

void
CPatchEngine::RegisterEvents( void )
{GUCEF_TRACE;
    
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
{GUCEF_TRACE;
    
    return false;
}

/*-------------------------------------------------------------------------*/
                                   
bool
CPatchEngine::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchEngine: Loading configuration" );
    
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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

    return m_localRoot;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::SetLocalTempStorageRootDir( const CORE::CString& tempStorageRoot )
{GUCEF_TRACE;

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
{GUCEF_TRACE;

    return m_tempStorageRoot;
}
    
/*-------------------------------------------------------------------------*/
    
bool
CPatchEngine::SetPatchListURL( const CORE::CString& patchListURL )
{GUCEF_TRACE;

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
{GUCEF_TRACE;

    return m_patchListURL;
}

/*-------------------------------------------------------------------------*/

bool
CPatchEngine::SetPatchListCodec( const CORE::CString& patchListCodec )
{GUCEF_TRACE;

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
{GUCEF_TRACE;

    return m_patchListCodec;
}

/*-------------------------------------------------------------------------*/

bool
CPatchEngine::Start( void )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchEngine: Starting" );
    
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
            if ( m_url.SetURL( m_patchListURL ) )
            {
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
    }
    
    NotifyObservers( PatchProcessFailedEvent );
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchEngine::Stop( void )
{GUCEF_TRACE;
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchEngine: Stopping" );
    
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
{GUCEF_TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/

bool
CPatchEngine::ProcessRecievedPatchList( void )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchEngine: Processing patch list" );
    
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
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchEngine: Error while processing patch list" );
    
    // If we get here then we failed to decode the raw data into a patch set
    NotifyObservers( PatchListDecodingFailedEvent );
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchEngine::OnNotify( CORE::CNotifier* notifier                 ,
                        const CORE::CEvent& eventid               ,
                        CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Call base-class version
        CORE::CForwardingNotifier::OnNotify( notifier  ,
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
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchEngine: Patch list data received" );
                
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
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchEngine: Aborting patch list retrieval" );
                
                // Someone has called Stop() while we where busy with our data retrieval
                m_stopSignalGiven = false;
                m_isActive = false;
                NotifyObservers( PatchProcessAbortedEvent );            
            }
            else
            if ( eventid == CORE::CURL::URLDataRetrievalErrorEvent )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPatchEngine: Patch list data retrieval error" );
                
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

/*-------------------------------------------------------------------------*/

const CString&
CPatchEngine::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::PATCHER::CPatchEngine";
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