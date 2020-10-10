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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* build configuration */
#define GUCEFCORE_MACROS_H
#endif /* GUCEFCORE_MACROS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

  /* Do not use WIN32_LEAN_AND_MEAN because it will remove timeBeginPeriod() etc. */
  #undef  WIN32_LEAN_AND_MEAN
  #include <windows.h>                /* Windows API */
  #include <psapi.h>
  #undef min
  #undef max
  #define MAX_DIR_LENGTH MAX_PATH

#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

  #include <sys/times.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <limits.h>                 /* Linux OS limits */
  #include <dlfcn.h>                  /* dynamic linking utilities */
  #include <dirent.h>
  #include <unistd.h>
  #define MAX_DIR_LENGTH PATH_MAX

#endif

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"               /* needed for str to int */
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#include "DVOSWRAP.h"               /* O/S Wrapping function prototypes*/

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace CORE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SProcCpuDataPoint
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    FILETIME userTime;
    FILETIME kernelTime;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    pid_t pid;
    UInt64 procUserModeJiffies;
    UInt64 procKernelModeJiffies;
    UInt64 globalJiffies;
    #else
    int foobar_NotSupported;
    #endif
};

struct SProcessId
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    DWORD pid;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    pid_t pid;
    #else
    int foobar_NotSupported;
    #endif
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void*
LoadModuleDynamicly( const char* filename )
{GUCEF_TRACE;

    char* fName = (char*) filename;
    const char* fileExt = Extract_File_Ext( filename );
    void* modulePtr = NULL;

    /*
     *  If no module extension was given we will add the O/S default
     */
    if ( fileExt == NULL )
    {
        UInt32 sLen = (UInt32) strlen( filename );
        fName = (char*) malloc( sLen + 7 );
        memcpy( fName, filename, sLen );

        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
        memcpy( fName+sLen, ".dll\0", 5 );
        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
        memcpy( fName+sLen, ".so\0", 4 );
        #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_APPLE )
        memcpy( fName+sLen, ".dylib\0", 7 );
        #endif
    }

    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    modulePtr = (void*) dlopen( fName, RTLD_NOW );
    if ( NULL == modulePtr )
    {
        /*
         *  It is possible the load failed due to missing "lib" prefix on linux/android.
         *  Check for this and compensate as needed
         */
        char hasLibPrefix = 0;
        const char* fileOnly = Extract_Filename( fName );
        UInt32 sLength = strlen( fileOnly );
        //if ( sLength >= 3 )
        //{
        //    if ( 0 == memcmp( "lib", fileOnly, 3 ) )
        //    {
        //        hasLibPrefix = 1;
        //    }
        //}

        //if ( 0 == hasLibPrefix )
        {
            /*
             *  No previous "lib" prefix was found, we will add one and try to load again
             */
            UInt32 fNameLen = strlen( fName );
            char* newFilePath = (char*) malloc( fNameLen+3+1 );
            memcpy( newFilePath, fName, fNameLen-sLength );
            memcpy( newFilePath+(fNameLen-sLength), "lib", 3 );
            memcpy( newFilePath+(fNameLen-sLength)+3, fileOnly, sLength+1 );

            modulePtr = (void*) dlopen( newFilePath, RTLD_NOW );

            free( newFilePath );
        }
    }

    if ( NULL == modulePtr )
    {
        GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "LoadModuleDynamicly() reports error: " + CString( dlerror() ) );
    }

    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    modulePtr = (void*) LoadLibrary( fName );

    #endif

    if ( fileExt == NULL )
    {
        free( fName );
    }

    return modulePtr;
}

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void*
GetModulePointer( const char* moduleName )
{GUCEF_TRACE;

    // If no module name is passed we get the pointer to the main process module

    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    // On linux the reference count is always incremented so we must decrement again right away to get
    // the same behaviour as the windows version
    void* modulePtr = (void*) dlopen( moduleName, RTLD_NOW );
    dlclose( modulePtr );
    return modulePtr;

    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return (void*) GetModuleHandleA( moduleName );

    #else
    #error Unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

void
UnloadModuleDynamicly( void *sohandle )
{GUCEF_TRACE;

    if ( NULL == sohandle ) return;

    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    dlclose( sohandle );

    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    FreeLibrary( (HMODULE)sohandle );

    #else
    #error Unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

TAnyPointer
GetFunctionAddress( void *sohandle           ,
                    const char* functionname ,
                    UInt32 parambytes        )
{GUCEF_TRACE;

    /*
     *      Calling Convention      Internal*       MSVC DLL (w/ DEF)       MSVC DLL (dllexport)  	DMC DLL         MinGW DLL       BCC DLL
     *      __stdcall               _Function@n  	Function                _Function@n             _Function@n     Function@n      Function
     *      __cdecl                 _Function       Function                Function                Function        Function        _Function
     */
    TAnyPointer fptr;
    if ( NULL == sohandle )
    {
        fptr.funcPtr = 0;
        return fptr;
    }

    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    fptr.objPtr = dlsym( sohandle     ,
                         functionname );
    return fptr;

    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    /*
     *      First we try a normal load using the given
     *      functionname.
     *
     *      Type: Function
     */
    fptr.funcPtr = (TDefaultFuncPtr) GetProcAddress( (HMODULE)sohandle ,
                                                     functionname      );
    if ( fptr.funcPtr == NULL )
    {
        char buffer[ 1024 ];
        UInt32 len = (UInt32)strlen( functionname );

        /*
         *      Maybe without the param bytes then :(
         *
         *      Type: _Function
         */
        strncpy( buffer+1, functionname, len+1 );
        *buffer = '_';
        fptr.funcPtr = (TDefaultFuncPtr) GetProcAddress( (HMODULE)sohandle ,
                                                         buffer            );

        /*
         *      Try adding the param bytes value
         *      ... So much for naming conventions :/
         *
         *      Type: _Function@n
         */
        if ( fptr.funcPtr == NULL )
        {
            sprintf( buffer+len+1, "@%d", parambytes );
            fptr.funcPtr = (TDefaultFuncPtr) GetProcAddress( (HMODULE)sohandle ,
                                                             buffer            );

            /*
             *      Last but not least try..
             *
             *      Type: Function@n
             */
            if ( fptr.funcPtr == NULL )
            {
                fptr.funcPtr = (TDefaultFuncPtr) GetProcAddress( (HMODULE)sohandle ,
                                                                  buffer+1         );
            }
        }

    }
    return fptr;

    #else
    #error Unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

static HWND
GetCurrentHWND( void )
{
        const char* hwndstr;
        HWND whandle = NULL;

        #ifdef SDL_SUPPORT

        /*
         *      If SDL is supported we will try to get the HWND from SDL
         */
        SDL_SysWMinfo winfo;
        SDL_GetWMInfo( &winfo );
        whandle = winfo.window;

        if ( !whandle ) {

        #endif

        #pragma warning( disable: 4047 ) // 'HWND' differs in levels of indirection from 'Int32'

        /*
         *      Try to get the HWND from the process environment settings
         */
        hwndstr = GUCEFGetEnv( "HWND" );
        if ( hwndstr != GUCEF_NULL )
        {

                #ifdef GUCEF_64BIT
                whandle = (HWND) StringToInt64( hwndstr );
                #else
                whandle = (HWND) Str_To_Int( hwndstr );
                #endif
        }
        else
        {
                /*
                 *      If all previous attempts failed then try to use the previous
                 *      clipboard owner (if there is one).
                 */
                whandle = GetClipboardOwner();
        }

        #ifdef SDL_SUPPORT
        }
        #endif

        return whandle;
}

#endif /* GUCEF_MSWIN_BUILD ? */

/*--------------------------------------------------------------------------*/

UInt32
StringToClipboard( const char *str )
{
    #ifdef GUCEF_MSWIN_BUILD

    if ( str )
    {
        UInt32 strlength = (UInt32) strlen( str );

        HWND whandle = GetCurrentHWND();
        UInt32 success = OSWRAP_FALSE;
        HGLOBAL hglbcopy;
        LPTSTR lptstrcopy;

        /*
         *      Open the clipboard with the current task as the owner
         */
        if ( !OpenClipboard( whandle ) ) return OSWRAP_FALSE;

        /*
         *      Try to empty the clipboard so that we can get ownership of the
         *      clipboard which is needed for placing data on it.
         */
        EmptyClipboard();

        /*
         *      Allocate global MS windows managed memory for the text
         */
        hglbcopy = GlobalAlloc( GMEM_MOVEABLE                 ,
                                (strlength+1) * sizeof(TCHAR) );
        if ( hglbcopy == NULL )
        {
                CloseClipboard();
                return OSWRAP_FALSE;
        }

        /*
         *      Now we have to lock the memory we just allocated so that
         *      windows keeps it's paws off of it. After that we can copy
         *      our text into the global memory buffer
         */
        lptstrcopy = (LPTSTR) GlobalLock( hglbcopy );
        memcpy( lptstrcopy, str, strlength+1 );
        GlobalUnlock( hglbcopy );

        /*
         *      We now attempt to set the string in the clipboard.
         *      This will fail if the handle we used in OpenClipboard() is
         *      NULL or incorrect.
         */
        if ( SetClipboardData( CF_TEXT, hglbcopy ) )
        {
            success = OSWRAP_TRUE;
        }
        else
        {
            success = OSWRAP_FALSE;
        }

        /*
         *      Close the clipboard so that other tasks have access again
         */
        CloseClipboard();

        return success;
    }
    return OSWRAP_FALSE;

    #else /* GUCEF_MSWIN_BUILD */

    return OSWRAP_FALSE;

    #endif /* OS WRAPPING */
}

/*--------------------------------------------------------------------------*/

/**
 *      Function that attempts to retrieve a string from the O/S clipboard
 *      If this functionality is not implemented for you O/S or if it fails then
 *      OSWRAP_FALSE will be returned. Otherwise OSWRAP_TRUE will be returned.
 *      It is assumed that dest has points to size number of allocated bytes.
 *      The actual number of bytes written into the destination buffer is
 *      written into wbytes. If you wish to retrieve all text on the clipboard
 *      you might want to continue calling this function as long as wbytes
 *      equals size. Do note that any other proccess can empty the clipboard
 *      in between calls.
 */
UInt32
StringFromClipboard( char *dest     ,
                     UInt32 size    ,
                     UInt32 *wbytes )
{
        #ifdef GUCEF_MSWIN_BUILD

        HWND whandle = GetCurrentHWND();
        HGLOBAL hglb;
        UInt32 success = OSWRAP_FALSE;

        #ifdef SDL_SUPPORT

        SDL_SysWMinfo winfo;

        #endif /* SDL_SUPPORT */

        if ( IsClipboardFormatAvailable( CF_TEXT ) )
        {
                /*
                 *      Open the clipboard with the current task as the owner
                 */
                if ( !OpenClipboard( whandle ) ) return OSWRAP_FALSE;

                /*
                 *      Get a pointer to the data using the global handle we will
                 *      obtain if possible. Then copy the data pointed to by the handle
                 *      into the buffer provided by the user
                 */
                hglb = GetClipboardData( CF_TEXT );
                if ( hglb != NULL )
                {
                        LPTSTR lptstr = (LPTSTR) GlobalLock( hglb );
                        if ( lptstr != NULL )
                        {
                                UInt32 offset = *wbytes;
                                UInt32 dsize = (UInt32) strlen( lptstr );
                                if ( dsize > offset )
                                {
                                        dsize -= offset;
                                        if ( dsize > size-1 ) dsize = size-1;
                                        strncpy( dest, lptstr+offset, dsize );
                                        *wbytes += dsize;

                                        success = OSWRAP_TRUE;
                                }
                                GlobalUnlock( hglb );
                        }
                        else
                        {
                                *wbytes = 0;
                        }
                }

                /*
                 *      Close the clipboard so that other tasks have access again
                 */
                CloseClipboard();

                return success;
        }
        return OSWRAP_FALSE;

        #else /* GUCEF_MSWIN_BUILD */

        return OSWRAP_FALSE;

        #endif /* OS WRAPPING */
}

/*--------------------------------------------------------------------------*/

UInt32
GUCEFSetEnv( const char* key   ,
             const char* value )
{GUCEF_TRACE;

    #ifdef GUCEF_MSWIN_BUILD

    UInt32 retval;
    char* envstr = (char*) malloc( strlen( key ) + strlen( value )+2 );
    sprintf( envstr, "%s=%s", key, value );
    retval = _putenv( envstr );
    free( envstr );
    return retval == 0;

    #else

    return setenv( key, value, 1 ) == 0;

    #endif
}

/*--------------------------------------------------------------------------*/

const char*
GUCEFGetEnv( const char* key )
{GUCEF_TRACE;

    return getenv( key );
}

/*--------------------------------------------------------------------------*/

UInt32
GUCEFGetTickCount( void )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return GetTickCount();

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    #if 1
    struct timespec now;
    clock_gettime( CLOCK_MONOTONIC, &now );
    return (UInt32) ( now.tv_sec*1000000000LL + now.tv_nsec );
    #else
    struct tms timeStorage;
    return (UInt32) times( &timeStorage );
    #endif

    #else
    #error unsupported platform
    #endif
}

/*--------------------------------------------------------------------------*/

void
ShowErrorMessage( const char* message     ,
                  const char* description )
{GUCEF_TRACE;

    #ifdef GUCEF_MSWIN_BUILD
    MessageBox( GetCurrentHWND()                    ,
                description                         ,
                message                             ,
                MB_OK | MB_ICONERROR | MB_TASKMODAL );
    #else
    fprintf( stderr, "%s : %s\n", message, description );
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
GetCPUCountPerPackage( void )
{GUCEF_TRACE;

    /* Number of Logical Cores per Physical Processor */
    UInt32 coreCount = 1;

    /* Initialize to 1 to support older processors. */
    #if ( ( GUCEF_COMPILER == GUCEF_COMPILER_MSVC ) && ( GUCEF_CPU_ARCHITECTURE == GUCEF_CPU_ARCHITECTURE_X86 ) )
    _asm
    {
        mov		eax, 1
        cpuid
        /* Test for HTT bit */
        test	edx, 0x10000000
        jz		Unp
        /* Multi-core or Hyperthreading supported...
        // Read the "# of Logical Processors per Physical Processor" field: */
        mov		eax, ebx
        and		eax, 0x00FF0000 /* Mask the "logical core counter" byte */
        shr		eax, 16 // Shift byte to be least-significant
        mov		coreCount, eax
        /* Uniprocessor (i.e. Pentium III or any AMD CPU excluding their new
        dual-core A64)  */
        Unp:
        /* coreCount will contain 1. */
    }
    #endif

    return coreCount;
}

/*--------------------------------------------------------------------------*/

UInt32
GetPhysicalCPUCount( void )
{GUCEF_TRACE;

    return GetLogicalCPUCount() / GetCPUCountPerPackage();
}

/*--------------------------------------------------------------------------*/

UInt32
GetLogicalCPUCount( void )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    SYSTEM_INFO systemInfo;
    ::GetSystemInfo( &systemInfo );
    return systemInfo.dwNumberOfProcessors;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    return (UInt32) ::sysconf( _SC_NPROCESSORS_ONLN );
    #else
    return 1;
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
GetProcessList( struct SProcessId** processList ,
                UInt32* processCount            )
{GUCEF_TRACE;

    if ( GUCEF_NULL == processCount )
        return OSWRAP_FALSE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    DWORD reservedListSizeInBytes = 1024 * sizeof(DWORD);
    DWORD usedListSizeInBytes = 0;
    DWORD* win32ProcessList = GUCEF_NULL;
    do
    {
        win32ProcessList = (DWORD*) realloc( win32ProcessList, reservedListSizeInBytes );
        if ( GUCEF_NULL == win32ProcessList )
            return OSWRAP_FALSE;

        if ( 0 == ::EnumProcesses( win32ProcessList, reservedListSizeInBytes, &usedListSizeInBytes ) )
        {
            return OSWRAP_FALSE;
        }

        if ( reservedListSizeInBytes != usedListSizeInBytes )
        {
            break;
        }
        else
        {
            // Guestimate: just double the storage
            reservedListSizeInBytes *= 2;
        }
    }
    while ( true );

    *processCount = usedListSizeInBytes / sizeof(DWORD);
    *processList = (struct SProcessId*) realloc( *processList, (*processCount) * sizeof(struct SProcessId) );

    for ( UInt32 i=0; i<(*processCount); ++i )
    {
        (*processList)[ i ].pid = win32ProcessList[ i ];
    }

    free( win32ProcessList );

    return OSWRAP_TRUE;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    UInt32 i=0;
    UInt32 reservedListSizeInBytes = 1024 * sizeof(Int32);
    UInt32 usedListSizeInBytes = 0;
    Int32* linuxProcessList = (Int32*) malloc( reservedListSizeInBytes );
    DIR* dp = ::opendir( "/proc/" );
    struct dirent* dirFilesys = NULL;
    while ( NULL != ( dirFilesys = ::readdir( dp ) ) )
    {
        if ( dirFilesys->d_type == DT_DIR )
        {
            if ( IsANumber( dirFilesys->d_name ) )
            {
                linuxProcessList[ i ] = StringToInt32( dirFilesys->d_name );
                ++i; usedListSizeInBytes += 4;

                if ( usedListSizeInBytes >= reservedListSizeInBytes )
                {
                    // Guestimate: just double the storage
                    reservedListSizeInBytes *= 2;

                    linuxProcessList = (Int32*) realloc( linuxProcessList, reservedListSizeInBytes );
                }
            }
        }
    }
    ::closedir( dp );

    *processCount = i;
    if ( i > 0 )
    {
        *processList = (struct SProcessId*) malloc( i * sizeof(struct SProcessId) );
        for ( UInt32 n=0; n<i; ++n )
        {
            (*processList)[ n ].pid = linuxProcessList[ n ];
        }
    }
    else
    {
        *processList = NULL;
    }
    free( linuxProcessList );

    return OSWRAP_TRUE;

    #else

    *processList = GUCEF_NULL;
    *processCount = 0;
    return OSWRAP_FALSE;

    #endif
}

/*--------------------------------------------------------------------------*/

void
FreeProcessList( TProcessId* processList )
{GUCEF_TRACE;

    if ( GUCEF_NULL != processList )
        free( processList );
}

/*--------------------------------------------------------------------------*/

TProcessId*
GetProcessIdAtIndex( TProcessId* processList ,
                     UInt32 index            )
{GUCEF_TRACE;

    if ( GUCEF_NULL != processList )
        return &processList[ index ];
    return GUCEF_NULL;
}

/*--------------------------------------------------------------------------*/

TProcessId*
CopyProcessId( TProcessId* pid )
{GUCEF_TRACE;

    if ( GUCEF_NULL != pid )
    {
        TProcessId* pidCopy = (TProcessId*) malloc( sizeof(TProcessId) );
        memcpy( pidCopy, pid, sizeof(TProcessId) );
        return pidCopy;
    }
    return GUCEF_NULL;
}

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
FreeProcessId( TProcessId* pid )
{GUCEF_TRACE;

    if ( GUCEF_NULL != pid )
        free( pid );
}

/*--------------------------------------------------------------------------*/

UInt32
GetProcessMemoryUsage( TProcessId* pid                     ,
                       TProcessMemoryUsageInfo* memUseInfo )
{GUCEF_TRACE;

    if ( GUCEF_NULL == pid || GUCEF_NULL == memUseInfo )
        return OSWRAP_FALSE;

    memset( memUseInfo, 0, sizeof(TProcessMemoryUsageInfo) );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    HANDLE hProcess = ::OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                      FALSE,
                                      pid->pid );
    if ( GUCEF_NULL == hProcess )
        return OSWRAP_FALSE;

    PROCESS_MEMORY_COUNTERS pmc;
    if ( ::GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc) ) )
    {
        memUseInfo->pageFaultCountInBytes = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.PageFaultCount;
        memUseInfo->pageFileUsageInBytes = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.PagefileUsage;
        memUseInfo->peakPageFileUsageInBytes = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.PeakPagefileUsage;
        memUseInfo->peakWorkingSetSizeInBytes  = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.PeakWorkingSetSize;
        //memUseInfo->quotaNonPagedPoolUsageInBytes  = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.QuotaNonPagedPoolUsage;
        //memUseInfo->quotaPagedPoolUsageInBytes  = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.QuotaPagedPoolUsage;
        //memUseInfo->quotaPeakNonPagedPoolUsageInBytes  = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.QuotaPeakNonPagedPoolUsage;
        //memUseInfo->quotaPeakPagedPoolUsageInBytes  = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.QuotaPeakPagedPoolUsage;
        memUseInfo->workingSetSizeInBytes  = (TProcessMemoryUsageInfo::TProcMemStatInt) pmc.WorkingSetSize;

        ::CloseHandle( hProcess );
        return OSWRAP_TRUE;
    }

    ::CloseHandle( hProcess );
    return OSWRAP_FALSE;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    long totalProgramSize = 0;
    long residentSetSize = 0;
    long sharedPages = 0;
    long codeSize = 0;
    long dataAndStack = 0;
    long librarySize = 0;
    long dirtyPages = 0;
    FILE* fp = NULL;
    char procStatPath[ 24 ];

    sprintf( procStatPath, "/proc/%d/statm", pid->pid );
    if ( (fp = ::fopen( procStatPath, "r" )) == NULL )
        return OSWRAP_FALSE;
    if ( ::fscanf( fp, "%ld%ld%ld%ld%ld%ld%ld", &totalProgramSize, &residentSetSize, &sharedPages, &codeSize, &dataAndStack, &librarySize, &dirtyPages ) < 1 )
    {
        fclose( fp );
        return OSWRAP_FALSE;
    }
    fclose( fp );

    long sizeOfPageInBytes = ::sysconf( _SC_PAGESIZE );

    memUseInfo->pageFaultCountInBytes = (TProcessMemoryUsageInfo::TProcMemStatInt) dirtyPages * sizeOfPageInBytes;
    memUseInfo->pageFileUsageInBytes = (TProcessMemoryUsageInfo::TProcMemStatInt) sharedPages * sizeOfPageInBytes;
    memUseInfo->workingSetSizeInBytes = (TProcessMemoryUsageInfo::TProcMemStatInt) residentSetSize * sizeOfPageInBytes;

    return OSWRAP_TRUE;

    #else

    return OSWRAP_FALSE;

    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
GetGlobalMemoryUsage( TGlobalMemoryUsageInfo* memUseInfo )
{GUCEF_TRACE;

    if ( GUCEF_NULL == memUseInfo )
        return OSWRAP_FALSE;

    memset( memUseInfo, 0, sizeof(TGlobalMemoryUsageInfo) );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    ::MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if ( 0 != ::GlobalMemoryStatusEx( &statex ) )
    {
        memUseInfo->memoryLoadPercentage = (UInt8) statex.dwMemoryLoad;
        memUseInfo->totalPhysicalMemoryInBytes = (TGlobalMemoryUsageInfo::TMemStatInt) statex.ullTotalPhys;
        memUseInfo->availablePhysicalMemoryInBytes = (TGlobalMemoryUsageInfo::TMemStatInt) statex.ullAvailPhys;
        memUseInfo->totalPageFileSizeInBytes = (TGlobalMemoryUsageInfo::TMemStatInt) statex.ullTotalPageFile;
        memUseInfo->availablePageFileSizeInBytes = (TGlobalMemoryUsageInfo::TMemStatInt) statex.ullAvailPageFile;
        memUseInfo->totalVirtualMemoryInBytes = (TGlobalMemoryUsageInfo::TMemStatInt) statex.ullTotalVirtual;
        memUseInfo->availableVirtualMemoryInBytes = (TGlobalMemoryUsageInfo::TMemStatInt) statex.ullAvailVirtual;
        memUseInfo->availExtendedVirtualMemoryInBytes = (TGlobalMemoryUsageInfo::TMemStatInt) statex.ullAvailExtendedVirtual;

        return OSWRAP_TRUE;
    }
    else
    {
        return OSWRAP_FALSE;
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    // @todo
    return OSWRAP_FALSE;

    #else

    return OSWRAP_FALSE;

    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
GetExeNameForProcessId( TProcessId* pid        ,
                        char* outNameBuffer    ,
                        UInt32 nameBufferSize  ,
                        UInt32* usedBufferSize )
{GUCEF_TRACE;

    if ( GUCEF_NULL == pid || GUCEF_NULL == outNameBuffer || GUCEF_NULL == usedBufferSize )
        return OSWRAP_FALSE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    HANDLE handle = ::OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION,
                                   FALSE,
                                   pid->pid /* This is the PID, you can find one from windows task manager */
                                 );
    if ( GUCEF_NULL != handle )
    {
        #if 1

        DWORD buffSize = (DWORD) nameBufferSize;
        if ( ::QueryFullProcessImageNameA( handle, 0, outNameBuffer, &buffSize ) )
        {
            *usedBufferSize = (UInt32) buffSize;
            ::CloseHandle( handle );
            return OSWRAP_TRUE;
        }
        else
        {
            *outNameBuffer = '\0';
            *usedBufferSize = 0;
        }

        #else

        // Needs PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,

        HMODULE hMod = 0;
        DWORD cbNeeded = 0;
        if ( EnumProcessModules( handle, &hMod, sizeof(hMod), &cbNeeded ) )
        {
            *usedBufferSize = (UInt32) GetModuleBaseNameA( handle, hMod, outNameBuffer, nameBufferSize );
            ::CloseHandle( handle );
            return OSWRAP_TRUE;
        }
        else
        {
            *outNameBuffer = '\0';
            *usedBufferSize = 0;
        }

        #endif

        ::CloseHandle( handle );
    }
    return OSWRAP_FALSE;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    // The following works on Linux 2.2 and later:

    char procInfoPath[ 21 ];
    sprintf( procInfoPath, "/proc/%d/exe", pid->pid );
    UInt32 maxNameBytes = PATH_MAX > nameBufferSize ? PATH_MAX : nameBufferSize;
    char* symLinkPath = (char*) malloc( maxNameBytes );
    ssize_t bytesWritten = ::readlink( procInfoPath, symLinkPath, maxNameBytes );
    if ( bytesWritten <= 0 )
    {
        // Failed to read the link
        // this is expected for some OS level and zombie procs
        free( symLinkPath );
        *outNameBuffer = '\0';
        *usedBufferSize = 0;
        return OSWRAP_FALSE;
    }

    // The symlink can have the path prefixed from where the executable is linked
    // We just want the name itself
    const char* symLinkFilename = Extract_Filename( symLinkPath );
    UInt32 symLinkFilenameLength = strlen( symLinkFilename )+1;
    if ( symLinkFilenameLength > maxNameBytes )
        symLinkFilenameLength = maxNameBytes;
    memcpy( outNameBuffer, symLinkFilename, symLinkFilenameLength );
    outNameBuffer[ symLinkFilenameLength-1 ] = '\0';
    *usedBufferSize = symLinkFilenameLength;
    free( symLinkPath );
    return OSWRAP_TRUE;

    #else

    return OSWRAP_FALSE;

    #endif
}

/*--------------------------------------------------------------------------*/

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

Int64
GetFiletimeDelta( LPFILETIME a, LPFILETIME b )
{GUCEF_TRACE;

    ULARGE_INTEGER converterStructA;
    converterStructA.HighPart = a->dwHighDateTime;
    converterStructA.LowPart = a->dwLowDateTime;

    ULARGE_INTEGER converterStructB;
    converterStructB.HighPart = b->dwHighDateTime;
    converterStructB.LowPart = b->dwLowDateTime;

    return converterStructA.QuadPart - converterStructB.QuadPart;
}

/*--------------------------------------------------------------------------*/

Int64
GetDurationSinceFiletimeInMs( LPFILETIME since )
{GUCEF_TRACE;

    if ( NULL == since )
        return 0;

    FILETIME systemNowTime;
    ::GetSystemTimeAsFileTime( &systemNowTime );

    Int64 timeDeltaInHundredNanoSecs = GetFiletimeDelta( &systemNowTime, since );

    // 100 nanoseconds is 0.0001 ms
    // so we need to divide by 10000
    return (Int64) ( timeDeltaInHundredNanoSecs / 10000 );
}

#endif

/*--------------------------------------------------------------------------*/

#if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

bool
GetProcUptime( pid_t pid, UInt64* uptimeInMs )
{
    FILE* fp = NULL;
    char procStatPath[ 22 ];
    *uptimeInMs = 0;

    sprintf( procStatPath, "/proc/%d/stat", pid );

    if ( ( fp = ::fopen( procStatPath, "r" ) ) == NULL )
        return false;

    char procState = 0;
    if ( ::fscanf( fp, "%*d%*s%*c%c", &procState ) != 1 )
    {
        fclose( fp );
        return false;
    }
    fclose( fp );

    // R  Running
    // S  Sleeping in an interruptible wait
    // D  Waiting in uninterruptible disk sleep
    // Z  Zombie
    // T  Stopped (on a signal) or (before Linux 2.6.33)
    //    trace stopped
    // t  Tracing stop (Linux 2.6.33 onward)
    // W  Paging (only before Linux 2.6.0)
    // X  Dead (from Linux 2.6.0 onward)
    // x  Dead (Linux 2.6.33 to 3.13 only)
    // K  Wakekill (Linux 2.6.33 to 3.13 only)
    // W  Waking (Linux 2.6.33 to 3.13 only)
    // P  Parked (Linux 3.9 to 3.13 only)
    switch ( procState )
    {
        case 'Z':
        case 'X':
        case 'x':
        {
            // Since the proc is dead the uptime should be 0
            return true;
        }
    }

    // The proc appears by all indications still alive
    // we now get the actual uptime
    sprintf( procStatPath, "/proc/%d", pid );
    struct stat statBuffer;
    if ( 0 != ::stat( procStatPath, &statBuffer ) )
    {
        // Proc probaby died while executing this code
        fclose( fp );
        return false;
    }

    time_t nowTime = ::time( NULL );
    *uptimeInMs = ( ( nowTime - statBuffer.st_mtim.tv_sec ) * 1000 ) + ( statBuffer.st_mtim.tv_nsec / 100000000 );
    return true;
}

/*--------------------------------------------------------------------------*/

/*

Table 1-3: Contents of the stat files (as of 2.6.22-rc3)

 Field          Content
  pid           process id
  tcomm         filename of the executable
  state         state (R is running, S is sleeping, D is sleeping in an
                uninterruptible wait, Z is zombie, T is traced or stopped)
  ppid          process id of the parent process
  pgrp          pgrp of the process
  sid           session id
  tty_nr        tty the process uses
  tty_pgrp      pgrp of the tty
  flags         task flags
  min_flt       number of minor faults
  cmin_flt      number of minor faults with child's
  maj_flt       number of major faults
  cmaj_flt      number of major faults with child's
  utime         user mode jiffies
  stime         kernel mode jiffies
  cutime        user mode jiffies with child's
  cstime        kernel mode jiffies with child's
*/

bool
GetProcJiffies( pid_t pid, UInt64* userModeJiffies, UInt64* kernelModeJiffies )
{
    FILE* fp = NULL;
    char procStatPath[ 22 ];
    sprintf( procStatPath, "/proc/%d/stat", pid );

    if ( ( fp = ::fopen( procStatPath, "r" ) ) == NULL )
        return false;
    if ( ::fscanf( fp, "%*d%*s%*s%*d%*d%*d%*d%*d%*d%*d%*d%*d%*d%d%d", userModeJiffies, kernelModeJiffies ) != 2 )
    {
        fclose( fp );
        return false;
    }
    fclose( fp );
    return true;
}

/*--------------------------------------------------------------------------*/

bool
GetGlobalJiffies( UInt64* totalJiffies )
{
    FILE* fp = NULL;
    if ( ( fp = ::fopen( "/proc/stat", "r" ) ) == NULL )
        return false;

    char cpuLine[ 1024 ];
    int eRead = ::fscanf( fp, "%*s %[^\n]", cpuLine );
    if ( eRead != 1 )
    {
        fclose( fp );
        return false;
    }
    fclose( fp );

    Int64 allJiffies = 0;
    Int64 moreJiffies = 0;
    const char* cpuLineOffset = cpuLine;
    while ( *cpuLineOffset != '\0' && ::sscanf( cpuLineOffset, "%ld", &moreJiffies ) == 1 )
    {
        allJiffies += moreJiffies;

        ++cpuLineOffset;
        while ( *cpuLineOffset != '\0' && *cpuLineOffset != ' ' )
            ++cpuLineOffset;
    }
    *totalJiffies = (UInt64) allJiffies;
    return true;
}

#endif

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C TProcCpuDataPoint*
CreateProcCpuDataPoint( TProcessId* pid )
{
    if ( GUCEF_NULL == pid )
        return GUCEF_NULL;

    TProcCpuDataPoint* dataPoint = (TProcCpuDataPoint*) malloc( sizeof( TProcCpuDataPoint ) );
    memset( dataPoint, 0, sizeof( TProcCpuDataPoint ) );

    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    dataPoint->pid = pid->pid;
    GetGlobalJiffies( &dataPoint->globalJiffies );
    GetProcJiffies( pid->pid, &dataPoint->procUserModeJiffies, &dataPoint->procKernelModeJiffies );
    #endif

    return dataPoint;
}

/*--------------------------------------------------------------------------*/

void
FreeProcCpuDataPoint( TProcCpuDataPoint* cpuDataDataPoint )
{
    if ( GUCEF_NULL != cpuDataDataPoint )
    {
        free( cpuDataDataPoint );
    }
}

/*--------------------------------------------------------------------------*/

UInt32
GetProcessCpuUsage( TProcessId* pid                             ,
                    TProcCpuDataPoint* previousCpuDataDataPoint ,
                    TProcessCpuUsageInfo* cpuUseInfo            )
{GUCEF_TRACE;

    if ( GUCEF_NULL == pid || GUCEF_NULL == previousCpuDataDataPoint || GUCEF_NULL == cpuUseInfo )
        return OSWRAP_FALSE;

    memset( cpuUseInfo, 0, sizeof(TProcessCpuUsageInfo) );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    HANDLE hProcess = ::OpenProcess( PROCESS_QUERY_INFORMATION,
                                     FALSE,
                                     pid->pid );
    if ( GUCEF_NULL == hProcess )
        return OSWRAP_FALSE;

    FILETIME creationTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    if ( ::GetProcessTimes( hProcess, &creationTime, &exitTime, &kernelTime, &userTime ) == TRUE )
    {
        cpuUseInfo->uptimeInMs = (UInt64) GetDurationSinceFiletimeInMs( &creationTime );
        cpuUseInfo->overallCpuConsumptionPercentage = 0; // <- todo


        // Overwrite the CPU data point making current the previous for next call to this function
        previousCpuDataDataPoint->kernelTime = kernelTime;
        previousCpuDataDataPoint->userTime = userTime;

        ::CloseHandle( hProcess );
        return OSWRAP_TRUE;
    }

    ::CloseHandle( hProcess );
    return OSWRAP_FALSE;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( previousCpuDataDataPoint->pid != pid->pid )
        return OSWRAP_FALSE;

    UInt64 globalJiffies = 0;
    UInt64 procKernelModeJiffies = 0;
    UInt64 procUserModeJiffies = 0;
    if ( GetGlobalJiffies( &globalJiffies ) &&
         GetProcJiffies( pid->pid, &procUserModeJiffies, &procKernelModeJiffies ) )
    {
        // Calculate the % of total CPU jiffies spent used by this proc during the time
        // that has passed between the 2 data sample points in time

        Int64 globalTotalJiffiesDelta = globalJiffies - previousCpuDataDataPoint->globalJiffies;
        if ( globalTotalJiffiesDelta == 0 )
            globalTotalJiffiesDelta = 1;

        Float64 userModeJiffiesDelta = procUserModeJiffies - previousCpuDataDataPoint->procUserModeJiffies;
        Float64 kernelModeJiffiesDelta = procKernelModeJiffies - previousCpuDataDataPoint->procKernelModeJiffies;
        Float64 userModePerc = 100.0 * ( userModeJiffiesDelta / globalTotalJiffiesDelta );
        Float64 kernelModePerc = 100.0 * ( kernelModeJiffiesDelta / globalTotalJiffiesDelta );

        // Overwrite the CPU data point making current the previous for next call to this function
        previousCpuDataDataPoint->procKernelModeJiffies = procKernelModeJiffies;
        previousCpuDataDataPoint->procUserModeJiffies = procUserModeJiffies;
        previousCpuDataDataPoint->globalJiffies = globalJiffies;

        cpuUseInfo->overallCpuConsumptionPercentage = userModePerc + kernelModePerc;
    }
    else
    {
        return OSWRAP_FALSE;
    }

    GetProcUptime( pid->pid, &cpuUseInfo->uptimeInMs );

    return OSWRAP_TRUE;

    #else

    return OSWRAP_FALSE;

    #endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/
