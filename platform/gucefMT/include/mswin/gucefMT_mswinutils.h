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

#ifndef GUCEF_MT_MSWINUTILS_H
#define GUCEF_MT_MSWINUTILS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_MACROS_H
#include "gucefMT_macros.h"  /* module config macros */
#define GUCEF_MT_MACROS_H
#endif /* GUCEF_MT_MACROS_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define WIN32_NT_IS_SUCCESS_TYPE( value )   ( value >= 0 && value <= 0x3FFFFFFF ) 
#define WIN32_NT_IS_INFO_TYPE( value )      ( value >= 0x40000000 && value <= 0x7FFFFFFF ) 
#define WIN32_NT_SUCCESS( value )           ( WIN32_NT_IS_SUCCESS_TYPE( value ) || WIN32_NT_IS_INFO_TYPE( value ) )

#define NTSTATUS_SUCCESS                    0x00000000L
#define NTSTATUS_NOT_SUPPORTED              0xC00000BBL

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef LONG    NTSTATUS;

enum WIN32_THREAD_INFORMATION_CLASS 
{
    ThreadBasicInformation          = 0,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending,
    ThreadHideFromDebugger
}; 
typedef enum WIN32_THREAD_INFORMATION_CLASS      WIN32_THREAD_INFORMATION_CLASS;

/*-------------------------------------------------------------------------*/

typedef struct {
  HANDLE UniqueProcess;
  HANDLE UniqueThread;
} WIN32_CLIENT_ID;

typedef LONG       WIN32_KPRIORITY;

struct WIN32_THREAD_BASIC_INFORMATION 
{
    NTSTATUS                ExitStatus;
    PVOID                   TebBaseAddress;
    WIN32_CLIENT_ID         ClientId;
    KAFFINITY               AffinityMask;
    WIN32_KPRIORITY         Priority;
    WIN32_KPRIORITY         BasePriority;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*-------------------------------------------------------------------------*/

/**
 *  The Windows NT kernel has an unofficial function called NtQueryInformationThread
 *  This provides some information regarding threads that is hard/impossible to access otherwise
 *  Since this is not officially a public function we have our own TryNtQueryInformationThread version
 *  which will attempt to use said function if it exists / can be accessed.
 *  
 *  Use WIN32_NT_SUCCESS to evaluate the return value
 *  Returns NTSTATUS_NOT_SUPPORTED if the function could not be dynamicallly linked and is thus unsupported
 */
GUCEF_MT_PUBLIC_C LONG 
TryNtQueryInformationThread( HANDLE ThreadHandle                                   ,
                             WIN32_THREAD_INFORMATION_CLASS ThreadInformationClass ,
                             void* ThreadInformation                               ,
                             ULONG ThreadInformationLength                         ,
                             PULONG ReturnLength                                   );

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */

#endif /* GUCEF_MT_MSWINUTILS_H ? */

