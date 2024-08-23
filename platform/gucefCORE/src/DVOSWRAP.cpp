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
#include <math.h>
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
  #include <powerbase.h>              /* needed for CPU stats */
  #undef min
  #undef max
  #define MAX_DIR_LENGTH MAX_PATH

  #ifndef GUCEF_CORE_MSWINUTILS_H
  #include "gucefCORE_mswinutils.h"
  #define GUCEF_CORE_MSWINUTILS_H
  #endif /* GUCEF_CORE_MSWINUTILS_H? */

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

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

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

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
 // defined elsewhere right now
#else
struct SProcessId
{
    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    pid_t pid;
    #else
    int foobar_NotSupported;
    #endif
};
#endif

/*--------------------------------------------------------------------------*/

#if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

class GUCEF_HIDDEN LinuxLogicalCpuInfo
{
    public:

    UInt16 processor;
    CString vendor_id;
    UInt32 cpuFamily;
    UInt32 model;
    CString modelName;
    UInt32 stepping;
    Float32 cpuMhz;             // per the docs this is just a snapshot, not a sampling over a time interval
    UInt32 cacheSizeInBytes;
    UInt16 physicalId;
    UInt16 siblings;
    UInt16 coreId;
    UInt16 cpuCores;
    UInt32 apicid;
    UInt32 initialApicid;
    bool fpu;
    bool fpu_exception;
    UInt32 cpuidLevel;
    bool wp;
    CString::StringSet flags;
    CString::StringSet vmxFlags;
    CString::StringSet bugs;
    Float32 bogomips;
    UInt16 clflushSize;
    UInt16 cache_alignment;
    CString addressSizes;

    LinuxLogicalCpuInfo( void )
        : processor( 0 )
        , vendor_id()
        , cpuFamily( 0 )
        , model( 0 )
        , modelName()
        , stepping( 0 )
        , cpuMhz( 0.0 )
        , cacheSizeInBytes( 0 )
        , physicalId( 0 )
        , siblings( 0 )
        , coreId( 0 )
        , cpuCores( 0 )
        , apicid( 0 )
        , initialApicid( 0 )
        , fpu( false )
        , fpu_exception( false )
        , cpuidLevel( 0 )
        , wp( false )
        , flags()
        , vmxFlags()
        , bugs()
        , bogomips( 0.0 )
        , clflushSize( 0 )
        , cache_alignment( 0 )
        , addressSizes()
    {GUCEF_TRACE;

    }
};

/*--------------------------------------------------------------------------*/

class GUCEF_HIDDEN AllLinuxProcCpuInfo
{
    public:

    typedef std::vector< LinuxLogicalCpuInfo >     LinuxLogicalCpuInfoVector;

    LinuxLogicalCpuInfoVector allCpuInfo;

