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

#ifndef GUCEF_CORE_CPLATFORMNATIVECONSOLELOGGER_H
#define GUCEF_CORE_CPLATFORMNATIVECONSOLELOGGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICONSOLELOGGER_H
#include "gucefCORE_CIConsoleLogger.h"
#define GUCEF_CORE_CICONSOLELOGGER_H
#endif /* GUCEF_CORE_CICONSOLELOGGER_H ? */

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
 *  Class which uses the strategy pattern to allocate a platform
 *  native console logger which can then be obtained using GetLogger().
 *  Note that it does not automatically register the logger with the 
 *  log manager.
 */
class GUCEF_CORE_PUBLIC_CPP CPlatformNativeConsoleLogger
{
    public:
    
    CPlatformNativeConsoleLogger( void );

    ~CPlatformNativeConsoleLogger();

    CIConsoleLogger* GetLogger( void );

    private:

    CPlatformNativeConsoleLogger& operator=( const CPlatformNativeConsoleLogger& src );
    CPlatformNativeConsoleLogger( const CPlatformNativeConsoleLogger& src );

    private:

    CIConsoleLogger* m_nativeConsoleLogger;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPLATFORMNATIVECONSOLELOGGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 19-05-2007 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/
