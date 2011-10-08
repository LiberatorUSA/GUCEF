/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCE_MYGUIOGRE_ETYPES_H
#define GUCE_MYGUIOGRE_ETYPES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCE_CORE_ETYPES_H
#include "guceCORE_ETypes.h"
#define GUCE_CORE_ETYPES_H
#endif /* GUCE_CORE_ETYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Maximum and minimal values for the simple types which we are about to
 *      define.
 */
#define GUCEFGUI_INT8MAX     GUCEFCORE_INT8MAX
#define GUCEFGUI_INT8MIN     GUCEFCORE_INT8MIN
#define GUCEFGUI_UINT8MAX    GUCEFCORE_UINT8MAX
#define GUCEFGUI_UINT8MIN    GUCEFCORE_UINT8MIN
#define GUCEFGUI_INT16MAX    GUCEFCORE_INT16MAX
#define GUCEFGUI_INT16MIN    GUCEFCORE_INT16MIN
#define GUCEFGUI_UINT16MAX   GUCEFCORE_UINT16MAX
#define GUCEFGUI_UINT16MIN   GUCEFCORE_UINT16MIN
#define GUCEFGUI_INT32MAX    GUCEFCORE_INT32MAX
#define GUCEFGUI_INT32MIN    GUCEFCORE_INT32MIN
#define GUCEFGUI_UINT32MAX   GUCEFCORE_UINT32MAX
#define GUCEFGUI_UINT32MIN   GUCEFCORE_UINT32MIN
#define GUCEFGUI_FLOAT32MAX  GUCEFCORE_FLOAT32MAX
#define GUCEFGUI_FLOAT32MIN  GUCEFCORE_FLOAT32MIN

/*--------------------------------------------------------------------------*/

/*
 *  When using C++ we should use namespacing
 */
#ifdef __cplusplus
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCE {
namespace MYGUIOGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CORE::UInt8 UInt8;    /* 1 byte, unsigned */
typedef CORE::Int8 Int8;      /* 1 byte, signed */

typedef CORE::UInt16 UInt16;   /* 2 bytes, unsigned */
typedef CORE::Int16 Int16;     /* 2 bytes, signed */

typedef CORE::UInt32 UInt32;   /* 4 bytes, unsigned */
typedef CORE::Int32  Int32;    /* 4 bytes, signed */

typedef CORE::Int64 Int64;     /* 8 bytes, signed */
typedef CORE::UInt64 UInt64;   /* 8 bytes, unsigned */

typedef CORE::Float32 Float32;  /* 4 bytes, signed, decimal */
typedef CORE::Float64 Float64;  /* 8 bytes, signed, decimal */

typedef CORE::TBuildinDataType TBuildinDataType;

typedef CORE::CString   CString; /* import the core string type */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIOGRE */
}; /* namespace GUCE */

/*--------------------------------------------------------------------------*/

#endif /* __cplusplus ? */

#endif /* GUCE_MYGUIOGRE_ETYPES_H ? */