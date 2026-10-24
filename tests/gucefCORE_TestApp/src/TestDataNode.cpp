/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#include <vector>

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestDataNode.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

/*-------------------------------------------------------------------------//
//                                                                         //
//      TESTS                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformDataNodeTests( void )
{GUCEF_TRACE;

    GUCEF_TESTFW_SUITE_SCOPE( "gucefCORE.DataNode" );

    GUCEF_TESTFW_TESTCASE( "DataNode: A mutable iterator walks every child once and then equals End()" )
    try
    {
        CORE::CDataNode root( "Root" );
        root.AddChild( "A" );
        root.AddChild( "B" );
        root.AddChild( "C" );

        CORE::CString seen;
        UInt32 steps = 0;
        CORE::CDataNode::iterator i = root.Begin();
        while ( i != root.End() && steps < 10 )
        {
            seen += (*i)->GetName();
            ++i;
            ++steps;
        }
        ASSERT_TRUE( "ABC" == seen && 3 == steps );
        ASSERT_TRUE( i == root.End() );

        // the const iterator agrees
        const CORE::CDataNode& constRoot = root;
        steps = 0;
        CORE::CDataNode::const_iterator c = constRoot.Begin();
        while ( c != constRoot.End() && steps < 10 )
        {
            ++c;
            ++steps;
        }
        ASSERT_TRUE( 3 == steps );

        // a node without children: Begin() is End() at once
        CORE::CDataNode empty( "Empty" );
        ASSERT_TRUE( empty.Begin() == empty.End() );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "DataNode: Children can be changed and deleted while walking with a mutable iterator" )
    try
    {
        CORE::CDataNode root( "Root" );
        for ( UInt32 n=0; n<5; ++n )
            root.AddChild( n % 2 == 0 ? "Keep" : "Drop" )->SetAttribute( "index", n );

        // step past a node before deleting it; a deleted node leaves its parent
        CORE::CDataNode::iterator i = root.Begin();
        while ( i != root.End() )
        {
            CORE::CDataNode* child = (*i);
            ++i;
            if ( child->GetName() == "Drop" )
                GUCEF_DELETE child;
            else
                child->SetAttribute( "seen", "yes" );
        }
        ASSERT_TRUE( 3 == root.GetNrOfDirectChildNodes() );
        UInt32 kept = 0;
        for ( CORE::CDataNode::iterator k = root.Begin(); k != root.End(); ++k )
        {
            ASSERT_TRUE( "Keep" == (*k)->GetName() && "yes" == (*k)->GetAttributeValue( "seen" ).AsString() );
            ++kept;
        }
        ASSERT_TRUE( 3 == kept );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "DataNode: A copy of a child is a node of its own, not in the tree of the original, and outlives that tree" )
    try
    {
        CORE::CDataNode* tree = GUCEF_NEW CORE::CDataNode( "root" );
        CORE::CDataNode* list = tree->AddChild( "list" );
        list->AddChild( "first" )->SetAttribute( "a", "1" );
        CORE::CDataNode* second = list->AddChild( "second" );
        second->SetAttribute( "b", "2" );
        second->AddChild( "inner" );
        list->AddChild( "third" );

        CORE::CDataNode copy( *second );
        ASSERT_TRUE( GUCEF_NULL == copy.GetParent() && "second" == copy.GetName() && "2" == copy.GetAttributeValue( "b" ).AsString() );
        ASSERT_TRUE( GUCEF_NULL != copy.FindChild( "inner" ) && copy.FindChild( "inner" )->GetParent() == &copy );
        ASSERT_TRUE( second == list->FindChild( "second" ) && 4 == list->GetNrOfChildNodes() );   // the original stays in its tree

        // the copies kept by value, as a list of settings keeps them, live on after the tree is gone
        std::vector< CORE::CDataNode > kept;
        kept.push_back( *second );
        kept.push_back( *list->FindChild( "third" ) );
        GUCEF_DELETE tree;
        ASSERT_TRUE( 2 == kept.size() && "second" == kept[ 0 ].GetName() && GUCEF_NULL == kept[ 1 ].GetParent() );

        #ifdef GUCEF_RVALUE_REFERENCES_SUPPORTED
        // a moved node takes its children along: they have it as their parent
        CORE::CDataNode* source = GUCEF_NEW CORE::CDataNode( "source" );
        source->AddChild( "child" );
        CORE::CDataNode moved( GUCEF_MOVE( *source ) );
        GUCEF_DELETE source;
        ASSERT_TRUE( GUCEF_NULL != moved.FindChild( "child" ) && &moved == moved.FindChild( "child" )->GetParent() );
        #endif
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/
