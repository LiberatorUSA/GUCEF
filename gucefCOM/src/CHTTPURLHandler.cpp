/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#include "CURLHandlerRegistry.h"
#define GUCEF_CORE_CURLHANDLERREGISTRY_H
#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */

#include "CHTTPURLHandler.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPURLHandler::CHTTPURLHandler( void )
    : CURLHandler()               ,
      m_httpClient( NULL )        ,
      m_transferFinished( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
        
CHTTPURLHandler::CHTTPURLHandler( const CHTTPURLHandler& src )
    : CURLHandler( src )          ,
      m_httpClient( NULL )        ,
      m_transferFinished( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
        
CHTTPURLHandler::~CHTTPURLHandler()
{GUCEF_TRACE;

    if ( NULL != m_httpClient )
    {
        UnsubscribeFrom( m_httpClient );
        delete m_httpClient;
        m_httpClient = NULL;
    }
}

/*-------------------------------------------------------------------------*/
        
CHTTPURLHandler&
CHTTPURLHandler::operator=( const CHTTPURLHandler& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPURLHandler::Activate( CORE::CURL& url )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPURLHandler(" + CORE::PointerToString( this ) + "): Activating using URL: " + url.GetURL() ); 
    
    m_transferFinished = false;
    
    delete m_httpClient;
    m_httpClient = new CHTTPClient( url.GetPulseGenerator() );
    SubscribeTo( m_httpClient );
    
    return m_httpClient->Get( url.GetURL() );
}

/*-------------------------------------------------------------------------*/
        
void
CHTTPURLHandler::Deactivate( CORE::CURL& url )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPURLHandler(" + CORE::PointerToString( this ) + "): Deactivating" );
    
    if ( NULL != m_httpClient )
    {
        m_httpClient->Close();
    }
}

/*-------------------------------------------------------------------------*/
                         
bool
CHTTPURLHandler::IsActive( const CORE::CURL& url ) const
{GUCEF_TRACE;

    if ( NULL != m_httpClient )
    {
        return m_httpClient->IsConnected();
    }
    return false;
}

/*-------------------------------------------------------------------------*/
        
CORE::CICloneable*
CHTTPURLHandler::Clone( void ) const
{GUCEF_TRACE;

    return new CHTTPURLHandler( *this );
}

/*-------------------------------------------------------------------------*/

void
CHTTPURLHandler::OnNotify( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventid               ,
                           CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    // We only accept events from our own HTTP client
    if ( notifier == m_httpClient )
    {
        if ( ( eventid == CHTTPClient::HTTPErrorEvent )       ||
             ( eventid == CHTTPClient::ConnectionErrorEvent )  )
        {
            
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPURLHandler(" + CORE::PointerToString( this ) + "): Data retrieval error" );
            NotifyObservers( CIURLEvents::URLDataRetrievalErrorEvent );
            return;
        }
        if ( eventid == CHTTPClient::HTTPDataRecievedEvent )
        {
            // pass on the eventdata since it's a cloneable buffer it  will sort itself out
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPURLHandler(" + CORE::PointerToString( this ) + "): Data received" );
            NotifyObservers( CIURLEvents::URLDataRecievedEvent, eventdata );
            return;
        }
        if ( eventid == CHTTPClient::HTTPTransferFinishedEvent )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPURLHandler(" + CORE::PointerToString( this ) + "): Transfer finished" );
            m_transferFinished = true;
            NotifyObservers( CIURLEvents::URLAllDataRecievedEvent );
            return;
        }
        if ( eventid == CHTTPClient::ConnectingEvent )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPURLHandler(" + CORE::PointerToString( this ) + "): Connecting" );
            NotifyObservers( CIURLEvents::URLActivateEvent );
            return;
        }
        if ( eventid == CHTTPClient::DisconnectedEvent )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPURLHandler(" + CORE::PointerToString( this ) + "): Disconnected" );
            if ( !m_transferFinished )
            {
                NotifyObservers( CIURLEvents::URLDeactivateEvent );
            }
            else
            {
                NotifyObservers( CIURLEvents::URLAllDataRecievedEvent );
            }
            return;
        }        
    }
}

/*-------------------------------------------------------------------------*/

void 
CHTTPURLHandler::Register( void )
{GUCEF_TRACE;

    CORE::CURLHandlerRegistry* registry = CORE::CURLHandlerRegistry::Instance();
    if ( !registry->IsRegistered( "http" ) )
    {
        registry->Register( "http", new CHTTPURLHandler() );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/