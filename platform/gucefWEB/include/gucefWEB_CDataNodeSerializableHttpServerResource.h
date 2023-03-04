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

#ifndef GUCEF_WEB_CDATANODESERIALIZABLEHTTPSERVERRESOURCE_H
#define GUCEF_WEB_CDATANODESERIALIZABLEHTTPSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H
#include "gucefMT_CObjectScopeReadOnlyLock.h"
#define GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H
#endif /* GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H ? */

#ifndef GUCEF_MT_CNOLOCK_H
#include "gucefMT_CNoLock.h"
#define GUCEF_MT_CNOLOCK_H
#endif /* GUCEF_MT_CNOLOCK_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CIDATANODESERIALIZABLE_H
#include "gucefCORE_CIDataNodeSerializable.h"
#define GUCEF_CORE_CIDATANODESERIALIZABLE_H
#endif /* GUCEF_CORE_CIDATANODESERIALIZABLE_H ? */

#ifndef GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#include "gucefCORE_CDataNodeSerializableSettings.h"
#define GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#endif /* GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H ? */

#ifndef GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefWEB_CCodecBasedHTTPServerResource.h"
#define GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_WEB_CSTDPARAMPARSER_H
#include "gucefWEB_CStdParamParser.h"
#define GUCEF_WEB_CSTDPARAMPARSER_H
#endif /* GUCEF_WEB_CSTDPARAMPARSER_H ? */

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

class CDataNodeSerializableHttpServerResource : public CCodecBasedHTTPServerResource
{
    public:

    CDataNodeSerializableHttpServerResource( void );
    
    CDataNodeSerializableHttpServerResource( CORE::CIDataNodeSerializable* serializable                                ,
                                             const CORE::CDataNodeSerializableSettings* serializerOptions = GUCEF_NULL ,
                                             MT::CILockable* serializableLock = GUCEF_NULL                             );

    virtual ~CDataNodeSerializableHttpServerResource() GUCEF_VIRTUAL_OVERRIDE;

    void LinkTo( const CORE::CDataNodeSerializableSettings* serializerOptions ,
                 CORE::CIDataNodeSerializable* serializable                   ,
                 MT::CILockable* serializableLock                             );

    virtual bool Serialize( const CString& resourcePath         ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& query          ) GUCEF_VIRTUAL_OVERRIDE;

    virtual TDeserializeState Deserialize( const CString& resourcePath   ,
                                           const CORE::CDataNode& input  ,
                                           const CString& representation ,
                                           bool isDeltaUpdateOnly        ) GUCEF_VIRTUAL_OVERRIDE;

    protected:

    const CORE::CDataNodeSerializableSettings* m_serializerOptions;
    CORE::CIDataNodeSerializable* m_serializable;
    MT::CILockable* m_serializableLock;

    private:

    CDataNodeSerializableHttpServerResource( const CDataNodeSerializableHttpServerResource& src );            /**< not implemented, dont use */
    CDataNodeSerializableHttpServerResource& operator=( const CDataNodeSerializableHttpServerResource& src ); /**< not implemented, dont use */
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CDataNodeSerializableHttpServerResource, MT::CMutex >    CDataNodeSerializableHttpServerResourcePtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CDATANODESERIALIZABLEHTTPSERVERRESOURCE_H ? */
