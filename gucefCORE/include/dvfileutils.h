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

#ifndef GUCEF_CORE_DVFILEUTILS_H
#define GUCEF_CORE_DVFILEUTILS_H

/*-------------------------------------------------------------------------*/

/*
 *      The utilities are meant to be cross-platform replacements for
 *      O/S dependant file utilities. Compile time build switches are used
 *      to direct these functions to the O/S native versions of the function.  
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"     /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

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
 *	Forward declarations of types used
 */
struct SDI_Data; /* structure used for dir iteration data */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *	Prevent C++ name mangling
 */
#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*/

/**
 *  Function that checks if the given path points to a valid item
 *  the item can be either a directory or a file
 */
GUCEFCORE_EXPORT_C UInt32
Is_Path_Valid( const char* path );

/*-------------------------------------------------------------------------*/

/**
 *      Function that attempts to create a directory using the system default
 *      security attributes. Returns true (1) if successful and false (0) in
 *      case of an error. Unlike the normal create dir functions for windows
 *      this one will create all dir's in the string given
 *      (Like the Linux functions).
 */
GUCEFCORE_EXPORT_C UInt32 
Create_Directory( const char *new_dir );

/*-------------------------------------------------------------------------*/

/**
 *      Function that attempts to remove the specified directory.
 *      The del_files parameter allows you to specify whether you wish to delete
 *      all the files and dirs in the directory before deleting the directory
 *      itself. If false and files or dirs are found the directory removal will
 *      fail.
 */
GUCEFCORE_EXPORT_C UInt32 
Remove_Directory( const char *dir  ,
                  UInt32 del_files );

/*-------------------------------------------------------------------------*/

/**
 *      Function that will attempt to delete the given file.
 *      If succesful true (1) is returned, otherwise false (0).
 */
GUCEFCORE_EXPORT_C UInt32 
Delete_File( const char *filename );

/*-------------------------------------------------------------------------*/

/**
 *      Function that will attempt to copy the given file.
 *      src is the source file.
 *      dst is the destination file.
 *      This function does not overwrite any existing file.
 *      If succesful true (1) is returned, otherwise false (0).
 */
GUCEFCORE_EXPORT_C UInt32 
Copy_File( const char *dst, const char *src );

/*-------------------------------------------------------------------------*/

/**
 *      moves a file from one location to the other
 *      If successful true (1) is returned, otherwise false (0).
 */
GUCEFCORE_EXPORT_C UInt32 
Move_File( const char *dst, const char *src );

/*-------------------------------------------------------------------------*/

/**
 *      Function that will attempt to execute the given program.
 *      cmdline are the command line arguments to be passed to the program.
 *      If successful true (1) is returned, otherwise false (0).
 *      This function returns regardless of the state of the executed program.
 */
GUCEFCORE_EXPORT_C UInt32 
Execute_Program( const char *filename, const char *cmdline );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry iteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to iterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT thread-safe.
 *
 *      This function allocates data storage for the dir iteration process.
 *      In case of error NULL is returned.
 */
GUCEFCORE_EXPORT_C struct SDI_Data* 
DI_First_Dir_Entry( const char *path );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry iteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to iterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Function that selects the next directory entry. If there are no more
 *      directory entry's available i.e. we iterated over all entry's then
 *      0 is returned in which case you should call DI_Cleanup(),
 *      otherwise 1 is returned.
 */
