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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPURLHandler::CHTTPURLHandler( void )
    : m_httpClient()              ,
      m_transferFinished( false )
{TRACE;

    SubscribeTo( &m_httpClient );
}

/*-------------------------------------------------------------------------*/
        
CHTTPURLHandler::CHTTPURLHandler( const CHTTPURLHandler& src )
    : m_httpClient()              ,
      m_transferFinished( false )
{TRACE;

    SubscribeTo( &m_httpClient );
}

/*-------------------------------------------------------------------------*/
        
CHTTPURLHandler::~CHTTPURLHandler()
{TRACE;

    UnsubscribeFrom( &m_httpClient );
}

/*-------------------------------------------------------------------------*/
        
CHTTPURLHandler&
CHTTPURLHandler::operator=( const CHTTPURLHandler& src )
{TRACE;

    if ( &src != this )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPURLHandler::Activate( CORE::CURL& url )
{TRACE;

    m_transferFinished = false;
    return m_httpClient.Get( url.GetURL() );
}

/*-------------------------------------------------------------------------*/
        
void
CHTTPURLHandler::Deactivate( CORE::CURL& url )
{TRACE;

    m_httpClient.Close();
}

/*-------------------------------------------------------------------------*/
                         
bool
CHTTPURLHandler::IsActive( const CORE::CURL& url ) const
{TRACE;

    return m_httpClient.IsConnected();
}

/*-------------------------------------------------------------------------*/
        
CORE::CICloneable*
CHTTPURLHandler::Clone( void ) const
{TRACE;

    return new CHTTPURLHandler( *this );
}

/*-------------------------------------------------------------------------*/

void
CHTTPURLHandler::OnNotify( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventid               ,
                           CORE::CICloneable* eventdata /* = NULL */ )
{TRACE;

    // We only accept events from our own HTTP client
    if ( notifier == &m_httpClient )
    {
        if ( ( eventid == CHTTPClient::HTTPErrorEvent )       ||
             ( eventid == CHTTPClient::ConnectionErrorEvent )  )
        {
            NotifyObservers( CIURLEvents::URLDataRetrievalErrorEvent );
            return;
        }
        if ( eventid == CHTTPClient::HTTPDataRecievedEvent )
        {
            //CHTTPClient::THTTPDataRecievedEventData* eData = static_cast< CHTTPClient::THTTPDataRecievedEventData* >( eventdata );            
            //CORE::CMFileAccess mAccess( eData->GetData().GetConstBufferPtr(), eData->GetData().GetDataSize() );
            
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
{TRACE;
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