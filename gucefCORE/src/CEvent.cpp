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

#ifndef GUCEF_CORE_CEVENTTYPEREGISTRY_H
#include "CEventTypeRegistry.h"
#define GUCEF_CORE_CEVENTTYPEREGISTRY_H
#endif /* GUCEF_CORE_CEVENTTYPEREGISTRY_H ? */

#ifndef GUCEF_CORE_CEVENTDATAMEMORYPOOL_H
#include "CEventDataMemoryPool.h"
#define GUCEF_CORE_CEVENTDATAMEMORYPOOL_H
#endif /* GUCEF_CORE_CEVENTDATAMEMORYPOOL_H ? */

#include "CEvent.h"

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CEvent::CEvent( void )
        : _storagelocked( false ) ,
          _eventtype( 0 )         ,
          _eventdataid( 0 )       ,
          _pumpclientid( 0 ) 
{
        /*
         *      Nothing to do here
         */
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( UInt32 eventtype )
        : _storagelocked( false ) ,
          _eventtype( eventtype ) ,
          _eventdataid( 0 )       ,
          _pumpclientid( 0 ) 
{
        GUCEF_BEGIN;
        _eventdataid = CEventDataMemoryPool::Instance()->GetBlockRefrence();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CEvent::CEvent( const CEvent& src )
        : _pumpclientid( src._pumpclientid )   ,
          _eventtype( src._eventtype )         ,
          _eventdataid( src._eventdataid )     ,
          _storagelocked( true )
{
        GUCEF_BEGIN;
        CEventDataMemoryPool::Instance()->CopyBlockRefrence( _eventdataid );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CEvent::~CEvent()
{
        GUCEF_BEGIN;
        CEventDataMemoryPool::Instance()->UnrefrenceBlock( _eventdataid );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CEvent&
CEvent::operator=( const CEvent& src )
{
        GUCEF_BEGIN;
        CEventDataMemoryPool* edmp = CEventDataMemoryPool::Instance();
        edmp->UnrefrenceBlock( _eventdataid );
        _pumpclientid = src._pumpclientid;
        _eventtype = src._eventtype;
        _eventdataid = src._eventdataid;
        _storagelocked = true;
        edmp->CopyBlockRefrence( _eventdataid );
        GUCEF_END;
        return *this;
}

/*-------------------------------------------------------------------------*/

void
CEvent::SetData( const void* src , 
                 UInt32 size     )
{
        GUCEF_BEGIN;
        if ( _storagelocked ) 
        {
                GUCEF_END;
                return; 
        }                
        CEventDataMemoryPool::Instance()->SetData( _eventdataid ,
                                                   src          ,
                                                   size         ); 
        _storagelocked = true;
        GUCEF_END;                                                                 
}

/*-------------------------------------------------------------------------*/

void
CEvent::GetData( void* dest  ,
                 UInt32 size ) const
{        
        GUCEF_BEGIN;
        CEventDataMemoryPool::Instance()->GetData( _eventdataid ,
                                                   dest         ,
                                                   size         );
        GUCEF_END;                                                   
}

/*-------------------------------------------------------------------------*/

bool
CEvent::GetIsDataLocked( void )
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _storagelocked;
}

/*-------------------------------------------------------------------------*/

UInt32
CEvent::GetPumpClientID( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _pumpclientid;
}

/*-------------------------------------------------------------------------*/

void
CEvent::SetPumpClientID( UInt32 clientid )
{
        GUCEF_BEGIN;
        _pumpclientid = clientid;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

UInt32
CEvent::GetEventType( void ) const
{                       
        GUCEF_BEGIN;
        GUCEF_END;
        return _eventtype;
}

/*-------------------------------------------------------------------------*/

CString
CEvent::GetTypeName( void )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( CString, CEventTypeRegistry::Instance()->GetTypeName( _eventtype ) );
}

/*-------------------------------------------------------------------------*/

UInt32
CEvent::GetMaxDataSize( void )
{               
        GUCEF_BEGIN;
        GUCEF_END_RET( UInt32, CEventTypeRegistry::Instance()->GetMaxDataSize( _eventtype ) );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
