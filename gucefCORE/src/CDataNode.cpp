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

CDataNode::CDataNode( void )
        : _pparent( NULL ) ,
          _pfchild( NULL ) ,
          _plchild( NULL ) ,
          _pnext( NULL )   ,
          _pprev( NULL )   ,
          _name()          ,
          m_value()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CDataNode::CDataNode( const CString& name )
        : _name( name )    ,
          m_value()        ,
          _pparent( NULL ) ,
          _pfchild( NULL ) ,
          _plchild( NULL ) ,
          _pnext( NULL )   ,
          _pprev( NULL )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CDataNode::CDataNode( const CDataNode& src )
        : _name( src._name )       ,
          m_value( src.m_value )   ,
          _atts( src._atts )       ,
          _pparent( src._pparent ) ,
          _pnext( src._pnext )     ,
          _pprev( src._pprev )     ,
          _pfchild( NULL )         ,
          _plchild( NULL )
{GUCEF_TRACE;
    
    const CDataNode* n = src._pfchild;
    while ( 0 != n )
    {
        AddChild( *n );
        n = n->_pnext;
    }             
}

/*-------------------------------------------------------------------------*/
        
CDataNode::~CDataNode()
{GUCEF_TRACE;
        
    Detach();             
    DelSubTree();
}

/*-------------------------------------------------------------------------*/

