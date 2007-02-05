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

/*
 *      This is the main include header for the gucefMT library.
 *      It is part of a framework called GUCEF.
 *      All classes in this library have to do with threading and
 *      threadsafety.
 */

#ifndef GUCEF_MT_GUCEFMT_H
#define GUCEF_MT_GUCEFMT_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration      
 */
#ifndef GUCEF_MT_GUCEFMT_CONFIG_H
#include "gucefMT_config.h"
#define GUCEF_MT_GUCEFMT_CONFIG_H
#endif /* GUCEF_MT_GUCEFMT_CONFIG_H ? */

#ifndef GUCEF_MT_BASICMACROS_H
#include "gucefMT_basicMacros.h"
#define GUCEF_MT_BASICMACROS_H
#endif /* GUCEF_MT_BASICMACROS_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"
#define GUCEF_MT_GUCEFMT_MACROS_H
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */

/*
 *      Simple types
 */
#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

/*
 *      C implementation of a readers writers lock.
 */
#ifndef GUCEF_MT_DVRWLOCK_H
#include "gucefMT_dvrwlock.h"
#define GUCEF_MT_DVRWLOCK_H
#endif /* GUCEF_MT_DVRWLOCK_H ? */

/**
 *      Wrapper functions for threading related O/S functionality
 */
#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

/*
 *      C implementation of a cross-platform mutex wrapper
 */
#ifndef GUCEF_MT_MUTEX_H
#include "gucefMT_mutex.h"
#define GUCEF_MT_MUTEX_H
#endif /* GUCEF_MT_MUTEX_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

/*
 *      class that implements a recursive mutex
 *      supports mswindows and linux
 */
#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

/*
 *      implementation of a thread mailbox.
 *      basicly just a tread-safe stack for event data 
 */
#ifndef GUCEF_MT_CTMAILBOX_H
#include "gucefMT_CTMailBox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

/*
 *      Wrapper class for my C implementation of a readers/writers lock
 */
#ifndef GUCEF_MT_CREADWRITELOCK_H
#include "gucefMT_CReadWriteLock.h"
#define GUCEF_MT_CREADWRITELOCK_H
#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

/*
 *      Wrapper class for SDL semaphore's.
 *      The usage of this class allows you to forgo having to type the same
 *      tests and allocation/de-allocation code all the time.
 */
#ifndef GUCEF_MT_CSEMAPHORE_H
#include "gucefMT_CSemaphore.h"
#define GUCEF_MT_CSEMAPHORE_H
#endif /* GUCEF_MT_CSEMAPHORE_H ? */

/*
 *      O/S Wrapper class for a critical section.
 *
 *      A critical section object provides synchronization similar to that
 *      provided by a mutex object, except that a critical section can be used
 *      only by the threads of a single process. Event, mutex, and semaphore
 *      objects can also be used in a single-process application, but critical
 *      section objects provide a slightly faster, more efficient mechanism
 *      for mutual-exclusion synchronization (a processor-specific test and set
 *      instruction). Like a mutex object, a critical section object can be
 *      owned by only one thread at a time, which makes it useful for protecting
 *      a shared resource from simultaneous access. There is no guarantee about
 *      the order that threads obtain ownership of the critical section,
 *      however, the system is fair to all threads. Unlike a mutex object, there
 *      is no way to tell whether a critical section has been abandoned.
 *
 */
#ifndef GUCEF_MT_CCRITICALSECTION_H
#include "gucefMT_CCriticalSection.h"
#define GUCEF_MT_CCRITICALSECTION_H
#endif /* GUCEF_MT_CCRITICALSECTION_H ? */

/*
 *      Base class for an active object.
 *      Independant activity is implemented using threads.
 */
#ifndef GUCEF_MT_CACTIVEOBJECT_H
#include "gucefMT_CActiveObject.h"
#define GUCEF_MT_CACTIVEOBJECT_H
#endif /* GUCEF_MT_CACTIVEOBJECT_H ? */

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEFMT_EXPORT_C const char* GUCEF_MT_ModuleCompileTimeStamp( void );

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_GUCEFMT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-03-2004 :
       - Made the initial version of the library header. The classes here
         where originally part of one large GUCEF codebase without segmentation.

-----------------------------------------------------------------------------*/



