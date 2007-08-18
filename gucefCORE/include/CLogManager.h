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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#define GUCEF_CORE_CLOGMANAGER_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <set>
#include <map>

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"           /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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

class CILogger;
class CString;

/*-------------------------------------------------------------------------*/

class GUCEFCORE_EXPORT_CPP CLogManager
{
    public:
    
    typedef enum TLogMsgType
    {
        LOG_ERROR,     /**< I strongly suggest you use this for logging serious errors only */
        LOG_STANDARD,  /**< can be anything */
        LOG_USER,      /**< typically info that relates to user actions */
        LOG_SYSTEM,    /**< typically info that relates to the state of of the system */
        LOG_DEV,       /**< typically info that will be of interest to developers */
        LOG_DEBUG,     /**< typically info that wont be logged in a release build, just for debugging */
        LOG_SERVICE,   /**< typically info useful to a service engineer */
        LOG_PROTECTED, /**< typically used for logging info that should be encrypted before being stored */
        LOG_CALLSTACK /**< typically used for logging the call stack of a running application for debugging purposes */
    };
    
    static CLogManager* Instance( void );
    
    void AddLogger( CILogger* loggerImp );
    
    void RemoveLogger( CILogger* loggerImp );
    
    void ClearLoggers( void );
    
    bool IsLoggingEnabled( const TLogMsgType logMsgType ,
                           const Int32 logLevel         ) const;
                           
    void Log( const TLogMsgType logMsgType ,
              const Int32 logLevel         ,
              const CString& logMessage    );

    void FlushLogs( void );
    
    private:
    friend class CGUCEFCOREModule;
    
    static void Deinstance( void );
    
    private:
    
    CLogManager( void );
    ~CLogManager();
    CLogManager( const CLogManager& src );              /**< not implemented, don't use */
    CLogManager& operator=( const CLogManager& src );   /**< not implemented, don't use */
    
    private:
    
    typedef std::set< CILogger* > TLoggerList;
    
    TLoggerList m_loggers;
    std::map< TLogMsgType, bool > m_msgTypeEnablers;
    Int32 m_maxLogLevel;
    static CLogManager* g_instance;
    static MT::CMutex g_dataLock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#undef GUCEF_ERROR_LOG
#define GUCEF_ERROR_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_ERROR, logLevel, logMessage );

/*-------------------------------------------------------------------------*/

#undef GUCEF_LOG
#define GUCEF_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_STANDARD, logLevel, logMessage );
#define GUCEF_STANDARD_LOG GUCEF_LOG

/*-------------------------------------------------------------------------*/

#undef GUCEF_USER_LOG
#define GUCEF_USER_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_USER, logLevel, logMessage );

/*-------------------------------------------------------------------------*/

#undef GUCEF_SYSTEM_LOG
#define GUCEF_SYSTEM_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_SYSTEM, logLevel, logMessage );

/*-------------------------------------------------------------------------*/

#undef GUCEF_DEV_LOG
#define GUCEF_DEV_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_DEV, logLevel, logMessage );

/*-------------------------------------------------------------------------*/

#undef GUCEF_SERVICE_LOG
#define GUCEF_SERVICE_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_SERVICE, logLevel, logMessage );

/*-------------------------------------------------------------------------*/

#undef GUCEF_PROTECTED_LOG
#define GUCEF_PROTECTED_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_PROTECTED, logLevel, logMessage );

/*-------------------------------------------------------------------------*/

#undef GUCEF_CALLSTACK_LOG
#define GUCEF_CALLSTACK_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_CALLSTACK, logLevel, logMessage );

/*-------------------------------------------------------------------------*/

#undef GUCEF_DEBUG_LOG
#ifdef GUCEF_CORE_DEBUG_MODE
#define GUCEF_DEBUG_LOG( logLevel, logMessage ) GUCEF::CORE::CLogManager::Instance()->Log( GUCEF::CORE::CLogManager::LOG_DEBUG, logLevel, logMessage );
#else
#define GUCEF_DEBUG_LOG
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

-----------------------------------------------------------------------------*/