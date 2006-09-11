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

#include "CFPCodecRegistry.h"

#ifndef GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_ESSENTIALS_H
#endif /* GUCEF_ESSENTIALS_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex CFPCodecRegistry::_datalock;
CFPCodecRegistry* CFPCodecRegistry::_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CFPCodecRegistry::CFPCodecRegistry( void )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CFPCodecRegistry::CFPCodecRegistry( const CFPCodecRegistry& src )
{TRACE;
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CFPCodecRegistry::~CFPCodecRegistry()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CFPCodecRegistry& 
CFPCodecRegistry::operator=( const CFPCodecRegistry& src )
{TRACE;
        /* dummy, do not use */
        return *this;
}

/*-------------------------------------------------------------------------*/

CFPCodecRegistry*
CFPCodecRegistry::Instance( void )
{TRACE;
        _datalock.Lock();
        if ( !_instance )        
        {
                _instance = new CFPCodecRegistry();
                CHECKMEM( _instance, sizeof(CFPCodecRegistry) ); 
        }
        _datalock.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CFPCodecRegistry::Deinstance( void )
{TRACE;
        _datalock.Lock();
        CHECKMEM( _instance, sizeof(CFPCodecRegistry) );
        delete _instance;
        _instance = NULL;
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void 
CFPCodecRegistry::LockData( void ) const
{TRACE;
        _datalock.Lock();
}

/*-------------------------------------------------------------------------*/

void 
CFPCodecRegistry::UnlockData( void ) const
{TRACE;
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/