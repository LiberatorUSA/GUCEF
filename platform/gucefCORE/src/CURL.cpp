/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#include <stdio.h>                    /* needed for FILE* utils */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

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
    : CForwardingNotifier() ,
      CIURLEvents()         ,
      m_handler( NULL )     ,
      m_url()               ,
      m_pulseGenerator( CCoreGlobal::Instance()->GetPulseGenerator() )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CURL::CURL( const CURL& src )
    : CForwardingNotifier( src ) 
    , CIURLEvents()         
    , m_handler( NULL )     
    , m_url( src.m_url )               
    , m_pulseGenerator( src.m_pulseGenerator )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CURL::CURL( PulseGeneratorPtr pulseGenerator )
    : CForwardingNotifier()               ,
      CIURLEvents()                       ,
      m_handler( NULL )                   ,
      m_url()                             ,
      m_pulseGenerator( pulseGenerator )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CURL::CURL( const CString& url )
    : CForwardingNotifier()                ,
      CIURLEvents()                        ,
      m_handler( GetHandlerForURL( url ) ) ,
      m_url( url )                         ,
      m_pulseGenerator( CCoreGlobal::Instance()->GetPulseGenerator() )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CURL::CURL( const CString& url               ,
            PulseGeneratorPtr pulseGenerator )
    : CForwardingNotifier()                ,
      CIURLEvents()                        ,
      m_handler( GetHandlerForURL( url ) ) ,
      m_url( url )                         ,
      m_pulseGenerator( pulseGenerator )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

void
CURL::Initialize( void )
{GUCEF_TRACE;

    if ( m_handler == NULL )
    {
        m_url = NULL;
    }

    AddForwardingForEvent( URLActivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( URLDeactivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( URLDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( URLAllDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( URLDataRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
}

/*-------------------------------------------------------------------------*/

CURL::~CURL()
{GUCEF_TRACE;

    if ( m_handler )
    {
        UnsubscribeFrom( m_handler );
        GUCEF_DELETE m_handler;
    }
}

/*-------------------------------------------------------------------------*/

CURL&
CURL::operator=( const CURL& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        GUCEF_DELETE m_handler;
        m_handler = NULL;

        m_url = src.m_url;
        m_pulseGenerator = src.m_pulseGenerator;

        // We clone the handler, we want one exclusive to ourselves
        m_handler = static_cast< CURLHandler* >( src.m_handler->Clone() );

        SubscribeTo( m_handler );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CURL::Refresh( void )
{GUCEF_TRACE;

    Deactivate();
    Activate();
}

/*-------------------------------------------------------------------------*/

PulseGeneratorPtr
CURL::GetPulseGenerator( void )
{GUCEF_TRACE;

    return m_pulseGenerator;
}

/*-------------------------------------------------------------------------*/

bool
CURL::operator==( const CURL& other ) const
{GUCEF_TRACE;

        return m_url == other.m_url;
}

/*-------------------------------------------------------------------------*/

bool
CURL::operator!=( const CURL& other ) const
{GUCEF_TRACE;

    return !( (*this) == other );
}

/*-------------------------------------------------------------------------*/

bool
CURL::SetURL( const CString& url )
{GUCEF_TRACE;

    CORE::CString lowerCaseURL( url.Lowercase() );
    lowerCaseURL = lowerCaseURL.ReplaceChar( '\\', '/' );

    CURLHandler* newHandler = GetHandlerForURL( lowerCaseURL );
    if ( NULL != newHandler )
    {
        m_url = url;

        if ( m_handler )
        {
            UnsubscribeFrom( m_handler );
            GUCEF_DELETE m_handler;
            m_handler = NULL;
        }

        m_handler = newHandler;
        SubscribeTo( m_handler );

        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

const CString&
CURL::GetURL( void ) const
{GUCEF_TRACE;
        return m_url;
}

/*-------------------------------------------------------------------------*/

bool
CURL::Activate( void )
{GUCEF_TRACE;

    if ( m_handler != NULL )
    {
        return m_handler->Activate( *this );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CURL::Deactivate( void )
{GUCEF_TRACE;

    if ( m_handler != NULL )
    {
        m_handler->Deactivate( *this );
    }
}

/*-------------------------------------------------------------------------*/

bool
CURL::IsActive( void ) const
{GUCEF_TRACE;

    if ( m_handler != NULL )
    {
        return m_handler->IsActive( *this );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CURLHandler*
CURL::GetHandlerForURL( const CString& url ) const
{GUCEF_TRACE;

    CORE::CString protocolName( url.SubstrToSubstr( "://" ) );
    if ( protocolName.Length() )
    {
        CURLHandlerRegistry* registry = &CCoreGlobal::Instance()->GetUrlHandlerRegistry();
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
{GUCEF_TRACE;

    // Mandatory: call the base-class implementation
    CForwardingNotifier::OnNotify( notifier  ,
                                   eventid   ,
                                   eventdata );
}

/*-------------------------------------------------------------------------*/

const CString&
CURL::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::CORE::CURL";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
