/*
 *  gucefWEB: GUCEF module providing Web application functionality 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_WEB_CHTTPENCODINGTYPES_H
#include "gucefWEB_CHttpEncodingTypes.h"
#define GUCEF_WEB_CHTTPENCODINGTYPES_H
#endif /* GUCEF_WEB_CHTTPENCODINGTYPES_H ? */

#include "gucefWEB_CHttpCodecLinks.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CHttpCodecLinks::CHttpCodecLinks( void )
    : m_serializeRepToCodecMap()
    , m_deserializeRepToCodecMap()
    , m_deserializationReps()
    , m_serializationReps()
    , m_dataLock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CHttpCodecLinks::CHttpCodecLinks( const CHttpCodecLinks& src )
    : m_serializeRepToCodecMap( src.m_serializeRepToCodecMap )
    , m_deserializeRepToCodecMap( src.m_deserializeRepToCodecMap )
    , m_deserializationReps( src.m_deserializationReps )
    , m_serializationReps( src.m_serializationReps )
    , m_dataLock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CHttpCodecLinks::~CHttpCodecLinks()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CHttpCodecLinks& 
CHttpCodecLinks::operator=( const CHttpCodecLinks& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        MT::CScopeMutex srcLock( src.m_dataLock );
        MT::CScopeMutex lock( m_dataLock );

        m_serializeRepToCodecMap = src.m_serializeRepToCodecMap;
        m_deserializeRepToCodecMap = src.m_deserializeRepToCodecMap;
        m_deserializationReps = src.m_deserializationReps;
        m_serializationReps = src.m_serializationReps;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CHttpCodecLinks::TMimeTypeCodecPtr 
CHttpCodecLinks::GetSerializationCodec( const CString& mimeType ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_dataLock );
    TStringToCodecMap::const_iterator i = m_serializeRepToCodecMap.find( mimeType  );
    if ( i != m_serializeRepToCodecMap.end() )
    {
        return (*i).second;
    }
    return CHttpCodecLinks::TMimeTypeCodecPtr();
}

/*-------------------------------------------------------------------------*/

CHttpCodecLinks::TMimeTypeCodecPtr 
CHttpCodecLinks::GetDeserializationCodec( const CString& mimeType ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_dataLock );
    TStringToCodecMap::const_iterator i = m_deserializeRepToCodecMap.find( mimeType  );
    if ( i != m_deserializeRepToCodecMap.end() )
    {
        return (*i).second;
    }
    return CHttpCodecLinks::TMimeTypeCodecPtr();
}

/*-------------------------------------------------------------------------*/

void
CHttpCodecLinks::GetSupportedSerializationMimeTypes( CORE::CString::StringVector& reps ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_dataLock );
    reps = m_serializationReps;
}

/*-------------------------------------------------------------------------*/

void 
CHttpCodecLinks::GetSupportedDeserializationMimeTypes( CORE::CString::StringVector& reps ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_dataLock );
    reps = m_deserializationReps;
}

/*-------------------------------------------------------------------------*/

