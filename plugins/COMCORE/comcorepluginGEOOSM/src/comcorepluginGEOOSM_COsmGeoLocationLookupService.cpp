/*
 *  comcorepluginGEOOSM: Generic GUCEF COMCORE plugin 
 *  providing Geo Data services based on OpenStreetMap data.
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

#include <curl/curl.h>

#include "comcorepluginGEOOSM_COsmGeoLocationLookupService.h"

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCOREPLUGIN {
namespace GEOOSM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString COsmGeoLocationLookupService::TypeName = "OsmGeoLocationLookupService";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

COsmGeoLocationLookupService::COsmGeoLocationLookupService( void )
    : COMCORE::CIGeoLocationLookupServiceProvider()
    , CORE::CTSharedObjCreator< COsmGeoLocationLookupService, MT::CMutex >( this )
    , m_jsonCodec()
    , m_lazyInitOccured( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

COsmGeoLocationLookupService::~COsmGeoLocationLookupService()
{GUCEF_TRACE;

    Shutdown();
}

/*-------------------------------------------------------------------------*/

bool
COsmGeoLocationLookupService::CUrlInit( void )
{GUCEF_TRACE;

    curl_global_init( CURL_GLOBAL_DEFAULT );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
COsmGeoLocationLookupService::Init( void )
{GUCEF_TRACE;

    if ( !m_lazyInitOccured )
    {
        MT::CScopeMutex lock( m_dataLock );
        if ( !m_lazyInitOccured )
        {        
            if ( CUrlInit() )
            {
                m_jsonCodec = CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().Lookup( "json" );
                if ( !m_jsonCodec.IsNULL() )
                {
                    m_lazyInitOccured = true;
                    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:Init: Lazy init completed" );
                    return true;    
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:Init: Cannot complete lazy init: Failed to lookup JSON codec" );
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:Init: Curl init failed" );
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
COsmGeoLocationLookupService::Shutdown( void )
{GUCEF_TRACE;

    if ( m_lazyInitOccured )
    {
        MT::CScopeMutex lock( m_dataLock );
        if ( m_lazyInitOccured )
        {
            m_jsonCodec.Unlink();
            curl_global_cleanup();

            m_lazyInitOccured = false;
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:Shutdown: shutdown completed" );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
COsmGeoLocationLookupService::ParseGeoLocationLookupResult( const CORE::CDynamicBuffer& readBuffer ,
                                                            CORE::CGeoLocation& outLocation        )
{GUCEF_TRACE;

    if ( !m_jsonCodec.IsNULL() )
    {
        CORE::CDynamicBufferAccess bufferAccess( readBuffer );
        CORE::CDataNode document;
    
        if ( m_jsonCodec->BuildDataTree( &document, &bufferAccess ) )
        {
            CORE::CVariant latValue = document.GetAttributeValueOrChildValueByName( "lat" );
            CORE::CVariant lonValue = document.GetAttributeValueOrChildValueByName( "lon" );
            if ( latValue.IsInitialized() && lonValue.IsInitialized() )
            {               
                outLocation = CORE::CGeoLocation( latValue.AsFloat64(), lonValue.AsFloat64() );
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:ParseGeoLocationLookupResult: Successfully obtained geo location: " + outLocation.ToHumanReadableString() );
                return true;
            }
            else
            {
                // the API could change over time breaking this code
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:ParseGeoLocationLookupResult: Failed to find lattitude and longtitude in reponse" );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:ParseAddressLookupResult: Failed to build data tree from JSON document" );
            
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
COsmGeoLocationLookupService::TryLookupGeoLocation( const CORE::CString& country          ,
                                                    const CORE::CString& stateOrProvice   ,
                                                    const CORE::CString& city             ,
                                                    const CORE::CString& street           ,
                                                    const CORE::CString& streetNr         ,
                                                    const CORE::CString& zipOrPostalCode  , 
                                                    CORE::CGeoLocation& outLocation       )
{GUCEF_TRACE;

    if ( !Init() )
        return false;
    
    bool success = true;    
    CORE::CString url = "https://nominatim.openstreetmap.org/ui/search.html?format=jsonv2"; 
    
    if ( !street.IsNULLOrEmpty() )
    {
        url += "street=" + street + streetNr;
    }
    if ( !city.IsNULLOrEmpty() )
    {
        url += "&city=" + city;
    }
    if ( !stateOrProvice.IsNULLOrEmpty() )
    {
        url += "&state=" + stateOrProvice;
    }
    if ( !country.IsNULLOrEmpty() )
    {
        url += "&country=" + country;
    }
    if ( !zipOrPostalCode.IsNULLOrEmpty() )
    {
        url += "&postalcode=" + zipOrPostalCode;
    }

    CORE::CDynamicBuffer readBuffer;

    CURL* curl = curl_easy_init();
    CURLcode res;

    if ( GUCEF_NULL != curl ) 
    {
        curl_easy_setopt( curl, CURLOPT_URL, url.C_String() );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &readBuffer );
        curl_easy_setopt( curl, CURLOPT_USERAGENT, "Mozilla/5.0 (compatible; AcmeInc/1.0)" );

        res = curl_easy_perform( curl );
        if ( res == CURLE_OK )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:TryLookupLocation: Successfully retrieved " +
                                CORE::ToString( readBuffer.GetDataSize() ) + " bytes of data" );
            
            success = success && ParseGeoLocationLookupResult( readBuffer  , 
                                                               outLocation );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:TryLookupLocation: curl_easy_perform() failed: " +
                                CORE::CString( curl_easy_strerror( res ) ) );
        }

        curl_easy_cleanup( curl );
        curl = GUCEF_NULL;
    }

    return success;
}

/*-------------------------------------------------------------------------*/

bool 
COsmGeoLocationLookupService::TryLookupGeoLocation( const COMCORE::CIPv4Address& ipAddress , 
                                                    CORE::CGeoLocation& outLocation        ) 
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
COsmGeoLocationLookupService::TryLookupGeoLocation( const COMCORE::CIPv6Address& ipAddress , 
                                                    CORE::CGeoLocation& outLocation        )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
COsmGeoLocationLookupService::ParseAddressLookupResult( const CORE::CDynamicBuffer& readBuffer ,
                                                        CORE::CString& country                 ,
                                                        CORE::CString& stateOrProvice          ,
                                                        CORE::CString& city                    ,
                                                        CORE::CString& street                  ,
                                                        CORE::CString& streetNr                ,
                                                        CORE::CString& zipOrPostalCode         ,
                                                        Int16& timeZoneOffsetInMins            ,
                                                        bool& hasTimeZoneOffsetInMins          )
{GUCEF_TRACE;

    if ( !m_jsonCodec.IsNULL() )
    {
        CORE::CDynamicBufferAccess bufferAccess( readBuffer );
        CORE::CDataNode document;
    
        if ( m_jsonCodec->BuildDataTree( &document, &bufferAccess ) )
        {
            CORE::CDataNode* addressNode = document.FindChild( "address" );
            if ( addressNode != GUCEF_NULL )
            {
                country = addressNode->GetAttributeValueOrChildValueByName( "country" ).AsString();
                
                const CORE::CVariant& province = addressNode->GetAttributeValueOrChildValueByName( "province" );
                if ( province.IsInitialized() )
                {
                    stateOrProvice = province.AsString();
                }
                else
                {
                    stateOrProvice =  addressNode->GetAttributeValueOrChildValueByName( "state" ).AsString();
                }
                city = addressNode->GetAttributeValueOrChildValueByName( "city" ).AsString();
                street = addressNode->GetAttributeValueOrChildValueByName( "road" ).AsString();
                streetNr = addressNode->GetAttributeValueOrChildValueByName( "house_number" ).AsString();
                zipOrPostalCode = addressNode->GetAttributeValueOrChildValueByName( "postcode" ).AsString();
            }
            else
            {
                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:ParseAddressLookupResult: Failed to find address node" );
                return false;
            }

            CORE::CDataNode* timeZoneNode = document.FindChild( "timezone" );
            if ( timeZoneNode != GUCEF_NULL )
            {
                timeZoneOffsetInMins = timeZoneNode->GetValue().AsInt16();
                hasTimeZoneOffsetInMins = true;
            }
            else
            {
                hasTimeZoneOffsetInMins = false;
            }

            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:ParseAddressLookupResult: Failed to build data tree from JSON document" );            
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
COsmGeoLocationLookupService::TryLookupLocation( const CORE::CGeoLocation& geoLoc , 
                                                 CORE::CString& country           ,
                                                 CORE::CString& stateOrProvice    ,
                                                 CORE::CString& city              ,
                                                 CORE::CString& street            ,
                                                 CORE::CString& streetNr          ,
                                                 CORE::CString& zipOrPostalCode   ,
                                                 Int16& timeZoneOffsetInMins      ,
                                                 bool& hasTimeZoneOffsetInMins    )
{GUCEF_TRACE;

    if ( !Init() )
        return false;

    bool success = true;    
    CORE::CString url = "https://nominatim.openstreetmap.org/reverse?format=json&lat=" + 
            CORE::ToString( geoLoc.GetLatitude() ) + "&lon=" + 
            CORE::ToString( geoLoc.GetLongitude() ) + "&addressdetails=1";

    CORE::CDynamicBuffer readBuffer;

    CURL* curl = curl_easy_init();
    CURLcode res;

    if ( GUCEF_NULL != curl ) 
    {
        curl_easy_setopt( curl, CURLOPT_URL, url.C_String() );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt( curl, CURLOPT_USERAGENT, "Mozilla/5.0 (compatible; AcmeInc/1.0)");

        res = curl_easy_perform( curl );
        if ( res == CURLE_OK )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:TryLookupLocation: Successfully retrieved " +
                                CORE::ToString( readBuffer.GetDataSize() ) + " bytes of data" );
            
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:TryLookupLocation: received payload: " + CORE::CString( (char*)readBuffer.GetConstBufferPtr(), readBuffer.GetDataSize() ) );
            
            success = success && ParseAddressLookupResult( readBuffer              , 
                                                           country                 , 
                                                           stateOrProvice          , 
                                                           city                    , 
                                                           street                  , 
                                                           streetNr                , 
                                                           zipOrPostalCode         , 
                                                           timeZoneOffsetInMins    , 
                                                           hasTimeZoneOffsetInMins );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "OsmGeoLocationLookupService:TryLookupLocation: curl_easy_perform() failed: " +
                                CORE::CString( curl_easy_strerror( res ) ) );
        }

        curl_easy_cleanup( curl );
        curl = GUCEF_NULL;
    }

    return success;
}

/*-------------------------------------------------------------------------*/

// Callback function to handle the data received from the cURL request
size_t 
COsmGeoLocationLookupService::WriteCallback( void* contents, size_t size, size_t nmemb, void* userp )
{GUCEF_TRACE;

    ((CORE::CDynamicBuffer*)userp)->Append( contents, (UInt32)( size * nmemb ) );
    return size * nmemb;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GEOOSM */
}; /* namespace COMCOREPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
