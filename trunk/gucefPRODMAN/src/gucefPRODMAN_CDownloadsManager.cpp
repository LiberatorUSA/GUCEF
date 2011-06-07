/*
 *  gucefPRODMAN: Product management module
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCE_CORE_MISCUTILS_H
#include "miscutils.h"
#define GUCE_CORE_MISCUTILS_H
#endif /* GUCE_CORE_MISCUTILS_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#include "guCORE_CDownloadsManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GU {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const GUCEF::CORE::CEvent CDownloadsManager::DownloadStartedEvent = "GU::CORE::CDownloadsManager::DownloadStartedEvent";
const GUCEF::CORE::CEvent CDownloadsManager::DownloadStoppedEvent = "GU::CORE::CDownloadsManager::DownloadStartedEvent";
const GUCEF::CORE::CEvent CDownloadsManager::DownloadStopRequestedEvent = "GU::CORE::CDownloadsManager::DownloadStartedEvent";
const GUCEF::CORE::CEvent CDownloadsManager::DownloadFinishedEvent = "GU::CORE::CDownloadsManager::DownloadStartedEvent";
const GUCEF::CORE::CEvent CDownloadsManager::DownloadPausedEvent = "GU::CORE::CDownloadsManager::DownloadStartedEvent";
const GUCEF::CORE::CEvent CDownloadsManager::DownloadResumedEvent = "GU::CORE::CDownloadsManager::DownloadStartedEvent";
const GUCEF::CORE::CEvent CDownloadsManager::PatchTaskEventReceivedEvent = "GU::CORE::CDownloadsManager::PatchTaskEventReceivedEvent";        

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CDownloadsManager::RegisterEvents( void )
{GUCEF_TRACE;

    DownloadStartedEvent.Initialize();
    DownloadStoppedEvent.Initialize();
    DownloadStopRequestedEvent.Initialize();
    DownloadFinishedEvent.Initialize();
    DownloadPausedEvent.Initialize();
    DownloadResumedEvent.Initialize();
    PatchTaskEventReceivedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/
 
CDownloadsManager::CDownloadsManager( void )
    : m_productMap()                                                        ,
      m_patchEngineConfigTemplatePath( "Products/PatchConfigTemplate.xml" ) ,
      m_patchEngineConfigTemplateCodec( "xml" )                             ,
      m_patchManager()                                                      ,
      m_adListUrlRetiever()                                                 ,
      m_resumeDownloadsOnStartup( false )
{GUCEF_TRACE;

    RegisterEvents();
    
    SubscribeTo( &m_adListUrlRetiever                                                ,
                 GUCEF::CORE::CURLDataRetriever::URLAllDataRecievedEvent             ,
                 &TEventCallback( this, &CDownloadsManager::OnADListRetrievalEvent ) );
    SubscribeTo( &m_adListUrlRetiever                                                ,
                 GUCEF::CORE::CURLDataRetriever::URLDataRetrievalErrorEvent          ,
                 &TEventCallback( this, &CDownloadsManager::OnADListRetrievalEvent ) );                 
}

/*-------------------------------------------------------------------------*/
    
