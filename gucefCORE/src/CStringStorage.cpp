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

#include "CStringStorage.h"

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

#define CACHEDLIFESPAN  2

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CStringStorage* CStringStorage::_instance = NULL;
MT::CMutex CStringStorage::_mutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SStringData
{
        char* string;
        UInt32 refcount;
        Int32 chache;
};

typedef struct SStringData TStringData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CStringStorage::CStringStorage( void )
{
        GUCEF_BEGIN;
        /* nothing to do here */
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/
 
#pragma warning( push )
#pragma warning( disable : 4100 ) /* unreferenced formal parameter */ 
CStringStorage::CStringStorage( const CStringStorage& src )
{
        GUCEF_BEGIN;
        /* dummy, should never be used */
        GUCEF_END;
}
#pragma warning( pop ) 

/*-------------------------------------------------------------------------*/

#pragma warning( push )
#pragma warning( disable : 4100 ) /* unreferenced formal parameter */ 
CStringStorage&
CStringStorage::operator=( const CStringStorage& src )
{
        GUCEF_BEGIN;
        /* dummy, should never be used */
        GUCEF_END;
        return *this;
}
#pragma warning( pop ) 

/*-------------------------------------------------------------------------*/

CStringStorage*
CStringStorage::Instance( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        if ( !_instance )
        {
                _instance = new CStringStorage();
                CHECKMEM( _instance, sizeof(CStringStorage) );
        }
        _mutex.Unlock();
        GUCEF_END;
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CStringStorage::Deinstance( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        delete _instance;
        _instance = NULL;
        _mutex.Unlock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

UInt32
CStringStorage::RegisterString( const char* str   ,
                                UInt32 length     ,
                                const char** strp )
{
        GUCEF_BEGIN;
        CHECKMEM( this, sizeof(CStringStorage) );
        
        if ( !length ) 
        {
                GUCEF_END;
                return 0;
        }                

        _mutex.Lock();
        TStringData* sdata = new TStringData;
        CHECKMEM( sdata, sizeof(TStringData) );
        sdata->string = new char[ length+1 ];        
        sdata->string[ length ] = 0;
        memcpy( sdata->string, str, length );
        CHECKMEM( sdata->string, length+1 );
        sdata->refcount = 1;
        sdata->chache = 0;
        UInt32 stringid = _strings.AddEntry( sdata )+1;
        *strp = sdata->string;
        _mutex.Unlock();
        GUCEF_END;
        return stringid;
}

/*-------------------------------------------------------------------------*/

UInt32
CStringStorage::RegisterStringCombo( const char* string1 ,
                                     UInt32 length1      ,
                                     const char* string2 ,
                                     UInt32 length2      ,
                                     const char** strp   )
{
        GUCEF_BEGIN;
        CHECKMEM( this, sizeof(CStringStorage) );
        
        _mutex.Lock();
        TStringData* sdata = new TStringData;
        CHECKMEM( sdata, sizeof(TStringData) );
        UInt32 length = length1+length2;
        sdata->string = new char[ length+1 ];
        CHECKMEM( sdata->string, length );
        sdata->string[ length ] = 0;
        memcpy( sdata->string, string1, length1 );
        memcpy( sdata->string+length1, string2, length2 );
        *strp = sdata->string;
        sdata->refcount = 1;
        sdata->chache = 0;
        UInt32 stringid = _strings.AddEntry( sdata )+1;
        _mutex.Unlock();
        GUCEF_END;
        return stringid;
}

/*-------------------------------------------------------------------------*/

void
CStringStorage::RegisterStringCopy( UInt32 stringid )
{
        GUCEF_BEGIN;
        CHECKMEM( this, sizeof(CStringStorage) );
        
        if ( !stringid ) 
        {
                GUCEF_END;
                return;
        }                
        
        _mutex.Lock();
        CHECKMEM( _strings[ stringid-1 ], sizeof(TStringData) );            
        ++( (TStringData*) _strings[ stringid-1 ] )->refcount;
        _mutex.Unlock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CStringStorage::UnregisterString( UInt32 stringid )
{
        GUCEF_BEGIN;
        CHECKMEM( this, sizeof(CStringStorage) );
        
        if ( !stringid ) 
        {
                GUCEF_END;
                return;
        }                
        
        _mutex.Lock();        
        TStringData* sdata = (TStringData*)_strings[ stringid-1 ];
        CHECKMEM( sdata, sizeof(TStringData) );
        sdata->refcount--;
        if ( ( sdata->chache <= 0 ) &&
             ( sdata->refcount == 0 ) )
        {
                CHECKMEM( sdata, sizeof(TStringData) );
                delete []sdata->string;
                delete sdata;
                _strings.SetEntry( stringid-1 ,
                                   NULL       );
        }
        _mutex.Unlock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

/**
 *      Called each event pump update cycle after the events for 
 *      the current cycle have been pumped to all clients.
 *
 *      @param tickcount the tick count when the Update process commenced.
 *      @param deltaticks ticks since the last Update process commenced.
 */        
void 
CStringStorage::Update( UInt32 tickcount  ,
                        UInt32 deltaticks )
{
        GUCEF_BEGIN;
      /*  TStringData* sdata;
        for ( Int32 i=0; i<_strings.GetLast(); ++i )        
        {
                CHECKMEM( _strings[ i ], sizeof(TStringData) );
                sdata = (TStringData*)_strings[ i ];
                if ( sdata )
                {                        
                        if ( ( sdata->chache <= 0 ) &&
                             ( sdata->refcount == 0 ) )
                        {
                                delete []sdata->string;
                                delete sdata;
                                _strings.SetEntry( i    ,
                                                   NULL );                                
                        }
                        else
                        if ( sdata->refcount )
                        {
                                sdata->chache = 0;                        
                        }
                }       
        }   */
        GUCEF_END;             
}

/*-------------------------------------------------------------------------*/

Int32 
CStringStorage::Cache( const CString& str )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        CHECKMEM( _strings[ str.GetID()-1 ], sizeof(TStringData) );
        ///((TStringData*)_strings[ str.GetID()-1 ])->chache++;        
        _mutex.Unlock();
        GUCEF_END_RET( Int32, str.GetID() );
}

/*-------------------------------------------------------------------------*/
        
CString 
CStringStorage::DeCache( Int32 stringid )
{                       
        GUCEF_BEGIN;
        _mutex.Lock();                
        CHECKMEM( _strings[ stringid-1 ], sizeof(TStringData) );
        //TStringData* sdata = (TStringData*)_strings[ stringid-1 ];        
        //--sdata->chache;
        //CString decached( sdata->string );
        _mutex.Unlock();
           CString decached;
        GUCEF_END;   
        return decached;       
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
