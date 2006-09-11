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
class EXPORT_CPP CMemorySegment
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