CDownloadsManager::~CDownloadsManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CDownloadsManager::ObtainAvailableDownloadsList( const CString& listUrl )
{GUCEF_TRACE;

    // Check if we need to allocate storage
    if ( NULL == m_adListUrlRetiever.GetIOAccess() )
    {
        GUCEF::CORE::CDynamicBuffer* listBuffer = new GUCEF::CORE::CDynamicBuffer( true );
        GUCEF::CORE::CDynamicBufferAccess* bufferAccess = new GUCEF::CORE::CDynamicBufferAccess( listBuffer, true );
        
        m_adListUrlRetiever.SetIOAccess( bufferAccess );
    }
    
    GUCEF::CORE::CURL& url = m_adListUrlRetiever.GetURL();
    if ( url.SetURL( listUrl ) )
    {
        return url.Activate();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CDownloadsManager::RefreshAvailableDownloadsList( void )
{GUCEF_TRACE;

    // Check if we need to allocate storage
    if ( NULL == m_adListUrlRetiever.GetIOAccess() )
    {
        GUCEF::CORE::CDynamicBuffer* listBuffer = new GUCEF::CORE::CDynamicBuffer( true );
        GUCEF::CORE::CDynamicBufferAccess* bufferAccess = new GUCEF::CORE::CDynamicBufferAccess( listBuffer, true );
        
        m_adListUrlRetiever.SetIOAccess( bufferAccess );
    }
    
    m_adListUrlRetiever.GetURL().Refresh();
}
    
/*-------------------------------------------------------------------------*/

bool
CDownloadsManager::StartDownload( const CProductInfo& productInfo )
{GUCEF_TRACE;

    GUCEF::CORE::CDataNode configTemplate;
    CString productString = productInfo.GetCombinedProductString();
    
    if ( GUCE::CORE::VFSLoadDataTree( m_patchEngineConfigTemplatePath  ,
                                      m_patchEngineConfigTemplateCodec ,
                                      &configTemplate                  ) )
    {
        GUCEF::CORE::CDataNode* node = configTemplate.Find( "CPatchEngine" );
        node->SetAttribute( "PatchListURL", productInfo.GetPatchListURL() );
        
        m_productMap[ productString ] = productInfo;
        
        return m_patchManager.StartTask( productString  ,
                                         configTemplate );
    }
    return false;                          
}

/*-------------------------------------------------------------------------*/

bool
CDownloadsManager::PauseDownload( const CProductInfo& productInfo )
{GUCEF_TRACE;

    return m_patchManager.PauseTask( productInfo.GetCombinedProductString() );
}

/*-------------------------------------------------------------------------*/
    
bool
CDownloadsManager::ResumeDownload( const CProductInfo& productInfo )
{GUCEF_TRACE;

    return m_patchManager.ResumeTask( productInfo.GetCombinedProductString() );
}

/*-------------------------------------------------------------------------*/
    
bool
CDownloadsManager::RequestDownloadToStop( const CProductInfo& productInfo )
{GUCEF_TRACE;

    return m_patchManager.RequestTaskToStop( productInfo.GetCombinedProductString() );
}

/*-------------------------------------------------------------------------*/
        
void
CDownloadsManager::GetDownloadList( TProductMap& list ) const
{GUCEF_TRACE;

    list = m_productMap;
}

/*-------------------------------------------------------------------------*/

void
CDownloadsManager::OnADListRetrievalEvent( GUCEF::CORE::CNotifier* notifier    ,
                                           const GUCEF::CORE::CEvent& eventid  ,
                                           GUCEF::CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    if ( GUCEF::CORE::CURLDataRetriever::URLAllDataRecievedEvent == eventid )
    {
        GUCEF::CORE::CIOAccess* access = m_adListUrlRetiever.GetIOAccess();
        access->Setpos( 0 );
        
        GUCEF::CORE::CDataNode dataTree;
        if ( GUCE::CORE::LoadDataTree( *access   ,
                                       "xml"     ,
                                       &dataTree ) )
        {
            const GUCEF::CORE::CDataNode* rootNode = dataTree.Find( "ProductList" );
            GUCEF::CORE::CDataNode::const_iterator i = dataTree.ConstBegin();
            while ( i != dataTree.ConstEnd() )
            {
                
                ++i;
            }
        }
    }
    else
    if ( GUCEF::CORE::CURLDataRetriever::URLDataRetrievalErrorEvent == eventid )
    {
        
    }    
}

/*-------------------------------------------------------------------------*/

bool
CDownloadsManager::SaveConfig( GUCEF::CORE::CDataNode& node )
{GUCEF_TRACE;

    GUCEF::CORE::CDataNode* n = node.Structure( "GU%CORE%CDownloadsManager", '%' );
    n->SetAttribute( "AvailableDownloadsListURL", m_adListUrlRetiever.GetURL().GetURL() );
    n->SetAttribute( "ResumeDownloadsOnStartup", GUCEF::CORE::BoolToString( m_resumeDownloadsOnStartup ) );
    return true;
}

/*-------------------------------------------------------------------------*/
                                
bool
CDownloadsManager::LoadConfig( const GUCEF::CORE::CDataNode& node )
{GUCEF_TRACE;

    GUCEF::CORE::CDataNode* n = node.Search( "GU%CORE%CDownloadsManager", '%', false );
    if ( NULL != n )
    {
        GUCEF::CORE::CDataNode::TKeyValuePair* att = NULL;
        att = n->GetAttribute( "AvailableDownloadsListURL" );
        if ( NULL != att )
        {
            m_adListUrlRetiever.GetURL().SetURL( att->second );
        }
        att = n->GetAttribute( "ResumeDownloadsOnStartup" );
        if ( NULL != att )
        {
            m_resumeDownloadsOnStartup = GUCEF::CORE::StringToBool( att->second );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CDownloadsManager::OnNotify( GUCEF::CORE::CNotifier* notifier    ,
                             const GUCEF::CORE::CEvent& eventid  ,
                             GUCEF::CORE::CICloneable* eventdata )

{GUCEF_TRACE;
                             
    GUCEF::PATCHER::CPatchSetDirEngineEvents::DirProcessingStartedEvent.GetID();// == eventid;
    GUCEF::PATCHER::CPatchEngine::PatchListDataReceivedEvent == eventid;
    if ( GUCEF::PATCHER::CPatchManager::PatchTaskStartedEvent == eventid )
    {
       // const CString& taskName = static_cast< GUCEF::PATCHER::CPatchManager::TPatchTaskStartedEventData* >( eventdata )->GetData();
       // TProductMap::iterator i = m_productMap.find( taskName );
       // if ( i != m_productMap.end() )
        {
       //     TDownloadStartedEventData eData( (*i).second );
         //   NotifyObservers( DownloadStartedEvent, &eData );
        }
    }
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GU */

/*-------------------------------------------------------------------------*/
