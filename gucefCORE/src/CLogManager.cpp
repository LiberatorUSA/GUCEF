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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "CLogManager.h"

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

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

CLogManager* CLogManager::g_instance = NULL;
MT::CMutex CLogManager::g_dataLock;

extern "C" {

const Int32 LOGLEVEL_CRITICAL = GUCEFCORE_INT32MAX - 1;
const Int32 LOGLEVEL_VERY_IMPORTANT = 250000;
const Int32 LOGLEVEL_IMPORTANT = 100000;
const Int32 LOGLEVEL_NORMAL = 50000;
const Int32 LOGLEVEL_BELOW_NORMAL = 25000;
const Int32 LOGLEVEL_EVERYTHING = 0;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CLogManager::CLogManager( void )
    : m_loggers()                         ,
      m_msgTypeEnablers()                 ,
      m_maxLogLevel( GUCEFCORE_INT32MAX ) ,
      m_bootstrapLog()
{GUCEF_TRACE;

    m_msgTypeEnablers[ LOG_ERROR ] = true;
    m_msgTypeEnablers[ LOG_STANDARD ] = true;
    m_msgTypeEnablers[ LOG_USER ] = true;
    m_msgTypeEnablers[ LOG_SYSTEM ] = true;
    m_msgTypeEnablers[ LOG_DEV ] = true;
    m_msgTypeEnablers[ LOG_DEBUG ] = true;
    m_msgTypeEnablers[ LOG_SERVICE ] = true;
    m_msgTypeEnablers[ LOG_PROTECTED ] = true;
    m_msgTypeEnablers[ LOG_CALLSTACK ] = true;
    m_msgTypeEnablers[ LOG_EXCEPTION ] = true;
    m_msgTypeEnablers[ LOG_CONSOLE ] = true;
}

/*-------------------------------------------------------------------------*/

CLogManager::~CLogManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CLogManager*
CLogManager::Instance( void )
{GUCEF_TRACE;

    if ( g_instance == NULL )
    {
        g_dataLock.Lock();
        if ( g_instance == NULL )
        {
            g_instance = new CLogManager();
        }
        g_dataLock.Unlock();
    }

    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CLogManager::Deinstance( void )
{GUCEF_TRACE;

    g_dataLock.Lock();

    delete g_instance;
    g_instance = NULL;

    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::FlushBootstrapLogEntriesToLogs( void )
{GUCEF_TRACE;

    Log( LOG_SYSTEM, LOGLEVEL_NORMAL, "LogManager: Flushing all bootstrap log entries to the currently registered loggers" );

    g_dataLock.Lock();

    if ( m_loggers.size() > 0 )
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

    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::AddLogger( CILogger* loggerImp )
{GUCEF_TRACE;

    // Do not add bogus loggers
    assert( NULL != loggerImp );

    g_dataLock.Lock();
    m_loggers.insert( loggerImp );
    g_dataLock.Unlock();

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Added logger" );
}

/*-------------------------------------------------------------------------*/

void
CLogManager::RemoveLogger( CILogger* loggerImp )
{GUCEF_TRACE;

    // Do not pass in bogus pointers
    assert( NULL != loggerImp );

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Removing logger" );

    g_dataLock.Lock();
    m_loggers.erase( loggerImp );
    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::ClearLoggers( void )
{GUCEF_TRACE;

    g_dataLock.Lock();
    m_loggers.clear();
    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

 bool
 CLogManager::IsLoggingEnabled( const TLogMsgType logMsgType ,
                                const Int32 logLevel         ) const
{GUCEF_TRACE;

    g_dataLock.Lock();

    bool retValue = false;
    if ( logLevel < m_maxLogLevel )
    {
        retValue = (*m_msgTypeEnablers.find( logMsgType )).second;
    }

    g_dataLock.Unlock();
    return retValue;
}

/*-------------------------------------------------------------------------*/

void
CLogManager::Log( const TLogMsgType logMsgType ,
                  const Int32 logLevel         ,
                  const CString& logMessage    )
{GUCEF_TRACE;

    Log( logMsgType             ,
         logLevel               ,
         logMessage             ,
         MT::GetCurrentTaskID() );
}

/*-------------------------------------------------------------------------*/

void
CLogManager::Log( const TLogMsgType logMsgType ,
                  const Int32 logLevel         ,
                  const CString& logMessage    ,
                  const UInt32 threadId        )
{GUCEF_TRACE;

    g_dataLock.Lock();

    if ( logLevel < m_maxLogLevel )
    {
        if ( (*m_msgTypeEnablers.find( logMsgType )).second )
        {
            if ( m_loggers.size() > 0 )
            {
                TLoggerList::const_iterator i = m_loggers.begin();
                while ( i != m_loggers.end() )
                {
                    CILogger* logger = (*i);
                    if ( NULL != logger )
                    {
                        logger->Log( logMsgType ,
                                     logLevel   ,
                                     logMessage ,
                                     threadId   );
                    }
                    ++i;
                }

                // We want to make certain that errors are always in the log file.
                // We might crash moments later which might cause some loggers not
                // to write the error entry to their respective output media
                if ( LOG_ERROR == logMsgType )
                {
                    FlushLogs();
                }
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
    }



    g_dataLock.Unlock();
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

    g_dataLock.Lock();

    TLoggerList::const_iterator i = m_loggers.begin();
    while ( i != m_loggers.end() )
    {
        CILogger* logger = (*i);
        if ( NULL != logger )
        {
            logger->FlushLog();
        }
        ++i;
    }

    g_dataLock.Unlock();
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
