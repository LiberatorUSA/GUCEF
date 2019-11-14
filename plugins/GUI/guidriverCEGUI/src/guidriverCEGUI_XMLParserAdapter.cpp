/*
 *  guidriverCEGUI: glue module for the CEGUI GUI backend
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
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

#include "CEGUI/ResourceProvider.h"
#include "CEGUI/System.h"
#include "CEGUI/XMLHandler.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_MACROS_H
#include "guidriverCEGUI_macros.h" 
#define GUCEF_GUIDRIVERCEGUI_MACROS_H
#endif /* GUCEF_GUIDRIVERCEGUI_MACROS_H ? */

#include "guidriverCEGUI_XMLParserAdapter.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERCEGUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

XMLParserAdapter::XMLParserAdapter( void )
    : ::CEGUI::XMLParser()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

XMLParserAdapter::~XMLParserAdapter()
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

void
XMLParserAdapter::parseDataNodes( ::CEGUI::XMLHandler& handler     , 
                                  const CORE::CDataNode& dataNodes )
{GUCEF_TRACE;

    // Convert attributes of current node
    ::CEGUI::XMLAttributes attrs;
    CORE::CDataNode::TAttributeMap::const_iterator a = dataNodes.AttributeBegin();
    while ( a != dataNodes.AttributeEnd() )
    {
        attrs.add( (*a).first, (*a).second.value );
        ++a;
    }

    handler.elementStart( dataNodes.GetName(), attrs );

    const CORE::CString& value = dataNodes.GetValue();
    if ( !value.IsNULLOrEmpty() )
    {
        handler.text( value );
    }

    // Recursively handle the children
    CORE::CDataNode::const_iterator i = dataNodes.ConstBegin();
    while ( i != dataNodes.ConstEnd() )
    {
        parseDataNodes( handler, *(*i) );        
        ++i;
    }

    handler.elementEnd( dataNodes.GetName() );
}

/*-------------------------------------------------------------------------*/

void 
XMLParserAdapter::parseXML( ::CEGUI::XMLHandler& handler            ,
                            const ::CEGUI::RawDataContainer& source , 
                            const ::CEGUI::String& schemaName       )
{GUCEF_TRACE;

    CORE::CDynamicBufferAccess bufferAccess;
    bufferAccess.LinkTo( source.getDataPtr(), source.getSize() );
    
    CORE::CDataNode dataNodes;
    if ( !m_codec->BuildDataTree( &dataNodes, &bufferAccess ) )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "XMLParserAdapter:parseXML: Failed to build data tree from source buffer" );
        return;
    }
    
    parseDataNodes( handler, dataNodes );
}

/*-------------------------------------------------------------------------*/

bool 
XMLParserAdapter::initialiseImpl( void )
{GUCEF_TRACE;

    return CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( "xml", m_codec, false );
}

/*-------------------------------------------------------------------------*/

void 
XMLParserAdapter::cleanupImpl( void )
{GUCEF_TRACE;

    m_codec = nullptr;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERCEGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

