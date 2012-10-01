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

#ifndef GUCEF_PRODMAN_CPRODUCTINFOLISTRETRIEVEDEVENTDATA_H
#define GUCEF_PRODMAN_CPRODUCTINFOLISTRETRIEVEDEVENTDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CICloneable.h"
#include "gucefPRODMAN_CProductInfoList.h"

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
                  
class GUCEF_PRODMAN_PUBLIC_CPP CProductInfoListRetrievedEventData : public CORE::CICloneable
{
    public:

    CProductInfoListRetrievedEventData( const CORE::CString& listSourceLocation ,
                                        CProductInfoListPtr productInfoList     );

    virtual ~CProductInfoListRetrievedEventData();

    const CORE::CString& GetListSourceLocation( void ) const;

    CProductInfoListPtr GetProductInfoList( void );

    virtual CORE::CICloneable* Clone( void ) const;

    private:

    CProductInfoListRetrievedEventData( const CProductInfoListRetrievedEventData& src );
    CProductInfoListRetrievedEventData& operator=( const CProductInfoListRetrievedEventData& other );

    private:

    CORE::CString m_listSourceLocation;
    CProductInfoListPtr m_productInfoList;
};


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PRODMAN */
}; /* namespace GUCEF*/

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PRODMAN_CPRODUCTINFOLISTRETRIEVEDEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-02-2008 :
        - Initial implementation
          
---------------------------------------------------------------------------*/

