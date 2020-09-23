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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "CDataNode.h"  /* definition of the class implemented here */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDataNode::CDataNode( int nodeType )
    : m_nodeType( nodeType )
    , _pparent( NULL ) 
    , m_children()     
    , _pnext( NULL )   
    , _pprev( NULL )   
    , _name()          
    , m_value()
    , m_typeOfValue( MT::DATATYPE_STRING )
    , m_associatedData( GUCEF_NULL )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CDataNode::CDataNode( const CString& name, int nodeType )
    : m_nodeType( nodeType )
    , _name( name )    
    , m_value()
    , m_typeOfValue( MT::DATATYPE_STRING )        
    , _pparent( NULL ) 
    , m_children()     
    , _pnext( NULL )   
    , _pprev( NULL )
    , m_associatedData( GUCEF_NULL )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CDataNode::CDataNode( const CDataNode& src )
    : m_nodeType( src.m_nodeType )
    , _name( src._name )       
    , m_value( src.m_value ) 
    , m_typeOfValue( src.m_typeOfValue )  
    , _atts( src._atts )       
    , _pparent( src._pparent ) 
    , _pnext( src._pnext )     
    , _pprev( src._pprev )     
    , m_children()
    , m_associatedData( GUCEF_NULL )
{GUCEF_TRACE;
    
    if ( GUCEF_NULL != src.m_associatedData )
        m_associatedData = src.m_associatedData->Clone();    
    
    TDataNodeList::const_iterator n = src.m_children.cbegin();
    while ( n != src.m_children.cend() )
    {
        AddChild( *(*n) );
        ++n;
    }             
}

/*-------------------------------------------------------------------------*/
        
CDataNode::~CDataNode()
{GUCEF_TRACE;
        
    delete m_associatedData;
    m_associatedData = GUCEF_NULL; 
    
    Detach();             
    DelSubTree();
}

/*-------------------------------------------------------------------------*/

void
CDataNode::Clear( void )
{GUCEF_TRACE;

    m_nodeType = GUCEF_DATATYPE_OBJECT;
    _name.Clear();
    m_value.Clear();
    DelSubTree();
    ClearAttributes();
}

/*-------------------------------------------------------------------------*/

void       
CDataNode::Detach( void )
{GUCEF_TRACE;

    if ( 0 != _pparent )
    {
        _pparent->DetachChild( this );                        
    }
    _pnext = NULL;
    _pprev = NULL;
    _pparent = NULL;     
}

/*-------------------------------------------------------------------------*/

void
CDataNode::DetachChild( CDataNode* child )
{GUCEF_TRACE;       

    if ( nullptr == child ) return;
    
    TDataNodeList::iterator i = m_children.begin();
    while ( i != m_children.end() )
    {
        if ( (*i) == child )
        {   
            if ( nullptr != child->_pnext )
            {
                child->_pnext->_pprev = child->_pprev;
            }
            if ( nullptr != child->_pprev )
            {
                child->_pprev->_pnext = child->_pnext;
            }            

            // remove the entry but dont delete the object
            m_children.erase( i );
            return;
        }
        ++i;
    }                     
}

/*-------------------------------------------------------------------------*/

CDataNode& 
CDataNode::operator=( const CDataNode& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        // detach from our current tree
        Detach();
        DelSubTree();
        
        m_nodeType = src.m_nodeType;
        _name = src._name;
        m_value = src.m_value;
        m_typeOfValue = src.m_typeOfValue;
        _atts = src._atts;

        SetAssociatedData( src.m_associatedData );
    }               
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::Copy( const CDataNode& root )
{GUCEF_TRACE;

    if ( this != &root )
    {
        *this = root;
        CopySubTree( root );
    }
}

/*-------------------------------------------------------------------------*/
        
void 
CDataNode::SetName( const CString& name )
{GUCEF_TRACE;

    _name = name;
}

/*-------------------------------------------------------------------------*/
        
const CString&
CDataNode::GetName( void ) const
{GUCEF_TRACE;

    return _name;
}

/*-------------------------------------------------------------------------*/

void
CDataNode::SetValue( const CString& value )
{GUCEF_TRACE;

    m_value = value;
}

/*-------------------------------------------------------------------------*/

