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

#ifndef GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDER_H
#define GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */ 

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_PRODMAN_CPRODUCTINFOLIST_H
#include "gucefPRODMAN_CProductInfoList.h"
#define GUCEF_PRODMAN_CPRODUCTINFOLIST_H
#endif /* GUCEF_PRODMAN_CPRODUCTINFOLIST_H ? */

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
                  
class GUCEF_PRODMAN_PUBLIC_CPP CProductInfoListProvider : public CORE::CObservingNotifier
{
    public:
    
    static const CORE::CEvent ProductInfoListRetrievalStartedEvent;
    static const CORE::CEvent ProductInfoListRetrievedEvent;
    static const CORE::CEvent ProductInfoListRetrievalErrorEvent;

    static void RegisterEvents( void );

    public:

    CProductInfoListProvider( void );

    virtual ~CProductInfoListProvider();

    virtual bool RetrieveList( const CORE::CString& listLocation ) = 0;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    CProductInfoListProvider( const CProductInfoListProvider& src );
    CProductInfoListProvider& operator=( const CProductInfoListProvider& other );
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CProductInfoListProvider, MT::CMutex >   CProductInfoListProviderPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PRODMAN */
}; /* namespace GUCEF*/

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PRODMAN_CPRODUCTINFOLISTPROVIDER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-02-2008 :
        - Initial implementation
          
---------------------------------------------------------------------------*/

