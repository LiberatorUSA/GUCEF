/*
 *  gucefWEB: GUCEF module providing Web application functionality 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CSTDCODECPLUGINMANAGER_H
#include "CStdCodecPluginManager.h"
#define GUCEF_CORE_CSTDCODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CSTDCODECPLUGINMANAGER_H ? */

#include "gucefWEB_CGlobalHttpCodecLinks.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CGlobalHttpCodecLinks::CGlobalHttpCodecLinks( void )
    : CHttpCodecLinks()
{GUCEF_TRACE;

    CORE::CConfigStore& configStore = CORE::CCoreGlobal::Instance()->GetConfigStore();
    CORE::CGUCEFApplication& app = CORE::CCoreGlobal::Instance()->GetApplication();
    CORE::CStdCodecPluginManager& codecPluginMngr = CORE::CCoreGlobal::Instance()->GetStdCodecPluginManager();

    TEventCallback callback( this, &CGlobalHttpCodecLinks::OnEventThatMightHaveChangedCodecs );
    SubscribeTo( &configStore, configStore.GlobalConfigLoadCompletedEvent, callback );

    TEventCallback callback2( this, &CGlobalHttpCodecLinks::OnEventThatMightHaveChangedCodecs );
    SubscribeTo( &app, app.FirstCycleEvent, callback2 );

    TEventCallback callback3( this, &CGlobalHttpCodecLinks::OnEventThatMightHaveChangedEncodeCodecs );
    SubscribeTo( &codecPluginMngr, codecPluginMngr.StdCodecRegisteredEvent, callback3 );

    TEventCallback callback4( this, &CGlobalHttpCodecLinks::OnEventThatMightHaveChangedEncodeCodecs );
    SubscribeTo( &codecPluginMngr, codecPluginMngr.StdCodecUnregisteredEvent, callback4 );
}

/*-------------------------------------------------------------------------*/

CGlobalHttpCodecLinks::~CGlobalHttpCodecLinks()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void 
CGlobalHttpCodecLinks::OnEventThatMightHaveChangedMimeCodecs( CORE::CNotifier* notifier    ,
                                                              const CORE::CEvent& eventid  ,
                                                              CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    InitMimeCodecLinks();
}

/*-------------------------------------------------------------------------*/

void 
CGlobalHttpCodecLinks::OnEventThatMightHaveChangedEncodeCodecs( CORE::CNotifier* notifier    ,
                                                                const CORE::CEvent& eventid  ,
                                                                CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    InitEncodingCodecLinks();
}

/*-------------------------------------------------------------------------*/

void 
CGlobalHttpCodecLinks::OnEventThatMightHaveChangedCodecs( CORE::CNotifier* notifier    ,
                                                          const CORE::CEvent& eventid  ,
                                                          CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    InitMimeCodecLinks();
    InitEncodingCodecLinks();
}

/*-------------------------------------------------------------------------*/

const CString& 
CGlobalHttpCodecLinks::GetClassTypeName( void ) const
{
    static const CORE::CString classTypeName = "GUCEF::COM::CGlobalHttpCodecLinks";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
