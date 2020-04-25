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

#include <assert.h>

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_CILOGGER_H
#include "CILogger.h"
#define GUCEF_CORE_CILOGGER_H
#endif /* GUCEF_CORE_CILOGGER_H ? */

#ifndef GUCEF_CORE_CMULTILOGGER_H
#include "gucefCORE_CMultiLogger.h"
#define GUCEF_CORE_CMULTILOGGER_H
#endif /* GUCEF_CORE_CMULTILOGGER_H ? */

#ifndef GUCEF_CORE_CLOGGINGTASK_H
#include "gucefCORE_CLoggingTask.h"
#define GUCEF_CORE_CLOGGINGTASK_H
#endif /* GUCEF_CORE_CLOGGINGTASK_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "CLogManager.h"

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

  #ifndef GUCEF_CORE_CANDROIDSYSTEMLOGGER_H
  #include "gucefCORE_CAndroidSystemLogger.h"
  #define GUCEF_CORE_CANDROIDSYSTEMLOGGER_H
  #endif /* GUCEF_CORE_CANDROIDSYSTEMLOGGER_H ? */

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

extern "C" {

const Int32 LOGLEVEL_CRITICAL = GUCEFCORE_INT32MAX - 1;
const Int32 LOGLEVEL_VERY_IMPORTANT = 250000;
const Int32 LOGLEVEL_IMPORTANT = 100000;
const Int32 LOGLEVEL_NORMAL = 50000;
const Int32 LOGLEVEL_BELOW_NORMAL = 25000;
const Int32 LOGLEVEL_EVERYTHING = 0;

}

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )
static CAndroidSystemLogger androidSystemLogger;
#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CLogManager::CLogManager( void )
    : m_loggers( new CMultiLogger() )
    , m_loggingTask( GUCEF_NULL )
    , m_useLogThread( false )
    , m_bootstrapLog() 
    , m_busyLogging( false ) 
    , m_redirectToLogQueue( false )
    , m_dataLock()
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )
    AddLogger( &androidSystemLogger);
    #endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ? */
}

/*-------------------------------------------------------------------------*/

