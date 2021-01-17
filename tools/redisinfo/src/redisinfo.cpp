/*
 *  redisinfo: Service for obtaining redis info from a cluster
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#include <string.h>

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefWEB_CDummyHTTPServerResource.h"
#define GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#include "redisinfo.h"

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#include "crc16.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE                  10000

const CORE::CString RedisInfoService::HashSlotFileCodec = "json"; 

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

Settings::Settings( void )
    : clusterName()
    , redisAddress()
    , collectMetrics( true )
    , metricPrefix()
    , gatherInfoReplication( true )
    , gatherInfoPersistence( true )
    , gatherInfoStats( true )
    , gatherInfoCommandStats( true )
    , gatherInfoMemory( true )
    , gatherStreamInfo( true )
    , streamsToGatherInfoFrom()
    , streamIndexingInterval( 1000 * 60 )
    , gatherInfoClients( true )
    , gatherInfoCpu( true )
    , gatherInfoKeyspace( true )
    , gatherClusterInfo( true )
    , gatherErrorReplyCount( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Settings::Settings( const Settings& src )
    : clusterName( src.clusterName )
    , redisAddress( src.redisAddress )
    , collectMetrics( src.collectMetrics )
    , metricPrefix()
    , gatherInfoReplication( src.gatherInfoReplication )
    , gatherInfoPersistence( src.gatherInfoPersistence )
    , gatherInfoStats( src.gatherInfoStats )
    , gatherInfoCommandStats( src.gatherInfoCommandStats )
    , gatherInfoMemory( src.gatherInfoMemory )
    , gatherStreamInfo( src.gatherStreamInfo )
    , streamsToGatherInfoFrom( src.streamsToGatherInfoFrom )
    , streamIndexingInterval( src.streamIndexingInterval )
    , gatherInfoClients( src.gatherInfoClients )
    , gatherInfoCpu( src.gatherInfoCpu )
    , gatherInfoKeyspace( src.gatherInfoKeyspace )
    , gatherClusterInfo( src.gatherClusterInfo )
    , gatherErrorReplyCount( src.gatherErrorReplyCount )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Settings&
Settings::operator=( const Settings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        clusterName = src.clusterName;
        redisAddress = src.redisAddress;
        collectMetrics = src.collectMetrics;
        metricPrefix = src.metricPrefix;
        gatherInfoReplication = src.gatherInfoReplication;
        gatherInfoPersistence = src.gatherInfoPersistence;
        gatherInfoStats = src.gatherInfoStats;
        gatherInfoCommandStats = src.gatherInfoCommandStats;
        gatherInfoMemory = src.gatherInfoMemory;
        gatherStreamInfo = src.gatherStreamInfo;
        streamsToGatherInfoFrom = src.streamsToGatherInfoFrom;
        streamIndexingInterval = src.streamIndexingInterval;
        gatherInfoClients = src.gatherInfoClients;
        gatherInfoCpu = src.gatherInfoCpu;
        gatherInfoKeyspace = src.gatherInfoKeyspace;
        gatherClusterInfo = src.gatherClusterInfo;
        gatherErrorReplyCount = src.gatherErrorReplyCount;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
Settings::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    tree.SetAttribute( "clusterName", clusterName );
    tree.SetAttribute( "redisAddress", redisAddress.AddressAndPortAsString() );
    tree.SetAttribute( "collectMetrics", collectMetrics );
    tree.SetAttribute( "metricPrefix", metricPrefix );
    tree.SetAttribute( "gatherInfoReplication", gatherInfoReplication );
    tree.SetAttribute( "gatherInfoPersistence", gatherInfoPersistence );
    tree.SetAttribute( "gatherInfoStats", gatherInfoStats );
    tree.SetAttribute( "gatherInfoCommandStats", gatherInfoCommandStats );
    tree.SetAttribute( "gatherInfoMemory", gatherInfoMemory );
    tree.SetAttribute( "gatherStreamInfo", gatherStreamInfo );
    tree.SetAttribute( "streamsToGatherInfoFrom", CORE::ToString( streamsToGatherInfoFrom ) ); 
    tree.SetAttribute( "streamIndexingInterval", CORE::ToString( streamIndexingInterval ) );
    tree.SetAttribute( "gatherInfoClients", gatherInfoClients );
    tree.SetAttribute( "gatherInfoCpu", gatherInfoCpu );
    tree.SetAttribute( "gatherInfoKeyspace", gatherInfoKeyspace );
    tree.SetAttribute( "gatherClusterInfo", gatherClusterInfo );
    tree.SetAttribute( "gatherErrorReplyCount", gatherErrorReplyCount );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
Settings::LoadConfig( const CORE::CDataNode& tree )
{GUCEF_TRACE;

    clusterName = tree.GetAttributeValueOrChildValueByName( "clusterName", clusterName );
    redisAddress.SetHostnameAndPort( tree.GetAttributeValueOrChildValueByName( "redisAddress", redisAddress.AddressAndPortAsString() ) );
    collectMetrics = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "collectMetrics" ), collectMetrics );
    metricPrefix = tree.GetAttributeValueOrChildValueByName( "metricPrefix", metricPrefix );
    gatherInfoReplication = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherInfoReplication" ), gatherInfoReplication );
    gatherInfoPersistence = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherInfoPersistence" ), gatherInfoPersistence );
    gatherInfoStats = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherInfoStats" ), gatherInfoStats );
    gatherInfoCommandStats = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherInfoCommandStats" ), gatherInfoCommandStats );
    gatherInfoMemory = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherInfoMemory" ), gatherInfoMemory );
    gatherStreamInfo = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherStreamInfo" ), gatherStreamInfo );
    streamsToGatherInfoFrom = CORE::StringToStringVector( tree.GetAttributeValueOrChildValueByName( "streamsToGatherInfoFrom" ), streamsToGatherInfoFrom );
    streamIndexingInterval = CORE::StringToInt32( tree.GetAttributeValueOrChildValueByName( "streamIndexingInterval" ), streamIndexingInterval );
    gatherInfoClients = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherInfoClients" ), gatherInfoClients );
    gatherInfoCpu = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherInfoCpu" ), gatherInfoCpu );
    gatherInfoKeyspace = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherInfoKeyspace" ), gatherInfoKeyspace );
    gatherClusterInfo = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherClusterInfo" ), gatherClusterInfo );
    gatherErrorReplyCount = CORE::StringToBool( tree.GetAttributeValueOrChildValueByName( "gatherErrorReplyCount" ), gatherErrorReplyCount );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
Settings::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "redisinfo::Settings";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

RedisInfoService::RedisInfoService()
    : CORE::CTaskConsumer()
    , m_redisContext( GUCEF_NULL )
    , m_redisPacketArgs()
    , m_metricsTimer( GUCEF_NULL )
    , m_redisReconnectTimer( GUCEF_NULL )
    , m_streamIndexingTimer( GUCEF_NULL )
    , m_filteredStreamNames()
    , m_redisNodesMap()
    , m_hashSlotOriginStrMap()
    , m_redisClusterErrorReplies( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

RedisInfoService::~RedisInfoService()
{GUCEF_TRACE;

    RedisDisconnect();

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    delete m_streamIndexingTimer;
    m_streamIndexingTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback1( this, &RedisInfoService::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback1                      );

    TEventCallback callback2( this, &RedisInfoService::OnRedisReconnectTimerCycle );
    SubscribeTo( m_redisReconnectTimer          ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback2                      );

    TEventCallback callback3( this, &RedisInfoService::OnStreamIndexingTimerCycle );
    SubscribeTo( m_streamIndexingTimer          ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback3                      );
}

/*-------------------------------------------------------------------------*/

