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

#include "CStringList.h"
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
          _pprev( NULL )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDataNode::CDataNode( const CString& name )
        : _name( name )    ,
          _pparent( NULL ) ,
          _pfchild( NULL ) ,
          _plchild( NULL ) ,
          _pnext( NULL )   ,
          _pprev( NULL )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDataNode::CDataNode( const CDataNode& src )
        : _name( src._name )                ,
          _atts( src._atts.GetArraySize() ) ,
          _pparent( src._pparent )          ,
          _pnext( src._pnext )              ,
          _pprev( src._pprev )              ,
          _pfchild( NULL )                  ,
          _plchild( NULL )
{
        GUCEF_BEGIN;        
        TNodeAtt* srcatt;
        TNodeAtt* att;
        UInt32 count = src._atts.GetCount();
        for ( UInt32 i=0; i<count; ++i )        
        {
                srcatt = static_cast<TNodeAtt*>( src._atts[ i ] );
                att = new TNodeAtt;
                *att = *srcatt;
                _atts.SetEntry( i, att );
                
                CHECKMEM( att, sizeof(TNodeAtt) );       
        }
        
        const CDataNode* n = src._pfchild;
        while ( n )
        {
                AddChild( *n );
                n = n->_pnext;
        }
        GUCEF_END;               
}

/*-------------------------------------------------------------------------*/
        
CDataNode::~CDataNode()
{        
        GUCEF_BEGIN;
        Detach();
        
        UInt32 count = _atts.GetLast()+1;
        for ( UInt32 i=0; i<count; ++i )
        {
                CHECKMEM( _atts[ i ], sizeof(TNodeAtt) );
                delete static_cast<TNodeAtt*>( _atts[ i ] );
        }                
        DelSubTree();
        GUCEF_END;        
}

/*-------------------------------------------------------------------------*/

void       
CDataNode::Detach( void )
{
        GUCEF_BEGIN;
        if ( _pparent )
        {
                _pparent->DetachChild( this );                        
        }
        _pnext = NULL;
        _pprev = NULL;
        _pparent = NULL;
        GUCEF_END;        
}

/*-------------------------------------------------------------------------*/

void
CDataNode::DetachChild( CDataNode* child )
{       
        GUCEF_BEGIN;
        if ( !child )
        {
                GUCEF_END;
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
                GUCEF_END;
                return;                
        }
        /*
         *      Check if the given node is the first node in our list
         */             
        if ( child == _pfchild )
        {
                _pfchild = _pfchild->_pnext;
                _pfchild->_pprev = NULL;
                GUCEF_END;
                return;                        
        }
        /*
         *      Check if the given node is the last node in our list
         */          
        if ( child == _plchild ) 
        {
                _plchild = _plchild->_pprev;
                _plchild->_pnext = NULL;
                GUCEF_END;
                return;
        }
        
        /*
         *      
         */
        child->_pnext->_pprev = child->_pprev;
        child->_pprev->_pnext = child->_pnext;
        GUCEF_END;                              
}

/*-------------------------------------------------------------------------*/

