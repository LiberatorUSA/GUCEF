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
{GUCEF_TRACE;

    AddEventForwarding( URLActivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDeactivateEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLAllDataRecievedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( URLDataRetrievalErrorEvent, EVENTORIGINFILTER_TRANSFER );
    
    SubscribeTo( &m_url );
}

/*-------------------------------------------------------------------------*/

CURLDataRetriever::~CURLDataRetriever()
{GUCEF_TRACE;

    UnsubscribeFrom( &m_url );
}

/*-------------------------------------------------------------------------*/
    
CURL&
CURLDataRetriever::GetURL( void )
{GUCEF_TRACE;

    return m_url;
}

/*-------------------------------------------------------------------------*/

void
CURLDataRetriever::SetIOAccess( CIOAccess* ioAccess )
{GUCEF_TRACE;

    m_ioAccess = ioAccess;
}

/*-------------------------------------------------------------------------*/
    
CIOAccess*
CURLDataRetriever::GetIOAccess( void ) const
{GUCEF_TRACE;

    return m_ioAccess;
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