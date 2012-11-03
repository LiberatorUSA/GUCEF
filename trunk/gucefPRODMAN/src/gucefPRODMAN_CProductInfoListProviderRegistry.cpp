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

#include "gucefPRODMAN_CProductInfoListProviderRegistry.h"

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
                  
CProductInfoListProviderRegistry::CProductInfoListProviderRegistry( void )
    : m_registry()
{
}

/*-------------------------------------------------------------------------*/

CProductInfoListProviderRegistry::~CProductInfoListProviderRegistry()
{
}

/*-------------------------------------------------------------------------*/

bool
CProductInfoListProviderRegistry::TryGetProvider( const CORE::CString& providerType     ,
                                                  CProductInfoListProviderPtr& provider )
{
    return m_registry.TryLookup( providerType  ,
                                 provider      ,
                                 false         );
}

/*-------------------------------------------------------------------------*/
    
void
CProductInfoListProviderRegistry::RegisterProvider( const CORE::CString& providerType    ,
                                                    CProductInfoListProviderPtr provider )
{
    m_registry.Register( providerType, provider );
}

/*-------------------------------------------------------------------------*/

void
CProductInfoListProviderRegistry::UnregisterProvider( const CORE::CString& providerType )
{
    m_registry.Unregister( providerType );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PRODMAN */
}; /* namespace GUCEF*/

/*-------------------------------------------------------------------------*/
