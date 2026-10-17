/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *
 *  Copyright (C) 1998 - 2026.  Dinand Vanvelzen
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_GUI_TYPES_H
#include "gucefGUI_Types.h"
#define GUCEF_GUI_TYPES_H
#endif /* GUCEF_GUI_TYPES_H ? */

#ifndef GUCEF_GUI_CUVECTOR_H
#include "gucefGUI_CUVector.h"
#define GUCEF_GUI_CUVECTOR_H
#endif /* GUCEF_GUI_CUVECTOR_H ? */

#include "gucefGUI_CWidgetTypeRegistry.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace {

bool
IsInteger( const CString& text )
{
    CString trimmed = text.Trim( true ).Trim( false );
    if ( trimmed.IsNULLOrEmpty() )
        return false;
    char* end = GUCEF_NULL;
    strtol( trimmed.C_String(), &end, 10 );
    return '\0' == *end;
}

bool
IsNumber( const CString& text )
{
    CString trimmed = text.Trim( true ).Trim( false );
    if ( trimmed.IsNULLOrEmpty() )
        return false;
    char* end = GUCEF_NULL;
    strtod( trimmed.C_String(), &end );
    return '\0' == *end;
}

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPropertyDescriptor::CPropertyDescriptor( void )
    : name()
    , valueType( VALUETYPE_STRING )
    , defaultValue()
    , flags( FLAG_DEFAULT )
    , category()
    , description()
    , enumValues()
{
}

/*-------------------------------------------------------------------------*/

CPropertyDescriptor::CPropertyDescriptor( const CString& propertyName       ,
                                          TValueType type                   ,
                                          const CString& defaultVal         ,
                                          UInt32 propertyFlags              ,
                                          const CString& propertyCategory   ,
                                          const CString& propertyDescription )
    : name( propertyName )
    , valueType( type )
    , defaultValue( defaultVal )
    , flags( propertyFlags )
    , category( propertyCategory )
    , description( propertyDescription )
    , enumValues()
{
}

/*-------------------------------------------------------------------------*/

bool
CPropertyDescriptor::HasFlag( UInt32 flag ) const
{
    return flag == ( flags & flag );
}

/*-------------------------------------------------------------------------*/

bool
CPropertyDescriptor::Validate( const CString& value, CString& normalized ) const
{
    switch ( valueType )
    {
        case VALUETYPE_BOOL:
        {
            CString lower = value.Trim( true ).Trim( false ).Lowercase();
            if ( lower == "true" || lower == "1" || lower == "yes" || lower == "on" )
            {
                normalized = "true";
                return true;
            }
            if ( lower == "false" || lower == "0" || lower == "no" || lower == "off" )
            {
                normalized = "false";
                return true;
            }
            return false;
        }
        case VALUETYPE_INT:
        {
            if ( !IsInteger( value ) )
                return false;
            normalized = CORE::Int32ToString( CORE::StringToInt32( value.Trim( true ).Trim( false ) ) );
            return true;
        }
        case VALUETYPE_FLOAT:
        {
            if ( !IsNumber( value ) )
                return false;
            normalized = value.Trim( true ).Trim( false );
            return true;
        }
        case VALUETYPE_UDIM:
        {
            CUDim dim;
            if ( !CUDim::FromString( value, dim ) )
                return false;
            normalized = dim.ToString();
            return true;
        }
        case VALUETYPE_UVECTOR:
        {
            CUVector vec;
            if ( !CUVector::FromString( value, vec ) )
                return false;
            normalized = vec.ToString();
            return true;
        }
        case VALUETYPE_COLOUR:
        {
            CColour colour;
            if ( !CColour::FromString( value, colour ) )
                return false;
            normalized = colour.ToString();
            return true;
        }
        case VALUETYPE_RECT:
        {
            CRect rect;
            if ( !CRect::FromString( value, rect ) )
                return false;
            normalized = rect.ToString();
            return true;
        }
        case VALUETYPE_ENUM:
        {
            for ( UInt32 i=0; i<(UInt32)enumValues.size(); ++i )
            {
                if ( enumValues[ i ].Equals( value, false ) )
                {
                    normalized = enumValues[ i ];
                    return true;
                }
            }
            return false;
        }
        case VALUETYPE_INTLIST:
        {
            if ( value.IsNULLOrEmpty() )
            {
                normalized = value;
                return true;
            }
            CString::StringVector parts = value.ParseElements( ',', true );
            CString result;
            for ( UInt32 i=0; i<(UInt32)parts.size(); ++i )
            {
                if ( !IsInteger( parts[ i ] ) )
                    return false;
                if ( i > 0 )
                    result += ',';
                result += CORE::Int32ToString( CORE::StringToInt32( parts[ i ].Trim( true ).Trim( false ) ) );
            }
            normalized = result;
            return true;
        }
        case VALUETYPE_STRING:
        case VALUETYPE_IMAGE:
        case VALUETYPE_FONT:
        case VALUETYPE_STRINGLIST:
        default:
        {
            normalized = value;
            return true;
        }
    }
}

/*-------------------------------------------------------------------------*/

