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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H */

#include "gucefCORE_CVersionRange.h"

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

const CVersion&
CVersionRange::GetMinimum( void ) const
{GUCEF_TRACE;

    return m_minVersion;
}

/*-------------------------------------------------------------------------*/

bool
CVersionRange::GetIsMinimumIncluded( void ) const
{GUCEF_TRACE;

    return m_minIncluded;
}

/*-------------------------------------------------------------------------*/

const CVersion&
CVersionRange::GetMaximum( void ) const
{GUCEF_TRACE;

    return m_maxVersion;
}

/*-------------------------------------------------------------------------*/

bool
CVersionRange::GetIsMaximumIncluded( void ) const
{GUCEF_TRACE;

    return m_maxIncluded;
}

/*-------------------------------------------------------------------------*/

CVersionRange::CVersionRange( void )
    : m_minVersion()        ,
      m_minIncluded( true ) ,
      m_maxVersion()        ,
      m_maxIncluded( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVersionRange::CVersionRange( const CVersion& min ,
                              bool includeMin     ,
                              const CVersion& max ,
                              bool includeMax     )
    : m_minVersion( min )         ,
      m_minIncluded( includeMin ) ,
      m_maxVersion( max )         ,
      m_maxIncluded( includeMax )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVersionRange::CVersionRange( const CVersionRange& src )
    : m_minVersion( src.m_minVersion )   ,
      m_minIncluded( src.m_minIncluded ) ,
      m_maxVersion( src.m_maxVersion )   ,
      m_maxIncluded( src.m_maxIncluded )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVersionRange::~CVersionRange()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CVersionRange::operator<( const CVersionRange& other ) const
{GUCEF_TRACE;

    return m_maxVersion < other.m_maxVersion &&
           m_minVersion < other.m_minVersion &&
           m_maxIncluded != other.m_maxIncluded &&
           m_minIncluded != other.m_minIncluded;
}

/*-------------------------------------------------------------------------*/

bool
CVersionRange::operator==( const CVersionRange& other ) const
{GUCEF_TRACE;

    return m_maxVersion == other.m_maxVersion &&
           m_maxIncluded == other.m_maxIncluded &&
           m_minVersion == other.m_minVersion &&
           m_minIncluded == other.m_minIncluded;
}

/*-------------------------------------------------------------------------*/

CVersionRange&
CVersionRange::operator=( const CVersionRange& other )
{GUCEF_TRACE;

    if ( &other != this )
    {
        m_maxVersion = other.m_maxVersion;
        m_maxIncluded = other.m_maxIncluded;
        m_minVersion = other.m_minVersion;
        m_minIncluded = other.m_minIncluded;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CVersionRange&
CVersionRange::operator=( const CVersion& other )
{GUCEF_TRACE;

    m_maxVersion = other;
    m_maxIncluded = true;
    m_minVersion = other;
    m_minIncluded = true;
    return *this;
}

/*-------------------------------------------------------------------------*/

TVersionRange
CVersionRange::GetCStyle( void ) const
{GUCEF_TRACE;

    TVersionRange range;
    range.max = m_maxVersion.GetCStyleVersion();
    range.includeMax = m_maxIncluded ? 1 : 0;
    range.min = m_minVersion.GetCStyleVersion();
    range.includeMin = m_minIncluded ? 1 : 0;
    return range;
}

/*-------------------------------------------------------------------------*/

void
CVersionRange::SetCStyle( const TVersionRange& version )
{GUCEF_TRACE;

    m_maxVersion.SetCStyleVersion( version.max );
    m_maxIncluded = version.includeMax != 0;
    m_minVersion.SetCStyleVersion( version.min );
    m_minIncluded = version.includeMin != 0;
}

/*-------------------------------------------------------------------------*/

bool
CVersionRange::FromString( const CString& rangeStr )
{GUCEF_TRACE;

    if ( rangeStr.IsNULLOrEmpty() )
        return false;

    Int32 offset = 0;

    CString::StringVector sections = rangeStr.ParseElements( ',', false );
    if ( sections.size() == 1 )
    {
        // 1.0  = x >= 1.0
        // [1.0] = x == 1.0
        // (1.0) = invalid

        CString& versionStr = sections[ 0 ];
        char firstChar = versionStr[ 0 ];
        char lastChar = versionStr[ versionStr.Length()-1 ];

        //if ( firstChar == '(' && lastChar == ')' )
        //    return false;

        if ( firstChar == '[' && lastChar == ']' )
        {
            // Exact match only, no range
            CString semVersionStr = versionStr.SubstrFromRange( 1, versionStr.Length()-1 );
            if ( m_minVersion.FromString( semVersionStr ) )
            {
                m_maxVersion = m_minVersion;
                m_hasMinimum = m_hasMaximum = true;
                m_minIncluded = m_maxIncluded = true;
                return true;
            }
            return false;
        }

        // Use default of >= ?
        bool firstIsDigit = firstChar >= '0' && firstChar <= '9';
        bool lastIsDigit = lastChar >= '0' && lastChar <= '9';
        if ( firstIsDigit && lastIsDigit )
        {
            if ( m_minVersion.FromString( versionStr ) )
            {
                m_maxVersion = m_minVersion;
                m_hasMinimum = m_minIncluded = true;
                m_hasMaximum = m_maxIncluded = false;
                return true;
            }
        }
        return false;
    }
    else
    if ( sections.size() >= 2 )
    {
        //(,1.0]  = x <= 1.0
        //(,1.0)  = x < 1.0
        //(1.0,) = 1.0 < x
        //(1.0,2.0) = 1.0 < x < 2.0
        //[1.0,2.0] = 1.0 <= x <= 2.0

        CString& versionStrA = sections[ 0 ];
        CString& versionStrB = sections[ 1 ];

        char firstCharA = versionStrA[ 0 ];
        char lastCharA = versionStrA[ versionStrA.Length()-1 ];
        char firstCharB = versionStrB[ 0 ];
        char lastCharB = versionStrB[ versionStrB.Length()-1 ];

        bool firstIsDigitA = firstCharA >= '0' && firstCharA <= '9';
        bool lastIsDigitA = lastCharA >= '0' && lastCharA <= '9';
        bool firstIsDigitB = firstCharB >= '0' && firstCharB <= '9';
        bool lastIsDigitB = lastCharB >= '0' && lastCharB <= '9';

        bool hasMaximum = versionStrB.Length() > 1;
        bool hasMinimum = versionStrA.Length() > 1;

        if ( hasMinimum )
        {
            if ( !m_minVersion.FromString( versionStrA.CutChars( 1, true, 0 ) ) )
                return false;
            m_hasMinimum = hasMinimum;
        }
        if ( hasMaximum )
        {
            if ( !m_maxVersion.FromString( versionStrB.CutChars( 1, false, 0 ) ) )
                return false;
            m_hasMaximum = hasMaximum;
        }

        // Sanity check for invalid syntax
        if ( !lastIsDigitA || firstIsDigitA || !firstIsDigitB || lastIsDigitB )
            return false;

    }
    else
    {
        return false;
    }





// @TODO: this code is unfinished
// dont use
return false;

    //CString rangeStr.SubstrToChar( ',', true )
}

/*-------------------------------------------------------------------------*/

CString
CVersionRange::ToString( void ) const
{                     return "";
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

