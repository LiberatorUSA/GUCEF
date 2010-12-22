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

#include "gucefCORE_CPlatformNativeConsoleLogger.h"

#if GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32

#ifndef GUCEF_CORE_CMSWINCONSOLELOGGER_H
#include "CMSWinConsoleLogger.h"
#define GUCEF_CORE_CMSWINCONSOLELOGGER_H
#endif /* GUCEF_CORE_CMSWINCONSOLELOGGER_H ? */

#elif GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX

#ifndef GUCEF_CORE_CXTERMCONSOLELOGGER_H
#include "CXTermConsoleLogger.h"
#define GUCEF_CORE_CXTERMCONSOLELOGGER_H
#endif /* GUCEF_CORE_CXTERMCONSOLELOGGER_H ? */

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPlatformNativeConsoleLogger::CPlatformNativeConsoleLogger( void )
    : m_nativeConsoleLogger( NULL )
{
    #if GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32

    m_nativeConsoleLogger = new CMSWinConsoleLogger();

    #elif GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX

    m_nativeConsoleLogger = new CXTermConsoleLogger();

    #endif
}

/*-------------------------------------------------------------------------*/

CPlatformNativeConsoleLogger::~CPlatformNativeConsoleLogger()
{
    delete m_nativeConsoleLogger;
    m_nativeConsoleLogger = NULL;
}

/*-------------------------------------------------------------------------*/

CILogger*
CPlatformNativeConsoleLogger::GetLogger( void )
{
    return m_nativeConsoleLogger;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
