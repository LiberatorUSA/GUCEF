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

#ifndef GUCEF_MT_THREADSTATUS_H
#define GUCEF_MT_THREADSTATUS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"             /* Simple types used */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H */

#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"             /* often used gucef macros */
#define GUCEF_MT_GUCEFMT_MACROS_H
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace MT {
#endif 

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum EThreadStatus 
{
    THREADSTATUS_UNDEFINED        = 0,    /**< not a real task status but rather an initialization value */

    THREADSTATUS_CREATION_FAILED     ,    /**< there was an attempt to start the thread but the related O/S functions reported an error and no thread was launched, there likely will not be a threadId */
    THREADSTATUS_STARTUP_FAILED      ,    /**< there was an attempt to start the thread but the thread activation handler reported an error and no further processing occured */
    THREADSTATUS_RUNNING             ,    /**< the thread has been successfully launched and its presumably performing work */
    THREADSTATUS_PAUSED              ,    /**< the thread is currently paused, startup had completed and work had started but subsequently the thread was put on pause */
    THREADSTATUS_RESUMED             ,    /**< same as 'THREADSTATUS_RUNNING' except that the task had been paused during its run cycle */
    THREADSTATUS_STOPPED             ,    /**< the thread has been stopped on external request instead of finishing a finite length task */
    THREADSTATUS_FINISHED            ,    /**< the thread has stopped because it finished a finite length task */
    THREADSTATUS_KILLED              ,    /**< the thread has ended because it was killed */
    THREADSTATUS_DEADLOCKED               /**< the thread is detected to be in a deadlocked state */
};
typedef enum EThreadStatus TThreadStatus;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ Name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C const char* 
ThreadStatusToThreadStatusString( TThreadStatus threadStatus );

/*-------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C TThreadStatus 
ThreadStatusStringToThreadStatus( const char* threadStatusStr );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace MT */
}; /* namespace GUCEF */
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_THREADSTATUS_H ? */
