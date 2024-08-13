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

#ifndef GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H
#define GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CGEOLOCATION_H
#include "gucefCORE_CGeoLocation.h"
#define GUCEF_CORE_CGEOLOCATION_H
#endif /* GUCEF_CORE_CGEOLOCATION_H ? */

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* macros and build config for the COMCORE library */
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

#ifndef GUCEF_COMCORE_CIPV4ADDRESS_H
#include "gucefCOMCORE_CIPv4Address.h"   
#define GUCEF_COMCORE_CIPV4ADDRESS_H
#endif /* GUCEF_COMCORE_CIPV4ADDRESS_H ? */

#ifndef GUCEF_COMCORE_CIPV6ADDRESS_H
#include "gucefCOMCORE_CIPv6Address.h"   
#define GUCEF_COMCORE_CIPV6ADDRESS_H
#endif /* GUCEF_COMCORE_CIPV6ADDRESS_H ? */

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

/**
 *  Interface class representing a service provider which can be used to lookup
 *  geographical locations based on IP addresses or textual addresses and visa versa.
 */
class GUCEF_COMCORE_EXPORT_CPP CIGeoLocationLookupServiceProvider
{
    public:

    CIGeoLocationLookupServiceProvider( void );

    virtual ~CIGeoLocationLookupServiceProvider( void );

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
                                       CORE::CGeoLocation& outLocation ) = 0;

    /**
     *  Tries to lookup the geographical location information based on the provided IPv4 address.
     */
    virtual bool TryLookupGeoLocation( const CIPv4Address& ipAddress   , 
                                       CORE::CGeoLocation& outLocation ) = 0;

    /**
     *  Tries to lookup the geographical location information based on the provided IPv6 address.
     */
    virtual bool TryLookupGeoLocation( const CIPv6Address& ipAddress   , 
                                       CORE::CGeoLocation& outLocation ) = 0;

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
                                    CString& zipOrPostalCode         ) = 0;

    /**
     *  Tries to lookup the location information based on the IPv4 address.
     *  Best effort is made to fill in as much information as possible.
     * 
     *  The default implementation is a convenience method which calls the TryLookupLocation method
     *  to first translate the IP address to a geographical location and then calls the TryLookupLocation
     */
    virtual bool TryLookupLocation( const CIPv4Address& ipAddress , 
                                    CString& country              ,
                                    CString& stateOrProvice       ,
                                    CString& city                 ,
                                    CString& street               ,
                                    CString& streetNr             ,
                                    CString& zipOrPostalCode      );

    /**
     *  Tries to lookup the location information based on the IPv4 address.
     *  Best effort is made to fill in as much information as possible.
     * 
     *  The default implementation is a convenience method which calls the TryLookupLocation method
     *  to first translate the IP address to a geographical location and then calls the TryLookupLocation
     */
    virtual bool TryLookupLocation( const CIPv6Address& ipAddress , 
                                    CString& country              ,
                                    CString& stateOrProvice       ,
                                    CString& city                 ,
                                    CString& street               ,
                                    CString& streetNr             ,
                                    CString& zipOrPostalCode      );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CIGEOLOCATIONLOOKUPSERVICEPROVIDER_H ? */
