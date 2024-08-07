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

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

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
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CHttpCodecLinks::CHttpCodecLinks( const CHttpCodecLinks& src )
    : m_serializeRepToCodecMap( src.m_serializeRepToCodecMap )
    , m_deserializeRepToCodecMap( src.m_deserializeRepToCodecMap )
    , m_deserializationReps( src.m_deserializationReps )
    , m_serializationReps( src.m_serializationReps )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CHttpCodecLinks::~CHttpCodecLinks()
{GUCEF_TRACE;

    RemoveEncodingCodecLinks();
    RemoveMimeCodecLinks();    
}

/*-------------------------------------------------------------------------*/
    
CHttpCodecLinks& 
CHttpCodecLinks::operator=( const CHttpCodecLinks& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        MT::CObjectScopeLock srcLock( src );
        MT::CObjectScopeLock lock( this );

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

    MT::CObjectScopeLock lock( this );
    TStringToMimeTypeCodecMap::const_iterator i = m_serializeRepToCodecMap.find( mimeType  );
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

    MT::CObjectScopeLock lock( this );
    TStringToMimeTypeCodecMap::const_iterator i = m_deserializeRepToCodecMap.find( mimeType  );
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

    MT::CObjectScopeLock lock( this );
    reps = m_serializationReps;
}

/*-------------------------------------------------------------------------*/

void 
CHttpCodecLinks::GetSupportedDeserializationMimeTypes( CORE::CString::StringVector& reps ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    reps = m_deserializationReps;
}

/*-------------------------------------------------------------------------*/

bool 
CHttpCodecLinks::InitMimeCodecLinks( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    CORE::CDStoreCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry();

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
    if ( codecRegistry.TryLookup( "INI", codec, false ) )
    {
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeAny ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeIni ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeIni ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeIniProper ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeIniProper ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitMimeCodecLinks: Hooked up INI codec to MIME types" );
    }
    if ( codecRegistry.TryLookup( "DNC", codec, false ) )
    {
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeGupDataNodeContainer ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeGupDataNodeContainer ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitMimeCodecLinks: Hooked up GUP's DNC codec to MIME types" );
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

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitMimeCodecLinks: Hooked up XML codec to MIME types" );
    }
    if ( codecRegistry.TryLookup( "YAML", codec, false ) || codecRegistry.TryLookup( "YML", codec, false ) )
    {
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlProposed ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlProposed ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlApp ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlApp ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlAppExt ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlAppExt ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlText ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlText ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlTextExt ] = codec;
        m_deserializeRepToCodecMap[ CHttpMimeTypes::MimeTypeYamlTextExt ] = codec;
        
        // As the newer more capable Web standard format, overrule XML with YAML if available
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeAny ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeText ] = codec;
        m_serializeRepToCodecMap[ CHttpMimeTypes::MimeTypeOctet ] = codec;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitMimeCodecLinks: Hooked up YAML codec to MIME types" );
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

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitMimeCodecLinks: Hooked up JSON codec to MIME types" );
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
    if ( m_serializeRepToCodecMap.find( CHttpMimeTypes::MimeTypeYamlProposed ) != m_serializeRepToCodecMap.end() )
    {
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeYamlProposed );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeYamlText );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeYamlTextExt );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeYamlApp );
        m_serializationReps.push_back( CHttpMimeTypes::MimeTypeYamlAppExt );
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
    if ( m_deserializeRepToCodecMap.find( CHttpMimeTypes::MimeTypeYamlProposed ) != m_deserializeRepToCodecMap.end() )
    {
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeYamlProposed );
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeYamlText );
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeYamlTextExt );
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeYamlApp );
        m_deserializationReps.push_back( CHttpMimeTypes::MimeTypeYamlAppExt );
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

    return ( !m_serializeRepToCodecMap.empty() || !m_deserializeRepToCodecMap.empty() );
}

/*-------------------------------------------------------------------------*/

CHttpCodecLinks::TEncodingCodecPtr 
CHttpCodecLinks::GetEncodingCodec( const CORE::CString& encodingType ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    TStringToEncodingCodecMap::const_iterator i = m_encodingRepToCodecMap.find( encodingType );
    if ( i != m_encodingRepToCodecMap.end() )
    {
        return (*i).second;
    }
    return CHttpCodecLinks::TEncodingCodecPtr();
}

/*-------------------------------------------------------------------------*/

bool 
CHttpCodecLinks::InitEncodingCodecLinks( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();

    // First obtain the registery specific to compression codecs
    CORE::CCodecRegistry::TCodecFamilyRegistryPtr codecFamilyRegistry;
    if ( codecRegistry.TryLookup( CORE::CoreCodecTypes::CompressionCodec, codecFamilyRegistry, false ) && codecFamilyRegistry )
    {
        // Hook up common compression codecs as used within the context of HTTP based interactions
        CORE::CCodecRegistry::TICodecPtr codec;
        if ( ( codecFamilyRegistry->TryLookup( CHttpEncodingTypes::EncodingTypeGZip, codec, false )    ||
               codecFamilyRegistry->TryLookup( CHttpEncodingTypes::EncodingTypeGZipAlt, codec, false )  ) &&
              codec )
        {
            m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeGZip ] = codec;
            m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeGZipAlt ] = codec;

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitEncodingCodecLinks: Hooked up GZIP encoding codec" );
        }
        if ( codecFamilyRegistry->TryLookup( CHttpEncodingTypes::EncodingTypeDeflate, codec, false ) && codec )
        {
            m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeDeflate ] = codec;

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitEncodingCodecLinks: Hooked up Deflate encoding codec" );
        }
        if ( codecFamilyRegistry->TryLookup( CHttpEncodingTypes::EncodingTypeCompress, codec, false ) && codec )
        {
            m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeCompress ] = codec;

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitEncodingCodecLinks: Hooked up Compress encoding codec" );
        }
        if ( codecFamilyRegistry->TryLookup( CHttpEncodingTypes::EncodingTypeBrotli, codec, false ) && codec )
        {
            m_encodingRepToCodecMap[ CHttpEncodingTypes::EncodingTypeBrotli ] = codec;

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):InitEncodingCodecLinks: Hooked up Brotli encoding codec" );
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CHttpCodecLinks::RemoveMimeCodecLinks( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_serializeRepToCodecMap.clear();
    m_deserializeRepToCodecMap.clear();
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):RemoveMimeCodecLinks: Cleared all codec references" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CHttpCodecLinks::RemoveEncodingCodecLinks( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_encodingRepToCodecMap.clear();
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "HttpCodecLinks(" + CORE::ToString( this ) + "):RemoveEncodingCodecLinks: Cleared all codec references" );
    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CHttpCodecLinks::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::WEB::CHttpCodecLinks";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus 
CHttpCodecLinks::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    // no-op: override to implement
    return MT::LOCKSTATUS_NOT_APPLICABLE;
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus 
CHttpCodecLinks::Unlock( void ) const
{GUCEF_TRACE;

    // no-op: override to implement
    return MT::LOCKSTATUS_NOT_APPLICABLE;
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CHttpCodecLinks::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
