/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#include "gucefCOM_CCodecBasedHTTPServerResource.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CCodecBasedHTTPServerResource::CCodecBasedHTTPServerResource( void )
    : CDefaultHTTPServerResource()
    , m_serializeRepToCodecMap()
    , m_deserializeRepToCodecMap()
    , m_allowCreate( false )
    , m_allowDeserialize( false )
    , m_allowSerialize( false )
{GUCEF_TRACE;

    InitCodecLinks();
}

/*-------------------------------------------------------------------------*/
    
CCodecBasedHTTPServerResource::CCodecBasedHTTPServerResource( const CCodecBasedHTTPServerResource& src )
    : CDefaultHTTPServerResource( src )
    , m_serializeRepToCodecMap( src.m_serializeRepToCodecMap )
    , m_deserializeRepToCodecMap( src.m_deserializeRepToCodecMap )
    , m_allowCreate( src.m_allowCreate )
    , m_allowDeserialize( src.m_allowDeserialize )
    , m_allowSerialize( src.m_allowSerialize )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CCodecBasedHTTPServerResource::~CCodecBasedHTTPServerResource()
{GUCEF_TRACE;

    m_serializeRepToCodecMap.clear();
    m_deserializeRepToCodecMap.clear();
}

/*-------------------------------------------------------------------------*/
    
