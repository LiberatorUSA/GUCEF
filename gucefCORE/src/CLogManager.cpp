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

#ifndef GUCEF_CORE_CILOGGER_H
#include "CILogger.h"
#define GUCEF_CORE_CILOGGER_H
#endif /* GUCEF_CORE_CILOGGER_H ? */

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CLogManager::CLogManager( void )
    : m_loggers()                         ,
      m_msgTypeEnablers()                 ,
      m_maxLogLevel( GUCEFCORE_INT32MAX )
{TRACE;

    m_msgTypeEnablers[ LOG_ERROR ] = true;
    m_msgTypeEnablers[ LOG_STANDARD ] = true;
    m_msgTypeEnablers[ LOG_USER ] = true;
    m_msgTypeEnablers[ LOG_SYSTEM ] = true;
    m_msgTypeEnablers[ LOG_DEV ] = true;
    m_msgTypeEnablers[ LOG_DEBUG ] = true;
    m_msgTypeEnablers[ LOG_SERVICE ] = true;
    m_msgTypeEnablers[ LOG_PROTECTED ] = true;
}

/*-------------------------------------------------------------------------*/

CLogManager::~CLogManager()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CLogManager*
CLogManager::Instance( void )
{TRACE;
    
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
{TRACE;

    g_dataLock.Lock();
    
    delete g_instance;
    g_instance = NULL;
    
    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CLogManager::AddLogger( CILogger* loggerImp )
{TRACE;

    g_dataLock.Lock();
    m_loggers.insert( loggerImp );
    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/
    
void
CLogManager::RemoveLogger( CILogger* loggerImp )
{TRACE;

    g_dataLock.Lock();
    m_loggers.erase( loggerImp );
    g_dataLock.Unlock();
}
    
/*-------------------------------------------------------------------------*/
    
void
CLogManager::ClearLoggers( void )
{TRACE;

    g_dataLock.Lock();
    m_loggers.clear();
    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/
    
 bool
 CLogManager::IsLoggingEnabled( const TLogMsgType logMsgType ,
                                const Int32 logLevel         ) const
{TRACE;

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
                  const CString& logMessage    ) const
{TRACE;

    g_dataLock.Lock();
    
    if ( logLevel < m_maxLogLevel )
    {
        if ( (*m_msgTypeEnablers.find( logMsgType )).second )
        {
            TLoggerList::const_iterator i = m_loggers.begin();
            while ( i != m_loggers.end() )
            {
                (*i)->Log( logMsgType ,
                           logLevel   ,
                           logMessage );
                
                ++i;
            }
        }
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