/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "gucefGUI_CUDim.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CUDim::CUDim( void )
    : m_relativeValue( 0.0 ) ,
      m_pixelValue( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUDim::CUDim( const CUDim& src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUDim::~CUDim()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUDim&
CUDim::operator=( const CUDim& src )
{GUCEF_TRACE;
    
    if ( this == &src )
    {
        m_relativeValue = src.m_relativeValue;
        m_pixelValue = src.m_pixelValue;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CUDim::operator==( const CUDim& other ) const
{GUCEF_TRACE;

    return m_relativeValue == other.m_relativeValue &&
           m_pixelValue == other.m_pixelValue;
}

/*-------------------------------------------------------------------------*/

bool
CUDim::operator!=( const CUDim& other ) const
{GUCEF_TRACE;

    return m_relativeValue != other.m_relativeValue ||
           m_pixelValue != other.m_pixelValue;
}

/*-------------------------------------------------------------------------*/

void
CUDim::SetRelativeValue( const Float32 relativeValue ) 
{GUCEF_TRACE;

    m_relativeValue = relativeValue;
}

/*-------------------------------------------------------------------------*/

Float32
CUDim::GetRelativeValue( void ) const
{GUCEF_TRACE;

    return m_relativeValue;
}

/*-------------------------------------------------------------------------*/
    
void
CUDim::SetPixelValue( const UInt32 pixelValue )
{GUCEF_TRACE;

    m_pixelValue = pixelValue;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CUDim::GetPixelValue( void ) const
{GUCEF_TRACE;

    return m_pixelValue;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
