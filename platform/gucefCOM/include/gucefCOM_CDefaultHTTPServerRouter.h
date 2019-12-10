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

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H
#define GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_COM_CIHTTPSERVERROUTER_H
#include "gucefCOM_CIHTTPServerRouter.h"
#define GUCEF_COM_CIHTTPSERVERROUTER_H
#endif /* GUCEF_COM_CIHTTPSERVERROUTER_H ? */

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

class GUCEF_COM_EXPORT_CPP CDefaultHTTPServerRouter : public CIHTTPServerRouter
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

    protected:

    typedef std::map< CString, THTTPServerResourcePtr > TResourceMap;

    CString m_mountPath;
    CString m_serviceRoot;
    CIHTTPServerRouterController* m_controller;
    TResourceMap m_resourceMap;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
