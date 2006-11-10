/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#include <stdio.h>                    /* needed for FILE* utils */

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#include "CURLHandlerRegistry.h"      /* central registry for URL handlers */
#define GUCEF_CORE_CURLHANDLERREGISTRY_H
#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */ 

#ifndef GUCEF_CORE_CURLHANDLER_H
#include "CURLHandler.h"              /* handler for the url protocol */
#define GUCEF_CORE_CURLHANDLER_H
#endif /* GUCEF_CORE_CURLHANDLER_H ? */

#include "CURL.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CURL::CURL( void )
        : m_handler( NULL )                                          ,
          m_url()                                                    ,
          m_URLActivateEvent( URLActivateEvent )                     ,
          m_URLDeactivateEvent( URLDeactivateEvent )                 ,
          m_URLDataRecievedEvent( URLDataRecievedEvent )             ,
          m_URLAllDataRecievedEvent( URLAllDataRecievedEvent )       ,
          m_URLDataRetrievalErrorEvent( URLDataRetrievalErrorEvent )         
{TRACE;
        
    AddEventForwarding( m_URLActivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( m_URLDeactivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( m_URLDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( m_URLAllDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( m_URLDataRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );  
}
        
/*-------------------------------------------------------------------------*/        
        
CURL::CURL( const CString& url )
        : m_handler( GetHandlerForURL( url ) )                       ,
          m_url( url )                                               ,
          m_URLActivateEvent( URLActivateEvent )                     ,
          m_URLDeactivateEvent( URLDeactivateEvent )                 ,
          m_URLDataRecievedEvent( URLDataRecievedEvent )             ,
          m_URLAllDataRecievedEvent( URLAllDataRecievedEvent )       ,
          m_URLDataRetrievalErrorEvent( URLDataRetrievalErrorEvent )           
{TRACE;

    AddEventForwarding( m_URLActivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( m_URLDeactivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( m_URLDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( m_URLAllDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( m_URLDataRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
}

/*-------------------------------------------------------------------------*/

CURL::~CURL()
{TRACE;        

    if ( m_handler )
    {
        UnsubscribeFrom( m_handler );
        delete m_handler;
    }
}

/*-------------------------------------------------------------------------*/

CURL& 
CURL::operator=( const CURL& src )
{TRACE;
        if ( &src != this )
        {
                delete m_handler;
                m_handler = NULL;

                m_url = src.m_url;
                
                // We clone the handler, we want one exclusive to ourselves
                m_handler = static_cast< CURLHandler* >( src.m_handler->Clone() );
                
                SubscribeTo( m_handler );
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

bool
CURL::operator==( const CURL& other ) const
{TRACE;
        return m_url == other.m_url;
}

/*-------------------------------------------------------------------------*/
        
bool
CURL::operator!=( const CURL& other ) const
{TRACE;
        return !( (*this) == other );
}
        
/*-------------------------------------------------------------------------*/        

bool 
CURL::SetURL( const CString& url )
{TRACE; 

    CURLHandler* newHandler = GetHandlerForURL( url );
    if ( NULL != newHandler )
    {
        m_url = url;
        
        if ( m_handler )
        {
            UnsubscribeFrom( m_handler );
            delete m_handler;
            m_handler = NULL;
        }
        
        m_handler = newHandler;
        SubscribeTo( m_handler );
        
        return true;            
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/
        
CString 
CURL::GetURL( void ) const
{TRACE;
        return m_url;
}

/*-------------------------------------------------------------------------*/

bool
CURL::Activate( void )
{TRACE;
        return m_handler->Activate( *this );
}

/*-------------------------------------------------------------------------*/
        
void
CURL::Deactivate( void )
{TRACE;
        m_handler->Deactivate( *this );
}

/*-------------------------------------------------------------------------*/

bool
CURL::IsActive( void ) const
{TRACE;
        return m_handler->IsActive( *this );
}

/*-------------------------------------------------------------------------*/

CURLHandler* 
CURL::GetHandlerForURL( const CString& url ) const
{TRACE;

    CORE::CString protocolName( url.SubstrToSubstr( ":\\" ) );
    if ( protocolName.Length() )
    {
        CURLHandlerRegistry* registry = CURLHandlerRegistry::Instance();
        if ( registry->IsRegistered( protocolName ) )
        {
            // We clone the handler stored in the registry, we want one exclusive to ourselves
            return static_cast<CURLHandler*>( registry->Lookup( protocolName )->Clone() );
        }
    }
    
    return NULL;
}

/*-------------------------------------------------------------------------*/

void
CURL::OnNotify( CNotifier* notifier                 ,
                const CEvent& eventid               ,
                CICloneable* eventdata /* = NULL */ )
{TRACE;
    
    // Manditory: call the base-class implementation
    CObservingNotifier::OnNotify( notifier  ,
                                  eventid   ,
                                  eventdata );
}

/*-------------------------------------------------------------------------*/

CEvent
CURL::GetURLActivateEventID( void ) const
{TRACE;
    
    return m_URLActivateEvent;
}

/*-------------------------------------------------------------------------*/

CEvent
CURL::GetURLDeactivateEventID( void ) const
{TRACE;
    
    return m_URLDeactivateEvent;
}

/*-------------------------------------------------------------------------*/

CEvent
CURL::GetURLDataRecievedEventID( void ) const
{TRACE;
    
    return m_URLDataRecievedEvent;
}

/*-------------------------------------------------------------------------*/

CEvent
CURL::GetURLAllDataRecievedEventID( void ) const
{TRACE;

    return m_URLAllDataRecievedEvent;
}

/*-------------------------------------------------------------------------*/

CEvent
CURL::GetURLDataRetrievalErrorEventID( void ) const
{TRACE;

    return m_URLDataRetrievalErrorEvent;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
