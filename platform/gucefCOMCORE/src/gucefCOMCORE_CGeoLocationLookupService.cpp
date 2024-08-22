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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefCOMCORE_CGeoLocationLookupService.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CGeoLocationLookupService::ClassTypeName = "GUCEF::COMCORE::CGeoLocationLookupService";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CGeoLocationLookupService::CGeoLocationLookupService( void )
    : CORE::CGloballyConfigurable()
    , CIGeoLocationLookupServiceProvider()
    , m_registry()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGeoLocationLookupService::~CGeoLocationLookupService()
{GUCEF_TRACE;

    m_registry.UnregisterAll();
}

/*-------------------------------------------------------------------------*/

bool 
CGeoLocationLookupService::TryLookupGeoLocation( const CString& country          ,
                                                 const CString& stateOrProvice   ,
                                                 const CString& city             ,
                                                 const CString& street           ,
                                                 const CString& streetNr         ,
                                                 const CString& zipOrPostalCode  , 
                                                 CORE::CGeoLocation& outLocation )
{GUCEF_TRACE;

    TGeoLocationLookupServiceRegistry::TRegisteredObjPtrVector objs;
    m_registry.GetRegisteredObjs( objs );

    TGeoLocationLookupServiceRegistry::TRegisteredObjPtrVector::iterator i = objs.begin();
    while ( i != objs.end() )
    {
        if ( (*i)->TryLookupGeoLocation( country          ,
                                         stateOrProvice   ,
                                         city             ,
                                         street           ,
                                         streetNr         ,
                                         zipOrPostalCode  , 
                                         outLocation      ) )
        {
            return true;
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CGeoLocationLookupService::TryLookupGeoLocation( const CIPv4Address& ipAddress   , 
                                                 CORE::CGeoLocation& outLocation ) 
{GUCEF_TRACE;

    TGeoLocationLookupServiceRegistry::TRegisteredObjPtrVector objs;
    m_registry.GetRegisteredObjs( objs );

    TGeoLocationLookupServiceRegistry::TRegisteredObjPtrVector::iterator i = objs.begin();
    while ( i != objs.end() )
    {
        if ( (*i)->TryLookupGeoLocation( ipAddress   ,
                                         outLocation ) )
        {
            return true;
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CGeoLocationLookupService::TryLookupGeoLocation( const CIPv6Address& ipAddress   , 
                                                 CORE::CGeoLocation& outLocation ) 
{GUCEF_TRACE;

    TGeoLocationLookupServiceRegistry::TRegisteredObjPtrVector objs;
    m_registry.GetRegisteredObjs( objs );

    TGeoLocationLookupServiceRegistry::TRegisteredObjPtrVector::iterator i = objs.begin();
    while ( i != objs.end() )
    {
        if ( (*i)->TryLookupGeoLocation( ipAddress   ,
                                         outLocation ) )
        {
            return true;
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CGeoLocationLookupService::TryLookupLocation( const CORE::CGeoLocation& geoLoc , 
                                              CString& country                 ,
                                              CString& stateOrProvice          ,
                                              CString& city                    ,
                                              CString& street                  ,
                                              CString& streetNr                ,
                                              CString& zipOrPostalCode         ,
                                              Int16& timeZoneOffsetInMins      ,
                                              bool& hasTimeZoneOffsetInMins    )
{GUCEF_TRACE;

    TGeoLocationLookupServiceRegistry::TRegisteredObjPtrVector objs;
    m_registry.GetRegisteredObjs( objs );

    TGeoLocationLookupServiceRegistry::TRegisteredObjPtrVector::iterator i = objs.begin();
    while ( i != objs.end() )
    {
        if ( (*i)->TryLookupLocation( geoLoc                 ,
                                      country                ,
                                      stateOrProvice         ,
                                      city                   ,
                                      street                 ,
                                      streetNr               ,
                                      zipOrPostalCode        ,
                                      timeZoneOffsetInMins   ,
                                      hasTimeZoneOffsetInMins ) )
        {
            return true;
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const CString& 
CGeoLocationLookupService::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

CGeoLocationLookupService::TGeoLocationLookupServiceRegistry& 
CGeoLocationLookupService::GetGeoLocationLookupServiceRegistry( void )
{GUCEF_TRACE;

    return m_registry;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
