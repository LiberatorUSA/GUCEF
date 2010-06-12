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

#ifndef GUCEF_CORE_MEMORYSEGMENT_H
#define GUCEF_CORE_MEMORYSEGMENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"           /* often used gucef macros */
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

/**
 *  Very simple utility class that is used internally by certain memory pool
 *  implementations for sorting and storing memory pool administration related
 *  data. Basicly a data container with a configurable compare functionality
 */
class GUCEF_CORE_PUBLIC_CPP CMemorySegment
{
    public:

    CMemorySegment( unsigned long offset    ,
                    unsigned long size      ,
                    bool comparesize = true );

    CMemorySegment( const CMemorySegment& src );

    ~CMemorySegment();

    CMemorySegment& operator=( const CMemorySegment& src );

    bool operator<( const CMemorySegment& other ) const;

    bool operator>( const CMemorySegment& other ) const;

    bool operator==( const CMemorySegment& other ) const;

    bool operator!=( const CMemorySegment& other ) const;

    void SetSize( unsigned long newsize );

    unsigned long GetSize( void ) const;

    void SetOffset( unsigned long newoffset );

    unsigned long GetOffset( void ) const;

    private:
    CMemorySegment( void );


    unsigned long m_size;
    unsigned long m_offset;
    bool m_sizecompare;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_MEMORYSEGMENT_H ? */
