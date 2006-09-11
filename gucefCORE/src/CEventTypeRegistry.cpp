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

#ifndef GUCEF_CORE_CEVENTDATAMEMORYPOOL_H
#include "CEventDataMemoryPool.h"
#define GUCEF_CORE_CEVENTDATAMEMORYPOOL_H
#endif /* GUCEF_CORE_CEVENTDATAMEMORYPOOL_H ? */

#include "CEventTypeRegistry.h"

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventTypeRegistry* CEventTypeRegistry::_instance = NULL;
MT::CMutex CEventTypeRegistry::_mutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SETypeData
{
        UInt32 mdsize;
        CString name;        
};

typedef struct SETypeData TETypeData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventTypeRegistry::CEventTypeRegistry( void )
        : _mdsize( 0 )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CEventTypeRegistry*
CEventTypeRegistry::Instance( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        if ( !_instance )
        {
                _instance = new CEventTypeRegistry();
                CHECKMEM( _instance, sizeof(CEventTypeRegistry) );
        }
        _mutex.Unlock();
        GUCEF_END;
        return _instance;
}

/*-------------------------------------------------------------------------*/

UInt32
CEventTypeRegistry::RegisterType( const CString& name ,
                                  UInt32 maxdatasize  )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        TETypeData* td;
        for ( UInt32 i=0; i<_etypes.GetCount(); ++i )
        {
                td = (TETypeData*)_etypes[ i ];
                CHECKMEM( td, sizeof(TETypeData) );
                if ( td->name == name )
                {
                        DEBUGOUTPUT( "Event type already registered" );
                        _mutex.Unlock();
                        GUCEF_END;
                        return i+1;
                }
        }
        if ( maxdatasize > _mdsize )
        {
                DEBUGOUTPUT( "Increasing block size of the event data memory pool" );
                
                _mdsize = maxdatasize;
                CEventDataMemoryPool::Instance()->SetBlockSize( _mdsize );
        }
        td = new TETypeData;
        td->name = name;
        td->mdsize = maxdatasize;
        CHECKMEM( td, sizeof(TETypeData) );
        UInt32 eventtype = _etypes.AddEntry( td )+1;
        _mutex.Unlock();
        GUCEF_END;
        return eventtype;
}

/*-------------------------------------------------------------------------*/

UInt32
CEventTypeRegistry::GetMaxDataSize( UInt32 eventtype ) const
{
        GUCEF_BEGIN;
        if ( eventtype )
        {
                _mutex.Lock();
                UInt32 mdsize = ((TETypeData*) _etypes[ eventtype-1 ])->mdsize;
                _mutex.Unlock();
                GUCEF_END;
                return mdsize;
        }
        GUCEF_END;
        return 0;                
}

/*-------------------------------------------------------------------------*/

UInt32
CEventTypeRegistry::GetLargestDataBlock( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _mdsize;
}

/*-------------------------------------------------------------------------*/

UInt32
CEventTypeRegistry::GetType( const CString& name ) const
{
        GUCEF_BEGIN;
        _mutex.Lock();
        for ( Int32 i=0; i<=_etypes.GetLast(); ++i )
        {
                CHECKMEM( _etypes[ i ], sizeof(TETypeData) );
                if ( ((TETypeData*) _etypes[ i ])->name == name )
                {
                        _mutex.Unlock();
                        return i+1;
                }
        }
        _mutex.Unlock();
        GUCEF_END;
        return 0;
}

/*-------------------------------------------------------------------------*/

CString
CEventTypeRegistry::GetTypeName( UInt32 eventtype ) const
{
        GUCEF_BEGIN;
        if ( eventtype )
        {
                _mutex.Lock();
                CHECKMEM( _etypes[ eventtype-1 ], sizeof(TETypeData) );
                CString name = ((TETypeData*) _etypes[ eventtype-1 ])->name;
                _mutex.Unlock();
                GUCEF_END;
                return name;    
        }
        GUCEF_END;
        return 0;                    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
 