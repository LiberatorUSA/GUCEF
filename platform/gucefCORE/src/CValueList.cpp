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

#include <assert.h>

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#include "CValueList.h"

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

GUCEF_IMPLEMENT_MSGEXCEPTION( CValueList, EUnknownKey );
GUCEF_IMPLEMENT_MSGEXCEPTION( CValueList, EIndexOutOfRange );

/*-------------------------------------------------------------------------*/

CValueList::CValueList( void )
    : CIConfigurable()
    , m_list()
    , m_allowDuplicates( false )
    , m_allowMultipleValues( true )
    , m_configNamespace()
    , m_configKeyNamespace()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CValueList::CValueList( const CValueList& src )
    : CIConfigurable()
    , m_list( src.m_list )
    , m_allowDuplicates( src.m_allowDuplicates )
    , m_allowMultipleValues( src.m_allowMultipleValues )
    , m_configNamespace( src.m_configNamespace )
    , m_configKeyNamespace( src.m_configKeyNamespace )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueList::~CValueList()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueList&
CValueList::operator=( const CValueList& src )
{GUCEF_TRACE;

     if ( this != &src )
     {
        m_list = src.m_list;
        m_allowDuplicates = src.m_allowDuplicates;
        m_allowMultipleValues = src.m_allowMultipleValues;
        m_configNamespace = src.m_configNamespace;
        m_configKeyNamespace = src.m_configKeyNamespace;
     }
     return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CValueList::operator[]( const CString& key )
{GUCEF_TRACE;

    return GetValue( key );
}

/*-------------------------------------------------------------------------*/

const CString&
CValueList::operator[]( const CString& key ) const
{GUCEF_TRACE;

    return GetValue( key );
}

/*-------------------------------------------------------------------------*/

void 
CValueList::SetMultiple( const CValueList& src )
{GUCEF_TRACE;

    TValueMap::const_iterator i = src.m_list.begin();
    while ( i != src.m_list.end() )
    {
        const CString& key = (*i).first;
        const TStringVector& values = (*i).second;

        TStringVector::const_iterator n = values.begin();
        while ( n != values.end() )
        {
            // We will just use the 'set' function
            // the settings on this value list instance will ensure proper merge strategy
            Set( key, (*n) );
            ++n;
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CValueList::SetMultiple( int argc    ,
                         char** argv )
{GUCEF_TRACE;

    for ( int i=0; i<argc; ++i )
    {
        Set( argv[ i ] );
    }
}

/*-------------------------------------------------------------------------*/

void
CValueList::SetConfigNamespace( const CString& configNamespace )
{GUCEF_TRACE;

    m_configNamespace = configNamespace;
}

/*-------------------------------------------------------------------------*/

const CString&
CValueList::GetConfigNamespace( void ) const
{GUCEF_TRACE;

    return m_configNamespace;
}

/*-------------------------------------------------------------------------*/

void
CValueList::SetConfigKeyNamespace( const CString& configKeyNamespace )
{GUCEF_TRACE;

    m_configKeyNamespace = configKeyNamespace;
}

/*-------------------------------------------------------------------------*/

const CString&
CValueList::GetConfigKeyNamespace( void ) const
{GUCEF_TRACE;

    return m_configKeyNamespace;
}

/*-------------------------------------------------------------------------*/

bool
CValueList::SaveConfig( CDataNode& tree ) const
{GUCEF_TRACE;

    CDataNode* nodeNamespaceRoot = tree.Structure( m_configNamespace, '/' );
    if ( GUCEF_NULL != nodeNamespaceRoot )
    {
        if ( m_allowDuplicates || m_allowMultipleValues )
        {
            TValueMap::const_iterator i = m_list.begin();
            while ( i != m_list.end() )
            {
                const TStringVector& values = (*i).second;
                TStringVector::const_iterator n = values.begin();
                while ( n != values.end() )
                {
                    CDataNode* child = nodeNamespaceRoot->AddChild( m_configKeyNamespace + (*i).first );
                    child->SetValue( (*n) );
                    ++n;
                }
                ++i;
            }
        }
        else
        {
            TValueMap::const_iterator i = m_list.begin();
            while ( i != m_list.end() )
            {
                const TStringVector& values = (*i).second;
                if ( !values.empty() )
                    nodeNamespaceRoot->SetAttribute( m_configKeyNamespace + (*i).first, values.front() );
                ++i;
            }
        }
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CValueList::LoadConfig( const CDataNode& treeroot )
{GUCEF_TRACE;

    const CDataNode* nodeNamespaceRoot = treeroot.Search( m_configNamespace, '/', true, true );
    if ( GUCEF_NULL != nodeNamespaceRoot )
    {
        // Get the key-value combos from the attributes
        CDataNode::TAttributeMap::const_iterator i = nodeNamespaceRoot->AttributeBegin();
        while ( i != nodeNamespaceRoot->AttributeEnd() )
        {
            if ( m_configKeyNamespace.IsNULLOrEmpty() )
            {
                Set( (*i).first, (*i).second );
            }
            else
            {
                if ( 0 == (*i).first.HasSubstr( m_configKeyNamespace ) )
                {
                    CString keyname = (*i).first.CutChars( m_configKeyNamespace.Length(), true, 0 );
                    Set( keyname, (*i).second );
                }
            }
            ++i;
        }

        // Also get the key-value combo's of Child nodes names + default value
        CDataNode::const_iterator n = nodeNamespaceRoot->Begin();
        while ( n != nodeNamespaceRoot->End() )
        {
            CString value = (*n)->GetValue().AsString();
            if ( !value.IsNULLOrEmpty() )
            {
                if ( m_configKeyNamespace.IsNULLOrEmpty() )
                {
                    Set( (*n)->GetName(), value );
                }
                else
                {
                    if ( 0 == (*n)->GetName().HasSubstr( m_configKeyNamespace ) )
                    {
                        CString keyname = (*n)->GetName().CutChars( m_configKeyNamespace.Length(), true, 0 );
                        Set( keyname, value );
                    }
                }
            }
            ++n;
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CValueList::Serialize( CDataNode& domRootNode                        ,
                       const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    if ( CDataNodeSerializableSettings::DataNodeSerializableLod_KeyOnly == settings.levelOfDetail )
    {
        domRootNode.SetNodeType( GUCEF_DATATYPE_ARRAY );        
        TValueMap::const_iterator i = m_list.begin();
        while ( i != m_list.end() )
        {
            domRootNode.AddValueAsChild( (*i).first );
            ++i;
        }
        return true;
    }
    else
    {
        // For now just use SaveConfig until we disentangle this
        return SaveConfig( domRootNode );
    }
}

/*-------------------------------------------------------------------------*/

bool
CValueList::Deserialize( const CDataNode& domRootNode                  ,
                         const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    // For now just use LoadConfig until we disentangle this
    return LoadConfig( domRootNode );

}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CValueList::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::CORE::CValueList";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
CValueList::SetMultiple( const CString& keyandvalue       ,
                         const char pairSeparator         ,
                         const char kvSeperator           ,
                         const CString* optionalKeyPrefix )
{GUCEF_TRACE;

    CString::StringVector kvPairs = keyandvalue.ParseElements( pairSeparator, false ); 
    CString::StringVector::iterator i = kvPairs.begin();
    while ( i != kvPairs.end() )
    {
        Set( (*i), kvSeperator, optionalKeyPrefix );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CValueList::SetAllowDuplicates( const bool allowDuplicates )
{GUCEF_TRACE;

    m_allowDuplicates = allowDuplicates;
}

/*-------------------------------------------------------------------------*/

bool
CValueList::GetAllowDuplicates( void ) const
{GUCEF_TRACE;

    return m_allowDuplicates;
}

/*-------------------------------------------------------------------------*/

void
CValueList::SetAllowMultipleValues( const bool allowMultipleValues )
{GUCEF_TRACE;

    m_allowMultipleValues = allowMultipleValues;
}

/*-------------------------------------------------------------------------*/

bool
CValueList::GetAllowMultipleValues( void ) const
{GUCEF_TRACE;

    return m_allowMultipleValues;
}

/*-------------------------------------------------------------------------*/

void
CValueList::Set( const CString& keyAndValue       , 
                 const char kvSeperator           ,
                 const CString* optionalKeyPrefix )
{GUCEF_TRACE;

    CString key( keyAndValue.SubstrToChar( kvSeperator, true ) );
    CString value( keyAndValue.SubstrToChar( kvSeperator, false ) );

    if ( GUCEF_NULL == optionalKeyPrefix )
    {
        Set( key   ,
             value );
    }
    else
    {
        Set( *optionalKeyPrefix + key ,
             value                    );
    }
}

/*-------------------------------------------------------------------------*/

void
CValueList::Set( const CString& key   ,
                 const CString& value )
{GUCEF_TRACE;

    if ( key.Length() > 0 )
    {
        TStringVector& values = m_list[ key ];
        if ( m_allowDuplicates )
        {
            values.push_back( value );
        }
        else
        {
            // Duplicates are not allowed so if an identical
            // value is already in the list we should not add it
            TStringVector::iterator n = values.begin();
            while ( n != values.end() )
            {
                if ( (*n) == value )
                {
                    // The given value is already in the list
                    return;
                }
                ++n;
            }

            if ( !m_allowMultipleValues )
            {
                values.clear();
            }
            values.push_back( value );
        }
    }
}

/*-------------------------------------------------------------------------*/

CValueList::TStringVector
CValueList::GetValueVectorAlways( const CString& key ) const
{GUCEF_TRACE;

    TStringVector results;
    TValueMap::const_iterator i = m_list.find( key );
    if ( i != m_list.end() )
    {
        results = (*i).second;
    }
    return results;
}

/*-------------------------------------------------------------------------*/

CValueList::TStringVector
CValueList::GetValueVectorAlways( const CString& key                 ,
                                  char valueSepChar                  ,
                                  const TStringVector& defaultValues ) const
{GUCEF_TRACE;

    const TStringVector* firstPass = GUCEF_NULL;
    TStringVector results;
    TValueMap::const_iterator i = m_list.find( key );
    if ( i != m_list.end() )
    {
        firstPass = &(*i).second;
        TStringVector::const_iterator n = firstPass->begin();
        while ( n != firstPass->end() )
        {
            TStringVector subSetResults = (*n).ParseElements( valueSepChar,  false );
            TStringVector::iterator m = subSetResults.begin();
            while ( m != subSetResults.end() )
            {
                results.push_back( (*m) );
                ++m;
            }
            ++n;
        }
    }

    if ( results.empty() )
    {
        results = defaultValues;
    }
    return results;
}

/*-------------------------------------------------------------------------*/

CValueList::TStringVector
CValueList::GetValueVectorAlways( const CString& key, char valueSepChar ) const
{GUCEF_TRACE;

    TStringVector emptyDefault;
    return GetValueVectorAlways( key, valueSepChar, emptyDefault );
}

/*-------------------------------------------------------------------------*/

const CString&
CValueList::GetValueAlways( const CString& key          ,
                            const CString& defaultValue ) const
{GUCEF_TRACE;

    TValueMap::const_iterator i = m_list.find( key );
    if ( i != m_list.end() )
    {
        return (*i).second[ 0 ];
    }

    return defaultValue;
}

/*-------------------------------------------------------------------------*/

const CString&
CValueList::GetValue( const CString& key ) const
{GUCEF_TRACE;

    TValueMap::const_iterator i = m_list.find( key );
    if ( i != m_list.end() )
    {
        return (*i).second[ 0 ];
    }

    GUCEF_EMSGTHROW( EUnknownKey, CString( "CValueList::GetValue(): The given key \"" + key + "\" is not found" ).C_String() );
}

/*-------------------------------------------------------------------------*/

CString&
CValueList::GetValue( const CString& key )
{GUCEF_TRACE;

    TValueMap::iterator i = m_list.find( key );
    if ( i != m_list.end() )
    {
        return (*i).second[ 0 ];
    }

    GUCEF_EMSGTHROW( EUnknownKey, "CValueList::GetValue(): The given key is not found" );
}

/*-------------------------------------------------------------------------*/

CValueList::TStringVector&
CValueList::GetValueVector( const CString& key )
{GUCEF_TRACE;

    TValueMap::iterator i = m_list.find( key );
    if ( i != m_list.end() )
    {
        return (*i).second;
    }

    GUCEF_EMSGTHROW( EUnknownKey, "CValueList::GetValue(): The given key is not found" );
}

/*-------------------------------------------------------------------------*/

const CValueList::TStringVector&
CValueList::GetValueVector( const CString& key ) const
{GUCEF_TRACE;

    TValueMap::const_iterator i = m_list.find( key );
    if ( i != m_list.end() )
    {
        return (*i).second;
    }

    GUCEF_EMSGTHROW( EUnknownKey, "CValueList::GetValue(): The given key is not found" );
}

/*-------------------------------------------------------------------------*/

const CString&
CValueList::GetValue( const UInt32 index ) const
{GUCEF_TRACE;

    if ( index < m_list.size() )
    {
        TValueMap::const_iterator i = m_list.begin();
        for ( UInt32 n=0; n<index; ++n ) { ++i; }

        return (*i).second[ 0 ];
    }

    GUCEF_EMSGTHROW( EIndexOutOfRange, "CValueList::GetValue( index ): The given index is invalid" );
}

/*-------------------------------------------------------------------------*/

CString&
CValueList::GetValue( const UInt32 index )
{GUCEF_TRACE;

    if ( index < m_list.size() )
    {
        TValueMap::iterator i = m_list.begin();
        for ( UInt32 n=0; n<index; ++n ) { ++i; }

        return (*i).second[ 0 ];
    }

    GUCEF_EMSGTHROW( EIndexOutOfRange, "CValueList::GetValue( index ): The given index is invalid" );
}

/*-------------------------------------------------------------------------*/

CString
CValueList::GetPair( const UInt32 index ) const
{GUCEF_TRACE;

    const CString& key = GetKey( index );
    const CString& value = GetValue( index );

    // if no exception was thrown we can now proceed to build the pair
    CString pair = key + '=' + value;
    return pair;
}

/*-------------------------------------------------------------------------*/

CString
CValueList::GetAllPairs( const UInt32 index          ,
                         const CString& seperatorStr ) const
{GUCEF_TRACE;

    const CString& key = GetKey( index );
    const TStringVector& values = GetValueVector( index );

    CString resultStr;
    bool first = true;
    for ( UInt32 i=0; i<values.size(); ++i )
    {
        if ( !first )
        {
            resultStr += seperatorStr;
        }
        else
        {
            first = false;
        }

        resultStr += key + '=' + values[ i ];
    }

    return resultStr;
}

/*-------------------------------------------------------------------------*/

CString
CValueList::GetAllPairs( const CString& seperatorStr ,
                         bool envelopElements        ,
                         const CString& envelopStr   ) const
{GUCEF_TRACE;

    CString resultStr;
    bool first = true;
    TValueMap::const_iterator i = m_list.begin();
    while ( i != m_list.end() )
    {
        const CString& key = (*i).first;
        const TStringVector& values = (*i).second;

        TStringVector::const_iterator n = values.begin();
        while ( n != values.end() )
        {
            if ( !first )
            {
                resultStr += seperatorStr;
            }
            else
            {
                first = false;
            }

            if ( envelopElements )
                resultStr += envelopStr + key + envelopStr + '=' + envelopStr + (*n) + envelopStr;
            else
                resultStr += key + '=' + (*n);
            ++n;
        }
        ++i;
    }
    return resultStr;
}

/*-------------------------------------------------------------------------*/

CString
CValueList::GetPair( const CString& key ) const
{GUCEF_TRACE;

    const CString& value = GetValue( key );

    // if no exception was thrown we can now proceed to build the pair
    CString pair = key + '=' + value;
    return pair;
}

/*-------------------------------------------------------------------------*/

const CString&
CValueList::GetKey( const UInt32 index ) const
{GUCEF_TRACE;

    if ( index < m_list.size() )
    {
        TValueMap::const_iterator i = m_list.begin();
        for ( UInt32 n=0; n<index; ++n ) { ++i; }

        return (*i).first;
    }

    GUCEF_EMSGTHROW( EIndexOutOfRange, "CValueList::GetKey( index ): The given index is invalid" );
}

/*-------------------------------------------------------------------------*/

bool
CValueList::HasKey( const CString& key ) const
{GUCEF_TRACE;

    return m_list.end() != m_list.find( key );
}

/*-------------------------------------------------------------------------*/

CValueList::TStringVector
CValueList::GetKeysWithWildcardKeyMatch( const CString& searchStr ,
                                         char wildCardChar        ,
                                         bool caseSensitive       ) const
{GUCEF_TRACE;

    TStringVector keys;
    TValueMap::const_iterator i = m_list.begin();
    while ( i != m_list.end() )
    {
        const CString& key = (*i).first;
        if ( key.WildcardEquals( searchStr, wildCardChar, caseSensitive ) )
        {
            keys.push_back( key );
        }
        ++i;
    }
    return keys;
}

/*-------------------------------------------------------------------------*/

void
CValueList::Delete( const CString& key )
{GUCEF_TRACE;

    m_list.erase( key );
}

/*-------------------------------------------------------------------------*/

void
CValueList::DeleteAll( void )
{GUCEF_TRACE;

    m_list.clear();
}

/*-------------------------------------------------------------------------*/

UInt32
CValueList::GetCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_list.size();
}

/*-------------------------------------------------------------------------*/

const CValueList::TStringVector&
CValueList::GetValueVector( const UInt32 index ) const
{GUCEF_TRACE;

    if ( index < m_list.size() )
    {
        TValueMap::const_iterator i = m_list.begin();
        for ( UInt32 n=0; n<index; ++n ) { ++i; }

        return (*i).second;
    }

    GUCEF_EMSGTHROW( EIndexOutOfRange, "CValueList::GetValueVector( index ): The given index is invalid" )
}

/*-------------------------------------------------------------------------*/

CValueList::TValueMap::const_iterator
CValueList::GetDataBeginIterator( void ) const
{GUCEF_TRACE;

    return m_list.begin();
}

/*-------------------------------------------------------------------------*/

CValueList::TValueMap::const_iterator
CValueList::GetDataEndIterator( void ) const
{GUCEF_TRACE;

    return m_list.end();
}

/*-------------------------------------------------------------------------*/

void
CValueList::Clear( void )
{GUCEF_TRACE;

    m_list.clear();
    m_allowDuplicates = false;
    m_allowMultipleValues = true;
}

/*-------------------------------------------------------------------------*/

void 
CValueList::CopySettingsFrom( const CValueList& other )
{GUCEF_TRACE;

    m_allowDuplicates = other.m_allowDuplicates;
    m_allowMultipleValues = other.m_allowMultipleValues;
    m_configNamespace = other.m_configNamespace;
    m_configKeyNamespace = other.m_configKeyNamespace;    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
