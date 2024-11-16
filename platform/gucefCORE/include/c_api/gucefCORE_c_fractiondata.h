/*
 *  gucefMT: GUCEF module providing multithreading solutions
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef GUCEF_CORE_C_FRACTIONDATA_H
#define GUCEF_CORE_C_FRACTIONDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"           /* include the GUCEF common config */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

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

struct SInt32T2Fraction
{
    Int32 numerator;
    Int32 denominator;
};
typedef struct SInt32T2Fraction TInt32T2Fraction;

struct SUInt32T2Fraction
{
    UInt32 numerator;
    UInt32 denominator;
};
typedef struct SUInt32T2Fraction TUInt32T2Fraction;

struct SInt64WInt32Fraction
{
    Int64 numerator;
    Int32 denominator;
};
typedef struct SInt64WInt32Fraction TInt64WInt32Fraction;

struct SUInt64WUInt32Fraction
{
    UInt64 numerator;
    UInt32 denominator;
};
typedef struct SUInt64WUInt32Fraction TUInt64WUInt32Fraction;

struct SInt64T2Fraction
{
    Int64 numerator;
    Int64 denominator;
};
typedef struct SInt64T2Fraction TInt64T2Fraction;

struct SUInt64T2Fraction
{
    UInt64 numerator;
    UInt64 denominator;
};
typedef struct SUInt64T2Fraction TUInt64T2Fraction;

struct SFraction
{
    union FractionUnionType
    {
        TInt32T2Fraction int32t2_data;
        TUInt32T2Fraction uint32t2_data;
        TInt64WInt32Fraction int64_int32_data;
        TUInt64WUInt32Fraction uint64_uint32_data;
    } union_data;
};
typedef struct SFraction TFraction;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_C_FRACTIONDATA_H ? */