bool 
RedisInfoService::LoadConfig( const Settings& settings )
{GUCEF_TRACE;

    m_settings = settings;
    return true;
}

/*-------------------------------------------------------------------------*/

const Settings& 
RedisInfoService::GetSettings( void ) const
{GUCEF_TRACE;

    return m_settings;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
RedisInfoService::CalculateRedisHashSlot( const CORE::CString& keyStr ) const
{GUCEF_TRACE;

    // The following code is copied from: https://redis.io/topics/cluster-spec

    // Minor changes were made to accept a string object and use platform types
    CORE::UInt32 keylen = keyStr.Length();
    const char* key = keyStr.C_String();

    int s, e; /* start-end indexes of { and } */

    /* Search the first occurrence of '{'. */
    for (s = 0; s < keylen; s++)
        if (key[s] == '{') break;

    /* No '{' ? Hash the whole key. This is the base case. */
    if ( s == keylen)
        return sw::redis::crc16(key,keylen) & 16383;

    /* '{' found? Check if we have the corresponding '}'. */
    for (e = s+1; e < keylen; e++)
        if (key[e] == '}') break;

    /* No '}' or nothing between {} ? Hash the whole key. */
    if (e == keylen || e == s+1)
        return sw::redis::crc16(key,keylen) & 16383;

    /* If we are here there is both a { and a } on its right. Hash
     * what is in the middle between { and }. */
    return sw::redis::crc16(key+s+1,e-s-1) & 16383;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::CalculateKeysForAllHashSlots( TUInt32ToStringSetMap& hashMap , 
                                                CORE::UInt32 minKeysPerSlot    , 
                                                CORE::UInt32 maxKeysPerSlot    ) const
{GUCEF_TRACE;

    // Since we need to generate a key per slot a minimum lower than 1 makes no sense
    if ( minKeysPerSlot < 1 )
        minKeysPerSlot = 1;
    
    CORE::UInt32 lowestKeysInSlot = 0;
    
    CORE::UInt64 i=0;
    size_t lastSize=0;
    while ( hashMap.size() < 16383 || lowestKeysInSlot < minKeysPerSlot )
    {
        CORE::CString hashOriginString = CORE::ToString( i );
        CORE::UInt32 hashSlot = CalculateRedisHashSlot( hashOriginString );
        
        TStringSet& hashOriginStringSet = hashMap[ hashSlot ];
        if ( hashOriginStringSet.size() < maxKeysPerSlot || 0 == maxKeysPerSlot )
        {
            hashOriginStringSet.insert( hashOriginString );

            if ( hashMap.size() >= 16383 )
            {
                // Check for lowest keys per slow threshold
                lowestKeysInSlot = minKeysPerSlot;
                TUInt32ToStringSetMap::iterator n = hashMap.begin();
                while ( n != hashMap.end() )
                {
                    if ( (*n).second.size() < lowestKeysInSlot )
                        lowestKeysInSlot = (*n).second.size();
                    ++n;
                }
            }
        }

        if ( hashMap.size() > lastSize )
        {
            CORE::Float32 percDone = hashMap.size() / 163.83f;
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService:CalculateKeysForAllHashSlots: Now " + CORE::ToString( percDone ) + "% done generating hashes for all slots" );
            lastSize = hashMap.size();
        }
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::SerializeKeysForHashSlots( const TUInt32ToStringSetMap& hashMap, CORE::CDataNode& doc ) const
{GUCEF_TRACE;

    doc.Clear();
    doc.SetName( "RedisHashMap" );
    doc.SetNodeType( GUCEF_DATATYPE_ARRAY );
    TUInt32ToStringSetMap::const_iterator i = hashMap.begin();
    while ( i != hashMap.end() )
    {        
        CORE::CDataNode* slotEntry = doc.AddChild( "slot", GUCEF_DATATYPE_OBJECT );
        slotEntry->SetAttribute( "id", CORE::ToString( (*i).first ), GUCEF_DATATYPE_UINT32 );
        
        CORE::CDataNode* originStringArray = slotEntry->AddChild( "hashOriginStrings", GUCEF_DATATYPE_ARRAY );
        const TStringSet& hashOriginStrings = (*i).second;
        TStringSet::const_iterator n = hashOriginStrings.begin();
        while ( n != hashOriginStrings.end() )
        {
            originStringArray->AddChild( (*n), GUCEF_DATATYPE_STRING );
            ++n;
        }
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::DeserializeKeysForHashSlots( TUInt32ToStringSetMap& hashMap, const CORE::CDataNode& doc ) const
{GUCEF_TRACE;

    CORE::CDataNode::const_iterator i = doc.ConstBegin();
    while ( i != doc.ConstEnd() )
    {        
        CORE::CString hashSlot = (*i)->GetAttributeValueOrChildValueByName( "id" );
        CORE::CString::StringVector hashOriginStrings = (*i)->GetChildrenValues();

        if ( !hashSlot.IsNULLOrEmpty() && !hashOriginStrings.empty() )
        {            
            TStringSet& hashEntries = hashMap[ CORE::StringToUInt16( hashSlot ) ];
            CORE::CString::StringVector::iterator n = hashOriginStrings.begin();
            while ( n != hashOriginStrings.end() )
            {
                hashEntries.insert( (*n) ); 
                ++n;
            }
        }
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::SaveDocTo( const CORE::CDataNode& doc     , 
                             const CORE::CString& codecName , 
                             const CORE::CString& vfsPath   ) const
{GUCEF_TRACE;
    
    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec; 
    CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( codecName, codec, false );
    if ( !codec )
        return false;

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    VFS::CVFS::CVFSHandlePtr file = vfs.GetFile( vfsPath, "wb", true );
    if ( !file )
        return false;

    return codec->StoreDataTree( &doc, file->GetAccess() );
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::LoadDocFrom( CORE::CDataNode& doc           , 
                               const CORE::CString& codecName , 
                               const CORE::CString& vfsPath   ) const
{GUCEF_TRACE;
    
    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec; 
    CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( codecName, codec, false );
    if ( !codec )
        return false;

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    VFS::CVFS::CVFSHandlePtr file = vfs.GetFile( vfsPath, "rb", false );
    if ( !file )
        return false;

    return codec->BuildDataTree( &doc, file->GetAccess() );
}

/*-------------------------------------------------------------------------*/

CORE::CString
RedisInfoService::GetType( void ) const
{GUCEF_TRACE;

    return "RedisInfoService";
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::ProvideHashSlotMapDoc( void )
{GUCEF_TRACE;

    const CORE::CString hashSlotFile = "InstallPath/redis/clusters/" + m_settings.clusterName + "/RedisHashMap.v1.json";
   
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    if ( !vfs.FileExists( hashSlotFile ) )
    {
        if ( CalculateKeysForAllHashSlots( m_hashSlotOriginStrMap, 1, 1 ) )
        {
            CORE::CDataNode hashSlotsDoc;
            if ( SerializeKeysForHashSlots( m_hashSlotOriginStrMap, hashSlotsDoc ) )
            {
                return SaveDocTo( hashSlotsDoc, HashSlotFileCodec, hashSlotFile );
            }
        }
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::LoadHashSlotMap( void )
{GUCEF_TRACE;  
   
    const CORE::CString hashSlotFile = "InstallPath/redis/clusters/" + m_settings.clusterName + "/RedisHashMap.v1.json";
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    if ( !vfs.FileExists( hashSlotFile ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):LoadHashSlotMap: hash slot map did not exist, attempting to generate a new one" );
        if ( !ProvideHashSlotMapDoc() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):LoadHashSlotMap: hash slot map did not exist and failed to provide a new one" );
            return false;
        }
    }
    else
    {
        CORE::CDataNode hashSlotsDoc;
        if ( LoadDocFrom( hashSlotsDoc, HashSlotFileCodec, hashSlotFile ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):LoadHashSlotMap: Loaded hash slot map document, deserializing..." );
            if ( DeserializeKeysForHashSlots( m_hashSlotOriginStrMap, hashSlotsDoc ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):LoadHashSlotMap: Successfully loaded hash slot map" );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):LoadHashSlotMap: Failed to parse hash slot map document" );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):LoadHashSlotMap: Failed to load hash slot map document" );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::RefreshRedisNodePipes( void )
{GUCEF_TRACE;

    RedisNodeMap redisNodes;
    if ( GetRedisClusterSlots( redisNodes ) )
    {
        // Due to the way the Redis library API works we need the hash slot mapping to be able to create
        // a pipeline to each node since there is no API call to get a connection to a specific node
        // It is abstracted away from us. The hash map allows us to work around that.
        if ( m_hashSlotOriginStrMap.empty() )
        {
            if ( !LoadHashSlotMap() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):RefreshRedisNodePipes: Unable to lazy load the hash map, as such cannot generate strings to establish dedicated pipes to the nodes" );                
                return false;
            }
        }    
        
        RedisNodeMap::iterator i = redisNodes.begin();
        while ( i != redisNodes.end() ) 
        {
            RedisNodeWithPipe& node = m_redisNodesMap[ (*i).first ];
            node = (*i).second;

            TStringSet hashStrings = m_hashSlotOriginStrMap[ node.startSlot ];
            if ( !hashStrings.empty() )
            {
                try
                {
                    CORE::CString hashString = *hashStrings.begin();
                    sw::redis::StringView hashStringSV( hashString.C_String(), hashString.Length() );
                   
                    delete node.redisPipe;                    
                    node.redisPipe = GUCEF_NULL;
                    node.redisPipe = new sw::redis::Pipeline( m_redisContext->pipeline( hashStringSV ) );
                }
                catch ( const sw::redis::Error& e )
                {
                    GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RefreshRedisNodePipes: Redis++ exception: " + e.what() );
                    ++m_redisClusterErrorReplies;
                    RedisDisconnect();
                    return false;
                }
                catch ( const std::exception& e )
                {
                    GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RefreshRedisNodePipes: exception: " + e.what() );
                    return false;
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):RefreshRedisNodePipes: Unable to find a hash slot entry for node " + 
                    node.nodeId + " with start slot " + CORE::ToString( node.startSlot ) + ", as such cannot generate string to establish a dedicated pipe to the node" );
            }
            ++i;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::ProvideRedisNodesDoc( void )
{GUCEF_TRACE;

    static const CORE::CString redisNodesCodec = "json";     
    const CORE::CString redisNodesFile = "InstallPath/" + m_settings.clusterName + "/RedisNodes.v1.json";
    
    RedisNodeMap redisNodes;
    if ( GetRedisClusterSlots( redisNodes ) )
    {
        CORE::CDataNode clusterNodesDoc;
        if ( SerializeRedisClusterNodeMap( redisNodes, clusterNodesDoc ) )
        {
            return SaveDocTo( clusterNodesDoc, redisNodesCodec, redisNodesFile );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    delete m_streamIndexingTimer;
    m_streamIndexingTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );

    if ( m_settings.streamIndexingInterval > 0 && IsStreamIndexingNeeded() )
    {
        m_streamIndexingTimer->SetInterval( (CORE::UInt32) m_settings.streamIndexingInterval );
    }

    delete m_redisReconnectTimer;
    m_redisReconnectTimer = new CORE::CTimer( *GetPulseGenerator(), 100 );
    
    delete m_metricsTimer;
    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
    m_metricsTimer->SetEnabled( m_settings.collectMetrics );
    
    RegisterEventHandlers();

    //LoadHashSlotMap();

    // Setup connection to Redis
    // Note that if there is an error here we will just keep on trying automatically
    RedisConnect();
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
    {
        m_redisReconnectTimer->SetEnabled( true );
    }
    
    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::OnTaskEnding( CORE::CICloneable* taskdata ,
                                bool willBeForced           )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::OnTaskEnded( CORE::CICloneable* taskData ,
                               bool wasForced              )
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    delete m_redisReconnectTimer;
    m_redisReconnectTimer = GUCEF_NULL;

    CORE::CTaskConsumer::OnTaskEnded( taskData, wasForced );
}

/*-------------------------------------------------------------------------*/

RedisNode::RedisNode( void )
    : host()
    , nodeId()
    , startSlot( 0 )
    , endSlot( 0 )
    , replicas()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

RedisNode& 
RedisNode::operator=( const RedisNode& other )
{GUCEF_TRACE;

    if ( this != &other )
    {
        host = other.host;
        nodeId = other.nodeId;
        startSlot = other.startSlot;
        endSlot = other.endSlot;
        replicas = other.replicas;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

RedisNode* 
RedisNode::FindReplica( const CORE::CString& replicaNodeId ,
                        const CORE::CString& parentNodeId  ,
                        bool createIfNotFound              )
{GUCEF_TRACE;

    if ( nodeId == parentNodeId )
    {
        RedisNodeVector::iterator i = replicas.begin();
        while ( i != replicas.end() )
        {
            if ( (*i).nodeId == replicaNodeId )
                return &(*i);
            ++i;
        }
        if ( createIfNotFound )
        {
            RedisNode newReplica;
            newReplica.nodeId = replicaNodeId;
            newReplica.startSlot = startSlot;
            newReplica.endSlot = endSlot;
            replicas.push_back( newReplica );
            return &replicas.back();
        }
        return GUCEF_NULL;
    }
    else
    {
        RedisNodeVector::iterator i = replicas.begin();
        while ( i != replicas.end() )
        {
            RedisNode* replica = (*i).FindReplica( replicaNodeId, parentNodeId, createIfNotFound );
            if ( GUCEF_NULL != replica )
                return replica;
            ++i;
        }
        return GUCEF_NULL;
    }
}

/*-------------------------------------------------------------------------*/

RedisNodeWithPipe::RedisNodeWithPipe( void )
    : RedisNode()
    , redisPipe( GUCEF_NULL ) 
    , redisErrorReplies( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

RedisNodeWithPipe& 
RedisNodeWithPipe::operator=( const RedisNode& other )
{GUCEF_TRACE;

    if ( this != &other )
    {
        RedisNode::operator=( other );
        //redisPipe
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
RedisNodeWithPipe::GetRedisErrorRepliesCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 nodeRedisErrorReplies = redisErrorReplies;
        redisErrorReplies = 0;
        return nodeRedisErrorReplies;
    }
    else
        return redisErrorReplies;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
RedisInfoService::GetRedisClusterErrorRepliesCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 redisErrorReplies = m_redisClusterErrorReplies;
        m_redisClusterErrorReplies = 0;
        return redisErrorReplies;
    }
    else
        return m_redisClusterErrorReplies;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoKeyspace( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "info" );
    static CORE::CString typeParam( "keyspace" );

    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisInfo( cmdParam, typeParam, kv, &(*i).second ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoPersistence( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "info" );
    static CORE::CString typeParam( "persistence" );

    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisInfo( cmdParam, typeParam, kv, &(*i).second ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoReplication( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "info" );
    static CORE::CString typeParam( "replication" );

    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisInfo( cmdParam, typeParam, kv, &(*i).second ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoStats( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "info" );
    static CORE::CString typeParam( "stats" );

    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisInfo( cmdParam, typeParam, kv, &(*i).second ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoCommandStats( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "info" );
    static CORE::CString typeParam( "commandstats" );

    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisInfo( cmdParam, typeParam, kv, &(*i).second ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoClients( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "info" );
    static CORE::CString typeParam( "clients" );

    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisInfo( cmdParam, typeParam, kv, &(*i).second ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoMemory( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "info" );
    static CORE::CString typeParam( "memory" );

    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisInfo( cmdParam, typeParam, kv, &(*i).second ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoCpu( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "info" );
    static CORE::CString typeParam( "cpu" );
    
    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisInfo( cmdParam, typeParam, kv, &(*i).second ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisClusterInfo( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString cmdParam( "cluster" );
    static CORE::CString typeParam( "info" );
    
    return GetRedisInfo( cmdParam, typeParam, kv );
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfo( struct redisReply* reply       ,
                                const CORE::CString& type      , 
                                CORE::CValueList& kv           ,
                                const CORE::CString& keyPrefix )
{GUCEF_TRACE;

    if ( GUCEF_NULL == reply )
        return false;

    if ( REDIS_REPLY_STRING == reply->type )
    {
        CORE::CString replyStr = reply->str;
        replyStr = replyStr.ReplaceSubstr( "\r\n", "\n" );

        CORE::CValueList kvCombined;
        kvCombined.SetMultiple( replyStr, '\n', ':' );

        CORE::CValueList::TValueMap::const_iterator i = kvCombined.GetDataBeginIterator();
        while ( i != kvCombined.GetDataEndIterator() )
        {
            if ( !(*i).second.empty() )
            {
                const CORE::CString& key = (*i).first;
                const CORE::CString& comboValue = (*i).second.front();
                CORE::CString::StringVector values = comboValue.ParseElements( ',', false );
                if ( values.size() == 1 )
                {
                    // Filter anything labeled as for human viewing since its redundant plus 
                    // we are all about automation in this code
                    if ( -1 == key.HasSubstr( "_human", false ) )
                        kv.Set( keyPrefix + key, comboValue );
                }
                else
                if ( values.size() > 1 )
                {
                    CORE::CString extraKeyPrefix = keyPrefix + key + '_';
                    CORE::CString::StringVector::iterator n = values.begin();
                    while ( n != values.end() )
                    {
                        // Filter anything labeled as for human viewing since its redundant plus 
                        // we are all about automation in this code
                        if ( -1 == (*n).HasSubstr( "_human", false ) )
                            kv.Set( (*n), '=', &extraKeyPrefix );
                        ++n;
                    }
                }
            }
            ++i;
        }
        return true;
    }

    if ( REDIS_REPLY_ERROR == reply->type )
    {
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfo( const CORE::CString& cmd  ,
                                const CORE::CString& type , 
                                CORE::CValueList& kv      ,
                                RedisNodeWithPipe* node   ) 
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
        return false;
    
    try
    {
        sw::redis::StringView infoCmdSV( cmd.C_String(), cmd.Length() );
        sw::redis::StringView typeParamSV( type.C_String(), type.Length() );

        if ( GUCEF_NULL != node )
        {
            node->redisPipe->command( infoCmdSV, typeParamSV );
            sw::redis::QueuedReplies redisReplies = node->redisPipe->exec();
            size_t replyCount = redisReplies.size();
            if ( replyCount > 0 )
            {
                redisReply& reply = redisReplies.get( 0 );
                return GetRedisInfo( &reply, type, kv, node->nodeId + '.' );
            }
        }
        else
        {
            auto reply = m_redisContext->command( infoCmdSV, typeParamSV );
            return GetRedisInfo( reply.get(), type, kv, CORE::CString::Empty );
        }
    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: Redis++ exception: " + e.what() );
        if ( GUCEF_NULL != node )
            ++node->redisErrorReplies;
        RedisDisconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: exception: " + e.what() );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisStreamInfo( struct redisReply* replyNode           ,
                                      CORE::CValueList& info                 ,
                                      const CORE::CString& optionalKeyPrefix ,
                                      bool statLikeValuesOnly                )
{GUCEF_TRACE;

    if ( GUCEF_NULL != replyNode )
    {
        int type = replyNode->type;
        if ( REDIS_REPLY_ARRAY == type )
        {
            size_t i=0;
            size_t infoCount = replyNode->elements;
            while ( i < infoCount ) 
            {
                CORE::CString infoElementName;
                if ( REDIS_REPLY_STRING == replyNode->element[ i ]->type )
                {
                    infoElementName = optionalKeyPrefix + replyNode->element[ i ]->str;
                    ++i;

                    switch( replyNode->element[ i ]->type )
                    {
                        case REDIS_REPLY_VERB:
                        case REDIS_REPLY_STATUS:
                        case REDIS_REPLY_STRING: 
                        { 
                            if ( !statLikeValuesOnly )
                                info.Set( infoElementName, replyNode->element[ i ]->str ); 
                            break; 
                        }
                        case REDIS_REPLY_ERROR:
                        case REDIS_REPLY_INTEGER: 
                        { 
                            info.Set( infoElementName, CORE::ToString( replyNode->element[ i ]->integer ) ); 
                            break; 
                        }
                        case REDIS_REPLY_DOUBLE: 
                        { 
                            info.Set( infoElementName, CORE::ToString( replyNode->element[ i ]->dval ) ); 
                            break; 
                        }
                        case REDIS_REPLY_BOOL: 
                        { 
                            if ( !statLikeValuesOnly )
                                info.Set( infoElementName, CORE::ToString( CORE::StringToBool( replyNode->element[ i ]->str ) ) ); 
                            break; 
                        }
                        case REDIS_REPLY_ARRAY: 
                        { 
                            return GetRedisStreamInfo( replyNode->element[ i ], info, optionalKeyPrefix, statLikeValuesOnly ); 
                        }
                        default:
                            break;
                    }
                }
                else
                    break;                        
                ++i;
            }
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

RedisNodeWithPipe*
RedisInfoService::FindNodeInfo( CORE::UInt32 hashSlot )
{GUCEF_TRACE;

    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {           
        RedisNodeWithPipe& node = (*i).second;
        if ( node.startSlot <= hashSlot && node.endSlot >= hashSlot ) 
            return &node;
        if ( node.endSlot > hashSlot )
            break;
        ++i;
    }
    
    return GUCEF_NULL;        
}

/*-------------------------------------------------------------------------*/

RedisNodeWithPipe*
RedisInfoService::FindNodeInfo( const CORE::CString& hashable )
{GUCEF_TRACE;

    CORE::UInt32 hashSlot = CalculateRedisHashSlot( hashable );    
    return FindNodeInfo( hashSlot );
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisStreamInfo( const CORE::CString& streamName        ,
                                      CORE::CValueList& info                 ,
                                      const CORE::CString& optionalKeyPrefix ,
                                      bool statLikeValuesOnly                )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
        return false;
    
    RedisNodeWithPipe* node = FindNodeInfo( streamName );
    if ( GUCEF_NULL == node )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisStreamInfo: No known node could house the hash slot for stream \"" + streamName + "\"" );
        return false;
    }

    try
    {
        CORE::CString xinfoCmd( "XINFO" );
        CORE::CString streamParam( "STREAM" );

        sw::redis::StringView xinfoCmdSV( xinfoCmd.C_String(), xinfoCmd.Length() );
        sw::redis::StringView streamParamSV( streamParam.C_String(), streamParam.Length() );
        sw::redis::StringView streamNameParamSV( streamName.C_String(), streamName.Length() );

        node->redisPipe->command( xinfoCmdSV, streamParamSV, streamNameParamSV );
        sw::redis::QueuedReplies redisReplies = node->redisPipe->exec();
        size_t replyCount = redisReplies.size();
        if ( replyCount > 0 )
        {
            redisReply& reply = redisReplies.get( 0 );
            return GetRedisStreamInfo( &reply, info, optionalKeyPrefix, statLikeValuesOnly );
        }

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisStreamInfo: Obtained " + CORE::ToString( info.GetCount() ) + " pieces of information" );
    }    
    catch ( const sw::redis::MovedError& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisStreamInfo: Redis++ Moved exception: " + e.what() );
        if ( GUCEF_NULL != node )
            ++node->redisErrorReplies;
        RedisDisconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisStreamInfo: Redis++ exception: " + e.what() );
        if ( GUCEF_NULL != node )
            ++node->redisErrorReplies;
        RedisDisconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisStreamInfo: exception: " + e.what() );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisStreamInfoForAllStreams( CORE::CValueList& info  ,
                                                   bool statLikeValuesOnly )
{GUCEF_TRACE;

    bool totalSuccess = true;
    CORE::CString::StringVector::iterator i = m_filteredStreamNames.begin();
    while ( i != m_filteredStreamNames.end() )
    {
        CORE::CString keyPrefix = (*i).ReplaceChar( '.', '_' ).ReplaceChar( '{', '_' ).ReplaceChar( '}', '_' ) + '.';
        totalSuccess = GetRedisStreamInfo( (*i), info, keyPrefix, statLikeValuesOnly ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisKeysForNode( RedisNodeWithPipe& node           ,
                                       CORE::CString::StringVector& keys ,
                                       const CORE::CString& keyType      )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext || GUCEF_NULL == node.redisPipe )
        return false;
    
    CORE::UInt32 keysFound = 0;
    try
    {
        static const CORE::CString scanCmd( "SCAN" );
        CORE::CString itteratorParam( "0" );
        static const CORE::CString typeParam( "TYPE" );

        sw::redis::StringView scanCmdSV( scanCmd.C_String(), scanCmd.Length() );        
        do
        {
            sw::redis::StringView itteratorParamSV( itteratorParam.C_String(), itteratorParam.Length() );
            sw::redis::StringView typeParamSV( typeParam.C_String(), typeParam.Length() );
            sw::redis::StringView typeValueParamSV( keyType.C_String(), keyType.Length() );

            node.redisPipe->command( scanCmdSV, itteratorParamSV, typeParamSV, typeValueParamSV );
            sw::redis::QueuedReplies redisReplies = node.redisPipe->exec();
            size_t replyCount = redisReplies.size();
            for ( size_t r=0; r<replyCount; ++r )
            {
                redisReply& reply = redisReplies.get( r );
                if ( REDIS_REPLY_ARRAY == reply.type )
                {
                    size_t ittCount = reply.elements;

                    if ( ittCount > 0 && REDIS_REPLY_STRING == reply.element[ 0 ]->type )
                        itteratorParam = reply.element[ 0 ]->str;
                    else
                        return false;

                    if ( ittCount > 1 && REDIS_REPLY_ARRAY == reply.element[ 1 ]->type )
                    {
                        size_t keyCount = reply.element[ 1 ]->elements;
                        struct redisReply** keyList = reply.element[ 1 ]->element;
                        for ( size_t n=0; n<keyCount; ++n )
                        {
                            if ( REDIS_REPLY_STRING == keyList[ n ]->type )
                            {
                                keys.push_back( keyList[ n ]->str );
                                ++keysFound;
                            }
                        }
                    }
                    else
                        return false;
                }
            }
        }
        while ( itteratorParam != "0" );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisKeys: Found " + CORE::ToString( keysFound ) + " keys for Redis node " + node.nodeId );
    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisKeys: Redis Node " + node.nodeId + " : Redis++ exception: " + e.what() );
        ++node.redisErrorReplies;
        RedisDisconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisKeys: Redis Node " + node.nodeId + " : exception: " + e.what() );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisKeys( CORE::CString::StringVector& keys ,
                                const CORE::CString& keyType      )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
        return false;

    bool totalSuccess = true;
    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        totalSuccess = GetRedisKeysForNode( (*i).second, keys, keyType ) && totalSuccess;        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisClusterNodeMap( RedisNodeMap& nodeMap )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
        return false;
    
    try
    {
        CORE::CString clusterCmd( "CLUSTER" );
        CORE::CString nodesParam( "NODES" );

        sw::redis::StringView clusterCmdSV( clusterCmd.C_String(), clusterCmd.Length() );
        sw::redis::StringView nodesParamSV( nodesParam.C_String(), nodesParam.Length() );

        auto reply = m_redisContext->command( clusterCmdSV, nodesParamSV );
        if ( reply )
        {
            if ( REDIS_REPLY_STRING == reply->type )
            {
                CORE::CString allText = reply->str;
                CORE::CString::StringVector lines = allText.ParseElements( '\n', false );
                
                // First add all the primary nodes
                CORE::CString::StringVector::iterator i = lines.begin();
                while ( i != lines.end() )
                {
                    CORE::CString::StringVector properties = (*i).ParseElements( ' ', true );
                    if ( properties.size() >= 8 )
                    {                  
                        bool isReplica = properties[ 2 ].HasSubstr( "slave" ) > -1;
                        if ( !isReplica )
                        {
                            CORE::CString& nodeId = properties[ 0 ];
                            CORE::CString ipAndPort = properties[ 1 ].SubstrToChar( '@' );
                            //CORE::CString& parentNodeId = properties[ 3 ];
                            //bool isConnected = properties[ 7 ] == "connected";

                            CORE::Int32 startSlot = -1;
                            CORE::Int32 endSlot = -1;
                            if ( properties.size() >= 9 )
                            {
                                startSlot = CORE::StringToInt32( properties[ 8 ].SubstrToChar( '-', true ), -1 );
                                endSlot = CORE::StringToInt32( properties[ 8 ].SubstrToChar( '-', false ), -1 );
                            }

                            RedisNode& node = nodeMap[ startSlot ];
                            node.nodeId = nodeId;
                            node.host.SetHostnameAndPort( ipAndPort );
                            node.startSlot = startSlot;
                            node.endSlot = endSlot; 
                        }
                    }
                    ++i;
                }
                
                // Now add the replicas
                i = lines.begin();
                while ( i != lines.end() )
                {
                    CORE::CString::StringVector properties = (*i).ParseElements( ' ', false );
                    if ( properties.size() >= 8 )
                    {
                        bool isReplica = properties[ 2 ].HasSubstr( "slave" ) > -1;
                        if ( isReplica )
                        {
                            CORE::CString& nodeId = properties[ 0 ];
                            CORE::CString ipAndPort = properties[ 1 ].SubstrToChar( '@' );                    
                            CORE::CString& parentNodeId = properties[ 3 ];
                            //bool isConnected = properties[ 7 ] == "connected";

                            RedisNodeMap::iterator n = nodeMap.begin();
                            while ( n != nodeMap.end() ) 
                            {
                                RedisNode* replicaNode = (*n).second.FindReplica( nodeId, parentNodeId, true );
                                if ( GUCEF_NULL != replicaNode )
                                {
                                    replicaNode->host.SetHostnameAndPort( ipAndPort );
                                    break;
                                }
                                ++n;
                            }
                        }
                    }
                    ++i;
                }
            }
        }
        return true;
    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: Redis++ exception: " + e.what() );
        ++m_redisClusterErrorReplies;
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: exception: " + e.what() );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisClusterSlots( RedisNodeMap& nodeMap )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
        return false;
    
    try
    {
        CORE::CString clusterCmd( "CLUSTER" );
        CORE::CString slotsParam( "SLOTS" );

        sw::redis::StringView clusterCmdSV( clusterCmd.C_String(), clusterCmd.Length() );
        sw::redis::StringView slotsParamSV( slotsParam.C_String(), slotsParam.Length() );

        auto reply = m_redisContext->command( clusterCmdSV, slotsParamSV );
        if ( reply )
        {
            int type = reply->type;
            if ( REDIS_REPLY_ARRAY == type )
            {
                size_t max = reply->elements;
                for ( size_t i=0; i<max; ++i )
                {
                    const struct redisReply* e = reply->element[ i ];
                    int eType = e->type;
                    if ( REDIS_REPLY_ARRAY == eType )
                    {
                        size_t eMax = e->elements;

                        long long startSlot = -1;
                        if ( eMax > 0 && REDIS_REPLY_INTEGER == e->element[ 0 ]->type )
                            startSlot = e->element[ 0 ]->integer;
                        else
                            return false;

                        long long endSlot = -1;
                        if ( eMax > 1 && REDIS_REPLY_INTEGER == e->element[ 1 ]->type )
                            endSlot = e->element[ 1 ]->integer;
                        else
                            return false;

                        for ( size_t m=2; m<eMax; ++m )
                        {
                            const struct redisReply* slotSegment = e->element[ m ];
                            int sType = slotSegment->type;
                            if ( REDIS_REPLY_ARRAY == sType )
                            {
                                size_t slotSegAttCount = slotSegment->elements;

                                CORE::CString ip;
                                if ( slotSegAttCount > 0 && REDIS_REPLY_STRING == slotSegment->element[ 0 ]->type )
                                    ip = slotSegment->element[ 0 ]->str;
                                else
                                    return false;

                                long long port = -1;
                                if ( slotSegAttCount > 1 && REDIS_REPLY_INTEGER == slotSegment->element[ 1 ]->type )
                                    port = slotSegment->element[ 1 ]->integer;
                                else
                                    return false;

                                // Read the optional Node ID if present. This depends on the Redis version
                                CORE::CString nodeId;
                                if ( slotSegAttCount > 2 && REDIS_REPLY_STRING == slotSegment->element[ 2 ]->type )
                                    nodeId = slotSegment->element[ 2 ]->str;

                                RedisNode& node = nodeMap[ (CORE::UInt32) startSlot ];
                                node.startSlot = (CORE::UInt32) startSlot;
                                node.endSlot = (CORE::UInt32) endSlot;
                                node.nodeId = nodeId;
                                node.host.SetHostnameAndPort( ip, port );
                            }
                        }
                    }
                }
            }
            else
            {
                if ( REDIS_REPLY_ERROR == type )
                {
                    ++m_redisClusterErrorReplies;
                    return false;
                }
            }
        }

    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisClusterSlots: Redis++ exception: " + e.what() );
        ++m_redisClusterErrorReplies;
        RedisDisconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisClusterSlots: exception: " + e.what() );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
RedisInfoService::SerializeRedisClusterNodeMap( const RedisNodeMap& nodeMap, CORE::CDataNode& doc ) const
{GUCEF_TRACE;

    doc.Clear();
    doc.SetName( "RedisClusterNodes" );
    doc.SetNodeType( GUCEF_DATATYPE_ARRAY );
    RedisNodeMap::const_iterator i = nodeMap.begin();
    while ( i != nodeMap.end() )
    {        
        CORE::CDataNode* redisNodeEntry = doc.AddChild( "node", GUCEF_DATATYPE_OBJECT );
        redisNodeEntry->SetAttribute( "id", (*i).second.nodeId, GUCEF_DATATYPE_STRING );
        redisNodeEntry->SetAttribute( "startSlot", CORE::ToString( (*i).second.startSlot ), GUCEF_DATATYPE_UINT32 );
        redisNodeEntry->SetAttribute( "endSlot", CORE::ToString( (*i).second.endSlot ), GUCEF_DATATYPE_UINT32 );
        redisNodeEntry->SetAttribute( "host", (*i).second.host.GetHostname(), GUCEF_DATATYPE_STRING );
        redisNodeEntry->SetAttribute( "port", (*i).second.host.PortAsString(), GUCEF_DATATYPE_STRING );
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::RedisDisconnect( void )
{GUCEF_TRACE;

    try
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisDisconnect: Beginning cleanup" );

        RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
        while ( i != m_redisNodesMap.end() )
        {
            sw::redis::Pipeline* redisPipe = (*i).second.redisPipe;
            delete redisPipe;
            redisPipe = GUCEF_NULL;
            ++i;
        }
        m_redisNodesMap.clear();
        
        delete m_redisContext;
        m_redisContext = GUCEF_NULL;

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisDisconnect: Finished cleanup" );
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisClusterErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisDisconnect: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisClusterErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisDisconnect: Redis++ exception: " + e.what() );
        RedisDisconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisDisconnect: exception: " + e.what() );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::RedisConnect( void )
{GUCEF_TRACE;

    RedisDisconnect();
    
    try
    {
        sw::redis::ConnectionOptions rppConnectionOptions;
        rppConnectionOptions.host = m_settings.redisAddress.GetHostname();  // Required.
        rppConnectionOptions.port = m_settings.redisAddress.GetPortInHostByteOrder(); // Optional. The default port is 6379.
        //rppConnectionOptions.password = "auth";   // Optional. No password by default.
        //rppConnectionOptions.db = 1;  // Optional. Use the 0th database by default.

        // Optional. Timeout before we successfully send request to or receive response from redis.
        // By default, the timeout is 0ms, i.e. never timeout and block until we send or receive successfuly.
        // NOTE: if any command is timed out, we throw a TimeoutError exception.
        rppConnectionOptions.socket_timeout = std::chrono::milliseconds( 100 );
        rppConnectionOptions.connect_timeout = std::chrono::milliseconds( 100 );

        rppConnectionOptions.keep_alive = true;

        // Connect to the Redis cluster
        delete m_redisContext;
        m_redisContext = new sw::redis::RedisCluster( rppConnectionOptions );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisConnect: Successfully created a Redis context" );

        ProvideRedisNodesDoc();
        RefreshRedisNodePipes();
        
        if ( m_settings.gatherStreamInfo ) 
        {
            if ( IsStreamIndexingNeeded() )
            {
                m_streamIndexingTimer->TriggerNow();
                m_streamIndexingTimer->SetEnabled( true );
            }
            else
            {
                m_filteredStreamNames = m_settings.streamsToGatherInfoFrom;
                if ( m_filteredStreamNames.empty() )
                {
                    // One-time fetch of all stream names as a fallback for no stream name or patterns having been specified
                    GetRedisKeys( m_filteredStreamNames, "stream" );
                }
            }
        }
        return true;
    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisConnect: Redis++ exception: " + e.what() );
        ++m_redisClusterErrorReplies;
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisConnect: exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::SendKeyValueStats( const CORE::CValueList& kv        , 
                                     const CORE::CString& metricPrefix )
{GUCEF_TRACE;

    CORE::CValueList::TValueMap::const_iterator i = kv.GetDataBeginIterator();
    while ( i != kv.GetDataEndIterator() )
    {
        if ( !(*i).second.empty() )
        {
            const CORE::CString& key = (*i).first;
            const CORE::CString& value = (*i).second.front();

            //if ( 0 != CORE::IsANumber( value.C_String() ) )
            {
                if ( -1 != value.HasChar( '.' ) )
                {
                    GUCEF_METRIC_GAUGE( metricPrefix + key, CORE::StringToDouble( value ), 1.0f );
                }
                else
                {
                    GUCEF_METRIC_GAUGE( metricPrefix + key, CORE::StringToInt64( value ), 1.0f );
                }
            }
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::IsStreamIndexingNeeded( void ) const
{GUCEF_TRACE;

    CORE::CString::StringVector::const_iterator i = m_settings.streamsToGatherInfoFrom.begin();
    while ( i != m_settings.streamsToGatherInfoFrom.end() )
    {
        if ( (*i).HasChar( '*' ) > -1 )
        {
            return true;
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::OnStreamIndexingTimerCycle( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_settings.streamIndexingInterval <= 0 )
        return;
    
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):OnStreamIndexingTimerCycle: Attempting to scan all keys in the target node for matches to the configured streams names with wildcards" );
    
    // First we will need to fetch all stream names in the cluster
    // Redis does not support a wildcard filter for this so we have to fetch everything
    CORE::CString::StringVector allStreamNames;
    if ( GetRedisKeys( allStreamNames, "stream" ) )
    {
        CORE::CString::StringVector filteredStreamNames;
        CORE::CString::StringVector::iterator i = m_settings.streamsToGatherInfoFrom.begin();
        while ( i != m_settings.streamsToGatherInfoFrom.end() )
        {
            if ( (*i).HasChar( '*' ) > -1 )
            {
                CORE::CString::StringVector::iterator n = allStreamNames.begin();
                while ( n != allStreamNames.end() )
                {
                    if ( (*n).WildcardEquals( (*i), '*', true ) )
                    {
                        filteredStreamNames.push_back( (*n) );
                    }
                    ++n;
                }
            }
            ++i;
        }
        m_filteredStreamNames = filteredStreamNames;

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisInfoService(" + CORE::PointerToString( this ) + "):OnStreamIndexingTimerCycle: Downloaded and found " + 
            CORE::ToString( allStreamNames.size() ) + " streams. Filtered those using wilcard pattern matching down to " + CORE::ToString( m_filteredStreamNames.size() ) +
            " streams which we will collect metric for"  );
    }
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::OnRedisReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    m_redisReconnectTimer->SetEnabled( false );

    if ( !RedisConnect() )
    {
        m_redisReconnectTimer->SetEnabled( true );
    }
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::SendErrorReplyStats( const CORE::CString& metricPrefix )
{GUCEF_TRACE;

    CORE::CString clusterMetricPrefix = metricPrefix + "ClusterErrorReplies";
    CORE::UInt32 clusterErrorReplyCount = GetRedisClusterErrorRepliesCounter( true );
    GUCEF_METRIC_COUNT( clusterMetricPrefix, clusterErrorReplyCount, 1.0f );

    RedisNodeWithPipeMap::iterator i = m_redisNodesMap.begin();
    while ( i != m_redisNodesMap.end() )
    {
        RedisNodeWithPipe& node = (*i).second;
        CORE::CString nodeMetricPrefix = metricPrefix + node.nodeId + ".ErrorReplies";
        CORE::UInt32 nodeErrorReplyCount = node.GetRedisErrorRepliesCounter( true );
        GUCEF_METRIC_COUNT( nodeMetricPrefix, nodeErrorReplyCount, 1.0f );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_settings.gatherClusterInfo )
    {
        CORE::CValueList kv;
        if ( GetRedisClusterInfo( kv ) )
        {
            // Cluster info is all numeric except for the cluster state
            // We turn the state into a number here so we can sent it as a stat
            static const CORE::CString clusterStateStatName = "cluster_state";
            CORE::CString clusterStateStr = kv.GetValueAlways( clusterStateStatName );
            if ( !clusterStateStr.IsNULLOrEmpty() )
            {
                if ( "ok" == clusterStateStr )
                {
                    static const CORE::CString clusterStateStatOk = "1";
                    kv.Set( clusterStateStatName, clusterStateStatOk );
                }
                else
                {
                    static const CORE::CString clusterStateStatFail = "0";
                    kv.Set( clusterStateStatName, clusterStateStatFail );
                }
            }
            CORE::CString metricPrefix = m_settings.metricPrefix + "ClusterInfo.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoCommandStats )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoCommandStats( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "CommandStats.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoMemory )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoMemory( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "Memory.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoReplication )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoReplication( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "Replication.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoPersistence )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoPersistence( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "Persistence.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoCommandStats )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoStats( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "Stats.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoClients )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoClients( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "Clients.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoCpu )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoCpu( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "Cpu.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoKeyspace )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoKeyspace( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "Keyspace.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherStreamInfo )
    {
        CORE::CValueList kv;
        if ( GetRedisStreamInfoForAllStreams( kv, true ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + "StreamInfo.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherErrorReplyCount )
    {
        SendErrorReplyStats( m_settings.metricPrefix );
    }
}

/*-------------------------------------------------------------------------*/

RestApiRedisInfoInfoResource::RestApiRedisInfoInfoResource( RedisInfo* app )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiRedisInfoInfoResource::~RestApiRedisInfoInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiRedisInfoInfoResource::Serialize( CORE::CDataNode& output             ,
                                         const CORE::CString& representation ,
                                         const CORE::CString& params         )
{GUCEF_TRACE;

    output.Clear();
    output.SetName( "info" );
    output.SetNodeType( GUCEF_DATATYPE_OBJECT );
    output.SetAttribute( "application", "redisinfo", GUCEF_DATATYPE_STRING );
    output.SetAttribute( "buildDateTime", __TIMESTAMP__ , GUCEF_DATATYPE_STRING );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false", GUCEF_DATATYPE_BOOLEAN_STRING );
    #else
    output.SetAttribute( "isReleaseBuild", "true", GUCEF_DATATYPE_BOOLEAN_STRING );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiRedisInfoConfigResource::RestApiRedisInfoConfigResource( RedisInfo* app, bool appConfig )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
    m_allowDeserialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiRedisInfoConfigResource::~RestApiRedisInfoConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiRedisInfoConfigResource::Serialize( CORE::CDataNode& output             ,
                                           const CORE::CString& representation ,
                                           const CORE::CString& params         )
{GUCEF_TRACE;

    if ( m_appConfig )
    {
        const CORE::CValueList& loadedConfig = m_app->GetAppConfig();
        return loadedConfig.SaveConfig( output );
    }
    else
    {
        const CORE::CDataNode& globalConfig = m_app->GetGlobalConfig();
        output.Copy( globalConfig );
        return true;
    }
}

/*-------------------------------------------------------------------------*/

RestApiRedisInfoConfigResource::TDeserializeState
RestApiRedisInfoConfigResource::Deserialize( const CORE::CDataNode& input        ,
                                             const CORE::CString& representation ,
                                             bool isDeltaUpdateOnly              )
{GUCEF_TRACE;

    if ( m_appConfig )
    {
        CORE::CValueList loadedAppConfig;

        if ( isDeltaUpdateOnly )
        {
            // Grab a copy of the current app config
            loadedAppConfig = m_app->GetAppConfig();
            loadedAppConfig.SetAllowMultipleValues( false );
            loadedAppConfig.SetAllowDuplicates( false );
        }
        else
        {
            loadedAppConfig.CopySettingsFrom( m_app->GetAppConfig() );
        }

        if ( loadedAppConfig.LoadConfig( input ) )
        {
            if ( isDeltaUpdateOnly )
            {
                loadedAppConfig.SetAllowMultipleValues( m_app->GetAppConfig().GetAllowMultipleValues() );
                loadedAppConfig.SetAllowDuplicates( m_app->GetAppConfig().GetAllowDuplicates() );
            }

            // First put the app in standby mode before we mess with the settings
            if ( !m_app->SetStandbyMode( true ) )
                return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;

            const CORE::CDataNode& globalConfig = m_app->GetGlobalConfig();
            if ( m_app->LoadConfig( loadedAppConfig, globalConfig ) )
            {
                if ( !m_app->IsGlobalStandbyEnabled() )
                {
                    if ( m_app->SetStandbyMode( false ) )
                        return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
                    else
                        return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
                }
                else
                {
                    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RestApiRedisInfoConfigResource: IsGlobalStandbyEnabled is true. We will leave the app in standby mode" );
                    return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
                }
            }
            else
            {
                return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
            }
        }

        return TDeserializeState::DESERIALIZESTATE_CORRUPTEDINPUT;
    }
    else
    {
        if ( isDeltaUpdateOnly )
        {
            //// Grab a copy of the current global config
            //CORE::CDataNode globalConfigCopy = m_app->GetGlobalConfig();
            //if ( globalConfigCopy.Merge( input ) )
            //{
            //    const CORE::CValueList& loadedAppConfig = m_app->GetAppConfig();
            //    if ( m_app->LoadConfig( loadedAppConfig, globalConfigCopy ) )
            //    {
            //        return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
            //    }
            //    else
            //    {
            //        return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
            //    }
            //}

            return TDeserializeState::DESERIALIZESTATE_CORRUPTEDINPUT;
        }
        else
        {
            const CORE::CValueList& loadedAppConfig = m_app->GetAppConfig();
            if ( m_app->LoadConfig( loadedAppConfig, input ) )
            {
                return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
            }
            else
            {
                return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

RedisInfo::RedisInfo( void )
    : CORE::CObserver()
    , m_isInStandby( false )
    , m_globalStandbyEnabled( false )
    , m_redisHost()
    , m_redisPort()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_infoServices()
    , m_settings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

RedisInfo::~RedisInfo()
{GUCEF_TRACE;

    m_httpServer.Close();
}

/*-------------------------------------------------------------------------*/

bool
RedisInfo::IsGlobalStandbyEnabled( void ) const
{GUCEF_TRACE;

    return m_globalStandbyEnabled;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfo::Start( void )
{GUCEF_TRACE;

    m_isInStandby = true;
    bool success = SetStandbyMode( m_globalStandbyEnabled );

    if ( success )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo: Opening REST API" );
        if ( m_httpServer.Listen() )
        {
            success = true;
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo: REST API port opened successfully" );
        }
        else
        {
            success = false;
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "RedisInfo: Unable to open REST API port (port in use?)" );
        }
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfo::SetStandbyMode( bool putInStandbyMode )
{GUCEF_TRACE;

    // Check if we need to do anything
    if ( m_isInStandby == putInStandbyMode )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo:SetStandbyMode( " + CORE::BoolToString( putInStandbyMode ) + " ): Already in the desired mode (" + CORE::BoolToString( m_isInStandby ) + "), nothing to do" );
        return true;
    }

    if ( putInStandbyMode )
    {
        bool totalSuccess = true;
        CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();

        // Signal all threads to stop gracefully

        TStringToInfoServiceMap::iterator i = m_infoServices.begin();
        while ( i != m_infoServices.end() )
        {
            if ( !taskManager.RequestTaskToStop( (*i).second.StaticCast< CORE::CTaskConsumer >(), true ) )
            {
                totalSuccess = false;
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo:SetStandbyMode( true ): Failed to signal task for redis info target \"" + (*i).first + "\" to stop" );
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo:SetStandbyMode( true ): Requested task for redis info target \"" + (*i).first + "\" to stop" );
            }
            ++i;
        }

        m_isInStandby = totalSuccess;
        return totalSuccess;
    }
    else
    {
        bool totalSuccess = true;

        CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();

        TStringToSettingsMap::iterator i = m_settings.begin();
        while ( i != m_settings.end() )
        {
            RedisInfoServicePtr redisInfoTarget( new RedisInfoService() );
            if ( redisInfoTarget->LoadConfig( (*i).second ) )
            {
                if ( taskManager.StartTask( redisInfoTarget ) )
                {
                    m_infoServices[ (*i).first ] = redisInfoTarget; 

                    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo::SetStandbyMode( false ): Successfully started task for redis info target \"" + (*i).second.clusterName + "\"" );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo::SetStandbyMode( false ): Failed to start task for redis info target \"" + (*i).second.clusterName + "\"" );
                    totalSuccess = false;
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo::SetStandbyMode( false ): Failed to set settings for task for redis info target \"" + (*i).second.clusterName + "\"" );
                totalSuccess = false;
            }
            ++i;
        }

        m_isInStandby = !totalSuccess;
        return totalSuccess;
    }
}

/*-------------------------------------------------------------------------*/

bool
RedisInfo::LoadConfig( const CORE::CValueList& appConfig   ,
                       const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    m_globalStandbyEnabled = CORE::StringToBool( appConfig.GetValueAlways( "GlobalStandbyEnabled" ), false );
    CORE::CDataNode::TDataNodeVector redisInfoTargets = globalConfig.SearchForAll( "RedisInfoTarget", '/', true, true );
    CORE::CDataNode::TDataNodeVector::iterator i = redisInfoTargets.begin();
    while ( i != redisInfoTargets.end() )
    {
        Settings redisTargetSettings;
        if ( redisTargetSettings.LoadConfig( *(*i) ) )
        {
            m_settings[ redisTargetSettings.clusterName ] = redisTargetSettings;
        }
        ++i;
    }

    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );

    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort" ) ), 10000 ) );

    m_httpRouter.SetResourceMapping( "/info", RestApiRedisInfoInfoResource::THTTPServerResourcePtr( new RestApiRedisInfoInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiRedisInfoConfigResource::THTTPServerResourcePtr( new RestApiRedisInfoConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiRedisInfoConfigResource::THTTPServerResourcePtr( new RestApiRedisInfoConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping( CORE::ResolveVars( appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ) ), RestApiRedisInfoInfoResource::THTTPServerResourcePtr( new WEB::CDummyHTTPServerResource() )  );

    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList&
RedisInfo::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
RedisInfo::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/

