/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#include "CMFileAccess.h"
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
    : m_httpClient()
{TRACE;

    SubscribeTo( &m_httpClient );
}

/*-------------------------------------------------------------------------*/
        
CHTTPURLHandler::CHTTPURLHandler( const CHTTPURLHandler& src )
    : m_httpClient()
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
        if ( ( eventid == m_httpClient.GetHTTPErrorEventID() )       ||
             ( eventid == m_httpClient.GetConnectionErrorEventID() )  )
        {
            NotifyObservers( CIURLEvents::URLDataRetrievalErrorEvent );
            return;
        }
        if ( eventid == m_httpClient.GetHTTPDataRecievedEventID() )
        {
            CHTTPClient::THTTPDataRecievedEventData* eData = static_cast< CHTTPClient::THTTPDataRecievedEventData* >( eventdata );            
            CORE::CMFileAccess mAccess( eData->GetData().GetConstBufferPtr(), eData->GetData().GetDataSize() );
            NotifyObservers( CIURLEvents::URLDataRecievedEvent, &mAccess );
            return;
        }
        if ( eventid == m_httpClient.GetHTTPTransferFinishedEventID() )
        {
            NotifyObservers( CIURLEvents::URLAllDataRecievedEvent );
            return;
        }
        if ( eventid == m_httpClient.GetConnectingEventID() )
        {
            NotifyObservers( CIURLEvents::URLActivateEvent );
            return;
        }
        if ( eventid == m_httpClient.GetDisconnectedEventID() )
        {
            NotifyObservers( CIURLEvents::URLDeactivateEvent );
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