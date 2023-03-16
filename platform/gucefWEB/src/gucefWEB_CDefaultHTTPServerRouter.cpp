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

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

#include "gucefWEB_CDefaultHTTPServerRouter.h"

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

CDefaultHTTPServerRouter::CDefaultHTTPServerRouter( void )
    : CIHTTPServerRouter()
    , m_mountPath()
    , m_serviceRoot()
    , m_controller( GUCEF_NULL )
    , m_resourceTree()
    , m_resourceMap()
    , m_uriIsCaseSensitive( true ) // Default because RFC 3986 defines URIs as case-sensitive except for the scheme and host components
    , m_wildcardMatchUris( true )
    , m_rwLock( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerRouter::CDefaultHTTPServerRouter( const CDefaultHTTPServerRouter& src )
    : CIHTTPServerRouter( src )
    , m_mountPath( src.m_mountPath )
    , m_serviceRoot( src.m_serviceRoot )
    , m_controller( src.m_controller )
    , m_resourceTree()
    , m_resourceMap( src.m_resourceMap )
    , m_uriIsCaseSensitive( src.m_uriIsCaseSensitive )
    , m_wildcardMatchUris( src.m_wildcardMatchUris )
    , m_rwLock( src.m_rwLock )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerRouter::~CDefaultHTTPServerRouter()
{GUCEF_TRACE;

    MT::CScopeWriterLock writeLock( m_rwLock );

    m_resourceTree.Clear();
    m_resourceMap.clear();
    m_controller = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerRouter&
CDefaultHTTPServerRouter::operator=( const CDefaultHTTPServerRouter& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        MT::CScopeWriterLock writeLock( m_rwLock );

        m_mountPath = src.m_mountPath;
        m_serviceRoot = src.m_serviceRoot;
        m_controller = src.m_controller;
        m_resourceTree = src.m_resourceTree;
        m_resourceMap = src.m_resourceMap;
        m_uriIsCaseSensitive = src.m_uriIsCaseSensitive;
        m_wildcardMatchUris = src.m_wildcardMatchUris;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CDefaultHTTPServerRouter::SetWildcardMatchUris( bool useWildcards )
{GUCEF_TRACE;
    
    m_wildcardMatchUris = useWildcards;
}

/*-------------------------------------------------------------------------*/

bool
CDefaultHTTPServerRouter::GetWildcardMatchUris( void ) const
{GUCEF_TRACE;

    return m_wildcardMatchUris;
}

/*-------------------------------------------------------------------------*/

CIHTTPServerRouter::THTTPServerResourcePtr 
CDefaultHTTPServerRouter::ResolveUriToResource( const CString& uri )
{GUCEF_TRACE;

    MT::CScopeReaderLock readLock( m_rwLock );

    CORE::CDataNode* resourceNode = GUCEF_NULL;
    if ( m_uriIsCaseSensitive )
    {
        resourceNode = m_resourceTree.Search( uri, '/', true, true, m_wildcardMatchUris, '*' );
    }
    else
    {
        CString lcUri = uri.Lowercase();
        resourceNode = m_resourceTree.Search( lcUri, '/', true, true, m_wildcardMatchUris, '*' );
    }

    if ( GUCEF_NULL != resourceNode )
    {
        THTTPServerResourcePtr* ptrPtr = static_cast< THTTPServerResourcePtr* >( resourceNode->GetAssociatedData() );
        if ( GUCEF_NULL != ptrPtr )
        {
            return *ptrPtr;
        }
    }
    return CIHTTPServerRouter::THTTPServerResourcePtr();   
}

/*-------------------------------------------------------------------------*/

bool 
CDefaultHTTPServerRouter::SetResourceMapping( const CString& uriSegment       ,
                                              THTTPServerResourcePtr resource )
{GUCEF_TRACE;

    MT::CScopeWriterLock writeLock( m_rwLock );
    
    CORE::CDataNode* resourceNode = GUCEF_NULL;
    if ( m_uriIsCaseSensitive )
    {
        resourceNode = m_resourceTree.Structure( uriSegment, '/' );
    }
    else
    {
        CString lcuriSegment = uriSegment.Lowercase();
        resourceNode = m_resourceTree.Structure( lcuriSegment, '/' );
    }

    if ( GUCEF_NULL != resourceNode )
    {
        THTTPServerResourcePtr& rscPtr = m_resourceMap[ uriSegment ];
        rscPtr = resource;
        resourceNode->SetAssociatedData( &rscPtr );    
        resource->SetURL( uriSegment );
        return true; 
    }
    return false;  
}

/*-------------------------------------------------------------------------*/

bool 
CDefaultHTTPServerRouter::RemoveResourceMapping( const CString& uriSegment )
{GUCEF_TRACE;

    MT::CScopeWriterLock writeLock( m_rwLock );
    
    CORE::CDataNode* resourceNode = GUCEF_NULL;
    if ( m_uriIsCaseSensitive )
    {
        resourceNode = m_resourceTree.Search( uriSegment, '/', true, false );
    }
    else
    {
        CString lcUriSegment = uriSegment.Lowercase();
        resourceNode = m_resourceTree.Search( lcUriSegment, '/', true, false );
    }

    if ( GUCEF_NULL != resourceNode )
    {
        resourceNode->SetAssociatedData( GUCEF_NULL );
        m_resourceMap.erase( uriSegment );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CDefaultHTTPServerRouter::RemoveResourceMapping( THTTPServerResourcePtr resource )
{GUCEF_TRACE;

    if ( resource.IsNULL() )
        return false;
    
    CString uriSegment = resource->GetURL();
    return RemoveResourceMapping( uriSegment );
}

/*-------------------------------------------------------------------------*/

bool 
CDefaultHTTPServerRouter::RemoveAllResourceMappings( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock writeLock( m_rwLock );
    m_resourceTree.Clear();
    m_resourceMap.clear();
    return true;
}

/*-------------------------------------------------------------------------*/
    
void
CDefaultHTTPServerRouter::SetServiceRoot( const CString& serviceRoot )
{GUCEF_TRACE;
    
    MT::CScopeWriterLock writeLock( m_rwLock );    
    m_serviceRoot = serviceRoot;
}

/*-------------------------------------------------------------------------*/
    
const CString& 
CDefaultHTTPServerRouter::GetServiceRoot( void ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock readLock( m_rwLock );
    return m_serviceRoot;
}

/*-------------------------------------------------------------------------*/
    
void 
CDefaultHTTPServerRouter::SetMountPath( const CString& mountPath )
{GUCEF_TRACE;

    MT::CScopeWriterLock writeLock( m_rwLock );
    m_mountPath = mountPath;
}

/*-------------------------------------------------------------------------*/
    
const CString& 
CDefaultHTTPServerRouter::GetMountPath( void ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock readLock( m_rwLock );
    return m_mountPath;
}

/*-------------------------------------------------------------------------*/
    
void 
CDefaultHTTPServerRouter::SetRouterController( CIHTTPServerRouterController& controller )
{GUCEF_TRACE;

    MT::CScopeWriterLock writeLock( m_rwLock );
    m_controller = &controller;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace WEB */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
