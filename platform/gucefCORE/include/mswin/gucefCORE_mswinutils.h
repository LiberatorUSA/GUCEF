/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_CORE_MSWINUTILS_H
#define GUCEF_CORE_MSWINUTILS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* module config macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

#ifndef GUCEF_MT_MSWINUTILS_H
#include "gucefMT_mswinutils.h"
#define GUCEF_MT_MSWINUTILS_H
#endif /* GUCEF_MT_MSWINUTILS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace CORE {
#endif   /* __cplusplus */

#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef struct _CLIENT_ID
{
     PVOID UniqueProcess;
     PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

/*-------------------------------------------------------------------------*/

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation = 0,
    ProcessDebugPort = 7,
    ProcessWow64Information = 26,
    ProcessImageFileName = 27,
    ProcessBreakOnTermination = 29
} PROCESSINFOCLASS;

/*-------------------------------------------------------------------------*/

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

/*-------------------------------------------------------------------------*/

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    BYTE Reserved1[16];
    PVOID Reserved2[10];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

/*-------------------------------------------------------------------------*/

typedef
VOID
(NTAPI *PPS_POST_PROCESS_INIT_ROUTINE) (
    VOID
    );

/*-------------------------------------------------------------------------*/

typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

/*-------------------------------------------------------------------------*/

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID Reserved4[3];
    PVOID AtlThunkSListPtr;
    PVOID Reserved5;
    ULONG Reserved6;
    PVOID Reserved7;
    ULONG Reserved8;
    ULONG AtlThunkSListPtr32;
    PVOID Reserved9[45];
    BYTE Reserved10[96];
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
    BYTE Reserved11[128];
    PVOID Reserved12[1];
    ULONG SessionId;
} PEB, *PPEB;

/*-------------------------------------------------------------------------*/

typedef struct _PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

/*-------------------------------------------------------------------------*/

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

/*-------------------------------------------------------------------------*/

/**
 *  Windows implementation of the ProcessId structure
 */
struct SProcessId
{
    DWORD pid;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/** 
 *  Returns the handle of the current module the code is compiled in.
 *  Nowadays in Win32 this means the same as getting the HINSTANCE for the
 *  application. They are only different in 16-bit windows.
 */
GUCEF_CORE_PUBLIC_C HMODULE
GetCurrentModuleHandle( void );

/*-------------------------------------------------------------------------*/

/**
 *  Function to clear the console window.
 *  This is not always available on all windowed platforms thus 
 *  we provide a version here
 */
GUCEF_CORE_PUBLIC_C void 
cls( void );

/*-------------------------------------------------------------------------*/

/**
 *  The Windows NT kernel has a function called NtOpenProcess
 *  Since we dont want to hard link against ntdll with all the issues that creates (dll hell)
 *  we have our own TryNtOpenProcess version which will attempt to use said function if it exists / can be accessed.
 *  
 *  Use WIN32_NT_SUCCESS to evaluate the return value
 *  Returns NTSTATUS_NOT_SUPPORTED if the function could not be dynamicallly linked and is thus unsupported
 */
GUCEF_CORE_PUBLIC_C NTSTATUS 
TryNtOpenProcess( PHANDLE ProcessHandle               ,
                  ACCESS_MASK DesiredAccess           ,
                  POBJECT_ATTRIBUTES ObjectAttributes ,
                  PCLIENT_ID ClientId                 );

/*-------------------------------------------------------------------------*/

/**
 *  The Windows NT kernel has an unofficial function called NtQueryInformationProcess
 *  This provides some access to process information that is hard/impossible to access otherwise
 *  Since this is not officially a public function we have our own NtQueryInformationProcess version
 *  which will attempt to use said function if it exists / can be accessed.
 *  
 *  Use WIN32_NT_SUCCESS to evaluate the return value
 *  Returns NTSTATUS_NOT_SUPPORTED if the function could not be dynamicallly linked and is thus unsupported
 */
GUCEF_CORE_PUBLIC_C NTSTATUS
TryNtQueryInformationProcess( HANDLE ProcessHandle                     ,
                              PROCESSINFOCLASS ProcessInformationClass ,
                              PVOID ProcessInformation                 ,
                              ULONG ProcessInformationLength           ,
                              PULONG ReturnLength                      );

/*-------------------------------------------------------------------------*/

/**
 *  The Windows NT kernel has an unofficial function called NtReadVirtualMemory
 *  This provides some access to process information that is hard/impossible to access otherwise
 *  Since this is not officially a public function we have our own TryNtReadVirtualMemory version
 *  which will attempt to use said function if it exists / can be accessed.
 *  
 *  Use WIN32_NT_SUCCESS to evaluate the return value
 *  Returns NTSTATUS_NOT_SUPPORTED if the function could not be dynamicallly linked and is thus unsupported
 */
GUCEF_CORE_PUBLIC_C NTSTATUS 
TryNtReadVirtualMemory( HANDLE ProcessHandle       ,
                        PVOID BaseAddress          ,
                        PVOID Buffer               ,
                        SIZE_T NumberOfBytesToRead ,
                        PSIZE_T NumberOfBytesRead  );

/*-------------------------------------------------------------------------*/

#ifndef RtlOffsetToPointer
  #define RtlOffsetToPointer( B,O )  ( (PCHAR)( ((PCHAR)(B)) + ((ULONG_PTR)(O)) ) )
#endif


/*-------------------------------------------------------------------------*/

/**
 *  From ntdef.h
 *  To avoid including that header everywhere causing issues
 */
#ifndef InitializeObjectAttributes
    #define InitializeObjectAttributes( p, n, a, r, s ) {   \
        (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
        (p)->RootDirectory = r;                             \
        (p)->Attributes = a;                                \
        (p)->ObjectName = n;                                \
        (p)->SecurityDescriptor = s;                        \
        (p)->SecurityQualityOfService = NULL;               \
        }
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */

#endif /* GUCEF_CORE_MSWINUTILS_H ? */