CString
CPropertyDescriptor::ValueTypeToString( TValueType type )
{
    switch ( type )
    {
        case VALUETYPE_STRING: return "String";
        case VALUETYPE_BOOL: return "Bool";
        case VALUETYPE_INT: return "Int";
        case VALUETYPE_FLOAT: return "Float";
        case VALUETYPE_UDIM: return "UDim";
        case VALUETYPE_UVECTOR: return "UVector";
        case VALUETYPE_COLOUR: return "Colour";
        case VALUETYPE_RECT: return "Rect";
        case VALUETYPE_ENUM: return "Enum";
        case VALUETYPE_IMAGE: return "Image";
        case VALUETYPE_FONT: return "Font";
        case VALUETYPE_STRINGLIST: return "StringList";
        case VALUETYPE_INTLIST: return "IntList";
        default: return "Unknown";
    }
}

/*-------------------------------------------------------------------------*/

CWidgetTypeInfo::CWidgetTypeInfo( void )
    : typeName()
    , baseTypeName()
    , category()
    , description()
    , creator( GUCEF_NULL )
    , properties()
{
}

/*-------------------------------------------------------------------------*/

CWidgetTypeInfo&
CWidgetTypeInfo::AddProperty( const CPropertyDescriptor& descriptor )
{
    properties.push_back( descriptor );
    return *this;
}

/*-------------------------------------------------------------------------*/

CWidgetTypeRegistry::CWidgetTypeRegistry( void )
    : m_types()
    , m_lock()
{
}

/*-------------------------------------------------------------------------*/

CWidgetTypeRegistry::~CWidgetTypeRegistry()
{
}

/*-------------------------------------------------------------------------*/

bool
CWidgetTypeRegistry::RegisterType( const CWidgetTypeInfo& typeInfo )
{
    if ( typeInfo.typeName.IsNULLOrEmpty() )
        return false;

    MT::CScopeMutex lock( m_lock );
    m_types[ typeInfo.typeName ] = typeInfo;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CWidgetTypeRegistry::UnregisterType( const CString& typeName )
{
    MT::CScopeMutex lock( m_lock );
    return m_types.erase( typeName ) > 0;
}

/*-------------------------------------------------------------------------*/

const CWidgetTypeInfo*
CWidgetTypeRegistry::GetTypeInfo( const CString& typeName ) const
{
    MT::CScopeMutex lock( m_lock );
    TTypeMap::const_iterator i = m_types.find( typeName );
    if ( i != m_types.end() )
        return &(*i).second;
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
CWidgetTypeRegistry::IsRegistered( const CString& typeName ) const
{
    return GUCEF_NULL != GetTypeInfo( typeName );
}

/*-------------------------------------------------------------------------*/

CWidgetTypeRegistry::TStringSet
CWidgetTypeRegistry::GetTypeNames( void ) const
{
    MT::CScopeMutex lock( m_lock );
    TStringSet names;
    TTypeMap::const_iterator i = m_types.begin();
    while ( i != m_types.end() )
    {
        names.insert( (*i).first );
        ++i;
    }
    return names;
}

/*-------------------------------------------------------------------------*/

GUCEF::vector< CString >
CWidgetTypeRegistry::GetTypeChain( const CString& typeName ) const
{
    GUCEF::vector< CString > chain;
    CString current = typeName;
    while ( !current.IsNULLOrEmpty() && chain.size() < 64 )
    {
        const CWidgetTypeInfo* info = GetTypeInfo( current );
        if ( GUCEF_NULL == info )
            break;
        chain.push_back( current );
        current = info->baseTypeName;
    }
    return chain;
}

/*-------------------------------------------------------------------------*/

bool
CWidgetTypeRegistry::IsA( const CString& typeName, const CString& baseTypeName ) const
{
    GUCEF::vector< CString > chain = GetTypeChain( typeName );
    for ( UInt32 i=0; i<(UInt32)chain.size(); ++i )
        if ( chain[ i ] == baseTypeName )
            return true;
    return false;
}

/*-------------------------------------------------------------------------*/

CWidgetTypeRegistry::TDescriptorVector
CWidgetTypeRegistry::GetProperties( const CString& typeName ) const
{
    TDescriptorVector result;
    GUCEF::vector< CString > chain = GetTypeChain( typeName );
    for ( Int32 c=(Int32)chain.size()-1; c>=0; --c )
    {
        const CWidgetTypeInfo* info = GetTypeInfo( chain[ c ] );
        for ( UInt32 p=0; p<(UInt32)info->properties.size(); ++p )
        {
            // a derived type may redefine a property (for example another default): the most derived wins
            bool replaced = false;
            for ( UInt32 r=0; r<(UInt32)result.size(); ++r )
            {
                if ( result[ r ]->name == info->properties[ p ].name )
                {
                    result[ r ] = &info->properties[ p ];
                    replaced = true;
                    break;
                }
            }
            if ( !replaced )
                result.push_back( &info->properties[ p ] );
        }
    }
    return result;
}

/*-------------------------------------------------------------------------*/

const CPropertyDescriptor*
CWidgetTypeRegistry::FindProperty( const CString& typeName, const CString& propertyName ) const
{
    GUCEF::vector< CString > chain = GetTypeChain( typeName );
    for ( UInt32 c=0; c<(UInt32)chain.size(); ++c )
    {
        const CWidgetTypeInfo* info = GetTypeInfo( chain[ c ] );
        for ( UInt32 p=0; p<(UInt32)info->properties.size(); ++p )
            if ( info->properties[ p ].name == propertyName )
                return &info->properties[ p ];
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CWidget*
CWidgetTypeRegistry::CreateWidget( const CString& typeName ) const
{
    const CWidgetTypeInfo* info = GetTypeInfo( typeName );
    if ( GUCEF_NULL == info || GUCEF_NULL == info->creator )
        return GUCEF_NULL;
    return info->creator();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
