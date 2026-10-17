/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#include <stdlib.h>

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestPluginUnload.h"

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace {

const char* TestPluginGroup = "testplugin_tests";

bool
LoadTestPlugin( void )
{
    #ifdef GUCEF_DEBUG_MODE
    const CORE::CString first = "$MODULEDIR$/gucefCORE_TestPlugin_d";
    const CORE::CString second = "$MODULEDIR$/gucefCORE_TestPlugin";
    #else
    const CORE::CString first = "$MODULEDIR$/gucefCORE_TestPlugin";
    const CORE::CString second = "$MODULEDIR$/gucefCORE_TestPlugin_d";
    #endif

    CORE::CPluginControl& pluginControl = CORE::CCoreGlobal::Instance()->GetPluginControl();
    return pluginControl.AddPluginFromDir( first, TestPluginGroup, true ) || pluginControl.AddPluginFromDir( second, TestPluginGroup, true );
}

/** How often the plugin was asked to load and to unload, as it counted */
int
Calls( const char* variable )
{
    const char* value = getenv( variable );
    return GUCEF_NULL != value ? atoi( value ) : 0;
}

/** The plugin is still known to the group, so it can be loaded again */
bool
IsKnownToGroup( void )
{
    CORE::CPluginControl::TStringSet modules;
    CORE::CCoreGlobal::Instance()->GetPluginControl().GetAvailablePluginsForGroup( TestPluginGroup, modules );
    return 1 == modules.size();
}

void
SetFlag( const char* variable, bool on )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    _putenv_s( variable, on ? "1" : "0" );
    #else
    setenv( variable, on ? "1" : "0", 1 );
    #endif
}

void
SetUnloadThrows( bool throws )
{
    SetFlag( "GUCEF_TESTPLUGIN_UNLOAD_THROWS", throws );
}

void
SetLoadThrows( bool throws )
{
    SetFlag( "GUCEF_TESTPLUGIN_LOAD_THROWS", throws );
}

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      TESTS                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformPluginUnloadTests( void )
{GUCEF_TRACE;

    GUCEF_TESTFW_SUITE_SCOPE( "gucefCORE.PluginUnload" );

    if ( !LoadTestPlugin() )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Plugin unload TESTS skipped: gucefCORE_TestPlugin is not available" );
        return;
    }

    GUCEF_TESTFW_TESTCASE( "PluginUnload: A generic plugin unloads and loads again" )
    try
    {
        CORE::CPluginControl& pluginControl = CORE::CCoreGlobal::Instance()->GetPluginControl();
        const int loads = Calls( "GUCEF_TESTPLUGIN_LOADS" ), unloads = Calls( "GUCEF_TESTPLUGIN_UNLOADS" );
        ASSERT_TRUE( loads >= 1 );
        ASSERT_TRUE( pluginControl.UnloadPluginGroup( TestPluginGroup ) );
        ASSERT_TRUE( unloads + 1 == Calls( "GUCEF_TESTPLUGIN_UNLOADS" ) );
        ASSERT_TRUE( IsKnownToGroup() );
        ASSERT_TRUE( pluginControl.LoadPluginGroup( TestPluginGroup ) );
        ASSERT_TRUE( loads + 1 == Calls( "GUCEF_TESTPLUGIN_LOADS" ) );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "PluginUnload: A plugin whose Unload throws is still unloaded, is not called again, and loads again" )
    try
    {
        CORE::CPluginControl& pluginControl = CORE::CCoreGlobal::Instance()->GetPluginControl();
        const int loads = Calls( "GUCEF_TESTPLUGIN_LOADS" ), unloads = Calls( "GUCEF_TESTPLUGIN_UNLOADS" );
        SetUnloadThrows( true );
        pluginControl.UnloadPluginGroup( TestPluginGroup );        // reports the broken plugin, it must not take the process down
        SetUnloadThrows( false );
        ASSERT_TRUE( unloads + 1 == Calls( "GUCEF_TESTPLUGIN_UNLOADS" ) );    // asked once, not again by the plugin object

        ASSERT_TRUE( IsKnownToGroup() );
        ASSERT_TRUE( pluginControl.LoadPluginGroup( TestPluginGroup ) );
        ASSERT_TRUE( loads + 1 == Calls( "GUCEF_TESTPLUGIN_LOADS" ) );
        ASSERT_TRUE( pluginControl.UnloadPluginGroup( TestPluginGroup ) );
    }
    catch ( ... )
    {
        SetUnloadThrows( false );
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "PluginUnload: A plugin whose Load throws is reported as not loaded, and loads once it works" )
    try
    {
        CORE::CPluginControl& pluginControl = CORE::CCoreGlobal::Instance()->GetPluginControl();
        const int loads = Calls( "GUCEF_TESTPLUGIN_LOADS" );
        SetLoadThrows( true );
        ASSERT_FALSE( pluginControl.LoadPluginGroup( TestPluginGroup ) );
        SetLoadThrows( false );
        ASSERT_TRUE( loads + 1 == Calls( "GUCEF_TESTPLUGIN_LOADS" ) );
        ASSERT_TRUE( IsKnownToGroup() );

        ASSERT_TRUE( pluginControl.LoadPluginGroup( TestPluginGroup ) );
        ASSERT_TRUE( loads + 2 == Calls( "GUCEF_TESTPLUGIN_LOADS" ) );
        ASSERT_TRUE( pluginControl.UnloadPluginGroup( TestPluginGroup ) );
    }
    catch ( ... )
    {
        SetLoadThrows( false );
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/
