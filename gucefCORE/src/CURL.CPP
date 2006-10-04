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

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CURL::URLActivateEvent = "GUCEF::CORE::CURL::URLActivateEvent";
const CString CURL::URLDeactivateEvent = "GUCEF::CORE::CURL::URLDeactivateEvent";
const CString CURL::URLDataRecievedEvent = "GUCEF::CORE::CURL::URLDataRecievedEvent";
const CString CURL::URLAllDataRecievedEvent = "GUCEF::CORE::CURL::URLAllDataRecievedEvent";
const CString CURL::URLDataRetrievalErrorEvent = "GUCEF::CORE::CURL::URLDataRetrievalErrorEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CURL::CURL( void )
        : m_handler( NULL )                  ,
          m_url()                            ,
          m_hURLActivateEvent( 0 )           ,
          m_hURLDeactivateEvent( 0 )         ,
          m_hURLDataRecievedEvent( 0 )       ,
          m_hURLAllDataRecievedEvent( 0 )    ,
          m_hURLDataRetrievalErrorEvent( 0 )
{TRACE;

        CNotificationIDRegistry* registry = CNotificationIDRegistry::Instance();
        m_hURLActivateEvent = registry->Lookup( CURLHandler::URLActivateEvent ); 
        m_hURLDeactivateEvent = registry->Lookup( CURLHandler::URLDeactivateEvent );
        m_hURLDataRecievedEvent = registry->Lookup( CURLHandler::URLDataRecievedEvent );
        m_hURLAllDataRecievedEvent = registry->Lookup( CURLHandler::URLAllDataRecievedEvent );
        m_hURLDataRetrievalErrorEvent = registry->Lookup( CURLHandler::URLDataRetrievalErrorEvent );    
}
        
/*-------------------------------------------------------------------------*/        
        
CURL::CURL( const CString& url )
        : m_handler( GetHandlerForURL( url ) ) ,
          m_url( url )                         ,
          m_hURLActivateEvent( 0 )             ,
          m_hURLDeactivateEvent( 0 )           ,
          m_hURLDataRecievedEvent( 0 )         ,
          m_hURLAllDataRecievedEvent( 0 )      ,
          m_hURLDataRetrievalErrorEvent( 0 )          
{TRACE;

        CNotificationIDRegistry* registry = CNotificationIDRegistry::Instance();
        m_hURLActivateEvent = registry->Lookup( CURLHandler::URLActivateEvent ); 
        m_hURLDeactivateEvent = registry->Lookup( CURLHandler::URLDeactivateEvent );
        m_hURLDataRecievedEvent = registry->Lookup( CURLHandler::URLDataRecievedEvent );
        m_hURLAllDataRecievedEvent = registry->Lookup( CURLHandler::URLAllDataRecievedEvent );
        m_hURLDataRetrievalErrorEvent = registry->Lookup( CURLHandler::URLDataRetrievalErrorEvent );   
        
        //m_handler
}

/*-------------------------------------------------------------------------*/

CURL::~CURL()
{TRACE;        
        AsObserver().UnsubscribeFrom( *m_handler );
        delete m_handler;
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
                m_handler = static_cast< CURLHandler* >( src.m_handler->Clone() );
                m_dataHandlers = src.m_dataHandlers;
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

void 
CURL::SetURL( const CString& url )
{TRACE; 
        m_url = url;
        delete m_handler;
        m_handler = GetHandlerForURL( url );
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
        return m_handler->Activate( *this, m_dataHandlers );
}

/*-------------------------------------------------------------------------*/
        
void
CURL::Deactivate( void )
{TRACE;
        m_handler->Deactivate( *this, m_dataHandlers );
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
        return static_cast<CURLHandler*>( CURLHandlerRegistry::Instance()->Lookup( url.SubstrToSubstr( ":\\" ) )->Clone() );
}

/*-------------------------------------------------------------------------*/

void
CURL::RegisterEvents( void )
{TRACE;

    CNotificationIDRegistry* registry( CNotificationIDRegistry::Instance() );
    registry->Register( URLActivateEvent, true );
    registry->Register( URLDeactivateEvent, true );
    registry->Register( URLDataRecievedEvent, true );
    registry->Register( URLAllDataRecievedEvent, true );
    registry->Register( URLDataRetrievalErrorEvent, true );
}

/*-------------------------------------------------------------------------*/

void
CURL::OnNotify( CNotifier* notifier                 ,
                const UInt32 eventid                ,
                CICloneable* eventdata /* = NULL */ )
{TRACE;
    
    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
