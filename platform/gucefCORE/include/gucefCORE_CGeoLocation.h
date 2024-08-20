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

#ifndef GUCEF_CORE_CGEOLOCATION_H
#define GUCEF_CORE_CGEOLOCATION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class representing a geographical location via the internationally
 *  recognized method of latitude, longitude and altitude. 
 *  This class makes it easy to break down the location into its components and convert
 *  between different representations.
 */
class GUCEF_CORE_PUBLIC_CPP CGeoLocation 
{
    public:
    
    // Constructor using DMS components
    CGeoLocation( Int32 latDeg = 0               , 
                  Int32 latMin = 0               , 
                  Float64 latSec = 0.0           ,
                  Int32 lonDeg = 0               , 
                  Int32 lonMin = 0               , 
                  Float64 lonSec = 0.0           ,
                  Float64 altitudeInMeters = 0.0 );

    // Constructor using decimal degrees
    CGeoLocation( Float64 latitude               , 
                  Float64 longitude              , 
                  Float64 altitudeInMeters = 0.0 );

    void SetLatitude( Int32 deg, Int32 min, Float64 sec );
    void SetLatitude( Float64 latitude );
    void SetLongitude( Int32 deg, Int32 min, Float64 sec );    
    void SetLongitude( Float64 longitude );
    void SetAltitudeInMeters( Float64 altInMeters );

    Int32 GetLatitudeDegrees( void ) const;
    Int32 GetLatitudeMinutes( void ) const;
    Float64 GetLatitudeSeconds( void ) const;
    Float64 GetLatitude( void ) const;
    
    Int32 GetLongitudeDegrees( void ) const;
    Int32 GetLongitudeMinutes( void ) const;
    Float64 GetLongitudeSeconds( void ) const;
    Float64 GetLongitude( void ) const;

    Float64 GetAltitudeInMeters( void ) const;

    bool operator==( const CGeoLocation& other ) const;
    bool operator!=( const CGeoLocation& other ) const;

    /**
     *  Nonsensical comparison, but needed for sorting purposes
     */
    bool operator<( const CGeoLocation& other ) const;

    bool IsInNorthernHemisphere( void ) const;
    bool IsInSouthernHemisphere( void ) const;
    bool IsInWestDirectionFromMeridian( void ) const;
    bool IsInEastDirectionFromMeridian( void ) const;
    bool IsAboveSeaLevel( void ) const;

    /**
     *  Convert to ISO 6709 string
     */ 
    CString ToISO6709String( void ) const;

    CGeoLocation& FromISO6709String( const std::string& iso6709 );

    static CGeoLocation ConvertISO6709StringToGeoLocation( const std::string& iso6709 );

    CString ToGeoMicroformatString() const;

    static CGeoLocation ConvertGeoMicroformatToGeoLocation( const std::string& geoMicroformat );

    CString ToHumanReadableString( void ) const;
    
    /**
     *  Calculate the distance between two geographical locations.
     *  The distance is calculated using the Haversine formula.
     *  The result is in kilometers.
     * 
     *  @param location1 The first location
     *  @param location2 The second location
     *  @param includeAltitude Whether to include the altitude in the calculation, the default is true
     *  @param celestialBodyRadiusInKm The radius of the celestial body in kilometers, the default is Earth's radius
     */
    static Float64 CalculateDistanceBetweenInKm( const CGeoLocation& location1            , 
                                                 const CGeoLocation& location2            , 
                                                 bool includeAltitude = true              ,
                                                 Float64 celestialBodyRadiusInKm = 6371.0 );

    // Helper function to convert DMS to decimal degrees
    static Float64 ToDecimalDegrees( Int32 deg, Int32 min, Float64 sec );

    private:

    Int32 m_latDeg;
    Int32 m_latMin;
    Float64 m_latSec;
    Int32 m_lonDeg;
    Int32 m_lonMin;    
    Float64 m_lonSec; 
    Float64 m_altitudeInMeters;
};

/*-------------------------------------------------------------------------*/

inline CString ToString( const CGeoLocation& src ) { return src.ToISO6709String(); }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CGEOLOCATION_H ? */
