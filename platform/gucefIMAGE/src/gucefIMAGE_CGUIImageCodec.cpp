/*
 *  gucefIMAGE: GUCEF module providing image utilities
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#include "gucefIMAGE_CGUIImageCodec.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIImageCodec::CGUIImageCodec( void )
    : CICodec()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CGUIImageCodec::CGUIImageCodec( const CGUIImageCodec& src )
    : CICodec( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CGUIImageCodec&
CGUIImageCodec::operator=( const CGUIImageCodec& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CICodec::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
CGUIImageCodec::~CGUIImageCodec()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CGUIImageCodec::Encode( CORE::CIOAccess& source ,
                        CORE::CIOAccess& dest   )
{GUCEF_TRACE;

    // Since we are encoding from the native format to the native format nothing has to
    // be altered and we can simply copy the data
    dest.Write( source );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIImageCodec::Decode( CORE::CIOAccess& source ,
                        CORE::CIOAccess& dest   )
{GUCEF_TRACE;

    // Since we are decoding from the native format to the native format nothing has to
    // be altered and we can simply copy the data
    dest.Write( source );
    return true;
}

/*-------------------------------------------------------------------------*/
                         
CORE::CString
CGUIImageCodec::GetFamilyName( void ) const
{GUCEF_TRACE;

    return "ImageCodec";
}

/*-------------------------------------------------------------------------*/
    
CORE::CString
CGUIImageCodec::GetType( void ) const
{GUCEF_TRACE;
    
    return "gui";
}

/*-------------------------------------------------------------------------*/
    
CORE::CICloneable*
CGUIImageCodec::Clone( void ) const
{GUCEF_TRACE;

    return new CGUIImageCodec( *this );
}

/*-------------------------------------------------------------------------*/

const CString&
CGUIImageCodec::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::IMAGE::CGUIImageCodec";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/