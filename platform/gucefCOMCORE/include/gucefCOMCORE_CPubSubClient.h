/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CPUBSUBCLIENT_H
#define GUCEF_COMCORE_CPUBSUBCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CITYPENAMED_H
#include "CITypeNamed.h"
#define GUCEF_CORE_CITYPENAMED_H
#endif /* GUCEF_CORE_CITYPENAMED_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H
#include "gucefCOMCORE_CPubSubClientFeatures.h"    
#define GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#include "gucefCOMCORE_CPubSubClientTopic.h"    
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H ? */

#include "gucefCOMCORE_CPubSubClientTopicConfig.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/
 
/**
 *  Base class for all pub-sub client implementations
 */
class GUCEF_COMCORE_EXPORT_CPP CPubSubClient : public CORE::CObservingNotifier ,
                                               public virtual CORE::CIConfigurable
{
    public:

    CPubSubClient( void );

    CPubSubClient( const CPubSubClient& src );

    virtual ~CPubSubClient();

    CPubSubClient& operator=( const CPubSubClient& src );

    virtual bool GetSupportedFeatures( CPubSubClientFeatures& features ) = 0;

    virtual CPubSubClientTopic* CreateTopicAccess( const CPubSubClientTopicConfig& topicConfig ) = 0;

    virtual CPubSubClientTopic* GetTopicAccess( const CString& topicName ) = 0;

    virtual void GetTopicNameList( CString::StringSet& topicNameList ) = 0;

    virtual void DestroyTopicAccess( const CString& topicName ) = 0;

    virtual bool Connect( void ) = 0;

    virtual bool Disconnect( void ) = 0;

    virtual bool IsConnected( void ) = 0;

    /**
     *  Provide ability to get the textual name of the formatter
     */
    virtual const CString& GetType( void ) const = 0;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

typedef CORE::CTBasicSharedPtr< CPubSubClient, MT::CMutex > CPubSubClientPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPUBSUBCLIENT_H ? */

