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
    , m_usedInRouteMap()
    , m_spilloverInfoMap()
    , m_spilloverInfoForTargetsMap()
    , m_pulseGenerator( GUCEF_NULL )
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
    : activeSide( GUCEF_NULL )
    , toSide( GUCEF_NULL )
    , toSideIsHealthy( true )
    , toSideLastHealthStatusChange( CORE::CDateTime::PastMax )
    , failoverSide( GUCEF_NULL )
    , failoverSideIsHealthy( true )
    , failoverSideLastHealthStatusChange( CORE::CDateTime::PastMax )
    , spilloverBufferSide( GUCEF_NULL )
    , spilloverBufferSideIsHealthy( true )
    , spilloverBufferSideLastHealthStatusChange( CORE::CDateTime::PastMax )
    , flowingIntoSpillover( true )
    , deadLetterSide( GUCEF_NULL )
    , deadLetterSideIsHealthy( true )
    , deadLetterSideLastHealthStatusChange( CORE::CDateTime::PastMax )
    , routeSwitchingTimer()
    , spilloverInfo( GUCEF_NULL )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouter::CRouteInfo::CRouteInfo( const CRouteInfo& src )
    : activeSide( src.activeSide )
    , toSide( src.toSide )
    , toSideIsHealthy( src.toSideIsHealthy )
    , toSideLastHealthStatusChange( src.toSideLastHealthStatusChange )
    , failoverSide( src.failoverSide )
    , failoverSideIsHealthy( src.failoverSideIsHealthy )
    , failoverSideLastHealthStatusChange( src.failoverSideLastHealthStatusChange )
    , spilloverBufferSide( src.spilloverBufferSide )
    , spilloverBufferSideIsHealthy( src.spilloverBufferSideIsHealthy )
    , spilloverBufferSideLastHealthStatusChange( src.spilloverBufferSideLastHealthStatusChange )
    , flowingIntoSpillover( src.flowingIntoSpillover )
    , deadLetterSide( src.deadLetterSide )
    , deadLetterSideIsHealthy( src.deadLetterSideIsHealthy )
    , deadLetterSideLastHealthStatusChange( src.deadLetterSideLastHealthStatusChange )
    , routeSwitchingTimer( src.routeSwitchingTimer )
    , spilloverInfo( src.spilloverInfo )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouter::CRouteInfo::IsSpilloverEgressOngoing( void ) const
{GUCEF_TRACE;
    
    if ( GUCEF_NULL != spilloverInfo )
        return spilloverInfo->IsEgressOngoing();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::CRouteInfo::IsSpilloverIngressOngoing( void ) const
{GUCEF_TRACE;
    
    if ( GUCEF_NULL != spilloverInfo )
        return spilloverInfo->IsIngressOngoing();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::CRouteInfo::IsSpilloverInActiveUse( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != spilloverInfo )
        return spilloverInfo->IsInActiveUse();
    return false;    
}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouter::CSpilloverInfo::CSpilloverInfo( void )
    : spilloverIngressMsgCount( 0 )
    , spilloverEgressMsgCount( 0 )
    , endOfDataEventOccured( false )
    , endOfDataEventSupported( false )
    , route( GUCEF_NULL )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::CSpilloverInfo::IsEgressOngoing( void ) const
{GUCEF_TRACE;
    
    // Is the spillover active and in egress mode?
    if ( GUCEF_NULL != route && route->activeSide == route->spilloverBufferSide && !route->flowingIntoSpillover )
    {    
        // did any messages flow into the spillover to begin with?
        if ( spilloverIngressMsgCount > 0 )
        {
            // Favor the end-of-data mechanism if supported
            if ( endOfDataEventSupported )
                return !endOfDataEventOccured;

            // Alternatively rely on the ingress vs egress message count
            return spilloverEgressMsgCount < spilloverIngressMsgCount;
        }
    }
    return false; 
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::CSpilloverInfo::IsIngressOngoing( void ) const
{GUCEF_TRACE;

    // Is the spillover active and in ingress mode?
    return GUCEF_NULL != route && route->activeSide == route->spilloverBufferSide && route->flowingIntoSpillover;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::CSpilloverInfo::IsInActiveUse( void ) const
{GUCEF_TRACE;

    return IsIngressOngoing() || IsEgressOngoing();
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;
    
    return m_lock.WriterStart( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouter::Unlock( void ) const
{GUCEF_TRACE;
    
    return m_lock.WriterStop();
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::ReadOnlyLock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;
    
    return m_lock.ReaderStart( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouter::ReadOnlyUnlock( void ) const
{GUCEF_TRACE;
    
    return m_lock.ReaderStop();
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

    m_spilloverInfoForTargetsMap.clear();
    m_spilloverInfoMap.clear();
    m_usedInRouteMap.clear();
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
    
    // Validate side capabilities relative to the assigned roles
    CORE::UInt32 sideClientCapabilityErrors = 0;
    TSidePtrToRouteInfoVectorMap::iterator r = m_routeMap.begin();
    while ( r != m_routeMap.end() )
    {
        CPubSubClientSide* fromSide = (*r).first;
        TRouteInfoVector& multiRouteInfo = (*r).second;
        TRouteInfoVector::iterator n = multiRouteInfo.begin();
        while ( n != multiRouteInfo.end() )
        {
            CRouteInfo& routeInfo = (*n);

            if ( GUCEF_NULL != routeInfo.toSide )
            {
                CPubSubClientFeatures features;
                if ( routeInfo.toSide->GetPubSubClientSupportedFeatures( features ) )
                {
                    // the 'to' side must be capable of:
                    //      - receiving messages from the 'from' side and thus for the client that means capable of publishing

                    bool isValid = true;
                    if ( !features.supportsPublishing )
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: 'to' side's client does not support publishing. This is invalid, disabling route: \"" +
                            fromSide->GetSideId() + "\" -> \"" + routeInfo.toSide->GetSideId() );                        
                        isValid = false; 
                        ++sideClientCapabilityErrors;                       
                    }
                    
                    if ( !isValid )
                        routeInfo.toSide = GUCEF_NULL;
                }
            }
            if ( GUCEF_NULL != routeInfo.deadLetterSide )
            {
                CPubSubClientFeatures features;
                if ( routeInfo.deadLetterSide->GetPubSubClientSupportedFeatures( features ) )
                {
                    // the 'dead letter' side must be capable of:
                    //      - receiving messages from the 'from' side and thus for the client that means capable of publishing

                    bool isValid = true;
                    if ( !features.supportsPublishing )
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: 'dead letter' side's client does not support publishing. This is invalid, disabling route: \"" +
                            fromSide->GetSideId() + "\" -> \"" + routeInfo.deadLetterSide->GetSideId() );                        
                        isValid = false;
                        ++sideClientCapabilityErrors;
                    }
                    
                    if ( !isValid )
                        routeInfo.deadLetterSide = GUCEF_NULL;
                }
            }
            if ( GUCEF_NULL != routeInfo.failoverSide )
            {
                CPubSubClientFeatures features;
                if ( routeInfo.failoverSide->GetPubSubClientSupportedFeatures( features ) )
                {
                    // the 'failover' side must be capable of:
                    //      - receiving messages from the 'from' side and thus for the client that means capable of publishing

                    bool isValid = true;
                    if ( !features.supportsPublishing )
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: 'failover' side's client does not support publishing. This is invalid, disabling route: \"" +
                            fromSide->GetSideId() + "\" -> \"" + routeInfo.failoverSide->GetSideId() );                        
                        isValid = false;                        
                        ++sideClientCapabilityErrors;
                    }
                    
                    if ( !isValid )
                        routeInfo.failoverSide = GUCEF_NULL;
                }
            }
            if ( GUCEF_NULL != routeInfo.spilloverBufferSide )
            {
                bool isValid = true;

                // The spillover only works if you also have a surviving 'to' or 'failover' side
                // since its not a final destination in of itself
                CPubSubClientFeatures features;
                if ( GUCEF_NULL != routeInfo.toSide || GUCEF_NULL != routeInfo.failoverSide )
                {                                    
                    if ( routeInfo.spilloverBufferSide->GetPubSubClientSupportedFeatures( features ) )
                    {
                        // the 'spill over' side must be capable of:
                        //      - receiving messages from the 'from' side and thus for the client that means capable of publishing
                        //      - read back messages from the 'spillover' side and thus for the client that means capable of subscribing
                    
                        if ( !features.supportsPublishing )
                        {
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: 'spillover' side's client does not support publishing. This is invalid, disabling route: \"" +
                                fromSide->GetSideId() + "\" -> \"" + routeInfo.spilloverBufferSide->GetSideId() );                        
                            isValid = false;                        
                            ++sideClientCapabilityErrors;
                        }
                        if ( !features.supportsSubscribing )
                        {
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: 'spillover' side's client does not support subscribing. This is invalid, disabling route: \"" +
                                fromSide->GetSideId() + "\" -> \"" + routeInfo.spilloverBufferSide->GetSideId() );                        
                            isValid = false;                        
                            ++sideClientCapabilityErrors;
                        }
                    }
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: having a 'spillover' side requires also having a surviving 'to' or 'failover' side. Having neither is invalid, disabling route: \"" +
                        fromSide->GetSideId() + "\" -> \"" + routeInfo.spilloverBufferSide->GetSideId() );
                    isValid = false;
                }

                // We now also check if its unique, which is a requirement, and add it to a lookup map for faster ack management
                TSidePtrToSpilloverInfoMap::iterator s = m_spilloverInfoMap.find( routeInfo.spilloverBufferSide );
                if ( s == m_spilloverInfoMap.end() )
                {
                    // Add this one to the lookup map as part of the validation
                    // this is both initialization as it is uniqueness validation
                    CSpilloverInfo& newSpilloverInfo = m_spilloverInfoMap[ routeInfo.spilloverBufferSide ];
                    newSpilloverInfo.route = &routeInfo;
                    newSpilloverInfo.endOfDataEventSupported = features.supportsSubscriptionEndOfDataEvent;
                    routeInfo.spilloverInfo = &newSpilloverInfo;

                    // We also add the non-spillover sides for the return flow
                    // this will be used to link back acks from the spillover's egress
                    if ( GUCEF_NULL != routeInfo.toSide )
                        m_spilloverInfoForTargetsMap[ routeInfo.toSide ] = &newSpilloverInfo;
                    if ( GUCEF_NULL != routeInfo.failoverSide )
                        m_spilloverInfoForTargetsMap[ routeInfo.failoverSide ] = &newSpilloverInfo;

                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: Spillover buffers must be unique to a given route, they cannot be shared. The configuration is invalid for spillover buffer \"" + routeInfo.spilloverBufferSide->GetSideId() + 
                        "\" with 'from' side \"" + fromSide->GetSideId() + "\". Removing spillover buffer feature from this route as a best-effort fallback" );
                    isValid = false;
                }

                if ( !isValid )
                    routeInfo.spilloverBufferSide = GUCEF_NULL;
            }

            // Check if the route is still partially viable even with errors
            // This requires at least 1 surviving pathway
            if ( GUCEF_NULL == routeInfo.toSide              &&
                 GUCEF_NULL == routeInfo.failoverSide        &&
                 GUCEF_NULL == routeInfo.spilloverBufferSide )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: No surviving viable pathways for side " + fromSide->GetSideId() + " post client capability verification" );
                return false;                
            }

            ++n;
        }
        ++r;
    }

    if ( 0 == sideClientCapabilityErrors )
    {
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter:BuildRoutes: No issues were detected with pubsub client capabilities for the assigned roles on the configured routes" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter:BuildRoutes: A total of " + CORE::ToString( sideClientCapabilityErrors ) + " issues were detected with pubsub client capabilities for the assigned roles on the configured routes" );
    }

    // For any valid spillovers we will need to auto generate, if it was not explicitly specified, a route from that spillover to the intended 'to' destination
    // since the originally intended traffic was 'from' -> 'to' but it will really at times flow 'from' -> 'spillover' -> 'to'
    // hence we need the 'spillover' -> 'to' route defined for proper flow to occur
    r = m_routeMap.begin();
    while ( r != m_routeMap.end() )
    {
        CPubSubClientSide* fromSide = (*r).first;
        TRouteInfoVector& multiRouteInfo = (*r).second;
        TRouteInfoVector::iterator n = multiRouteInfo.begin();
        while ( n != multiRouteInfo.end() )
        {
            CRouteInfo& routeInfo = (*n);

            if ( GUCEF_NULL != routeInfo.spilloverBufferSide )
            {
                // We found a validated spillover side
                // now lets make sure we have an egress route for it
                TSidePtrToRouteInfoVectorMap::iterator r2 = m_routeMap.find( routeInfo.spilloverBufferSide );
                if ( r2 != m_routeMap.end() )
                {
                    CPubSubClientSide* toSide = GUCEF_NULL != routeInfo.toSide ? routeInfo.toSide : routeInfo.failoverSide;
                    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubFlowRouter:BuildRoutes: Egress route for spillover buffer \"" + routeInfo.spilloverBufferSide->GetSideId() + 
                        "\" to side \"" + toSide->GetSideId() + "\" exists as an explicitly provided configured route. No such route will be auto-generated" );
                }
                else
                {
                    // We are lacking an egress route
                    // Since this is a spillover its pretty clear what the intent is and as such we can auto-generate this route
                    // Doing so cuts down on the complexity of the config that needs to be provided while retaining the option to explicitly configure should it be needed
                    TRouteInfoVector& spilloverMultiRouteInfo = m_routeMap[ routeInfo.spilloverBufferSide ];

                    // The same destinations still apply coming out of the spillover
                    // we do need to get rid of the spillover itself to make sure we dont create an endless loop
                    CRouteInfo newSpilloverBypassRoute( routeInfo );
                    newSpilloverBypassRoute.spilloverBufferSide = GUCEF_NULL;
                    newSpilloverBypassRoute.spilloverInfo = GUCEF_NULL;

                    spilloverMultiRouteInfo.push_back( newSpilloverBypassRoute );

                    CPubSubClientSide* toSide = GUCEF_NULL != routeInfo.toSide ? routeInfo.toSide : routeInfo.failoverSide;
                    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubFlowRouter:BuildRoutes: Auto-generated egress route for spillover buffer \"" + routeInfo.spilloverBufferSide->GetSideId() + 
                        "\" to side \"" + toSide->GetSideId() + "\"" );
                }
            }

            ++n;
        }
        ++r;
    }

    // Now set up the reverse lookup map for efficient updates to routes
    TSidePtrToRouteInfoPtrSetMap sideUsedInMap;
    r = m_routeMap.begin();
    while ( r != m_routeMap.end() )
    {
        TRouteInfoVector& multiRouteInfo = (*r).second;

        TRouteInfoVector::iterator n = multiRouteInfo.begin();
        while ( n != multiRouteInfo.end() )
        {
            CRouteInfo& routeInfo = (*n);

            if ( GUCEF_NULL != routeInfo.toSide )
                sideUsedInMap[ routeInfo.toSide ].insert( &routeInfo );
            if ( GUCEF_NULL != routeInfo.failoverSide )
                sideUsedInMap[ routeInfo.failoverSide ].insert( &routeInfo );
            if ( GUCEF_NULL != routeInfo.spilloverBufferSide )
                sideUsedInMap[ routeInfo.spilloverBufferSide ].insert( &routeInfo );
            if ( GUCEF_NULL != routeInfo.deadLetterSide )
                sideUsedInMap[ routeInfo.deadLetterSide ].insert( &routeInfo );

            ++n;
        }
        ++r;
    }
    // Sets are slower than vectors so we convert to a vector for runtime efficiency
    // The set already got rid of duplicate links and this will remain static going forward
    TSidePtrToRouteInfoPtrSetMap::iterator s = sideUsedInMap.begin();
    while ( s != sideUsedInMap.end() )
    {
        TRouteInfoPtrSet& ptrSet = (*s).second;
        TRouteInfoPtrSet::iterator s2 = ptrSet.begin();
        while ( s2 != ptrSet.end() )
        {
            m_usedInRouteMap[ (*s).first ].push_back( (*s2) );
            ++s2;
        }
        ++s;
    }
    
    // Now set up all the event handlers 
    r = m_routeMap.begin();
    while ( r != m_routeMap.end() )
    {
        RegisterSideEventHandlers( (*r).first );

        TRouteInfoVector& multiRouteInfo = (*r).second;
        TRouteInfoVector::iterator n = multiRouteInfo.begin();
        while ( n != multiRouteInfo.end() )
        {
            CRouteInfo& routeInfo = (*n);
            RegisterRouteEventHandlers( routeInfo );
            DetermineActiveRoute( routeInfo );
            ++n;
        }

        ++r;
    }

    UpdatePulseGeneratorUsage();

    return true;
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::RegisterRouteEventHandlers( CRouteInfo& routeInfo )
{GUCEF_TRACE;

    TEventCallback callback( this, &CPubSubFlowRouter::OnRouteSwitchTimerCycle );
    SubscribeTo( &routeInfo.routeSwitchingTimer ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::UpdatePulseGeneratorUsage( void )
{GUCEF_TRACE;
    
    TSidePtrToRouteInfoVectorMap::iterator r = m_routeMap.begin();
    while ( r != m_routeMap.end() )
    {
        TRouteInfoVector& multiRouteInfo = (*r).second;
        TRouteInfoVector::iterator n = multiRouteInfo.begin();
        while ( n != multiRouteInfo.end() )
        {
            CRouteInfo& routeInfo = (*n);
            routeInfo.routeSwitchingTimer.SetPulseGenerator( m_pulseGenerator );
            ++n;
        }
        ++r;
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::SetPulseGenerator( CORE::CPulseGenerator* pulseGenerator )
{GUCEF_TRACE;
    
    MT::CScopeWriterLock lock( m_lock );
    m_pulseGenerator = pulseGenerator;
    UpdatePulseGeneratorUsage();
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

    {
        // since spillover buffers are unique for a given route, garanteed by route building validation, logically only 1 thread would be 
        // updating the spillover info, we just need the mapping to remain in existance.
        // as such a read lock will suffice
        MT::CScopeReaderLock lock( m_lock );

        // Check if msgReceiverSide is a spillover target
        TSidePtrToSpilloverInfoPtrMap::iterator i = m_spilloverInfoForTargetsMap.find( msgReceiverSide );
        if ( i != m_spilloverInfoForTargetsMap.end() )
        {
            // Check if this is normal flow or spillover egress
            CSpilloverInfo* spilloverInfo = (*i).second;
            if ( spilloverInfo->IsEgressOngoing() )
                --spilloverInfo->spilloverIngressMsgCount;
        }
        else
        {
            // Check if msgReceiverSide is a spillover
            TSidePtrToSpilloverInfoMap::iterator n = m_spilloverInfoMap.find( msgReceiverSide );
            if ( i != m_spilloverInfoForTargetsMap.end() )
            {
                CSpilloverInfo& spilloverInfo = (*n).second;
                ++spilloverInfo.spilloverIngressMsgCount;
            }
        }
    }

    // Right now we dont use the route info but instead use the source location
    // we can revisit this later but it is fragile due to reliance on GetOpaqueUserData()
    // Due to this we right now dont need a lock here for the ack portion

    // Note that acks are always ONLY to the side that originated the message since that side is the only
    // one that knows about that specific message and whatever conventions its abstracting wrt the underlying message format

    CORE::UInt32 invokerThreadId = MT::GetCurrentTaskID();
    
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
        TRouteInfoVector::iterator n = multiRouteInfo.begin();
        while ( n != multiRouteInfo.end() )
        {            
            CRouteInfo& routeInfo = (*n);
            CPubSubClientSide* targetSide = GUCEF_NULL;

            // Make sure we are not in spillover egress mode
            // if spillover egress is active the route cannot be published to from the 'from' side
            if ( !routeInfo.IsSpilloverEgressOngoing() )
            {
                switch ( routeType )
                {                                
                    case RouteType::Active: { targetSide = routeInfo.activeSide; break; } 

                    case RouteType::Primary: { targetSide = routeInfo.toSide; break; }
                    case RouteType::Failover: { targetSide = routeInfo.failoverSide; break; }
                    case RouteType::SpilloverBuffer: { targetSide = routeInfo.spilloverBufferSide; break; }
                    case RouteType::DeadLetter: { targetSide = routeInfo.deadLetterSide; break; }
                    default: { break; }
                }
            }

            if ( GUCEF_NULL != targetSide )
            {                    
                if ( !targetSide->PublishMsgs( msgs ) )
                {
                    publishIstotalSuccess = false;
                }
            }   
            else
            {
                publishIstotalSuccess = false;
            }
            ++n;
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

        TEventCallback callback2( this, &CPubSubFlowRouter::OnSidePubSubClientInstantiation );
        SubscribeTo( side                                              ,
                     CPubSubClientSide::PubSubClientInstantiationEvent ,
                     callback2                                         );
    }
}

/*-------------------------------------------------------------------------*/

void 
CPubSubFlowRouter::RegisterSidePubSubClientEventHandlers( CPubSubClientPtr& client )
{GUCEF_TRACE;

    if ( !client.IsNULL() ) 
    {
        CPubSubClientFeatures clientFeatures;
        client->GetSupportedFeatures( clientFeatures );
        
        // client and topic level subscriptions are currently only used for EOD
        // if we have another use-case later remove this check
        if ( clientFeatures.supportsSubscriptionEndOfDataEvent )
        {
            TEventCallback callback( this, &CPubSubFlowRouter::OnSidePubSubClientTopicCreation );
            SubscribeTo( client.GetPointerAlways()              ,
                         CPubSubClient::TopicAccessCreatedEvent ,
                         callback                               );

            TEventCallback callback2( this, &CPubSubFlowRouter::OnSidePubSubClientTopicsAutoCreation );
            SubscribeTo( client.GetPointerAlways()                   ,
                         CPubSubClient::TopicsAccessAutoCreatedEvent ,
                         callback2                                   );

            // register event handlers for all already existing topics (if any)
            RegisterSidePubSubClientTopicEventHandlers( client );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::RegisterSidePubSubClientTopicEventHandlers( CPubSubClientTopic* topicAccess )
{GUCEF_TRACE;

    if ( GUCEF_NULL != topicAccess ) 
    {
        TEventCallback callback( this, &CPubSubFlowRouter::OnSidePubSubClientTopicCreation );
        SubscribeTo( topicAccess                                    ,
                     CPubSubClientTopic::SubscriptionEndOfDataEvent ,
                     callback                                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::RegisterSidePubSubClientTopicEventHandlers( CPubSubClientPtr& client )
{GUCEF_TRACE;

    if ( !client.IsNULL() ) 
    {
        CPubSubClient::PubSubClientTopicSet topics;
        client->GetAllCreatedTopicAccess( topics );
        
        CPubSubClient::PubSubClientTopicSet::iterator i = topics.begin();
        while ( i != topics.end() )
        {
            RegisterSidePubSubClientTopicEventHandlers( (*i) );
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouter::ConfigureSpillover( CPubSubClientSide* spilloverSide, bool flowIntoSpillover )
{GUCEF_TRACE;

    // pull a copy of the config
    CPubSubSideChannelSettings sideSettings = spilloverSide->GetSideSettings();

    // (Re)Configure for the intended flow direction
    if ( flowIntoSpillover ) 
    {
        // Configure for ingress aka publishing
        sideSettings.pubsubClientConfig.desiredFeatures.supportsPublishing = true;
        sideSettings.pubsubClientConfig.desiredFeatures.supportsSubscribing = false;
        
        CPubSubClientConfig::TPubSubClientTopicConfigVector& topicConfigs = sideSettings.pubsubClientConfig.topics;
        CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator t = topicConfigs.begin();
        while ( t != topicConfigs.end() )
        {
            CPubSubClientTopicConfig& topicConfig = (*t);

            topicConfig.needPublishSupport = true;
            topicConfig.needSubscribeSupport = false;

            ++t;
        }
    }
    else
    {
        // Configure for egress aka subscribing
        sideSettings.pubsubClientConfig.desiredFeatures.supportsPublishing = false;
        sideSettings.pubsubClientConfig.desiredFeatures.supportsSubscribing = true;

        CPubSubClientConfig::TPubSubClientTopicConfigVector& topicConfigs = sideSettings.pubsubClientConfig.topics;
        CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator t = topicConfigs.begin();
        while ( t != topicConfigs.end() )
        {
            CPubSubClientTopicConfig& topicConfig = (*t);

            topicConfig.needPublishSupport = false;
            topicConfig.needSubscribeSupport = true;

            ++t;
        }
    }

    if ( spilloverSide->DisconnectPubSubClient() )
    {
        if ( spilloverSide->LoadConfig( sideSettings ) )
        {
            return spilloverSide->ConnectPubSubClient();
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::DetermineActiveRoute( CRouteInfo& routeInfo )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_lock );
       
    CPubSubClientSide* newActiveSide = GUCEF_NULL;
    
    CORE::UInt64 primarySideHealthDurationInMs = (CORE::UInt64) routeInfo.toSideLastHealthStatusChange.GetTimeDifferenceInMillisecondsToNow();
    
    // Try our primary 'to' route first
    // This is the normal 99% (hopefully) case
    if ( GUCEF_NULL != routeInfo.toSide && 
         routeInfo.toSideIsHealthy && 
         primarySideHealthDurationInMs > m_config.minPrimarySideGoodHealthDurationBeforeActivationInMs )
    {                    
        if ( routeInfo.activeSide != routeInfo.toSide )
        {        
            // Before (re)activating the primary route check to make sure we didnt use the spillover
            // if so need we need to egress those messages first before restoring the normal route
            if ( !routeInfo.IsSpilloverInActiveUse() )
            {
                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                    "):DetermineActiveRoute: Switching active route to the Primary side" );

                routeInfo.activeSide = routeInfo.toSide;
                routeInfo.routeSwitchingTimer.SetEnabled( false );
            }
            else
            {                
                if ( !routeInfo.IsSpilloverEgressOngoing() )
                {
                    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                        "):DetermineActiveRoute: Primary side health is restored but spillover was used. Commencing spillover egress" );

                    // Switch from ingress to egress
                    if ( ConfigureSpillover( routeInfo.spilloverBufferSide, false ) )
                    {
                        routeInfo.flowingIntoSpillover = false;
                    }
                    else
                    {
                        // rely on timer cycle to try again, nothing we can do
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                            "):DetermineActiveRoute: Primary side health is restored but spillover was used. Failed to reconfigure spillover for egress" );
                    }
                }
                else
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                        "):DetermineActiveRoute: Primary side health is Ok and spillover egress is in-progress" );
                }
            }
        }
    }   
    else
    {
        // Primary 'to' route is not available
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
            "):DetermineActiveRoute: The Primary route is not available. Its health status last changed " + CORE::ToString( primarySideHealthDurationInMs ) +
            "ms ago" );

        if ( primarySideHealthDurationInMs > m_config.minBadHealthDurationBeforeSpilloverInMs )
        {        
            CORE::UInt64 failoverSideHealthDurationInMs = (CORE::UInt64) routeInfo.failoverSideLastHealthStatusChange.GetTimeDifferenceInMillisecondsToNow();
            
            // Try the failover route if one is available
            if ( GUCEF_NULL != routeInfo.failoverSide && 
                 routeInfo.failoverSideIsHealthy && 
                 failoverSideHealthDurationInMs > m_config.minFailoverSideGoodHealthDurationBeforeActivationInMs )
            {
                if ( routeInfo.activeSide != routeInfo.failoverSide )
                {
                    // Before (re)activating the failover route check to make sure we didnt use the spillover
                    // if so need we need to egress those messages first before restoring the normal route
                    if ( !routeInfo.IsSpilloverInActiveUse() )
                    {
                        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                            "):DetermineActiveRoute: Switching active route to the Failover side" );
        
                        routeInfo.activeSide = routeInfo.failoverSide;
                        routeInfo.routeSwitchingTimer.SetEnabled( false );
                    }
                    else
                    {                
                        if ( !routeInfo.IsSpilloverEgressOngoing() )
                        {
                            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                                "):DetermineActiveRoute: Failover side health is restored but spillover was used. Commencing spillover egress" );

                            // Switch from ingress to egress                            
                            if ( ConfigureSpillover( routeInfo.spilloverBufferSide, false ) )
                            {
                                routeInfo.flowingIntoSpillover = false;
                            }
                            else
                            {
                                // rely on timer cycle to try again, nothing we can do
                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                                    "):DetermineActiveRoute: Failover side health is restored but spillover was used. Failed to reconfigure spillover for egress" );
                            }
                        }
                        else
                        {
                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                                "):DetermineActiveRoute: Failover side health is Ok and spillover egress is in-progress" );
                        }
                    }
                }
            }
            else
            {            
                // It seems the primary and failover routes have failed us
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                    "):DetermineActiveRoute: The Primary and Failover sides for the route are not available. Failover health status last changed " + CORE::ToString( failoverSideHealthDurationInMs ) +
                    "ms ago" );
                
                // If there is a spillover available we can try using that route as a different type of failover
                if ( GUCEF_NULL != routeInfo.spilloverBufferSide && 
                     routeInfo.spilloverBufferSideIsHealthy && 
                     routeInfo.spilloverBufferSideLastHealthStatusChange.GetTimeDifferenceInMillisecondsToNow() > m_config.minSpilloverSideGoodHealthDurationBeforeActivationInMs )
                {
                    // At this point the spillover is healthy and the primary and failover are not
                    if ( primarySideHealthDurationInMs > m_config.minBadHealthDurationBeforeSpilloverInMs ||
                         failoverSideHealthDurationInMs > m_config.minBadHealthDurationBeforeSpilloverInMs )
                    {
                        // At this point we also know we waited the configured time before failing over to the spillover
                        // this is important because for a quick flickering connection its not worth the overhead to fail over to the spillover buffer which likely involves setup cost
                        // plus the ongoing headache of having to read back from the spillover once the primary or failover routes are restored.
                        // Thus if a simple retry saves the day between 'from' and 'to' you should favor that instead of the spillover mechanism
                        if ( routeInfo.activeSide != routeInfo.spilloverBufferSide )
                        {
                            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                                "):DetermineActiveRoute: Switching active route to the Spillover buffer side" );
        
                            routeInfo.activeSide = routeInfo.spilloverBufferSide;
                            routeInfo.routeSwitchingTimer.SetEnabled( false );

                            // If the spillover is configured for egress instead of ingress we need to either wait of egress to finish 
                            // or if its finished we need to reconfigure for ingress
                            if ( !routeInfo.flowingIntoSpillover )
                            {
                                CSpilloverInfo& spilloverInfo = m_spilloverInfoMap[ routeInfo.spilloverBufferSide ];
                                
                                // If the spillover is performing egress we need to let it complete first                               
                                if ( !spilloverInfo.IsEgressOngoing()  )
                                {                                
                                    // reconfigure the spillover for data ingress
                                    // this is an expensive operation 
                                    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
                                        "):DetermineActiveRoute: Reconfiguring spillover side " + routeInfo.spilloverBufferSide->GetSideId() + " for data ingress" );

                                    if ( ConfigureSpillover( routeInfo.spilloverBufferSide, true ) )
                                    {
                                        routeInfo.flowingIntoSpillover = true;
                                    }
                                    else
                                    {
                                        // Something went wrong, abort and try again next cycle
                                        routeInfo.activeSide = GUCEF_NULL;
                                        routeInfo.routeSwitchingTimer.SetEnabled( true );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }                
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::OnRouteSwitchTimerCycle( CORE::CNotifier* notifier    ,
                                            const CORE::CEvent& eventId  ,
                                            CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CORE::CTimer* routeSwitchTimer = static_cast< CORE::CTimer* >( notifier );
    if ( GUCEF_NULL != routeSwitchTimer )
    {
        CRouteInfo* routeInfo = static_cast< CRouteInfo* >( routeSwitchTimer->GetOpaqueUserData() );
        if ( GUCEF_NULL != routeInfo )
        {
            DetermineActiveRoute( *routeInfo );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::UpdateRoutesBasedOnSideHealthStatus( CPubSubClientSide* side ,
                                                        bool isHealthy          )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_lock );
    
    TSidePtrToRouteInfoPtrVectorMap::iterator i = m_usedInRouteMap.find( side );
    if ( i != m_usedInRouteMap.end() )
    {
        TRouteInfoPtrVector& multiRouteInfo = (*i).second;

        CORE::UInt32 flagsUpdated = 0;
        TRouteInfoPtrVector::iterator n = multiRouteInfo.begin();
        while ( n != multiRouteInfo.end() )
        {
            CRouteInfo* routeInfo = (*n);
        
            if ( side == routeInfo->toSide )
            {
                if ( routeInfo->toSideIsHealthy != isHealthy )
                {
                    routeInfo->toSideIsHealthy = isHealthy;                    
                    routeInfo->toSideLastHealthStatusChange = CORE::CDateTime::NowUTCDateTime();
                    ++flagsUpdated;
                    routeInfo->routeSwitchingTimer.SetEnabled( true );
                }
            }
            if ( side == routeInfo->failoverSide )
            {
                if ( routeInfo->failoverSideIsHealthy != isHealthy )
                {
                    routeInfo->failoverSideIsHealthy = isHealthy;                    
                    routeInfo->failoverSideLastHealthStatusChange = CORE::CDateTime::NowUTCDateTime();
                    ++flagsUpdated;
                    routeInfo->routeSwitchingTimer.SetEnabled( true );
                }
            }
            if ( side == routeInfo->toSide )
            {
                if ( routeInfo->deadLetterSideIsHealthy != isHealthy )
                {
                    routeInfo->deadLetterSideIsHealthy = isHealthy;                    
                    routeInfo->deadLetterSideLastHealthStatusChange = CORE::CDateTime::NowUTCDateTime();
                    ++flagsUpdated;
                    routeInfo->routeSwitchingTimer.SetEnabled( true );
                }
            }
            if ( side == routeInfo->spilloverBufferSide )
            {
                if ( routeInfo->spilloverBufferSideIsHealthy != isHealthy )
                {
                    routeInfo->spilloverBufferSideIsHealthy = isHealthy;                    
                    routeInfo->spilloverBufferSideLastHealthStatusChange = CORE::CDateTime::NowUTCDateTime();
                    ++flagsUpdated;
                    routeInfo->routeSwitchingTimer.SetEnabled( true );
                }
            }

            ++n;
        }

        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouter(" + CORE::PointerToString( this ) +
            "):UpdateRoutesBasedOnSideHealthStatus: Updated " + CORE::ToString( multiRouteInfo.size() ) + " routes that have side \"" + 
            side->GetSideId() + "\" for a total of " + CORE::ToString( flagsUpdated ) + " health status updates. Healthy=" + CORE::ToString( isHealthy ) );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::OnSideHealthStatusChange( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CPubSubClientSide* side = static_cast< CPubSubClientSide* >( notifier ); 
    CPubSubClientSide::THealthStatusChangeEventData* healthStatus = static_cast< CPubSubClientSide::THealthStatusChangeEventData* >( eventData );

    if ( GUCEF_NULL == side || GUCEF_NULL == healthStatus )
        return;

    UpdateRoutesBasedOnSideHealthStatus( side, *healthStatus );
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::OnSidePubSubClientTopicEndOfData( CORE::CNotifier* notifier    ,
                                                     const CORE::CEvent& eventId  ,
                                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CPubSubClientTopic* topic = static_cast< CPubSubClientTopic* >( notifier ); 
    if ( GUCEF_NULL == topic )
        return;

    CPubSubClient* client = topic->GetClient();
    if ( GUCEF_NULL == client )
        return;
        
    CPubSubClientSide* side = static_cast< CPubSubClientSide* >( client->GetOpaqueUserData() );
    if ( GUCEF_NULL == side )
        return;
    
    MT::CScopeReaderLock readLock( m_lock );
    
    // Check if this side is a spillover and in doing so get the spillover info
    TSidePtrToSpilloverInfoMap::iterator i = m_spilloverInfoMap.find( side );
    if ( i != m_spilloverInfoMap.end() )
    {
        // The end-of-data side is a spillover side

        // Now we will perform the next check since it might be worth it...
        // We really care about the aggregate status in the router, not per topic
        if ( client->AreAllSubscriptionsAtEndOfData() )
        {
            CSpilloverInfo& spilloverInfo = (*i).second;
            spilloverInfo.endOfDataEventOccured = true;

            readLock.EarlyUnlock(); // @TODO: reader to writer transition support            
            
            // if an entire client and thus a side (since its a 1:1 relationship) has reached EOD
            // we check to see if we need to update the active route            
            DetermineActiveRoute( *spilloverInfo.route );
        }
    }    
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::OnSidePubSubClientInstantiation( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventId  ,
                                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CPubSubClientPtr* pubsubClientPtr = static_cast< CPubSubClientPtr* >( eventData ); 
    if ( GUCEF_NULL == pubsubClientPtr )
        return;
    RegisterSidePubSubClientEventHandlers( *pubsubClientPtr );   
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::OnSidePubSubClientTopicCreation( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventId  ,
                                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CPubSubClient* pubsubClient = static_cast< CPubSubClient* >( notifier ); 
    if ( GUCEF_NULL == pubsubClient || GUCEF_NULL == eventData )
        return;

    CORE::CString topicName = *static_cast< CPubSubClient::TopicAccessCreatedEventData* >( eventData );
    CPubSubClientTopic* topicAccess = pubsubClient->GetTopicAccess( topicName );
    if ( GUCEF_NULL != topicAccess ) 
    {
        RegisterSidePubSubClientTopicEventHandlers( topicAccess );   
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouter::OnSidePubSubClientTopicsAutoCreation( CORE::CNotifier* notifier    ,
                                                         const CORE::CEvent& eventId  ,
                                                         CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CPubSubClient* pubsubClient = static_cast< CPubSubClient* >( notifier ); 
    if ( GUCEF_NULL == pubsubClient || GUCEF_NULL == eventData )
        return;

    CPubSubClient::PubSubClientTopicSet& topics = *static_cast< CPubSubClient::TopicsAccessAutoCreatedEventData* >( eventData );
    CPubSubClient::PubSubClientTopicSet::iterator i = topics.begin();
    while ( i != topics.end() )
    {
        RegisterSidePubSubClientTopicEventHandlers( (*i) );
        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
