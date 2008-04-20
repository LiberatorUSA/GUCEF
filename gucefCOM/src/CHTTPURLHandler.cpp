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

#include "CMFileAccess.h"

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
            NotifyObservers( CIURLEvents::URLDataRetrievalErrorEvent );
            return;
        }
        if ( eventid == CHTTPClient::HTTPDataRecievedEvent )
        {
            // pass on the eventdata since it's a cloneable buffer it  will sort itself out
            NotifyObservers( CIURLEvents::URLDataRecievedEvent, eventdata );
            return;
        }
        if ( eventid == CHTTPClient::HTTPTransferFinishedEvent )
        {
            m_transferFinished = true;
            NotifyObservers( CIURLEvents::URLAllDataRecievedEvent );
            return;
        }
        if ( eventid == CHTTPClient::ConnectingEvent )
        {
            NotifyObservers( CIURLEvents::URLActivateEvent );
            return;
        }
        if ( eventid == CHTTPClient::DisconnectedEvent )
        {
            if ( !m_transferFinished )
            {
                NotifyObservers( CIURLEvents::URLDeactivateEvent );
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