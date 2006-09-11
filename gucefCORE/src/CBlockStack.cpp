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

#include "CBlockStack.h"

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

CBlockStack::CBlockStack( void )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CBlockStack::CBlockStack( UInt32 capacity  ,
                          UInt32 blocksize )
        : _pool( capacity  ,
                 blocksize ) ,
          _itemcount( 0 )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CBlockStack::~CBlockStack()
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CBlockStack::Push( const void *srcdata )
{                               
        GUCEF_BEGIN;        
        if ( _itemcount == _pool.GetCapacity() )
        {
                _pool.SetCapacity( _pool.GetCapacity()+5 );
        }
        _pool.ShiftBlocksUp();
        _pool.SetBlock( 0, srcdata );
        ++_itemcount;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

bool
CBlockStack::Pop( void* datadest )
{       
        GUCEF_BEGIN;        
        if ( _itemcount )
        {
                memcpy( datadest             ,
                        _pool.GetBlock( 0 )  ,
                        _pool.GetBlockSize() );
                _pool.ShiftBlocksDown();
                --_itemcount;
                GUCEF_END;
                return true;
        }
        GUCEF_END;
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CBlockStack::GetTop( void* datadest )
{
        GUCEF_BEGIN;
        if ( _itemcount )
        {
                memcpy( datadest                       ,
                        _pool.GetBlock( _itemcount-1 ) ,
                        _pool.GetBlockSize()           );
                --_itemcount;
                GUCEF_END;
                return true;
        }
        GUCEF_END;
        return false;
}

/*-------------------------------------------------------------------------*/

void 
CBlockStack::SetTop( const void *srcdata )
{
        GUCEF_BEGIN;        
        if ( _itemcount == _pool.GetCapacity() )
        {
                _pool.SetCapacity( _pool.GetCapacity()+5 );
        }
        _pool.SetBlock( _itemcount, srcdata );
        ++_itemcount;
        GUCEF_END;        
}

/*-------------------------------------------------------------------------*/

void* 
CBlockStack::PeekTop( void ) const
{       
        GUCEF_BEGIN;        
        if ( _itemcount )
        {
                GUCEF_END_RET( void*, _pool.GetBlock( _itemcount-1 ) );
        }
        GUCEF_END;
        return NULL;                
}

/*-------------------------------------------------------------------------*/       

void* 
CBlockStack::Peek( void ) const
{
        GUCEF_BEGIN;
        if ( _itemcount )
        {                
                GUCEF_END_RET( void*, _pool.GetBlock( 0 ) );
        }
        GUCEF_END;
        return NULL;                 
}

/*-------------------------------------------------------------------------*/

UInt32
CBlockStack::GetItemCount( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;        
        return _itemcount;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

