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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefPRODMAN_CProductInfoListRetrievedEventData.h"

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
                  
CProductInfoListRetrievedEventData::CProductInfoListRetrievedEventData( const CORE::CString& listSourceLocation ,
                                                                        CProductInfoListPtr productInfoList     )
    : m_listSourceLocation( listSourceLocation ) ,
      m_productInfoList( productInfoList )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CProductInfoListRetrievedEventData::~CProductInfoListRetrievedEventData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CProductInfoListRetrievedEventData::GetListSourceLocation( void ) const
{GUCEF_TRACE;

    return m_listSourceLocation;
}

/*-------------------------------------------------------------------------*/

CProductInfoListPtr
CProductInfoListRetrievedEventData::GetProductInfoList( void )
{GUCEF_TRACE;

    return m_productInfoList;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CProductInfoListRetrievedEventData::Clone( void ) const
{GUCEF_TRACE;

    return new CProductInfoListRetrievedEventData( m_listSourceLocation, m_productInfoList );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PRODMAN */
}; /* namespace GUCEF*/

/*-------------------------------------------------------------------------*/
