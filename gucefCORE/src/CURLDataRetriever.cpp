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
    : m_url()
{TRACE;

    AddEventForwarding( URLActivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDeactivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLAllDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDataRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
    
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

void
CURLDataRetriever::SetIOAccess( CIOAccess* ioAccess )
{TRACE;

    m_ioAccess = ioAccess;
}

/*-------------------------------------------------------------------------*/
    
CIOAccess*
CURLDataRetriever::GetIOAccess( void ) const
{TRACE;

    return m_ioAccess;
}

/*-------------------------------------------------------------------------*/

void
CURLDataRetriever::OnNotify( CNotifier* notifier                 ,
                             const CEvent& eventid               ,
                             CICloneable* eventdata /* = NULL */ )
{TRACE;

    if ( m_ioAccess != NULL )
    {
        // We only process events from our own URL object
        if ( notifier == &m_url )
        {
            if ( eventid == URLActivateEvent )
            {
                // prepare to receive the data
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
                }
            }
            else
            if ( ( eventid == URLDeactivateEvent )         ||
                 ( eventid == URLAllDataRecievedEvent )    ||
                 ( eventid == URLDataRetrievalErrorEvent )  )
            {
                m_ioAccess->Close();
            }
        }
    }

    // Call the base class implementation to allow 
    // event forwarding to do its work
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