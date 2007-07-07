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

#include <vector>

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"          /* GUCEF CORE library API */
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_DRN_H
#include "gucefDRN.h"           /* GUCEF DRN library API */
#define GUCEF_DRN_H
#endif /* GUCEF_DRN_H ? */

#ifdef GUCEF_MSWIN_BUILD
  #include <windows.h>
  #undef min
  #undef max
#else
  #include <assert.h>
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define DRN_PEERCOMM_PORT   50456

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MSWIN_BUILD
  #define DebugBreak() assert( 0 )
#endif

/*-------------------------------------------------------------------------*/

#define ERRORHERE { GUCEF_ERROR_LOG( 0, GUCEF::CORE::CString( "Test failed @ " ) + GUCEF::CORE::CString( __FILE__ ) + ':' + GUCEF::CORE::Int32ToString( __LINE__ ) ); \
                    DebugBreak();                                                                                                                                     \
                  }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CTestPeerValidator : public DRN::CIDRNPeerValidator
{
    public:

    virtual bool IsPeerAddressValid( const CIPAddress& address     ,
                                     const CORE::CString& hostName ) const
    {GUCEF_TRACE;
    
        GUCEF_LOG( 0, "Question received: is peer address valid" );
        return ( hostName == "localhost" ) || ( hostName == "127.0.0.1" );
    }
    
    virtual bool IsPeerLoginRequired( const DRN::CDRNPeerLink& peerLink ) const
    {GUCEF_TRACE;
    
        GUCEF_LOG( 0, "Question received: is peer login required" );
        return true;
    }
    
    virtual bool IsPeerLoginValid( const DRN::CDRNPeerLink& peerLink ,
                                   const CORE::CString& accountName  ,
                                   const CORE::CString& password     ) const
    {GUCEF_TRACE;
    
        GUCEF_LOG( 0, "Question received: is peer login valid" );
        return  ( ( accountName == "ValidPeer" ) && ( password == "Password" ) );
    }

    virtual bool IsPeerServiceValid( const CORE::CString& serviceName     ,
                                     const CORE::TVersion& serviceVersion ) const
    {GUCEF_TRACE;
    
        if ( serviceName == "TestPeerToPeer" )
        {
            CORE::TVersion correctVersion;
            correctVersion.mayor = 0;
            correctVersion.minor = 1;
            correctVersion.patch = 0;
            correctVersion.release = 0;
            
            return ( memcmp( &serviceVersion, &correctVersion, sizeof( CORE::TVersion ) ) == 0 );
        }        
        return false;
    }    

    virtual bool IsPeerDataStreamSubscriptionAllowed( const DRN::CDRNPeerLink& peerLink   ,
                                                      const CORE::CString& dataStreamName ) const
    {GUCEF_TRACE;
    
        return true;
    }

    virtual bool IsPeerDataGroupSubscriptionAllowed( const DRN::CDRNPeerLink& peerLink  ,
                                                     const CORE::CString& dataGroupName ) const
    {GUCEF_TRACE;
    
        return true;
    }

    virtual bool IsPeerServiceCompatible( const DRN::CDRNPeerLink& peerLink    ,
                                          const CORE::CString& peerServiceName ) const
    {GUCEF_TRACE;
    
        return peerServiceName == "ValidService";
    }
                                                      
    virtual CORE::CString GetServiceName( const DRN::CDRNPeerLink& peerLink ) const
    {GUCEF_TRACE;
    
        return "ValidService";
    }
    
    virtual bool GetLoginForPeer( const DRN::CDRNPeerLink& peerLink ,
                                  CORE::CString& ourAccountName     ,
                                  CORE::CString& ourPassword        )
    {GUCEF_TRACE;
    
        GUCEF_LOG( 0, "Question received: What is the login for the given peer" );
        
        ourAccountName = "ValidPeer";
        ourPassword = "Password";
        return true;
    }
};

/*-------------------------------------------------------------------------*/

class CTestPeerToPeerSubSystem : public CORE::CGUCEFAppSubSystem
{
    private:

    CTestPeerValidator testValidator;        
    DRN::CDRNNode nodeA;
    DRN::CDRNNode nodeB; 
    DRN::CDRNNode::CDRNPeerLinkPtr m_linkA;
    DRN::CDRNNode::CDRNPeerLinkPtr m_linkB;
    DRN::CDRNPeerLinkData::TDRNDataStreamPtr m_streamA;
    DRN::CDRNPeerLinkData::TDRNDataStreamPtr m_streamB;
    DRN::CDRNPeerLinkData::TDRNDataGroupPtr  m_dataGroupA;
    DRN::CDRNPeerLinkData::TDRNDataGroupPtr  m_dataGroupB;
    DRN::CDRNDataGroup::CDRNDataGroupPropertiesPtr m_groupProperties;
    CORE::TStreamerStringString m_streamerA;
    CORE::TStreamerStringString m_streamerB;
    
