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
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefCORE_CVersion.h"

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

UInt16
CVersion::GetMajor( void ) const
{GUCEF_TRACE;

    return m_version.major;
}

/*-------------------------------------------------------------------------*/

UInt16
CVersion::GetMinor( void ) const
{GUCEF_TRACE;

    return m_version.minor;
}

/*-------------------------------------------------------------------------*/

UInt16
CVersion::GetPatch( void ) const
{GUCEF_TRACE;

    return m_version.patch;
}

/*-------------------------------------------------------------------------*/

UInt16
CVersion::GetRelease( void ) const
{GUCEF_TRACE;

    return m_version.release;
}

/*-------------------------------------------------------------------------*/

void 
CVersion::SetMajor( UInt16 major )
{GUCEF_TRACE;

    m_version.major = major;
}

/*-------------------------------------------------------------------------*/

void 
CVersion::SetMinor( UInt16 minor )
{GUCEF_TRACE;

    m_version.minor = minor;
}

/*-------------------------------------------------------------------------*/

void 
CVersion::SetPatch( UInt16 patch )
{GUCEF_TRACE;

    m_version.patch = patch;
}

/*-------------------------------------------------------------------------*/

void 
CVersion::SetRelease( UInt16 release )
{GUCEF_TRACE;

    m_version.release = release;
}

/*-------------------------------------------------------------------------*/

const TVersion& 
CVersion::GetCStyleVersion( void ) const
{GUCEF_TRACE;

    return m_version;
}
    
/*-------------------------------------------------------------------------*/
    
void
CVersion::SetCStyleVersion( const TVersion& version )
{GUCEF_TRACE;

    m_version = version;    
}

/*-------------------------------------------------------------------------*/
    
CVersion::CVersion( UInt16 major   , 
                    UInt16 minor   , 
                    UInt16 patch   ,  
                    UInt16 release )
{GUCEF_TRACE;

    m_version.major = major;
    m_version.minor = minor;
    m_version.patch = patch;
    m_version.release = release;
}

/*-------------------------------------------------------------------------*/

CVersion::CVersion( const CVersion& src )
    : m_version( src.m_version )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVersion::CVersion( const TVersion& version )
    : m_version( version )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVersion::~CVersion()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/

void
CVersion::Clear( void )
{GUCEF_TRACE;

    memset( &m_version, 0, sizeof( m_version ) ); 
}

/*-------------------------------------------------------------------------*/

bool
CVersion::operator<( const CVersion& other ) const
{GUCEF_TRACE;

    if ( m_version.major != other.m_version.major )
        return m_version.major < other.m_version.major;
    if ( m_version.minor != other.m_version.minor )
        return m_version.minor < other.m_version.minor;
    if ( m_version.patch != other.m_version.patch )
        return m_version.patch < other.m_version.patch;
    return m_version.release < other.m_version.release;
}

/*-------------------------------------------------------------------------*/

bool
CVersion::operator>( const CVersion& other ) const
{GUCEF_TRACE;

    return other < *this;
}

/*-------------------------------------------------------------------------*/

bool
CVersion::operator==( const CVersion& other ) const
{GUCEF_TRACE;

    return
        m_version.major == other.m_version.major &&
        m_version.minor == other.m_version.minor &&
        m_version.patch == other.m_version.patch &&
        m_version.release == other.m_version.release;
}

/*-------------------------------------------------------------------------*/

CVersion& 
CVersion::operator=( const CVersion& other )
{GUCEF_TRACE;

    if ( this != &other )
    {
        m_version = other.m_version;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CVersion&
CVersion::operator=( const TVersion& other )
{GUCEF_TRACE;

    m_version = other;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVersion::operator CString() const
{GUCEF_TRACE;

    return VersionToString( m_version );
}

/*-------------------------------------------------------------------------*/

bool 
CVersion::FromString( const CString& versionStr )
{GUCEF_TRACE;

    m_version = StringToVersion( versionStr );
    return true;    
}

/*-------------------------------------------------------------------------*/

CString
CVersion::ToString( bool includeMinor   ,
                    bool includePatch   ,
                    bool includeRelease ) const
{GUCEF_TRACE;

    return VersionToString( m_version, includeMinor, includePatch, includeRelease );    
}

/*-------------------------------------------------------------------------*/

bool 
CVersion::IsAllZero( void ) const
{GUCEF_TRACE;

    return 0 == m_version.major &&
           0 == m_version.minor &&
           0 == m_version.patch &&
           0 == m_version.release;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
