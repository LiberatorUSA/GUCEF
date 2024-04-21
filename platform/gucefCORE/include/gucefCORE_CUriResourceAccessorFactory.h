/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H
#define GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"    
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CTABSTRACTFACTORY_H
#include "CTAbstractFactory.h"
#define GUCEF_CORE_CTABSTRACTFACTORY_H
#endif /* GUCEF_CORE_CTABSTRACTFACTORY_H ? */

#ifndef GUCEF_CORE_CURIRESOURCEACCESSOR_H
#include "gucefCORE_CUriResourceAccessor.h"
#define GUCEF_CORE_CURIRESOURCEACCESSOR_H
#endif /* GUCEF_CORE_CURIRESOURCEACCESSOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Factory class for CUriResourceAccessor objects
 */
class GUCEF_CORE_PUBLIC_CPP CUriResourceAccessorFactory : public CTAbstractFactory< CString, CUriResourceAccessor, MT::CMutex > 
{
    public:

    CUriResourceAccessorFactory( void );

    virtual ~CUriResourceAccessorFactory( void );

    /**
     *  Utility specialization of Create() which uses the contextually more natural Uri class
     */
    TProductPtr CreateAccessor( const CUri& uri );

    /**
     *  Utility shortcut version of GetResourceAccess() which instantiates the correct accessor 
     *  first in order to obtain resource access. 
     *  Note that for repeated resource access, it is more efficient to cache the accessor
     */
    bool GetResourceAccess( const CUri& uri                                     ,
                            IOAccessPtr& accessToResource                       ,
                            CUriResourceAccessor::TURI_RESOURCEACCESS_MODE mode );

    private:

    CUriResourceAccessorFactory( const CUriResourceAccessorFactory& src );              /**< not implemented, not supported >*/
    CUriResourceAccessorFactory& operator=( const CUriResourceAccessorFactory& src );   /**< not implemented, not supported >*/

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H ? */
