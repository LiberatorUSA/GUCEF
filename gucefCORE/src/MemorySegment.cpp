/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
