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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#include "gucefCORE_CLoggingTask.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define MAXMAILITEMSPERCYCLE    50

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

CLoggingTask::CLoggingTask( CILogger& loggerBackend )
    : CTaskConsumer()                   ,
      CILogger()                        ,
      m_loggerBackend( &loggerBackend ) ,
      m_mailbox()                       ,
      m_mailList()
{GUCEF_TRACE;

    m_mailList.reserve( MAXMAILITEMSPERCYCLE );
}

/*-------------------------------------------------------------------------*/

CLoggingTask::~CLoggingTask()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CLoggingTask::Log( const TLogMsgType logMsgType ,
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

bool
CLoggingTask::StartTask( void )
{GUCEF_TRACE;

    return CTaskManager::Instance()->StartTask( *this );
}

/*-------------------------------------------------------------------------*/

void
CLoggingTask::FlushLog( void )
{GUCEF_TRACE;

    m_mailbox.AddMail( MAILTYPE_FLUSHLOG, NULL );
}

/*-------------------------------------------------------------------------*/

bool
CLoggingTask::OnTaskStart( void* taskdata )
{GUCEF_TRACE;

    // This task does not need to perform any initialization/setup
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CLoggingTask::OnTaskCycle( void* taskdata )
{GUCEF_TRACE;

    CLoggingMail* loggingMail = NULL;
    if ( m_mailbox.GetMailList( m_mailList           ,
                                MAXMAILITEMSPERCYCLE ) )
    {
        while ( !m_mailList.empty() )
        {
            TLoggingMailBox::TMailElement& mailEntry = m_mailList[ m_mailList.size()-1 ];
            loggingMail = static_cast< CLoggingMail* >( mailEntry.data );

            switch ( mailEntry.eventid )
            {
                case MAILTYPE_NEWLOGMSG :
                {
                    m_loggerBackend->Log( loggingMail->logMsgType ,
                                          loggingMail->logLevel   ,
                                          loggingMail->logMessage ,
                                          loggingMail->threadId   );

                    break;
                }
                case MAILTYPE_FLUSHLOG :
                {
                    m_loggerBackend->FlushLog();
                    break;
                }

            }

            delete loggingMail;
            loggingMail = NULL;

            m_mailList.pop_back();
        }
    }

    // This is an infinate task so we always return false
    //  which indicates we are not done.
    return false;
}

/*-------------------------------------------------------------------------*/

void
CLoggingTask::LockData( void )
{GUCEF_TRACE;

    m_mailbox.LockData();
}

/*-------------------------------------------------------------------------*/

void
CLoggingTask::UnlockData( void )
{GUCEF_TRACE;

    m_mailbox.UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CLoggingTask::OnTaskEnd( void* taskdata )
{GUCEF_TRACE;

    TLoggingMailBox::TMailElement* entry;
    TMailList::iterator i( m_mailList.begin() );
    while ( i != m_mailList.end() )
    {
        entry = &(*i);
        delete entry->data;
        m_mailList.erase( i );
        i = m_mailList.begin();
    }
}

/*-------------------------------------------------------------------------*/

CLoggingTask::CLoggingMail::CLoggingMail( void )
    : CICloneable()                              ,
      logMsgType( CLogManager::LOG_UNKNOWNTYPE ) ,
      logLevel( LOGLEVEL_NORMAL )                ,
      logMessage()                               ,
      threadId( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CLoggingTask::CLoggingMail::CLoggingMail( const CLoggingMail& src )
    : CICloneable()                ,
      logMsgType( src.logMsgType ) ,
      logLevel( src.logLevel )     ,
      logMessage( src.logMessage ) ,
      threadId( src.threadId )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

MT::CICloneable*
CLoggingTask::CLoggingMail::Clone( void ) const
{GUCEF_TRACE;

    return new CLoggingMail( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
