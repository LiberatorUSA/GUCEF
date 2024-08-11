/*
 *  gucefIMAGE: GUCEF module providing image utilities
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