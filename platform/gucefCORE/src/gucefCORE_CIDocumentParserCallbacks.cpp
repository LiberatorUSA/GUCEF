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

#include "gucefCORE_CIDocumentParserCallbacks.h"    

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

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnDocumentBeginHandler( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );
        cppObj->OnDocumentBegin();
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnDocumentEndHandler( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );
        cppObj->OnDocumentEnd();
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnNodeBeginHandler( void* privdata             ,
                    const TVariantData* nodeId ,
                    int nodeType               ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != nodeId )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );
        
        CVariant cppNodeId;
        cppNodeId.LinkTo( nodeId );
        
        cppObj->OnNodeBegin( cppNodeId, nodeType );
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnNodeEndHandler( void* privdata             ,
                  const TVariantData* nodeId ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != nodeId )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );

        CVariant cppNodeId;
        cppNodeId.LinkTo( nodeId );

        cppObj->OnNodeEnd( cppNodeId );
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnNodeAttHandler( void* privdata               ,
                  const TVariantData* nodeId   ,
                  const TVariantData* attName  ,
                  const TVariantData* attValue ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != nodeId && GUCEF_NULL != attName && GUCEF_NULL != attValue )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );

        CVariant cppNodeId;
        cppNodeId.LinkTo( nodeId );
        CVariant cppAttName;
        cppAttName.LinkTo( attName );
        CVariant cppAttValue;
        cppAttValue.LinkTo( attValue );

        cppObj->OnNodeAttribute( cppNodeId, cppAttName, cppAttValue );
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnNodeValueHandler( void* privdata               ,
                    const TVariantData* nodeId   ,
                    const TVariantData* nodeValue ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != nodeId && GUCEF_NULL != nodeValue )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );

        CVariant cppNodeId;
        cppNodeId.LinkTo( nodeId );
        CVariant cppNodeValue;
        cppNodeValue.LinkTo( nodeValue );

        cppObj->OnNodeValue( cppNodeId, cppNodeValue );
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnNodeChildrenBeginHandler( void* privdata             ,
                            const TVariantData* nodeId ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != nodeId )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );

        CVariant cppNodeId;
        cppNodeId.LinkTo( nodeId );

        cppObj->OnNodeChildrenBegin( cppNodeId );
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnNodeChildrenEndHandler( void* privdata             ,
                          const TVariantData* nodeId ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != nodeId )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );

        CVariant cppNodeId;
        cppNodeId.LinkTo( nodeId );

        cppObj->OnNodeChildrenEnd( cppNodeId );
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnParseErrorHandler( void* privdata          ,
                     Int32 errorcode         ,
                     const char* description ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata )
    {
        CIDocumentParserCallbacks* cppObj = static_cast< CIDocumentParserCallbacks* >( privdata );
        cppObj->OnParseError( errorcode, description );
    }
}

/*-------------------------------------------------------------------------*/

CIDocumentParserCallbacks::CIDocumentParserCallbacks( void )
    : m_cStyleAccess()
{GUCEF_TRACE;

    InitCStyleAccess();
}

/*-------------------------------------------------------------------------*/

CIDocumentParserCallbacks::CIDocumentParserCallbacks( const CIDocumentParserCallbacks& src )
    : m_cStyleAccess()
{GUCEF_TRACE;

    InitCStyleAccess();       
}

/*-------------------------------------------------------------------------*/

CIDocumentParserCallbacks::~CIDocumentParserCallbacks()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDocumentParserCallbacks& 
CIDocumentParserCallbacks::operator=( const CIDocumentParserCallbacks& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

void
CIDocumentParserCallbacks::InitCStyleAccess( void )
{GUCEF_TRACE;

    memset( &m_cStyleAccess, 0, sizeof( m_cStyleAccess ) );
    m_cStyleAccess.OnDocumentBegin = OnDocumentBeginHandler;
    m_cStyleAccess.OnDocumentEnd = OnDocumentEndHandler;
    m_cStyleAccess.OnNodeBegin = OnNodeBeginHandler;
    m_cStyleAccess.OnNodeEnd = OnNodeEndHandler;
    m_cStyleAccess.OnNodeAttribute = OnNodeAttHandler;
    m_cStyleAccess.OnNodeValue = OnNodeValueHandler;
    m_cStyleAccess.OnNodeChildrenBegin = OnNodeChildrenBeginHandler;
    m_cStyleAccess.OnNodeChildrenEnd = OnNodeChildrenEndHandler;
    m_cStyleAccess.OnParserError = OnParseErrorHandler;
    m_cStyleAccess.privateData = this;
}

/*-------------------------------------------------------------------------*/

TDocumentParserCallbacks& 
CIDocumentParserCallbacks::GetCStyleAccess( void )
{GUCEF_TRACE;

    return m_cStyleAccess;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
