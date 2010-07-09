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

#ifndef GUCEF_CORE_CTHREADEDLOGGER_H
#define GUCEF_CORE_CTHREADEDLOGGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CACTIVEOBJECT_H
#include "gucefMT_CActiveObject.h"
#define GUCEF_MT_CACTIVEOBJECT_H
#endif /* GUCEF_MT_CACTIVEOBJECT_H */

#ifndef GUCEF_MT_CTMAILBOX_H
#include "gucefMT_CTMailBox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

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
 */
class GUCEF_CORE_PUBLIC_CPP CThreadedLogger : public MT::CActiveObject ,
                                              public CILogger
{
    public:
    
    CThreadedLogger( void );

    CThreadedLogger( CILogger* loggerBackend );

    /** 
     *
     */
    virtual void Log( const TLogMsgType logMsgType ,
                      const Int32 logLevel         ,
                      const CString& logMessage    ,
                      const UInt32 threadId        );

    virtual void FlushLog( void );
    
    void SetBackend( CILogger* loggerBackend );

    protected:

    virtual bool OnTaskStart( void* taskdata );

    virtual bool OnTaskCycle( void* taskdata );

    virtual void OnTaskEnd( void* taskdata );
    
    void LockData( void );
    
    void UnlockData( void );

    private:
    
    enum EMailType
    {
        MAILTYPE_NEWLOGMSG ,
        MAILTYPE_FLUSHLOG  ,
        
        MAILTYPE_UNKNOWN
    };
    typedef enum EMailType TMailType;
    typedef MT::CTMailBox< TMailType > TLoggingMailBox;
    
    class CLoggingMail : public CICloneable
    {
        public:
        
        TLogMsgType logMsgType;
        Int32 logLevel;
        CString logMessage;
        UInt32 threadId;
        
        virtual CICloneable* Clone( void ) const;
    };
    
    private:
    
    CILogger* m_loggerBackend;
    TLoggingMailBox m_mailbox;    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTHREADEDLOGGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

-----------------------------------------------------------------------------*/
