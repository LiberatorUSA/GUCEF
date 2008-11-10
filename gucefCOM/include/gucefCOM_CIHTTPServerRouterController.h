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

#ifndef GUCEF_COM_CIHTTPSERVERROUTERCONTROLLER_H
#define GUCEF_COM_CIHTTPSERVERROUTERCONTROLLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

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

class GUCEF_COM_EXPORT_CPP CIHTTPServerRouterController
{   
    public:
    
    CIHTTPServerRouterController( void );
    
    CIHTTPServerRouterController( const CIHTTPServerRouterController& src );
    
    virtual ~CIHTTPServerRouterController();
    
    CIHTTPServerRouterController& operator=( const CIHTTPServerRouterController& src );
    
    virtual CIHTTPServerRouter* GetHandler( const CString& uri ) = 0;
    
    virtual CString GetUriAfterTheBaseAddress( CIHTTPServerRouter& router , 
                                               const CString& fullUri     ) = 0;
    
    virtual CString MakeUriAbsolute( CIHTTPServerRouter& router ,
                                     const CString& uriSuffix   ) = 0;
    
    virtual CString MakeUriAbsolute( CIHTTPServerRouter& router      ,
                                     const CString& uriWithAuthority ,
                                     const CString& uriSuffix        ) = 0;
    
    virtual void AddRouterMapping( CIHTTPServerRouter* router    ,
                                   const CString& uriBase        ,
                                   const CString& serviceRootUri ) = 0;
    
    virtual void RemoveRouterMapping( const CString& uriBase ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CIHTTPSERVERROUTERCONTROLLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
