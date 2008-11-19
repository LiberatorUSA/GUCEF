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

#include "MemorySegment.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CMemorySegment::CMemorySegment( void )
{
    /* dummy, do not use */
}

/*---------------------------------------------------------------------------*/

CMemorySegment::CMemorySegment( unsigned long offset          ,
                                unsigned long size            ,
                                bool comparesize /* = true */ )
    : m_sizecompare( comparesize ) ,
      m_offset( offset )           ,
      m_size( size )
{
    
}

/*---------------------------------------------------------------------------*/

CMemorySegment::CMemorySegment( const CMemorySegment& src )
    : m_sizecompare( src.m_sizecompare ) ,
      m_offset( src.m_offset )           ,
      m_size( src.m_size )
{
 
}

/*---------------------------------------------------------------------------*/

CMemorySegment::~CMemorySegment()
{
}

/*---------------------------------------------------------------------------*/

CMemorySegment&
CMemorySegment::operator=( const CMemorySegment& src )
{
    if ( this != &src )
    {
        m_sizecompare = src.m_sizecompare;
        m_offset = src.m_offset;
        m_size = src.m_size;     
    }
    return *this;
}

/*---------------------------------------------------------------------------*/

bool 
CMemorySegment::operator<( const CMemorySegment& other ) const
{
    if ( m_sizecompare )
    {
        return m_size < other.m_size;
    }
    return m_offset < other.m_offset;
}

/*---------------------------------------------------------------------------*/

bool 
CMemorySegment::operator>( const CMemorySegment& other ) const
{
    if ( m_sizecompare )
    {
        return m_size > other.m_size;
    }
    return m_offset > other.m_offset;
}

/*---------------------------------------------------------------------------*/

bool 
CMemorySegment::operator==( const CMemorySegment& other ) const
{
    if ( m_sizecompare ) 
    {
        return m_size == other.m_size;
    }
    return m_offset == other.m_offset;   
}

/*---------------------------------------------------------------------------*/

bool 
CMemorySegment::operator!=( const CMemorySegment& other ) const
{
    if ( m_sizecompare )
    {
        return m_size != other.m_size;
    }
    return m_offset != other.m_offset;     
}

/*---------------------------------------------------------------------------*/

void 
CMemorySegment::SetSize( unsigned long newsize )
{
    m_size = newsize;
}

/*---------------------------------------------------------------------------*/

unsigned long 
CMemorySegment::GetSize( void ) const
{
    return m_size;
}

/*---------------------------------------------------------------------------*/

void 
CMemorySegment::SetOffset( unsigned long newoffset )
{
    m_offset = newoffset;
}

/*---------------------------------------------------------------------------*/

unsigned long 
CMemorySegment::GetOffset( void ) const
{
    return m_offset;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*---------------------------------------------------------------------------*/
