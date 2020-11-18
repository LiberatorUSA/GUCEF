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

#define GUCEF_DEFAULT_UDP_RECEIVE_BUFFERS                           100
#define GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE                  10000
#define GUCEF_DEFAULT_UDP_OS_LEVEL_RECEIVE_BUFFER_SIZE              (1024 * 1024 * 10)
#define GUCEF_DEFAULT_UDP_MAX_SOCKET_CYCLES_PER_PULSE               25
#define GUCEF_DEFAULT_MAX_DEDICATED_REDIS_WRITER_MAIL_BULK_READ     100

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( void )
    : redisAddress()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( const ChannelSettings& src )
    : redisAddress( src.redisAddress )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings&
ChannelSettings::operator=( const ChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        redisAddress = src.redisAddress;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
ChannelSettings::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    tree.SetAttribute( "redisAddress", redisAddress.AddressAndPortAsString() );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
ChannelSettings::LoadConfig( const CORE::CDataNode& tree )
{GUCEF_TRACE;

    redisAddress.SetHostnameAndPort( tree.GetAttributeValueOrChildValueByName( "redisAddress", redisAddress.AddressAndPortAsString() ) );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
ChannelSettings::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "udp2rediscluster::ChannelSettings";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

RedisInfoService::RedisInfoService()
    : CORE::CTaskConsumer()
    , m_redisContext( GUCEF_NULL )
    , m_redisPacketArgs()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

RedisInfoService::~RedisInfoService()
{GUCEF_TRACE;

    delete m_redisContext;
    m_redisContext = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
RedisInfoService::RegisterEventHandlers( void )
{GUCEF_TRACE;

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
RedisInfoService::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    //RegisterEventHandlers();

    // Setup connection to Redis
    // Note that if there is an error here we will just keep on trying automatically
    RedisConnect();

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
{
}

/*-------------------------------------------------------------------------*/

bool
RedisInfoService::GetRedisClusterNodeMap( RedisNodeMap& nodeMap )
{
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
        rppConnectionOptions.host = m_channelSettings.redisAddress.GetHostname();  // Required.
        rppConnectionOptions.port = m_channelSettings.redisAddress.GetPortInHostByteOrder(); // Optional. The default port is 6379.
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

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisConnect: Successfully created a Redis context" );

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
