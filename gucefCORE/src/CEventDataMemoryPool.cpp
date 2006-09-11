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

#ifndef GUCEF_CORE_CMEMORYBLOCKPOOL_H
#include "CMemoryBlockPool.h"   
#define GUCEF_CORE_CMEMORYBLOCKPOOL_H
#endif /* GUCEF_CORE_CMEMORYBLOCKPOOL_H ? */

#ifndef GUCEF_CORE_CEVENTTYPEREGISTRY_H
#include "CEventTypeRegistry.h"
#define GUCEF_CORE_CEVENTTYPEREGISTRY_H
#endif /* GUCEF_CORE_CEVENTTYPEREGISTRY_H ? */

#ifndef GUCEF_CORE_CEVENTDATAMEMORYPOOL_H
#include "CEventDataMemoryPool.h"
#define GUCEF_CORE_CEVENTDATAMEMORYPOOL_H
#endif /* GUCEF_CORE_CEVENTDATAMEMORYPOOL_H ? */

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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define INITIAL_POOL_SIZE       50

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventDataMemoryPool* CEventDataMemoryPool::_instance = NULL;
MT::CMutex CEventDataMemoryPool::_mutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventDataMemoryPool::CEventDataMemoryPool( void )
        : _pool( INITIAL_POOL_SIZE                                       ,
                 CEventTypeRegistry::Instance()->GetLargestDataBlock() ) ,
          _usecount( 0 )
{
        GUCEF_BEGIN;
        _blockcounters = new UInt16[ INITIAL_POOL_SIZE*_pool.GetBlockSize() ];
        memset( _blockcounters                         ,
                0                                      ,
                INITIAL_POOL_SIZE*_pool.GetBlockSize() );
                
        CHECKMEM( _blockcounters, sizeof(UInt16) * (INITIAL_POOL_SIZE*_pool.GetBlockSize()) );                
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CEventDataMemoryPool::~CEventDataMemoryPool()
{
        GUCEF_BEGIN;
        delete []_blockcounters;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CEventDataMemoryPool*
CEventDataMemoryPool::Instance( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        if ( !_instance )
        {
                _instance = new CEventDataMemoryPool();
                CHECKMEM( _instance, sizeof(CEventDataMemoryPool) );
        }
        _mutex.Unlock();
        GUCEF_END;
        return _instance;
}

/*-------------------------------------------------------------------------*/

UInt32
CEventDataMemoryPool::GetBlockRefrence( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        UInt32 cap = _pool.GetCapacity();
        if ( _usecount )
        {
                for ( UInt32 i=0; i<cap; ++i )
                {
                        if ( !_blockcounters[ i ] )
                        {
                                ++_usecount;
                                ++_blockcounters[ i ];
                                _mutex.Unlock();
                                GUCEF_END;
                                return i+1;
                        }
                }
        }                
        _mutex.Unlock();
        GUCEF_END;
        return 1;
}

/*-------------------------------------------------------------------------*/

void
CEventDataMemoryPool::CopyBlockRefrence( UInt32 blockid )
{
        GUCEF_BEGIN;
        if ( blockid )
        {
                _mutex.Lock();
                ++_blockcounters[ blockid-1 ];
                _mutex.Unlock();
        }                
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CEventDataMemoryPool::UnrefrenceBlock( UInt32 blockid )
{
        GUCEF_BEGIN;
        if ( blockid )
        {
                _mutex.Lock();
                --_blockcounters[ blockid-1 ];
                if ( !_blockcounters[ blockid-1 ] ) 
                {
                        --_usecount;
                }
                _mutex.Unlock();
        }                
        GUCEF_END;
}                                                

/*-------------------------------------------------------------------------*/

void 
CEventDataMemoryPool::SetData( UInt32 blockid  ,
                               const void* src ,
                               UInt32 size     )
{
        GUCEF_BEGIN;
        if ( blockid )
        {
                _mutex.Lock();
                _pool.SetBlock( blockid-1 ,
                                src       ,
                                size      );
                _mutex.Unlock();
        }                
        GUCEF_END;
}  

/*-------------------------------------------------------------------------*/                             

void 
CEventDataMemoryPool::GetData( UInt32 blockid ,
                               void* dest     ,
                               UInt32 size    )
{
        GUCEF_BEGIN;
        if ( blockid )
        {
                _mutex.Lock();                
                memcpy( dest                        ,
                        _pool.GetBlock( blockid-1 ) ,
                        size                        );
                _mutex.Unlock();
        }
        GUCEF_END;                
}                               

/*-------------------------------------------------------------------------*/

void
CEventDataMemoryPool::SetBlockSize( UInt32 blocksize )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        _pool.SetBlockSize( blocksize , 
                            true      );
        _mutex.Unlock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

UInt32
CEventDataMemoryPool::GetBlockSize( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        UInt32 bs = _pool.GetBlockSize();
        _mutex.Unlock();
        GUCEF_END;
        return bs;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/




