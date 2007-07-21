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

#ifndef GUCEF_CORE_CDATANODE_H
#define GUCEF_CORE_CDATANODE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"      /* framework dynamic array implementation */
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"          /* framework ref-counted string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

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
 *      Simple datanode class.
 *      Can be used to build a tree of data with each tree node 
 *      having an optional number of attributes.
 *      Note that CDataNode objects always remain owner of all their
 *      children so do not delete any node or node attribute directly yourself.
 */
class GUCEFCORE_EXPORT_CPP CDataNode
{
        public:        

        /**
         *      Node attribute storage structure
         *      Stores the attribute name and value
         */
        struct SNodeAttribute
        {
                CString name;   /**< name of the attribute */
                CString value;  /**< value of the attribute */
        };
        typedef struct SNodeAttribute TNodeAtt;
        
        CDataNode( void );        
        
        /**
         *      Constructs a datanode with the given name
         *      The constructed node has no attributes and is not part of a tree
         *
         *      @param name The name of the node
         */
        CDataNode( const CString& name );
        
        /**
         *      Constructs a datanode tree that is an excact duplicate 
         *      of the given tree with src as the tree root.
         *
         *      @param src The tree that is to serve as an template
         */        
        CDataNode( const CDataNode& src );
        
        /**
         *      Cleans up all attributes and children
         */
        ~CDataNode();
        
        /**
         *      Turns the given node and it's sub-tree into an replica
         *      of the given node and it's sub-tree.
         *
         *      @param src tree you wish to replicate
         *      @return the modified node
         */
        CDataNode& operator=( const CDataNode& src );
        
        bool operator==( const CDataNode& other ) const;
        
        bool operator!=( const CDataNode& other ) const;
        
        bool Compare( const CDataNode& other         ,
                      const CStringList& excludeList ) const;
                      
        /**
         *  Checks the immediate children using Compare()
         *  If no "identical" child node can be found NULL
         *  will be returned.
         */
        const CDataNode* CompareChild( const CDataNode& other         ,
                                       const CStringList& excludeList ) const;                      
        
        /**
         *      Sets the new node name
         *
         *      @param name the new node name
         */
        void SetName( const CString& name );
        
        /**
         *      Gets the current node name
         *
         *      @return the current node name
         */
        const CString& GetName( void ) const;                
        
        bool IsAttribute( const CString& name ) const;
        
        const TNodeAtt* operator[]( UInt32 index ) const;
        
        TNodeAtt* operator[]( UInt32 index );
        
        const TNodeAtt* operator[]( const CString& name ) const;
        
        TNodeAtt* operator[]( const CString& name );
        
        const TNodeAtt* GetAttribute( UInt32 index ) const;
        
        TNodeAtt* GetAttribute( UInt32 index );
        
        const TNodeAtt* GetAttribute( const CString& name ) const;
        
        TNodeAtt* GetAttribute( const CString& name );              
                           
        bool SetAttribute( const CString& name  ,
                           const CString& value );                           
                           
        void DelAttribute( const CString& name );                           

        void ClearAttributes( void );
        
        void Clear( void );
        
        UInt32 GetAttCount( void ) const;
        
        CDataNode* FindRoot( void ) const;
        
        CDataNode* FindChild( const CString& name ) const;
        
        CDataNode* FindChild( const CString& name        ,
                              const CString& attribName  ,
                              const CString& attribValue );
        
        CDataNode* FindSibling( const CString& name );
        
        const CDataNode* FindSibling( const CString& name ) const;
        
        /**
         *      Searches the entire tree starting at the current level
         *      for an node with the given name
         */
        const CDataNode* Find( const CString& name ) const;

        /**
         *      Searches the entire tree starting at the current level
         *      for an node with the given name
         */
        CDataNode* Find( const CString& name );
        
        /**
         *      If the given node structure does not exists starting
         *      as a child node of the current node then the nodes
         *      will be created. This ensures that you have the
         *      requested tree structure. A pointer to the last 
         *      mentioned structure node is returned.
         *
         *      @param sequence the node structure sequence you wish to create
         *      @param seperator the char you used to seperate node names
         *      @return last node in the structure
         */
        CDataNode* Structure( const CString& sequence ,
                              char seperator          );        

        /**
         *      Builds a structure if where each node is named
         *      according to 'nodeName' and where each of those nodes is given
         *      an attribute with 'attribName' using a section from 'attribSequence'
         *      as the value for the attribute with 'seperator' used to separate
         *      the segments
         *
         *      @param nodeName name used for all the nodes in the structure
         *      @param attribName name of the attribute you want to use to store the attrib value
         *      @param attribSequence the attribute values you wish to set
         *      @param seperator the char you used to separate attribute values
         *      @return last node in the structure
         */
        CDataNode* Structure( const CString& nodeName       ,
                              const CString& attribName     ,
                              const CString& attribSequence ,
                              const char seperator          );
                                      
        /**
         *      Searches for the structure provided in the query.
         *      If the structure is found a pointer to the last node
         *      in the structure will be provided.
         *
         *      @param sequence the node structure sequence you wish to search for
         *      @param seperator the char you used to seperate node names
         *      @param fromcurrent wheter the requested structure must start at the current node.
         *      @return last node in the search structure
         */        
        CDataNode* Search( const CString& query ,
                           char seperator       ,
                           bool fromcurrent     ) const;
                           
