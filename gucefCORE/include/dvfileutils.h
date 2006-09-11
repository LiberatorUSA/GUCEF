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
 *      Function that attempts to create a directory using the system default
 *      security attributes. Returns true (1) if succesfull and false (0) in
 *      case of an error. Unlike the normal create dir functions for windows
 *      this one will create all dir's in the string given
 *      (Like the Linux functions).
 */
EXPORT_C UInt32 
Create_Directory( const char *new_dir );

/*-------------------------------------------------------------------------*/

/**
 *      Function that attempts to remove the specified directory.
 *      The del_files parameter allows you to specify wheter you wish to delete
 *      all the files and dirs in the directory before deleting the directory
 *      itself. If false and files or dirs are found the directory removal will
 *      fail.
 */
EXPORT_C UInt32 
Remove_Directory( const char *dir  ,
                  UInt32 del_files );

/*-------------------------------------------------------------------------*/

/**
 *      Function that will attempt to delete the given file.
 *      If succesfull true (1) is returned, otherwise false (0).
 */
EXPORT_C UInt32 
Delete_File( const char *filename );

/*-------------------------------------------------------------------------*/

/**
 *      Function that will attempt to copy the given file.
 *      src is the source file.
 *      dst is the destination file.
 *      This function does not overwrite any existing file.
 *      If succesfull true (1) is returned, otherwise false (0).
 */
EXPORT_C UInt32 
Copy_File( const char *dst, const char *src );

/*-------------------------------------------------------------------------*/

/**
 *      moves a file from one location to the other
 *      If succesfull true (1) is returned, otherwise false (0).
 */
EXPORT_C UInt32 
Move_File( const char *dst, const char *src );

/*-------------------------------------------------------------------------*/

/**
 *      Function that will attempt to execute the given program.
 *      cmdline are the command line arguments to be passed to the program.
 *      If successfull true (1) is returned, otherwise false (0).
 *      This function returns regardless of the state of the executed program.
 */
EXPORT_C UInt32 
Execute_Program( const char *filename, const char *cmdline );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry iteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to iterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT thread-safe.
 *
 *      This function allocates data storage for the dir itteration process.
 *      In case of error NULL is returned.
 */
EXPORT_C struct SDI_Data* 
DI_First_Dir_Entry( const char *path );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry itteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to itterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Function that selects the next directory entry. If there are no more
 *      directory entry's available ie we itterated over all entry's then
 *      0 is returned in which case you should call DI_Cleanup(),
 *      otherwise 1 is returned.
 */
EXPORT_C UInt32 
DI_Next_Dir_Entry( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry itteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to itterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      De-allocates data storage used for dir itteration which was created by
 *      a call to DI_First_Dir_Entry().
 */
EXPORT_C void 
DI_Cleanup( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry itteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to itterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Returns the dir entry name of the current directory entry. This may be
 *      a directory name or a filename. Use DI_Is_It_A_File() to determine which
 */
EXPORT_C const char* 
DI_Name( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry itteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to itterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Returns the size of the current entry which in the case of a file is the
 *      filesize.
 */
EXPORT_C UInt32 
DI_Size( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry itteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to itterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Returns the timestamp of the file. This is when the file was last
 *      modified.
 */
EXPORT_C UInt32 
DI_Timestamp( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that should be used for directory entry itteration.
 *      All the functions listed here with a DI_ prefix belong together and
 *      combined allow you to itterate trough a directory in a cross-platform
 *      manner. Note that the DI_ functions are NOT threadsafe.
 *
 *      Returns boolean int indicating wheter the current entry is a directory
 *      or a file. If it's not a file then it's a directory.
 */
EXPORT_C UInt32 
DI_Is_It_A_File( struct SDI_Data *data );

/*-------------------------------------------------------------------------*/

/**
 *      Function that returns the current working directory
 *      The current working dir is written into dest_buffer to max buf_length
 *      chars. You can use Max_Dir_Length() for buf_length in which case you
 *      should never have problems with a dest_buffer that's to small.
 */
EXPORT_C char* 
Get_Current_Dir( char* dest_buffer ,
                 UInt32 buf_length );

/*-------------------------------------------------------------------------*/

/**
 *      Function that returns maximum possible length of a directory on the
 *      current O/S. Including the null terminating char. You can also use the
 *      MAX_DIR_LENGTH define for this.
 */
EXPORT_C UInt32 
Max_Dir_Length( void );

/*-------------------------------------------------------------------------*/

/**
 *      Function that returns maximum possible length of a filename on the
 *      current O/S. Including the null terminating char. You can also use the
 *      MAX_FILENAME_LENGTH define for this. 
 */
EXPORT_C UInt32 
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
EXPORT_C UInt32 
Module_Path( char *dest, UInt32 dest_size );

/*-------------------------------------------------------------------------*/

/**
 *      Function for getting the size of a file using the fastest method
 *      possible for the target O/S.      
 */
EXPORT_C UInt32 
Filesize( const char *filename );

/*-------------------------------------------------------------------------*/

/**
 *      Returns a boolean value indicating wheter or not the given file
 *      exists. 1 is true and 0  is false. the fastest method possible for the
 *      target O/S will be used.
 */
EXPORT_C UInt32 
File_Exists( const char *filename );

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
EXPORT_C UInt32 
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
        - Changed EXPORT into EXPORT_C
- 13-08-2003 :
        - Added Create_Directory()
        - Added a set of functions for directory itteration.
              DI_First_Dir_Entry()
              DI_Next_Dir_Entry()
              DI_Cleanup()
              DI_Name()
              DI_Is_It_A_File()
              DI_Size()
              DI_Timestamp()
          You should use these functions instead of _findfirst or opendir() or
          whatever because this way your code can stay platform independant
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