void
CDataNode::Clear( void )
{GUCEF_TRACE;

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

        if ( !child )
        {
                return;
        }
        
        /*
         *      Check if the given node is the last of the nodes
         */
        if ( ( _pfchild == _plchild ) &&
             ( _plchild == child ) )
        {
                _pfchild = NULL;
                _plchild = NULL;
                return;                
        }
        /*
         *      Check if the given node is the first node in our list
         */             
        if ( child == _pfchild )
        {
                _pfchild = _pfchild->_pnext;
                _pfchild->_pprev = NULL;
                return;                        
        }
        /*
         *      Check if the given node is the last node in our list
         */          
        if ( child == _plchild ) 
        {
                _plchild = _plchild->_pprev;
                _plchild->_pnext = NULL;
                return;
        }
        
        /*
         *      
         */
        child->_pnext->_pprev = child->_pprev;
        child->_pprev->_pnext = child->_pnext;                           
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
        
        _name = src._name;
        m_value = src.m_value;
        _atts = src._atts;

    }               
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::Copy( const CDataNode& root )
{GUCEF_TRACE;

    *this = root;
    CopySubTree( root );
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
        return (*i).second;
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
                         const CString& value )
{GUCEF_TRACE;

    TAttributeMap::iterator i = _atts.find( name );
    if ( i == _atts.end() )
    {
        _atts.insert( TKeyValuePair( name, value ) );
        return true;
    }
    return false;                       
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::SetAttribute( const CString& name  ,
                         const CString& value )
{GUCEF_TRACE;

    _atts[ name ] = value;
    return true;                      
}

/*-------------------------------------------------------------------------*/                         
        
UInt32 
CDataNode::GetAttCount( void ) const
{GUCEF_TRACE;

    return _atts.size();
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::CopySubTree( const CDataNode& root )
{GUCEF_TRACE;

        CDataNode* n = root._pfchild;
        while ( n )
        {
                CHECKMEM( n, sizeof(CDataNode) );
                AddChild( *n );
                n = n->_pnext;
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
{        
        GUCEF_BEGIN;
        if ( _pfchild )
        {
                if ( _pfchild->_name == name )
                {
                        GUCEF_END;
                        return _pfchild;
                }
                
                CDataNode* nn = _pfchild->_pnext;
                while ( ( nn != _pfchild ) &&
                        ( nn != NULL ) )
                {
                        if ( nn->_name == name )
                        {
                                GUCEF_END;
                                return nn;
                        }
                        nn = nn->_pnext;
                }
        }
        GUCEF_END;
        return NULL;
}

/*-------------------------------------------------------------------------*/

CDataNode::TConstDataNodeSet
CDataNode::FindChildrenOfType( const CString& name  ,
                               const bool recursive ) const
{GUCEF_TRACE;

    TConstDataNodeSet children;
    const CDataNode* child = _pfchild;
    
    while ( 0 != child )
    {
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
        child = child->_pnext;
    }
    return children;
}

/*-------------------------------------------------------------------------*/

CDataNode::TDataNodeSet
CDataNode::FindChildrenOfType( const CString& name  ,
                               const bool recursive )
{GUCEF_TRACE;

    TDataNodeSet children;
    CDataNode* child = _pfchild;
    
    while ( 0 != child )
    {
        if ( child->_name == name )
        {
            children.insert( child );            
        }
        if ( recursive )
        {
            TDataNodeSet subSet( child->FindChildrenOfType( name, recursive ) );
            TDataNodeSet::const_iterator i = subSet.begin();
            while ( i != subSet.end() )
            {
                children.insert( (*i) );
                ++i;
            }
        }                
        child = child->_pnext;
        if ( child == _pfchild ) break;
    }
    return children;
}

/*-------------------------------------------------------------------------*/

CDataNode*
CDataNode::FindChild( const CString& name        ,
                      const CString& attribName  ,
                      const CString& attribValue )
{GUCEF_TRACE;

    if ( _pfchild != NULL )
    {
        CDataNode* node = _pfchild;
        do
        {
            // Check if the name matches
            if ( node->_name == name )
            {
                // See if the node has the attribute we want
                TKeyValuePair* att = node->GetAttribute( attribName );
                if ( att != NULL )
                {
                    // See if the node attribute has the value we want
                    if ( att->second == attribValue )
                    {
                        return node;
                    }
                }
            }
            
            // Move to the next node
            node = node->_pnext;
            
        } while ( node != _pfchild );
    }
    return NULL;    
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
        const UInt32 count = other._atts.size();
        
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
                                if ( att->second != att2->second )
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
    if ( n ) 
    {
        return n;
    }                

    n = _pfchild;
    CDataNode* result = NULL;
    while ( n != NULL )
    {
        result = n->Find( name );
        if ( NULL != result )
        {
            return result;
        }
        n = n->_pnext;
    }
    return NULL;       
}

/*-------------------------------------------------------------------------*/

const CDataNode* 
CDataNode::Find( const CString& name ) const
{GUCEF_TRACE;

    const CDataNode* n = FindSibling( name );
    if ( n ) 
    {
        return n;
    }                

    n = _pfchild;
    const CDataNode* result = NULL;
    while ( n != NULL )
    {
        result = n->Find( name );
        if ( NULL != result )
        {
            return result;
        }
        n = n->_pnext;
    }
    return NULL;      
}

/*-------------------------------------------------------------------------*/

UInt32
CDataNode::GetNrOfChildNodes( void ) const
{GUCEF_TRACE;

    UInt32 count = 0;
    const CDataNode* childNode = _pfchild;
    while ( childNode != NULL )
    {
        count += childNode->GetNrOfChildNodes()+1;
        childNode = childNode->_pnext;
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
CDataNode::Search( const CString& query ,
                   char seperator       ,
                   bool fromcurrent     ) const
{
        GUCEF_BEGIN;
        
        if ( fromcurrent )
        {
                CString thisname( query.SubstrToChar( seperator, true ) );
                if ( thisname == _name )
                {
                        CString remnant( query.CutChars( thisname.Length()+1, true ) );
                        
                        CString leftover;
                        CDataNode* n = WalkTree( remnant   ,
                                                 seperator ,
                                                 leftover  );
                        if ( !leftover.Length() )
                        {       
                                GUCEF_END;
                                return n;
                        }                                 
                }
                GUCEF_END;
                return NULL;                                            
        }
        else
        {
                CString leftover;
                CDataNode* n = WalkTree( query     ,
                                         seperator ,
                                         leftover  );
                if ( !leftover.Length() )
                {       
                        GUCEF_END;
                        return n;
                }
                GUCEF_END;
                return NULL;
        }                
}

/*-------------------------------------------------------------------------*/

CDataNode*
CDataNode::WalkTree( const CString& sequence ,
                     char seperator          ,
                     CString& sleftover      ) const
{
        GUCEF_BEGIN;
        sleftover = sequence;
        
        GUCEF_END_RET( CDataNode*, WalkTreeImp( sleftover ,
                                                seperator ) );
}                     

/*-------------------------------------------------------------------------*/

CDataNode*
CDataNode::WalkTreeImp( CString& sleftover ,
                        char seperator     ) const
{GUCEF_TRACE;
        if ( _pfchild )
        {
                CDataNode* sn = (CDataNode*) this;
                CDataNode* retval = (CDataNode*) this;        
                CString leftover( sleftover );
                CString deepest( sleftover );
                CString searchseg( sleftover.SubstrToChar( seperator, true ) );        
                CString left( sleftover.CutChars( searchseg.Length()+1, true ) );
                                                  
                CDataNode* n = _pfchild;                                                                                    
                while ( n )
                {
                        if ( n->_name == searchseg )
                        {
                                deepest = left;
                                leftover = left;
                                
                                if ( 0 == left.Length() )
                                {
                                        // nothing left to search for so
                                        // no point in continuing
                                        sleftover = deepest;
                                        return n;
                                }
                        }        
                        
                        // search the tree for our leftover
                        sn = n->WalkTreeImp( leftover  ,
                                             seperator );
                        
                        // if what we found is better then what we found so far then
                        // substitute the current deepest node for the new node.
                        if ( deepest.Length() > leftover.Length() )                  
                        {
                                deepest = leftover;
                                retval = sn;
                                CHECKMEM( retval, sizeof(CDataNode) );
                        }                       
                        n = n->_pnext;
                }
                sleftover = deepest;
                return retval;
        }
        return (CDataNode*)this;                                                                          
}                     

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::Structure( const CString& sequence ,
                      char seperator          )
{
        // walk the tree
        GUCEF_BEGIN;
        CString buildseg;
        CDataNode* sn( WalkTree( sequence  ,
                                 seperator ,
                                 buildseg  ) );
        CHECKMEM( sn, sizeof(CDataNode) );
                 
        // do we need to add nodes ?
        if ( buildseg.Length() )
        {
                CDataNode child;
                CString name;
                while ( buildseg.Length() )
                {
                        name = buildseg.SubstrToChar( seperator, true );
                        child.SetName( name );                        
                        buildseg = buildseg.CutChars( name.Length()+1, true );                        
                        sn = sn->AddChild( child );
                        
                        CHECKMEM( sn, sizeof(CDataNode) );
                }
        }
        GUCEF_END;
        return sn;        
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
{
        GUCEF_BEGIN;
        if ( _pfchild )
        {
                CDataNode* n = _pfchild;
                CDataNode* nn;
                while ( n )
                {
                        nn = n->_pnext;
                        delete n;
                        n = nn;        
                }
                _pfchild = NULL;
        }
        GUCEF_END;        
}        

/*-------------------------------------------------------------------------*/
      
CDataNode* 
CDataNode::AddChild( const CDataNode& newnode )
{
        GUCEF_BEGIN;
        CDataNode* n = new CDataNode( newnode );
        
        /*
         *      Attach the new node to this tree
         */
        if ( _plchild )
        {          
                n->_pprev = _plchild;
                _plchild = n;
                n->_pprev->_pnext = n;
        }
        else
        {               
                n->_pprev = NULL;                
                _pfchild = n;
                _plchild = n;
        }
        n->_pnext = NULL;
        n->_pparent = this;
        GUCEF_END;
        return n;                                  
}

/*-------------------------------------------------------------------------*/

CDataNode*
CDataNode::AddChild( const CString& nodeName )
{
    CDataNode newNode( nodeName );
    return AddChild( newNode );
}

/*-------------------------------------------------------------------------*/

CString
CDataNode::GetChildValueByName( const CString& name ) const
{
    CDataNode* childNode = FindChild( name );
    if ( NULL != childNode )
    {
        return childNode->GetValue();
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

CString
CDataNode::GetAttributeValueOrChildValueByName( const CString& name ) const
{
    CString value = GetAttributeValue( name );
    if ( 0 == value.Length() )
    {
        value = GetChildValueByName( name );
    }
    return value;
}

/*-------------------------------------------------------------------------*/

CDataNode::TStringVector
CDataNode::GetChildrenValuesByName( const CString& name ) const
{
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
{
        GUCEF_BEGIN;
        Detach();
        delete this;             
        GUCEF_END;
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
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _pfchild != NULL;
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::GetParent( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _pparent;
}

/*-------------------------------------------------------------------------*/
        
CDataNode* 
CDataNode::GetFirstChild( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _pfchild;
}

/*-------------------------------------------------------------------------*/

CDataNode* 
CDataNode::GetLastChild( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _plchild;
}

/*-------------------------------------------------------------------------*/
        
CDataNode* 
CDataNode::GetNext( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _pnext;
}

/*-------------------------------------------------------------------------*/
        
CDataNode* 
CDataNode::GetPrevious( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _pprev;
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator 
CDataNode::Begin( void )
{
        GUCEF_BEGIN;        
        iterator i( this );
        GUCEF_END;
        return i;
}

/*-------------------------------------------------------------------------*/

CDataNode::iterator 
CDataNode::End( void )
{
        GUCEF_BEGIN;
        iterator i( this );
        i._pos = NULL;
        i._atend = true;
        GUCEF_END;        
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
                if ( parent->_pfchild )
                {
                        _pfchild = parent->_pfchild;
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
                if ( parent->_pfchild )
                {
                        _pfchild = parent->_pfchild;
                        _pos = parent->_pfchild;
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