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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_PATCHERAPP_CPATCHERAPPGLOBAL_H
#include "gucefPATCHERAPP_CPatcherAppGlobal.h"
#define GUCEF_PATCHERAPP_CPATCHERAPPGLOBAL_H
#endif /* GUCEF_PATCHERAPP_CPATCHERAPPGLOBAL_H ? */

#ifndef GUCEF_PATCHERAPP_CMAINPATCHERAPPLOGIC_H
#include "gucefPATCHERAPP_CMainPatcherAppLogic.h"
#define GUCEF_PATCHERAPP_CMAINPATCHERAPPLOGIC_H
#endif /* GUCEF_PATCHERAPP_CMAINPATCHERAPPLOGIC_H ? */

#include "gucefPATCHERAPP_main.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHERAPP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CORE::Int32 GUCEF_PLUGIN_CALLSPEC_PREFIX 
GUCEFPlugin_Load( CORE::UInt32 argc, const char** argv ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    return CPatcherAppGlobal::Instance()->GetMainLogic().Init( argc, argv );
}

/*--------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX 
GUCEFPlugin_Unload( void ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    CPatcherAppGlobal::Instance()->GetMainLogic().Shutdown();
    CPatcherAppGlobal::Deinstance();
}

/*--------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX 
GUCEFPlugin_GetVersion( GUCEF::CORE::TVersion* versionInfo ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( 0 != versionInfo )
    {
        versionInfo->major = 0; 
        versionInfo->minor = 1;
        versionInfo->patch = 0;
        versionInfo->release = 0;
    }
}

/*--------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX 
GUCEFPlugin_GetCopyright( void ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    return "Copyright (C) Dinand Vanvelzen, released under LGPLv3";
}

/*--------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX 
GUCEFPlugin_GetDescription( void ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    return "Patcher application module, providing updating/patching/deployment capabilities";
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHERAPP */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
