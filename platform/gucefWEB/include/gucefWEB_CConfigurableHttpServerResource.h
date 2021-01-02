/*
 *  gucefCOM: GUCEF module providing communication implementations 
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

#ifndef GUCEF_COM_CCONFIGURABLEHTTPSERVERRESOURCE_H
#define GUCEF_COM_CCONFIGURABLEHTTPSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefCOM_CCodecBasedHTTPServerResource.h"
#define GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_COM_PUBLIC_CPP CConfigurableHttpServerResource : public CCodecBasedHTTPServerResource
{    
    public:
    
    CConfigurableHttpServerResource( CORE::CIConfigurable* configurable = GUCEF_NULL );

    CConfigurableHttpServerResource( const CConfigurableHttpServerResource& src );
    
    virtual ~CConfigurableHttpServerResource();
    
    CConfigurableHttpServerResource& operator=( const CConfigurableHttpServerResource& src );

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CString& params               ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Deserialize the resource from the given data tree 
     *  @param isDeltaUpdateOnly Signals whether we are trying to deserialize a full resource in one go or just apply a delta update
     */
    virtual TDeserializeState Deserialize( const CORE::CDataNode& input  ,
                                           const CString& representation ,
                                           bool isDeltaUpdateOnly        ) GUCEF_VIRTUAL_OVERRIDE;

    protected:

    CORE::CIConfigurable* m_configurable;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CCONFIGURABLEHTTPSERVERRESOURCE_H ? */
