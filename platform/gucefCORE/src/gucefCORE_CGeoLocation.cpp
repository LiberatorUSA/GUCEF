/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#include "gucefCORE_CGeoLocation.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CGeoLocation::CGeoLocation( Int32 latDeg             , 
                            Int32 latMin             , 
                            Float64 latSec           ,
                            Int32 lonDeg             , 
                            Int32 lonMin             , 
                            Float64 lonSec           ,
                            Float64 altitudeInMeters )
    : m_latDeg( latDeg )
    , m_latMin( latMin )
    , m_latSec( latSec )
    , m_lonDeg( lonDeg )
    , m_lonMin( lonMin )
    , m_lonSec( lonSec )
    , m_altitudeInMeters( altitudeInMeters ) 
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGeoLocation::CGeoLocation( Float64 latitude         , 
                            Float64 longitude        , 
                            Float64 altitudeInMeters ) 
    : m_latDeg( 0 )
    , m_latMin( 0 )
    , m_latSec( 0.0 )
    , m_lonDeg( 0 )
    , m_lonMin( 0 )
    , m_lonSec( 0.0 )
    , m_altitudeInMeters( altitudeInMeters ) 
{GUCEF_TRACE;

    SetLatitude( latitude );
    SetLongitude( longitude );
}

/*-------------------------------------------------------------------------*/

Int32 
CGeoLocation::GetLatitudeDegrees( void ) const 
{GUCEF_TRACE; 
    
    return m_latDeg; 
}

/*-------------------------------------------------------------------------*/

Int32 
CGeoLocation::GetLatitudeMinutes( void ) const 
{GUCEF_TRACE; 
    
    return m_latMin; 
}

/*-------------------------------------------------------------------------*/

Float64 
CGeoLocation::GetLatitudeSeconds( void ) const 
{GUCEF_TRACE; 
    
    return m_latSec; 
}

/*-------------------------------------------------------------------------*/

Int32 
CGeoLocation::GetLongitudeDegrees( void ) const 
{GUCEF_TRACE; 
    
    return m_lonDeg; 
}

/*-------------------------------------------------------------------------*/

Int32 
CGeoLocation::GetLongitudeMinutes( void ) const 
{GUCEF_TRACE; 
    
    return m_lonMin; 
}

/*-------------------------------------------------------------------------*/

Float64 
CGeoLocation::GetLongitudeSeconds( void ) const 
{GUCEF_TRACE; 
    
    return m_lonSec; 
}

/*-------------------------------------------------------------------------*/

Float64 
CGeoLocation::GetAltitudeInMeters( void ) const 
{GUCEF_TRACE; 
    
    return m_altitudeInMeters; 
}

/*-------------------------------------------------------------------------*/

Float64 
CGeoLocation::GetLatitude( void ) const
{GUCEF_TRACE; 
    
    return ToDecimalDegrees( m_latDeg, m_latMin, m_latSec );
}

/*-------------------------------------------------------------------------*/

Float64 
CGeoLocation::GetLongitude( void ) const
{GUCEF_TRACE; 
    
    return ToDecimalDegrees( m_lonDeg, m_lonMin, m_lonSec );
}

/*-------------------------------------------------------------------------*/

void 
CGeoLocation::SetLatitude( Int32 deg, Int32 min, Float64 sec ) 
{GUCEF_TRACE;

    m_latDeg = deg;
    m_latMin = min;
    m_latSec = sec;
}

/*-------------------------------------------------------------------------*/

void 
CGeoLocation::SetLongitude( Int32 deg, Int32 min, Float64 sec ) 
{GUCEF_TRACE;

    m_lonDeg = deg;
    m_lonMin = min;
    m_lonSec = sec;
}

/*-------------------------------------------------------------------------*/

void 
CGeoLocation::SetLatitude( Float64 latitude ) 
{GUCEF_TRACE;

    m_latDeg = static_cast<Int32>(latitude);
    m_latMin = static_cast<Int32>((latitude - m_latDeg) * 60);
    m_latSec = (latitude - m_latDeg - m_latMin / 60.0) * 3600.0;
}

/*-------------------------------------------------------------------------*/

void 
CGeoLocation::SetLongitude( Float64 longitude ) 
{GUCEF_TRACE;

    m_lonDeg = static_cast<Int32>(longitude);
    m_lonMin = static_cast<Int32>((longitude - m_lonDeg) * 60);
    m_lonSec = (longitude - m_lonDeg - m_lonMin / 60.0) * 3600.0;
}

/*-------------------------------------------------------------------------*/

