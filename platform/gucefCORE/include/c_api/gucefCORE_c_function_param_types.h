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

#ifndef GUCEF_CORE_C_FUNCTION_PARAM_TYPES_H
#define GUCEF_CORE_C_FUNCTION_PARAM_TYPES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"   
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace CORE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum EFunctionParamType
{
    FUNCTIONPARAMTYPE_UNKNOWN        = 0,

    FUNCTIONPARAMTYPE_INPUT_REFERENCE,       /**< Input parameter passed by reference. */
    FUNCTIONPARAMTYPE_INPUT_VALUE,           /**< Input parameter passed by value. */
    FUNCTIONPARAMTYPE_OUTPUT_REFERENCE,      /**< Output parameter passed by reference. */
    FUNCTIONPARAMTYPE_OUTPUT_VALUE,          /**< Output parameter passed by value. */
    FUNCTIONPARAMTYPE_INOUT_REFERENCE,       /**< Input/output parameter passed by reference. */

    FUNCTIONPARAMTYPE_RETURN_VALUE ,         /**< Return value of a function. */
    FUNCTIONPARAMTYPE_RETURN_VOID            /**< non-return value of a function. it doesnt have a return value */
};

typedef enum EFunctionParamType TFunctionParamType;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_C_FUNCTION_PARAM_TYPES_H ? */
