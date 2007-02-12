/*
 *  gucefMT: GUCEF module providing multithreading solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
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
{
        #ifdef GUCEF_MSWIN_BUILD
        _data = new CRITICAL_SECTION;
        InitializeCriticalSection( (CRITICAL_SECTION*) _data );
        #else
        _data = new CMutex();
        #endif
}

/*--------------------------------------------------------------------------*/

CCriticalSection::~CCriticalSection()
{
        #ifdef GUCEF_MSWIN_BUILD
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
        #ifdef GUCEF_MSWIN_BUILD
        EnterCriticalSection( (CRITICAL_SECTION*) _data );
        #else
        ((CMutex*) _data)->Lock();
        #endif
}

/*--------------------------------------------------------------------------*/

void
CCriticalSection::Leave( void )
{
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