void 
CGeoLocation::SetAltitudeInMeters( Float64 altInMeters ) 
{GUCEF_TRACE;

    m_altitudeInMeters = altInMeters; 
}

/*-------------------------------------------------------------------------*/

bool 
CGeoLocation::operator==( const CGeoLocation& other ) const 
{GUCEF_TRACE;

    return m_latDeg == other.m_latDeg && 
           m_latMin == other.m_latMin && 
           ( ( (Float32) std::fabs( m_latSec - other.m_latSec ) ) < GUCEF_FLOAT32_COMPARISON_EPSILON ) &&
           m_lonDeg == other.m_lonDeg && 
           m_lonMin == other.m_lonMin && 
           ( ( (Float32) std::fabs( m_lonSec - other.m_lonSec ) ) < GUCEF_FLOAT32_COMPARISON_EPSILON ) &&
           ( ( (Float32) std::fabs( m_altitudeInMeters - other.m_altitudeInMeters ) ) < GUCEF_FLOAT32_COMPARISON_EPSILON );
}

/*-------------------------------------------------------------------------*/

bool 
CGeoLocation::operator!=( const CGeoLocation& other ) const 
{GUCEF_TRACE;

    return !(*this == other);
}

/*-------------------------------------------------------------------------*/

bool 
CGeoLocation::operator<( const CGeoLocation& other ) const 
{GUCEF_TRACE;

    return m_latDeg < other.m_latDeg || 
           m_latMin < other.m_latMin || 
           m_latSec < other.m_latSec ||
           m_lonDeg < other.m_lonDeg || 
           m_lonMin < other.m_lonMin || 
           m_lonSec < other.m_lonSec ||
           m_altitudeInMeters < other.m_altitudeInMeters;
}

/*-------------------------------------------------------------------------*/

// Convert to ISO 6709 string
CString 
CGeoLocation::ToISO6709String( void ) const 
{GUCEF_TRACE;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6)
        << (m_latDeg >= 0 ? "+" : "") << m_latDeg << "°" << m_latMin << "'" << m_latSec << "\""
        << (m_lonDeg >= 0 ? "+" : "") << m_lonDeg << "°" << m_lonMin << "'" << m_lonSec << "\"";
    
    if ( (Float32) std::fabs( m_altitudeInMeters ) < GUCEF_FLOAT32_COMPARISON_EPSILON ) 
    {
        oss << ( m_altitudeInMeters >= 0 ? "+" : "") << m_altitudeInMeters << "CRS84";
    }
    return oss.str();
}

/*-------------------------------------------------------------------------*/

CGeoLocation 
CGeoLocation::ConvertISO6709StringToGeoLocation( const std::string& iso6709 ) 
{GUCEF_TRACE;

    Int32 latDeg = 0, latMin = 0, lonDeg = 0, lonMin = 0;
    Float64 latSec = 0.0, lonSec = 0.0, alt = 0.0;
    char latSign = '+', lonSign = '+', altSign = '+';
    
    std::istringstream iss( iso6709 );
    iss >> latSign >> latDeg >> latMin >> latSec >> lonSign >> lonDeg >> lonMin >> lonSec;
    
    if ( iss.peek() == '+' || iss.peek() == '-' ) 
    {
        iss >> altSign >> alt;
    }

    if ( latSign == '-' ) 
        latDeg = -latDeg;
    if ( lonSign == '-' ) 
        lonDeg = -lonDeg;
    if ( altSign == '-' ) 
        alt = -alt;

    return CGeoLocation( latDeg, latMin, latSec, lonDeg, lonMin, lonSec, alt );
}

/*-------------------------------------------------------------------------*/

// Convert to Geo microformat string
CString
CGeoLocation::ToGeoMicroformatString( void ) const
{GUCEF_TRACE;

    std::ostringstream oss;
    oss << "<span class=\"geo\"><abbr class=\"latitude\" title=\""
        << std::fixed << std::setprecision(6) << m_latDeg << "°" << m_latMin << "'" << m_latSec << "\""
        << "\"></abbr><abbr class=\"longitude\" title=\""
        << std::fixed << std::setprecision(6) << m_lonDeg << "°" << m_lonMin << "'" << m_lonSec << "\""
        << "\"></abbr></span>";

    return oss.str();
}

/*-------------------------------------------------------------------------*/