const CString& 
CDataNode::GetValue( void ) const
{
    return m_value;
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::HasValue( void ) const
{GUCEF_TRACE;
    
    return !m_value.IsNULLOrEmpty();
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::SetValueType( int typeId )
{GUCEF_TRACE;

    m_typeOfValue = typeId;
}

/*-------------------------------------------------------------------------*/

int 
CDataNode::GetValueType( void ) const
{GUCEF_TRACE;

    return m_typeOfValue;
}

/*-------------------------------------------------------------------------*/

int 
CDataNode::GetNodeType( void ) const
{GUCEF_TRACE;

    return m_nodeType;
}

/*-------------------------------------------------------------------------*/
        
bool 
CDataNode::IsAttribute( const CString& name ) const
{GUCEF_TRACE;

    return _atts.end() != _atts.find( name );
}

/*-------------------------------------------------------------------------*/
        
const CDataNode::TKeyValuePair* 
CDataNode::operator[]( UInt32 index ) const
{GUCEF_TRACE;
        
    UInt32 n = 0;
    TAttributeMap::const_iterator i = _atts.begin();
    while ( i != _atts.end() )
    {
        if ( n == index )
        {
            return const_cast< const CDataNode::TKeyValuePair* >( &(*i) );
        }
        ++n;        
        ++i;
    }
    return 0;        
}

/*-------------------------------------------------------------------------*/
        
CDataNode::TKeyValuePair* 
CDataNode::operator[]( UInt32 index )
{GUCEF_TRACE;

    UInt32 n = 0;
    TAttributeMap::iterator i = _atts.begin();
    while ( i != _atts.end() )
    {
        if ( n == index )
        {
            return &(*i);
        }
        ++n;        
        ++i;
    }
    return 0;  
}

/*-------------------------------------------------------------------------*/
        
const CDataNode::TKeyValuePair* 
CDataNode::operator[]( const CString& name ) const
{GUCEF_TRACE;

    TAttributeMap::const_iterator i = _atts.find( name );
    if ( i != _atts.end() )
    {
        return &(*i);
    }
    return 0;
}

/*-------------------------------------------------------------------------*/
        
CDataNode::TKeyValuePair* 
CDataNode::operator[]( const CString& name )
{GUCEF_TRACE;

    TAttributeMap::iterator i = _atts.find( name );
    if ( i != _atts.end() )
    {
        return &(*i);
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CDataNode::ClearAttributes( void )
{GUCEF_TRACE;

    _atts.clear();
}

/*-------------------------------------------------------------------------*/
        
const CDataNode::TKeyValuePair* 
CDataNode::GetAttribute( UInt32 index ) const
{
    return this->operator[]( index );      
}

/*-------------------------------------------------------------------------*/
        
CDataNode::TKeyValuePair* 
CDataNode::GetAttribute( UInt32 index )
{GUCEF_TRACE;

    return this->operator[]( index ); 
}

/*-------------------------------------------------------------------------*/
        
const CDataNode::TKeyValuePair* 
CDataNode::GetAttribute( const CString& name ) const
{GUCEF_TRACE;

    return this->operator[]( name );
}

/*-------------------------------------------------------------------------*/

CString 
CDataNode::GetAttributeValue( const CString& name ) const
{GUCEF_TRACE;

    return GetAttributeValue( name, CString::Empty );
}

/*-------------------------------------------------------------------------*/

CString 
CDataNode::GetAttributeValue( const CString& name         , 
                              const CString& defaultValue ) const
{GUCEF_TRACE;

    TAttributeMap::const_iterator i = _atts.find( name );
    if ( i != _atts.end() )
    {
        return (*i).second.value;
    }
    return defaultValue;
}

/*-------------------------------------------------------------------------*/
        
CDataNode::TKeyValuePair* 
CDataNode::GetAttribute( const CString& name )
{GUCEF_TRACE;

    TAttributeMap::iterator i = _atts.find( name );
    if ( i != _atts.end() )
    {
        return &(*i);
    }
    return 0;
}

/*-------------------------------------------------------------------------*/
   
bool
CDataNode::AddAttribute( const TKeyValuePair& att )
{GUCEF_TRACE;

    TAttributeMap::iterator i = _atts.find( att.first );
    if ( i == _atts.end() )
    {
        _atts.insert( att );
        return true;
    }
    return false;     
}

/*-------------------------------------------------------------------------*/   
        
bool 
CDataNode::AddAttribute( const CString& name  ,
                         const CString& value ,
                         int typeOfValue      )
{GUCEF_TRACE;

    TAttributeMap::iterator i = _atts.find( name );
    if ( i == _atts.end() )
    {
        auto& att = _atts[ name ]; 
        att.value = value;
        att.type = typeOfValue;
        return true;
    }
    return false;                       
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::SetAttribute( const CString& name  ,
                         const CString& value ,
                         int typeOfValue      )
{GUCEF_TRACE;

    auto& att = _atts[ name ];
    att.value = value;
    att.type = typeOfValue;
    return true;                      
}

/*-------------------------------------------------------------------------*/

bool
CDataNode::SetAttribute( const char* name  ,
                         const char* value ,
                         int typeOfValue   )
{GUCEF_TRACE;

    auto& att = _atts[ name ];
    att.value = value;
    att.type = typeOfValue;
    return true;                      
}
/*-------------------------------------------------------------------------*/

bool 
CDataNode::SetAttribute( const CString& name ,
                         bool value          )
{GUCEF_TRACE;

    auto& att = _atts[ name ];
    att.value = CORE::BoolToString( value );
    att.type = GUCEF_DATATYPE_BOOLEAN_STRING;
    return true;                      
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::SetAttribute( const CString& name ,
                         UInt32 value        )
{GUCEF_TRACE;

    auto& att = _atts[ name ];
    att.value = CORE::UInt32ToString( value );
    att.type = GUCEF_DATATYPE_UINT32;
    return true;                      
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::SetAttribute( const CString& name ,
                         Int32 value         )
{GUCEF_TRACE;

    auto& att = _atts[ name ];
    att.value = CORE::Int32ToString( value );
    att.type = GUCEF_DATATYPE_INT32;
    return true;                      
}

/*-------------------------------------------------------------------------*/                         
        
UInt32 
CDataNode::GetAttCount( void ) const
{GUCEF_TRACE;

    return (UInt32) _atts.size();
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::CopySubTree( const CDataNode& root )
{GUCEF_TRACE;

    if ( this != &root )
    {
        TDataNodeList::const_iterator n = root.m_children.cbegin();
        while ( n != root.m_children.cend() )
        {
            AddChild( *(*n) );
            ++n;
        }                                
    }
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::DelAttribute( const CString& name )
{
    _atts.erase( name );      
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::FindRoot( void ) const
{
        GUCEF_BEGIN;
        if ( !_pparent )
        {
                GUCEF_END;
                return (CDataNode*)this;
        }        
        GUCEF_END_RET( CDataNode*, _pparent->FindRoot() );
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::FindChild( const CString& name ) const
{GUCEF_TRACE;
        
    TDataNodeList::const_iterator i = m_children.cbegin();
    while ( i != m_children.end() )
    {
        if ( (*i)->_name == name )
            return (*i);
        ++i;
    }
    return nullptr;
}

/*-------------------------------------------------------------------------*/

CDataNode::TConstDataNodeSet
CDataNode::FindChildrenOfType( const CString& name  ,
                               const bool recursive ) const
{GUCEF_TRACE;

    TConstDataNodeSet children;
    TDataNodeList::const_iterator m = m_children.cbegin();
    while ( m != m_children.cend() )
    {
        const CDataNode* child = (*m);
        if ( child->_name == name )
        {
            children.insert( child );            
        }
        if ( recursive )
        {
            
            TConstDataNodeSet subSet( child->FindChildrenOfType( name, recursive ) );
            TConstDataNodeSet::const_iterator i = subSet.begin();
            while ( i != subSet.end() )
            {
                children.insert( (*i) );
                ++i;
            }
        }        
        ++m;
    }
    return children;
}

/*-------------------------------------------------------------------------*/

CDataNode::TDataNodeSet
CDataNode::FindNodesOfType( const CString& name  ,
                            const bool recursive )
{GUCEF_TRACE;

    TDataNodeSet children = FindChildrenOfType( name, recursive );
    if ( _name == name )
    {
        children.insert( this );            
    }
    return children;
}

/*-------------------------------------------------------------------------*/

CDataNode::TDataNodeSet
CDataNode::FindChildrenOfType( const CString& name  ,
                               const bool recursive )
{GUCEF_TRACE;

    TDataNodeSet children;
    TDataNodeList::iterator m = m_children.begin();
    while ( m != m_children.end() )
    {
        if ( (*m)->_name == name )
        {
            children.insert( (*m) );            
        }
        if ( recursive )
        {
            TDataNodeSet subSet( (*m)->FindChildrenOfType( name, recursive ) );
            TDataNodeSet::const_iterator i = subSet.begin();
            while ( i != subSet.end() )
            {
                children.insert( (*i) );
                ++i;
            }
        }
        ++m;
    }
    return children;
}

/*-------------------------------------------------------------------------*/

CDataNode*
CDataNode::FindChild( const CString& name        ,
                      const CString& attribName  ,
                      const CString& attribValue )
{GUCEF_TRACE;

    TDataNodeList::iterator i = m_children.begin();
    while ( i != m_children.end() )
    {
        // Check if the name matches
        if ( (*i)->_name == name )
        {
            // See if the node has the attribute we want
            TKeyValuePair* att = (*i)->GetAttribute( attribName );
            if ( att != NULL )
            {
                // See if the node attribute has the value we want
                if ( att->second.value == attribValue )
                {
                    return (*i);
                }
            }
        }
        ++i;
    }  
    return nullptr;    
}

/*-------------------------------------------------------------------------*/

bool
CDataNode::Compare( const CDataNode& other        ,
                    const TStringSet& excludeList ) const
{GUCEF_TRACE;

    // identical nodes should of course have identical names
    if ( _name == other._name )
    {
        const TKeyValuePair* att;
        const TKeyValuePair* att2;
        const UInt32 count = (UInt32) other._atts.size();
        
        // if one has a bigger list then the other then they cannot be the same
        if ( _atts.size() == count )
        {
            // same number of entries in the lists, we will have to compare each item
            for ( UInt32 i=0; i<count; ++i )
            {
                    att = other[ i ];
                    if ( att )
                    {
                        if ( excludeList.end() == excludeList.find( att->first ) )
                        {
                            att2 = GetAttribute( att->first );
                            if ( att2 )
                            {
                                if ( att->second.value != att2->second.value )
                                {
                                        return false;
                                }
                            }                            
                        }
                    }                                                        
            }
            
            // Everything checked out,.. nodes are identical content wise when taking
            // the exclusion list into account.
            return true;
        }
        return false;             
    }
    return false;    
}

/*-------------------------------------------------------------------------*/

const CDataNode* 
CDataNode::CompareChild( const CDataNode& other        ,
                         const TStringSet& excludeList ) const
{GUCEF_TRACE;

    const_iterator i( ConstBegin() );
    while ( i != ConstEnd() )
    {
        if ( (*i)->Compare( other       , 
                            excludeList ) )
        {
            return (*i);
        }                             
        
        ++i;
    }
    return NULL;   
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::operator==( const CDataNode& other ) const
{GUCEF_TRACE;
    
    return Compare( other, TStringSet() );
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::operator!=( const CDataNode& other ) const
{GUCEF_TRACE;
    return !( (*this) == other );
}

/*-------------------------------------------------------------------------*/

const CDataNode* 
CDataNode::FindSibling( const CString& name ) const
{GUCEF_TRACE;

    if ( name == _name )
    {
            return this;
    }
    
    CDataNode* n = _pnext;
    while ( n )
    {
            if ( n->_name == name )
            {       
                    return n;
            }
            n = n->_pnext;
    }
    n = _pprev;
    while ( n )
    {
            if ( n->_name == name )
            {       
                    return n;
            }
            n = n->_pprev;
    }        
    return NULL;        
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::FindSibling( const CString& name )
{GUCEF_TRACE;

    if ( name == _name )
    {
            return this;
    }
    
    CDataNode* n = _pnext;
    while ( n )
    {
            if ( n->_name == name )
            {       
                    return n;
            }
            n = n->_pnext;
    }
    n = _pprev;
    while ( n )
    {
            if ( n->_name == name )
            {       
                    return n;
            }
            n = n->_pprev;
    }        
    return NULL;        
}

/*-------------------------------------------------------------------------*/

CDataNode::TAttributeMap::const_iterator
CDataNode::AttributeBegin( void ) const
{GUCEF_TRACE;

    return _atts.begin();
}

/*-------------------------------------------------------------------------*/
    
CDataNode::TAttributeMap::const_iterator
CDataNode::AttributeEnd( void ) const
{GUCEF_TRACE;
    
    return _atts.end();
}

/*-------------------------------------------------------------------------*/

CString
CDataNode::GetPathToRoot( char nodeNameSeperator , 
                          bool includeThisNode   ) const
{GUCEF_TRACE;

    if ( NULL != _pparent )
    {
        return _pparent->GetPathToRoot( nodeNameSeperator, includeThisNode ) + nodeNameSeperator + _name;
    }
    return _name;
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::Find( const CString& name )
{GUCEF_TRACE;    

    CDataNode* n = FindSibling( name );
    if ( nullptr != n ) 
    {
        return n;
    }                

    TDataNodeList::iterator m = m_children.begin();
    while ( m != m_children.end() )
    {
        n = (*m)->Find( name );
        if ( nullptr != n )
        {
            return n;
        }
        ++m;
    }
    return nullptr;       
}

/*-------------------------------------------------------------------------*/

const CDataNode* 
CDataNode::Find( const CString& name ) const
{GUCEF_TRACE;    

    const CDataNode* n = FindSibling( name );
    if ( nullptr != n ) 
    {
        return n;
    }                

    TDataNodeList::const_iterator m = m_children.cbegin();
    while ( m != m_children.cend() )
    {
        n = (*m)->Find( name );
        if ( nullptr != n )
        {
            return n;
        }
        ++m;
    }
    return nullptr;       
}

/*-------------------------------------------------------------------------*/

UInt32
CDataNode::GetNrOfChildNodes( void ) const
{GUCEF_TRACE;

    UInt32 count = 0;
    TDataNodeList::const_iterator i = m_children.cbegin();
    while ( i != m_children.cend() )
    {
        count += (*i)->GetNrOfChildNodes()+1;
        ++i;
    }
    return count;
}

/*-------------------------------------------------------------------------*/

UInt32
CDataNode::GetNrOfTreeNodes( void ) const
{GUCEF_TRACE;

    return FindRoot()->GetNrOfChildNodes()+1;    
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::Search( const CString& query     ,
                   char seperator           ,
                   bool fromcurrent         ,
                   bool treatChildAsCurrent ) const
{GUCEF_TRACE;

    if ( fromcurrent )
    {
        CString thisname( query.SubstrToChar( seperator, true ) );
        if ( thisname == _name )
        {
            CString remnant( query.CutChars( thisname.Length()+1, true ) );
            if ( remnant.Length() > 0 )
            {            
                CString leftover;
                TDataNodeVector results = WalkTree( remnant   ,
                                                    seperator ,
                                                    leftover  );
                if ( 0 == leftover.Length() && !results.empty() )
                {       
                    return *results.begin();
                }                                 
            }
            else
            {   
                // this node is the leaf node we are searching for
                return const_cast< CDataNode* >( this );
            }
        }

        if ( treatChildAsCurrent )
        {
            TDataNodeList::const_iterator i = m_children.begin();
            while ( i != m_children.end()  )
            {
                CDataNode* result = (*i)->Search( query       ,
                                                  seperator   ,
                                                  fromcurrent ,
                                                  false       );                
                if ( nullptr != result )
                    return result;
                ++i;
            }
        }
        return nullptr;                                            
    }
    else
    {
        CString leftover;
        TDataNodeVector results = WalkTree( query     ,
                                            seperator ,
                                            leftover  );
        if ( 0 == leftover.Length() && !results.empty() )
        {       
            return *results.begin();
        }
        return nullptr;
    }                
}

/*-------------------------------------------------------------------------*/

CDataNode::TDataNodeVector 
CDataNode::SearchForAll( const CString& query     ,
                         char seperator           ,
                         bool fromcurrent         ,
                         bool treatChildAsCurrent ) const
{GUCEF_TRACE;

    if ( fromcurrent )
    {
        CString thisname( query.SubstrToChar( seperator, true ) );
        if ( thisname == _name )
        {
            CString remnant( query.CutChars( thisname.Length()+1, true ) );
            if ( remnant.Length() > 0 )
            {            
                CString leftover;
                TDataNodeVector results = WalkTree( remnant   ,
                                                    seperator ,
                                                    leftover  );
                if ( 0 == leftover.Length() )
                {       
                    return results;
                }                                 
            }
            else
            {   
                // this node is the leaf node we are searching for
                TDataNodeVector result;
                result.push_back( const_cast< CDataNode* >( this ) );
                return result;
            }
        }
        else
        if ( treatChildAsCurrent )
        {
            TDataNodeVector results;
            TDataNodeList::const_iterator i = m_children.begin();
            while ( i != m_children.end()  )
            {
                TDataNodeVector childResults = (*i)->SearchForAll( query       ,
                                                                   seperator   ,
                                                                   fromcurrent ,
                                                                   false       );                
                TDataNodeVector::iterator n = childResults.begin();
                while ( n != childResults.end() )
                {
                    results.push_back( (*n) );
                    ++n;
                }
                ++i;
            }
            return results;
        }
        return TDataNodeVector();                                            
    }
    else
    {
        CString leftover;
        TDataNodeVector results = WalkTree( query     ,
                                            seperator ,
                                            leftover  );
        if ( 0 == leftover.Length() )
        {       
            return results;
        }
        return TDataNodeVector();
    }
}

/*-------------------------------------------------------------------------*/

CDataNode::TDataNodeVector
CDataNode::WalkTree( const CString& sequence ,
                     char seperator          ,
                     CString& sleftover      ) const
{GUCEF_TRACE;

    sleftover = sequence;
        
    return WalkTreeImp( sleftover ,
                        seperator );
}                     

/*-------------------------------------------------------------------------*/

CDataNode::TDataNodeVector
CDataNode::WalkTreeImp( CString& sleftover ,
                        char seperator     ) const
{GUCEF_TRACE;
          
    if ( !m_children.empty() )
    {
        CString searchseg( sleftover.SubstrToChar( seperator, true ) );
        CString left( sleftover.CutChars( searchseg.Length()+1, true ) );
        
        TDataNodeVector resultSet;                        
        CDataNode* sn = (CDataNode*) this;       
        CString bestMatchLeftover( left );
                                                  
        CDataNode* n = m_children.front();                                                                                    
        while ( n )
        {
            // Are we looking for more nesting or a leaf node?
            if ( 0 == left.Length() )
            {
                // We are looking for a leaf node. Check if the current child matches
                if ( n->_name == searchseg )
                {
                    // nothing left to search for, so
                    // no point in continuing
                    resultSet.push_back( n );
                }        
            }
            else
            {            
                // check if this node could be a link in the search chain
                if ( n->_name == searchseg )
                {
                    // search the tree for our leftover
                    CString childLeftover = left;
                    TDataNodeVector childResultSet = n->WalkTreeImp( childLeftover, seperator );
                        
                    // if what we found is better then what we found so far then
                    // substitute the current deepest nodes with the new deeper nodes.
                    if ( bestMatchLeftover.Length() > childLeftover.Length() )                  
                    {
                        // We found a better match, switch to that one
                        resultSet = childResultSet;
                        bestMatchLeftover = childLeftover;
                    }
                    else
                    if ( bestMatchLeftover.Length() == childLeftover.Length() )
                    {
                        if ( childResultSet.empty() )
                        {
                            // The child match itself is currently the best match
                            resultSet.push_back( n );
                        }
                        else
                        {
                            // We found more equally good matches deeper down the child tree
                            TDataNodeVector::iterator m = childResultSet.begin();
                            while ( m != childResultSet.end() )
                            {
                                resultSet.push_back( (*m) );
                                ++m;
                            }
                        }
                    }
                }
            }
            n = n->_pnext;
        }

        if ( !resultSet.empty() )
            sleftover = bestMatchLeftover;
        return resultSet;
    }
    
    TDataNodeVector resultSet;
    resultSet.push_back( const_cast<CDataNode*>( this ) );
    return resultSet;                                                                          
}                     

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::Structure( const CString& sequence ,
                      char seperator          )
{GUCEF_TRACE;

    // walk the tree
    CString buildseg;
    TDataNodeVector walkResults = WalkTree( sequence  ,
                                            seperator ,
                                            buildseg  );
                 
    // do we need to add nodes ?
    if ( 0 < buildseg.Length() )
    {
        CDataNode* parentNode = this;
        if ( !walkResults.empty() )
            parentNode = *walkResults.begin(); 
        
        CDataNode child;
        CString name;
        while ( buildseg.Length() )
        {
            name = buildseg.SubstrToChar( seperator, true );
            child.SetName( name );                        
            buildseg = buildseg.CutChars( name.Length()+1, true );                        
            parentNode = parentNode->AddChild( child );
        }
        return parentNode;
    }
    
    if ( walkResults.empty() )
        return nullptr;
    return *walkResults.begin();        
}                      

/*-------------------------------------------------------------------------*/

CDataNode*
CDataNode::Structure( const CString& nodeName       ,
                      const CString& attribName     ,
                      const CString& attribSequence ,
                      const char seperator          )
{GUCEF_TRACE;

    // Prepare some variables for the first loop iteration
    CDataNode* node = this;
    CString attSeqRemnant = attribSequence;
    CString attValue = attSeqRemnant.SubstrToChar( seperator );
    bool childExists = true;
    CDataNode* childNode = NULL;
    
    do
    {
        // First we check if we can skip the search for a child node
        // This is a minor optimization        
        if ( childExists )
        {
            // See if there already is a node of the given type
            childNode = node->FindChild( nodeName   ,
                                         attribName ,
                                         attValue   );
            if ( childNode == NULL )
            {
                childExists = false;
            }
        }
        
        // Check if we have to create a new node
        if ( childNode == NULL )
        {
            // No such node exists, we will create it
            CDataNode newChild( nodeName );
            newChild.AddAttribute( attribName, attValue );
            childNode = node->AddChild( newChild );
        }
        
        node = childNode;
        
        // Get the next segment
        attSeqRemnant = attSeqRemnant.CutChars( attValue.Length()+1, true );
        attValue = attSeqRemnant.SubstrToChar( seperator );
        
    } while ( attSeqRemnant.Length() > 0 );
    
    return childNode;
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::DelSubTree( void )
{GUCEF_TRACE;

    TDataNodeList::iterator i = m_children.begin();
    while ( !m_children.empty() )
    {
        CDataNode* child = m_children.front();
        m_children.erase( m_children.begin() );
        delete child;
    }  
}        

/*-------------------------------------------------------------------------*/
      
CDataNode* 
CDataNode::AddChild( const CDataNode& newnode )
{GUCEF_TRACE;

    CDataNode* n = new CDataNode( newnode );
    n->_pprev = nullptr;
    n->_pnext = nullptr;
    n->_pparent = this;        

    if ( !m_children.empty() )
    {
        CDataNode* last = m_children.back();
        last->_pnext = n;
        n->_pprev = last;
    }

    m_children.push_back( n );
    return n;                                  
}

/*-------------------------------------------------------------------------*/

CDataNode*
CDataNode::AddChild( const CString& nodeName, int nodeType )
{GUCEF_TRACE;

    CDataNode newNode( nodeName, nodeType );
    return AddChild( newNode );
}

/*-------------------------------------------------------------------------*/

CString
CDataNode::GetChildValueByName( const CString& name ) const
{GUCEF_TRACE;

    CDataNode* childNode = FindChild( name );
    if ( NULL != childNode )
    {
        return childNode->GetValue();
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

CString
CDataNode::GetAttributeValueOrChildValueByName( const CString& name, const CString& defaultValue ) const
{GUCEF_TRACE;

    CString value = GetAttributeValue( name );
    if ( value.IsNULLOrEmpty() )
    {
        value = GetChildValueByName( name );
        if ( value.IsNULLOrEmpty() )
        {
            return defaultValue;
        }
    }
    return value;
}

/*-------------------------------------------------------------------------*/

CDataNode::TStringVector
CDataNode::GetAttributeValueOrChildValuesByName( const CString& name ) const
{GUCEF_TRACE;

    TStringVector results = GetChildrenValuesByName( name );
    CString attValue = GetAttributeValue( name );
    if ( !attValue.IsNULLOrEmpty() )
    {
        results.push_back( attValue );
    }
    return results;
}

/*-------------------------------------------------------------------------*/

CDataNode::TStringVector
CDataNode::GetChildrenValuesByName( const CString& name ) const
{GUCEF_TRACE;

    TStringVector results;
    
    TConstDataNodeSet childNodes = FindChildrenOfType( name, false );
    TConstDataNodeSet::iterator i = childNodes.begin();
    while ( i != childNodes.end() )
    {
        const CString& childValue = (*i)->GetValue();
        if ( !childValue.IsNULLOrEmpty() )
        {
            results.push_back( childValue );
        }
        ++i;
    }

    return results;
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::Delete( void )
{GUCEF_TRACE;

    Detach();
    delete this;             
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CDataNode::GetAssociatedData( void ) const
{GUCEF_TRACE;

    return m_associatedData;
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::SetAssociatedData( CICloneable* associatedData )
{GUCEF_TRACE;
    
    delete m_associatedData;
    m_associatedData = GUCEF_NULL;
    
    if ( GUCEF_NULL != associatedData )    
        m_associatedData = associatedData->Clone();
}

/*-------------------------------------------------------------------------*/
        
bool 
CDataNode::DelChild( const CString& name )
{
        GUCEF_BEGIN;
        CDataNode* n = FindChild( name );
        if ( n )
        {       
                delete n;
                GUCEF_END;
                return true;
        }
        GUCEF_END;
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CDataNode::HasChildren( void ) const
{GUCEF_TRACE;

    return !m_children.empty();
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::GetParent( void ) const
{GUCEF_TRACE;

    return _pparent;
}

/*-------------------------------------------------------------------------*/
        
CDataNode* 
CDataNode::GetFirstChild( void ) const
{GUCEF_TRACE;

    if ( !m_children.empty() )
        return m_children.front();
    return nullptr;
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::GetLastChild( void ) const
{GUCEF_TRACE;

    if ( !m_children.empty() )
        return m_children.back();
    return nullptr;
}

/*-------------------------------------------------------------------------*/
        
CDataNode* 
CDataNode::GetNext( void ) const
{GUCEF_TRACE;

    return _pnext;
}

/*-------------------------------------------------------------------------*/
        
CDataNode* 
CDataNode::GetPrevious( void ) const
{GUCEF_TRACE;

    return _pprev;
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator 
CDataNode::Begin( void )
{GUCEF_TRACE;

    iterator i( this );
    return i;
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator 
CDataNode::End( void )
{GUCEF_TRACE;

    iterator i( this );
    i._pos = NULL;
    i._atend = true;      
    return i;
}

/*-------------------------------------------------------------------------*/
        
CDataNode::const_iterator 
CDataNode::Begin( void ) const
{
        GUCEF_BEGIN;
        const_iterator i( this );
        GUCEF_END;
        return i;
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator 
CDataNode::End( void ) const
{
        GUCEF_BEGIN;
        const_iterator i( this );
        i._pos = NULL;
        i._atend = true;
        i._atstart = false;
        GUCEF_END;       
        return i;
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator 
CDataNode::ConstBegin( void ) const
{                
        GUCEF_BEGIN;
        const_iterator i( this );
        GUCEF_END;        
        return i;
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator 
CDataNode::ConstEnd( void ) const
{
        GUCEF_BEGIN;
        const_iterator i( this );
        i._pos = NULL;
        i._atend = true;
        i._atstart = false;
        GUCEF_END;        
        return i;
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator::const_iterator( const CDataNode* parent )
        : _pfchild( NULL )  ,
          _pos( NULL )      ,
          _atend( true )    ,
          _atstart( false )
{
        GUCEF_BEGIN;
        if ( parent )
        {
                if ( !parent->m_children.empty() )
                {
                        _pfchild = parent->m_children.front();
                        _pos = _pfchild;
                        _atstart = true;
                        _atend = false;
                }
        }
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator::const_iterator( const const_iterator& src )
        : _pfchild( src._pfchild ) ,
          _pos( src._pos )         ,
          _atend( src._atend )     ,
          _atstart( src._atstart )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator::~const_iterator()
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator& 
CDataNode::const_iterator::operator=( const const_iterator& src )
{
        GUCEF_BEGIN;
        if ( this != &src )
        {
                _pos = src._pos;
                _pfchild = src._pfchild;
                _atend = src._atend;
                _atstart = src._atstart;
                GUCEF_END;
                return *this;
        }
        GUCEF_END;
        return *this;
}

/*-------------------------------------------------------------------------*/

const CDataNode* 
CDataNode::const_iterator::operator*( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _pos;
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator& 
CDataNode::const_iterator::operator++( void )
{
        GUCEF_BEGIN;
        if ( _pos )
        {
                if ( _pos->_pnext )
                {
                        _pos = _pos->_pnext;
                        GUCEF_END;
                        return *this;
                }
                _pos = NULL;
                _atend = true;
                _atstart = false;
                GUCEF_END;
                return *this;
        }
        GUCEF_END;
        return *this;                
}

/*-------------------------------------------------------------------------*/

CDataNode::const_iterator& 
CDataNode::const_iterator::operator--( void )
{
        GUCEF_BEGIN;
        if ( _pfchild )
        {
                if ( _pos == _pfchild )
                {
                        _pos = NULL;
                        _atstart = true;
                        _atend = false;
                        GUCEF_END;
                        return *this;
                }
                _pos = _pos->_pprev;
                GUCEF_END;
                return *this;              
        }
        GUCEF_END;                
        return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::const_iterator::operator!=( const const_iterator& src ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return !( ( _pos == src._pos ) && 
                  ( _atend == src._atend ) &&
                  ( _atstart == src._atstart ) ); 
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::const_iterator::operator==( const const_iterator& src ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return ( ( _pos == src._pos ) && 
                 ( _atend == src._atend ) &&
                 ( _atstart == src._atstart ) ); 
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator::iterator( CDataNode* parent )
        : _pfchild( NULL )  ,
          _pos( NULL )      ,
          _atend( true )    ,
          _atstart( false )
{
        GUCEF_BEGIN;
        if ( parent )
        {
                if ( !parent->m_children.empty() )
                {
                        _pos = parent->m_children.front();
                        _pfchild = _pos;
                        _atstart = true;
                        _atend = false;                        
                }
        }
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator::iterator( const iterator& src )
        : _pfchild( src._pfchild ) ,
          _pos( src._pos )         ,
          _atend( src._atend )     ,
          _atstart( src._atstart )
{
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator::~iterator()
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator& 
CDataNode::iterator::operator=( const iterator& src )
{
        GUCEF_BEGIN;
        if ( this != &src )
        {
                _pos = src._pos;
                _pfchild = src._pfchild;
                _atend = src._atend;
                _atstart = src._atstart;
                GUCEF_END;
                return *this;
        }
        GUCEF_END;
        return *this;
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::iterator::operator*( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _pos;
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator& 
CDataNode::iterator::operator++( void )
{
        GUCEF_BEGIN;
        if ( _pos )
        {
                if ( _pos->_pnext )
                {
                        _pos = _pos->_pnext;
                        GUCEF_END;
                        return *this;
                }
                _pos = NULL;
                _atend = true;
                _atstart = false;
                GUCEF_END;
                return *this;
        }
        GUCEF_END;
        return *this;                
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator& 
CDataNode::iterator::operator--( void )
{
        GUCEF_BEGIN;
        if ( _pfchild )
        {
                if ( _pos == _pfchild )
                {
                        _pos = NULL;
                        _atstart = true;
                        _atend = false;
                        GUCEF_END;
                        return *this;
                }
                _pos = _pos->_pprev;
                GUCEF_END;
                return *this;              
        }
        GUCEF_END;                
        return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::iterator::operator!=( const iterator& src ) const
{
        GUCEF_BEGIN;
        GUCEF_END; 
        return !( ( _pos == src._pos ) && 
                  ( _atend == src._atend ) &&
                  ( _atstart == src._atstart ) ); 
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::iterator::operator==( const iterator& src ) const
{
        GUCEF_BEGIN;
        GUCEF_END; 
        return ( ( _pos == src._pos ) && 
                 ( _atend == src._atend ) &&
                 ( _atstart == src._atstart ) ); 
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/