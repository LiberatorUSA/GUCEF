/*
 *  gucefMT: GUCEF module providing multithreading solutions
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CCRITICALSECTION_H
#include "gucefMT_CCriticalSection.h"
#endif /* GUCEF_MT_CCRITICALSECTION_H ? */

#ifdef GUCEF_MSWIN_BUILD
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
{GUCEF_TRACE;

        #ifdef GUCEF_MSWIN_BUILD
        _data = GUCEF_NEW CRITICAL_SECTION;
        InitializeCriticalSection( (CRITICAL_SECTION*) _data );
        #else
        _data = GUCEF_NEW CMutex();
        #endif
}

/*--------------------------------------------------------------------------*/

CCriticalSection::~CCriticalSection()
{GUCEF_TRACE;

        #ifdef GUCEF_MSWIN_BUILD
        DeleteCriticalSection( (CRITICAL_SECTION*) _data );
        GUCEF_DELETE (CRITICAL_SECTION*) _data;
        #else
        GUCEF_DELETE (CMutex*) _data;
        #endif
}

/*--------------------------------------------------------------------------*/

void
CCriticalSection::Enter( void )
{GUCEF_TRACE;

        #ifdef GUCEF_MSWIN_BUILD
        EnterCriticalSection( (CRITICAL_SECTION*) _data );
        #else
        ((CMutex*) _data)->Lock();
        #endif
}

/*--------------------------------------------------------------------------*/

void
CCriticalSection::Leave( void )
{GUCEF_TRACE;

        #ifdef GUCEF_MSWIN_BUILD
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
