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

#ifndef GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H
#define GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_MT_CREADWRITELOCK_H
#include "gucefMT_CReadWriteLock.h"
#define GUCEF_MT_CREADWRITELOCK_H
#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_WEB_CIHTTPSERVERROUTER_H
#include "gucefWEB_CIHTTPServerRouter.h"
#define GUCEF_WEB_CIHTTPSERVERROUTER_H
#endif /* GUCEF_WEB_CIHTTPSERVERROUTER_H ? */

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

class GUCEF_WEB_PUBLIC_CPP CDefaultHTTPServerRouter : public CIHTTPServerRouter
{
    public:

    CDefaultHTTPServerRouter( void );
    
    CDefaultHTTPServerRouter( const CDefaultHTTPServerRouter& src );
    
    virtual ~CDefaultHTTPServerRouter();
    
    CDefaultHTTPServerRouter& operator=( const CDefaultHTTPServerRouter& src );
    
    virtual THTTPServerResourcePtr ResolveUriToResource( const CString& uri );
    
    virtual void SetServiceRoot( const CString& serviceRoot );
    
    virtual const CString& GetServiceRoot( void ) const;
    
    virtual void SetMountPath( const CString& mountPath );
    
    virtual const CString& GetMountPath( void ) const;
    
    virtual void SetRouterController( CIHTTPServerRouterController& controller );

    bool SetResourceMapping( const CString& uriSegment       ,
                             THTTPServerResourcePtr resource );

    bool RemoveResourceMapping( const CString& uriSegment );

    bool RemoveResourceMapping( THTTPServerResourcePtr resource );

    bool RemoveAllResourceMappings( void );

    void SetWildcardMatchUris( bool useWildcards );

    bool GetWildcardMatchUris( void ) const;

    protected:

    CString m_mountPath;
    CString m_serviceRoot;
    CIHTTPServerRouterController* m_controller;
    CORE::CDataNode m_resourceMap;
    bool m_uriIsCaseSensitive;
    bool m_wildcardMatchUris;
    MT::CReadWriteLock m_rwLock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
