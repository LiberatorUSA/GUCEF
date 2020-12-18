/*
 *  gucefPRODMAN: Product management module
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDERREGISTRY_H
#define GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDERREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTREGISTRY_H
#include "CTRegistry.h"
#define GUCEF_CORE_CTREGISTRY_H
#endif /* GUCEF_CORE_CTREGISTRY_H ? */ 

#ifndef GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDER_H
#include "gucefPRODMAN_CProductInfoListProvider.h"
#define GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDER_H
#endif /* GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PRODMAN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/
                  
class GUCEF_PRODMAN_PUBLIC_CPP CProductInfoListProviderRegistry
{
    public:

    CProductInfoListProviderRegistry( void );

    virtual ~CProductInfoListProviderRegistry();

    bool TryGetProvider( const CORE::CString& providerType     ,
                         CProductInfoListProviderPtr& provider );
    
    void RegisterProvider( const CORE::CString& providerType    ,
                           CProductInfoListProviderPtr provider );

    void UnregisterProvider( const CORE::CString& providerType );

    private:

    CProductInfoListProviderRegistry( const CProductInfoListProviderRegistry& src );
    CProductInfoListProviderRegistry& operator=( const CProductInfoListProviderRegistry& other );

    private:

    CORE::CTRegistry< CProductInfoListProvider, MT::CMutex > m_registry;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PRODMAN */
}; /* namespace GUCEF*/

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDERREGISTRY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-02-2008 :
        - Initial implementation
          
---------------------------------------------------------------------------*/

