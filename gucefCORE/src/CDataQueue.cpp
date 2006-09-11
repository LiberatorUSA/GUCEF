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

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#include "CDataQueue.h"

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

CDataQueue::CDataQueue( void )
        : _queue( 0, sizeof( CDynamicBuffer* ) )       ,
          _freebuffers( 0, sizeof( CDynamicBuffer* ) )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDataQueue::~CDataQueue()
{
        GUCEF_BEGIN;
        CDynamicBuffer* buffer;
        while ( _queue.Pop( &buffer ) ) 
        {
                delete buffer;
        }
        while ( _freebuffers.Pop( &buffer ) ) 
        {
                delete buffer;
        }
        GUCEF_END;                
}
        
/*-------------------------------------------------------------------------*/        

void 
CDataQueue::Set( const void* src ,
                 UInt32 datasize )
{
        GUCEF_BEGIN;
        CDynamicBuffer* buffer;
        if ( !_freebuffers.Pop( &buffer ) )
        {
                // no more free buffers are available
                buffer = new CDynamicBuffer( true );                              
        }
        
        buffer->CopyFrom( datasize, src );        
        _queue.SetTop( &buffer );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/                 
                                 
void 
CDataQueue::Get( const void** dataptr ,
                 UInt32* datasize     )
{
        GUCEF_BEGIN;
        CDynamicBuffer* buffer = static_cast<CDynamicBuffer*>(_queue.Peek());
        
        if ( buffer )
        {
                *dataptr = buffer->GetBufferPtr();
                *datasize = buffer->GetBufferSize();
                
                _freebuffers.Push( &buffer );
                return;                
        }
        
        *dataptr = NULL;
        *datasize = 0;
        GUCEF_END;      
}

/*-------------------------------------------------------------------------*/

void 
CDataQueue::Drop( void )
{
        GUCEF_BEGIN;
        CDynamicBuffer* buffer;
        if ( _queue.Pop( &buffer ) )
        {
                _freebuffers.Push( &buffer );        
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
                  