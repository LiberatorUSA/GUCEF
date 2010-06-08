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

#include <malloc.h>		/* memory management */
#include <stdio.h>              /* standard I/O utils */
#include <string.h>             /* standard string utils */
#include <assert.h>

#ifndef GUCEF_CORE_CONFIG_H
#include "gucefCORE_config.h"     /* build defines */
#define GUCEF_CORE_CONFIG_H
#endif /* GUCEF_CORE_CONFIG_H ? */

#include "dvfileutils.h"	/* function prototypes */

#include "dvstrutils.h"         /* My own string utils */
#ifdef GUCEF_MSWIN_BUILD
  #include <windows.h>		/* WIN32 API */
  #undef min
  #undef max
  /* #include <dir.h>: obsolete *//* needed for MAXFILE define */
  #include <io.h>                 /* Dir itteration: findfirst ect. */
  #include <direct.h>             /* dir tools */
  #define MAX_DIR_LENGTH MAX_PATH
#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
  #include <dirent.h>             /* needed for dirent strcture */
  #include <unistd.h>             /* POSIX utilities */
  #include <limits.h>             /* Linux OS limits */
  #include <sys/stat.h>           /* needed for stat function */
  #include <errno.h>
  #define MAX_DIR_LENGTH PATH_MAX
#else
  #error Unsupported OS
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
 	#ifdef GUCEF_MSWIN_BUILD
	Int32 find_handle;        /* Unique handle identifying the file or set of files that resulted from a findfirst with the filter provided */
	struct _finddata_t  find; /* struct that stores entry data */
        #else
        DIR *dir;                 /* Directory stream */
        struct dirent *entry;     /* Pointer needed for functions to iterating directory entries. Stores entry name which is used to get stat */
        struct stat statinfo;     /* Struct needed for determining info about an entry with stat(). */
        #ifdef GUCEF_LINUX_BUILD
        #else
        /* -> empty struct because we don't support other OS's atm */
        #endif /* GUCEF_LINUX_BUILD ? */
        #endif /* WIN32_BUILD ? */
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
        #ifdef GUCEF_MSWIN_BUILD
        char *tmp_path;
        #endif /* WIN32_BUILD ? */

        if ( !path ) return NULL;

        /*
         *	Allocate data storage.
         */
        data = ( struct SDI_Data* ) malloc( sizeof( struct SDI_Data ) );

        #ifdef GUCEF_MSWIN_BUILD

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

        #else
        #ifdef GUCEF_LINUX_BUILD

        /*
         *	In Linux we use POSIX functions because these are independant of
         *	the Linux distribution. It may also provide use with support for
         *	other Unix based systems.
         */

        /*
         *	Attempt to open the directory
         */
        if ( ( data->dir = opendir( path ) ) == NULL )
        {
        	/*
                 *	Could not open directory
                 */
                closedir( data->dir );
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
        closedir( data->dir );
        free( data );
        return NULL;

        #else

        /*
         *	Unsupported O/S build
         */
        return NULL;

        #endif /* GUCEF_LINUX_BUILD */
        #endif /* WIN32_BUILD */
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
        #ifdef GUCEF_MSWIN_BUILD
        return !_findnext( data->find_handle, &data->find );
	#else
        #ifdef GUCEF_LINUX_BUILD

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

        #endif /* GUCEF_LINUX_BUILD ? */
        #endif /* WIN32_BUILD ? */
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
        #ifdef GUCEF_MSWIN_BUILD
        return (UInt32)data->find.time_write;
	#else
        #ifdef GUCEF_LINUX_BUILD
        return data->statinfo.st_mtime;
        #else

        /*
         *	Unsupported O/S build
         */

        #endif /* GUCEF_LINUX_BUILD ? */
        #endif /* WIN32_BUILD ? */
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
        #ifdef GUCEF_MSWIN_BUILD
        return data->find.size;
	#else
        #ifdef GUCEF_LINUX_BUILD
        return data->statinfo.st_size;
        #else

        /*
         *	Unsupported O/S build
         */

        #endif /* GUCEF_LINUX_BUILD ? */
        #endif /* WIN32_BUILD ? */
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
        #ifdef GUCEF_MSWIN_BUILD
        return !( data->find.attrib & _A_SUBDIR );
	#else
        #ifdef GUCEF_LINUX_BUILD
        return S_ISREG( data->statinfo.st_mode );
        #else

        /*
         *	Unsupported O/S build
         */

        #endif /* GUCEF_LINUX_BUILD ? */
        #endif /* WIN32_BUILD ? */
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
        #ifdef GUCEF_MSWIN_BUILD
        return data->find.name;
	#else
        #ifdef GUCEF_LINUX_BUILD
        return data->entry->d_name;
        #else

        /*
         *	Unsupported O/S build
         */

        #endif /* GUCEF_LINUX_BUILD ? */
        #endif /* WIN32_BUILD ? */
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
        #ifdef GUCEF_MSWIN_BUILD
        _findclose( data->find_handle );
        free( data );
	#else
        #ifdef GUCEF_LINUX_BUILD
	closedir( data->dir );
        free( data );
        #else

        /*
         *	Unsupported O/S build
         */

        #endif /* GUCEF_LINUX_BUILD ? */
        #endif /* WIN32_BUILD ? */
}