CGeoLocation 
CGeoLocation::ConvertGeoMicroformatToGeoLocation( const std::string& geoMicroformat ) 
{GUCEF_TRACE;

    Int32 latDeg = 0, latMin = 0, lonDeg = 0, lonMin = 0;
    Float64 latSec = 0.0, lonSec = 0.0;
    size_t latPos = geoMicroformat.find("class=\"latitude\" title=\"");
    size_t lonPos = geoMicroformat.find("class=\"longitude\" title=\"");

    if (latPos == std::string::npos || lonPos == std::string::npos) {
        throw std::runtime_error("Invalid Geo microformat string");
    }

    latPos += 25; // Move to the start of the latitude value
    lonPos += 26; // Move to the start of the longitude value

    size_t latEnd = geoMicroformat.find("\"", latPos);
    size_t lonEnd = geoMicroformat.find("\"", lonPos);

    if (latEnd == std::string::npos || lonEnd == std::string::npos) {
        throw std::runtime_error("Invalid Geo microformat string");
    }

    std::istringstream latStream(geoMicroformat.substr(latPos, latEnd - latPos));
    std::istringstream lonStream(geoMicroformat.substr(lonPos, lonEnd - lonPos));

    char dummy;
    latStream >> latDeg >> dummy >> latMin >> dummy >> latSec;
    lonStream >> lonDeg >> dummy >> lonMin >> dummy >> lonSec;

    return CGeoLocation( latDeg, latMin, latSec, lonDeg, lonMin, lonSec );
}

/*-------------------------------------------------------------------------*/

// Calculate distance between two GeoLocation instances using Haversine formula and altitude difference
Float64 
CGeoLocation::CalculateDistanceBetweenInKm( const CGeoLocation& loc1        , 
                                            const CGeoLocation& loc2        , 
                                            bool includeAltitude            , 
                                            Float64 celestialBodyRadiusInKm ) 
{GUCEF_TRACE;

    Float64 lat1 = loc1.GetLatitude() * GUCEF_FLOAT64_PI / 180.0;
    Float64 lon1 = loc1.GetLongitude() * GUCEF_FLOAT64_PI / 180.0;
    Float64 lat2 = loc2.GetLatitude() * GUCEF_FLOAT64_PI / 180.0;
    Float64 lon2 = loc2.GetLongitude() * GUCEF_FLOAT64_PI / 180.0;

    Float64 dlat = lat2 - lat1;
    Float64 dlon = lon2 - lon1;

    Float64 a = std::sin(dlat / 2) * std::sin(dlat / 2) +
                std::cos(lat1) * std::cos(lat2) *
                std::sin(dlon / 2) * std::sin(dlon / 2);
    Float64 c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    Float64 horizontalDistance = celestialBodyRadiusInKm * c;

    if ( includeAltitude )
    {
        Float64 altitudeDifferenceInMeters = loc2.GetAltitudeInMeters() - loc1.GetAltitudeInMeters();
        return std::sqrt( horizontalDistance * horizontalDistance + altitudeDifferenceInMeters * altitudeDifferenceInMeters );
    }

    return horizontalDistance;
}

/*-------------------------------------------------------------------------*/

// Convert to human-readable string
CString
CGeoLocation::ToHumanReadableString( void ) const 
{GUCEF_TRACE;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2)
        << "Latitude: " << m_latDeg << "°" << m_latMin << "'" << m_latSec << "\", "
        << "Longitude: " << m_lonDeg << "°" << m_lonMin << "'" << m_lonSec << "\", "
        << "Altitude: " << m_altitudeInMeters << " meters";
    return oss.str();
}

/*-------------------------------------------------------------------------*/

Float64 
CGeoLocation::ToDecimalDegrees( Int32 deg, Int32 min, Float64 sec ) 
{GUCEF_TRACE;

    return deg + min / 60.0 + sec / 3600.0;
}

/*-------------------------------------------------------------------------*/

bool
CGeoLocation::IsInNorthernHemisphere( void ) const
{GUCEF_TRACE;
    
    return m_latDeg >= 0;
}

/*-------------------------------------------------------------------------*/

bool
CGeoLocation::IsInSouthernHemisphere( void ) const
{GUCEF_TRACE;
    
    return m_latDeg < 0;
}

/*-------------------------------------------------------------------------*/

bool
CGeoLocation::IsInWestDirectionFromMeridian( void ) const
{GUCEF_TRACE;
        
    return m_lonDeg < 0;    
}

/*-------------------------------------------------------------------------*/

bool
CGeoLocation::IsInEastDirectionFromMeridian( void ) const
{GUCEF_TRACE;
    
    return m_lonDeg >= 0;
}

/*-------------------------------------------------------------------------*/

CGeoLocation& 
CGeoLocation::FromISO6709String( const std::string& iso6709 )
{GUCEF_TRACE;
    
    *this = ConvertISO6709StringToGeoLocation( iso6709 );
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
