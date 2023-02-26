/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#include "gucefCORE_CGlobalConfigValueList.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CGlobalConfigValueList::ClassTypeName = "GUCEF::CORE::CGlobalConfigValueList";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CGlobalConfigValueList::CGlobalConfigValueList( void )
    : CValueList()
    , CGloballyConfigurable( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGlobalConfigValueList::CGlobalConfigValueList( const CGlobalConfigValueList& src )
    : CValueList( src )
    , CGloballyConfigurable( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGlobalConfigValueList::CGlobalConfigValueList( const CValueList& src )
    : CValueList( src )
    , CGloballyConfigurable( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGlobalConfigValueList::~CGlobalConfigValueList( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGlobalConfigValueList& 
CGlobalConfigValueList::operator=( const CValueList& src )
{GUCEF_TRACE;

    CValueList::operator=( src );
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CGlobalConfigValueList::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    return CValueList::SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool 
CGlobalConfigValueList::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    return CValueList::LoadConfig( cfg );
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CGlobalConfigValueList::Clone( void ) const
{GUCEF_TRACE;

    return new CGlobalConfigValueList( *this );
}

/*-------------------------------------------------------------------------*/

const CString& 
CGlobalConfigValueList::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