        /**
         *      'Walks' the data node tree to find the sequence
         *      of nodes given. The algorith will attempt to get as 'deep'
         *      as possible using the specified node sequence.
         *      If no nodes where found that have an name as specified in the sequence
         *      at the correct location then that part of the sequence is considdered to 
         *      be a leftover. An pointer to the 'deepest' node is returned.
         *
         *      @param sequence the sequence of node names you are looking for, each name seperated with a seperator char
         *      @param seperator char that is used to seperate the different node names
         *      @param sleftover outputvar: the section of the sequence that could not be resolved.
         *      @return pointer to the deepest node found.
         */                           
        CDataNode* WalkTree( const CString& sequence ,
                             char seperator          ,
                             CString& sleftover      ) const;                           
        
        /**
         *      Returns wheter the current node has any children
         *
         *      @return whether the current node has any children
         */
        bool HasChildren( void ) const;
        
        /**
         *      Adds an child node to the given node using the
         *      given newnode as a template. 
         *
         *      @param newnode node that is to be used as an template
         *      @return pointer to the new child.
         */
        CDataNode* AddChild( const CDataNode& newnode );
        
        bool DelChild( const CString& name );
        
        void Copy( const CDataNode& root );

        void CopySubTree( const CDataNode& root );
        
        /**
         *      Deletes the entire sub-tree from the current node.
         *      So in essense the child nodes and their children ect.
         *      are deleted.      
         */
        void DelSubTree( void );
        
        /**
         *      Attempts to delete this node from the tree.         
         */
        void Delete( void );
        
        CDataNode* GetParent( void ) const;
        
        CDataNode* GetFirstChild( void ) const;
        
        CDataNode* GetLastChild( void ) const;
        
        CDataNode* GetNext( void ) const;
        
        CDataNode* GetPrevious( void ) const;
        
        class GUCEFCORE_EXPORT_CPP iterator
        {
                public:

                iterator( CDataNode* parent );

                iterator( const iterator& src );

                ~iterator();

                iterator& operator=( const iterator& src );

                CDataNode* operator*( void ) const; 

                iterator& operator++( void );

                iterator& operator--( void );

                bool operator!=( const iterator& src ) const;
                
                bool operator==( const iterator& src ) const;               

                private:
                friend class CDataNode;
                
                const CDataNode* _pfchild;   /**< first child node */
                CDataNode* _pos;             /**< current child node */
                bool _atend;                 /**< has the iterator reached the right end pos */ 
                bool _atstart;               /**< has the iterator reached the left end pos */ 
        };

        class GUCEFCORE_EXPORT_CPP const_iterator
        {
                public:

                const_iterator( const CDataNode* parent );

                const_iterator( const const_iterator& src );

                ~const_iterator();

                const_iterator& operator=( const const_iterator& src );

                const CDataNode* operator*( void ) const;

                const_iterator& operator++( void );

                const_iterator& operator--( void );

                bool operator!=( const const_iterator& src ) const;
                
                bool operator==( const const_iterator& src ) const;

                private:
                friend class CDataNode;
                
                const CDataNode* _pfchild;   /**< first child node */
                const CDataNode* _pos;       /**< current child node */
                bool _atend;                 /**< has the iterator reached the right end pos */ 
                bool _atstart;               /**< has the iterator reached the left end pos */ 
        };

        iterator Begin( void );

        iterator End( void );
        
        const_iterator Begin( void ) const;

        const_iterator End( void ) const;

        const_iterator ConstBegin( void ) const;

        const_iterator ConstEnd( void ) const;        
        
        private:
        friend class iterator;
        friend class const_iterator;
        
        bool AddAttribute( const TNodeAtt& att );
        
        bool AddAttribute( const CString& name  ,
                           const CString& value );        
        
        CDataNode* WalkTreeImp( CString& sleftover ,
                                char seperator     ) const;         
        
        void Detach( void );      /**< detaches the node from the tree */
        void DetachChild( CDataNode* child ); /**< detaches the given child node */
                                         
        CString _name;         /**< name of the node */                
        CDynamicArray _atts;   /**< list of node attributes */
        CDataNode* _pparent;   /**< parent node */
        CDataNode* _pfchild;   /**< first child node */
        CDataNode* _plchild;   /**< last child node */
        CDataNode* _pnext;     /**< next sibling node */
        CDataNode* _pprev;     /**< previous sibling node */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDATANODE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-12-2005 :
        - Fixed a bug in the tree walking algorithm where not all tree branches
          would be explored for the best match.
- 18-05-2005 :
        - Fixed a bug in Search(): The fromcurrent parameter had the wrong effect
- 26-04-2005 :
        - Fixed an iterator bug for both the const and non-const iterator.
          The  start and end flags where not correctly set causing scenarios
          to be possible where (i != end) == true even though there where no 
          node children.
        - Fixed a bug in CopySubTree(), only the first tree branch was copied.  
- 23-04-2005 :
        - Fixed a bug in AddChild() that would prevent you from adding more
          then 1 child to a node. This would also cause an access violation
          when cleaning up the tree.
- 05-04-2005 :
        - Added some commentary
        - Added Structure()
        - Added the fromcurrent parameter to Search()
- 04-04-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