GUCEFCORE_EXPORT_C UInt32 
DI_Next_Dir_Entry( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry iteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to iterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      De-allocates data storage used for dir iteration which was created by
 *      a call to DI_First_Dir_Entry().
 */
GUCEFCORE_EXPORT_C void 
DI_Cleanup( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry iteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to iterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Returns the dir entry name of the current directory entry. This may be
 *      a directory name or a filename. Use DI_Is_It_A_File() to determine which
 */
GUCEFCORE_EXPORT_C const char* 
DI_Name( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry iteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to iterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Returns the size of the current entry which in the case of a file is the
 *      file size.
 */
GUCEFCORE_EXPORT_C UInt32 
DI_Size( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry iteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to iterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Returns the timestamp of the file. This is when the file was last
 *      modified.
 */
GUCEFCORE_EXPORT_C UInt32 
DI_Timestamp( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry iteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to iterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Returns boolean int indicating whether the current entry is a directory
 *      or a file. If it's not a file then it's a directory.
 */
GUCEFCORE_EXPORT_C UInt32 
DI_Is_It_A_File( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that returns the current working directory
 *      The current working dir is written into dest_buffer to max buf_length
 *      chars. You can use Max_Dir_Length() for buf_length in which case you
 *      should never have problems with a dest_buffer that's to small.
 */
GUCEFCORE_EXPORT_C char* 
Get_Current_Dir( char* dest_buffer ,
                 UInt32 buf_length );

/*-------------------------------------------------------------------------*/

/**
 *      Function that returns maximum possible length of a directory on the
 *      current O/S. Including the null terminating char. You can also use the
 *      MAX_DIR_LENGTH define for this.
 */
GUCEFCORE_EXPORT_C UInt32 
Max_Dir_Length( void );

/*-------------------------------------------------------------------------*/

/**
 *      Function that returns maximum possible length of a filename on the
 *      current O/S. Including the null terminating char. You can also use the
 *      MAX_FILENAME_LENGTH define for this. 
 */
GUCEFCORE_EXPORT_C UInt32 
Max_Filename_Length( void );

/*-------------------------------------------------------------------------*/

/**
 *      Function that lets you obtain the path to the module that houses this
 *      function. So for example if you are calling this function from your  
 *      application while the function is part of a dll you use then the path 
 *      to the dll is returned. If the dll is the the same dir as the executable
 *      then this function offers a good portable method to get the path to
 *      the application. returns 1 in case of error, otherwise returns 0.
 */
GUCEFCORE_EXPORT_C UInt32 
Module_Path( char *dest, UInt32 dest_size );

/*-------------------------------------------------------------------------*/

/**
 *      Function for getting the size of a file using the fastest method
 *      possible for the target O/S.      
 */
GUCEFCORE_EXPORT_C UInt32 
Filesize( const char *filename );

/*-------------------------------------------------------------------------*/

/**
 *      Returns a boolean value indicating whether or not the given file
 *      exists. 1 is true and 0  is false. the fastest method possible for the
 *      target O/S will be used.
 */
GUCEFCORE_EXPORT_C UInt32 
File_Exists( const char *filename );

/*-------------------------------------------------------------------------*/

/**
 *      Writes a relative path into dest build from pathstr.
 *      The following prefix tags can be used:
 *        $CURWORKDIR$ : this will be replaced with the current working dir.
 *        $MODULEDIR$ : this will be replaced with the module dir.
 *      The following tag can be placed after one of the above mentioned tags
 *        $UPDIR$ : go up one dir 
 *      Dir separator chars will be forced to that of the the target O/S.
 *      It is assumed dest is large enough to store the data.
 *      The function returns the total number of bytes written to dest.
 */
GUCEFCORE_EXPORT_C UInt32 
Relative_Path( const char *pathstr ,
               char *dest          ,
               UInt32 buffersize   );

/*-------------------------------------------------------------------------*/

/*
 *	End of C++ name mangling protection
 */
#ifdef __cplusplus
};
#endif

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

#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-04-2005 :
        - Fixed a bug in File_Exists(): the return value was inverted.
- 23-04-2005 :
        - Fixed a bug in Relative_Path(): the $UPDIR$ parsing could cause
          read actions in invalid memory. 
- 16-07-2004 :
        - Added Filesize()
        - Added File_Exists()
- 22-02-2004 :
        - Fixed a bug in Create_Directory() that caused bogus directory's to
          be created under WIN32.
        - Added Remove_Directory()  
- 29-12-2003 :
        - Added Delete_File()
        - Added Copy_File()
        - Added Move_File()
        - Added ExecuteProgram()
- 18-08-2003 :
        - Changed EXPORT into GUCEFCORE_EXPORT_C
- 13-08-2003 :
        - Added Create_Directory()
        - Added a set of functions for directory iteration.
              DI_First_Dir_Entry()
              DI_Next_Dir_Entry()
              DI_Cleanup()
              DI_Name()
              DI_Is_It_A_File()
              DI_Size()
              DI_Timestamp()
          You should use these functions instead of _findfirst or opendir() or
          whatever because this way your code can stay platform independent
          while the real API calls are made by these functions depending on the
          build type.
          These functions allow support for:
             - Windows 98
             - Windows Millenium
             - Windows NT ( all versions )
             - Windows 2000
             - Windows XP
             - Newer windows versions ?
             - Linux/Unix variants that support POSIX
- 12-07-2003 :
        - Started this set of utilities as a way to provide me with a cross
          platform codebase for file/dir handling. SDL unfortionaty does not
          cover this so i needed to make portable code for this myself.
          Target platforms are WIN32 and Linux.

-----------------------------------------------------------------------------*/


