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

#include <assert.h>
#include <iostream>

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#define GUCEF_CORE_CSTDOSTREAMADAPTER_CPP
#include "CSTDOStreamAdapter.h"

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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

class CSTDOStreamAdapterBuffer : public std::basic_streambuf< char, std::char_traits< char > >
{
    public:

    typedef std::basic_streambuf< char, std::char_traits< char > > base;

    CSTDOStreamAdapterBuffer( CIOAccess& access );

    protected:

    virtual char overflow( char value );

    private:

    CSTDOStreamAdapterBuffer( const CSTDOStreamAdapterBuffer& src );
    CSTDOStreamAdapterBuffer& operator=( const CSTDOStreamAdapterBuffer& src );

    private:

    CIOAccess* m_access;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CSTDOStreamAdapterBuffer::CSTDOStreamAdapterBuffer( CIOAccess& access )
        : base()              ,
          m_access( &access )
{GUCEF_TRACE;

    assert( m_access );
}

/*-------------------------------------------------------------------------*/

char
CSTDOStreamAdapterBuffer::overflow( char value )
{GUCEF_TRACE;

    // put a character to stream
    if ( m_access->Write( &value, 1, 1 ) == 1 )
    {
    }
    return traits_type::eof();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CSTDOStreamAdapter::CSTDOStreamAdapter( CIOAccess& access )
    : std::ostream( 0 )                                   ,
      m_buffer( GUCEF_NEW CSTDOStreamAdapterBuffer( access ) )
{GUCEF_TRACE;

    std::ostream::init( m_buffer );
}

/*-------------------------------------------------------------------------*/

CSTDOStreamAdapter::~CSTDOStreamAdapter()
{GUCEF_TRACE;
    
    GUCEF_DELETE m_buffer;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
