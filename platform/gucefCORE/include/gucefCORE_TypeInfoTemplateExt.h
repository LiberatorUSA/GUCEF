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

#ifndef GUCEF_CORE_TYPEINFOTEMPLATEEXT_H
#define GUCEF_CORE_TYPEINFOTEMPLATEEXT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_TYPES_H
#include "gucef_types.h"
#define GUCEF_TYPES_H
#endif /* GUCEF_TYPES_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASS TEMPLATES                                                    //
//                                                                         //
//-------------------------------------------------------------------------*/

template <> inline UInt8 TryToGetGucefTypeIdForTType( const CORE::CString* dummyValue ) { return GUCEF_DATATYPE_STRING; }
template <> inline UInt8 TryToGetGucefTypeIdForTType( const CORE::CString::StringSet* dummyValue ) { return GUCEF_DATATYPE_SET; }
template <> inline UInt8 TryToGetGucefTypeIdForTType( const CORE::CString::StringVector* dummyValue ) { return GUCEF_DATATYPE_ARRAY; }
#if ( GUCEF_DEFAULT_STRING_FORMAT == GUCEF_DATATYPE_ASCII_STRING )
template <> inline UInt8 TryToGetGucefTypeIdForTType( const CORE::CUtf8String* dummyValue ) { return GUCEF_DATATYPE_UTF8_STRING; }
#else
template <> inline UInt8 TryToGetGucefTypeIdForTType( const CORE::CAsciiString* dummyValue ) { return GUCEF_DATATYPE_ASCII_STRING; }
#endif
template <> inline UInt8 TryToGetGucefTypeIdForTType( const CORE::CVariant* dummyValue ) { return GUCEF_DATATYPE_VARIANT_DATA; }
template <> inline UInt8 TryToGetGucefTypeIdForTType( const CORE::TVariantData* dummyValue ) { return GUCEF_DATATYPE_VARIANT_DATA; }
template <> inline UInt8 TryToGetGucefTypeIdForTType( const CORE::TDefaultFuncPtr* dummyValue ) { return GUCEF_DATATYPE_FUNCTION_MEMORY_ADDRESS; }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_TYPEINFOTEMPLATEEXT_H ? */
