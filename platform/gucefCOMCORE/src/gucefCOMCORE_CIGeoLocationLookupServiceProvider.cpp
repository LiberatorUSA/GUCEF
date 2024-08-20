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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCOMCORE_CIGeoLocationLookupServiceProvider.h"   

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

CIGeoLocationLookupServiceProvider::CIGeoLocationLookupServiceProvider( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIGeoLocationLookupServiceProvider::~CIGeoLocationLookupServiceProvider( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool 
CIGeoLocationLookupServiceProvider::TryLookupLocation( const CIPv4Address& ipAddress , 
                                                       CString& country              ,
                                                       CString& stateOrProvice       ,
                                                       CString& city                 ,
                                                       CString& street               ,
                                                       CString& streetNr             ,
                                                       CString& zipOrPostalCode      ,
                                                       Int16& timeZoneOffsetInMins   ,
                                                       bool& hasTimeZoneOffsetInMins )
{GUCEF_TRACE;

    CORE::CGeoLocation geoLocation;
    if ( TryLookupGeoLocation( ipAddress, geoLocation ) )
    {
        return TryLookupLocation( geoLocation, 
                                  country, 
                                  stateOrProvice, 
                                  city, 
                                  street, 
                                  streetNr, 
                                  zipOrPostalCode, 
                                  timeZoneOffsetInMins, 
                                  hasTimeZoneOffsetInMins );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CIGeoLocationLookupServiceProvider::TryLookupLocation( const CIPv6Address& ipAddress , 
                                                       CString& country              ,
                                                       CString& stateOrProvice       ,
                                                       CString& city                 ,
                                                       CString& street               ,
                                                       CString& streetNr             ,
                                                       CString& zipOrPostalCode      ,
                                                       Int16& timeZoneOffsetInMins   ,
                                                       bool& hasTimeZoneOffsetInMins )
{GUCEF_TRACE;

    CORE::CGeoLocation geoLocation;
    if ( TryLookupGeoLocation( ipAddress, geoLocation ) )
    {
        return TryLookupLocation( geoLocation, 
                                  country, 
                                  stateOrProvice, 
                                  city, 
                                  street, 
                                  streetNr, 
                                  zipOrPostalCode, 
                                  timeZoneOffsetInMins, 
                                  hasTimeZoneOffsetInMins );
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
