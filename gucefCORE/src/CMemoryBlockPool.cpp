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

#include <string.h>
#include <malloc.h>

#ifndef GUCEF_CORE_CMEMORYBLOCKPOOL_H
#include "CMemoryBlockPool.h"   
#define GUCEF_CORE_CMEMORYBLOCKPOOL_H
#endif /* GUCEF_CORE_CMEMORYBLOCKPOOL_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */  

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

CMemoryBlockPool::CMemoryBlockPool( void )
        : _pool( NULL )   ,
          _capacity( 0 )  ,
          _blocksize( 0 )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CMemoryBlockPool::CMemoryBlockPool( const CMemoryBlockPool& src )
{
        GUCEF_BEGIN;
        *this = src;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CMemoryBlockPool&
CMemoryBlockPool::operator=( const CMemoryBlockPool& src )
{
        GUCEF_BEGIN;
        CHECKMEM( _pool, _capacity*_blocksize );
        _capacity = src._capacity;
        _blocksize = src._blocksize;
        free( _pool );
        _pool = (UInt8*) malloc( _capacity*_blocksize );
        memcpy( _pool, src._pool, _capacity*_blocksize );
        CHECKMEM( _pool, _capacity*_blocksize );
        GUCEF_END;
        return *this;
}

/*-------------------------------------------------------------------------*/

CMemoryBlockPool::CMemoryBlockPool( UInt32 capacity  ,
                                    UInt32 blocksize )
        : _capacity( capacity )  ,
          _blocksize( blocksize )
{
        GUCEF_BEGIN;
        _pool = (UInt8*) malloc( capacity*blocksize );
        CHECKMEM( _pool, capacity*blocksize );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CMemoryBlockPool::~CMemoryBlockPool()
{
        GUCEF_BEGIN;
        CHECKMEM( _pool, _capacity*_blocksize );
        free( _pool );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CMemoryBlockPool::SetBlock( UInt32 blockindex ,
                            const void* dest  ,
                            UInt32 size       )
{
        GUCEF_BEGIN;
        CHECKMEMSEG( _pool, _pool+(blockindex*_blocksize), size );
        memcpy( _pool+(blockindex*_blocksize) ,
                dest                          ,
                size                          );
        GUCEF_END;                
}

/*-------------------------------------------------------------------------*/

void
CMemoryBlockPool::SetBlock( UInt32 blockindex ,
                            const void* dest  )
{
        GUCEF_BEGIN;
        CHECKMEMSEG( _pool, _pool+(blockindex*_blocksize), _blocksize );
        memcpy( _pool+(blockindex*_blocksize) ,
                dest                          ,
                _blocksize                    );
        GUCEF_END;                
}

/*-------------------------------------------------------------------------*/

void*
CMemoryBlockPool::GetBlock( UInt32 blockindex ) const
{
        GUCEF_BEGIN;
        CHECKMEMSEG( _pool, (_pool+(blockindex*_blocksize)), _blocksize );
        GUCEF_END;        
        return (_pool+(blockindex*_blocksize));
}

/*-------------------------------------------------------------------------*/

void
CMemoryBlockPool::SetCapacity( UInt32 capacity )
{
        GUCEF_BEGIN;
        if ( capacity == _capacity )
        {
                GUCEF_END; 
                return;
        }                
        _pool = (UInt8*) realloc( _pool, capacity*_blocksize );
        CHECKMEM( _pool, capacity*_blocksize );
        _capacity = capacity;        
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

UInt32
CMemoryBlockPool::GetCapacity( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _capacity;
}

/*-------------------------------------------------------------------------*/

bool
CMemoryBlockPool::SetBlockSize( UInt32 blocksize ,
                                bool movedata    )
{
        GUCEF_BEGIN;
        if ( blocksize == _blocksize ) 
        {
                GUCEF_END;
                return true;
        }                
        if ( blocksize > _blocksize )
        {                
                if ( movedata )
                {
                        UInt8* pool = _pool, *newpool = (UInt8*) malloc( _capacity*blocksize );
                        CHECKMEM( _pool, _capacity*_blocksize );
                        CHECKMEM( newpool, _capacity*blocksize );
                        for ( UInt32 i=0; i<_capacity; ++i )
                        {
                                memcpy( newpool, pool, _blocksize );
                                newpool += blocksize;
                                pool += _blocksize;
                        }
                        free( _pool );
                        _pool = newpool;                        
                }  
                else
                {
                        _pool = (UInt8*) realloc( _pool, _capacity*blocksize );
                        CHECKMEM( _pool, _capacity*blocksize );
                }
                _blocksize = blocksize;
                GUCEF_END;
                return true;                      
        }
        
        if ( movedata ) 
        {
                GUCEF_END;
                return false;
        }                
        _pool = (UInt8*) realloc( _pool, _capacity*blocksize );
        CHECKMEM( _pool, _capacity*blocksize );
        _blocksize = blocksize;
        GUCEF_END;
        return true;                                
}

/*-------------------------------------------------------------------------*/

UInt32
CMemoryBlockPool::GetBlockSize( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _blocksize;
}

/*-------------------------------------------------------------------------*/

void
CMemoryBlockPool::ShiftBlocksUp( void )
{
        GUCEF_BEGIN;
        if ( _capacity > 1 )
        {
                UInt8* i = (_pool+((_capacity-2)*_blocksize));
                while ( i >= _pool )
                {
                        CHECKMEMSEG( _pool, i, _blocksize );
                        CHECKMEMSEG( _pool, i+_blocksize, _blocksize );
                        memcpy( i+_blocksize ,
                                i            ,
                                _blocksize   );
                        i -= _blocksize;
                }
        }
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CMemoryBlockPool::ShiftBlocksDown( void )
{
        GUCEF_BEGIN;
        if ( _capacity > 1 )
        {
                UInt8* i = _pool+((_capacity-2)*_blocksize);
                while ( i >= _pool )
                {
                        CHECKMEMSEG( _pool, i, _blocksize );
                        CHECKMEMSEG( _pool, i+_blocksize, _blocksize );                
                        memcpy( i            ,
                                i+_blocksize ,
                                _blocksize   );
                        i -= _blocksize;
                }
        }
        GUCEF_END;        
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
 