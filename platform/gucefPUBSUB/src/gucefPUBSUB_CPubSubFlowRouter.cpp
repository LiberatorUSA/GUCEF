/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

#include "gucefPUBSUB_CPubSubFlowRouter.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubFlowRouter::CPubSubFlowRouter( void )
    : CORE::CObservingNotifier()
    , m_config()
    , m_routeMap()
    , m_lock( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouter::~CPubSubFlowRouter()
{GUCEF_TRACE;

    ClearRoutes();
}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouter::CRouteInfo::CRouteInfo( void )
    : toSide( GUCEF_NULL )
    , toSideIsHealthy( true )
    , failoverSide( GUCEF_NULL )
    , failoverSideIsHealthy( true )
    , spilloverBufferSide( GUCEF_NULL )
    , spilloverBufferSideIsHealthy( true )
    , deadLetterSide( GUCEF_NULL )
    , deadLetterSideIsHealthy( true )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::ClearRoutes( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_lock );

    // Unsubscribe the event handlers for each side we have routes for
    TSidePtrToRouteInfoVectorMap::iterator r = m_routeMap.begin();
    while ( r != m_routeMap.end() )
    {
        UnsubscribeFrom( (*r).first );
        ++r;
    }

    m_routeMap.clear();
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::NormalizeConfig( const CPubSubFlowRouterConfig& originalConfig ,
                                    TPubSubClientSidePtrVector& sides             ,
                                    CPubSubFlowRouterConfig& normalizedConfig     )
{GUCEF_TRACE;

    normalizedConfig.ackStyle = originalConfig.ackStyle;
    
    // First we gather all the 'known' sides
    // If the config says we map to or from anything '*' it still needs to map to an actual side
    
    CORE::CString::StringSet knownSides;

    TPubSubClientSidePtrVector::const_iterator i = sides.begin();
    while ( i != sides.end() )
    {
        knownSides.insert( (*i)->GetSideId() );       
        ++i;
    }

    CPubSubFlowRouterConfig::PubSubFlowRouteConfigVector::const_iterator n = originalConfig.routes.begin();
    while ( n != originalConfig.routes.end() )
    {
        if ( "*" != (*n).toSide )
            knownSides.insert( (*n).toSide );
        if ( "*" != (*n).fromSide )
            knownSides.insert( (*n).fromSide );
        ++n;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter:NormalizeConfig: There are " + CORE::ToString( knownSides.size() ) + " known sides" );

    // Now create a map of explicit routes
    // These override any wildcard routes and as such we wont auto generate routes for those
    CORE::CValueList explicitRoutes;
    explicitRoutes.SetAllowMultipleValues( true );
    explicitRoutes.SetAllowDuplicates( false );
    n = originalConfig.routes.begin();
    while ( n != originalConfig.routes.end() )
    {
        if ( "*" != (*n).fromSide &&  "*" != (*n).toSide )
        {
            explicitRoutes.Set( (*n).fromSide, (*n).toSide );
            
            // Nothing to normalize, just copy to the normalized output
            normalizedConfig.routes.push_back( (*n) );

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter:NormalizeConfig: We have an explicit route from \"" + 
                    (*n).fromSide + "\" to \"" + (*n).toSide + "\" with failover side \"" + (*n).failoverSide + "\" and spillover side \"" +
                    (*n).spilloverBufferSide + "\" and deadletter side \"" + (*n).deadLetterSide + "\"" );
        }
        ++n;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter:NormalizeConfig: There are " + CORE::ToString( explicitRoutes.GetValueCount() ) + " explicit routes configured" );
    
    // Now that we know about the IDs of all known sides we can populate the '*' as needed for the wildcard side
    // Note that we intentionally dont handle the * -> * case yet since that is the catch-all applied at the end
    n = originalConfig.routes.begin();
    while ( n != originalConfig.routes.end() )
    {
        if ( "*" == (*n).toSide && "*" != (*n).fromSide )
        {
             // Try to generate routes using all known sides
             CORE::CString::StringSet::iterator m = knownSides.begin();
             while ( m != knownSides.end() )
             {
                // Don't generate a route where the 'from' and 'to' are the same side
                if ( (*n).fromSide != (*m) )
                {
                    // Don't generate a route if an explicit route was specified
                    if ( !explicitRoutes.HasKeyAndValue( (*n).fromSide, (*m) ) )
                    {
                        CPubSubFlowRouteConfig route( (*n) );
                        route.toSide = (*m);
                        normalizedConfig.routes.push_back( route );

                        explicitRoutes.Set( route.fromSide, route.toSide );

                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter:NormalizeConfig: Generated route from \"" + 
                                route.fromSide + "\" to \"" + route.toSide + "\" with failover side \"" + route.failoverSide + "\" and spillover side \"" +
                                route.spilloverBufferSide + "\" and deadletter side \"" + route.deadLetterSide + "\"" );
                    }                
                }
                ++m;
             }
        }
        else
        if ( "*" == (*n).fromSide && "*" != (*n).toSide )
        {
             // Try to generate routes using all known sides
             CORE::CString::StringSet::iterator m = knownSides.begin();
             while ( m != knownSides.end() )
             {
                // Don't generate a route where the 'from' and 'to' are the same side
                if ( (*n).toSide != (*m) )
                {
                    // Don't generate a route if an explicit route was specified
                    if ( !explicitRoutes.HasKeyAndValue( (*n).toSide, (*m) ) )
                    {
                        CPubSubFlowRouteConfig route( (*n) );
                        route.fromSide = (*m);
                        normalizedConfig.routes.push_back( route );

                        explicitRoutes.Set( route.fromSide, route.toSide );

                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter:NormalizeConfig: Generated route from \"" + 
                                route.fromSide + "\" to \"" + route.toSide + "\" with failover side \"" + route.failoverSide + "\" and spillover side \"" +
                                route.spilloverBufferSide + "\" and deadletter side \"" + route.deadLetterSide + "\"" );
                    }                
                }
                ++m;
             }
        }

        ++n;
    }

    // Now handle the catch-all *->* route use-case
    bool foundCatchAll = false;
    n = originalConfig.routes.begin();
    while ( n != originalConfig.routes.end() )
    {
        if ( "*" == (*n).fromSide &&  "*" == (*n).toSide )
        {
            if ( !foundCatchAll )
            {
                foundCatchAll = true;
                
                // Generate a combo for anything that is not yet defined as an explicit route
                CORE::CString::StringSet::iterator m = knownSides.begin();
                while ( m != knownSides.end() )
                {
                    CORE::CString::StringSet::iterator k = knownSides.begin();
                    while ( k != knownSides.end() )
                    {
                        // Don't generate a route where the 'from' and 'to' are the same side
                        if ( (*m) != (*k) )
                        {
                            // Don't generate a route if an explicit route was specified
                            if ( !explicitRoutes.HasKeyAndValue( (*m), (*k) ) )
                            {
                                CPubSubFlowRouteConfig route( (*n) );
                                route.fromSide = (*m);
                                route.toSide = (*k);
                                normalizedConfig.routes.push_back( route );

                                explicitRoutes.Set( route.fromSide, route.toSide );

                                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter:NormalizeConfig: Generated catch-all route from \"" + 
                                        route.fromSide + "\" to \"" + route.toSide + "\" with failover side \"" + route.failoverSide + "\" and spillover side \"" +
                                        route.spilloverBufferSide + "\" and deadletter side \"" + route.deadLetterSide + "\"" );
                            }
                        }
                        ++k;
                    }
                    ++m;
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:NormalizeConfig: Found another catch-all *->* route in the configuration. This is not allowed. The first found catch-all will apply and the rest is ignored" );
            }
        }
        ++n;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter:NormalizeConfig: There are now " + CORE::ToString( normalizedConfig.routes.size() ) + " explicit routes configured" );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::BuildRoutes( const CPubSubFlowRouterConfig& config ,
                                TPubSubClientSidePtrVector& sides     )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_lock );
    
    // We might get called multiple times so wipe whatever we had first
    ClearRoutes();
    
    // Since we support wildcard based config to reduce config burden we need to normalize the config first
    CPubSubFlowRouterConfig normalizedConfig;
    if ( !NormalizeConfig( config, sides, normalizedConfig ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: Failed to normalize config" );
        return false;
    }
    
    // Now we generate the pointer map based on the normalized config
    UInt32 totalPossibleRoutes = 0;
    CPubSubFlowRouterConfig::PubSubFlowRouteConfigVector::const_iterator i = normalizedConfig.routes.begin();
    while ( i != normalizedConfig.routes.end() )
    {
        const CPubSubFlowRouteConfig& routeConfig = (*i);

        // Try to find actual side objects to match against the IDs in the config
        // Note that based on how sloppy the config is we may not find any matches
        UInt32 possibleRoutes = 0;
        CPubSubClientSidePtr fromSide;
        CPubSubClientSidePtr toSide;
        CPubSubClientSidePtr failoverSide;
        CPubSubClientSidePtr spilloverSide;
        CPubSubClientSidePtr deadletterSide;
        TPubSubClientSidePtrVector::iterator n = sides.begin();
        while ( n != sides.end() )
        {
            CORE::CString sideId = (*n)->GetSideId();
            if ( !sideId.IsNULLOrEmpty() )
            {
                if ( routeConfig.fromSide == sideId )
                    fromSide = (*n);
                else
                if ( routeConfig.toSide == sideId )
                {
                    toSide = (*n);
                    ++possibleRoutes;
                }
                else
                if ( routeConfig.failoverSide == sideId )
                {
                    failoverSide = (*n);
                    ++possibleRoutes;
                }
                else
                if ( routeConfig.spilloverBufferSide == sideId )
                {
                    spilloverSide = (*n);
                    ++possibleRoutes;
                }
                else
                if ( routeConfig.deadLetterSide == sideId )
                {
                    deadletterSide = (*n);
                    ++possibleRoutes;
                }
            }
            ++n;
        }        

        // The minimum you need is from and to sides, the rest is optional
        if ( !fromSide.IsNULL() && !toSide.IsNULL() )
        {
            // We found the side combo for which we have a route config
            TRouteInfoVector& multiRouteInfo = m_routeMap[ fromSide.GetPointerAlways() ]; 
            multiRouteInfo.push_back( CRouteInfo() );
            CRouteInfo& routeInfo = multiRouteInfo.back();
            routeInfo.toSide = toSide.GetPointerAlways();
            routeInfo.failoverSide = failoverSide.GetPointerAlways();
            routeInfo.spilloverBufferSide = spilloverSide.GetPointerAlways();
            routeInfo.deadLetterSide = deadletterSide.GetPointerAlways();           

            totalPossibleRoutes += possibleRoutes;
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: Unable to find sides referenced in route " +
                routeConfig.fromSide + " -> " + routeConfig.toSide + ". Route has no effect" );
        }

        ++i;
    }

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: Routes are now defined for " +
        CORE::ToString( m_routeMap.size() ) + " source sides, with a total of " + CORE::ToString( totalPossibleRoutes ) + " possible routes" );    
    
    // Now set up all the event handlers for each side
    TSidePtrToRouteInfoVectorMap::iterator r = m_routeMap.begin();
    while ( r != m_routeMap.end() )
    {
        RegisterSideEventHandlers( (*r).first );
        ++r;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::SaveConfig( CPubSubFlowRouterConfig& cfg ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_lock );
    cfg = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::LoadConfig( const CPubSubFlowRouterConfig& cfg )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_lock );
    m_config = cfg;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::AcknowledgeReceiptForSide( CPubSubClientSide* msgReceiverSide ,
                                              CIPubSubMsg::TNoLockSharedPtr& msg )
{GUCEF_TRACE;

    CORE::UInt32 invokerThreadId = MT::GetCurrentTaskID();

    // Right now we dont use the route info but instead use the source location
    // we can revisit this later but it is fragile due to reliance on GetOpaqueUserData()
    // Due to this we right now dont need a lock here

    // Note that acks are always ONLY to the side that originated the message since that side is the only
    // one that knows about that specific message and whatever conventions its abstracting wrt the underlying message format

    CPubSubClientTopic* originTopic = msg->GetOriginClientTopic();
    if ( GUCEF_NULL != originTopic )
    {
        CPubSubClient* originClient = originTopic->GetClient();
        if ( GUCEF_NULL != originClient )
        {
            CPubSubClientSide* originSide = static_cast< CPubSubClientSide* >( originClient->GetOpaqueUserData() );
            if ( GUCEF_NULL != originSide )
            {
                if ( invokerThreadId != originSide->GetDelegatorThreadId() )
                    return originSide->AcknowledgeReceiptASync( msg );
                else
                    return originSide->AcknowledgeReceiptSync( msg );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                    "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since parent client owning the origin topic provided on the message does not have a link back to the owner pubsub side. This should never happen." );
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since no parent client owns the origin topic provided on the message. This should never happen." );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
            "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since no origin topic was provided on the message. Check your config and backend feature compatibility" );
        return false;
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouter::PublishMsgs( CPubSubClientSide* fromSide                          , 
                                const CPubSubClientTopic::TPubSubMsgsRefVector& msgs ,
                                RouteType routeType                                  )
{GUCEF_TRACE;
    
    MT::CScopeReaderLock lock( m_lock );
    
    bool publishIstotalSuccess = true;
    TSidePtrToRouteInfoVectorMap::iterator i = m_routeMap.find( fromSide );
    if ( i != m_routeMap.end() )
    {
        TRouteInfoVector& multiRouteInfo = (*i).second; 
        switch ( routeType )
        {            
            case RouteType::Primary:
            {                
                TRouteInfoVector::iterator n = multiRouteInfo.begin();
                while ( n != multiRouteInfo.end() )
                {            
                    CRouteInfo& routeInfo = (*n);
                    bool publishSuccess = true;

                    // Try our primary 'to' route first
                    // This is the normal 99% (hopefully) case
                    if ( GUCEF_NULL != routeInfo.toSide && routeInfo.toSideIsHealthy )
                    {                    
                        if ( !routeInfo.toSide->PublishMsgs( msgs ) )
                        {
                            // Primary 'to' route failed
                            // Try the failover route if one is available
                            if ( GUCEF_NULL != routeInfo.failoverSide && routeInfo.failoverSideIsHealthy )
                                publishSuccess = routeInfo.failoverSide->PublishMsgs( msgs );
                            else
                                publishSuccess = false;
                        }
                    }   
                    else
                    {
                        // Primary 'to' route is not available
                        // Try the failover route if one is available
                        if ( GUCEF_NULL != routeInfo.failoverSide && routeInfo.failoverSideIsHealthy )
                            publishSuccess = routeInfo.failoverSide->PublishMsgs( msgs );
                        else
                            publishSuccess = false;
                    }                

                    if ( !publishSuccess )
                    {
                        // It seems the primary and failover routes have failed us
                        // If there is a spillover available we can try using that route as a different type of failover
                        if ( GUCEF_NULL != routeInfo.spilloverBufferSide && routeInfo.spilloverBufferSideIsHealthy )
                            publishSuccess = routeInfo.spilloverBufferSide->PublishMsgs( msgs );
                        else
                            publishSuccess = false;
                    }

                    publishIstotalSuccess = publishSuccess && publishIstotalSuccess;
                    ++n;
                }
                break;
            }
            case RouteType::Failover:
            {
                TRouteInfoVector::iterator n = multiRouteInfo.begin();
                while ( n != multiRouteInfo.end() )
                { 
                    CRouteInfo& routeInfo = (*n);
                    bool publishSuccess = true;
                    
                    // Try the failover route if one is available
                    if ( GUCEF_NULL != routeInfo.failoverSide && routeInfo.failoverSideIsHealthy )
                        publishSuccess = routeInfo.failoverSide->PublishMsgs( msgs );
                    else
                        publishSuccess = false;

                    if ( !publishSuccess )
                    {
                        // It seems the failover route has failed us
                        // If there is a spillover available we can try using that route as a different type of failover
                        if ( GUCEF_NULL != routeInfo.spilloverBufferSide && routeInfo.spilloverBufferSideIsHealthy )
                            publishSuccess = routeInfo.spilloverBufferSide->PublishMsgs( msgs );
                        else
                            publishSuccess = false;
                    } 

                    publishIstotalSuccess = publishSuccess && publishIstotalSuccess;
                    ++n;
                }                    
                break;
            }
            case RouteType::SpilloverBuffer:
            {
                bool spilloverRouteIstotalSuccess = true;
                TRouteInfoVector::iterator n = multiRouteInfo.begin();
                while ( n != multiRouteInfo.end() )
                { 
                    CRouteInfo& routeInfo = (*n);                
                    if ( GUCEF_NULL != routeInfo.spilloverBufferSide )
                    {
                        if ( routeInfo.spilloverBufferSideIsHealthy )
                            spilloverRouteIstotalSuccess = routeInfo.spilloverBufferSide->PublishMsgs( msgs ) && spilloverRouteIstotalSuccess;
                        else
                            spilloverRouteIstotalSuccess = false;    
                    }                
                    ++n;
                }
                publishIstotalSuccess = spilloverRouteIstotalSuccess; 
                break;
            }
            case RouteType::DeadLetter:
            {
                // If we get here clearly nothing else worked
                // Since the deadletter flag was passed we have exhausted all retry attempts etc
                bool deadLetterRouteIstotalSuccess = true;
                TRouteInfoVector::iterator n = multiRouteInfo.begin();
                while ( n != multiRouteInfo.end() )
                { 
                    CRouteInfo& routeInfo = (*n);                
                    if ( GUCEF_NULL != routeInfo.deadLetterSide )
                    {
                        if ( routeInfo.deadLetterSideIsHealthy )
                            deadLetterRouteIstotalSuccess = routeInfo.deadLetterSide->PublishMsgs( msgs ) && deadLetterRouteIstotalSuccess;
                        else
                            deadLetterRouteIstotalSuccess = false;    
                    }                
                    ++n;
                }
                publishIstotalSuccess = deadLetterRouteIstotalSuccess;                
                break;
            }
        }
    }
    else
    {
        // no route from this side
        publishIstotalSuccess = false;
    }
        
    return publishIstotalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouter::IsTrackingInFlightPublishedMsgsForAcksNeeded( CPubSubClientSide* sideWeAskFor ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_lock );
    
    TSidePtrToRouteInfoVectorMap::const_iterator i = m_routeMap.find( sideWeAskFor );
    if ( i != m_routeMap.end() )
    {
        const TRouteInfoVector& multiRouteInfo = (*i).second;
        TRouteInfoVector::const_iterator n = multiRouteInfo.begin();
        while ( n != multiRouteInfo.end() )
        {
            const CRouteInfo& routeInfo = (*n);
        
            // We need tracking as soon as 1 side anywhere needs a subscriber ack
            bool trackingNeeded = IsTrackingInFlightPublishedMsgsForAcksNeeded( routeInfo.toSide ) ||
                                  IsTrackingInFlightPublishedMsgsForAcksNeeded( routeInfo.failoverSide ) ||
                                  IsTrackingInFlightPublishedMsgsForAcksNeeded( routeInfo.deadLetterSide ) ||
                                  IsTrackingInFlightPublishedMsgsForAcksNeeded( routeInfo.spilloverBufferSide );
            
            if ( trackingNeeded )
                return trackingNeeded;
            ++n;
        }
    }
        
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouter::IsTrackingInFlightPublishedMsgsForAcksNeeded( const CPubSubClientSide* side ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != side )
    {
        const CPubSubSideChannelSettings& sideSettings = side->GetSideSettings();
        if ( side->HasSubscribersNeedingAcks() )
            return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void 
CPubSubFlowRouter::RegisterSideEventHandlers( CPubSubClientSide* side )
{GUCEF_TRACE;

    if ( GUCEF_NULL != side ) 
    {
        TEventCallback callback( this, &CPubSubFlowRouter::OnSideHealthStatusChange );
        SubscribeTo( side                                       ,
                     CPubSubClientSide::HealthStatusChangeEvent ,
                     callback                                   );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::OnSideHealthStatusChange( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
