/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_DVSTRUTILS_H
#define GUCEF_CORE_DVSTRUTILS_H

/*
 *      PORTABILITY :
 *       - Win32
 *       - Linux 
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
#include "ETypes.h"               /* often used types */
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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*--------------------------------------------------------------------------*/

/**
 *	Since strstr() is meant for null terminated strings it's not always
 *	what you want. This function compairs the data bytes and does not care
 *	about the actual content.
 */
GUCEFCORE_EXPORT_C Int32
Pos( const char *substr   ,
     UInt32 slength       ,
     const char *wholestr ,
     UInt32 wlength       );

/*--------------------------------------------------------------------------*/

/**
 *      return the file extension length
 */
GUCEFCORE_EXPORT_C UInt32
File_Ext_Length( const char *filename );

/*--------------------------------------------------------------------------*/

/**
 *      Extract the file extension.
 *      It creates a new const char* with the appropriate offset.
 */
GUCEFCORE_EXPORT_C const char*
Extract_File_Ext( const char *filename );

/*--------------------------------------------------------------------------*/

/**
 *      Extract the filename, removing path
 */
GUCEFCORE_EXPORT_C const char*
Extract_Filename( const char *filepath );

/*--------------------------------------------------------------------------*/

/**
 *	removes last subdir from path. If the path includes a filename then the
 *	filename will be removed aswell. Removal is done by changing chars into
 * 	'\0' and does not effect the actual string buffer size. The return value
 *	is the last non-zero char in the string. works for both win32 and Linux
 *	offcourse.
 */
GUCEFCORE_EXPORT_C UInt32
Strip_Last_Subdir( char *path );

/*--------------------------------------------------------------------------*/

/**
 *	set index such that we strip out any directories that this dir
 *	is a subdir of. The return value is the index.
 *	This function basicly returns the index of the section that would get
 *	stripped by Strip_Last_Subdir()
 */
GUCEFCORE_EXPORT_C UInt32
Last_Subdir( const char *path );

/*--------------------------------------------------------------------------*/

/**
 *      Removes the filename (if present) from the given file path and
 *      copy's the result into dest. If there is a seperator char prefexed to
 *      the filename then it will be removed aswell. The length of the path
 *      copyed into dest is returned.
 */
GUCEFCORE_EXPORT_C UInt32
_Strip_Filename( char *dest, const char *src_filepath );

/*--------------------------------------------------------------------------*/

/**
 *      Removes the filename (if present) from the given file path.
 *      Removal is done by means of setting char to '\0'. 
 *      If there is a seperator char prefexed to the filename then it 
 *      will be removed aswell. The new length of the path
 *      is returned.
 */
GUCEFCORE_EXPORT_C UInt32
Strip_Filename( char* filepath );

/*--------------------------------------------------------------------------*/

/**
 *      Converts any dir seperators that may be present in the given path string
 *      to a seperator which is valid for the current O/S. 
 */
GUCEFCORE_EXPORT_C void
Convert_Dir_Seperators( char *path );

/*--------------------------------------------------------------------------*/

/**
 *      Appends a new sub directory or filename to the path given.
 *      It is assumed path is large enough. Usefull if you don't want to type
 *      ifdef all the time because win32 and linux use different seperation chars
 *      Checks if there is already a seperator present as last char so you
 *      don't have to do this yourself. Dir seperation chars will be forced to
 *      the correct char for the current O/S.
 */
GUCEFCORE_EXPORT_C void 
Append_To_Path( char *path           ,
                const char *addition );

/*--------------------------------------------------------------------------*/

/**
 *      Remove all occurences of the specified char out of the buffer
 *      and returns the new string
 *      This allocates memory that should be deallocated with free !!!.
 */
GUCEFCORE_EXPORT_C char*
Remove_Char( char rchar         ,
             const char *buffer );

/*--------------------------------------------------------------------------*/

/**
 *      replace all occurences of the specified char with the new char
 *      the result is placed in a new buffer that's returned
 *	This allocates memory that should be deallocated with free !!!.
 */
GUCEFCORE_EXPORT_C char*
_Replace_Char( char oldc          ,
               char newc          ,
               const char *buffer );

/*--------------------------------------------------------------------------*/

/**
 *      replace all occurences of the specified char with the new char
 */
GUCEFCORE_EXPORT_C void
Replace_Char( char oldc    ,
              char newc    ,
              char *buffer );

/*--------------------------------------------------------------------------*/

/**
 *      if the range submitted is valid this function will copy
 *      that section out of the submitted string and return it
 *	This allocates memory that should be deallocated with free !!!. 
 */
GUCEFCORE_EXPORT_C char*
Get_Substr( UInt32 start         ,
            UInt32 end           ,
            const char *wholestr );

/*--------------------------------------------------------------------------*/

/**
 *      Find the offset of the given char, if not found returns -1
 *	starts looking at the front of the buffer
 */
GUCEFCORE_EXPORT_C Int32
Find_Char( char a             ,
           const char *buffer ,
           UInt32 size        );

/*--------------------------------------------------------------------------*/

/**
 *      Find the offset of the given char, if not found returns -1
 *      Additional option here is the starting offset and the front int is a
 *	boolean indicating wheter we start looking from the front of the string or
 *	from the end.
 */
GUCEFCORE_EXPORT_C Int32
_Find_Char( UInt32 offset      ,
            UInt32 front       ,
            char a             ,
            const char *buffer ,
            UInt32 size        );

/*--------------------------------------------------------------------------*/

/**
 *      Remove trailing spaces and end of line chars
 *	This allocates memory that should be deallocated with free !!!.
 */
GUCEFCORE_EXPORT_C char*
Trim( const char *str );

