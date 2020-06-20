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

#ifndef GUCEF_CORE_CLOGGINGTASK_H
#define GUCEF_CORE_CLOGGINGTASK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CTMAILBOX_H
#include "gucefMT_CTMailBox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

#ifndef GUCEF_CORE_CTASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CTASKCONSUMER_H
#endif /* GUCEF_CORE_CTASKCONSUMER_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CILOGGER_H
#include "CILogger.h"
#define GUCEF_CORE_CILOGGER_H
#endif /* GUCEF_CORE_CILOGGER_H ? */

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

/**
 *  class which provides a threaded wrapper for logging backends
 *  You can either provide a backend to be called or you an derive from
 *  this class and implement the appropriote handler member functions.
 */
class GUCEF_CORE_PUBLIC_CPP CLoggingTask : public CTaskConsumer ,
                                           public CILogger
{
    public:

    /**
     *  Default constructor which allows you to create the task consumer
     *  consumer and then hook up a logger backend at a later time.
     */
    CLoggingTask( void );
    
    CLoggingTask( CILogger& loggerBackend );
    
    virtual ~CLoggingTask();

    /** 
     *  Adds a log message to the mailbox of the threaded logger.
     *  The actual logging backend invocation will be performed within
     *  the thread dedicated to logging.
     */
    virtual void Log( const TLogMsgType logMsgType ,
                      const Int32 logLevel         ,
                      const CString& logMessage    ,
                      const UInt32 threadId        ) GUCEF_VIRTUAL_OVERRIDE;

    /** 
     *  Adds a log message to the mailbox of the threaded logger.
     *  The actual logging backend invocation will be performed within
     *  the thread dedicated to logging.
     */
    virtual void LogWithoutFormatting( const TLogMsgType logMsgType ,
                                       const Int32 logLevel         ,
                                       const CString& logMessage    ,
                                       const UInt32 threadId        ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void FlushLog( void ) GUCEF_VIRTUAL_OVERRIDE;
    
    /**
     *  Utility function for convenience, launches the task using 
     *  the taskmanager
     */
    bool StartTask( void );

    /**
     *  Utility function for convenience, Stops the task using 
     *  the taskmanager
     */
    bool StopTask( void );

    /**
     *  Utility function for convenience, Pauses the task using 
     *  the taskmanager
     */
    bool PauseTask( void );

    /**
     *  Utility function for convenience, Pauses the task using 
     *  the taskmanager
     */
    bool ResumeTask( void );

    void SetLoggerBackend( CILogger& loggerBackend );

    /**
     *  Returns the type of task this task consumer can handle
     */
    virtual CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool OnTaskStart( CICloneable* taskdata ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CICloneable* taskdata ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnd( CICloneable* taskdata ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycleLog( const TLogMsgType logMsgType ,
                                 const Int32 logLevel         ,
                                 const CString& logMessage    ,
                                 const UInt32 threadId        );

    virtual bool OnTaskCycleLogWithoutFormatting( const TLogMsgType logMsgType ,
                                                  const Int32 logLevel         ,
                                                  const CString& logMessage    ,
                                                  const UInt32 threadId        );

    virtual bool OnTaskCycleLogFlush( void );

    virtual bool IsReadyToProcessCycle( void ) const;

    private:
    
    enum EMailType
    {
        MAILTYPE_NEWLOGMSG ,
        MAILTYPE_FLUSHLOG  ,
        
        MAILTYPE_UNKNOWN
    };
    typedef enum EMailType TMailType;
    typedef MT::CTMailBox< TMailType > TLoggingMailBox;
    typedef MT::CTMailBox< TMailType >::TMailList TMailList;

    class CLoggingMail : public CICloneable
    {
        public:
        
        TLogMsgType logMsgType;
        Int32 logLevel;
        CString logMessage;
        UInt32 threadId;
        bool withoutFormatting;
        
        virtual CICloneable* Clone( void ) const;
        
        CLoggingMail( void );
        
        CLoggingMail( const CLoggingMail& src );
    };

    private:
    
    CILogger* m_loggerBackend;
    TLoggingMailBox m_mailbox;
    TMailList m_mailList;    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CLOGGINGTASK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/
