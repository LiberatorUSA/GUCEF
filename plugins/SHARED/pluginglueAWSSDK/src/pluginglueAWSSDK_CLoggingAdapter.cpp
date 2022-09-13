/*
 *  pluginglueAWSSDK: Library to support multiple AWS SDK based plugins that share overlap
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

#include <cstdarg>

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#include "pluginglueAWSSDK_CLoggingAdapter.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PLUGINGLUE {
namespace AWSSDK {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CLoggingAdapter::CLoggingAdapter( void )
    : Aws::Utils::Logging::LogSystemInterface()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CLoggingAdapter::~CLoggingAdapter()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CLoggingAdapter::MapLogLevel( Aws::Utils::Logging::LogLevel logLevel ,
                              CORE::TLogMsgType& gupLogType          , 
                              CORE::Int32& gupLogLevel               )
{GUCEF_TRACE;

    switch ( logLevel )
    {   
        case Aws::Utils::Logging::LogLevel::Off:
        case Aws::Utils::Logging::LogLevel::Trace:
        {
            gupLogType = CORE::TLogMsgType::LOG_DEBUG;
            gupLogLevel = CORE::LOGLEVEL_EVERYTHING;
            break;
        }
        case Aws::Utils::Logging::LogLevel::Debug:
        {
            gupLogType = CORE::TLogMsgType::LOG_DEBUG;
            gupLogLevel = CORE::LOGLEVEL_BELOW_NORMAL;
            break;
        }
        case Aws::Utils::Logging::LogLevel::Error:
        {
            gupLogType = CORE::TLogMsgType::LOG_ERROR;
            gupLogLevel = CORE::LOGLEVEL_VERY_IMPORTANT;
            break;
        }
        case Aws::Utils::Logging::LogLevel::Fatal:
        {
            gupLogType = CORE::TLogMsgType::LOG_ERROR;
            gupLogLevel = CORE::LOGLEVEL_CRITICAL;
            break;
        }
        case Aws::Utils::Logging::LogLevel::Info:
        {
            gupLogType = CORE::TLogMsgType::LOG_STANDARD;
            gupLogLevel = CORE::LOGLEVEL_IMPORTANT;
            break;
        }
        case Aws::Utils::Logging::LogLevel::Warn:
        {
            gupLogType = CORE::TLogMsgType::LOG_WARNING;
            gupLogLevel = CORE::LOGLEVEL_IMPORTANT;
            break;
        }
        default:
        {
            gupLogType = CORE::TLogMsgType::LOG_STANDARD;
            gupLogLevel = CORE::LOGLEVEL_NORMAL;
            break;
        }  
    }
}

/*-------------------------------------------------------------------------*/

Aws::Utils::Logging::LogLevel
CLoggingAdapter::MapLogLevel( CORE::TLogMsgType logType , 
                              CORE::Int32 logLevel      )
{GUCEF_TRACE;

    switch ( logType )
    {   
        case CORE::TLogMsgType::LOG_ERROR:
        case CORE::TLogMsgType::LOG_EXCEPTION:
        {
            if ( logLevel >= CORE::LOGLEVEL_CRITICAL )
                return Aws::Utils::Logging::LogLevel::Fatal;
            return Aws::Utils::Logging::LogLevel::Error; 
        }
        case CORE::TLogMsgType::LOG_WARNING:
        {
            return Aws::Utils::Logging::LogLevel::Warn; 
        }
        default:
        {
            if ( logLevel >= CORE::LOGLEVEL_CRITICAL )
                return Aws::Utils::Logging::LogLevel::Error;
            if ( logLevel >= CORE::LOGLEVEL_IMPORTANT )
                return Aws::Utils::Logging::LogLevel::Info;
            if ( logLevel >= CORE::LOGLEVEL_BELOW_NORMAL )
                return Aws::Utils::Logging::LogLevel::Debug;
            return Aws::Utils::Logging::LogLevel::Trace;
        }    
    }
}

/*-------------------------------------------------------------------------*/

Aws::Utils::Logging::LogLevel 
CLoggingAdapter::GetLogLevel( void ) const
{GUCEF_TRACE;

    CORE::CLogManager& logManager = CORE::CCoreGlobal::Instance()->GetLogManager();
    return MapLogLevel( CORE::LOG_STANDARD, logManager.GetMinLogLevel() );
}

/*-------------------------------------------------------------------------*/

void
CLoggingAdapter::Log( Aws::Utils::Logging::LogLevel logLevel , 
                      const char* tag                        , 
                      const char* formatStr                  , 
                      ...                                    )
{GUCEF_TRACE;

    std::va_list args;
    va_start( args, formatStr );

    va_list tmp_args; //unfortunately you cannot consume a va_list twice
    va_copy( tmp_args, args ); //so we have to copy it
    #ifdef WIN32
        const int requiredLength = _vscprintf(formatStr, tmp_args) + 1;
    #else
        const int requiredLength = vsnprintf(nullptr, 0, formatStr, tmp_args) + 1;
    #endif
    va_end( tmp_args );

    //Array<char> outputBuff(requiredLength);
    //#ifdef WIN32
    //    vsnprintf_s(outputBuff.GetUnderlyingData(), requiredLength, _TRUNCATE, formatStr, args);
    //#else
    //    vsnprintf(outputBuff.GetUnderlyingData(), requiredLength, formatStr, args);
    //#endif // WIN32

    //ss << outputBuff.GetUnderlyingData() << std::endl;  
  
    //ProcessFormattedStatement(ss.str());

    va_end( args );
}

/*-------------------------------------------------------------------------*/

void 
CLoggingAdapter::LogStream( Aws::Utils::Logging::LogLevel logLevel  , 
                            const char* tag                         , 
                            const Aws::OStringStream& messageStream )
{GUCEF_TRACE;

    CORE::Int32 guLogLvl = CORE::LOGLEVEL_BELOW_NORMAL;
    CORE::TLogMsgType guLogMsgType = CORE::TLogMsgType::LOG_UNKNOWNTYPE;
    MapLogLevel( logLevel, guLogMsgType, guLogLvl ); 

    if ( guLogMsgType == CORE::TLogMsgType::LOG_DEBUG )
    {
        // Allow for a chance to compile out some of the work involved
        GUCEF_DEBUG_LOG( guLogLvl, "AWS-SDK: " + messageStream.str() );
    }
    else
    {
        CORE::CCoreGlobal::Instance()->Log( guLogMsgType, guLogLvl, "AWS-SDK: " + messageStream.str() );
    }
}

/*-------------------------------------------------------------------------*/

void 
CLoggingAdapter::Flush()
{GUCEF_TRACE;

    CORE::CCoreGlobal::Instance()->GetLogManager().FlushLogs();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSDK */
}; /* namespace PLUGINGLUE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
