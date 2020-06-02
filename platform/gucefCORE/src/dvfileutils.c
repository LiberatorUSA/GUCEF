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

#include <malloc.h>             /* memory management */
#include <stdio.h>              /* standard I/O utils */
#include <string.h>             /* standard string utils */
#include <assert.h>
#include <limits.h>

#ifndef GUCEF_CORE_CONFIG_H
#include "gucefCORE_config.h"     /* build defines */
#define GUCEF_CORE_CONFIG_H
#endif /* GUCEF_CORE_CONFIG_H ? */

#include "dvfileutils.h"	/* function prototypes */

#include "dvstrutils.h"         /* My own string utils */
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #include <windows.h>		/* WIN32 API */
  #undef min
  #undef max
  /* #include <dir.h>: obsolete *//* needed for MAXFILE define */
  #include <io.h>                 /* Dir itteration: findfirst ect. */
  #include <direct.h>             /* dir tools */
  #define MAX_DIR_LENGTH MAX_PATH
#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
  #include <dirent.h>             /* needed for dirent strcture */
  #include <unistd.h>             /* POSIX utilities */
  #include <limits.h>             /* Linux OS limits */
  #include <sys/stat.h>           /* needed for stat function */
  #include <errno.h>
  #include <fcntl.h>
  #define MAX_DIR_LENGTH PATH_MAX
#else
  #error Unsupported OS
#endif
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )
  #include <android/log.h>
#endif

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

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

/*
 *	Structure used to store dir itteration data so it can be passed between
 *	the DI_ functions in a OS independant manner.
 */
struct SDI_Data
{
 	#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

	Int32 find_handle;        /* Unique handle identifying the file or set of files that resulted from a findfirst with the filter provided */
	struct _finddata_t  find; /* struct that stores entry data */

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    DIR *dir;                 /* Directory stream */
    struct dirent *entry;     /* Pointer needed for functions to iterating directory entries. Stores entry name which is used to get stat */
    struct stat statinfo;     /* Struct needed for determining info about an entry with stat(). */

    #else

    /* -> empty struct because we don't support other OS's atm */
    #error Unsupported OS

