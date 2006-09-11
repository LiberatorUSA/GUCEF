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
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CURLHandlerRegistry.h"

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

MT::CMutex CURLHandlerRegistry::_datalock;
CURLHandlerRegistry* CURLHandlerRegistry::_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CURLHandlerRegistry::CURLHandlerRegistry( void )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CURLHandlerRegistry::CURLHandlerRegistry( const CURLHandlerRegistry& src )
{
        GUCEF_BEGIN;
        /* dummy, do not use */
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CURLHandlerRegistry::~CURLHandlerRegistry()
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CURLHandlerRegistry& 
CURLHandlerRegistry::operator=( const CURLHandlerRegistry& src )
{
        GUCEF_BEGIN;
        /* dummy, do not use */
        GUCEF_END;
        return *this;
}

/*-------------------------------------------------------------------------*/

CURLHandlerRegistry*
CURLHandlerRegistry::Instance( void )
{
        GUCEF_BEGIN;
        _datalock.Lock();
        if ( !_instance )        
        {
                _instance = new CURLHandlerRegistry();
                CHECKMEM( _instance, sizeof(CURLHandlerRegistry) ); 
        }
        _datalock.Unlock();
        GUCEF_END;
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CURLHandlerRegistry::Deinstance( void )
{
        GUCEF_BEGIN;
        _datalock.Lock();
        CHECKMEM( _instance, sizeof(CURLHandlerRegistry) );
        delete _instance;
        _instance = NULL;
        _datalock.Unlock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void 
CURLHandlerRegistry::LockData( void ) const
{                  
        GUCEF_BEGIN;
        _datalock.Lock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void 
CURLHandlerRegistry::UnlockData( void ) const
{
        GUCEF_BEGIN;
        _datalock.Unlock();
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