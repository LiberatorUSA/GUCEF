/*
 *  gucefDRN: GUCEF module providing RAD networking trough data replication
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

#include <assert.h>

#ifndef GUCEF_DRN_CDRNPEERLINK_H
#include "gucefDRN_CDRNPeerLink.h"
#define GUCEF_DRN_CDRNPEERLINK_H
#endif /* GUCEF_DRN_CDRNPEERLINK_H ? */

#include "gucefDRN_CDRNPeerLinkData.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace DRN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CDRNPeerLinkData::CDRNPeerLinkData( CDRNPeerLink& peerLink )
    : m_peerLink( &peerLink )   ,
      m_subscribedDataGroups()  ,
      m_subscribedDataStreams() ,
      m_publicizedDataGroups()  ,
      m_publicizedDataStreams()
{GUCEF_TRACE;

    assert( m_peerLink!= NULL );
}

/*-------------------------------------------------------------------------*/
        
CDRNPeerLinkData::~CDRNPeerLinkData()
{GUCEF_TRACE;

    m_peerLink = NULL;
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::GetSubscribedDataGroups( TDRNDataGroupList& dataGroupList )
{GUCEF_TRACE;

    TDataGroupMap::iterator i = m_publicizedDataGroups.begin();
    while ( i != m_publicizedDataGroups.end() )
    {
//        dataGroupList.push_back( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::GetSubscribedDataStreams( TDRNDataStreamList& dataStreamList )
{GUCEF_TRACE;

    TDataStreamMap::iterator i = m_publicizedDataStreams.begin();
    while ( i != m_publicizedDataStreams.end() )
    {
//        dataStreamList.push_back( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::GetPublicizedDataGroups( TDRNDataGroupList& dataGroupList )
{GUCEF_TRACE;

    TDataGroupMap::iterator i = m_publicizedDataGroups.begin();
    while ( i != m_publicizedDataGroups.end() )
    {
//        dataGroupList.push_back( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::GetPublicizedDataStreams( TDRNDataStreamList& dataStreamList )
{GUCEF_TRACE;

    TDataStreamMap::iterator i = m_publicizedDataStreams.begin();
    while ( i != m_publicizedDataStreams.end() )
    {
        //dataStreamList.push_back( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::PublicizeStream( TDRNDataStreamPtr& dataStream )
{GUCEF_TRACE;

//    m_publicizedDataStreams[ dataStream->GetName() ] = dataStream;
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::StopStreamPublication( TDRNDataStreamPtr& dataStream )
{GUCEF_TRACE;

    m_publicizedDataStreams.erase( dataStream->GetName() );
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::StopStreamPublication( const CORE::CString& dataStreamName )
{GUCEF_TRACE;

    m_publicizedDataStreams.erase( dataStreamName );
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::PublicizeDataGroup( TDRNDataGroupPtr& dataGroup )
{GUCEF_TRACE;

//    m_publicizedDataGroups[ dataGroup->GetName() ] = dataGroup;
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::StopDataGroupPublication( TDRNDataStreamPtr& dataGroup )
{GUCEF_TRACE;

    m_publicizedDataGroups.erase( dataGroup->GetName() );
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::StopDataGroupPublication( const CORE::CString& dataGroupName )
{GUCEF_TRACE;

    m_publicizedDataGroups.erase( dataGroupName );
}

/*-------------------------------------------------------------------------*/

CDRNPeerLink&
CDRNPeerLinkData::GetPeerLink( void )
{GUCEF_TRACE;

    return *m_peerLink;
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::OnNotify( CORE::CNotifier* notifier                 ,
                            const CORE::CEvent& eventid               ,
                            CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataGroupPtr
CDRNPeerLinkData::GetPublicizedDataGroupWithName( const CORE::CString& name )
{GUCEF_TRACE;

    TDataGroupMap::iterator i = m_publicizedDataGroups.find( name );
    if ( i != m_publicizedDataGroups.end() )
    {
        return (*i).second.ptr;
    }
    return TDRNDataGroupPtr();
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataStreamPtr
CDRNPeerLinkData::GetPublicizedDataStreamWithName( const CORE::CString& name )
{GUCEF_TRACE;

    TDataStreamMap::iterator i = m_publicizedDataStreams.find( name );
    if ( i != m_publicizedDataStreams.end() )
    {
        return (*i).second.ptr;
    }
    return TDRNDataStreamPtr();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/