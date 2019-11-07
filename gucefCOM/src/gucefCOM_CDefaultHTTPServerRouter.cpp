/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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
