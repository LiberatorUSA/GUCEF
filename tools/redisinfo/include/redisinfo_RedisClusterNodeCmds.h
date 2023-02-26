/*
 *  pubsubpluginREDISCLUSTER: Generic GUCEF COMCORE plugin for providing pubsub via Redis
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

#ifndef REDISINFO_REDISCLUSTERNODECMDS_H
#define REDISINFO_REDISCLUSTERNODECMDS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "hiredis.h"
#include "async.h"
#include "redis++.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace REDISINFO {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< sw::redis::RedisCluster, MT::CMutex >    RedisClusterPtr;

/*--------------------------------------------------------------------------*/

class RedisClusterNodeCmds
{
    private:

    RedisClusterPtr m_clusterAccess;
    
    public:

    RedisClusterNodeCmds( RedisClusterPtr clusterAccess )
        : m_clusterAccess( clusterAccess )
    {
    }

    template <typename ...Args>
    sw::redis::ReplyUPtr command( sw::redis::Connection& connection, const sw::redis::StringView &cmd_name, Args &&...args ) 
    {
        auto cmd = []( sw::redis::Connection& connection, const sw::redis::StringView &name, Args &&...params ) 
        {
            sw::redis::CmdArgs cmd_args;
            cmd_args.append( name, std::forward<Args>(params)... );
            connection.send( cmd_args );
        };

        return m_clusterAccess->_command( cmd, connection, cmd_name, std::forward<Args>(args)... );
    }

    sw::redis::ReplyUPtr scan( sw::redis::Connection& connection, const CORE::CString& keyType, CORE::UInt32 iterationCounter, CORE::UInt32 maxResults )
    {
        static const CORE::CString scanCmd( "SCAN" );
        CORE::CString itteratorParam( CORE::ToString( iterationCounter ) );
        static const CORE::CString typeParam( "TYPE" );
        static const CORE::CString countParam( "COUNT" );         
        CORE::CString countValueParam( CORE::ToString( maxResults ) );        

        sw::redis::StringView scanCmdSV( scanCmd.C_String(), scanCmd.Length() );
        sw::redis::StringView itteratorParamSV( itteratorParam.C_String(), itteratorParam.Length() );
        sw::redis::StringView typeParamSV( typeParam.C_String(), typeParam.Length() );
        sw::redis::StringView typeValueParamSV( keyType.C_String(), keyType.Length() );
        sw::redis::StringView countParamSV( countParam.C_String(), countParam.Length() );
        sw::redis::StringView countValueParamSV( countValueParam.C_String(), countValueParam.Length() );

        return command( connection, scanCmdSV, itteratorParamSV, typeParamSV, typeValueParamSV, countParamSV, countValueParamSV );
    }   
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace REDISINFO */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* REDISINFO_REDISCLUSTERNODECMDS_H ? */
