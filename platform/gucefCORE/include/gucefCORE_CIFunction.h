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

#ifndef GUCEF_CORE_CIFUNCTION_H
#define GUCEF_CORE_CIFUNCTION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"   
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_SFINAE_UTILS_H
#include "gucefCORE_SFINAE_utils.h"
#define GUCEF_CORE_SFINAE_UTILS_H
#endif /* GUCEF_CORE_SFINAE_UTILS_H ? */

#ifndef GUCEF_CORE_CINAMEDINSTANCE_H
#include "CINamedInstance.h"
#define GUCEF_CORE_CINAMEDINSTANCE_H
#endif /* GUCEF_CORE_CINAMEDINSTANCE_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

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
 *  Interface class for implementation of dynamic functions
 *  Note that while functions can be implemented via templates as well this allows
 *  for runtime config driven data to be applied. In addition this also enables the
 *  use of a plugin model for functions on platforms that support plugin mechanics.
 *  Can also be used to tie-in scripting modules
 */
class GUCEF_CORE_PUBLIC_CPP CIFunction : public CINamedInstance
{
    public:

    CIFunction( void );

    CIFunction( const CIFunction& src );

    virtual ~CIFunction();

    CIFunction& operator=( const CIFunction& src );

    /**
     *  Executes the function.
     *  @return 'true' if the function was successfully execututed, meaning the intended logic was able to be invoked.
     */
    virtual bool ExecuteFunction( const CVariant::VariantVector& params       , 
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const = 0;

    /**
     *  Obtains meta-data on the function.
     *  @param requiredParams the numer and type of params that are required for the function to correctly execute
     */
    //virtual bool GetFunctionMetaData( CVariant::VariantVector& requiredParams  , 
    //                                  CVariant::VariantVector& optionalParams  ,
    //                                  CVariant::VariantVector& functionResults ) const = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIFUNCTION_H ? */