    public:
    
    CTestPeerToPeerSubSystem( void )    
        : CGUCEFAppSubSystem( true ) ,
          m_streamA( new DRN::CDRNDataStream( "TestStream" ) )      ,
          m_streamB( new DRN::CDRNDataStream( "TestStream" ) )      ,
          m_dataGroupA( new DRN::CDRNDataGroup( "TestDataGroup" ) ) ,
          m_dataGroupB( new DRN::CDRNDataGroup( "TestDataGroup" ) ) ,
          m_groupProperties( new DRN::CDRNDataGroupProperties() )
    {GUCEF_TRACE;
    
        m_groupProperties->SetAcceptNewPeerItems( true );
        m_groupProperties->SetAcceptNewStreamerItems( true );
        m_groupProperties->SetAcceptUpdatesFromStreamers( true );
        m_groupProperties->SetAllowUnreliableTransmission( false );
        m_groupProperties->SetEmitEntireGroupOnChange( false );
        
        m_dataGroupA->SetGroupProperties( m_groupProperties );
        m_dataGroupB->SetGroupProperties( m_groupProperties );
        
        m_streamerA.SetID( "StreamerA" );
        m_streamerB.SetID( "StreamerB" );
        m_streamerA.SetValue( "InitialStringA" );
        m_streamerB.SetValue( "InitialStringB" );
        
        m_dataGroupA->SubscribeTo( &m_streamerA );
        m_dataGroupB->SubscribeTo( &m_streamerB );
    }
    
    void SetupTestUtils( void )
    {GUCEF_TRACE;
    
        GUCEF_LOG( 0, "Setting Node Peer validators" );
        
        nodeA.SetPeerValidator( &testValidator );
        nodeB.SetPeerValidator( &testValidator );
        
        SubscribeTo( &nodeA );
        SubscribeTo( &nodeB );
        
        if ( nodeA.GetPeervalidator() != &testValidator )
        {
            GUCEF_ERROR_LOG( 0, "Setting Node Peer validators failed" );
            ERRORHERE;
        }
        
        if ( nodeB.GetPeervalidator() != &testValidator )
        {
            GUCEF_ERROR_LOG( 0, "Setting Node Peer validators failed" );
            ERRORHERE;
        }

        GUCEF_LOG( 0, "Attempting to listen on port " + GUCEF::CORE::Int32ToString( DRN_PEERCOMM_PORT ) );
        if ( !nodeA.ListenOnPort( DRN_PEERCOMM_PORT ) )
        {
            // the listen actions should succeed for our tests to work
            GUCEF_ERROR_LOG( 0, "Failed to open listening socket on port " + GUCEF::CORE::Int32ToString( DRN_PEERCOMM_PORT ) );
            ERRORHERE;
        }
        GUCEF_LOG( 0, "Successfully opened listen port" );
        
        GUCEF_LOG( 0, "Attempting to connect to node on port " + GUCEF::CORE::Int32ToString( DRN_PEERCOMM_PORT ) );
        if ( !nodeB.ConnectToPeer( "127.0.0.1", DRN_PEERCOMM_PORT ) )
        {
            // since we are connecting localhost this should always work
            GUCEF_ERROR_LOG( 0, "Failed to connect to peer node on port " + GUCEF::CORE::Int32ToString( DRN_PEERCOMM_PORT ) );
            ERRORHERE;
        }
        GUCEF_LOG( 0, "Successfully connected to a listen port" );
    }

    CORE::CString GetNodeName( CORE::CNotifier* notifier )
    {GUCEF_TRACE;

        if ( notifier == &nodeA )
        {
            return "Node A";
        }
        if ( notifier == &nodeB )
        {
            return "Node B";
        }
        return "Unknown Source";              
    }
    