    #endif
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SDI_Data*
DI_First_Dir_Entry( const char *path )
{
    /*
     *	Function that should be used for directory entry itteration.
     *	All the functions listed here with a DI_ prefix belong together and
     *	combined allow you to itterate trough a directory in a cross-platform
     *	manner. Note that the DI_ functions are NOT threadsafe.
     *
     *	This function allocates data storage for the dir itteration process.
     *	In case of error NULL is returned.
     */
    struct SDI_Data *data;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    char *tmp_path;

    #endif /* GUCEF_PLATFORM_MSWIN ? */

    if ( NULL == path ) return NULL;

    /*
     *	Allocate data storage.
     */
    data = ( struct SDI_Data* ) malloc( sizeof( struct SDI_Data ) );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    /*
     *	In Win32 we use _findfirst ect. because even though the posix
     *	functions are supported on windows NT they are not supported on
     *	Win98 and WinME and these functions are. No support for Win95 or
     *	older though.
     */
    tmp_path = (char*)calloc( strlen( path )+5, sizeof( char ) );
    strcpy( tmp_path, path );
    Append_To_Path( tmp_path, "*.*\0" );
    data->find_handle = (Int32) _findfirst( tmp_path, &data->find );
    free( tmp_path );

    /*
     *	Check if findfirst was successful
     */
    if ( data->find_handle == -1 )
    {
        /*
         *	There was an error
         */
        _findclose( data->find_handle );
        free( data );
        return NULL;
    }

    /*
     *	Successfully obtained first entry so we return the struct
     *	pointer
     */
    return data;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    /*
     *	In Linux we use POSIX functions because these are independant of
     *	the Linux distribution. It may also provide use with support for
     *	other Unix based systems.
     */

    /*
     *	Attempt to open the directory
     */
    data->dir = opendir( path );
    if ( NULL == data->dir )
    {
    	/*
         *	Could not open directory
         */
        free( data );
	    return NULL;
    }

    /*
     *	change working dir to be able to read file information
     */
    chdir( path );

    /*
     *	Read first entry
     */
    data->entry = readdir( data->dir );
    while( data->entry )
    {
        /*
         *	Get info on the entry.
         *	We only want regular files and directory entry's. We ignore the
         *	rest.
         */
        stat( data->entry->d_name, &data->statinfo );
        if ( S_ISREG( data->statinfo.st_mode ) || S_ISDIR( data->statinfo.st_mode ) )
        {
        	/*
             *	We found either a regular file or a directory
             *	entry which is now our current entry.
             */
            return data;
        }

        /*
         *	This entry is not what we want,.. skip to the next entry
         */
        data->entry = readdir( data->dir );
    }

    /*
     *	there was an error reading the entry data or no entry was found
     *	on the path specified that was a regular file or directory.
     */
    if ( NULL != data->dir )
    {
        closedir( data->dir );
    }
    free( data );
    return NULL;

    #else

    /*
     *	Unsupported O/S build
     */
    return NULL;

    #endif
}

/*-------------------------------------------------------------------------*/

UInt32
DI_Next_Dir_Entry( struct SDI_Data *data )
{
    /*
     *	Function that should be used for directory entry itteration.
     *	All the functions listed here with a DI_ prefix belong together and
     *	combined allow you to itterate trough a directory in a cross-platform
     *	manner. Note that the DI_ functions are NOT threadsafe.
     *
     *	Function that selects the next directory entry. If there are no more
     *	directory entry's available ie we itterated over all entry's then
     *	0 is returned in which case you should call DI_Cleanup(),
     *	otherwise 1 is returned.
     */
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return !_findnext( data->find_handle, &data->find );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    /*
     *	Read next entry
     */
    data->entry = readdir( data->dir );
    while( data->entry )
    {
        /*
         *	Get info on the entry.
         *	We only want regular files and directory entry's. We ignore the
         *	rest.
         */
        stat( data->entry->d_name, &data->statinfo );
        if ( S_ISREG( data->statinfo.st_mode ) || S_ISDIR( data->statinfo.st_mode ) )
        {
        	/*
             *	We found either a regular file or a directory
             *	entry which is now our current entry.
             */
            return 1;
        }

        /*
         *	This entry is not what we want,.. skip to the next entry
         */
        data->entry = readdir( data->dir );
    }

    /*
     *	Could not find any other entry's that where either a regular file
     *	or a directory.
     */
    return 0;

    #else

        /*
         *	Unsupported O/S build
         */
    #endif
}

/*-------------------------------------------------------------------------*/

UInt32
DI_Timestamp( struct SDI_Data *data )
{
    /*
     *	Function that should be used for directory entry itteration.
     *	All the functions listed here with a DI_ prefix belong together and
     *	combined allow you to itterate trough a directory in a cross-platform
     *	manner. Note that the DI_ functions are NOT threadsafe.
     *
     *	Returns the dir entry name of the current directory entry. This may be
     *	a directory name or a filename. Use DI_Is_It_A_File() to determine which
     */
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return (UInt32)data->find.time_write;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return data->statinfo.st_mtime;

    #else

    /*
     *	Unsupported O/S build
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

UInt32
DI_Size( struct SDI_Data *data )
{
    /*
     *	Function that should be used for directory entry itteration.
     *	All the functions listed here with a DI_ prefix belong together and
     *	combined allow you to itterate trough a directory in a cross-platform
     *	manner. Note that the DI_ functions are NOT threadsafe.
     *
     *	Returns the size of the current entry which in the case of a file is the
     *	filesize.
     */
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return data->find.size;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return data->statinfo.st_size;

    #else

    /*
     *	Unsupported O/S build
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

UInt32
DI_Is_It_A_File( struct SDI_Data *data )
{
    /*
     *	Function that should be used for directory entry itteration.
     *	All the functions listed here with a DI_ prefix belong together and
     *	combined allow you to itterate trough a directory in a cross-platform
     *	manner. Note that the DI_ functions are NOT threadsafe.
     *
     *	Returns boolean indicating wheter the current entry is a directory or
     *	a file.
     */
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return !( data->find.attrib & _A_SUBDIR );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return S_ISREG( data->statinfo.st_mode );

    #else

    /*
     *	Unsupported O/S build
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

const char*
DI_Name( struct SDI_Data *data )
{
    /*
     *	Function that should be used for directory entry itteration.
     *	All the functions listed here with a DI_ prefix belong together and
     *	combined allow you to itterate trough a directory in a cross-platform
     *	manner. Note that the DI_ functions are NOT threadsafe.
     *
     *	Returns the dir entry name of the current directory entry. This may be
     *	a directory name or a filename. Use DI_Is_It_A_File() to determine which
     */
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return data->find.name;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return data->entry->d_name;

    #else

    /*
     *	Unsupported O/S build
     */
    return NULL;

    #endif
}

/*-------------------------------------------------------------------------*/

void
DI_Cleanup( struct SDI_Data *data )
{
    /*
     *	Function that should be used for directory entry itteration.
     *	All the functions listed here with a DI_ prefix belong together and
     *	combined allow you to itterate trough a directory in a cross-platform
     *	manner. Note that the DI_ functions are NOT threadsafe.
     *
     *	De-allocates data storage used for dir itteration which was created by
     *	a call to DI_First_Dir_Entry().
     */
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    _findclose( data->find_handle );
    free( data );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( NULL != data->dir )
    {
        closedir( data->dir );
    }
    free( data );

    #else

    /*
     *	Unsupported O/S build
     */

    #endif
}

/*-------------------------------------------------------------------------*/

char*
Get_Current_Dir( char* dest_buffer, UInt32 buf_length )
{
    /*
     *	Function that returns the current directory
     */
    if ( !dest_buffer ) return NULL;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return _getcwd( dest_buffer, buf_length );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    /*
     *	This call can actually fail: if another process has succeeded
     *	in removing the current directory of a process
     */
    return getcwd( dest_buffer, buf_length );

    #else

    /*
     *	Unsupported O/S build
     */
    return NULL;

    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *	Function that returns maximum possible length of a directory on the
 *	current O/S.
 */
UInt32
Max_Dir_Length( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return MAX_PATH;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return PATH_MAX;

    #else

    /*
     *	Unsupported O/S build
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

UInt32
Max_Filename_Length( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return _MAX_FNAME;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return NAME_MAX+1;

    #else

    /*
     *	Unsupported O/S build
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

#if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

static int
recursive_mkdir( const char* dir, int accessPerms )
{
    char tmp[ PATH_MAX ];
    char *p = NULL;
    size_t len;
    int retValue=0;

    snprintf( tmp, sizeof(tmp), "%s", dir );
    len = strlen( tmp );
    if( tmp[ len-1 ] == '/' )
    {
        tmp[ len-1 ] = 0;
    }
    for( p=tmp+1; *p; ++p )
    {
        if( *p == '/' )
        {
            *p = 0;
            retValue = mkdir( tmp, accessPerms );
            if ( 0 != retValue )
            {
                if ( EEXIST != errno )
                {
                    return retValue;
                }
            }
            *p = '/';
        }
    }
    retValue = mkdir( tmp, accessPerms );
    if ( 0 != retValue )
    {
        if ( EEXIST != errno )
        {
            return retValue;
        }
        else return 0;
    }
    return retValue;
}

#endif

/*-------------------------------------------------------------------------*/

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

/**
 *	Recursive function that creates directories
 */
static UInt32
create_directory( const char *new_dir, UInt32 offset )
{
    Int32 idx = _Find_Char( offset, 1, '\\', new_dir, (UInt32)strlen( new_dir ) );
    if ( idx > 0 )
    {
        char* dir = NULL;

        /*
         *      Check for drive letter.
         */
        if ( ( idx > 1 ) && ( !offset ) )
        {
            if ( *(new_dir+1) == ':' )
            {
                return create_directory( new_dir, 3 );
            }
        }

        /*
         *	Sub-dir found
         */
        dir = (char*)calloc( idx+2, 1 );
        strncpy( dir, new_dir, idx );
        dir[ idx+1 ] = '\0';
        if ( !CreateDirectory( dir, NULL ) )
        {
            /*
            *	An error occurred.
            *	We will ignore the dir already exists error but
            *	abort on all others
            */
            if ( GetLastError() != ERROR_ALREADY_EXISTS )
            {
                free( dir );
                return 0;
            }
        }
        free( dir );

        return create_directory( new_dir, idx+1 );
    }
    else
    {
             if ( !CreateDirectory( new_dir, NULL ) )
             {
             	/*
                     *	An error occured.
                     *	We will ignore the dir already exists error but
                     *	abort on all others
                     */
             	if ( GetLastError() != ERROR_ALREADY_EXISTS )
                    {
                    	return 0;
                    }
                    return 1;
             }
             else
             return 1;
    }
}

#endif /* GUCEF_PLATFORM_MSWIN */

/*-------------------------------------------------------------------------*/

UInt32
Create_Directory( const char *new_dir )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return create_directory( new_dir, 0 );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    /*
     *	Use posix function. returns -1 on failure and 0 on sucess
     */
    return (UInt32) ( recursive_mkdir( new_dir, 0777 )+1 );

    #else

    /*
     *	Unsupported O/S build
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

UInt32
Create_Path_Directories( const char* path )
{
    if ( NULL != path )
    {
        int i, pathLength = strlen( path );
        char* pathBuffer = (char*) malloc( pathLength+1 );
        UInt32 returnValue = 0;
        memcpy( pathBuffer, path, pathLength+1 );

        i=pathLength;
        while ( i > 0 )
        {
            if ( pathBuffer[ i ] == '/' || pathBuffer[ i ] == '\\' )
            {
                pathBuffer[ i ] = '\0';
                break;
            }
            --i;
        }
        returnValue = Create_Directory( pathBuffer );
        free( pathBuffer );
        return returnValue;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

/**
 *      Function that attempts to remove the specified directory.
 *      The del_files parameter allows you to specify wheter you wish to delete
 *      all the files in the directory before deleting the directory itself.
 *      If false and files or dirs are found the directory removal will fail.
 */
UInt32
Remove_Directory( const char *dir  ,
                  UInt32 del_files )
{

    if ( del_files )
    {
        /*
         *      The user wishes to delete the directory even if the
         *      directory is not empty. We will proceed to delete all
         *      files in the directory first.
         */
        struct SDI_Data *ddata = DI_First_Dir_Entry( dir );
        UInt32 more = 1;
        if ( !ddata ) return 0;

        while ( more )
        {
            if ( DI_Is_It_A_File( ddata ) )
            {
                /*
                 *      Attempt to delete the file we found in
                 *      the directory.
                 */
                if ( !Delete_File( DI_Name( ddata ) ) ) return 0;
            }
            else
            {
                /*
                 *      Attempt to delete the directory we found
                 *      and all files inside it.
                 */
                char *subdir = calloc( strlen( dir ) + strlen( DI_Name( ddata ) )+1, 1 );
                strcpy( subdir, dir );
                Append_To_Path( subdir, DI_Name( ddata ) );
                if ( !Remove_Directory( subdir, del_files ) )
                {
                    free( subdir );
                    return 0;
                }
                free( subdir );
            }

            /*
             *      Itterate to next directory entry
             */
            more = DI_Next_Dir_Entry( ddata );
        }

        /*
         *      Clean-up our dir itterator
         */
        DI_Cleanup( ddata );
    }

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    /*
     *      Attempt to remove the directory itself. We can call the WIN32
     *      function directly here because it won't delete any files in the
     *      dir.
     */

    return RemoveDirectory( dir );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return 0 == rmdir( dir ) ? 1 : 0;

    #else

    /*
     *      Not implemented for your O/S
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *      Function that lets you obtain the path to the module that houses this
 *      function. So for example if you are calling this function from your
 *      application while the function is part of a dll you use then the path
 *      to the dll is returned. If the dll is the the same dir as the executable
 *      then this function offers a good portable method to get the path to
 *      the application.
 */
UInt32
Module_Path( char *dest, UInt32 dest_size )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return !GetModuleFileName( NULL, dest, dest_size );

    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_APPLE )

    std::string get_basepath() {
    std::string path = "./";
    ProcessSerialNumber PSN;
    ProcessInfoRec pinfo;
    FSSpec pspec;
    FSRef fsr;
    OSStatus err;
    /* set up process serial number */
    PSN.highLongOfPSN = 0;
    PSN.lowLongOfPSN = kCurrentProcess;
    /* set up info block */
    pinfo.processInfoLength = sizeof(pinfo);
    pinfo.processName = NULL;
    pinfo.processAppSpec = &pspec;
    /* grab the vrefnum and directory */
    err = GetProcessInformation(&PSN, &pinfo);
    if (! err ) {
    char c_path[2048];
    FSSpec fss2;
    int tocopy;
    err = FSMakeFSSpec(pspec.vRefNum, pspec.parID, 0, &fss2);
    if ( ! err ) {
    err = FSpMakeFSRef(&fss2, &fsr);
    if ( ! err ) {
    char c_path[2049];
    err = (OSErr)FSRefMakePath(&fsr, (UInt8*)c_path, 2048);
    if (! err ) {
    path = c_path;
    }
    }
    }
    }

    return (path);
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    pid_t pid = getpid();

    char linkStr[ 24 ];
    sprintf( linkStr, "%s%d%s", "/proc/", pid, "/exe\0" );

    int ch = readlink( linkStr, dest, dest_size );
    if ( -1 != ch )
    {
        dest[ ch ] = 0;
        return 0;
    }
    return 1;

    #else

    /*
     *  Not supported
     */
    return 0;

    #endif

}

/*-------------------------------------------------------------------------*/

UInt32
Delete_File( const char *filename )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return DeleteFile( filename );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return 0 == remove( filename ) ? 1 : 0;

    #else

    /*
     *  Not supported
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

#if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

ssize_t
do_sendfile( int out_fd, int in_fd, off_t offset, size_t count )
{
    ssize_t bytes_sent;
    size_t total_bytes_sent = 0;
    while ( total_bytes_sent < count )
    {
        if ( ( bytes_sent = sendfile( out_fd,
                                      in_fd,
                                      &offset,
                                      count - total_bytes_sent ) ) <= 0 )
        {
            if ( errno == EINTR || errno == EAGAIN )
            {
                // Interrupted system call/try again
                // Just skip to the top of the loop and try again
                continue;
            }
            return -1;
        }
        total_bytes_sent += bytes_sent;
    }
    return total_bytes_sent;
}

#endif

/*-------------------------------------------------------------------------*/

UInt32
Copy_File( const char *dst, const char *src )
{
    if ( 0 == Create_Path_Directories( dst ) ) return 0;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return 0 != CopyFile( src, dst, TRUE ) ? 1 : 0;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    {

    int read_fd;
    int write_fd;
    struct stat stat_buf;
    off_t offset = 0;

    /* Open the input file. */
    read_fd = open( src, O_RDONLY );
    if ( read_fd == -1 ) return 0;

    /* Stat the input file to obtain its size. */
    if ( fstat( read_fd, &stat_buf ) == -1 )  return 0;

    /*
     *  Open the output file for writing, with the same permissions as the
     *  source file.
     */
    write_fd = open( dst, O_WRONLY | O_CREAT, stat_buf.st_mode );
    if ( write_fd == -1 )  return 0;

    /* Blast the bytes from one file to the other. */
    if ( -1 == do_sendfile( write_fd, read_fd, offset, stat_buf.st_size ) )
    {
        close( read_fd );
        close( write_fd );
        return 0;
    }

    /* Close up. */
    close( read_fd );
    close( write_fd );

    }

    return 1;

    #else

    {

    char buffer[ 1024*512 ];
    UInt32 rbytes = 1;
    FILE *dst_fptr, *src_fptr = fopen( src, "rb" );
    if ( !src_fptr ) return 0;
    dst_fptr = fopen( dst, "wb" );
    if ( !dst_fptr )
    {
    	fclose(  src_fptr );
    	return 0;
    }
    while ( rbytes )
    {
    	rbytes = (UInt32)fread( buffer, 1, 1024*512, src_fptr );
            if ( rbytes )
            if ( !(rbytes == fwrite( buffer, 1, 1024*512, dst_fptr )) )
            {
            	/*
                 *	Problem writing file section
                 */
                fclose(  dst_fptr );
                fclose(  src_fptr );
                return 0;
            }
    }

    }

    return 1;

    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *	moves a file from one location to the other
 *	If successfull true (1) is returned, otherwise false (0).
 */
UInt32
Move_File( const char *dst, const char *src )
{
    if ( 0 == Create_Path_Directories( dst ) ) return 0;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return MoveFile( src, dst );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    struct stat originalPermissions;
    if ( 0 != stat( src, &originalPermissions ) ) return 0;

    chmod( src, 0777 );

    if ( 0 != rename( src, dst ) )
    {
        return 0;
    }

    chmod( dst, originalPermissions.st_mode );
    return 1;

    #else

    if ( 0 != Copy_File( dst, src ) )
    {
    	return Delete_File( src );
    }
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

static HANDLE
ExecuteProgramEx( const char *filename,
                  const char *cmdline )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    BOOL bres;
    char *tmp_lstr;
    si.cb = sizeof( si );
    si.lpReserved = NULL;
    si.lpDesktop = NULL;
    si.lpTitle = NULL,
    si.dwFlags = STARTF_FORCEONFEEDBACK;
    si.cbReserved2 = 0;
    si.lpReserved2 = NULL;
    if ( cmdline )
    {
        /*
         *      We need to concatonize the filename and cmdline strings
         *      We can't just pass them seperatly because winblows
         *      does not follow ANSI rules and will allow you to override
         *      argv[ 0 ] by passing them seperatly. This can have verry
         *      unexpected results.
         */
        tmp_lstr = calloc( strlen( filename )+strlen( cmdline )+2, 1 );
        sprintf( tmp_lstr, "%s %s", filename, cmdline );
        bres = CreateProcess( NULL, tmp_lstr, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        free( tmp_lstr );
    }
    else
    {
        bres = CreateProcess( filename, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    }
    if ( bres ) return pi.hProcess;
    return NULL;
}

#endif /* GUCEF_PLATFORM_MSWIN ? */

/*-------------------------------------------------------------------------*/

/**
 *      Function that will attempt to execute the given program.
 *      If successfull true (1) is returned, otherwise false (0).
 */
UInt32
Execute_Program( const char *filename ,
                 const char *cmdline  )
{
    if ( NULL != filename )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        HANDLE phdl = ExecuteProgramEx( filename, cmdline );
        if ( phdl )
        {
            CloseHandle( phdl );
            return 1;
        }
        return 0;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )


        #endif
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
Filesize( const char *filename )
{
    if ( NULL != filename )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        UInt32 lfilesize;
        WIN32_FIND_DATA FileInfo;
        HANDLE hFind;
        hFind = FindFirstFile( filename, &FileInfo );
        if ( hFind == INVALID_HANDLE_VALUE )
        {
            lfilesize = 0;
        }
        else
        {
            lfilesize = FileInfo.nFileSizeLow;
        }
        FindClose( hFind );
        return lfilesize;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        struct stat buf;
        UInt32 filesize;
        int result;
        result = stat( filename, &buf );
        if ( result == 0 ) return buf.st_size;
        return 0;

        #else

        UInt32 filesize = 0;
        FILE *fptr = fopen( filename, "rb" );
        fseek( fptr, 0, SEEK_END );
        filesize = ftell( fptr );
        fclose( fptr );
        return filesize;

        #endif
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
File_Exists( const char *filename )
{
    if ( NULL != filename )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        WIN32_FIND_DATA FileInfo;
        HANDLE hFind;
        hFind = FindFirstFile( filename, &FileInfo );
        if ( hFind != INVALID_HANDLE_VALUE )
        {
                FindClose( hFind );

                /* make sure we found a file not a directory */
                return !( FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY );
        }
        return 0;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        struct stat buf;
        return stat( filename, &buf ) == 0;

        #else

        FILE *fptr = fopen( filename, "rb" );
        fclose( fptr );
        return fptr > 0;

        #endif
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
Dir_Exists( const char *path )
{
    if ( GUCEF_NULL != path )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        WIN32_FIND_DATA FileInfo;
        HANDLE hFind = GUCEF_NULL;
        hFind = FindFirstFile( path, &FileInfo );
        if ( hFind != INVALID_HANDLE_VALUE )
        {
            FindClose( hFind );

            /* make sure we found a directory not a file */
            return ( FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY );
        }
        return 0;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        struct stat buf;
        if ( stat( filename, &buf ) == 0 )
            if ( buf.st_mode & S_IFDIR != 0 )
                return 1;
        return 0;

        #else

        FILE *fptr = fopen( filename, "rb" );
        fclose( fptr );
        return fptr > 0;

        #endif
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

/**
 *      Counts the number of $UPDIR$ segments from the start
 *      of the given string
 */
UInt32
GetUpCount( const char* pathstr ,
            UInt32 pstrlen      ,
            UInt32* offset      )
{
    const char* cp = pathstr;
    Int32 i, max = pstrlen - 7;
    UInt32 count=0;

    *offset = 0;
    for ( i=0; i<max; ++i )
    {
        if ( memcmp( cp, "$UPDIR$", 7 ) == 0 )
        {
            ++count;
            *offset = i+7;
        }
        ++cp;
    }
    return count;
}

/*-------------------------------------------------------------------------*/

/**
 *      Writes a relative path into dest build from pathstr.
 *      The following prefix tags can be used:
 *        $CURWORKDIR$ : this will be replaced with the current working dir.
 *        $MODULEDIR$ : this will be replaced with the module dir.
 *      The following tag can be placed after one of the above mentioned tags
 *        $UPDIR$ : go up one dir
 *      Dir seperator chars will be forced to that of the the target O/S.
 *      It is assumed dest is large enough to store the data.
 *      The function returns the total number of bytes written to dest.
 */
UInt32
Relative_Path( const char *pathstr ,
               char *dest          ,
               UInt32 buffersize   )
{
        #define MAXPATHTAGLENGTH 11

        UInt32 tagfound = 0;
        char tmpbuffer[ MAX_DIR_LENGTH ];
        const char* addition = "\0";
        UInt32 i, upOffset=0, upcount=0, pstrlen = (UInt32) strlen( pathstr );

        /*
         *      Check for the root dir tags that are to be prefixed
         */
        if ( pstrlen >= 11 )
        {
                if ( memcmp( pathstr, "$MODULEDIR$", 11 ) == 0 )
                {
                        Module_Path( tmpbuffer      ,
                                     MAX_DIR_LENGTH );
                        Strip_Filename( tmpbuffer );
                        upcount = GetUpCount( pathstr+11 ,
                                              pstrlen-11 ,
                                              &upOffset  );
                        addition = pathstr+11+upOffset;
                        tagfound = 1;
                }
                else
                if ( pstrlen >= 12 )
                {
                        if ( memcmp( pathstr, "$CURWORKDIR$", 12 ) == 0 )
                        {
                                Get_Current_Dir( tmpbuffer      ,
                                                 MAX_DIR_LENGTH );
                                upcount = GetUpCount( pathstr+12 ,
                                                      pstrlen-12 ,
                                                      &upOffset  );
                                addition = pathstr+12+upOffset;
                                tagfound = 1;
                        }
                }
        }

        if ( !tagfound )
        {
                UInt32 max = pstrlen+1;
                if ( max > buffersize ) max = buffersize;
                memcpy( dest, pathstr, max );
                return max;
        }

        /*
         *      Apply the $UPDIR$ tags
         */
        for ( i=0; i<upcount; ++i )
        {
                Strip_Last_Subdir( tmpbuffer );
        }


        /*
         *      Combine the parsed tag section with the rest of the path
         */
        Append_To_Path( tmpbuffer ,
                        addition  );

        /*
         *      Copy into the buffer provided by the user
         */
        i = (UInt32) strlen(tmpbuffer)+1;
        memcpy( dest      ,
                tmpbuffer ,
                i         );

        return i;
}

/*-------------------------------------------------------------------------*/

UInt32
Is_Path_Valid( const char* path )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return GetFileAttributes( path ) != INVALID_FILE_ATTRIBUTES;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    // @TODO

    #else

    /*
     *  Unsupported platform
     */
    return 0;

    #endif
}

/*-------------------------------------------------------------------------*/

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

time_t
FileTimeToUnixTime( const FILETIME* ft )
{
	/* the reverse of http://support.microsoft.com/kb/167296/en-us */
	ULONGLONG ull = *(const ULONGLONG*)((void*)(ft));
	ull -= 116444736000000000;
	ull /= 10000000;
	assert(ull < ULONG_MAX);
	return (time_t)(ull);
}

#endif

/*-------------------------------------------------------------------------*/

time_t
Get_Modification_Time( const char* path )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    WIN32_FILE_ATTRIBUTE_DATA data;
    if ( 0 != GetFileAttributesEx( path, GetFileExInfoStandard, &data ) )
    {
        return FileTimeToUnixTime( &data.ftLastWriteTime );
    }
    return -1;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    struct stat buf;

    /* Get File Statistics for stat.c. */
    if( stat( path, &buf ) == 0 )
    {
        /* get the date/time last modified */
        return buf.st_mtime;
    }
    return -1;

    #else

    /*
     *  Unsupported platform
     */
    return (time_t) 0;

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

/*-------------------------------------------------------------------------*/
