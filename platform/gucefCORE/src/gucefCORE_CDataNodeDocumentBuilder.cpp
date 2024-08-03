/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *
 *  Copyright (C) 1998 - 2024.  Dinand Vanvelzen
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

#include "gucefCORE_CDataNodeDocumentBuilder.h"   

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDataNodeDocumentBuilder::CDataNodeDocumentBuilder( void )
    : CIDocumentParserCallbacks()
    , document()
    , m_nodeCursor( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeDocumentBuilder::CDataNodeDocumentBuilder( const CDataNodeDocumentBuilder& src )
    : CIDocumentParserCallbacks()
    , document( src.document )
    , m_nodeCursor( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeDocumentBuilder::~CDataNodeDocumentBuilder()
{GUCEF_TRACE;

    m_nodeCursor = GUCEF_NULL;
    document.Clear();
}

/*-------------------------------------------------------------------------*/

CDataNodeDocumentBuilder&
CDataNodeDocumentBuilder::operator=( const CDataNodeDocumentBuilder& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        document = src.document;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnDocumentBegin( void )
{GUCEF_TRACE;

    m_nodeCursor = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnDocumentEnd( void )
{GUCEF_TRACE;

    m_nodeCursor = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnNodeBegin( const CVariant& nodeId, int nodeType )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_nodeCursor )
    {
        m_nodeCursor = m_nodeCursor->AddChild( nodeId, nodeType );
        return;
    }

    // First node
    m_nodeCursor = &document;
    m_nodeCursor->SetName( nodeId );
    m_nodeCursor->SetNodeType( nodeType );
}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnNodeEnd( const CVariant& nodeId )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_nodeCursor )
    {
        m_nodeCursor = m_nodeCursor->GetParent();
    }
}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnNodeAttribute( const CVariant& nodeId         , 
                                           const CVariant& attributeId    , 
                                           const CVariant& attributeValue )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_nodeCursor )
    {
        if ( attributeValue != CVariant::Empty )
        {
            if ( attributeId != CVariant::Empty )
            {
                m_nodeCursor->SetAttribute( attributeId, attributeValue );
            }
            else        
            {
                if ( GUCEF_DATATYPE_ARRAY == m_nodeCursor->GetNodeType() )
                    m_nodeCursor->AddValueAsChild( attributeValue );
                else
                    m_nodeCursor->SetValue( attributeValue );
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnNodeValue( const CVariant& nodeId    , 
                                       const CVariant& nodeValue )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_nodeCursor )
    {
        if ( nodeValue != CVariant::Empty )
        {
            if ( GUCEF_DATATYPE_ARRAY == m_nodeCursor->GetNodeType() )
                m_nodeCursor->AddValueAsChild( nodeValue );
            else
                m_nodeCursor->SetValue( nodeValue );
        }
    }
}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnNodeChildrenBegin( const CVariant& nodeId )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnNodeChildrenEnd( const CVariant& nodeId )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void 
CDataNodeDocumentBuilder::OnParseError( Int32 errorCode            , 
                                        const CString& description )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
