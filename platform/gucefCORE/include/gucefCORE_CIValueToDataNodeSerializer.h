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

#ifndef GUCEF_CORE_CIVALUETODATANODESERIALIZER_H
#define GUCEF_CORE_CIVALUETODATANODESERIALIZER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h" /* macros that are GUCEF specific and generic macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"           /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CITYPENAMED_H
#include "CITypeNamed.h"
#define GUCEF_CORE_CITYPENAMED_H
#endif /* GUCEF_CORE_CITYPENAMED_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_SFINAE_UTILS_H
#include "gucefCORE_SFINAE_utils.h"
#define GUCEF_CORE_SFINAE_UTILS_H
#endif /* GUCEF_CORE_SFINAE_UTILS_H ? */

#ifndef GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#include "gucefCORE_CDataNodeSerializableSettings.h"
#define GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#endif /* GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CIDATANODESERIALIZABLE_H
#include "gucefCORE_CIDataNodeSerializable.h"
#define GUCEF_CORE_CIDATANODESERIALIZABLE_H
#endif /* GUCEF_CORE_CIDATANODESERIALIZABLE_H ? */

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
 *  Abstract base class for adding a serialization API to decending classes.
 *  Serialize/Deserialze via the DataNode class as a DOM
 */
class GUCEF_CORE_PUBLIC_CPP CIValueToDataNodeSerializer
{
    public:

    CIValueToDataNodeSerializer( void );

    CIValueToDataNodeSerializer( const CIValueToDataNodeSerializer& src );

    virtual ~CIValueToDataNodeSerializer();

    CIValueToDataNodeSerializer& operator=( const CIValueToDataNodeSerializer& src );

