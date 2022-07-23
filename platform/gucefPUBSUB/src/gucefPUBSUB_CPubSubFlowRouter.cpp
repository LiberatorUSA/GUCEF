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
    : m_config()
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

void
CPubSubFlowRouter::ClearRoutes( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_lock );
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
                    (*n).fromSide + "\" to \"" + (*n).toSide + "\" with route type " + CPubSubFlowRouteConfig::RouteTypeToString( (*n).routeType ) );
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
                                route.fromSide + "\" to \"" + route.toSide + "\" with route type " + CPubSubFlowRouteConfig::RouteTypeToString( route.routeType ) );
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
                                route.fromSide + "\" to \"" + route.toSide + "\" with route type " + CPubSubFlowRouteConfig::RouteTypeToString( route.routeType ) );
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
                                        route.fromSide + "\" to \"" + route.toSide + "\" with route type " + CPubSubFlowRouteConfig::RouteTypeToString( route.routeType ) );
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
    
    CPubSubFlowRouterConfig::PubSubFlowRouteConfigVector::const_iterator i = normalizedConfig.routes.begin();
    while ( i != normalizedConfig.routes.end() )
    {
        const CPubSubFlowRouteConfig& routeConfig = (*i);

        // Try to find actual side objects to match against the IDs in the config
        // Note that based on how sloppy the config is we may not find any matches
        CPubSubClientSidePtr fromSide;
        CPubSubClientSidePtr toSide;
        TPubSubClientSidePtrVector::iterator n = sides.begin();
        while ( n != sides.end() )
        {
            CORE::CString sideId = (*n)->GetSideId();

            if ( routeConfig.fromSide == sideId )
                fromSide = (*n);
            else
            if ( routeConfig.toSide == sideId )
                toSide = (*n);

            ++n;
        }        

        if ( !fromSide.IsNULL() && !toSide.IsNULL() )
        {
            // We found the side combo for which we have a route config
            TRouteInfo& routeInfo = m_routeMap[ fromSide.GetPointerAlways() ];
            switch ( routeConfig.routeType )
            {
                case CPubSubFlowRouteConfig::RouteType::Primary:
                {
                    routeInfo.primaryRoutes.push_back( toSide );
                    break;
                }
                case CPubSubFlowRouteConfig::RouteType::Failover:
                {
                    routeInfo.failoverRoutes.push_back( toSide );
                    break;
                }
                case CPubSubFlowRouteConfig::RouteType::SpilloverBuffer:
                {
                    routeInfo.spilloverBufferRoutes.push_back( toSide );
                    break;
                }
                case CPubSubFlowRouteConfig::RouteType::DeadLetter:
                {
                    routeInfo.deadLetterRoutes.push_back( toSide );
                    break;
                }
                case CPubSubFlowRouteConfig::RouteType::Disabled:
                {                    
                    break;
                }
                default:
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: Unknown route type " + CPubSubFlowRouteConfig::RouteTypeToString( routeConfig.routeType ) + ". Route has no effect"  );
                    break;
                }
            }             
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: Unable to find sides referenced in route " +
                routeConfig.fromSide + " -> " + routeConfig.toSide + ". Route has no effect" );
        }

        ++i;
    }

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: Routes are now defined for " +
        CORE::ToString( m_routeMap.size() ) + " source sides" );    
    
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
                                bool isDeadLetter                                    )
{GUCEF_TRACE;
    
    MT::CScopeReaderLock lock( m_lock );
    
    TSidePtrToRouteInfoMap::iterator i = m_routeMap.find( fromSide );
    if ( i != m_routeMap.end() )
    {
        TRouteInfo& routeInfo = (*i).second;

        if ( !isDeadLetter )
        {
            // Try our primary routes first
            // This is the normal 99% (hopefully) case 
            if ( !routeInfo.primaryRoutes.empty() )
            {
                bool primaryRouteIstotalSuccess = true;
                TPubSubClientSideVector::iterator n = routeInfo.primaryRoutes.begin();
                while ( n != routeInfo.primaryRoutes.end() )
                {
                    primaryRouteIstotalSuccess = (*n)->PublishMsgs( msgs ) && primaryRouteIstotalSuccess;
                    ++n;
                }

                // If the primary route worked there is nothing more to do
                if ( primaryRouteIstotalSuccess )
                    return true;
            }

            // If we get here its time to try our failover route if we have any
            if ( !routeInfo.failoverRoutes.empty() )
            {
                bool failoverRouteIstotalSuccess = true;
                TPubSubClientSideVector::iterator n = routeInfo.failoverRoutes.begin();
                while ( n != routeInfo.failoverRoutes.end() )
                {
                    failoverRouteIstotalSuccess = (*n)->PublishMsgs( msgs ) && failoverRouteIstotalSuccess;
                    ++n;
                }

                // If the failover route worked there is nothing more to do
                if ( failoverRouteIstotalSuccess )
                    return true;
            }

            // If we get here its time to try our spillover route if we have any
            // Note that the spillover is not an equivelant of primary or failover routes, its merely a temporary detour
            if ( !routeInfo.spilloverBufferRoutes.empty() )
            {
                bool spilloverBufferRouteIstotalSuccess = true;
                TPubSubClientSideVector::iterator n = routeInfo.spilloverBufferRoutes.begin();
                while ( n != routeInfo.spilloverBufferRoutes.end() )
                {
                    spilloverBufferRouteIstotalSuccess = (*n)->PublishMsgs( msgs ) && spilloverBufferRouteIstotalSuccess;
                    ++n;
                }

                // If the spillover buffer route worked there is nothing more to do
                if ( spilloverBufferRouteIstotalSuccess )
                    return true;
            }
        }
        else
        {
            // If we get here clearly nothing else worked
            // Since the deadletter flag was passed we have exhausted all retry attempts etc
            if ( !routeInfo.deadLetterRoutes.empty() )
            {
                bool deadLetterRouteIstotalSuccess = true;
                TPubSubClientSideVector::iterator n = routeInfo.deadLetterRoutes.begin();
                while ( n != routeInfo.deadLetterRoutes.end() )
                {
                    deadLetterRouteIstotalSuccess = (*n)->PublishMsgs( msgs ) && deadLetterRouteIstotalSuccess;
                    ++n;
                }

                // If the dead letter route worked there is nothing more to do
                if ( deadLetterRouteIstotalSuccess )
                    return true;
            }
        }
    }
        
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouter::IsTrackingInFlightPublishedMsgsForAcksNeeded( CPubSubClientSide* sideWeAskFor ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_lock );
    
    TSidePtrToRouteInfoMap::const_iterator i = m_routeMap.find( sideWeAskFor );
    if ( i != m_routeMap.end() )
    {
        const TRouteInfo& routeInfo = (*i).second;
        bool trackingNeeded = IsTrackingInFlightPublishedMsgsForAcksNeeded( routeInfo.primaryRoutes ) ||
                              IsTrackingInFlightPublishedMsgsForAcksNeeded( routeInfo.failoverRoutes ) ||
                              IsTrackingInFlightPublishedMsgsForAcksNeeded( routeInfo.deadLetterRoutes ) ||
                              IsTrackingInFlightPublishedMsgsForAcksNeeded( routeInfo.spilloverBufferRoutes );
        return trackingNeeded;
    }
        
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouter::IsTrackingInFlightPublishedMsgsForAcksNeeded( const TPubSubClientSideVector& sides ) const
{GUCEF_TRACE;

    // We need tracking as soon as 1 side needs a subscriber ack
   
    TPubSubClientSideVector::const_iterator i = sides.begin();
    while ( i != sides.end() )
    {
        const CPubSubSideChannelSettings& sideSettings = (*i)->GetSideSettings();
        if ( (*i)->HasSubscribersNeedingAcks() )
            return true;
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
