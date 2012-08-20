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

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#include "CURLDataRetriever.h"

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

CURLDataRetriever::CURLDataRetriever( void )
    : CForwardingNotifier()      ,
      CIURLEvents()              ,
      m_ioAccess( NULL )         ,
      m_url()                    ,
      m_totalBytesReceived( 0 )  ,
      m_totalBytes( -1 )         ,
      m_allDataReceived( false )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

CURLDataRetriever::CURLDataRetriever( CPulseGenerator& pulseGenerator )
    : CForwardingNotifier()      ,
      CIURLEvents()              ,
      m_ioAccess( NULL )         ,
      m_url( pulseGenerator )    ,
      m_totalBytesReceived( 0 )  ,
      m_totalBytes( -1 )         ,
      m_allDataReceived( false ) ,
      m_ownStorage( false )
{GUCEF_TRACE;

    Initialize();
}

/*-------------------------------------------------------------------------*/

void
CURLDataRetriever::Initialize( void )
{GUCEF_TRACE;

    AddForwardingForEvent( URLActivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( URLDeactivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( URLDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( URLAllDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( URLDataRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
    
    SubscribeTo( &m_url );
}

/*-------------------------------------------------------------------------*/

CURLDataRetriever::~CURLDataRetriever()
{GUCEF_TRACE;

    UnsubscribeFrom( &m_url );

    if ( m_ownStorage )
    {
        delete m_ioAccess;
        m_ioAccess = NULL;
    }
}

/*-------------------------------------------------------------------------*/

void
CURLDataRetriever::SetMemoryBufferAsStorage( void )
{GUCEF_TRACE;

    // Cleanup whatever storage we already have
    if ( m_ownStorage )
    {
        delete m_ioAccess;        
    }
    m_ioAccess = new CDynamicBufferAccess( new CDynamicBuffer(), true );
    m_ownStorage = true;
}

/*-------------------------------------------------------------------------*/

void
CURLDataRetriever::SetFileAsStorage( const CString& filePath )
{GUCEF_TRACE;

    // Cleanup whatever storage we already have
    if ( m_ownStorage )
    {
        delete m_ioAccess;        
    }
    m_ioAccess = new CFileAccess( filePath );
    m_ownStorage = true;
}

/*-------------------------------------------------------------------------*/
    
CURL&
CURLDataRetriever::GetURL( void )
{GUCEF_TRACE;

    return m_url;
}

/*-------------------------------------------------------------------------*/

UInt64
CURLDataRetriever::GetTotalBytesReceived( void ) const
{GUCEF_TRACE;
    
    return m_totalBytesReceived;
}

/*-------------------------------------------------------------------------*/

void
CURLDataRetriever::SetIOAccess( CIOAccess* ioAccess )
{GUCEF_TRACE;
    
    // Cleanup whatever storage we already have
    if ( m_ownStorage )
    {
        delete m_ioAccess;        
    }
    
    m_ioAccess = ioAccess;
    m_ownStorage = false;
}

/*-------------------------------------------------------------------------*/
    
CIOAccess*
CURLDataRetriever::GetIOAccess( void ) const
{GUCEF_TRACE;

    return m_ioAccess;
}

/*-------------------------------------------------------------------------*/

Int64
CURLDataRetriever::GetTotalResourceBytes( void ) const
{GUCEF_TRACE;

    return m_totalBytes;
}

/*-------------------------------------------------------------------------*/

Float32
CURLDataRetriever::GetTransferProgress( void ) const
{GUCEF_TRACE;

    if ( m_totalBytes > 0 )
    {
        return (Float32) ( m_totalBytesReceived / (UInt64) m_totalBytes );
    }
    
    // If we get here then we don't know the total size and will either have 0% or 100%
    if ( m_allDataReceived )
    {
        return 100;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CURLDataRetriever::OnNotify( CNotifier* notifier                 ,
                             const CEvent& eventid               ,
                             CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( m_ioAccess != NULL )
    {
        // We only process events from our own URL object
        if ( notifier == &m_url )
        {
            if ( eventid == URLActivateEvent )
            {
                // prepare to receive the data
                m_totalBytesReceived = 0;
                m_totalBytes = -1;
                m_allDataReceived = false;
                m_ioAccess->Open();
            }
            else
            if ( eventid == URLDataRecievedEvent )
            {
                if ( m_ioAccess->Opened() )
                {
                    // append the received data to the IO media
                    const TURLDataRecievedEventData* data = static_cast< TURLDataRecievedEventData* >( eventdata );
                    m_ioAccess->Write( data->GetData().GetConstBufferPtr(), data->GetData().GetDataSize(), 1 );
                    
                    m_totalBytesReceived += data->GetData().GetDataSize();
                }
            }
            else
            if ( eventid == URLAllDataRecievedEvent )
            {
                m_allDataReceived = true;
                m_ioAccess->Close();
            }            
            else
            if ( ( eventid == URLDeactivateEvent )         ||
                 ( eventid == URLDataRetrievalErrorEvent )  )
            {
                m_ioAccess->Close();
            }
        }
    }

    // Call the base class implementation to allow 
    // event forwarding to do its work
    CForwardingNotifier::OnNotify( notifier  ,
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