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

#ifndef GUCEF_DRN_CDRNPEERLINKDATA_H
#define GUCEF_DRN_CDRNPEERLINKDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <vector>
#include <map>

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_DRN_CDRNDATAGROUP_H
#include "gucefDRN_CDRNDataGroup.h"
#define GUCEF_DRN_CDRNDATAGROUP_H
#endif /* GUCEF_DRN_CDRNDATAGROUP_H ? */

#ifndef GUCEF_DRN_CDRNDATASTREAM_H
#include "gucefDRN_CDRNDataStream.h"
#define GUCEF_DRN_CDRNDATASTREAM_H
#endif /* GUCEF_DRN_CDRNDATASTREAM_H ? */

#ifndef GUCEF_CORE_NUMERICIDS_H
#include "NumericIDs.h"
#define GUCEF_CORE_NUMERICIDS_H
#endif /* GUCEF_CORE_NUMERICIDS_H ? */

#ifndef GUCEF_DRN_MACROS_H
#include "gucefDRN_macros.h"
#define GUCEF_DRN_MACROS_H
#endif /* GUCEF_DRN_MACROS_H ? */

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

class CDRNPeerLink;

/*-------------------------------------------------------------------------*/

/**
 *  Class for managing all data related to a specific DRN link.
 */
class GUCEF_DRN_EXPORT_CPP CDRNPeerLinkData : public CORE::CObservingNotifier
{
    public:
    
    typedef CORE::CTSharedPtr< CDRNDataGroup, MT::CMutex >   TDRNDataGroupPtr;
    typedef CORE::CTSharedPtr< CDRNDataStream, MT::CMutex >  TDRNDataStreamPtr;
    typedef std::vector< TDRNDataGroupPtr >                  TDRNDataGroupList;
    typedef std::vector< TDRNDataStreamPtr >                 TDRNDataStreamList;
    
    void GetSubscribedDataGroups( TDRNDataGroupList& dataGroupList );
    
    void GetSubscribedDataStreams( TDRNDataStreamList& dataGroupList );

    TDRNDataGroupPtr GetSubscribedDataGroupWithName( const CORE::CString& name );
    
    TDRNDataStreamPtr GetSubscribedDataStreamWithName( const CORE::CString& name );

    TDRNDataGroupPtr GetSubscribedDataGroupWithID( const UInt16 dataGroupID );
    
    TDRNDataStreamPtr GetSubscribedDataStreamWithID( const UInt16 dataStreamID );

	/**
     *	Allows you to obtain the ID for the given data group
	 *	if it is subscribed. The ID is unique per link.
	 */
	bool GetSubscribedDataGroupID( const CORE::CString& name ,
		                           UInt16& id                );

	/**
     *	Allows you to obtain the ID for the given data stream
	 *	if it is subscribed. The ID is unique per link.
	 */
	bool GetSubscribedDataStreamID( const CORE::CString& name ,
		                            UInt16& id                );
        
    /**
     *  Returns a list data groups that are publicized on this link
     *  The peer connected via this link will be allowed to subscribe to these
     *  data groups
     */
    void GetPublicizedDataGroups( TDRNDataGroupList& dataGroupList );
    
    TDRNDataGroupPtr GetPublicizedDataGroupWithName( const CORE::CString& name );

	TDRNDataGroupPtr GetPublicizedDataGroupWithID( const UInt16 id );

	/**
     *	Allows you to obtain the ID for the given data group
	 *	if it is publicized. The ID is unique per link.
	 */
	bool GetPublicizedDataGroupID( const CORE::CString& name ,
		                           UInt16& id                );
    
    /**
     *  Returns a list data streams that are publicized on this link
     *  The peer connected via this link will be allowed to subscribe to these
     *  data streams
     */
    void GetPublicizedDataStreams( TDRNDataStreamList& dataGroupList );
    
    TDRNDataStreamPtr GetPublicizedDataStreamWithName( const CORE::CString& name );

