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

#ifndef GUCEF_CORE_CGENERICVALUETODATANODESERIALIZER_H
#define GUCEF_CORE_CGENERICVALUETODATANODESERIALIZER_H

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

#ifndef GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#include "gucefCORE_CDataNodeSerializableSettings.h"
#define GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#endif /* GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H ? */

#ifndef GUCEF_CORE_CIVALUETODATANODESERIALIZER_H
#include "gucefCORE_CIValueToDataNodeSerializer.h"
#define GUCEF_CORE_CIVALUETODATANODESERIALIZER_H
#endif /* GUCEF_CORE_CIVALUETODATANODESERIALIZER_H ? */

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
 *  Generic value Serialize/Deserialize via the DataNode class as a DOM 
 */
class GUCEF_CORE_PUBLIC_CPP CGenericValueToDataNodeSerializer : public CIValueToDataNodeSerializer
{
    public:

    CGenericValueToDataNodeSerializer( void );

    CGenericValueToDataNodeSerializer( const CGenericValueToDataNodeSerializer& src );

    virtual ~CGenericValueToDataNodeSerializer();

    CGenericValueToDataNodeSerializer& operator=( const CGenericValueToDataNodeSerializer& src );

    virtual bool Serialize( const CVariant& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const bool& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const Int8& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const UInt8& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const Int16& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const UInt16& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const Int32& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const UInt32& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const Int64& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const UInt64& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const Float32& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const Float64& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const void*& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const CAsciiString& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const CAsciiString::StringSet& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const CAsciiString::StringVector& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const CUtf8String& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const CUtf8String::StringSet& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const CUtf8String::StringVector& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Serialize( const CIDataNodeSerializable& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;    
    virtual bool Serialize( const CDataNode& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;    

    virtual bool Deserialize( bool& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( Int8& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( UInt8& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( Int16& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( UInt16& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( Int32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( UInt32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( Int64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( UInt64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( Float32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( Float64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( CAsciiString& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( CAsciiString::StringSet& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( CAsciiString::StringVector& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( CUtf8String& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( CUtf8String::StringSet& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( CUtf8String::StringVector& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Deserialize( CIDataNodeSerializable& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;

    static CGenericValueToDataNodeSerializer* Instance( void );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CGENERICVALUETODATANODESERIALIZER_H ? */
