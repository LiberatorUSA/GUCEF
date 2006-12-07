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
    : m_url()    ,
      m_buffer()
{TRACE;

    AddEventForwarding( GetURLActivateEventID(), EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( GetURLDeactivateEventID(), EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( GetURLDataRecievedEventID(), EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( GetURLAllDataRecievedEventID(), EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( GetURLDataRetrievalErrorEventID(), EVENTORIGINFILTER_TRANSFER );
    
    SubscribeTo( &m_url );
}

/*-------------------------------------------------------------------------*/

CURLDataRetriever::~CURLDataRetriever()
{TRACE;

    UnsubscribeFrom( &m_url );
}

/*-------------------------------------------------------------------------*/
    
CURL&
CURLDataRetriever::GetURL( void )
{TRACE;

    return m_url;
}

/*-------------------------------------------------------------------------*/
    
CDynamicBuffer&
CURLDataRetriever::GetBuffer( void )
{TRACE;

    return m_buffer;
}

/*-------------------------------------------------------------------------*/
    
CEvent
CURLDataRetriever::GetURLActivateEventID( void ) const
{TRACE;

    return m_url.GetURLActivateEventID();
}

/*-------------------------------------------------------------------------*/

CEvent
CURLDataRetriever::GetURLDeactivateEventID( void ) const
{TRACE;

    return m_url.GetURLDeactivateEventID();
}

/*-------------------------------------------------------------------------*/

CEvent
CURLDataRetriever::GetURLDataRecievedEventID( void ) const
{TRACE;

    return m_url.GetURLDataRecievedEventID();
}

/*-------------------------------------------------------------------------*/

CEvent
CURLDataRetriever::GetURLAllDataRecievedEventID( void ) const
{TRACE;

    return m_url.GetURLAllDataRecievedEventID();
}

/*-------------------------------------------------------------------------*/

CEvent
CURLDataRetriever::GetURLDataRetrievalErrorEventID( void ) const
{TRACE;

    return m_url.GetURLDataRetrievalErrorEventID();
}

/*-------------------------------------------------------------------------*/

void
CURLDataRetriever::OnNotify( CNotifier* notifier                 ,
                             const CEvent& eventid               ,
                             CICloneable* eventdata /* = NULL */ )
{TRACE;

    // We only process events from our own URL object
    if ( notifier == &m_url )
    {
        if ( eventid == GetURLActivateEventID() )
        {
            // perform a logical clear of the buffer
            m_buffer.Clear( true );
            return;
        }
        if ( eventid == GetURLDataRecievedEventID() )
        {
            // append the recieved data to the data in our buffer (if any)
            const TURLDataRecievedEventData* data = static_cast< TURLDataRecievedEventData* >( eventdata );
            m_buffer.Append( data->GetData().GetConstBufferPtr(), data->GetData().GetDataSize() );
            return;
        }        
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/