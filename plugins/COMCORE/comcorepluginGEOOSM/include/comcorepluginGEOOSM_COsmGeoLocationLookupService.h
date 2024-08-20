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

#ifndef COMCOREPLUGIN_GEOOSM_COSMGEOLOCATIONLOOKUPSERVICE_H
#define COMCOREPLUGIN_GEOOSM_COSMGEOLOCATIONLOOKUPSERVICE_H
                              
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H
#include "gucefCOMCORE_CIGeoLocationLookupServiceProvider.h"
#define GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H
#endif /* GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H ? */

#ifndef COMCOREPLUGIN_GEOOSM_MACROS_H
#include "comcorepluginGEOOSM_macros.h"
#define COMCOREPLUGIN_GEOOSM_MACROS_H
#endif /* COMCOREPLUGIN_GEOOSM_MACROS_H ? */

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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Implementation of a geo location lookup service based on OpenStreetMap data.
 */
class COMCOREPLUGIN_GEOOSM_PUBLIC_CPP COsmGeoLocationLookupService : public COMCORE::CIGeoLocationLookupServiceProvider ,
                                                                     public CORE::CTSharedObjCreator< COsmGeoLocationLookupService, MT::CMutex >
{
    public:

    typedef typename CORE::CTSharedObjCreator< COsmGeoLocationLookupService, MT::CMutex >::TSharedPtrType  COsmGeoLocationLookupServicePtr;

    static const CORE::CString TypeName;

    COsmGeoLocationLookupService( void );

    virtual ~COsmGeoLocationLookupService( void ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Tries to lookup the geographical location information based on the provided location information.
     *  Best effort is made based on whatever information was provided so please fill in as much information as possible.
     */
    virtual bool TryLookupGeoLocation( const CORE::CString& country          ,
                                       const CORE::CString& stateOrProvice   ,
                                       const CORE::CString& city             ,
                                       const CORE::CString& street           ,
                                       const CORE::CString& streetNr         ,
                                       const CORE::CString& zipOrPostalCode  , 
                                       CORE::CGeoLocation& outLocation       ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Tries to lookup the geographical location information based on the provided IPv4 address.
     */
    virtual bool TryLookupGeoLocation( const COMCORE::CIPv4Address& ipAddress , 
                                       CORE::CGeoLocation& outLocation        ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Tries to lookup the geographical location information based on the provided IPv6 address.
     */
    virtual bool TryLookupGeoLocation( const COMCORE::CIPv6Address& ipAddress , 
                                       CORE::CGeoLocation& outLocation        ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Tries to lookup the location information based on the provided geographical location.
     *  Best effort is made to fill in as much information as possible.
     */
    virtual bool TryLookupLocation( const CORE::CGeoLocation& geoLoc , 
                                    CORE::CString& country           ,
                                    CORE::CString& stateOrProvice    ,
                                    CORE::CString& city              ,
                                    CORE::CString& street            ,
                                    CORE::CString& streetNr          ,
                                    CORE::CString& zipOrPostalCode   ,
                                    Int16& timeZoneOffsetInMins      ,
                                    bool& hasTimeZoneOffsetInMins    ) GUCEF_VIRTUAL_OVERRIDE;


    bool Init( void );

    void Shutdown( void );

    private:

    static size_t WriteCallback( void* contents, size_t size, size_t nmemb, void* userp );

    bool CUrlInit( void );

    bool ParseAddressLookupResult( const CORE::CDynamicBuffer& readBuffer ,
                                   CORE::CString& country                 ,
                                   CORE::CString& stateOrProvice          ,
                                   CORE::CString& city                    ,
                                   CORE::CString& street                  ,
                                   CORE::CString& streetNr                ,
                                   CORE::CString& zipOrPostalCode         ,
                                   Int16& timeZoneOffsetInMins            ,
                                   bool& hasTimeZoneOffsetInMins          );
    
    bool ParseGeoLocationLookupResult( const CORE::CDynamicBuffer& readBuffer ,
                                       CORE::CGeoLocation& outLocation        );
    
    private:

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr m_jsonCodec;
    bool m_lazyInitOccured;
    MT::CMutex m_dataLock;

};

/*-------------------------------------------------------------------------*/

typedef COsmGeoLocationLookupService::COsmGeoLocationLookupServicePtr  COsmGeoLocationLookupServicePtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GEOOSM */
}; /* namespace COMCOREPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* COMCOREPLUGIN_GEOOSM_COSMGEOLOCATIONLOOKUPSERVICE_H ? */
