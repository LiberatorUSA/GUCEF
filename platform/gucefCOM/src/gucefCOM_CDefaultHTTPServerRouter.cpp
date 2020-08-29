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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCOM_CDefaultHTTPServerRouter.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDefaultHTTPServerRouter::CDefaultHTTPServerRouter( void )
    : CIHTTPServerRouter()
    , m_mountPath()
    , m_serviceRoot()
    , m_controller( GUCEF_NULL )
    , m_resourceMap()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerRouter::CDefaultHTTPServerRouter( const CDefaultHTTPServerRouter& src )
    : CIHTTPServerRouter( src )
    , m_mountPath( src.m_mountPath )
    , m_serviceRoot( src.m_serviceRoot )
    , m_controller( src.m_controller )
    , m_resourceMap( src.m_resourceMap )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerRouter::~CDefaultHTTPServerRouter()
{GUCEF_TRACE;

    m_resourceMap.clear();
    m_controller = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerRouter&
CDefaultHTTPServerRouter::operator=( const CDefaultHTTPServerRouter& src )
{GUCEF_TRACE;

    m_mountPath = src.m_mountPath;
    m_serviceRoot = src.m_serviceRoot;
    m_controller = src.m_controller;
    m_resourceMap = src.m_resourceMap;
    return *this;
}

/*-------------------------------------------------------------------------*/

CIHTTPServerRouter::THTTPServerResourcePtr 
CDefaultHTTPServerRouter::ResolveUriToResource( const CString& uri )
{GUCEF_TRACE;

    TResourceMap::iterator i = m_resourceMap.find( uri );
    if ( i != m_resourceMap.end() )
    {
        return (*i).second;
    }
    return CIHTTPServerRouter::THTTPServerResourcePtr();    
}

/*-------------------------------------------------------------------------*/

bool 
CDefaultHTTPServerRouter::SetResourceMapping( const CString& uriSegment       ,
                                              THTTPServerResourcePtr resource )
{GUCEF_TRACE;

    m_resourceMap[ uriSegment ] = resource;
    resource->SetURL( uriSegment );
    return true;    
}

/*-------------------------------------------------------------------------*/

bool 
CDefaultHTTPServerRouter::RemoveResourceMapping( const CString& uriSegment )
{GUCEF_TRACE;

    TResourceMap::iterator i = m_resourceMap.find( uriSegment );
    if ( i != m_resourceMap.end() )
    {
        m_resourceMap.erase( i );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
void
CDefaultHTTPServerRouter::SetServiceRoot( const CString& serviceRoot )
{GUCEF_TRACE;
    
    m_serviceRoot = serviceRoot;
}

/*-------------------------------------------------------------------------*/
    
const CString& 
CDefaultHTTPServerRouter::GetServiceRoot( void ) const
{GUCEF_TRACE;

    return m_serviceRoot;
}

/*-------------------------------------------------------------------------*/
    
void 
CDefaultHTTPServerRouter::SetMountPath( const CString& mountPath )
{GUCEF_TRACE;

    m_mountPath = mountPath;
}

/*-------------------------------------------------------------------------*/
    
const CString& 
CDefaultHTTPServerRouter::GetMountPath( void ) const
{GUCEF_TRACE;

    return m_mountPath;
}

/*-------------------------------------------------------------------------*/
    
void 
CDefaultHTTPServerRouter::SetRouterController( CIHTTPServerRouterController& controller )
{GUCEF_TRACE;

    m_controller = &controller;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