/*-------------------------------------------------------------------------*/

char*
Get_Current_Dir( char* dest_buffer, UInt32 buf_length )
{
        /*
         *	Function that returns the current directory
         */
        if ( !dest_buffer ) return NULL;

        #ifdef GUCEF_MSWIN_BUILD
        return _getcwd( dest_buffer, buf_length );
        #endif /* WIN32_BUILD */
        #ifdef GUCEF_LINUX_BUILD

        /*
         *	This call can actually fail: if another process has succeeded
         *	in removing the current directory of a process
         */
        return getcwd( dest_buffer, buf_length );
        #endif /* GUCEF_LINUX_BUILD */
}

/*-------------------------------------------------------------------------*/

/**
 *	Function that returns maximum possible length of a directory on the
 *	current O/S.
 */
UInt32
Max_Dir_Length( void )
{
        #ifdef GUCEF_MSWIN_BUILD
        return MAX_PATH;
        #endif /* WIN32_BUILD */
        #ifdef GUCEF_LINUX_BUILD
        return PATH_MAX;
        #endif /* GUCEF_LINUX_BUILD */
}

/*-------------------------------------------------------------------------*/

UInt32
Max_Filename_Length( void )
{
        #ifdef GUCEF_MSWIN_BUILD
        return _MAX_FNAME;
        /*return MAXFILE+MAXEXT; */
        #endif /* WIN32_BUILD */
        #ifdef GUCEF_LINUX_BUILD
        return NAME_MAX+1;
        #endif /* GUCEF_LINUX_BUILD */
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
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
#endif /* WIN32_BUILD */

/*-------------------------------------------------------------------------*/

UInt32
Create_Directory( const char *new_dir )
{
        #ifdef GUCEF_MSWIN_BUILD
	return create_directory( new_dir, 0 );
        #endif /* WIN32_BUILD */
        #ifdef GUCEF_LINUX_BUILD
        /*
         *	Use posix function. returns -1 on failure and 0 on sucess
         */
        return mkdir( new_dir, 0777 )+1;
        #endif /* GUCEF_LINUX_BUILD */
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

        #ifdef GUCEF_MSWIN_BUILD
        /*
         *      Attempt to remove the directory itself. We can call the WIN32
         *      function directly here because it won't delete any files in the
         *      dir.
         */
        return RemoveDirectory( dir );
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
        #ifdef GUCEF_MSWIN_BUILD
        return !GetModuleFileName( NULL, dest, dest_size );
        #else
        #ifdef GUCEF_LINUX_BUILD
        #endif /* WIN32_BUILD ? */
        #endif /* GUCEF_LINUX_BUILD ? */
}

/*-------------------------------------------------------------------------*/

/**
 *	Function that will attempt to delete the given file.
 *	If successfull true (1) is returned, otherwise false (0).
 */
UInt32
Delete_File( const char *filename )
{
        #ifdef GUCEF_MSWIN_BUILD
        return DeleteFile( filename );
        #else
        #ifdef GUCEF_LINUX_BUILD
        return remove( filename );
        #endif /* WIN32_BUILD ? */
        #endif /* GUCEF_LINUX_BUILD ? */
}

/*-------------------------------------------------------------------------*/

/**
 *	Function that will attempt to copy the given file.
 *	src is the source file.
 *	dst is the destination file.
 *	If successfull true (1) is returned, otherwise false (0).
 */
UInt32
Copy_File( const char *dst, const char *src )
{
        #ifdef GUCEF_MSWIN_BUILD
        return !CopyFile( dst, src, 0 );
        #else
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
        return 1;
        #endif /* WIN32_BUILD ? */
}

/*-------------------------------------------------------------------------*/

/**
 *	moves a file from one location to the other
 *	If successfull true (1) is returned, otherwise false (0).
 */
UInt32
Move_File( const char *dst, const char *src )
{
        #ifdef GUCEF_MSWIN_BUILD
        return MoveFile( src, dst );
        #else
        if ( Copy_File( dst, src ) )
        {
        	return Delete_File( src );

        }
        return 0;
        #endif /* WIN32_BUILD ? */
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
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
#endif /* WIN32_BUILD ? */

/*-------------------------------------------------------------------------*/

/**
 *      Function that will attempt to execute the given program.
 *      If successfull true (1) is returned, otherwise false (0).
 */
UInt32
Execute_Program( const char *filename ,
                 const char *cmdline  )
{
        if ( filename )
        {
                #ifdef GUCEF_MSWIN_BUILD
                HANDLE phdl = ExecuteProgramEx( filename, cmdline );
                if ( phdl )
                {
                        CloseHandle( phdl );
                        return 1;
                }
                return 0;
                #else
                #ifdef GUCEF_LINUX_BUILD
                #endif /* WIN32_BUILD ? */
                #endif /* GUCEF_LINUX_BUILD ? */
        }
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
Filesize( const char *filename )
{
    if ( filename )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
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
        #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
        struct stat buf;
        UInt32 filesize;
        int result;
        result = _stat( filename, &buf );
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

/**
 *      Returns a boolean value indicating wheter or not the given file
 *      exists. 1 is true and 0  is false.
 */
UInt32
File_Exists( const char *filename )
{
        if ( filename )
        {
                #ifdef GUCEF_MSWIN_BUILD

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


                #else
                #ifdef GUCEF_LINUX_BUILD
                struct stat buf;
                if ( _stat( filename, &buf ) == -1 )
                return errno == ENOENT;


                #else
                FILE *fptr = fopen( filename, "rb" );
                fclose( fptr );
                return fptr > 0;
                #endif /* WIN32_BUILD ? */
                #endif /* GUCEF_LINUX_BUILD ? */
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
    #ifdef GUCEF_MSWIN_BUILD
    return GetFileAttributes( path ) != INVALID_FILE_ATTRIBUTES;
    #else
    #ifdef GUCEF_LINUX_BUILD
    // @TODO
    #endif
    #endif
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

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
    #ifdef GUCEF_MSWIN_BUILD

    WIN32_FILE_ATTRIBUTE_DATA data;
    if ( 0 != GetFileAttributesEx( path, GetFileExInfoStandard, &data ) )
    {
        return FileTimeToUnixTime( &data.ftLastWriteTime );
    }
    return -1;

    #else

    struct stat buf;

    /* Get File Statistics for stat.c. */
    if( _stat( path, &buf ) == 0 )
    {
        /* get the date/time last modified */
        return buf.st_mtime;
    }
    return -1;

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
