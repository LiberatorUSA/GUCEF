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
        : m_handler( NULL ) ,
          m_url()       
{TRACE;
        
    AddEventForwarding( URLActivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDeactivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLAllDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDataRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );  
}
        
/*-------------------------------------------------------------------------*/        
        
CURL::CURL( const CString& url )
        : m_handler( GetHandlerForURL( url ) ) ,
          m_url( url )           
{TRACE;

    if ( m_handler == NULL )
    {
        m_url = NULL;
    }
    
    AddEventForwarding( URLActivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDeactivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLAllDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDataRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
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

    CORE::CString lowerCaseURL( url.Lowercase() );
    lowerCaseURL = lowerCaseURL.ReplaceChar( '\\', '/' );
    
    CURLHandler* newHandler = GetHandlerForURL( lowerCaseURL );
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
        
    if ( m_handler != NULL )
    {
        return m_handler->Activate( *this );
    }
    return false;
}

/*-------------------------------------------------------------------------*/
        
void
CURL::Deactivate( void )
{TRACE;

    if ( m_handler != NULL )
    {
        m_handler->Deactivate( *this );
    }
}

/*-------------------------------------------------------------------------*/

bool
CURL::IsActive( void ) const
{TRACE;

    if ( m_handler != NULL )
    {
        return m_handler->IsActive( *this );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CURLHandler* 
CURL::GetHandlerForURL( const CString& url ) const
{TRACE;

    CORE::CString protocolName( url.SubstrToSubstr( "://" ) );
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
    
    // Mandatory: call the base-class implementation
    CObservingNotifier::OnNotify( notifier  ,
                                  eventid   ,
                                  eventdata );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
