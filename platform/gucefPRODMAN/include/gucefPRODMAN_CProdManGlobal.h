/*
 *  gucefPRODMAN: Product management module
 *  Copyright (C) 2002 - 2012.  Dinand Vanvelzen
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

#ifndef GUCEF_PRODMAN_CPRODMANGLOBAL_H
#define GUCEF_PRODMAN_CPRODMANGLOBAL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PRODMAN_MACROS_H
#include "gucefPRODMAN_macros.h"
#define GUCEF_PRODMAN_MACROS_H
#endif /* GUCEF_PRODMAN_MACROS_H ? */

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

class CProductManager;
class CProductInfoListProviderRegistry;

/*-------------------------------------------------------------------------*/

/**
 *  Singular singleton providing access to all global ProdMan systems
 */
class GUCEF_PRODMAN_PUBLIC_CPP CProdManGlobal
{
    public:

    static CProdManGlobal* Instance( void );

    CProductManager& GetProductManager( void );

    CProductInfoListProviderRegistry& GetProductInfoListProviderRegistry( void );

    private:
    friend class CModule;

    static void Deinstance( void );

    private:

    CProdManGlobal( void );

    ~CProdManGlobal();

    void Initialize( void );

    private:

    CProductManager* m_productManager;
    CProductInfoListProviderRegistry* m_productInfoListProviderRegistry;

    static CProdManGlobal* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PRODMAN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PRODMAN_CPRODMANGLOBAL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-02-2007 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/
