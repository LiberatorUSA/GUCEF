/*
 *  PackageUpgrader: micro-service for exposing an HTTP endpoint for package upgrades
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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefCOM_CDummyHTTPServerResource.h"
#define GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H ? */

#include "PackageUpgrader.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <winsock2.h>
#endif

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/


RestApiPackageUpgraderPackagesResource::RestApiPackageUpgraderPackagesResource( PackageUpgrader* app )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiPackageUpgraderPackagesResource::~RestApiPackageUpgraderPackagesResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiPackageUpgraderPackagesResource::Serialize( CORE::CDataNode& output             ,
                                                   const CORE::CString& representation )
{GUCEF_TRACE;


    return true;
}

/*-------------------------------------------------------------------------*/

RestApiPackageUpgraderPackagesResource::TDeserializeState 
RestApiPackageUpgraderPackagesResource::Deserialize( const CORE::CDataNode& input        ,
                                                     const CORE::CString& representation ) 
{GUCEF_TRACE;


    return DESERIALIZESTATE_UNABLETOUPDATE;
}

/*-------------------------------------------------------------------------*/

RestApiPackageUpgraderInfoResource::RestApiPackageUpgraderInfoResource( PackageUpgrader* app )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiPackageUpgraderInfoResource::~RestApiPackageUpgraderInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiPackageUpgraderInfoResource::Serialize( CORE::CDataNode& output             ,
                                               const CORE::CString& representation )
{GUCEF_TRACE;

    output.SetName( "info" );
    output.SetAttribute( "application", "PackageUpgrader" );
    output.SetAttribute( "buildDateTime", __TIMESTAMP__ );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiPackageUpgraderConfigResource::RestApiPackageUpgraderConfigResource( PackageUpgrader* app, bool appConfig )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiPackageUpgraderConfigResource::~RestApiPackageUpgraderConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiPackageUpgraderConfigResource::Serialize( CORE::CDataNode& output             ,
                                                 const CORE::CString& representation )
{GUCEF_TRACE;

    if ( m_appConfig )
    {
        const CORE::CValueList& loadedConfig = m_app->GetAppConfig();
        return loadedConfig.SaveConfig( output );
    }

    const CORE::CDataNode& globalConfig = m_app->GetGlobalConfig();
    output.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

PackageUpgrader::PackageUpgrader( void )
    : m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_transmitMetrics( true )
{GUCEF_TRACE;

    RegisterEventHandlers();    
}

/*-------------------------------------------------------------------------*/

PackageUpgrader::~PackageUpgrader()
{GUCEF_TRACE;

    m_httpServer.Close();
}

/*-------------------------------------------------------------------------*/

void
PackageUpgrader::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &PackageUpgrader::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );

}

/*-------------------------------------------------------------------------*/

void
PackageUpgrader::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventId  ,
                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_transmitMetrics )
    {

    }
}

/*-------------------------------------------------------------------------*/

bool
PackageUpgrader::Start( void )
{GUCEF_TRACE;

    if ( m_transmitMetrics )
    {
        m_metricsTimer.SetInterval( 1000 );
        m_metricsTimer.SetEnabled( true );
    }  
    
    if ( m_httpServer.Listen() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PackageUpgrader: Opened REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        return true;
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PackageUpgrader: Failed to open REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
    return false;

}

/*-------------------------------------------------------------------------*/

bool 
PackageUpgrader::LoadConfig( const CORE::CValueList& appConfig   ,
                       const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    CORE::CString modeAsString = appConfig.GetValueAlways( "Mode", "Receiver" ).Lowercase();
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PackageUpgrader: Application mode: " + modeAsString );
        
    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort", "10000" ) ) ) );

    m_httpRouter.SetResourceMapping( "/packages", RestApiPackageUpgraderPackagesResource::THTTPServerResourcePtr( new RestApiPackageUpgraderPackagesResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/info", RestApiPackageUpgraderInfoResource::THTTPServerResourcePtr( new RestApiPackageUpgraderInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiPackageUpgraderInfoResource::THTTPServerResourcePtr( new RestApiPackageUpgraderConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiPackageUpgraderInfoResource::THTTPServerResourcePtr( new RestApiPackageUpgraderConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping(  appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ), RestApiPackageUpgraderInfoResource::THTTPServerResourcePtr( new COM::CDummyHTTPServerResource() )  );
    
    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList& 
PackageUpgrader::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode& 
PackageUpgrader::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/