	TDRNDataStreamPtr GetPublicizedDataStreamWithID( const UInt16 id );

	/**
     *	Allows you to obtain the ID for the given data stream
	 *	if it is publicized. The ID is unique per link.
	 */
	bool GetPublicizedDataStreamID( const CORE::CString& name ,
		                            UInt16& id                );

    void PublicizeStream( TDRNDataStreamPtr& dataStream );
    
    void StopStreamPublication( TDRNDataStreamPtr& dataStream );

    void StopStreamPublication( const CORE::CString& dataStreamName );
    
    void PublicizeDataGroup( TDRNDataGroupPtr& dataGroup );
    
    void StopDataGroupPublication( TDRNDataStreamPtr& dataGroup );
    
    void StopDataGroupPublication( const CORE::CString& dataGroupName );
    
    /**
     *  Returns access to the link for which this class maintains data
     */
    CDRNPeerLink& GetPeerLink( void );

    protected:
    
    /**
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined user data
     */
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );

    private:    
    friend class CDRNPeerLink;
    
    CDRNPeerLinkData( CDRNPeerLink& peerLink );
        
    virtual ~CDRNPeerLinkData();

    TDRNDataGroupPtr AddSubscribedDataGroup( const CORE::CString& dataGroupName ,
                                             const UInt16 dataGroupID           );

    TDRNDataStreamPtr AddSubscribedDataStream( const CORE::CString& dataStreamName ,
                                               const UInt16 dataStreamID           );

    void RemoveSubscribedDataGroup( const UInt16 dataGroupID );

    void RemoveSubscribedDataStream( const UInt16 dataStreamID );

    private:
    
    CDRNPeerLinkData( void );                                   /**< not implemented, makes no sense */
    CDRNPeerLinkData( const CDRNPeerLinkData& src );            /**< not implemented, makes no sense */
    CDRNPeerLinkData& operator=( const CDRNPeerLinkData& src ); /**< not implemented, makes no sense */
    
    void RemoveLinksTo( CORE::CNotifier* notifier );
    
    void OnDataGroupMutation( CORE::CNotifier* notifier    ,
                              CORE::CICloneable* eventdata ,
                              const Int32 changeType       );
    
    private:
    
    struct SDRNDataGroupEntry
    {
        TDRNDataGroupPtr ptr;
        CORE::T16BitNumericID* id;
    };
    typedef struct SDRNDataGroupEntry TDRNDataGroupEntry;
    
    struct SDRNDataStreamEntry
    {
        TDRNDataStreamPtr ptr;
        CORE::T16BitNumericID* id;
    };
    typedef struct SDRNDataStreamEntry TDRNDataStreamEntry;    
    
    typedef std::map< CORE::CString, TDRNDataGroupPtr >  TDataGroupMap;
    typedef std::map< CORE::CString, TDRNDataStreamPtr > TDataStreamMap;
    typedef std::map< UInt16, TDRNDataGroupEntry >  TDataGroupIDMap;
    typedef std::map< UInt16, TDRNDataStreamEntry > TDataStreamIDMap;
    typedef std::map< UInt16, TDRNDataGroupPtr >  TIDToDataGroupMap;
    typedef std::map< UInt16, TDRNDataStreamPtr > TIDToDataStreamMap;
        
    CORE::T16BitNumericIDGenerator m_idGenerator;
    TDataGroupMap m_subscribedDataGroups;
    TDataStreamMap m_subscribedDataStreams;
    TIDToDataGroupMap m_subscribedDataGroupsID;
    TIDToDataStreamMap m_subscribedDataStreamsID;    
    TDataGroupMap m_publicizedDataGroups;
    TDataStreamMap m_publicizedDataStreams;
    TDataGroupIDMap m_publicizedDataGroupsID;
    TDataStreamIDMap m_publicizedDataStreamsID;
    CDRNPeerLink* m_peerLink;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DRN_CDRNPEERLINKDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
