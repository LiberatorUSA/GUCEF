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

#include "gucefWEB_CConfigurableHttpServerResource.h"

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
    
CConfigurableHttpServerResource::CConfigurableHttpServerResource( CORE::CIConfigurable* configurable /* = GUCEF_NULL */ )
    : CCodecBasedHTTPServerResource()
    , m_configurable( configurable )
{GUCEF_TRACE;

    m_allowDeserialize = true;
    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/
    
CConfigurableHttpServerResource::CConfigurableHttpServerResource( const CConfigurableHttpServerResource& src )
    : CCodecBasedHTTPServerResource( src )
    , m_configurable( src.m_configurable )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigurableHttpServerResource::~CConfigurableHttpServerResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigurableHttpServerResource& 
CConfigurableHttpServerResource::operator=( const CConfigurableHttpServerResource& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CCodecBasedHTTPServerResource::operator=( src );
        m_configurable = src.m_configurable;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CConfigurableHttpServerResource::Serialize( CORE::CDataNode& output             ,
                                            const CORE::CString& representation ,
                                            const CString& params               )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_configurable )
    {
        return m_configurable->SaveConfig( output );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CConfigurableHttpServerResource::TDeserializeState 
CConfigurableHttpServerResource::Deserialize( const CORE::CDataNode& input  ,
                                              const CString& representation ,
                                              bool isDeltaUpdateOnly        )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_configurable )
    {
        if ( m_configurable->LoadConfig( input ) )
            return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
        else
            return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
    }
    return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
