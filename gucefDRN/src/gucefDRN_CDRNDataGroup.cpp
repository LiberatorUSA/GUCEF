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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CSTREAMEREVENTS_H
#include "CStreamerEvents.h"
#define GUCEF_CORE_CSTREAMEREVENTS_H
#endif /* GUCEF_CORE_CSTREAMEREVENTS_H ? */

#include "gucefDRN_CDRNDataGroup.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace DRN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CDRNDataGroup::ItemChangedEvent = "GUCEF::CORE::CDRNDataGroup::ItemChangedEvent";
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
   
CDRNDataGroup::CDRNDataGroup( const CORE::CString& groupName )
    : CObservingNotifier()                          ,
      m_properties( new CDRNDataGroupProperties() ) ,
      m_groupName( groupName )                      ,
      m_dataMap()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/
    
CDRNDataGroup::~CDRNDataGroup()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CDRNDataGroup::RegisterEvents( void )
{GUCEF_TRACE;

    ItemChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

bool
CDRNDataGroup::SetItem( const CORE::CDynamicBuffer& id        ,
                        const CORE::CDynamicBuffer& data      ,
                        const bool addIfNotFound /* = true */ )
{GUCEF_TRACE;

    TDataMap::iterator i = m_dataMap.find( id );
    if ( i != m_dataMap.end() )
    {
        (*i).second = data;
        
        struct ItemEntry entry = { &(*i).first, &(*i).second };
        ItemChangedEventData eData( entry );
        NotifyObservers( ItemChangedEvent, &eData );
        
        return true;
    }
    else
    {
        if ( addIfNotFound )
        {
            i = m_dataMap.insert( std::pair< CORE::CDynamicBuffer, CORE::CDynamicBuffer >( id, data ) ).first;
            
            struct ItemEntry entry = { &(*i).first, &(*i).second };
            ItemChangedEventData eData( entry );
            NotifyObservers( ItemChangedEvent, &eData );            
        }
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CDRNDataGroup::GetItem( const CORE::CDynamicBuffer& id ,
                        CORE::CDynamicBuffer& data     ) const
{GUCEF_TRACE;

    TDataMap::const_iterator i = m_dataMap.find( id );
    if ( i != m_dataMap.end() )
    {
        data = (*i).second;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CDRNDataGroup::HasItem( const CORE::CDynamicBuffer& id ) const
{GUCEF_TRACE;

    return m_dataMap.end() != m_dataMap.find( id );
}

/*-------------------------------------------------------------------------*/

void
CDRNDataGroup::OnNotify( CORE::CNotifier* notifier                 ,
                         const CORE::CEvent& eventid               ,
                         CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    // Call base-class implementation,.. mandatory
    CObservingNotifier::OnNotify( notifier  ,
                                  eventid   ,
                                  eventdata );
                                 
    if ( ( eventid == CORE::CStreamerEvents::IDStreamEvent )   ||
         ( eventid == CORE::CStreamerEvents::DataStreamEvent )  )
    {
        // If streamer updates are disabled there is no point in checking anything else
        if ( m_properties->GetAcceptUpdatesFromStreamers() )
        {
            const CORE::CStreamerEvents::SStreamEventData& eData = static_cast< CORE::CStreamerEvents::TStreamEventData* >( eventdata )->GetData();
            
            // make sure the streamed data has an ID, we do not accept data without an id
            if ( eData.id != NULL )
            {
                CORE::CDynamicBuffer id;
                eData.id->StreamToBuffer( id );
                
                CORE::CDynamicBuffer data;
                eData.data->StreamToBuffer( data );
                
                SetItem( id                                        ,
                         data                                      ,
                         m_properties->GetAcceptNewStreamerItems() );
            }
        }
    }
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CDRNDataGroup::GetName( void ) const
{GUCEF_TRACE;

    return m_groupName;
}

/*-------------------------------------------------------------------------*/

void
CDRNDataGroup::SetGroupProperties( const CDRNDataGroupPropertiesPtr& properties )
{GUCEF_TRACE;
    
    m_properties = properties;
}

/*-------------------------------------------------------------------------*/
    
CDRNDataGroup::CDRNDataGroupPropertiesPtr
CDRNDataGroup::GetGroupProperties( void ) const
{GUCEF_TRACE;

    return m_properties;
}

/*-------------------------------------------------------------------------*/

UInt32
CDRNDataGroup::GetItemCount( void ) const
{GUCEF_TRACE;
    
    return (UInt32) m_dataMap.size();
}

/*-------------------------------------------------------------------------*/

bool
CDRNDataGroup::GetIDAndDataAtIndex( const UInt32 index                ,
                                    const CORE::CDynamicBuffer** id   ,
                                    const CORE::CDynamicBuffer** data ) const
{GUCEF_TRACE;
    
    assert( NULL != id );
    assert( NULL != data );

    TDataMap::const_iterator i = m_dataMap.begin();
    for ( UInt32 n=0; n<index; ++n ) 
    { 
        ++i;
        if ( i == m_dataMap.end() )
        {
            return false;
        }
    };
    
    if ( *id != NULL )
    {
        *id = &(*i).first;
    }
    if ( *data != NULL )
    {
        *data = &(*i).second;
    }    
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