CCodecBasedHTTPServerResource&
CCodecBasedHTTPServerResource::operator=( const CCodecBasedHTTPServerResource& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_serializeRepToCodecMap = src.m_serializeRepToCodecMap;
        m_deserializeRepToCodecMap = src.m_deserializeRepToCodecMap;
        m_allowCreate = src.m_allowCreate;
        m_allowDeserialize = src.m_allowDeserialize;
        m_allowSerialize = src.m_allowSerialize;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CCodecBasedHTTPServerResource::InitCodecLinks( void )
{GUCEF_TRACE;

    static const CORE::CString MimeTypeAny = "*/*";
    static const CORE::CString MimeTypeJson = "application/json";
    static const CORE::CString MimeTypeJsonOld = "text/x-json";
    static const CORE::CString MimeTypeXml = "application/xml";
    static const CORE::CString MimeTypeXmlAlt = "text/xml";
    static const CORE::CString MimeTypeIni = "application/ini";
    static const CORE::CString MimeTypeIniProper = "zz-application/zz-winassoc-ini";
    static const CORE::CString MimeTypeText = "text/plain";
    static const CORE::CString MimeTypeOctet = "application/octet-stream";

    CORE::CDStoreCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry();

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
    if ( codecRegistry.TryLookup( "INI", codec, false ) )
    {
        m_serializeRepToCodecMap[ MimeTypeAny ] = codec;
        m_serializeRepToCodecMap[ MimeTypeIni ] = codec;
        m_deserializeRepToCodecMap[ MimeTypeIni ] = codec;
        m_serializeRepToCodecMap[ MimeTypeIniProper ] = codec;
        m_deserializeRepToCodecMap[ MimeTypeIniProper ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCodecBasedHTTPServerResource:InitCodecLinks: Hooked up INI codec to MIME types" );
    }
    if ( codecRegistry.TryLookup( "XML", codec, false ) )
    {
        m_serializeRepToCodecMap[ MimeTypeXml ] = codec;
        m_deserializeRepToCodecMap[ MimeTypeXml ] = codec;
        m_serializeRepToCodecMap[ MimeTypeXmlAlt ] = codec;                
        m_deserializeRepToCodecMap[ MimeTypeXmlAlt ] = codec;        

        // As a first pass, service other types with XML as well if reasonable it fits
        m_serializeRepToCodecMap[ MimeTypeAny ] = codec;
        m_serializeRepToCodecMap[ MimeTypeText ] = codec;
        m_serializeRepToCodecMap[ MimeTypeOctet ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCodecBasedHTTPServerResource:InitCodecLinks: Hooked up XML codec to MIME types" );
    }
    if ( codecRegistry.TryLookup( "JSON", codec, false ) )
    {
        m_serializeRepToCodecMap[ MimeTypeJson ] = codec;
        m_deserializeRepToCodecMap[ MimeTypeJson ] = codec;
        m_serializeRepToCodecMap[ MimeTypeJsonOld ] = codec;
        m_deserializeRepToCodecMap[ MimeTypeJsonOld ] = codec;
        
        // As the more Web standard format, overrule with JSON if available
        m_serializeRepToCodecMap[ MimeTypeAny ] = codec;
        m_serializeRepToCodecMap[ MimeTypeText ] = codec;
        m_serializeRepToCodecMap[ MimeTypeOctet ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCodecBasedHTTPServerResource:InitCodecLinks: Hooked up JSON codec to MIME types" );
    }

    // Now based on what we can actually do codec wise set our preferences
    // As an example, if we can serialize in XML and JSON we would prefer JSON.
    // This all just depends on which codecs are available
    // Note that the order of insertion matters because it denotes preference

    if ( m_serializeRepToCodecMap.find( MimeTypeJson ) != m_serializeRepToCodecMap.end() )
    {
        m_serializationReps.push_back( MimeTypeJson );
        m_serializationReps.push_back( MimeTypeJsonOld );
    }
    if ( m_serializeRepToCodecMap.find( MimeTypeXml ) != m_serializeRepToCodecMap.end() )
    {
        m_serializationReps.push_back( MimeTypeXml );
        m_serializationReps.push_back( MimeTypeXmlAlt );
    }
    if ( m_serializeRepToCodecMap.find( MimeTypeIni ) != m_serializeRepToCodecMap.end() )
    {
        m_serializationReps.push_back( MimeTypeIniProper );
        m_serializationReps.push_back( MimeTypeIni );
    }
    if ( !m_serializeRepToCodecMap.empty() )
    {
        m_serializationReps.push_back( MimeTypeText );
        m_serializationReps.push_back( MimeTypeOctet );
        m_serializationReps.push_back( MimeTypeAny );
    }

    // Now same thing but for deserialization
    
    if ( m_deserializeRepToCodecMap.find( MimeTypeJson ) != m_deserializeRepToCodecMap.end() )
    {
        m_deserializationReps.push_back( MimeTypeJson );
        m_deserializationReps.push_back( MimeTypeJsonOld );
    }
    if ( m_deserializeRepToCodecMap.find( MimeTypeXml ) != m_deserializeRepToCodecMap.end() )
    {
        m_deserializationReps.push_back( MimeTypeXml );
        m_deserializationReps.push_back( MimeTypeXmlAlt );
    }
    if ( m_deserializeRepToCodecMap.find( MimeTypeIni ) != m_deserializeRepToCodecMap.end() )
    {
        m_deserializationReps.push_back( MimeTypeIniProper );
        m_deserializationReps.push_back( MimeTypeIni );
    }

    return !m_serializeRepToCodecMap.empty() || !m_deserializeRepToCodecMap.empty();
}

/*-------------------------------------------------------------------------*/

CCodecBasedHTTPServerResource::TCreateState 
CCodecBasedHTTPServerResource::CreateResource( const CString& transactionID                  ,
                                               const CORE::CDynamicBuffer& inputBuffer       ,
                                               const CString& representation                 ,
                                               THTTPServerResourcePtr& resourceOutput        ,
                                               TStringVector& supportedRepresentationsOutput )
{GUCEF_TRACE;

    if ( !m_allowCreate )
        return TCreateState::CREATESTATE_FAILED;

    TStringToCodecMap::iterator i = m_deserializeRepToCodecMap.find( representation );
    if ( i != m_deserializeRepToCodecMap.end() )
    {        
        auto& codec = (*i).second;
        
        CORE::CDataNode rootNode;
        CORE::CDynamicBufferAccess bufferIoAccess( inputBuffer );
        if ( codec->BuildDataTree( &rootNode, &bufferIoAccess ) )
        {
            return CreateResource( transactionID                  , 
                                   rootNode                       , 
                                   representation                 , 
                                   resourceOutput                 , 
                                   supportedRepresentationsOutput );
        }
    }
    return TCreateState::CREATESTATE_DESERIALIZATIONFAILED;
}

/*-------------------------------------------------------------------------*/

CCodecBasedHTTPServerResource::TCreateState 
CCodecBasedHTTPServerResource::CreateResource( const CString& transactionID                  ,
                                               const CORE::CDataNode& input                  ,
                                               const CString& representation                 ,
                                               THTTPServerResourcePtr& resourceOutput        ,
                                               TStringVector& supportedRepresentationsOutput )
{GUCEF_TRACE;

    // By default we don't allow or implement more complex interaction patterns
    // such as dynamic resources or collections
    return TCreateState::CREATESTATE_FAILED;
}

/*-------------------------------------------------------------------------*/

bool 
CCodecBasedHTTPServerResource::Serialize( CORE::CDynamicBuffer& outputBuffer ,
                                          const CString& representation      )
{GUCEF_TRACE;

    if ( !m_allowSerialize )
        return false;
    
    TStringToCodecMap::iterator i = m_serializeRepToCodecMap.find( representation );
    if ( i != m_serializeRepToCodecMap.end() )
    {        
        CORE::CDataNode rootNode;
        if ( Serialize( rootNode, representation ) )
        {        
            auto& codec = (*i).second;
                
            CORE::CDynamicBufferAccess bufferIoAccess( &outputBuffer, false );
            if ( codec->StoreDataTree( &rootNode, &bufferIoAccess ) )
            {
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CCodecBasedHTTPServerResource::Serialize( CORE::CDataNode& output             ,
                                          const CORE::CString& representation )
{
    return false;
}

/*-------------------------------------------------------------------------*/

CCodecBasedHTTPServerResource::TDeserializeState 
CCodecBasedHTTPServerResource::Deserialize( const CORE::CDynamicBuffer& inputBuffer ,
                                            const CString& representation           )
{GUCEF_TRACE;

    if ( !m_allowDeserialize )
        return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;

    TStringToCodecMap::iterator i = m_deserializeRepToCodecMap.find( representation );
    if ( i != m_deserializeRepToCodecMap.end() )
    {        
        auto& codec = (*i).second;
        
        CORE::CDataNode rootNode;
        CORE::CDynamicBufferAccess bufferIoAccess( inputBuffer );
        if ( codec->BuildDataTree( &rootNode, &bufferIoAccess ) )
        {
            return Deserialize(  rootNode, representation );
        }
    }
    return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
}

/*-------------------------------------------------------------------------*/

CCodecBasedHTTPServerResource::TDeserializeState 
CCodecBasedHTTPServerResource::Deserialize( const CORE::CDataNode& input  ,
                                            const CString& representation )
{
    return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
