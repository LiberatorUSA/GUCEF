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

#ifndef GUCEF_CORE_CGENERICPLUGINMANAGER_H
#include "CGenericPluginManager.h"
#define GUCEF_CORE_CGENERICPLUGINMANAGER_H
#endif /* GUCEF_CORE_CGENERICPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#include "gucefCORE_cinterface.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace ::GUCEF::CORE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

int
GUCEF_CORE_GucefMain( int argc       ,
                      char** argv    ,
                      int runAppBool )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Invoking Main via C interface" );
    return CGUCEFApplication::Instance()->main( argc, argv, runAppBool > 0 ); 
}

/*-------------------------------------------------------------------------*/

int
GUCEF_CORE_GucefLoadConfig( const char* configPath ,
                            const char* dataCodec  )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Invoking LoadConfig via C interface" );
    return 0;
}

/*-------------------------------------------------------------------------*/

int
GUCEF_CORE_GucefLoadPlugin( const char* pluginPath , 
                            const char* pluginType ,
                            const char* groupName  ,
                            int argc               , 
                            char** argv            )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Invoking LoadGenericPlugin via C interface" );
    
    CPluginMetaData pluginMetaData;
    pluginMetaData.SetLoaderLogicTypeName( "gucefLOADER" );
    pluginMetaData.SetPluginType( pluginType );

    CString path( pluginPath );
    pluginMetaData.SetModuleFilename( ExtractFilename( path ) );
    pluginMetaData.SetFullModulePath( path.CutChars( pluginMetaData.GetModuleFilename().Length(), false ) );

    CValueList pluginParams;
    pluginParams.SetMultiple( argc, argv );
    pluginMetaData.SetParams( pluginParams );

    if ( CPluginControl::Instance()->AddPluginMetaData( pluginMetaData ,
                                                        groupName      ,
                                                        true           ) )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Successfully loaded plugin via C interface" );
        return 1;
    }
    GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "Failed to load plugin via C interface" );
    return 0;
}

/*-------------------------------------------------------------------------*/

void
GUCEF_CORE_GucefAddPluginDir( const char* pluginDir )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Invoking AddPluginDir via C interface" );
    
    CString pluginDirStr( pluginDir );
    CPluginControl::Instance()->AddPluginDir( pluginDirStr );
}

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
GUCEF_CORE_GucefSetDefaultPluginLoaderLogicType( const char* defaultLoaderLogicType )
{GUCEF_TRACE;
    
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Invoking SetDefaultPluginLoaderLogicType via C interface" );

    CString defaultLoaderLogicTypeStr( defaultLoaderLogicType );
    CPluginControl::Instance()->SetDefaultPluginLoadLogicType( defaultLoaderLogicTypeStr );
}

/*-------------------------------------------------------------------------*/