CDataNode& 
CDataNode::operator=( const CDataNode& src )
{
        GUCEF_BEGIN;
        if ( this != &src )
        {
                // detach from our current tree
                Detach();
                DelSubTree();
                
                ClearAttributes();
                
                // copy new attribute values
                TNodeAtt* srcatt;
                TNodeAtt* att;
                UInt32 count, i;
                _name = src._name;
                _atts.SetArraySize( src._atts.GetArraySize() );
                count = src._atts.GetCount();
                for ( i=0; i<count; ++i )        
                {
                        srcatt = static_cast<TNodeAtt*>( src._atts[ i ] );
                        att = new TNodeAtt;
                        *att = *srcatt;
                        _atts.SetEntry( i, att );
                        
                        CHECKMEM( att, sizeof(TNodeAtt) );
                }
        }               
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::Copy( const CDataNode& root )
{
    *this = root;
    
    CopySubTree( root );
}

/*-------------------------------------------------------------------------*/
        
void 
CDataNode::SetName( const CString& name )
{
        GUCEF_BEGIN;
        _name = name;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/
        
const CString&
CDataNode::GetName( void ) const
{TRACE;

        return _name;
}

/*-------------------------------------------------------------------------*/
        
bool 
CDataNode::IsAttribute( const CString& name ) const
{
        GUCEF_BEGIN;
        TNodeAtt* att;
        UInt32 count = _atts.GetLast()+1;
        for ( UInt32 i=0; i<count; ++i )
        {
                att = static_cast<TNodeAtt*>( _atts[ i ] );
                CHECKMEM( att, sizeof(TNodeAtt) );
                if ( att->name == name )
                {
                        GUCEF_END;
                        return true;
                }        
        }
        GUCEF_END;
        return false;
}

/*-------------------------------------------------------------------------*/
        
const CDataNode::TNodeAtt* 
CDataNode::operator[]( UInt32 index ) const
{        
        GUCEF_BEGIN;
        if ( index <= (UInt32)_atts.GetLast()+1 )
        {                
                GUCEF_END_RET( const CDataNode::TNodeAtt*, static_cast<TNodeAtt*>( _atts[ index ] ) );
        }
        GUCEF_END;
        return NULL;
}

/*-------------------------------------------------------------------------*/
        
CDataNode::TNodeAtt* 
CDataNode::operator[]( UInt32 index )
{
        GUCEF_BEGIN;
        if ( index <= (UInt32)_atts.GetLast()+1 )
        {
                GUCEF_END_RET( TNodeAtt*, static_cast<TNodeAtt*>( _atts[ index ] ) );
        }
        GUCEF_END;     
        return NULL;
}

/*-------------------------------------------------------------------------*/
        
const CDataNode::TNodeAtt* 
CDataNode::operator[]( const CString& name ) const
{
        GUCEF_BEGIN;
        TNodeAtt* att;
        UInt32 count = _atts.GetLast()+1;
        for ( UInt32 i=0; i<count; ++i )
        {
                att = static_cast<TNodeAtt*>( _atts[ i ] );
                if ( att->name == name )
                {
                        GUCEF_END;
                        return att;
                }        
        }       
        GUCEF_END;
        return NULL;
}

/*-------------------------------------------------------------------------*/
        
CDataNode::TNodeAtt* 
CDataNode::operator[]( const CString& name )
{
        GUCEF_BEGIN;
        TNodeAtt* att;
        UInt32 count = _atts.GetLast()+1;
        for ( UInt32 i=0; i<count; ++i )
        {
                att = static_cast<TNodeAtt*>( _atts[ i ] );
                if ( att->name == name )
                {
                        GUCEF_END;
                        return att;
                }        
        }       
        GUCEF_END;
        return NULL;
}

/*-------------------------------------------------------------------------*/

void
CDataNode::ClearAttributes( void )
{TRACE;

    // erase all current attributes
    UInt32 count = _atts.GetCount();                                        
    for ( UInt32 i=0; i<count; ++i )
    {
        delete static_cast<TNodeAtt*>( _atts[ i ] );
    }
    _atts.Clear();
}

/*-------------------------------------------------------------------------*/
        
const CDataNode::TNodeAtt* 
CDataNode::GetAttribute( UInt32 index ) const
{
        GUCEF_BEGIN;
        if ( index <= (UInt32)_atts.GetLast()+1 )
        {
                GUCEF_END_RET( TNodeAtt*, static_cast<TNodeAtt*>( _atts[ index ] ) );
        }
        GUCEF_END;
        return NULL;        
}

/*-------------------------------------------------------------------------*/
        
CDataNode::TNodeAtt* 
CDataNode::GetAttribute( UInt32 index )
{
        GUCEF_BEGIN;
        if ( index <= (UInt32)_atts.GetLast()+1 )
        {
                GUCEF_END_RET( TNodeAtt*, static_cast<TNodeAtt*>( _atts[ index ] ) );
        }
        GUCEF_END;
        return NULL;
}

/*-------------------------------------------------------------------------*/
        
const CDataNode::TNodeAtt* 
CDataNode::GetAttribute( const CString& name ) const
{
        GUCEF_BEGIN;
        TNodeAtt* att;
        UInt32 count = _atts.GetLast()+1;
        for ( UInt32 i=0; i<count; ++i )
        {
                att = static_cast<TNodeAtt*>( _atts[ i ] );
                if ( att->name == name )
                {
                        GUCEF_END;
                        return att;
                }        
        }        
        GUCEF_END;
        return NULL;
}

/*-------------------------------------------------------------------------*/
        
CDataNode::TNodeAtt* 
CDataNode::GetAttribute( const CString& name )
{TRACE;
        TNodeAtt* att;
        UInt32 count = _atts.GetLast()+1;
        for ( UInt32 i=0; i<count; ++i )
        {
                att = static_cast<TNodeAtt*>( _atts[ i ] );
                if ( att->name == name )
                {
                        return att;
                }        
        }      
        return NULL;
}

/*-------------------------------------------------------------------------*/
   
bool
CDataNode::AddAttribute( const TNodeAtt& att )
{
        GUCEF_BEGIN;
        if ( att.name.Length() && att.value.Length() )
        {
                TNodeAtt* newatt = new TNodeAtt;
                *newatt = att;
                _atts.AppendEntry( newatt );
                GUCEF_END;        
                return true;
        }  
        GUCEF_END;
        return false;                
}

/*-------------------------------------------------------------------------*/   
        
bool 
CDataNode::AddAttribute( const CString& name  ,
                         const CString& value )
{
        GUCEF_BEGIN;
        if ( name.Length() && value.Length() )
        {
                TNodeAtt* att = new TNodeAtt;
                CHECKMEM( att, sizeof(TNodeAtt) );
                att->name = name;
                att->value = value;
                _atts.AppendEntry( att );
                GUCEF_END;
                return true;
        }   
        GUCEF_END;
        return false;                        
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::SetAttribute( const CString& name  ,
                         const CString& value )
{TRACE;
        if ( name.Length() && value.Length() )
        {
                TNodeAtt* att = GetAttribute( name );
                if ( att )
                {                
                        att->value = value;
                        return true;
                }
                return AddAttribute( name, value );
        }
        return false;                        
}

/*-------------------------------------------------------------------------*/                         
        
UInt32 
CDataNode::GetAttCount( void ) const
{TRACE;
        return _atts.GetCount();
}

/*-------------------------------------------------------------------------*/

void 
CDataNode::CopySubTree( const CDataNode& root )
{TRACE;

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
        GUCEF_BEGIN;
        TNodeAtt* att;
        UInt32 count = _atts.GetLast()+1;
        for ( UInt32 i=0; i<count; ++i )
        {
                att = static_cast<TNodeAtt*>( _atts[ i ] );
                CHECKMEM( att, sizeof(TNodeAtt) );
                if ( att->name == name )
                {
                        delete att;
                        _atts.RemoveEntry( i );
                        GUCEF_END;
                        return;
                }        
        }
        GUCEF_END;        
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

bool
CDataNode::Compare( const CDataNode& other         ,
                    const CStringList& excludeList ) const
{TRACE;

    // identical nodes should offcourse have identical names
    if ( _name == other._name )
    {
        const TNodeAtt* att;
        const TNodeAtt* att2;
        const UInt32 count = other._atts.GetCount();
        
        // if one has a bigger list then the other then they cannot be the same
        if ( _atts.GetCount() == count )
        {
            // same number of entries in the lists, we will have to compare each item
            for ( UInt32 i=0; i<count; ++i )
            {
                    att = static_cast<const TNodeAtt*>( other._atts[ i ] );
                    if ( att )
                    {
                        if ( -1 == excludeList.Find( att->name ) )
                        {
                            att2 = GetAttribute( att->name );
                            if ( att2 )
                            {
                                if ( att->value != att2->value )
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
CDataNode::CompareChild( const CDataNode& other         ,
                         const CStringList& excludeList ) const
{TRACE;

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
{TRACE;
    
    CStringList excludeList;
    return Compare( other, excludeList );
}

/*-------------------------------------------------------------------------*/

bool 
CDataNode::operator!=( const CDataNode& other ) const
{TRACE;
    return !( (*this) == other );
}

/*-------------------------------------------------------------------------*/

const CDataNode* 
CDataNode::FindSibling( const CString& name ) const
{TRACE;
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
{TRACE;
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
CDataNode::Find( const CString& name )
{TRACE;    

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
{TRACE;

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
{TRACE;
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