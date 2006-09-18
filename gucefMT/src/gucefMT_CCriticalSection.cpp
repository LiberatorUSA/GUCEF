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

#ifndef GUCEF_MT_CCRITICALSECTION_H
#include "gucefMT_CCriticalSection.h"
#endif /* GUCEF_MT_CCRITICALSECTION_H ? */

#ifdef GUCEFMT_MSWIN_BUILD
#include <windows.h>
#else
#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#endif /* GUCEF_MT_CMUTEX_H ? */
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CCriticalSection::CCriticalSection( void )
{
        #ifdef GUCEFMT_MSWIN_BUILD
        _data = new CRITICAL_SECTION;
        InitializeCriticalSection( (CRITICAL_SECTION*) _data );
        #else
        _data = new CMutex();
        #endif
}

/*--------------------------------------------------------------------------*/

CCriticalSection::~CCriticalSection()
{
        #ifdef GUCEFMT_MSWIN_BUILD
        DeleteCriticalSection( (CRITICAL_SECTION*) _data );
        delete (CRITICAL_SECTION*) _data;
        #else
        delete (CMutex*) _data;
        #endif
}

/*--------------------------------------------------------------------------*/

void
CCriticalSection::Enter( void )
{
        #ifdef GUCEFMT_MSWIN_BUILD
        EnterCriticalSection( (CRITICAL_SECTION*) _data );
        #else
        ((CMutex*) _data)->Lock();
        #endif
}

/*--------------------------------------------------------------------------*/

void
CCriticalSection::Leave( void )
{
        #ifdef GUCEFMT_MSWIN_BUILD
        LeaveCriticalSection( (CRITICAL_SECTION*) _data );
        #else
        ((CMutex*) _data)->Unlock();
        #endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