    bool InitFromOS( void )
    {GUCEF_TRACE;

        FILE* procCpuInfoFile = fopen( "/proc/cpuinfo", "r" );
        if ( GUCEF_NULL != procCpuInfoFile )
        {
            allCpuInfo.clear();

            LinuxLogicalCpuInfo cpuInfo;
            char lineBuffer[ 2048 ];

            while ( GUCEF_NULL != fgets( lineBuffer, 2048, procCpuInfoFile ) )
            {
                // Lines with content have the form <key name><\t><:><value><\n>
                const char* tabChar = strchr( lineBuffer, '\t' );
                if ( GUCEF_NULL != tabChar )
                {
                    const char* colonChar = strchr( tabChar, ':' );
                    UInt32 keyStrLength = 0;
                    if ( GUCEF_NULL != colonChar )
                    {
                        keyStrLength = (UInt32) ( tabChar - lineBuffer );

                        if ( 0 == strncmp( "processor", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.processor = (UInt16) value;
                        }
                        else
                        if ( 0 == strncmp( "vendor_id", lineBuffer, keyStrLength ) )
                        {
                            const char* eolChar = strchr( colonChar+1, '\n' );
                            if ( GUCEF_NULL != eolChar )
                            {
                                cpuInfo.vendor_id.Set( colonChar+1, eolChar-(colonChar+1) );
                                cpuInfo.vendor_id.Trim( true );
                            }
                        }
                        else
                        if ( 0 == strncmp( "cpu family", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.cpuFamily = (UInt32) value;
                        }
                        else
                        if ( 0 == strncmp( "model", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.model = (UInt16) value;
                        }
                        else
                        if ( 0 == strncmp( "model name", lineBuffer, keyStrLength ) )
                        {
                            cpuInfo.modelName = colonChar+1;
                        }
                        else
                        if ( 0 == strncmp( "stepping", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.stepping = (UInt32) value;
                        }
                        else
                        if ( 0 == strncmp( "cpu MHz", lineBuffer, keyStrLength ) )
                        {
                            sscanf( colonChar+1, "%f", &cpuInfo.cpuMhz );
                        }
                        else
                        if ( 0 == strncmp( "cache size", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );

                            // Use old convention of KB == 1024 not 1000
                            cpuInfo.cacheSizeInBytes = (UInt32) value * 1024;
                        }
                        else
                        if ( 0 == strncmp( "physical id", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.physicalId = (UInt16) value;
                        }
                        else
                        if ( 0 == strncmp( "core id", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.coreId = (UInt16) value;
                        }
                        else
                        if ( 0 == strncmp( "cpu cores", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.cpuCores = (UInt16) value;
                        }
                        if ( 0 == strncmp( "apicid", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.apicid = (UInt32) value;
                        }
                        else
                        if ( 0 == strncmp( "initial apicid", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.initialApicid = (UInt32) value;
                        }
                        else
                        if ( 0 == strncmp( "fpu", lineBuffer, keyStrLength ) )
                        {
                            Int32 boolInt = String_To_Boolint( colonChar+1 );
                            cpuInfo.fpu = boolInt > 0;
                        }
                        else
                        if ( 0 == strncmp( "fpu_exception", lineBuffer, keyStrLength ) )
                        {
                            Int32 boolInt = String_To_Boolint( colonChar+1 );
                            cpuInfo.fpu_exception = boolInt > 0;
                        }
                        else
                        if ( 0 == strncmp( "cpuid level", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.cpuidLevel = (UInt32) value;
                        }
                        else
                        if ( 0 == strncmp( "wp", lineBuffer, keyStrLength ) )
                        {
                            Int32 boolInt = String_To_Boolint( colonChar+1 );
                            cpuInfo.wp = boolInt > 0;
                        }
                        else
                        if ( 0 == strncmp( "bogomips", lineBuffer, keyStrLength ) )
                        {
                            sscanf( colonChar+1, "%f", &cpuInfo.bogomips );
                        }
                        else
                        if ( 0 == strncmp( "clflush size", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.clflushSize = (UInt16) value;
                        }
                        else
                        if ( 0 == strncmp( "cache_alignment", lineBuffer, keyStrLength ) )
                        {
                            Int32 value = 0;
                            sscanf( colonChar+1, "%d", &value );
                            cpuInfo.cache_alignment = (UInt16) value;
                        }
                        else
                        if ( 0 == strncmp( "address sizes", lineBuffer, keyStrLength ) )
                        {
                            cpuInfo.addressSizes = colonChar+1;
                        }
                    }
                }
                else
                {
                    // Empty lines seperate the different sections
                    allCpuInfo.push_back( cpuInfo );
                }
            }
            fclose( procCpuInfoFile );
            return true;
        }
        return false;
    }

    AllLinuxProcCpuInfo( void )
        : allCpuInfo()
    {GUCEF_TRACE;

        InitFromOS();
    }
};

#endif


#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

// Note that this structure definition was accidentally omitted from WinNT.h.
// This error will be corrected in the future. In the meantime, to compile your application,
// include the structure definition contained in this topic in your source code.
// See: https://docs.microsoft.com/en-us/windows/win32/power/processor-power-information-str
typedef struct _PROCESSOR_POWER_INFORMATION {
  ULONG Number;
  ULONG MaxMhz;
  ULONG CurrentMhz;
  ULONG MhzLimit;
  ULONG MaxIdleState;
  ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

#endif

/*--------------------------------------------------------------------------*/

struct SCpuDataPoint
{
    TCpuStats cpuStats;
    UInt32 logicalCpuCount; // we keep a copy here in the private stucture for fast and safe access. Someone could alter the one in cpuStats

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    PPROCESSOR_POWER_INFORMATION cpuPowerInfo;
    SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION* cpuPerfInfo;
    SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION* prevCpuPerfInfo;
    FILETIME globalUserTime;
    FILETIME globalKernelTime;
    FILETIME globalIdleTime;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    AllLinuxProcCpuInfo* infoFromProcCpu;
    #else

    #endif
};

struct SProcCpuDataPoint
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    DWORD pid;
    HANDLE hProcess;
    FILETIME procUserTime;
    FILETIME procKernelTime;
    FILETIME globalUserTime;
    FILETIME globalKernelTime;
    FILETIME globalIdleTime;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    pid_t pid;
    UInt64 procUserModeJiffies;
    UInt64 procKernelModeJiffies;
    UInt64 globalJiffies;
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
    if ( GUCEF_NULL == modulePtr )
    {
        // It is possible the load failed due to missing "lib" prefix on linux/android.
        // Check for this and compensate as needed
        CString fileOnly = ExtractFilename( fName );
        if ( 0 != fileOnly.HasSubstr( "lib" ) )
        {
            // No module name previous "lib" prefix was found, we will add one and try to load again
            fileOnly = "lib" + fileOnly;
            CString newFilePath = CombinePath( StripFilename( fName ), fileOnly );

            modulePtr = (void*) dlopen( newFilePath.C_String(), RTLD_NOW );
        }
    }

    // It is possible that per Linux fashion the module is actually in a /lib/ dir
    // while the current dir is pointing at a /bin/ dir due to the pattern of allowing
    // $MODULEDIR$ variable based loading. We check for that here as well.
    if ( GUCEF_NULL == modulePtr )
    {
        CString pathOnly = StripFilename( fName );
        if ( "bin" == LastSubDir( pathOnly ) )
        {
            pathOnly = CombinePath( StripLastSubDir( pathOnly ), "lib" );
            CString fileOnly = ExtractFilename( fName );
            CString newFilePath = CombinePath( pathOnly, fileOnly );

            modulePtr = (void*) dlopen( newFilePath.C_String(), RTLD_NOW );

            if ( GUCEF_NULL == modulePtr )
            {
                // It is possible the load failed due to missing "lib" prefix on linux/android.
                // Check for this and compensate as needed
                if ( 0 != fileOnly.HasSubstr( "lib" ) )
                {
                    // No module name previous "lib" prefix was found, we will add one and try to load again
                    fileOnly = "lib" + fileOnly;
                    newFilePath = CombinePath( pathOnly, fileOnly );

                    modulePtr = (void*) dlopen( newFilePath.C_String(), RTLD_NOW );
                }
            }
        }
    }

    if ( GUCEF_NULL == modulePtr )
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

OSWRAP_BOOLINT
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
OSWRAP_BOOLINT
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

OSWRAP_BOOLINT
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


//::GetLogicalProcessorInformationEx


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

OSWRAP_BOOLINT
GetProcessList( TProcessId** processList ,
                UInt32* processCount     )
{GUCEF_TRACE;

    if ( GUCEF_NULL == processCount || GUCEF_NULL == processList )
        return OSWRAP_FALSE;

    // Just call the C++ implementation and convert the result for C
    TProcessIdVector procIds;
    if ( GetProcessList( procIds ) )
    {
        *processList = (TProcessId*) calloc( procIds.size(), sizeof(TProcessId) );
        if ( GUCEF_NULL != *processList )
        {
            for ( size_t i=0; i<procIds.size(); ++i )
            {
                (*processList)[ i ] = procIds[ i ];
                ++i;
            }
            OSWRAP_TRUE;
        }
    }
    return OSWRAP_FALSE;
}

/*--------------------------------------------------------------------------*/

void
FreeProcessList( TProcessId* processList )
{GUCEF_TRACE;

    if ( GUCEF_NULL != processList )
        free( processList );
}

/*--------------------------------------------------------------------------*/

TProcessId
GetProcessIdAtIndex( TProcessId* processList ,
                     UInt32 index            )
{GUCEF_TRACE;

    if ( GUCEF_NULL != processList )
        return processList[ index ];
    return 0;
}

/*--------------------------------------------------------------------------*/

OSWRAP_BOOLINT
IsProcessStillActive( TProcessId pid, OSWRAP_BOOLINT* status )
{GUCEF_TRACE;

    if ( GUCEF_NULL != status )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        // To get the alive status of another proc we need a handle to it
        HANDLE hProcess = ::OpenProcess( PROCESS_QUERY_INFORMATION,
                                         FALSE,
                                         pid );
        if ( GUCEF_NULL == hProcess )
            return OSWRAP_FALSE;

        *status = OSWRAP_TRUE;
        DWORD exitCode = 0;
        if ( ( ::GetExitCodeProcess( hProcess, &exitCode ) == FALSE ) || exitCode != STILL_ACTIVE )
        {
            *status = OSWRAP_FALSE;
        }

        ::CloseHandle( hProcess );
        return OSWRAP_TRUE;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        // @TODO: check under /proc

        #else

        return OSWRAP_FALSE;

        #endif
    }
    return OSWRAP_FALSE;
}

/*--------------------------------------------------------------------------*/

UInt32
GetProcessMemoryUsage( TProcessId pid                      ,
                       TProcessMemoryUsageInfo* memUseInfo )
{GUCEF_TRACE;

    if ( GUCEF_NULL == memUseInfo )
        return OSWRAP_FALSE;

    memset( memUseInfo, 0, sizeof(TProcessMemoryUsageInfo) );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    HANDLE hProcess = ::OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                      FALSE,
                                      pid );
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
    char procStatPath[ 64 ];

    sprintf( procStatPath, "/proc/%d/statm", pid );
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
GetExeNameForProcessId( TProcessId pid         ,
                        char* outNameBuffer    ,
                        UInt32 nameBufferSize  ,
                        UInt32* usedBufferSize )
{GUCEF_TRACE;

    if ( GUCEF_NULL == outNameBuffer || GUCEF_NULL == usedBufferSize )
        return OSWRAP_FALSE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    HANDLE handle = ::OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION,
                                   FALSE,
                                   pid /* This is the PID, you can find one from windows task manager */
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

    if ( 0 == pid )
        return OSWRAP_FALSE;

    // The following works on Linux 2.2 and later:

    char procInfoPath[ 64 ];
    sprintf( procInfoPath, "/proc/%d/exe", pid );
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
GetFiletimeAsUInt64( LPFILETIME a )
{GUCEF_TRACE;

    ULARGE_INTEGER converterStructA;
    converterStructA.HighPart = a->dwHighDateTime;
    converterStructA.LowPart = a->dwLowDateTime;
    return converterStructA.QuadPart;
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
CreateProcCpuDataPoint( TProcessId pid )
{GUCEF_TRACE;

    if ( 0 == pid )
        return GUCEF_NULL;

    TProcCpuDataPoint* dataPoint = (TProcCpuDataPoint*) malloc( sizeof( TProcCpuDataPoint ) );
    if ( GUCEF_NULL == dataPoint )
        return GUCEF_NULL;

    memset( dataPoint, 0, sizeof( TProcCpuDataPoint ) );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    dataPoint->pid = pid;
    dataPoint->hProcess = ::OpenProcess( PROCESS_QUERY_INFORMATION,
                                         FALSE,
                                         pid );
    if ( GUCEF_NULL != dataPoint->hProcess )
    {
        FILETIME dummy;
        ::GetProcessTimes( dataPoint->hProcess, &dummy, &dummy, &dataPoint->procKernelTime, &dataPoint->procUserTime );
        ::GetSystemTimes( &dataPoint->globalIdleTime, &dataPoint->globalKernelTime, &dataPoint->globalUserTime );
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    dataPoint->pid = pid;
    GetGlobalJiffies( &dataPoint->globalJiffies );
    GetProcJiffies( pid, &dataPoint->procUserModeJiffies, &dataPoint->procKernelModeJiffies );
    #endif

    return dataPoint;
}

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C TProcCpuDataPoint*
CopyProcCpuDataPoint( TProcCpuDataPoint* srcCpuDataDataPoint ,
                      TProcessId newProcId                   )
{GUCEF_TRACE;

    if ( GUCEF_NULL == srcCpuDataDataPoint )
        return GUCEF_NULL;

    TProcCpuDataPoint* dataPoint = (TProcCpuDataPoint*) malloc( sizeof( TProcCpuDataPoint ) );
    if ( GUCEF_NULL == dataPoint )
        return GUCEF_NULL;
    memset( dataPoint, 0, sizeof( TProcCpuDataPoint ) );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    if ( 0 == newProcId )
        dataPoint->pid = srcCpuDataDataPoint->pid;
    else
        dataPoint->pid = newProcId;

    dataPoint->hProcess = ::OpenProcess( PROCESS_QUERY_INFORMATION,
                                         FALSE,
                                         dataPoint->pid );
    if ( GUCEF_NULL == dataPoint->hProcess )
    {
        free( dataPoint );
        return GUCEF_NULL;
    }

    dataPoint->globalIdleTime = srcCpuDataDataPoint->globalIdleTime;
    dataPoint->globalKernelTime = srcCpuDataDataPoint->globalKernelTime;
    dataPoint->globalUserTime = srcCpuDataDataPoint->globalUserTime;
    dataPoint->procKernelTime = srcCpuDataDataPoint->procKernelTime;
    dataPoint->procUserTime = srcCpuDataDataPoint->procUserTime;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( 0 == newProcId )
        dataPoint->pid = srcCpuDataDataPoint->pid;
    else
        dataPoint->pid = newProcId;

    dataPoint->globalJiffies = srcCpuDataDataPoint->globalJiffies;
    dataPoint->procUserModeJiffies = srcCpuDataDataPoint->procUserModeJiffies;
    dataPoint->procKernelModeJiffies = srcCpuDataDataPoint->procKernelModeJiffies;

    #endif

    return dataPoint;
}

/*--------------------------------------------------------------------------*/

void
FreeProcCpuDataPoint( TProcCpuDataPoint* cpuDataDataPoint )
{GUCEF_TRACE;

    if ( GUCEF_NULL != cpuDataDataPoint )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
        if ( GUCEF_NULL != cpuDataDataPoint->hProcess )
        {
            ::CloseHandle( cpuDataDataPoint->hProcess );
            cpuDataDataPoint->hProcess = GUCEF_NULL;
        }
        #endif

        free( cpuDataDataPoint );
    }
}

/*--------------------------------------------------------------------------*/

UInt32
GetProcessCpuUsage( TProcessId pid                              ,
                    TProcCpuDataPoint* previousCpuDataDataPoint ,
                    TProcessCpuUsageInfo* cpuUseInfo            )
{GUCEF_TRACE;

    if ( GUCEF_NULL == previousCpuDataDataPoint || GUCEF_NULL == cpuUseInfo )
        return OSWRAP_FALSE;

    if ( previousCpuDataDataPoint->pid != pid )
        return OSWRAP_FALSE;

    memset( cpuUseInfo, 0, sizeof(TProcessCpuUsageInfo) );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    if ( GUCEF_NULL == previousCpuDataDataPoint->hProcess )
    {
        previousCpuDataDataPoint->hProcess = ::OpenProcess( PROCESS_QUERY_INFORMATION,
                                                            FALSE,
                                                            pid );
        if ( GUCEF_NULL == previousCpuDataDataPoint->hProcess )
            return OSWRAP_FALSE;
    }
    else
    {
        // Keeping the handle open between samples also keeps the process object around between CPU samplings
        // As such this may prevent us from properly letting go of the O/S resources
        // We must always check if the process exited if we are to hold onto a process handle
        DWORD exitCode = 0;
        if ( ( ::GetExitCodeProcess( previousCpuDataDataPoint->hProcess, &exitCode ) == FALSE ) || exitCode != STILL_ACTIVE )
        {
            ::CloseHandle( previousCpuDataDataPoint->hProcess );
            previousCpuDataDataPoint->hProcess = GUCEF_NULL;
            return OSWRAP_FALSE;
        }
    }

    FILETIME globalIdleTime;
    FILETIME globalKernelTime;
    FILETIME globalUserTime;
    if ( ::GetSystemTimes( &globalIdleTime, &globalKernelTime, &globalUserTime ) != TRUE )
        return OSWRAP_FALSE;

    FILETIME creationTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    if ( ::GetProcessTimes( previousCpuDataDataPoint->hProcess, &creationTime, &exitTime, &kernelTime, &userTime ) == TRUE )
    {
        cpuUseInfo->uptimeInMs = (UInt64) GetDurationSinceFiletimeInMs( &creationTime );

        UInt64 globalCpuTotal = GetFiletimeAsUInt64( &globalKernelTime ) + GetFiletimeAsUInt64( &globalUserTime );
        UInt64 prevGlobalCpuTotal = GetFiletimeAsUInt64( &previousCpuDataDataPoint->globalKernelTime ) + GetFiletimeAsUInt64( &previousCpuDataDataPoint->globalUserTime );
        Float64 globalCpuUseDelta = (Float64) ( globalCpuTotal - prevGlobalCpuTotal );
        if ( globalCpuUseDelta == 0 )
            globalCpuUseDelta = 0.01;

        UInt64 procCpuTotal = GetFiletimeAsUInt64( &kernelTime ) + GetFiletimeAsUInt64( &userTime );
        UInt64 prevProcCpuTotal = GetFiletimeAsUInt64( &previousCpuDataDataPoint->procKernelTime ) + GetFiletimeAsUInt64( &previousCpuDataDataPoint->procUserTime );
        Float64 procCpuUseDelta = (Float64) ( procCpuTotal - prevProcCpuTotal );

        cpuUseInfo->overallCpuConsumptionPercentage = procCpuUseDelta / ( globalCpuUseDelta / 100.0 );

        // Overwrite the CPU data point making current the previous for next call to this function
        previousCpuDataDataPoint->globalIdleTime = globalIdleTime;
        previousCpuDataDataPoint->globalKernelTime = globalKernelTime;
        previousCpuDataDataPoint->globalUserTime = globalUserTime;
        previousCpuDataDataPoint->procKernelTime = kernelTime;
        previousCpuDataDataPoint->procUserTime = userTime;

        return OSWRAP_TRUE;
    }

    return OSWRAP_FALSE;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    UInt64 globalJiffies = 0;
    UInt64 procKernelModeJiffies = 0;
    UInt64 procUserModeJiffies = 0;
    if ( GetGlobalJiffies( &globalJiffies ) &&
         GetProcJiffies( pid, &procUserModeJiffies, &procKernelModeJiffies ) )
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

    GetProcUptime( pid, &cpuUseInfo->uptimeInMs );

    return OSWRAP_TRUE;

    #else

    return OSWRAP_FALSE;

    #endif
}

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C TCpuDataPoint*
CreateCpuDataPoint( void )
{GUCEF_TRACE;

    TCpuDataPoint* dataPoint = (TCpuDataPoint*) malloc( sizeof( TCpuDataPoint ) );
    if ( GUCEF_NULL == dataPoint )
        return GUCEF_NULL;
    memset( dataPoint, 0, sizeof( TCpuDataPoint ) );

    dataPoint->cpuStats.logicalCpuCount = GetLogicalCPUCount();
    dataPoint->logicalCpuCount = dataPoint->cpuStats.logicalCpuCount;

    UInt32 cpuStatsDataSize = sizeof( TLogicalCpuStats ) * dataPoint->cpuStats.logicalCpuCount;
    dataPoint->cpuStats.logicalCpuStats = (TLogicalCpuStats*) malloc( cpuStatsDataSize );
    if ( GUCEF_NULL == dataPoint->cpuStats.logicalCpuStats )
    {
        free( dataPoint );
        return GUCEF_NULL;
    }
    memset( dataPoint->cpuStats.logicalCpuStats, 0, cpuStatsDataSize );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    UInt32 cpuPowerInfoDataSize = sizeof( PROCESSOR_POWER_INFORMATION ) * dataPoint->cpuStats.logicalCpuCount;
    dataPoint->cpuPowerInfo = (PPROCESSOR_POWER_INFORMATION) malloc( cpuPowerInfoDataSize );
    if ( GUCEF_NULL == dataPoint->cpuPowerInfo )
    {
        free( dataPoint->cpuStats.logicalCpuStats );
        free( dataPoint );
        return GUCEF_NULL;
    }
    memset( dataPoint->cpuPowerInfo, 0, cpuPowerInfoDataSize );

    UInt32 cpuPerfInfoDataSize = sizeof( SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION ) * dataPoint->cpuStats.logicalCpuCount;
    dataPoint->cpuPerfInfo = (SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION*) malloc( cpuPerfInfoDataSize );
    if ( GUCEF_NULL == dataPoint->cpuPerfInfo )
    {
        free( dataPoint->cpuPowerInfo );
        free( dataPoint->cpuStats.logicalCpuStats );
        free( dataPoint );
        return GUCEF_NULL;
    }
    memset( dataPoint->cpuPerfInfo, 0, cpuPerfInfoDataSize );
    dataPoint->prevCpuPerfInfo = (SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION*) malloc( cpuPerfInfoDataSize );
    if ( GUCEF_NULL == dataPoint->prevCpuPerfInfo )
    {
        free( dataPoint->cpuPerfInfo );
        free( dataPoint->cpuPowerInfo );
        free( dataPoint->cpuStats.logicalCpuStats );
        free( dataPoint );
        return GUCEF_NULL;
    }
    memset( dataPoint->prevCpuPerfInfo, 0, cpuPerfInfoDataSize );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    dataPoint->infoFromProcCpu = GUCEF_NEW AllLinuxProcCpuInfo();

    #endif

    return dataPoint;
}

/*--------------------------------------------------------------------------*/

void
FreeCpuDataPoint( TCpuDataPoint* cpuDataPoint )
{GUCEF_TRACE;

    if ( GUCEF_NULL != cpuDataPoint )
    {
        if ( GUCEF_NULL != cpuDataPoint->cpuStats.logicalCpuStats )
            free( cpuDataPoint->cpuStats.logicalCpuStats );

        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        if ( GUCEF_NULL != cpuDataPoint->cpuPowerInfo )
            free( cpuDataPoint->cpuPowerInfo );
        if ( GUCEF_NULL != cpuDataPoint->cpuPerfInfo )
            free( cpuDataPoint->cpuPerfInfo );
        if ( GUCEF_NULL != cpuDataPoint->prevCpuPerfInfo )
            free( cpuDataPoint->prevCpuPerfInfo );

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        GUCEF_DELETE cpuDataPoint->infoFromProcCpu;
        cpuDataPoint->infoFromProcCpu = GUCEF_NULL;

        #endif

        free( cpuDataPoint );
    }
}

/*--------------------------------------------------------------------------*/

UInt32
GetCpuStats( TCpuDataPoint* previousCpuDataDataPoint ,
             TCpuStats** cpuStats                    )
{GUCEF_TRACE;

    if ( GUCEF_NULL == previousCpuDataDataPoint || GUCEF_NULL == cpuStats )
        return OSWRAP_FALSE;
    *cpuStats = GUCEF_NULL;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    // For getting the overall CPU usage the recommended way is to "Use GetSystemTimes instead to retrieve this information."
    // Do note that while the below follows said recommendation from Microsoft, it is not the most accurate way to get the CPU usage relative to
    // the per core CPU usage which is also obtained and thus there may be some discrepancies between the two values.
    // Then again the NtQuerySystemInformation call is not garantueed to work as its an undocumented API call.
    FILETIME globalIdleTime;
    FILETIME globalKernelTime;
    FILETIME globalUserTime;
    if ( ::GetSystemTimes( &globalIdleTime, &globalKernelTime, &globalUserTime ) != TRUE )
        return OSWRAP_FALSE;

    UInt64 globalCpuIdle = GetFiletimeAsUInt64( &globalIdleTime );
    UInt64 prevGlobalCpuIdle = GetFiletimeAsUInt64( &previousCpuDataDataPoint->globalIdleTime );
    UInt64 globalCpuKernel = GetFiletimeAsUInt64( &globalKernelTime );
    UInt64 prevGlobalCpuKernel = GetFiletimeAsUInt64( &previousCpuDataDataPoint->globalKernelTime );
    UInt64 globalCpuUser = GetFiletimeAsUInt64( &globalUserTime );
    UInt64 prevGlobalCpuUser = GetFiletimeAsUInt64( &previousCpuDataDataPoint->globalUserTime );

    UInt64 globalCpuKernelDelta = globalCpuKernel - prevGlobalCpuKernel;
    UInt64 globalCpuUserDelta = globalCpuUser - prevGlobalCpuUser;
    UInt64 globalCpuIdleDelta = globalCpuIdle - prevGlobalCpuIdle;
    UInt64 globalCpuSystemDelta = globalCpuKernelDelta + globalCpuUserDelta;
    if ( 0 == globalCpuSystemDelta )
        globalCpuSystemDelta = 1;

    previousCpuDataDataPoint->cpuStats.cpuUsePercentage = ( (globalCpuSystemDelta - globalCpuIdleDelta) * 100 ) / ( globalCpuSystemDelta * 1.0 );

    // make the current info the 'previous' info to prepare for the next call to this function
    previousCpuDataDataPoint->globalIdleTime = globalIdleTime;
    previousCpuDataDataPoint->globalKernelTime = globalKernelTime;
    previousCpuDataDataPoint->globalUserTime = globalUserTime;

    NTSTATUS ntStatus = TryNtQuerySystemInformation( SystemProcessorPerformanceInformation                                                        ,
                                                     previousCpuDataDataPoint->cpuPerfInfo                                                        ,
                                                     sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * previousCpuDataDataPoint->logicalCpuCount ,
                                                     NULL                                                                                         );
    if ( WIN32_NT_SUCCESS( ntStatus ) )
    {
        for ( UInt32 i=0; i<previousCpuDataDataPoint->logicalCpuCount; ++i )
        {
            SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION& corePerfInfo = previousCpuDataDataPoint->cpuPerfInfo[ i ];
            SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION& prevCorePerfInfo = previousCpuDataDataPoint->prevCpuPerfInfo[ i ];

            UInt64 coreIdle = corePerfInfo.IdleTime.QuadPart;
            UInt64 prevCoreIdle = prevCorePerfInfo.IdleTime.QuadPart;
            UInt64 coreKernel = corePerfInfo.KernelTime.QuadPart;
            UInt64 prevCoreKernel = prevCorePerfInfo.KernelTime.QuadPart;
            UInt64 coreUser = corePerfInfo.UserTime.QuadPart;
            UInt64 prevCoreUser = prevCorePerfInfo.UserTime.QuadPart;

            UInt64 coreKernelDelta = coreKernel - prevCoreKernel;
            UInt64 coreUserDelta = coreUser - prevCoreUser;
            UInt64 coreIdleDelta = coreIdle - prevCoreIdle;
            UInt64 coreSystemDelta = coreKernelDelta + coreUserDelta;
            if ( 0 == coreSystemDelta )
                coreSystemDelta = 1;

            Float64 coreUsePercentage = ( (coreSystemDelta - coreIdleDelta) * 100 ) / ( coreSystemDelta * 1.0 );
            previousCpuDataDataPoint->cpuStats.logicalCpuStats[ i ].cpuUsePercentage = coreUsePercentage;
        }

        // make the current info the 'previous' info to prepare for the next call to this function
        memcpy( previousCpuDataDataPoint->prevCpuPerfInfo, previousCpuDataDataPoint->cpuPerfInfo, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * previousCpuDataDataPoint->logicalCpuCount );
    }
    else
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GetCpuStats: call to TryNtQuerySystemInformation failed with NTSTATUS " + ToString( ntStatus ) );
    }

    LONG retVal = ::CallNtPowerInformation( ProcessorInformation,
                                            NULL,
                                            0,
                                            previousCpuDataDataPoint->cpuPowerInfo,
                                            sizeof(PROCESSOR_POWER_INFORMATION) * previousCpuDataDataPoint->logicalCpuCount );
    if ( 0 == retVal )
    {
        for ( UInt32 i=0; i<previousCpuDataDataPoint->logicalCpuCount; ++i )
        {
            PROCESSOR_POWER_INFORMATION* powerInfo = &previousCpuDataDataPoint->cpuPowerInfo[ i ];
            TLogicalCpuStats* lCpuStats = &previousCpuDataDataPoint->cpuStats.logicalCpuStats[ i ];

            lCpuStats->cpuCurrentFrequencyInMhz = (Float32) powerInfo->CurrentMhz;
            lCpuStats->cpuMaxFrequencyInMhz = (Float32) powerInfo->MhzLimit;
            lCpuStats->cpuSpecMaxFrequencyInMhz = (Float32) powerInfo->MaxMhz;
        }
    }
    else
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GetCpuStats: call to CallNtPowerInformation failed with status " + ToString( retVal ) );
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    //if ( previousCpuDataDataPoint->infoFromProcCpu->RefreshFromOS() )
    if ( GUCEF_NULL != previousCpuDataDataPoint->infoFromProcCpu )
    {
        for ( UInt32 i=0; i<previousCpuDataDataPoint->cpuStats.logicalCpuCount; ++i )
        {
            LinuxLogicalCpuInfo* linuxLCpuInfo = &previousCpuDataDataPoint->infoFromProcCpu->allCpuInfo[ i ];
            if ( GUCEF_NULL != linuxLCpuInfo )
            {
                TLogicalCpuStats* lCpuStats = &previousCpuDataDataPoint->cpuStats.logicalCpuStats[ i ];
                memset( lCpuStats, 0, sizeof(TLogicalCpuStats) );

                lCpuStats->cpuCurrentFrequencyInMhz = linuxLCpuInfo->cpuMhz;
                lCpuStats->cpuMaxFrequencyInMhz = 0.0;
                lCpuStats->cpuSpecMaxFrequencyInMhz = 0.0;
            }
        }
    }

    #endif

    *cpuStats = &previousCpuDataDataPoint->cpuStats;
    return OSWRAP_TRUE;
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
