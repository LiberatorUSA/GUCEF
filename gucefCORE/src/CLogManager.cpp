/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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