/*--------------------------------------------------------------------------*/

/**
 *      Remove trailing spaces and end of line chars,
 *      Same as Trim but replaces the old string
 *	This allocates memory that should be deallocated with free !!!. 
 */
GUCEFCORE_EXPORT_C void
_Trim( char **str );

/*--------------------------------------------------------------------------*/

/**
 *      Remove leading spaces
 *	This allocates memory that should be deallocated with free !!!.
 */
GUCEFCORE_EXPORT_C char*
LTrim( char *str );

/*--------------------------------------------------------------------------*/

/**
 *      Remove leading spaces,
 *      Same as FTrim but replaces the old string
 *	This allocates memory that should be deallocated with free !!!. 
 */
GUCEFCORE_EXPORT_C void
_LTrim( char **str );

/*--------------------------------------------------------------------------*/

/**
 *      String to boolint conversion
 *      "true" or "false" OR "on" or "off" give 1 and 0 respectivly
 */
GUCEFCORE_EXPORT_C UInt32
String_To_Boolint( const char *teststr );

/*--------------------------------------------------------------------------*/

/**
 *      boolint to string conversion
 *      returns "true" or "false"
 */
GUCEFCORE_EXPORT_C const char*
Boolint_To_String( Int32 b );

/*--------------------------------------------------------------------------*/

/**
 *      Makes the given string uppercase
 */
GUCEFCORE_EXPORT_C void
Uppercase( char *str );

/*--------------------------------------------------------------------------*/

/**
 *      Makes the given string lowercase
 */
GUCEFCORE_EXPORT_C void
Lowercase( char *str );

/*--------------------------------------------------------------------------*/

/**
 *      Converts an null-terminated string to a int
 */
GUCEFCORE_EXPORT_C Int32
Str_To_Int( const char *digits );

/*--------------------------------------------------------------------------*/

/**
 *      Converts an null-terminated string to a float
 */
GUCEFCORE_EXPORT_C Float32
Str_To_Float( const char *digits );

/*--------------------------------------------------------------------------*/

/**
 *      Return the number of times this char occures
 *	buffer is array of chars to check
 *	buffer_size is the size of the char array
 *	t is the char we are counting.
 */
GUCEFCORE_EXPORT_C UInt32
Count_Char( const char *buffer  ,
            UInt32 buffer_size  ,
            char t              );

/*--------------------------------------------------------------------------*/

/**
 *      Change all tabs into space chars
 */
GUCEFCORE_EXPORT_C char*
Tab_To_Space( char *buffer    ,
              UInt32 numspace );

/*--------------------------------------------------------------------------*/

/**
 *      Reverse the string
 */
GUCEFCORE_EXPORT_C void
Reverse( char *buffer );

/*--------------------------------------------------------------------------*/

/**
 *      Function that simply checks if the given string is an IP address in
 *      string form or something else, a host name for example.
 *      returns 1 if it's an IP address otherwise 0.
 */
GUCEFCORE_EXPORT_C UInt32
Check_If_IP( const char *buffer );

/*--------------------------------------------------------------------------*/

/**
 *	Copyes size char's from src to destination.
 *	This does not care about a null terminator !!!.
 */
GUCEFCORE_EXPORT_C void
Buffer_Copy( char *dest      ,
             const char *src ,
             UInt32 size     );

/*--------------------------------------------------------------------------*/

/**
 *	Attempt to find and return the index of a null terminator in the given
 *	string. The maximum length of the string is provided. If no null
 *	terminator is found then -1 is returned.
 */
GUCEFCORE_EXPORT_C Int32
Find_Null( UInt32 start_at_front  ,
           const char *src        ,
           UInt32 size            );

/*--------------------------------------------------------------------------*/

/**
 *      Converts the given pointer value into a 32 bit integer
 *      Data may be lost if the pointer is 64 bit
 */
GUCEFCORE_EXPORT_C Int32
PtrToInt32( const void* ptr );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   };
#endif /* __cplusplus */ 

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

#endif /* GUCEF_CORE_DVSTRUTILS_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-05-2005 :
        - Added PtrToInt32()
- 07-05-2005 :
        - Fixed two bugs in Check_If_IP() that could cause the test to return
          the wrong result.
- 25-09-2004 :
        - String_To_Boolint() is no longer case-sensitive.
- 16-07-2004 :
        - Boolint_To_String() now returns a const char* and no longer allocates
          any memory.
- 12-06-2004 :
        - Added support in Find_NULL() to select which side to start seaching.
        - Added support in Find_Char() for non-null-terminated strings.
        - Added support in _Find_Char() for non-null-terminated strings.        
- 26-03-2004 :
        - Added Convert_Dir_Seperators() which makes sure the given path string
          has valid dir seperation chars for the current O/S.
        - Modified Append_To_Path() so that it also takes into account any dir
          seperation char that the addition string might have prefixed.
          I Also added a call to Convert_Dir_Seperators() so that we always
          have a valid path.
- 12-11-2003 :
        - Fixed an initialization bug in Count_Char()
- 22-10-2003 :
        - Added Find_Null() which i need to prevent malicious users from
          crashing my server by sending a string without a '\0' :o)
- 18-08-2003 :
        - Added some stuff to comments.
        - Changed EXPORT into GUCEFCORE_EXPORT_C
- 13-08-2003 :
        - Added this section
        - replaced all simple types ( int ect. ) with my own.
        - Added Strip_Last_Subdir()
        - Added Last_Subdir()
        - Modified _Find_Char so you can specify wheter to start search from the
          front or the back of the string.
        - Added Append_To_Path()

---------------------------------------------------------------------------*/


