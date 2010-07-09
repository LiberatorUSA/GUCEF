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

#include "gucefCORE_CThreadedLogger.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTITLITIES                                                         //
//                                                                         //
//-------------------------------------------------------------------------*/

CThreadedLogger::CThreadedLogger( void )
    : m_loggerBackend( NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CThreadedLogger::CThreadedLogger( CILogger* loggerBackend )
    : m_loggerBackend( loggerBackend )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CThreadedLogger::~CThreadedLogger()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CThreadedLogger::Log( const TLogMsgType logMsgType ,
                      const Int32 logLevel         ,
                      const CString& logMessage    ,
                      const UInt32 threadId        )
{GUCEF_TRACE;

    CLoggingMail logMsg;
    logMsg.logLevel = logLevel;
    logMsg.logMessage = logMessage;
    logMsg.logMsgType = logMsgType;
    logMsg.threadId = threadId;
    
    m_mailbox.AddMail( MAILTYPE_NEWLOGMSG, &logMsg );
}

/*-------------------------------------------------------------------------*/

void
CThreadedLogger::FlushLog( void )
{GUCEF_TRACE;

    m_mailbox.AddMail( MAILTYPE_FLUSHLOG, NULL );
}

/*-------------------------------------------------------------------------*/
    
void
CThreadedLogger::SetBackend( CILogger* loggerBackend )
{GUCEF_TRACE;

    // Atomic assignment of backend implementation
    m_mailbox.LockData();
    m_loggerBackend = loggerBackend;
    m_mailbox.UnlockData();
}

/*-------------------------------------------------------------------------*/

bool
CThreadedLogger::OnTaskStart( void* taskdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CThreadedLogger::OnTaskCycle( void* taskdata )
{GUCEF_TRACE;

    LockData();

    if ( NULL != m_loggerBackend )
    {
        TMailType mailType;
        CLoggingMail* loggingMail = NULL;
        if ( m_mailbox.GetMail( mailType     , 
                                &loggingMail ) )
        {
            switch ( mailType )
            {
                case MAILTYPE_NEWLOGMSG :
                {
                    m_loggerBackend->Log( loggingMail->logMsgType , 
                                          loggingMail->logLevel   ,
                                          loggingMail->logMsgType ,
                                          loggingMail->threadId   );
                    
                    break;
                }
                case MAILTYPE_FLUSHLOG :
                {
                    m_loggerBackend->FlushLog();
                    break;
                }
                
            }
        }
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CThreadedLogger::LockData( void )
{GUCEF_TRACE;

    m_mailbox.LockData();
}

/*-------------------------------------------------------------------------*/

void
CThreadedLogger::UnlockData( void )
{GUCEF_TRACE;

    m_mailbox.UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CThreadedLogger::OnTaskEnd( void* taskdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