CLogManager::~CLogManager()
{GUCEF_TRACE;

    ClearLoggers();

    delete m_loggingTask;
    m_loggingTask = GUCEF_NULL;
    
    delete m_loggers;
    m_loggers = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CLogManager::RedirectToBootstrapLogQueue( bool redirect )
{GUCEF_TRACE;
    
    m_dataLock.Lock();

    m_redirectToLogQueue = redirect;
    if ( redirect )
    {
        Log( LOG_SYSTEM, LOGLEVEL_NORMAL, "LogManager: Redirecting all log statements to the bootstrap log queue" );
    }
    else
    {
        Log( LOG_SYSTEM, LOGLEVEL_NORMAL, "LogManager: Turning off redirect of all log statements to the bootstrap log queue" );
        FlushBootstrapLogEntriesToLogs();
    }

    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::FlushBootstrapLogEntriesToLogs( void )
{GUCEF_TRACE;

    Log( LOG_SYSTEM, LOGLEVEL_NORMAL, "LogManager: Flushing all bootstrap log entries to the currently registered loggers" );

    m_dataLock.Lock();

    if ( m_loggers->GetLoggerCount() > 0 )
    {
        TBootstrapLogVector::iterator i = m_bootstrapLog.begin();
        while ( i != m_bootstrapLog.end() )
        {
            TBootstrapLogEntry& entry = (*i);
            Log( entry.logMsgType ,
                 entry.logLevel   ,
                 entry.logMessage ,
                 entry.threadId   );

            ++i;
        }
        m_bootstrapLog.clear();

        Log( LOG_SYSTEM, LOGLEVEL_NORMAL, "LogManager: Finished flushing all bootstrap log entries to the currently registered loggers" );
    }
    else
    {
        Log( LOG_ERROR, LOGLEVEL_NORMAL, "LogManager: Unable to flush bootstrap log entries since no loggers are currently registered" );
    }

    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::AddLogger( CILogger* loggerImp )
{GUCEF_TRACE;

    m_dataLock.Lock();
    if ( m_useLogThread )
        m_loggingTask->PauseTask();    
    m_loggers->AddLogger( loggerImp );
    if ( m_useLogThread )
        m_loggingTask->ResumeTask();
    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::RemoveLogger( CILogger* loggerImp )
{GUCEF_TRACE;

    m_dataLock.Lock();
    if ( m_useLogThread )
        m_loggingTask->PauseTask();
    m_loggers->RemoveLogger( loggerImp );
    if ( m_useLogThread )
        m_loggingTask->ResumeTask();
    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::ClearLoggers( void )
{GUCEF_TRACE;

    m_dataLock.Lock();
    if ( m_useLogThread )
        m_loggingTask->PauseTask();
    m_loggers->ClearLoggers();
    if ( m_useLogThread )
        m_loggingTask->ResumeTask();
    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

 bool
 CLogManager::IsLoggingEnabled( const TLogMsgType logMsgType ,
                                const Int32 logLevel         ) const
{GUCEF_TRACE;

    m_dataLock.Lock();
    bool retValue = m_loggers->IsLoggingEnabled( logMsgType, logLevel );
    m_dataLock.Unlock();
    return retValue;
}

/*-------------------------------------------------------------------------*/

void
CLogManager::SetMinLogLevel( const Int32 logLevel )
{GUCEF_TRACE;

    m_loggers->SetMinLogLevel( logLevel );
}

/*-------------------------------------------------------------------------*/

Int32 
CLogManager::GetMinLogLevel( void ) const
{GUCEF_TRACE;

    return m_loggers->GetMinLogLevel();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::Log( const TLogMsgType logMsgType ,
                  const Int32 logLevel         ,
                  const CString& logMessage    )
{GUCEF_TRACE;

    Log( logMsgType, logLevel, logMessage, MT::GetCurrentTaskID() );
}

/*-------------------------------------------------------------------------*/

void
CLogManager::Log( const TLogMsgType logMsgType ,
                  const Int32 logLevel         ,
                  const CString& logMessage    ,
                  const UInt32 threadId        )
{GUCEF_TRACE;

    m_dataLock.Lock();

    // The loglevel must be such so that the message given falls under the global logging
    // cut off.
    // Additionally if a thread generates log message via logging logic then this
    // would result in a endless loop. To this end the "m_busyLogging" flag is used
    // if this flag is true then log messages will be dropped since the thread is within
    // the logger->Log() call and thus the message is generated due to the logging
    // activity itself.
    if ( !m_busyLogging )
    {
     
        if ( m_loggers->GetLoggerCount() > 0  && !m_redirectToLogQueue )
        {
            m_busyLogging = true;
            if ( m_useLogThread )
            {
                m_loggingTask->Log( logMsgType ,
                                    logLevel   ,
                                    logMessage ,
                                    threadId   );
            }
            else
            {
                m_loggers->Log( logMsgType ,
                                logLevel   ,
                                logMessage ,
                                threadId   );
            }
            m_busyLogging = false;
        }
        else
        {
            // We do not have any loggers yet so we will add the log entry to
            // the bootstrap log
            TBootstrapLogEntry entry;
            entry.logLevel = logLevel;
            entry.logMsgType = logMsgType;
            entry.logMessage = logMessage;
            entry.threadId = threadId;
            m_bootstrapLog.push_back( entry );
        }
    }

    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

const CString&
CLogManager::GetLogMsgTypeString( const TLogMsgType logMsgType )
{GUCEF_TRACE;

    switch ( logMsgType )
    {
        case CLogManager::LOG_ERROR :
        {
            static CString typeStr = "ERROR";
            return typeStr;
        }
        case CLogManager::LOG_WARNING :
        {
            static CString typeStr = "WARNING";
            return typeStr;
        }
        case CLogManager::LOG_STANDARD :
        {
            static CString typeStr = "STANDARD";
            return typeStr;
        }
        case CLogManager::LOG_USER :
        {
            static CString typeStr = "USER";
            return typeStr;
        }
        case CLogManager::LOG_SYSTEM :
        {
            static CString typeStr = "SYSTEM";
            return typeStr;
        }
        case CLogManager::LOG_DEV :
        {
            static CString typeStr = "DEV";
            return typeStr;
        }
        case CLogManager::LOG_DEBUG :
        {
            static CString typeStr = "DEBUG";
            return typeStr;
        }
        case CLogManager::LOG_SERVICE :
        {
            static CString typeStr = "SERVICE";
            return typeStr;
        }
        case CLogManager::LOG_PROTECTED :
        {
            static CString typeStr = "PROTECTED";
            return typeStr;
        }
        case CLogManager::LOG_CALLSTACK :
        {
            static CString typeStr = "CALLSTACK";
            return typeStr;
        }
        case CLogManager::LOG_EXCEPTION :
        {
            static CString typeStr = "EXCEPTION";
            return typeStr;
        }
        case CLogManager::LOG_CONSOLE :
        {
            static CString typeStr = "CONSOLE";
            return typeStr;
        }
        default :
        {
            static CString typeStr = "";
            return typeStr;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CLogManager::FlushLogs( void )
{GUCEF_TRACE;

    m_dataLock.Lock();
    if ( m_useLogThread )
    {
        m_loggingTask->FlushLog();
    }
    else
    {
        m_loggers->FlushLog();
    }
    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void 
CLogManager::SetUseLoggingThread( bool useLogThread )
{GUCEF_TRACE;

    m_dataLock.Lock();
    if ( useLogThread != m_useLogThread )
    {
        if ( useLogThread )
        {
            m_loggingTask = new CLoggingTask( *m_loggers );
            if ( m_loggingTask->StartTask() )
                m_useLogThread = useLogThread;
        }
        else
        {
            if ( m_loggingTask->StopTask() )
            {
                m_useLogThread = useLogThread;

                delete m_loggingTask;
                m_loggingTask = GUCEF_NULL;
            }
        }
    }
    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

bool 
CLogManager::GetUseLoggingThread( void ) const
{GUCEF_TRACE;

    return m_useLogThread;
}

/*-------------------------------------------------------------------------*/

CString
LogLevelToString( const Int32 logLevel )
{GUCEF_TRACE;

    switch ( logLevel )
    {
        case LOGLEVEL_CRITICAL : return "CRITICAL";
        case LOGLEVEL_VERY_IMPORTANT : return "VERY_IMPORTANT";
        case LOGLEVEL_IMPORTANT : return "IMPORTANT";
        case LOGLEVEL_NORMAL : return "NORMAL";
        case LOGLEVEL_BELOW_NORMAL : return "BELOW_NORMAL";
        case LOGLEVEL_EVERYTHING : return "EVERYTHING";
        default : return Int32ToString( logLevel );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
