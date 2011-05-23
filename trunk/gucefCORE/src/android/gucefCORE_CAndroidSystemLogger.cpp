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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

static android_LogPriority
LogTypeAndLevelToAndroidPrio( const TLogMsgType logMsgType ,
                              const Int32 logLevel         )
{
    switch ( logMsgType )
    {
        case LOG_EXCEPTION:
        case LOG_ERROR:
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
        case LOG_DEV:
        case LOG_DEBUG:
        case LOG_CALLSTACK:
        {
            if ( logLevel >= LOGLEVEL_NORMAL )
            {
                return ANDROID_LOG_DEBUG;
            }
            return ANDROID_LOG_VERBOSE;
        }
        case LOG_USER:
        case LOG_STANDARD:
        case LOG_SYSTEM:
        case LOG_SERVICE:
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
        case LOG_PROTECTED:
        {
            // Android does not support encypted logs
            return ANDROID_LOG_SILENT;
        }
        case LOG_UNKNOWNTYPE:
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
    : CILogger() ,
      m_tag()
{GUCEF_TRACE;

    m_tag = "GUCEF:AndroidSystemLogger:" + PointerToString( this );
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

    // Just use standard GUCEF log formatting
    CString formattedLogMsg = FormatStdLogMessage( logMsgType ,
                                                   logLevel   ,
                                                   logMessage ,
                                                   threadId   );
                                                   
    android_LogPriority androidLogPrio = LogTypeAndLevelToAndroidPrio( logMsgType, logLevel );    
    __android_log_write( androidLogPrio, m_tag.C_String(), formattedLogMsg.C_String() );                             
}

/*-------------------------------------------------------------------------*/

void
CAndroidSystemLogger::LogWithoutFormatting( const TLogMsgType logMsgType ,
                                            const Int32 logLevel         ,
                                            const CString& logMessage    ,
                                            const UInt32 threadId        )
{GUCEF_TRACE;

    android_LogPriority androidLogPrio = LogTypeAndLevelToAndroidPrio( logMsgType, logLevel );    
    __android_log_write( androidLogPrio, m_tag.C_String(), logMessage.C_String() );
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