    CORE::CString GetLinkNodeName( CORE::CNotifier* notifier )
    {GUCEF_TRACE;
    
        DRN::CDRNPeerLink* link = static_cast< DRN::CDRNPeerLink* >( notifier );
        if ( &link->GetParentNode() == &nodeA )
        {
            return "Node A";
        }
        if ( &link->GetParentNode() == &nodeB )
        {
            return "Node B";
        }
        return "Unknown Source";              
    }

    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL )
    {GUCEF_TRACE;
    
        if ( CORE::CGUCEFApplication::AppInitEvent == eventid )
        {
            SetupTestUtils();
        }
        else
        if ( DRN::CDRNNode::LinkEstablishedEvent == eventid )
        {
            GUCEF_LOG( 0, GetNodeName( notifier ) + ": Link established" );
            
            DRN::CDRNNode::LinkEstablishedEventData* eData = static_cast< DRN::CDRNNode::LinkEstablishedEventData* >( eventdata );
            
            if ( notifier == &nodeA )
            {
                m_linkA = eData->GetData();            
                SubscribeTo( &(*m_linkA) );                
                m_linkA->GetLinkData().PublicizeStream( m_streamA );
                m_linkA->GetLinkData().PublicizeDataGroup( m_dataGroupA );
            }
            else
            if ( notifier == &nodeB )
            {
                m_linkB = eData->GetData();            
                SubscribeTo( &(*m_linkB) );
                m_linkB->GetLinkData().PublicizeStream( m_streamB );
                m_linkB->GetLinkData().PublicizeDataGroup( m_dataGroupB );
            }            
        }
        else
        if ( DRN::CDRNPeerLink::ConnectedEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": Link connected" );
        }
        else
        if ( DRN::CDRNNode::LinkErrorEvent == eventid )
        {
            // We should not get here with our test app
            GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Link error" );
            ERRORHERE;            
        }
        else
        if ( DRN::CDRNPeerLink::LinkCorruptionEvent == eventid )
        {
            // We should not get here with our test app
            GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Link corruption" );
            ERRORHERE;            
        }        
        else
        if ( DRN::CDRNPeerLink::LinkProtocolMismatchEvent == eventid )
        {
            // We should not get here with our test app
            GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Link protocol mismatch" );
            ERRORHERE;            
        }
        else
        if ( DRN::CDRNPeerLink::LinkIncompatibleEvent == eventid )
        {
            // We should not get here with our test app
            GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Link incompatible" );
            ERRORHERE;
        }
        else
        if ( DRN::CDRNPeerLink::AuthenticationSuccessEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": We successfully authenticated at the peer" );
        }
        else
        if ( DRN::CDRNPeerLink::PeerAuthenticationSuccessEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": A peer successfully authenticated with our node" );
        }        
        else
        if ( DRN::CDRNPeerLink::PeerAuthenticationFailureEvent == eventid )
        {
            GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": We failed to authenticate at the peer" );
            ERRORHERE;
        }
        else
        if ( DRN::CDRNPeerLink::PeerAuthenticationFailureEvent == eventid )
        {
            GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": A peer failed to authenticate with our node" );
            ERRORHERE;
        }        
        else        
        if ( DRN::CDRNPeerLink::CompatibleServiceEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": The peer is hosting a compatible service" );
        }
        else
        if ( DRN::CDRNPeerLink::IllegalRequestEvent == eventid )
        {
            GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": The peer claims we made an illegal request" );
            ERRORHERE;            
        }
        else
        if ( DRN::CDRNPeerLink::IncompatibleServiceEvent == eventid )
        {
            GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": The peer is hosting a Incompatible service" );
            ERRORHERE;
        }        
        else
        if ( DRN::CDRNPeerLink::LinkProtocolMatchEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": The peer is using a compatible version of the DRN protocol" );
        }        
        else
        if ( DRN::CDRNPeerLink::LinkOperationalForPeerEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": The link is now operational for the peer" );
        }
        else        
        if ( DRN::CDRNPeerLink::LinkOperationalForUsEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": The link is now operational for us" );
            
            DRN::CDRNNode::CDRNPeerLinkPtr link = notifier == m_linkA ? m_linkA : m_linkB;

            if ( !link->RequestDataGroupList() )
            {
                // We should not get here with our test app
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to request the peer data group list" );
                ERRORHERE;                
            }            
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": Requested data group list from peer" );

            if ( !link->RequestStreamList() )
            {
                // We should not get here with our test app
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to request the peer data stream list" );
                ERRORHERE;                
            }
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": Requested data stream list from peer" );

            if ( !link->RequestPeerList() )
            {
                // We should not get here with our test app
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to request the peer's peer list" );
                ERRORHERE;                
            }
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": Requested peer list from peer" );
        }        
        else
        if ( DRN::CDRNPeerLink::PeerListReceivedFromPeerEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": received peer list from peer" );
        }
        else
        if ( DRN::CDRNPeerLink::StreamListReceivedFromPeerEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": received data stream list from peer" );
            
            DRN::CDRNNode::CDRNPeerLinkPtr link = notifier == m_linkA ? m_linkA : m_linkB;
            
            if ( !link->RequestStreamSubscription( "TestStream" ) )
            {
                // We should not get here with our test app
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to request stream subscription" );
                ERRORHERE;  
            }
        }
        else
        if ( DRN::CDRNPeerLink::DataGroupListReceivedFromPeerEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": received data group list from peer" );
            
            DRN::CDRNNode::CDRNPeerLinkPtr link = notifier == m_linkA ? m_linkA : m_linkB;
            
            if ( !link->RequestDataGroupSubscription( "TestDataGroup" ) )
            {
                // We should not get here with our test app
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to request data group subscription" );
                ERRORHERE;  
            }            
        }                
        else
        if ( DRN::CDRNPeerLink::SubscribedToDataStreamEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": Successfully subscribed to peer data stream" );
            
            DRN::CDRNPeerLink::SubscribedToDataStreamEventData* eData = static_cast< DRN::CDRNPeerLink::SubscribedToDataStreamEventData* >( eventdata );
            if ( NULL == eData )
            {
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to obtain name of the subscribed data stream" );
                ERRORHERE; 
            }
            
            CORE::CString streamName = eData->GetData();
            if ( streamName.Length() == 0 )
            {
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to obtain a proper name for the subscribed data stream" );
                ERRORHERE;             
            }
            
            DRN::CDRNNode::CDRNPeerLinkPtr link = notifier == m_linkA ? m_linkA : m_linkB;
            DRN::CDRNPeerLinkData::TDRNDataStreamPtr stream = link->GetLinkData().GetSubscribedDataStreamWithName( streamName );
            
            if ( NULL == stream )
            {
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to obtain the subscribed data stream" );
                ERRORHERE;                 
            }
            
            // Now we subscribe this test object to the subscribed stream 
            SubscribeTo( stream.GetPointer() );            
            
            // Send some data using the stream
            stream->SendData( "TESTDATA", 8 );
        }        
        else
        if ( DRN::CDRNPeerLink::SubscribedToDataGroupEvent == eventid )
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": Successfully subscribed to peer data group" );   
            
            DRN::CDRNPeerLink::SubscribedToDataGroupEventData* eData = static_cast< DRN::CDRNPeerLink::SubscribedToDataGroupEventData* >( eventdata );
            if ( NULL == eData )
            {
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to obtain name of the subscribed data group" );
                ERRORHERE; 
            }
            
            CORE::CString groupName = eData->GetData();
            if ( groupName.Length() == 0 )
            {
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to obtain a proper name for the subscribed data group" );
                ERRORHERE;             
            }
            
            DRN::CDRNNode::CDRNPeerLinkPtr link = notifier == m_linkA ? m_linkA : m_linkB;
            DRN::CDRNPeerLinkData::TDRNDataGroupPtr dataGroup = link->GetLinkData().GetSubscribedDataGroupWithName( groupName );
            
            if ( NULL == dataGroup )
            {
                GUCEF_ERROR_LOG( 0, GetLinkNodeName( notifier ) + ": Failed to obtain the subscribed data group" );
                ERRORHERE;                 
            }
            
            // Now we subscribe this test object to the subscribed stream 
            SubscribeTo( dataGroup.GetPointer() );                    
            
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": Changing value of a streamer" ); 
            
            CORE::TStreamerStringString& streamer = notifier == m_linkA ? m_streamerA : m_streamerB;
            streamer.SetValue( "ValueChange1" );
        }
        else   
        if ( DRN::CDRNDataStream::DataTransmittedEvent == eventid )
        {
            GUCEF_LOG( 0, "Data was written to a stream" );
        }
        else         
        if ( DRN::CDRNPeerLink::DisconnectedEvent == eventid )
        {
            GUCEF_LOG( 0, GetNodeName( notifier ) + ": The link has been disconnected, shutting down" );
            CORE::CGUCEFApplication::Instance()->Stop();
        }
        else          
        if ( CORE::CNotifier::SubscribeEvent == eventid )
        {
            // Don't do anything,.. ignore
        }        
        else
        {
            GUCEF_LOG( 0, GetLinkNodeName( notifier ) + ": Unhandled event: " + eventid.GetName() );
        }        
    }      
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformPeerToPeerTest( void )
{GUCEF_TRACE;
  
    try
    {
        GUCEF_LOG( 0, "*** Commencing gucefDRN Peer to peer test ***" );
        
        CTestPeerToPeerSubSystem testSubSystem;
        CORE::CGUCEFApplication::Instance()->main( 0, NULL, true );
    }
    catch ( ... )
    {
        GUCEF_ERROR_LOG( 1, "unhandled exception during PerformPeerToPeerTest()" );
    } 
    
    GUCEF_LOG( 0, "*** Completed gucefDRN Peer to peer test ***" );
}

/*-------------------------------------------------------------------------*/
