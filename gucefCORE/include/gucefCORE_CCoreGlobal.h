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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#define GUCEF_CORE_CCOREGLOBAL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_CSYSCONSOLE_H
#include "CSysConsole.h"
#define GUCEF_CORE_CSYSCONSOLE_H
#endif /* GUCEF_CORE_CSYSCONSOLE_H ? */

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#include "CURLHandlerRegistry.h"
#define GUCEF_CORE_CURLHANDLERREGISTRY_H
#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#include "CDStoreCodecPluginManager.h"
#define GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CGENERICPLUGINMANAGER_H
#include "CGenericPluginManager.h"
#define GUCEF_CORE_CGENERICPLUGINMANAGER_H
#endif /* GUCEF_CORE_CGENERICPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CSTDCODECPLUGINMANAGER_H
#include "CStdCodecPluginManager.h"
#define GUCEF_CORE_CSTDCODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CSTDCODECPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H
#include "CExclusiveActivationManager.h"
#define GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H
#endif /* GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

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
 *  Singular singleton providing access to all global Core systems
 */
class GUCEF_CORE_PUBLIC_CPP CCoreGlobal
{
    public:

    static CCoreGlobal* Instance( void );

    CGUCEFApplication& GetApplication( void );
    
    /**
     *  Convenience function providing access to the Application
     *  pulse generator
     */
    CPulseGenerator& GetPulseGenerator( void );

    CTaskManager& GetTaskManager( void );

    CLogManager& GetLogManager( void );

    CPluginControl& GetPluginControl( void );

    CSysConsole& GetSysConsole( void );

    CConfigStore& GetConfigStore( void );

    CNotificationIDRegistry& GetNotificationIDRegistry( void );

    CURLHandlerRegistry& GetUrlHandlerRegistry( void );

    CDStoreCodecRegistry& GetDStoreCodecRegistry( void );

    CDStoreCodecPluginManager& GetDStoreCodecPluginManager( void );

    CGenericPluginManager& GetGenericPluginManager( void );

    CStdCodecPluginManager& GetStdCodecPluginManager( void );

    CExclusiveActivationManager& GetExclusiveActivationManager( void );

    private:
    friend class CGUCEFCOREModule;

    static void Deinstance( void );

    private:

    CTaskManager* m_taskManager;
    CURLHandlerRegistry* m_urlHandlerRegistry;
    CDStoreCodecRegistry* m_dstoreCodecRegistry;
    CExclusiveActivationManager* m_exclusiveActivationManager;
    CGUCEFApplication* m_application;
    CLogManager* m_logManager;
    CDStoreCodecPluginManager* m_dstoreCodecPluginManager;
    CGenericPluginManager* m_genericPluginManager;
    CPluginControl* m_pluginControl;
    CSysConsole* m_sysConsole;
    CNotificationIDRegistry* m_notificationIdRegistry;
    CStdCodecPluginManager* m_stdCodecPluginManager;
    CConfigStore* m_configStore;

    static CCoreGlobal* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/
