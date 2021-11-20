/*
 *  pluginglueAWSSDK: Library to support multiple AWS SDK based plugins that share overlap
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

#ifndef GUCEF_PLUGINGLUE_AWSSDK_CCLIENTCONFIGURATION_H
#define GUCEF_PLUGINGLUE_AWSSDK_CCLIENTCONFIGURATION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_PLUGINGLUE_AWSSDK_MACROS_H
#include "pluginglueAWSSDK_macros.h"
#define GUCEF_PLUGINGLUE_AWSSDK_MACROS_H
#endif /* GUCEF_PLUGINGLUE_AWSSDK_MACROS_H ? */

#ifndef GUCEF_DYNNEWOFF_H
#include "gucef_dynnewoff.h"
#define GUCEF_DYNNEWOFF_H
#endif /* GUCEF_DYNNEWOFF_H ? */

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PLUGINGLUE {
namespace AWSSDK {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/
                  
/**
 *  Adds convenient config loading to the SDK client config structure
 */
class GUCEF_PLUGINGLUE_AWSSDK_EXPORT_CPP CClientConfiguration : public Aws::Client::ClientConfiguration ,
                                                                public virtual CORE::CIConfigurable
{
    public:

    CClientConfiguration( void );
    CClientConfiguration( const CORE::CString& settingsPrefix );
    CClientConfiguration( const CClientConfiguration& src );
    virtual ~CClientConfiguration();
    CClientConfiguration& operator=( const CClientConfiguration& src );

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    void SetSettingsPrefix( const CORE::CString& prefix );

    const CORE::CString& GetSettingsPrefix( void ) const;

    private:

    CORE::CString m_settingsPrefix;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSDK */
}; /* namespace PLUGINGLUE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLUGINGLUE_AWSSDK_CCLIENTCONFIGURATION_H ? */
