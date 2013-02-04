/*
 *  gucefPATCHERAPP: Application module for the patcher application
 *  Copyright (C) 2002 - 2013.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_PATCHERAPP_CPATCHERAPPGLOBAL_H
#define GUCEF_PATCHERAPP_CPATCHERAPPGLOBAL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PATCHERAPP_MACROS_H
#include "gucefPATCHERAPP_macros.h"
#define GUCEF_PATCHERAPP_MACROS_H
#endif /* GUCEF_PATCHERAPP_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHERAPP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  Forward declarations of global classes
 */
class CPatcherAppConfig;
class CMainPatcherAppLogic;

/*-------------------------------------------------------------------------*/

class CPatcherAppGlobal
{
    public:

    CPatcherAppConfig& GetConfig( void );

    CMainPatcherAppLogic& GetMainLogic( void );
    
    static CPatcherAppGlobal* Instance( void );

    static void Deinstance( void );

    private:

    CPatcherAppGlobal( void );

    virtual ~CPatcherAppGlobal();

    CPatcherAppGlobal( const CPatcherAppGlobal& src );

    CPatcherAppGlobal& operator=( const CPatcherAppGlobal& src );
    
    void Init( void );

    private:

    static CPatcherAppGlobal* g_instance;
    CPatcherAppConfig* m_config;
    CMainPatcherAppLogic* m_mainLogic;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHERAPP */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHERAPP_CPATCHERAPPGLOBAL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-01-2013 :
        - Dinand: Initial version.

---------------------------------------------------------------------------*/