bool 
CHttpCodecLinks::InitMimeCodecLinks( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_dataLock );

    CORE::CDStoreCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry();

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
    if ( codecRegistry.TryLookup( "INI", codec, false ) )
    {
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeAny ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeIni ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeIni ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeIniProper ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeIniProper ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCodecBasedHTTPServerResource:InitCodecLinks: Hooked up INI codec to MIME types" );
    }
    if ( codecRegistry.TryLookup( "XML", codec, false ) )
    {
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeXml ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeXml ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeXmlAlt ] = codec;                
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeXmlAlt ] = codec;        

        // As a first pass, service other types with XML as well if reasonable it fits
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeAny ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeText ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeOctet ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCodecBasedHTTPServerResource:InitCodecLinks: Hooked up XML codec to MIME types" );
    }
    if ( codecRegistry.TryLookup( "JSON", codec, false ) )
    {
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeJson ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeJson ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeJsonOld ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeJsonOld ] = codec;
        
        // As the more Web standard format, overrule with JSON if available
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeAny ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeText ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeOctet ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCodecBasedHTTPServerResource:InitCodecLinks: Hooked up JSON codec to MIME types" );
    }

    // Now based on what we can actually do codec wise set our preferences
    // As an example, if we can serialize in XML and JSON we would prefer JSON.
    // This all just depends on which codecs are available
    // Note that the order of insertion matters because it denotes preference

    if ( m_serializeRepToCodecMap.find( CHttpMimeTypes::MimeTypeJson ) != m_serializeRepToCodecMap.end() )
    {
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeJson );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeJsonOld );
    }
    if ( m_serializeRepToCodecMap.find( CHttpMimeTypes::MimeTypeXml ) != m_serializeRepToCodecMap.end() )
    {
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeXml );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeXmlAlt );
    }
    if ( m_serializeRepToCodecMap.find( CHttpMimeTypes::MimeTypeIni ) != m_serializeRepToCodecMap.end() )
    {
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeIniProper );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeIni );
    }
    if ( !m_serializeRepToCodecMap.empty() )
    {
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeText );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeOctet );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeAny );
    }

    // Now same thing but for deserialization
    
    if ( m_deserializeRepToCodecMap.find( CHttpMimeTypes::MimeTypeJson ) != m_deserializeRepToCodecMap.end() )
    {
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeJson );
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeJsonOld );
    }
    if ( m_deserializeRepToCodecMap.find( CHttpMimeTypes::MimeTypeXml ) != m_deserializeRepToCodecMap.end() )
    {
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeXml );
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeXmlAlt );
    }
    if ( m_deserializeRepToCodecMap.find( CHttpMimeTypes::MimeTypeIni ) != m_deserializeRepToCodecMap.end() )
    {
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeIniProper );
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeIni );
    }

    return !m_serializeRepToCodecMap.empty() || !m_deserializeRepToCodecMap.empty();
}

/*-------------------------------------------------------------------------*/

bool 
CHttpCodecLinks::InitEncodingCodecLinks( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_dataLock );

    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();

    CORE::CCodecRegistry::TICodecPtr codec;
    if ( codecRegistry.TryLookup( CHttpEncodingTypes::EncodingTypeGZip, codec, false )    ||
         codecRegistry.TryLookup( CHttpEncodingTypes::EncodingTypeGZipAlt, codec, false )  )
    {
        m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeGZip ] = codec;
        m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeGZipAlt ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHttpCodecLinks:InitCodecLinks: Hooked up GZIP encoding codec" );
    }
    if ( codecRegistry.TryLookup( CHttpEncodingTypes::EncodingTypeDeflate, codec, false ) )
    {
        m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeDeflate ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHttpCodecLinks:InitCodecLinks: Hooked up Deflate encoding codec" );
    }
    if ( codecRegistry.TryLookup( CHttpEncodingTypes::EncodingTypeCompress, codec, false ) )
    {
        m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeCompress ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHttpCodecLinks:InitCodecLinks: Hooked up Compress encoding codec" );
    }
    if ( codecRegistry.TryLookup( CHttpEncodingTypes::EncodingTypeBrotli, codec, false ) )
    {
        m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeBrotli ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHttpCodecLinks:InitCodecLinks: Hooked up Brotli encoding codec" );
    }
}

/*-------------------------------------------------------------------------*/

bool 
CHttpCodecLinks::Lock( void ) const
{GUCEF_TRACE;

    return m_dataLock.Lock();
}

/*-------------------------------------------------------------------------*/

bool 
CHttpCodecLinks::Unlock( void ) const
{GUCEF_TRACE;

    return m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

const CString& 
CHttpCodecLinks::GetClassTypeName( void ) const
{
    static const CORE::CString classTypeName = "GUCEF::COM::CHttpCodecLinks";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
