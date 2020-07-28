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

#include <queue>

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_COM_CHTTPSERVER_H
#include "gucefCOM_CHTTPServer.h"
#define GUCEF_COM_CHTTPSERVER_H
#endif /* GUCEF_COM_CHTTPSERVER_H ? */

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H
#include "gucefCOM_CDefaultHTTPServerRouter.h"
#define GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H
#endif /* GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H ? */

#ifndef GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefCOM_CCodecBasedHTTPServerResource.h"
#define GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class PackageUpgrader;

class RestApiPackageUpgraderPackagesResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiPackageUpgraderPackagesResource( PackageUpgrader* app );

    virtual ~RestApiPackageUpgraderPackagesResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation ) GUCEF_VIRTUAL_OVERRIDE;

    virtual TDeserializeState Deserialize( const CORE::CDataNode& input        ,
                                           const CORE::CString& representation ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    PackageUpgrader* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiPackageUpgraderInfoResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiPackageUpgraderInfoResource( PackageUpgrader* app );

    virtual ~RestApiPackageUpgraderInfoResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    PackageUpgrader* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiPackageUpgraderConfigResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiPackageUpgraderConfigResource( PackageUpgrader* app, bool appConfig );

    virtual ~RestApiPackageUpgraderConfigResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    PackageUpgrader* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class PackageUpgrader : public CORE::CObservingNotifier
{
    public:

    PackageUpgrader( void );
    virtual ~PackageUpgrader();

    bool Start( void );

    bool LoadConfig( const CORE::CValueList& appConfig   ,
                     const CORE::CDataNode& globalConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    private:

    void RegisterEventHandlers( void );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );
    
    private:

    typedef CORE::CTEventHandlerFunctor< PackageUpgrader > TEventCallback;

    COM::CHTTPServer m_httpServer;
    COM::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;

    CORE::CTimer m_metricsTimer;
    bool m_transmitMetrics;
};

/*-------------------------------------------------------------------------*/
