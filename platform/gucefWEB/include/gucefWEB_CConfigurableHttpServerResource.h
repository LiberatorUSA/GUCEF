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

#ifndef GUCEF_WEB_CCONFIGURABLEHTTPSERVERRESOURCE_H
#define GUCEF_WEB_CCONFIGURABLEHTTPSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefWEB_CCodecBasedHTTPServerResource.h"
#define GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_WEB_PUBLIC_CPP CConfigurableHttpServerResource : public CCodecBasedHTTPServerResource
{    
    public:
    
    CConfigurableHttpServerResource( CORE::CIConfigurable* configurable = GUCEF_NULL  ,
                                     bool allowSerialize = true                       ,
                                     bool allowDeserialize = true                     ,
                                     MT::CILockable* lockForConfigurable = GUCEF_NULL );

    CConfigurableHttpServerResource( const CConfigurableHttpServerResource& src );
    
    virtual ~CConfigurableHttpServerResource();
    
    CConfigurableHttpServerResource& operator=( const CConfigurableHttpServerResource& src );

    virtual bool Serialize( const CString& resourcePath         ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CString& params               ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Deserialize the resource from the given data tree 
     *  @param isDeltaUpdateOnly Signals whether we are trying to deserialize a full resource in one go or just apply a delta update
     */
    virtual TDeserializeState Deserialize( const CString& resourcePath   ,
                                           const CORE::CDataNode& input  ,
                                           const CString& representation ,
                                           bool isDeltaUpdateOnly        ) GUCEF_VIRTUAL_OVERRIDE;

    protected:

    CORE::CIConfigurable* m_configurable;
    MT::CILockable* m_lockForConfigurable;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CCONFIGURABLEHTTPSERVERRESOURCE_H ? */
