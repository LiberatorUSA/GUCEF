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

#ifndef GUCEF_WEB_CTDATANODESERIALIZABLEHTTPSERVERRESOURCEEXPANSION_H
#define GUCEF_WEB_CTDATANODESERIALIZABLEHTTPSERVERRESOURCEEXPANSION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_WEB_CDATANODESERIALIZABLEHTTPSERVERRESOURCE_H
#include "gucefWEB_CDataNodeSerializableHttpServerResource.h"
#define GUCEF_WEB_CDATANODESERIALIZABLEHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDATANODESERIALIZABLEHTTPSERVERRESOURCE_H ? */

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

/**
 *  Helper template to wrap a DataNodeSerializable object as a HttpServerResource
 */
template < class T >
class CTDataNodeSerializableHttpServerResourceExpansion : public CDataNodeSerializableHttpServerResource
{
    public:

    CTDataNodeSerializableHttpServerResourceExpansion( void );
    
    virtual ~CTDataNodeSerializableHttpServerResourceExpansion() GUCEF_VIRTUAL_OVERRIDE;


    protected:

    T m_serializableObj;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

template < class T >
CTDataNodeSerializableHttpServerResourceExpansion< T >::CTDataNodeSerializableHttpServerResourceExpansion( void )
    : CDataNodeSerializableHttpServerResource()
{GUCEF_TRACE;
    
    LinkTo( GUCEF_NULL, &m_serializableObj, GUCEF_NULL );
}

/*-------------------------------------------------------------------------*/

template < class T >
CTDataNodeSerializableHttpServerResourceExpansion< T >::~CTDataNodeSerializableHttpServerResourceExpansion( void )
{GUCEF_TRACE;
    
    Unlink();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace WEB */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CTDATANODESERIALIZABLEHTTPSERVERRESOURCEEXPANSION_H ? */
