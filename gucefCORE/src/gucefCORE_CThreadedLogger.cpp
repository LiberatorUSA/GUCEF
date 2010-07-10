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

CThreadedLogger::CThreadedLogger( CILogger& loggerBackend )
    : MT::CActiveObject()               ,
      CILogger()                        ,
      m_loggerBackend( &loggerBackend ) ,
      m_mailbox()                       ,
      m_mailList()
{GUCEF_TRACE;

    m_mailList.reserve( MAXMAILITEMSPERCYCLE );
}

/*-------------------------------------------------------------------------*/

CThreadedLogger::~CThreadedLogger()
{GUCEF_TRACE;
    
    Deactivate( true );
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

bool
CThreadedLogger::OnTaskStart( void* taskdata )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CThreadedLogger::OnTaskCycle( void* taskdata )
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

CThreadedLogger::CLoggingMail::CLoggingMail( void )
    : CICloneable()                              ,
      logMsgType( CLogManager::LOG_UNKNOWNTYPE ) ,
      logLevel( LOGLEVEL_NORMAL )                ,
      logMessage()                               ,
      threadId( 0 )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CThreadedLogger::CLoggingMail::CLoggingMail( const CLoggingMail& src )
    : CICloneable()                ,
      logMsgType( src.logMsgType ) ,
      logLevel( src.logLevel )     ,
      logMessage( src.logMessage ) ,
      threadId( src.threadId )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

MT::CICloneable*
CThreadedLogger::CLoggingMail::Clone( void ) const
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
