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

#ifndef GUCEF_CORE_C_FUNCTION_DEFINITION_H
#define GUCEF_CORE_C_FUNCTION_DEFINITION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"   
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_C_VARIANTDATA_H
#include "gucefCORE_c_variantdata.h"
#define GUCEF_CORE_C_VARIANTDATA_H
#endif /* GUCEF_CORE_C_VARIANTDATA_H ? */

#ifndef GUCEF_CORE_C_FUNCTION_PARAM_TYPES_H
#include "gucefCORE_c_function_param_types.h"
#define GUCEF_CORE_C_FUNCTION_PARAM_TYPES_H
#endif /* GUCEF_CORE_C_FUNCTION_PARAM_TYPES_H ? */

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

struct SFunctionParamMeta
{
    TFunctionParamType   paramType;
    UInt8                paramDataType;
    UInt8                isParamConst;
    const TVariantData*  defaultValue;      /**< optional pointer to default value if any, if present means the param is optional since the default value can be used */
    const char*          paramName;
};
typedef struct SFunctionParamMeta TFunctionParamMeta;

/*-------------------------------------------------------------------------*/

struct SFunctionDefinition
{
    UInt32               numParams;    
    SFunctionParamMeta*  params;
    UInt8                isFunctionConst;
    const char*          functionName;
};
typedef struct SFunctionDefinition TFunctionDefinition;

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

#endif /* GUCEF_CORE_C_FUNCTION_DEFINITION_H ? */
