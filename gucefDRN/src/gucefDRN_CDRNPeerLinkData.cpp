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
    : m_peerLink( &peerLink )     ,
      m_subscribedDataGroups()    ,
      m_subscribedDataStreams()   ,
      m_publicizedDataGroups()    ,
      m_publicizedDataStreams()   ,
      m_subscribedDataGroupsID()  ,
      m_subscribedDataStreamsID() ,
      m_publicizedDataGroupsID()  ,
      m_publicizedDataStreamsID()
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

    TDataGroupMap::iterator i = m_subscribedDataGroups.begin();
    while ( i != m_subscribedDataGroups.end() )
    {
        dataGroupList.push_back( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::GetSubscribedDataStreams( TDRNDataStreamList& dataStreamList )
{GUCEF_TRACE;

    TDataStreamMap::iterator i = m_subscribedDataStreams.begin();
    while ( i != m_subscribedDataStreams.end() )
    {
        dataStreamList.push_back( (*i).second );
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
        dataGroupList.push_back( (*i).second );
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
        dataStreamList.push_back( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::PublicizeStream( TDRNDataStreamPtr& dataStream )
{GUCEF_TRACE;

    m_publicizedDataStreams[ dataStream->GetName() ] = dataStream;    
    TDRNDataStreamEntry entry = { dataStream, new CORE::T16BitNumericID( m_idGenerator.GenerateID() ) };
    m_publicizedDataStreamsID[ *entry.id ] = entry;
    
    SubscribeTo( dataStream.GetPointer(), CDRNDataStream::DataTransmittedEvent );
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::StopStreamPublication( TDRNDataStreamPtr& dataStream )
{GUCEF_TRACE;

    TDataStreamMap::iterator i = m_publicizedDataStreams.find( dataStream->GetName() );
    if ( i != m_publicizedDataStreams.end() )
    {
        UnsubscribeFrom( (*i).second.GetPointer() );
        m_publicizedDataStreams.erase( i );
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::StopStreamPublication( const CORE::CString& dataStreamName )
{GUCEF_TRACE;

    TDataStreamMap::iterator i = m_publicizedDataStreams.find( dataStreamName );
    if ( i != m_publicizedDataStreams.end() )
    {
        UnsubscribeFrom( (*i).second.GetPointer() );
        m_publicizedDataStreams.erase( i );
    }
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::PublicizeDataGroup( TDRNDataGroupPtr& dataGroup )
{GUCEF_TRACE;

    m_publicizedDataGroups[ dataGroup->GetName() ] = dataGroup;    
    TDRNDataGroupEntry entry = { dataGroup, new CORE::T16BitNumericID( m_idGenerator.GenerateID() ) };
    m_publicizedDataGroupsID[ *entry.id ] = entry;
    
    SubscribeTo( dataGroup.GetPointer(), CDRNDataGroup::ItemChangedEvent );    
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLinkData::StopDataGroupPublication( TDRNDataStreamPtr& dataGroup )
{GUCEF_TRACE;

    TDataGroupMap::iterator i = m_publicizedDataGroups.find( dataGroup->GetName() );
    if ( i != m_publicizedDataGroups.end() )
    {
        UnsubscribeFrom( (*i).second.GetPointer() );
        m_publicizedDataGroups.erase( i );
    }    
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::StopDataGroupPublication( const CORE::CString& dataGroupName )
{GUCEF_TRACE;

    TDataGroupMap::iterator i = m_publicizedDataGroups.find( dataGroupName );
    if ( i != m_publicizedDataGroups.end() )
    {
        UnsubscribeFrom( (*i).second.GetPointer() );
        m_publicizedDataGroups.erase( i );
    }    
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

    if ( CDRNDataStream::DataTransmittedEvent == eventid )
    {
        CDRNDataStream& stream = static_cast< CDRNDataStream& >( *notifier );
        
        UInt16 id = 0;
        if ( GetPublicizedDataStreamID( stream.GetName() ,
                                        id               ) )
        {
            CDRNDataStream::DataTransmittedEventData& eData = static_cast< CDRNDataStream::DataTransmittedEventData& >( *eventdata );
            m_peerLink->SendStreamDataToPeer( id              ,
                                              eData.GetData() ,
                                              false           );
        }
    }
    else
    if ( CDRNDataGroup::ItemChangedEvent == eventid )
    {
        CDRNDataGroup& dataGroup = static_cast< CDRNDataGroup& >( *notifier );
        
        UInt16 id = 0;
        if ( GetPublicizedDataGroupID( dataGroup.GetName() ,
                                       id                  ) )
        {
            CDRNDataGroup::ItemChangedEventData& eData = static_cast< CDRNDataGroup::ItemChangedEventData& >( *eventdata );
            m_peerLink->SendDataGroupItemUpdateToPeer( dataGroup             ,
                                                       id                    ,
                                                       *eData.GetData().id   ,
                                                       *eData.GetData().data );
        }        
    }
    else
    if ( ( CORE::CNotifier::UnsubscribeEvent == eventid ) ||
         ( CORE::CNotifier::DestructionEvent == eventid )  )
    {
        RemoveLinksTo( notifier );
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::RemoveLinksTo( CORE::CNotifier* notifier )
{GUCEF_TRACE;

    //@TODO
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataGroupPtr
CDRNPeerLinkData::GetPublicizedDataGroupWithName( const CORE::CString& name )
{GUCEF_TRACE;

    TDataGroupMap::iterator i = m_publicizedDataGroups.find( name );
    if ( i != m_publicizedDataGroups.end() )
    {
        return (*i).second;
    }
    return TDRNDataGroupPtr();
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataGroupPtr
CDRNPeerLinkData::GetPublicizedDataGroupWithID( const UInt16 id )
{GUCEF_TRACE;

    TDataGroupIDMap::iterator i = m_publicizedDataGroupsID.find( id );
    if ( i != m_publicizedDataGroupsID.end() )
    {
        return (*i).second.ptr;
    }
    return TDRNDataGroupPtr();
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLinkData::GetPublicizedDataGroupID( const CORE::CString& name ,
										    UInt16& id                )
{GUCEF_TRACE;

	TDataGroupIDMap::iterator i = m_publicizedDataGroupsID.begin();
	while ( i != m_publicizedDataGroupsID.end() )
	{
		if ( name == (*i).second.ptr->GetName() )
		{
			id = (*i).first;
			return true;
		}
		++i;
	}
	return false;
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLinkData::GetSubscribedDataGroupID( const CORE::CString& name ,
		                                    UInt16& id                )
{GUCEF_TRACE;

	TIDToDataGroupMap::iterator i = m_subscribedDataGroupsID.begin();
	while ( i != m_subscribedDataGroupsID.end() )
	{
		if ( name == (*i).second->GetName() )
		{
			id = (*i).first;
			return true;
		}
		++i;
	}
	return false;
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLinkData::GetSubscribedDataStreamID( const CORE::CString& name ,
		                                     UInt16& id                )
{GUCEF_TRACE;
	
	TIDToDataStreamMap::iterator i = m_subscribedDataStreamsID.begin();
	while ( i != m_subscribedDataStreamsID.end() )
	{
		if ( name == (*i).second->GetName() )
		{
			id = (*i).first;
			return true;
		}
		++i;
	}
	return false;
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataStreamPtr
CDRNPeerLinkData::GetPublicizedDataStreamWithName( const CORE::CString& name )
{GUCEF_TRACE;

    TDataStreamMap::iterator i = m_publicizedDataStreams.find( name );
    if ( i != m_publicizedDataStreams.end() )
    {
        return (*i).second;
    }
    return TDRNDataStreamPtr();
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataStreamPtr
CDRNPeerLinkData::GetPublicizedDataStreamWithID( const UInt16 id )
{GUCEF_TRACE;

    TDataStreamIDMap::iterator i = m_publicizedDataStreamsID.find( id );
    if ( i != m_publicizedDataStreamsID.end() )
    {
        return (*i).second.ptr;
    }
    return TDRNDataStreamPtr();	
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLinkData::GetPublicizedDataStreamID( const CORE::CString& name ,
										     UInt16& id                )
{GUCEF_TRACE;

	TDataStreamIDMap::iterator i = m_publicizedDataStreamsID.begin();
	while ( i != m_publicizedDataStreamsID.end() )
	{
		if ( name == (*i).second.ptr->GetName() )
		{
			id = (*i).first;
			return true;
		}
		++i;
	}
	return false;
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::RemoveSubscribedDataStream( const UInt16 dataStreamID )
{GUCEF_TRACE;

    TDRNDataStreamPtr dataStream;
    TIDToDataStreamMap::iterator i = m_subscribedDataStreamsID.find( dataStreamID );
    if ( i != m_subscribedDataStreamsID.end() )
    {
        dataStream = (*i).second;
        m_subscribedDataStreamsID.erase( i );
        m_subscribedDataStreams.erase( dataStream->GetName() );
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLinkData::RemoveSubscribedDataGroup( const UInt16 dataGroupID )
{GUCEF_TRACE;

    TDRNDataGroupPtr dataGroup;
    TIDToDataGroupMap::iterator i = m_subscribedDataGroupsID.find( dataGroupID );
    if ( i != m_subscribedDataGroupsID.end() )
    {
        dataGroup = (*i).second;
        m_subscribedDataGroupsID.erase( i );
        m_subscribedDataGroups.erase( dataGroup->GetName() );
    }
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataGroupPtr
CDRNPeerLinkData::AddSubscribedDataGroup( const CORE::CString& dataGroupName ,
                                          const UInt16 dataGroupID           )
{GUCEF_TRACE;

    TDRNDataGroupPtr dataGroupPtr( new CDRNDataGroup( dataGroupName ) );
    m_subscribedDataGroups.insert( std::pair< CORE::CString, TDRNDataGroupPtr >( dataGroupName, dataGroupPtr ) );    
    m_subscribedDataGroupsID.insert( std::pair< UInt16, TDRNDataGroupPtr >( dataGroupID, dataGroupPtr ) );
    
    return dataGroupPtr;
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataStreamPtr
CDRNPeerLinkData::AddSubscribedDataStream( const CORE::CString& dataStreamName ,
                                           const UInt16 dataStreamID           )
{GUCEF_TRACE;

    TDRNDataStreamPtr dataStreamPtr( new CDRNDataStream( dataStreamName ) );
    m_subscribedDataStreams.insert( std::pair< CORE::CString, TDRNDataStreamPtr >( dataStreamName, dataStreamPtr ) );
    m_subscribedDataStreamsID.insert( std::pair< UInt16, TDRNDataStreamPtr >( dataStreamID, dataStreamPtr ) );
    
    return dataStreamPtr;
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataGroupPtr
CDRNPeerLinkData::GetSubscribedDataGroupWithName( const CORE::CString& name )
{GUCEF_TRACE;

    TDataGroupMap::iterator i = m_subscribedDataGroups.find( name );
    if ( i != m_subscribedDataGroups.end() )
    {
        return (*i).second;
    }
    return TDRNDataGroupPtr();
}

/*-------------------------------------------------------------------------*/
    
CDRNPeerLinkData::TDRNDataStreamPtr
CDRNPeerLinkData::GetSubscribedDataStreamWithName( const CORE::CString& name )
{GUCEF_TRACE;

    TDataStreamMap::iterator i = m_subscribedDataStreams.find( name );
    if ( i != m_subscribedDataStreams.end() )
    {
        return (*i).second;
    }
    return TDRNDataStreamPtr();
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataGroupPtr
CDRNPeerLinkData::GetSubscribedDataGroupWithID( const UInt16 dataGroupID )
{GUCEF_TRACE;

    TIDToDataGroupMap::iterator i = m_subscribedDataGroupsID.find( dataGroupID );
    if ( i != m_subscribedDataGroupsID.end() )
    {
        return (*i).second;
    }
    return TDRNDataGroupPtr();
}

/*-------------------------------------------------------------------------*/

CDRNPeerLinkData::TDRNDataStreamPtr
CDRNPeerLinkData::GetSubscribedDataStreamWithID( const UInt16 dataStreamID )
{GUCEF_TRACE;

    TIDToDataStreamMap::iterator i = m_subscribedDataStreamsID.find( dataStreamID );
    if ( i != m_subscribedDataStreamsID.end() )
    {
        return (*i).second;
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