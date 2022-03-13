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

#include "gucefCORE_CEnumerator.h"

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

CConstEnumerator::CConstEnumerator( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConstEnumerator::CConstEnumerator( const CConstEnumerator& src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConstEnumerator::~CConstEnumerator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConstEnumerator&
CConstEnumerator::operator=( const CConstEnumerator& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

CConstEnumerator& 
CConstEnumerator::operator++( void )
{GUCEF_TRACE;

    if ( !MoveNext() )
    {
        
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CConstEnumerator& 
CConstEnumerator::operator--( void )
{GUCEF_TRACE;

    if ( !MovePrev() )
    {
        
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CConstEnumerator::operator<( const CConstEnumerator& other )
{GUCEF_TRACE;

    return 0 > Compare( other );
}

/*-------------------------------------------------------------------------*/

bool
CConstEnumerator::operator<=( const CConstEnumerator& other )
{GUCEF_TRACE;

    return 0 >= Compare( other );
}

/*-------------------------------------------------------------------------*/

bool
CConstEnumerator::operator>( const CConstEnumerator& other )
{GUCEF_TRACE;

    return 0 < Compare( other );
}

/*-------------------------------------------------------------------------*/

bool
CConstEnumerator::operator>=( const CConstEnumerator& other )
{GUCEF_TRACE;

    return 0 <= Compare( other );
}

/*-------------------------------------------------------------------------*/

bool
CConstEnumerator::operator==( const CConstEnumerator& other )
{GUCEF_TRACE;

    return 0 == Compare( other );
}

/*-------------------------------------------------------------------------*/

bool
CConstEnumerator::operator!=( const CConstEnumerator& other )
{GUCEF_TRACE;

    return 0 != Compare( other );
}

/*-------------------------------------------------------------------------*/
//
//CConstEnumerator::CStyleAccess()
//{GUCEF_TRACE;
//
//}

/*-------------------------------------------------------------------------*/

CEnumerator::CEnumerator( void )
    : CConstEnumerator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEnumerator::CEnumerator( const CEnumerator& src )
    : CConstEnumerator( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEnumerator::~CEnumerator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEnumerator&
CEnumerator::operator=( const CEnumerator& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CConstEnumerator::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CEnumerator& 
CEnumerator::operator++( void )
{GUCEF_TRACE;

    CConstEnumerator::operator++();
    return *this;
}

/*-------------------------------------------------------------------------*/

CEnumerator& 
CEnumerator::operator--( void )
{GUCEF_TRACE;

    CConstEnumerator::operator--();
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CEnumerator::operator<( const CEnumerator& other )
{GUCEF_TRACE;

    return CConstEnumerator::operator<( other );
}

/*-------------------------------------------------------------------------*/

bool
CEnumerator::operator<=( const CEnumerator& other )
{GUCEF_TRACE;

    return CConstEnumerator::operator<=( other );
}

/*-------------------------------------------------------------------------*/

bool
CEnumerator::operator>( const CEnumerator& other )
{GUCEF_TRACE;

    return CConstEnumerator::operator>( other );
}

/*-------------------------------------------------------------------------*/

bool
CEnumerator::operator>=( const CEnumerator& other )
{GUCEF_TRACE;

    return CConstEnumerator::operator>=( other );
}

/*-------------------------------------------------------------------------*/

bool
CEnumerator::operator==( const CEnumerator& other )
{GUCEF_TRACE;

    return CConstEnumerator::operator==( other );
}

/*-------------------------------------------------------------------------*/

bool
CEnumerator::operator!=( const CEnumerator& other )
{GUCEF_TRACE;

    return CConstEnumerator::operator!=( other );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
