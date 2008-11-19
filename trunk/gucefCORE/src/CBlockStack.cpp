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