    virtual bool Serialize( const CVariant& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const bool value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const Int8 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const UInt8 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const Int16 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const UInt16 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const Int32 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const UInt32 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const Int64 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const UInt64 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const Float32 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const Float64 value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const void* value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CAsciiString& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CAsciiString::StringSet& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CAsciiString::StringVector& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CUtf8String& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CUtf8String::StringSet& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CUtf8String::StringVector& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CIDataNodeSerializable& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CIConfigurable& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Serialize( const CDataNode& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;

    virtual bool Deserialize( bool& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( Int8& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( UInt8& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( Int16& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( UInt16& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( Int32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( UInt32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( Int64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( UInt64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( Float32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( Float64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( CAsciiString& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( CAsciiString::StringSet& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( CAsciiString::StringVector& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( CUtf8String& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( CUtf8String::StringSet& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( CUtf8String::StringVector& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( CIDataNodeSerializable& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;
    virtual bool Deserialize( CIConfigurable& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) = 0;



    // redirect pointers to reference param versions
    template < typename S >
    bool SerializeType( const S* mappedType, CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
       {    typedef typename remove_pointer< S >::type T;
            return SerializeReferenceType< T >( static_cast< T >( *mappedType ), domRootNode, serializerOptions ); }

    // redirect types that are themselves pointers to reference param versions
    template < typename S >
    bool SerializeType( typename EnableIf< TypeIsPointerType< S >::value, S >::type mappedType, CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
       {    typedef typename remove_pointer< S >::type T;
            return SerializeReferenceType< T >( static_cast< T >( *mappedType ), domRootNode, serializerOptions ); }

    // SFINAE variant for shared pointers where the caller intent is to utilize the referenced object via the pointer to said object
    template < typename S >
    bool SerializeType( const typename EnableIfFirstOf2< CORE::TypeHasMemberFunctionGetPointerAlways< S >::value, TypeIsPointerType< S >::value, S, S >::type& mappedType, CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
        { return GUCEF_NULL != &mappedType ?
                                             ( GUCEF_NULL != mappedType.GetPointerAlways() ? SerializeReferenceType< typename S::TContainedType >( *mappedType.GetPointerAlways(), domRootNode, serializerOptions ) : false ) :
                                             false; }

    // SFINAE variant in case the variant for shared pointers does not apply. This variant is for remaining reference types
    template < typename S >
    bool SerializeType( const typename EnableIfNot2< CORE::TypeHasMemberFunctionGetPointerAlways< S >::value, TypeIsPointerType< S >::value, S, S >::type& mappedType, CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
        { return SerializeReferenceType< S >( mappedType, domRootNode, serializerOptions ); }

    // We provide a specialization for CIDataNodeSerializable since that type should take priority
    // reason being that it can handle CDataNodeSerializableSettings plus helps to disambiguate in the case of multiple inhertitance
    template < typename S >
    bool SerializeReferenceType( const typename EnableIf< CORE::TypeHasMemberFunctionForDataNodeSerialization< S >::value, S >::type& mappedType, CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
        { return GUCEF_NULL != &mappedType ? Serialize( static_cast< const CIDataNodeSerializable& >( mappedType ), domRootNode, serializerOptions ) : false; }

    // Any remaining reference type which does not support the IDataNodeSerializable interface would use this version
    template < typename S >
    bool SerializeReferenceType( const typename EnableIfNot< CORE::TypeHasMemberFunctionForDataNodeSerialization< S >::value, S >::type& mappedType, CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
        { return GUCEF_NULL != &mappedType ? Serialize( mappedType, domRootNode, serializerOptions ) : false; }

    // redirect pointers to reference param versions
    template < typename S >
    bool DeserializeType( S* mappedType, const CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
       {    typedef typename remove_pointer< S >::type T;
            return DeserializeReferenceType< T >( static_cast< T& >( *mappedType ), domRootNode, serializerOptions ); }

    // redirect types that are themselves pointers to reference param versions
    template < typename S >
    bool DeserializeType( typename EnableIf< TypeIsPointerType< S >::value, S >::type mappedType, const CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
       {    typedef typename remove_pointer< S >::type T;
            return DeserializeReferenceType< T >( static_cast< T& >( *mappedType ), domRootNode, serializerOptions ); }

    // SFINAE variant for shared pointers where the caller intent is to utilize the referenced object via the pointer to said object
    template < typename S >
    bool DeserializeType( typename EnableIfFirstOf2< CORE::TypeHasMemberFunctionGetPointerAlways< S >::value, TypeIsPointerType< S >::value, S, S >::type& mappedType, const CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
        { return GUCEF_NULL != &mappedType ?
                                             ( GUCEF_NULL != mappedType.GetPointerAlways() ? DeserializeReferenceType< typename S::TContainedType >( *mappedType.GetPointerAlways(), domRootNode, serializerOptions ) : false ) :
                                             false; }

    // SFINAE variant in case the variant for shared pointers does not apply. This variant is for remaining reference types
    template < typename S >
    bool DeserializeType( typename EnableIfNot2< CORE::TypeHasMemberFunctionGetPointerAlways< S >::value, TypeIsPointerType< S >::value, S, S >::type& mappedType, const CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
       { return DeserializeReferenceType< S >( mappedType, domRootNode, serializerOptions ); }

    // We provide a specialization for CIDataNodeSerializable since that type should take priority
    // reason being that it can handle CDataNodeSerializableSettings plus helps to disambiguate in the case of multiple inhertitance
    template < typename S >
    bool DeserializeReferenceType( typename EnableIf< CORE::TypeHasMemberFunctionForDataNodeDeserialization< S >::value, S >::type& mappedType, const CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
        { return GUCEF_NULL != &mappedType ? Deserialize( static_cast< CIDataNodeSerializable& >( mappedType ), domRootNode, serializerOptions ) : false; }

    // Any remaining reference type which does not support the IDataNodeSerializable interface would use this version
    template < typename S >
    bool DeserializeReferenceType( typename EnableIfNot< CORE::TypeHasMemberFunctionForDataNodeDeserialization< S >::value, S >::type& mappedType, const CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& serializerOptions )
        { return GUCEF_NULL != &mappedType ? Deserialize( static_cast< S& >( mappedType ), domRootNode, serializerOptions ) : false; }

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIVALUETODATANODESERIALIZER_H ? */
