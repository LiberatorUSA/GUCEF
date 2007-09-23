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

#ifndef GUCEF_CORE_CILOGGER_H
#include "CILogger.h"
#define GUCEF_CORE_CILOGGER_H
#endif /* GUCEF_CORE_CILOGGER_H ? */

#include "CLogManager.h"

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CLogManager::CLogManager( void )
    : m_loggers()                         ,
      m_msgTypeEnablers()                 ,
      m_maxLogLevel( GUCEFCORE_INT32MAX )
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
}

/*-------------------------------------------------------------------------*/

CLogManager::~CLogManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CLogManager*
CLogManager::Instance( void )
{GUCEF_TRACE;
    
    g_dataLock.Lock();
    
    if ( g_instance == NULL )
    {
        g_instance = new CLogManager();
    }
    
    g_dataLock.Unlock();
    
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
CLogManager::AddLogger( CILogger* loggerImp )
{GUCEF_TRACE;

    // Do not add bogus loggers
    assert( NULL != loggerImp );
    
    g_dataLock.Lock();
    m_loggers.insert( loggerImp );
    g_dataLock.Unlock();
    
    GUCEF_SYSTEM_LOG( 0, "Added logger" );
}

/*-------------------------------------------------------------------------*/
    
void
CLogManager::RemoveLogger( CILogger* loggerImp )
{GUCEF_TRACE;

    // Do not pass in bogus pointers
    assert( NULL != loggerImp );

    GUCEF_SYSTEM_LOG( 0, "Removing logger" );
    
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

    g_dataLock.Lock();
    
    if ( logLevel < m_maxLogLevel )
    {
        if ( (*m_msgTypeEnablers.find( logMsgType )).second )
        {
            TLoggerList::const_iterator i = m_loggers.begin();
            while ( i != m_loggers.end() )
            {
                CILogger* logger = (*i);
                if ( NULL != logger )
                {
                    logger->Log( logMsgType ,
                                 logLevel   ,
                                 logMessage );
                }
                ++i;
            }
        }
    }
    
    // We want to make certain that errors are always in the log file.
    // We might crash moments later which might cause some loggers not
    // to write the error entry to their respective output media
    if ( LOG_ERROR == logMsgType )
    {
        FlushLogs();
    }
    
    g_dataLock.Unlock();
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/