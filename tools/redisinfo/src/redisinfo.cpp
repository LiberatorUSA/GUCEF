/*
 *  redisinfo: Service for obtaining redis info from a cluster
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

#include <string.h>

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefCOM_CDummyHTTPServerResource.h"
#define GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H ? */

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

Settings::Settings( void )
    : redisAddress()
    , collectMetrics( true )
    , metricPrefix()
    , gatherInfoReplication( true )
    , gatherInfoStats( true )
    , gatherInfoCommandStats( true )
    , gatherInfoMemory( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Settings::Settings( const Settings& src )
    : redisAddress( src.redisAddress )
    , collectMetrics( src.collectMetrics )
    , metricPrefix()
    , gatherInfoReplication( src.gatherInfoReplication )
    , gatherInfoStats( src.gatherInfoStats )
    , gatherInfoCommandStats( src.gatherInfoCommandStats )
    , gatherInfoMemory( src.gatherInfoMemory )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Settings&
Settings::operator=( const Settings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        redisAddress = src.redisAddress;
        collectMetrics = src.collectMetrics;
        metricPrefix = src.metricPrefix;
        gatherInfoReplication = src.gatherInfoReplication;
        gatherInfoStats = src.gatherInfoStats;
        gatherInfoCommandStats = src.gatherInfoCommandStats;
        gatherInfoMemory = src.gatherInfoMemory;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
Settings::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    tree.SetAttribute( "redisAddress", redisAddress.AddressAndPortAsString() );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
Settings::LoadConfig( const CORE::CDataNode& tree )
{GUCEF_TRACE;

    redisAddress.SetHostnameAndPort( tree.GetAttributeValueOrChildValueByName( "redisAddress", redisAddress.AddressAndPortAsString() ) );
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
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

RedisInfoService::~RedisInfoService()
{GUCEF_TRACE;

    delete m_redisContext;
    m_redisContext = GUCEF_NULL;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback1( this, &RedisInfoService::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback1                      );
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

CORE::CString
RedisInfoService::GetType( void ) const
{GUCEF_TRACE;

    return "RedisInfoService";
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::ProvideHashSlotMapDoc( void )
{GUCEF_TRACE;

    static const CORE::CString hashSlotFileCodec = "json"; 
    static const CORE::CString hashSlotFile = "InstallPath/RedisHashMap.v1.json";
   
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    if ( !vfs.FileExists( hashSlotFile ) )
    {
        TUInt32ToStringSetMap hashMap;
        if ( CalculateKeysForAllHashSlots( hashMap, 1, 1 ) )
        {
            CORE::CDataNode hashSlotsDoc;
            if ( SerializeKeysForHashSlots( hashMap, hashSlotsDoc ) )
            {
                return SaveDocTo( hashSlotsDoc, hashSlotFileCodec, hashSlotFile );
            }
        }
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::ProvideRedisNodesDoc( void )
{GUCEF_TRACE;

    static const CORE::CString redisNodesCodec = "json"; 
    static const CORE::CString redisNodesFile = "InstallPath/RedisNodes.v1.json";

    RedisNodeMap redisNodesMap;
    if ( GetRedisClusterNodeMap( redisNodesMap ) )
    {
        CORE::CDataNode clusterNodesDoc;
        if ( SerializeRedisClusterNodeMap( redisNodesMap, clusterNodesDoc ) )
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

    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
    m_metricsTimer->SetEnabled( m_settings.collectMetrics );
    m_metricsTimer->SetInterval( 1000 );

    RegisterEventHandlers();

    ProvideHashSlotMapDoc();

    // Setup connection to Redis
    // Note that if there is an error here we will just keep on trying automatically
    if ( RedisConnect() )
    {
        ProvideRedisNodesDoc();
    }
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

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

    CORE::CTaskConsumer::OnTaskEnded( taskData, wasForced );
}

/*-------------------------------------------------------------------------*/

RedisNode::RedisNode( void )
    : host()
    , nodeId()
    , startSlot( 0 )
    , endSlot( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoReplication( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString typeParam( "replication" );
    return GetRedisInfo( typeParam, kv );
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoStats( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString typeParam( "stats" );
    return GetRedisInfo( typeParam, kv );
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoCommandStats( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString typeParam( "commandstats" );
    return GetRedisInfo( typeParam, kv );
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfoMemory( CORE::CValueList& kv )
{GUCEF_TRACE;
    
    static CORE::CString typeParam( "memory" );
    return GetRedisInfo( typeParam, kv );
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisInfo( const CORE::CString& type, CORE::CValueList& kv )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
        return false;
    
    try
    {
        CORE::CString infoCmd( "INFO" );

        sw::redis::StringView infoCmdSV( infoCmd.C_String(), infoCmd.Length() );
        sw::redis::StringView typeParamSV( type.C_String(), type.Length() );

        auto reply = m_redisContext->command( infoCmdSV, typeParamSV );
        if ( reply )
        {
            int type = reply->type;
            if ( REDIS_REPLY_STRING == type )
            {
                CORE::CValueList kvCombined;
                kvCombined.SetMultiple( reply->str, '\n', ':' );

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
                            kv.Set( key, comboValue );
                        }
                        else
                        if ( values.size() > 1 )
                        {
                            CORE::CString keyPrefix = key + '_';
                            CORE::CString::StringVector::iterator n = values.begin();
                            while ( n != values.end() )
                            {
                                kv.Set( (*n), '=', &keyPrefix );
                                ++n;
                            }
                        }
                    }
                    ++i;
                }
            }
            else
            {
                if ( REDIS_REPLY_ERROR == type )
                {
                    return false;
                }
            }
        }
    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: Redis++ exception: " + e.what() );
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
RedisInfoService::GetRedisClusterNodeMap( RedisNodeMap& nodeMap )
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
                    return false;
                }
            }
        }

    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: Redis++ exception: " + e.what() );
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
RedisInfoService::RedisConnect( void )
{GUCEF_TRACE;

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

        return true;
    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisConnect: Redis++ exception: " + e.what() );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfoService(" + CORE::PointerToString( this ) + "):RedisConnect: exception: " + e.what() );
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
            GUCEF_METRIC_GAUGE( metricPrefix + key, CORE::StringToDouble( value ), 1.0f );
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_settings.gatherInfoCommandStats )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoCommandStats( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + ".CommandStats.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoMemory )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoMemory( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + ".Memory.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoReplication )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoReplication( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + ".Replication.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
    if ( m_settings.gatherInfoCommandStats )
    {
        CORE::CValueList kv;
        if ( GetRedisInfoStats( kv ) )
        {
            CORE::CString metricPrefix = m_settings.metricPrefix + ".Stats.";
            SendKeyValueStats( kv, metricPrefix );
        }
    }
}

/*-------------------------------------------------------------------------*/

RestApiRedisInfoInfoResource::RestApiRedisInfoInfoResource( RedisInfo* app )
    : COM::CCodecBasedHTTPServerResource()
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
    : COM::CCodecBasedHTTPServerResource()
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
    , m_infoService()
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
    bool errorOccured = !SetStandbyMode( m_globalStandbyEnabled );

    if ( !errorOccured )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo: Opening REST API" );
        return m_httpServer.Listen();
    }
    return errorOccured;
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

        if ( !taskManager.RequestTaskToStop( m_infoService.StaticCast< CORE::CTaskConsumer >(), true ) )
        {
            totalSuccess = false;
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo:SetStandbyMode( true ): Failed to signal task to stop" );
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo:SetStandbyMode( true ): Requested task to stop" );
        }

        m_isInStandby = totalSuccess;
        return totalSuccess;
    }
    else
    {
        bool totalSuccess = true;

        CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();

        m_infoService = RedisInfoServicePtr( new RedisInfoService() );
        if ( !m_infoService->LoadConfig( m_settings ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo::SetStandbyMode( false ): Failed to set settings for task" );
            totalSuccess = false;
        }

        if ( !taskManager.StartTask( m_infoService ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisInfo::SetStandbyMode( false ): Failed to start task" );
            totalSuccess = false;
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

    m_transmitMetrics = CORE::StringToBool( appConfig.GetValueAlways( "TransmitMetrics" ), true );
    m_globalStandbyEnabled = CORE::StringToBool( appConfig.GetValueAlways( "GlobalStandbyEnabled" ), false );
    m_redisHost = CORE::ResolveVars( appConfig.GetValueAlways( "RedisHost", "127.0.0.1" ) );
    m_redisPort = CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RedisPort" ) ), 6379 );

    m_settings.redisAddress.SetHostname( m_redisHost );
    m_settings.redisAddress.SetPortInHostByteOrder( m_redisPort );
    
    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );

    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort" ) ), 10000 ) );

    m_httpRouter.SetResourceMapping( "/info", RestApiRedisInfoInfoResource::THTTPServerResourcePtr( new RestApiRedisInfoInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiRedisInfoConfigResource::THTTPServerResourcePtr( new RestApiRedisInfoConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiRedisInfoConfigResource::THTTPServerResourcePtr( new RestApiRedisInfoConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping( CORE::ResolveVars( appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ) ), RestApiRedisInfoInfoResource::THTTPServerResourcePtr( new COM::CDummyHTTPServerResource() )  );

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
