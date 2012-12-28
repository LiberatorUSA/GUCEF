/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#include "gucefCORE_CAndroidSystemLogger.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

#include <android/log.h>

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef void ( *TAndroidLogWriteFunc )( android_LogPriority, const char*, const char* );

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

static android_LogPriority
LogTypeAndLevelToAndroidPrio( const CLogManager::TLogMsgType logMsgType ,
                              const Int32 logLevel                      )
{
    switch ( logMsgType )
    {
        case CLogManager::LOG_EXCEPTION:
        case CLogManager::LOG_ERROR:
        {
            if ( logLevel < LOGLEVEL_CRITICAL )
            {
                if ( logLevel >= LOGLEVEL_NORMAL )
                {
                    return ANDROID_LOG_ERROR;
                }
                return ANDROID_LOG_WARN;
            }
            return ANDROID_LOG_FATAL;
        }
        case CLogManager::LOG_DEV:
        case CLogManager::LOG_DEBUG:
        case CLogManager::LOG_CALLSTACK:
        {
            if ( logLevel >= LOGLEVEL_NORMAL )
            {
                return ANDROID_LOG_DEBUG;
            }
            return ANDROID_LOG_VERBOSE;
        }
        case CLogManager::LOG_USER:
        case CLogManager::LOG_STANDARD:
        case CLogManager::LOG_SYSTEM:
        case CLogManager::LOG_SERVICE:
        {
            if ( logLevel >= LOGLEVEL_NORMAL )
            {
                return ANDROID_LOG_INFO;
            }
            if ( logLevel >= LOGLEVEL_BELOW_NORMAL )
            {
                return ANDROID_LOG_DEBUG;
            }
            return ANDROID_LOG_VERBOSE;
        }
        case CLogManager::LOG_PROTECTED:
        {
            // Android does not support encypted logs
            return ANDROID_LOG_SILENT;
        }
        case CLogManager::LOG_UNKNOWNTYPE:
        {
            return ANDROID_LOG_UNKNOWN;
        }
        default:
        {
            return ANDROID_LOG_DEFAULT;
        }
    }
}

/*-------------------------------------------------------------------------*/

CAndroidSystemLogger::CAndroidSystemLogger( void )
    : CILogger()        ,
      m_tag()           ,
      m_logFunc( NULL )
{GUCEF_TRACE;

    m_tag = "GUCEF:AndroidSystemLogger:" + PointerToString( this );

    // We load the log module dynamically to avoid adding dependencies
    void* logModule = LoadModuleDynamicly( "log.so" );
    if ( NULL != logModule )
    {
        // Find the address of the Android log function
        void* funcAddr = GetFunctionAddress( logModule                                     ,
                                             "__android_log_write"                         ,
                                             sizeof(android_LogPriority)+(2*sizeof(char*)) ).objPtr;
        if ( NULL != funcAddr )
        {
            m_logFunc = funcAddr;
        }
    }
}

/*-------------------------------------------------------------------------*/

CAndroidSystemLogger::~CAndroidSystemLogger()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CAndroidSystemLogger::Log( const TLogMsgType logMsgType ,
                           const Int32 logLevel         ,
                           const CString& logMessage    ,
                           const UInt32 threadId        )
{GUCEF_TRACE;

    if ( NULL != m_logFunc )
    {
        // Just use standard GUCEF log formatting
        CString formattedLogMsg = FormatStdLogMessage( logMsgType ,
                                                       logLevel   ,
                                                       logMessage ,
                                                       threadId   );

        android_LogPriority androidLogPrio = LogTypeAndLevelToAndroidPrio( logMsgType, logLevel );
        ( (TAndroidLogWriteFunc) m_logFunc )( androidLogPrio, m_tag.C_String(), formattedLogMsg.C_String() );
    }
}

/*-------------------------------------------------------------------------*/

void
CAndroidSystemLogger::LogWithoutFormatting( const TLogMsgType logMsgType ,
                                            const Int32 logLevel         ,
                                            const CString& logMessage    ,
                                            const UInt32 threadId        )
{GUCEF_TRACE;

    if ( NULL != m_logFunc )
    {
        android_LogPriority androidLogPrio = LogTypeAndLevelToAndroidPrio( logMsgType, logLevel );
        ( (TAndroidLogWriteFunc) m_logFunc )( androidLogPrio, m_tag.C_String(), logMessage.C_String() );
    }
}

/*-------------------------------------------------------------------------*/

void
CAndroidSystemLogger::FlushLog( void )
{GUCEF_TRACE;

    // Android does not support direct flush control of its system log
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ? */
