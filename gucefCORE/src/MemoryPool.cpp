/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#include <stdlib.h> /* needed for realloc() & free() */

#include "MemorySegment.h"
#include "MemoryPool.h"

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

CMemoryPool::CMemoryPool( void )
{   

}

/*-------------------------------------------------------------------------*/

CMemoryPool::CMemoryPool( const CMemoryPool& src )
{

}

/*-------------------------------------------------------------------------*/

CMemoryPool::~CMemoryPool()
{

}

/*-------------------------------------------------------------------------*/

CMemoryPool&
CMemoryPool::operator=( const CMemoryPool& src )
{
    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void* 
CMemoryPool::AllocateBestFit( unsigned long bytesize )
{
    return Allocate( bytesize );
}

/*-------------------------------------------------------------------------*/

void* 
CMemoryPool::AllocateWorstFit( unsigned long bytesize )
{
    return Allocate( bytesize );    
}

/*-------------------------------------------------------------------------*/

void* 
CMemoryPool::AllocateFirstFit( unsigned long bytesize )
{
    return Allocate( bytesize );
}

/*-------------------------------------------------------------------------*/

void 
CMemoryPool::DeallocateLogicalUnit( void* ptr )
{
    Deallocate( ptr );
}

/*-------------------------------------------------------------------------*/

void 
CMemoryPool::DeallocateAndMerge( void* ptr )
{
    Deallocate( ptr );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
