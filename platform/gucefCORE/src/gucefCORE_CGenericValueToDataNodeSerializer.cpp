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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_CGenericValueToDataNodeSerializer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static CGenericValueToDataNodeSerializer g_instance;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CGenericValueToDataNodeSerializer* 
CGenericValueToDataNodeSerializer::Instance( void )
{GUCEF_TRACE;
    
    return &g_instance;
}

/*-------------------------------------------------------------------------*/

CGenericValueToDataNodeSerializer::CGenericValueToDataNodeSerializer( void )
    : CIValueToDataNodeSerializer()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGenericValueToDataNodeSerializer::CGenericValueToDataNodeSerializer( const CGenericValueToDataNodeSerializer& src )
    : CIValueToDataNodeSerializer( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGenericValueToDataNodeSerializer::~CGenericValueToDataNodeSerializer()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGenericValueToDataNodeSerializer& 
CGenericValueToDataNodeSerializer::operator=( const CGenericValueToDataNodeSerializer& src )
{GUCEF_TRACE;  

    if ( this != &src )
    {
        CIValueToDataNodeSerializer::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CVariant& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;  

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const bool& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;  

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const Int8& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;  

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const UInt8& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;  

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const Int16& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;  

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const UInt16& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;  

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const Int32& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;  

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const UInt32& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const Int64& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const UInt64& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const Float32& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const Float64& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const void*& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CAsciiString& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CAsciiString::StringSet& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    bool totalSuccess = true;
    domNode.SetNodeType( GUCEF_DATATYPE_ARRAY );
    CAsciiString::StringSet::const_iterator i = value.begin();
    while ( i != value.end() )
    {
        totalSuccess = GUCEF_NULL != domNode.AddValueAsChild( ToString( (*i) ) ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CAsciiString::StringVector& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    bool totalSuccess = true;
    domNode.SetNodeType( GUCEF_DATATYPE_ARRAY );
    CAsciiString::StringVector::const_iterator i = value.begin();
    while ( i != value.end() )
    {
        totalSuccess = GUCEF_NULL != domNode.AddValueAsChild( ToString( (*i) ) ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CUtf8String& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    domNode.SetValue( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CUtf8String::StringSet& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    bool totalSuccess = true;
    domNode.SetNodeType( GUCEF_DATATYPE_ARRAY );
    CUtf8String::StringSet::const_iterator i = value.begin();
    while ( i != value.end() )
    {
        totalSuccess = GUCEF_NULL != domNode.AddValueAsChild( ToString( (*i) ) ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CUtf8String::StringVector& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    bool totalSuccess = true;
    domNode.SetNodeType( GUCEF_DATATYPE_ARRAY );
    CUtf8String::StringVector::const_iterator i = value.begin();
    while ( i != value.end() )
    {
        totalSuccess = GUCEF_NULL != domNode.AddValueAsChild( ToString( (*i) ) ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CIConfigurable& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    return value.SaveConfig( domNode );
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CIDataNodeSerializable& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    return value.Serialize( domNode, settings );
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Serialize( const CDataNode& value, CDataNode& domNode, const CDataNodeSerializableSettings& settings )  
{GUCEF_TRACE;

    return GUCEF_NULL != domNode.AddChild( value );
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( bool& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsBool();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( Int8& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsInt8();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( UInt8& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsUInt8();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( Int16& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsInt16();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( UInt16& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsUInt16();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( Int32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsInt32();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( UInt32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsUInt32();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( Int64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsInt64();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( UInt64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsUInt64();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( Float32& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsFloat32();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( Float64& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsFloat64();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( CAsciiString& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsAsciiString();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( CAsciiString::StringSet& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    //value = domNode.GetValue().AsAsciiString();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( CAsciiString::StringVector& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    //value = domNode.GetValue().AsAsciiString();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( CUtf8String& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    value = domNode.GetValue().AsUtf8String();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( CUtf8String::StringSet& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    //value = domNode.GetValue().AsUtf8String();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( CUtf8String::StringVector& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    //value = domNode.GetValue().AsUtf8String();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( CIDataNodeSerializable& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    return value.Deserialize( domNode, settings );
}

/*-------------------------------------------------------------------------*/

bool 
CGenericValueToDataNodeSerializer::Deserialize( CIConfigurable& value, const CDataNode& domNode, const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    return value.LoadConfig( domNode );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
