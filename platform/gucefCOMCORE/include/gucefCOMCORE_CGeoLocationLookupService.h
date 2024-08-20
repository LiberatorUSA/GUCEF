/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CGEOLOCATIONLOOKUPSERVICE_H
#define GUCEF_COMCORE_CGEOLOCATIONLOOKUPSERVICE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#include "gucefCORE_CGloballyConfigurable.h"
#define GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#endif /* GUCEF_CORE_CGLOBALLYCONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CTGLOBALONREGISTRY_H
#include "gucefCORE_CTGlobalONRegistry.h"
#define GUCEF_CORE_CTGLOBALONREGISTRY_H
#endif /* GUCEF_CORE_CTGLOBALONREGISTRY_H ? */

#ifndef GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H
#include "gucefCOMCORE_CIGeoLocationLookupServiceProvider.h"
#define GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H
#endif /* GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CComCoreGlobal;

/**
 *  class representing geo location lookup services which can be used to lookup
 *  geographical locations based on IP addresses or textual addresses and visa versa.
 */
class GUCEF_COMCORE_EXPORT_CPP CGeoLocationLookupService : public CORE::CGloballyConfigurable        ,
                                                           public CIGeoLocationLookupServiceProvider
{
    public:

    typedef CORE::CTGlobalONRegistry< CIGeoLocationLookupServiceProvider, MT::CMutex > TGeoLocationLookupServiceRegistry;

    static const CString ClassTypeName;

    /**
     *  Tries to lookup the geographical location information based on the provided location information.
     *  Best effort is made based on whatever information was provided so please fill in as much information as possible.
     */
    virtual bool TryLookupGeoLocation( const CString& country          ,
                                       const CString& stateOrProvice   ,
                                       const CString& city             ,
                                       const CString& street           ,
                                       const CString& streetNr         ,
                                       const CString& zipOrPostalCode  , 
                                       CORE::CGeoLocation& outLocation ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Tries to lookup the geographical location information based on the provided IPv4 address.
     */
    virtual bool TryLookupGeoLocation( const CIPv4Address& ipAddress   , 
                                       CORE::CGeoLocation& outLocation ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Tries to lookup the geographical location information based on the provided IPv6 address.
     */
    virtual bool TryLookupGeoLocation( const CIPv6Address& ipAddress   , 
                                       CORE::CGeoLocation& outLocation ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Tries to lookup the location information based on the provided geographical location.
     *  Best effort is made to fill in as much information as possible.
     */
    virtual bool TryLookupLocation( const CORE::CGeoLocation& geoLoc , 
                                    CString& country                 ,
                                    CString& stateOrProvice          ,
                                    CString& city                    ,
                                    CString& street                  ,
                                    CString& streetNr                ,
                                    CString& zipOrPostalCode         ,
                                    Int16& timeZoneOffsetInMins      ,
                                    bool& hasTimeZoneOffsetInMins    ) GUCEF_VIRTUAL_OVERRIDE;
    
    /**
     *  Provides access to the registry where all providers are registered
     */
    TGeoLocationLookupServiceRegistry& GetGeoLocationLookupServiceRegistry( void );

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CComCoreGlobal;

    CGeoLocationLookupService( void );

    virtual ~CGeoLocationLookupService() GUCEF_VIRTUAL_OVERRIDE;

    private:

    TGeoLocationLookupServiceRegistry m_registry;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CGEOLOCATIONLOOKUPSERVICE_H ? */
