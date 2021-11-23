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

#include "GucefLogServiceLib_CILogSvcServerLogger.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace LOGSERVICELIB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CILogSvcServerLogger::CILogSvcServerLogger( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CILogSvcServerLogger::~CILogSvcServerLogger()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CILogSvcServerLogger::CILogSvcServerLogger( const CILogSvcServerLogger& src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CILogSvcServerLogger&
CILogSvcServerLogger::operator=( const CILogSvcServerLogger& src )
{GUCEF_TRACE;
    
    return *this;
}

/*-------------------------------------------------------------------------*/
//
//CORE::CString
//CILogSvcServerLogger::FormatStdLogMessage( const bool logAppName                        ,
//                                           const bool logProcessName                    ,
//                                           const bool logProcessId                      ,
//                                           const CLogSvcServer::TClientInfo& clientInfo ,
//                                           const TLogMsgType logMsgType                 ,
//                                           const CORE::Int32 logLevel                   ,
//                                           const CORE::CString& logMessage              ,
//                                           const CORE::UInt32 threadId                  )
//{GUCEF_TRACE;
//
//    CORE::CString actualLogMsg;
//    if ( logAppName )
//    {
//        actualLogMsg += "[APPNAME=" + clientInfo.appName + "]";
//    }
//    if ( logProcessName )
//    {
//        actualLogMsg += "[PROCESSNAME=" + clientInfo.processName + "]";
//    }
//    if ( logProcessId )
//    {
//        actualLogMsg += "[PROCESSID=" + clientInfo.processId + "]";
//    }
//                        
//    actualLogMsg += CORE::CILogger::FormatStdLogMessage( logMsgType ,
//                                                         logLevel   ,
//                                                         logMessage ,
//                                                         threadId   );
//         
//    return actualLogMsg;
//}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace LOGSERVICELIB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/