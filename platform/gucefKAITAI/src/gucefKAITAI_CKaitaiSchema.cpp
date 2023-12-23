/*
 *  gucefKAITAI: Platform module supporting the Kaitai binary description format
 *
 *  Copyright (C) 1998 - 2023.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

#include "gucefKAITAI_CKaitaiSchema.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace KAITAI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CKaitaiSchema::ClassTypeName = "GUCEF::KAITAI::CKaitaiSchema";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CKaitaiSchema::CKaitaiSchema( void )
    : CORE::CIDataNodeSerializable()
    , id()
    , imports()
    , endianess( GUCEF_BYTEORDER_UNKNOWN_ENDIAN )
    , fields()
    , enums()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CKaitaiSchema::CKaitaiSchema( const CKaitaiSchema& src )
    : CORE::CIDataNodeSerializable( src )
    , id( src.id )
    , imports( src.imports )
    , endianess( src.endianess )
    , fields( src.fields )
    , enums( src.enums )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CKaitaiSchema& 
CKaitaiSchema::operator=( const CKaitaiSchema& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CORE::CIDataNodeSerializable::operator=( src );
        id = src.id;
        imports = src.imports;
        endianess = src.endianess;
        fields = src.fields;
        enums = src.enums;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CKaitaiSchema::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CKaitaiSchema( *this );
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CKaitaiSchema::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

bool
CKaitaiSchema::Serialize( CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    return false;
}


/*-------------------------------------------------------------------------*/

bool 
CKaitaiSchema::DeserializeEnumDefinitions( const CORE::CDataNode& domRootNode                  , 
                                           const CORE::CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    const CORE::CDataNode* enumsNode = domRootNode.FindChild( "enums" );
    if ( GUCEF_NULL != enumsNode )
    {
        bool totalSuccess = true;
        CORE::CDataNode::const_iterator i = enumsNode->ConstBegin();
        while ( i != enumsNode->ConstEnd() )
        {
            const CORE::CDataNode* enumNode = (*i);
            
            CKaitaiSchemaEnumDefinition enumDef;
            if ( enumDef.Deserialize( *enumNode, settings ) && !enumDef.id.IsNULLOrEmpty() )
            {
                enums[ enumDef.id ] = enumDef;    
            }
            else
            {
                totalSuccess = false;
            }
            ++i;
        }

        return totalSuccess;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CKaitaiSchema::DeserializeFieldSequence( const CORE::CDataNode& domRootNode                  , 
                                         const CORE::CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    const CORE::CDataNode* seqNode = domRootNode.FindChild( "seq" );
    if ( GUCEF_NULL != seqNode )
    {
        bool totalSuccess = true;
        CORE::CDataNode::const_iterator i = seqNode->ConstBegin();
        while ( i != seqNode->ConstEnd() )
        {
            const CORE::CDataNode* fieldNode = (*i);
            
            CKaitaiSchemaFieldDefinition field;
            if ( field.Deserialize( *fieldNode, settings ) && !field.id.IsNULLOrEmpty() )
            {
                fields[ field.id ] = field;    
            }
            else
            {
                totalSuccess = false;
            }
            ++i;
        }

        return totalSuccess;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CKaitaiSchema::DeserializeMetaData( const CORE::CDataNode& domRootNode                  , 
                                    const CORE::CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;
    
    const CORE::CDataNode* metaNode = domRootNode.FindChild( "meta" );
    if ( GUCEF_NULL != metaNode )
    {
        id = metaNode->GetAttributeValueOrChildValueByName( "id", id, true ).AsString( id, true );

        const CORE::CString& endianCurrentStr = CKaitaiSchemaFieldDefinition::EndianValueToString( endianess );
        CORE::CString endianStr = metaNode->GetAttributeValueOrChildValueByName( "endian", endianCurrentStr, true ).AsString( endianCurrentStr, true );
        endianess = CKaitaiSchemaFieldDefinition::StringToEndianValue( endianStr );

        CORE::CDataNode::TVariantVector importsList;
        if ( domRootNode.GetValuesOfChildByName( "imports", importsList ) )
            imports = CORE::ToStringSet( importsList );
        
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKaitaiSchema::Deserialize( const CORE::CDataNode& domRootNode, const CORE::CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    if ( DeserializeMetaData( domRootNode, settings ) )
    {
        bool totalSuccess = true;
        totalSuccess = DeserializeFieldSequence( domRootNode, settings ) && totalSuccess;
        totalSuccess = DeserializeEnumDefinitions( domRootNode, settings ) && totalSuccess;
        return totalSuccess;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAITAI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
