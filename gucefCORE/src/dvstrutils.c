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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>      /* string handling */
#include <malloc.h>      /* memory allocation */
#include "gucefCORE_ETypes.h"      /* simple types used */
#include "dvstrutils.h"  /* ths file's header */
#include <stdio.h>       /* standard I/O handling */

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
   ASCII :
   number 49-57
   uppercase 65-90
   lowercase 97-122
*/

/*--------------------------------------------------------------------------*/

void
Buffer_Copy( char *dest, const char *src, UInt32 size )
{
        /*
         *	Copyes size char's from src to destination.
         */
        UInt32 i;
        for ( i=0; i<size; i++ )
        {
        	dest[ i ] = src[ i ];
        }
}

/*--------------------------------------------------------------------------*/

const char*
Extract_Filename( const char *filepath )
{
        /*
         *      Extract the filename, if there is any path prefix it will be
         *      removed
         */
        UInt32 i, length = (UInt32)strlen( filepath );
        for ( i = length; i >= 0; --i )
        {
                if ( ( ( filepath[ i ] == '/' ) || (  filepath[ i ] == '\\' ) ) && ( i != length ) )
                {
                        /*
                         *      Extract filename
                         */
                        return filepath+i+1;
                }
        }
        return filepath;
}

/*--------------------------------------------------------------------------*/

void
Reverse( char *buffer )
{
        /*
         *      Reverse the given string
         */
        char aux;
        int i, length = (UInt32) strlen( buffer );

        for ( i=0; i<length/2; i++ )
        {
                aux = buffer[ i ]                                       ;
                buffer[ i ] = buffer[ length-1 - i ]                    ;
                buffer[ length-1 - i ] = aux                            ;
        }
}

/*--------------------------------------------------------------------------*/

void
Uppercase( char *str )
{
        /*
         *      Makes the given string uppercase
         */
        int length = (UInt32) strlen( str );
        int i;
        for ( i=0; i<=length; i++ )
        {
                /*
                 *      Test for lowercase letter
                 */
                if ( ( str[i] > 96 ) && ( str[i] < 123 ) )
                {
                        /*
                         *      Convert char
                         */
                        str[i] -= 32                                    ;
                }
        }
}

/*--------------------------------------------------------------------------*/

void
Lowercase( char *str )
{
        /*
         *      Makes the given string lowercase
         */
        UInt32 length = (UInt32)strlen( str )                                      ;
        UInt32 i                                                           ;
        for ( i=0; i<=length; i++ )
        {
                /*
                 *      Test for uppercase letter
                 */
                if ( ( str[i] > 64 ) && ( str[i] < 91 ) )
                {
                        /*
                         *      Convert char
                         */
                        str[i] += 32                                    ;
                }
        }
}

/*--------------------------------------------------------------------------*/

UInt32
String_To_Boolint( const char *teststr )
{
        /*
         *      String to bool conversion
         */
        if ( teststr )
        {
                char tests[ 7 ];
                UInt32 max = (UInt32) strlen( teststr )+1;
                if ( 7 < max ) max = 7;
                memcpy( tests, teststr, max );
                Lowercase( tests );
                
                return( ( strcmp( tests, "enable"  ) == 0 ) ||
                        ( strcmp( tests, "true"  ) == 0 )   ||
                        ( strcmp( tests, "yes"  ) == 0 )    ||
                        ( strcmp( tests, "on"  ) == 0 )     ||
                        ( strcmp( tests, "1"  ) == 0 )      );
        }
        else
        {
                return 0;
        }
}

/*--------------------------------------------------------------------------*/

const char*
Boolint_To_String( Int32 b )
{
        if ( b )
        {
                return "true";
        }
        return "false";
}

/*--------------------------------------------------------------------------*/

char*
Get_Substr( UInt32 start, UInt32 end, const char *wholestr )
{
        /*
         *      if the range submitted is valid this function will copy
         *      that section out of the submitted string and return it
         */
        char *tmp                                                 ;
        char achar[2]                                                   ;
        UInt32 i                                                           ;
        if ( ( start >= 0 ) && ( start < end ) && ( ( unsigned int ) end <= strlen( wholestr ) ) )
        {
                /*
                 *      range is valid, copy section
                 */
                tmp = ( char* ) malloc( ( end - start +2)*sizeof(char) );
                tmp[ 0 ] = 0                                            ;
                achar[1] = 0                                            ;
                for ( i=start; i<=end; i++ )
                {
                        achar[ 0 ] = wholestr[ i ]                      ;
                        strcat( tmp, achar )                            ;
                }

                /*
                 *      return selected section
                 */
                return( tmp )                                           ;
        }
        else
        {
                /*
                 *      Invalid range
                 */
                return( NULL )                                          ;
        }
}

/*--------------------------------------------------------------------------*/

char*
LTrim( char *str )
{
        /*
         *      Remove leading spaces,
         *      returns a pointer to the new trimmed string
         */

        int length = (int)strlen( str )-1                                    ;
        int end = 0                                                     ;
        int i = 0                                                       ;
        char *tmp                                                 ;

        /*
         *      Loop to find new string end
         */
        while ( i<=length )
        {
                /*
                 *      Test for valid remaining char
                 */
                if ( str[i] == 32 )
                {
                        /*
                         *      Move end marker
                         */
                        end++                                           ;
                }
                else
                {
                        /*
                         *      exit loop
                         */
                        i=length                                        ;
                }
                i++                                                     ;
        }

        /*
         *      Copy string segment and return it
         */
        tmp = Get_Substr( end, length, str )                            ;
        return( tmp )                                                   ;
}

/*--------------------------------------------------------------------------*/

void
_LTrim( char **str )
{
        /*
         *      Remove leading spaces,
         *      Same as LTrim but replaces the old string
         */
        char *tmp = LTrim( *str )                                       ;
        free( *str )                                                    ;
        *str = tmp                                                      ;
}

/*--------------------------------------------------------------------------*/

char*
Trim( const char *str )
{
        /*
         *      Remove trailing spaces and end of line chars,
         *      returns a pointer to the new trimmed string
         */

        int i = (int)strlen( str )-1                                         ;
        int end = i                                                     ;
        char *tmp                                                 ;

        /*
         *      Loop to find new string end
         */
        while ( i>=0 )
        {
                /*
                 *      Test for valid remaining char
                 */
                if ( ( str[i] == 10 ) || ( str[i] == 13 ) || ( str[i] == 32 ) )
                {
                        /*
                         *      Move end marker
                         */
                        end--                                           ;
                }
                else
                {
                        /*
                         *      exit loop
                         */
                        i=-1;
                }
                i--                                                     ;
        }

        /*
         *      Copy string segment and return it
         */
        tmp = Get_Substr( 0, end, str )                                 ;
        return( tmp )                                                   ;
}
/*--------------------------------------------------------------------------*/

void
_Trim( char **str )
{
        /*
         *      Remove trailing spaces and end of line chars,
         *      Same as Trim but replaces the old string
         */
        char *tmp = Trim( *str )                                            ;
        free( *str )                                                        ;
        *str = tmp                                                          ;
}

/*--------------------------------------------------------------------------*/

char*
Remove_Char( char rchar, const char *buffer )
{
        /*
         *      Remove all occurences of the specified char out of the buffer
         */
        int length = (int)strlen( buffer )                                   ;
        int i                                                           ;
        int last                                                        ;
        int occ = 0                                                     ;
        char *nstr                                               ;

        /*
         *      Loop trough string counting occurences of specified char
         */
        for ( i=0; i<=length; i++ )
        {
                /*
                 *      test for char, if found increment counter
                 */
                if ( buffer[ i ] == rchar )
                {
                        occ++;
                }
        }

        /*
         *      Allocate memory for the new buffer
         */
        nstr = ( char* ) calloc( length+1 -occ, 1 );

        /*
         *      Now we copy the string chars without the
         *      specified ones into the new buffer
         */
        last = 0;
        for ( i=0; i<=length; i++ )
        {
                /*
                 *      test for char, if not found add char
                 */
                if ( buffer[ i ] != rchar )
                {
                        /*
                         *      Append char to new buffer
                         */
                        nstr[last] = buffer[ i ]                        ;
                        last++                                          ;
                }
        }
        return nstr;
}

/*---------------------------------------------------------------------------*/

void
Replace_Char( char oldc, char newc, char *buffer )
{
        /*
         *      replace all occurences of the specified char with the new char
         */

        int length = (int)strlen( buffer )                                   ;
        int i                                                           ;

        /*
         *      loop trough string
         */
        for ( i=0; i<length; i++ )
        {
                /*
                 *      test for match
                 */
                if ( buffer[i] == oldc )
                {
                        /*
                         *      Replace char
                         */
                        buffer[i] = newc                                ;
                }
        }
}

/*---------------------------------------------------------------------------*/

char*
_Replace_Char( char oldc, char newc, const char *buffer )
{
        /*
         *      First we allocate a new buffer
         *      and copy string into it
         */
        UInt32 len = (UInt32)strlen( buffer )+1;
        char *newbuf = ( char* ) malloc( len );
        strcpy( newbuf, buffer );

        /*
         *      Now we do the replace
         */
        Replace_Char( oldc, newc, newbuf );

        return newbuf;
}

/*---------------------------------------------------------------------------*/

/**
 *	Since strstr() is meant for null terminated strings it's not always
 *	what you want. This function compairs the data bytes and does not care
 *	about the actual content.
 */
Int32
Pos( const char *substr, UInt32 slength, const char *wholestr, UInt32 wlength )
{
	UInt32 i;
        for ( i=0; i<wlength-slength; i++ )
        {
        	if ( memcmp( substr, wholestr+i, slength ) == 0 )
                {
                	return i;
                }
        }
        return -1;

}

/*---------------------------------------------------------------------------*/

Int32
_Find_Char( UInt32 offset, UInt32 front, char a, const char *buffer, UInt32 size )
{
        /*
         *      Find the offset of the given char, if not found return -1
         *      This fuction is meant for a single char, if you are searching
         *      for a multi-character section then use Pos
         */
        Int32 i;

        if ( offset > size ) { return -1; }

        if ( front )
        {
                /*
                 *	Search for char starting at front of array
                 */
                for ( i=offset; i<(Int32)size; i++ )
                {
                        if ( buffer[ i ] == a )
                        {
                                /*
                                 *      Char found
                                 */
                                return i;
                        }
                }
                /*
                 *      If we arrive here no match was found
                 */
                return( -1 );
        }

        /*
         *	Search for char starting at end of array
         */
        for ( i=size-offset-1; i>=0; i-- )
        {
                if ( buffer[ i ] == a )
                {
                        /*
                         *      Char found
                         */
                        return i;
                }
        }

        /*
         *      If we arrive here no match was found
         */
        return -1;
}
/*---------------------------------------------------------------------------*/

Int32
Find_Char( char a, const char *buffer, UInt32 size )
{
        /*
         *      Find the offset of the given char, if not found return -1
         *      This fuction is meant for a single char, if you are searching
         *      for a multi-character section then use Pos
         */
        return( _Find_Char( 0, 1, a, buffer, size ) );
}

/*---------------------------------------------------------------------------*/

UInt32
File_Ext_Length( const char *filename )
{
        /*
         *      return the file extension length
         */
        int length = (int)strlen( filename );
        int i; 
        for ( i=length; i>=0; i-- )
        {
                if ( filename[i] == '.' ) // '.' = ASCII 46
                {
                        /*
                         *      Return extension
                         */
                        return length-i;
                }
        }
        return 0;
}

/*---------------------------------------------------------------------------*/

const char*
Extract_File_Ext( const char *filename )
{
        /*
         *      Extract the file extension
         */
        UInt32 len = (UInt32)strlen( filename );
        const char* i;
        for ( i = filename+len; i>=0; i-- )
        {
                if ( ( *i == '.' ) && ( (UInt32)(i-filename) != len ) )
                {
                        /*
                         *      Return extension
                         */
                        return i+1;
                }
        }
        return NULL;
}

/*---------------------------------------------------------------------------*/

Int32
Str_To_Int( const char *digits )
{
        /*
         *      Converts an null-terminated string to a int
         */
        int i, num;
        if ( digits == NULL ) return 0; 

        /*
         *      Convert each digit char and add into result.
         */
        i = 0;
        num = 0;
        while ( digits[ i ] >= '0' && digits[ i ] <= '9' )
        {
                num = (num * 10) + ( digits[ i ] - '0')                 ;
                i++                                                     ;
        }

        /*
         *      Check that there were no non-digits at end.
         */
        if (*digits != 0)
        {
                /*
                 *      Invalid input
                 */
              //  return 0               bogus ???                                 ;
        }

        /*
         *      Return converted int
         */
        return num                                                      ;
}

/*---------------------------------------------------------------------------*/

Float32
Str_To_Float( const char *digits )
{
        /*
         *      Converts an null-terminated string to a float
         */
        UInt32 len = (UInt32)strlen( digits ); 
        char *num;
        float flt;

        /*
         *      Find decimal seperator
         */
        int idx = Find_Char( ',', digits, len );
        if ( idx < 0 )
        {
                idx = Find_Char( '.', digits, len );
        }
        if ( idx < 0 )
        {
                /*
                 *      It is in fact an int not a float
                 */
                return (Float32) Str_To_Int( digits );
        }

        /*
         *      Change the 2 parts into ints and calculate float
         */
        num = Get_Substr( 0, idx, digits );
        flt = (float)Str_To_Int( num );
        free( num );
        num = Get_Substr( idx, len-1, digits );
        flt += ( Str_To_Int( num ) / ( len * 10 ) );
        free( num );
        
        return( flt );
}

/*---------------------------------------------------------------------------*/

UInt32
Last_Subdir( const char *path )
{
        /*
         *	set index such that we strip out any directories that this dir
         *	is a subdir of. The return value is the index.
         *	This function basicly returns the index of the section that would get stripped
         *	by Strip_Last_Subdir()
         */
        Int32 max = (Int32)strlen( path );
        #ifdef GUCEF_MSWIN_BUILD
        Int32 index = _Find_Char( 0, 0, '\\', path, max );
	if ( index < 0 ) return 0;
        if ( index == (max-1) )
        {
        	index = _Find_Char( 1, 0, '\\', path, max );

        }
        if ( index < max ) index++;
        return index;
        #else
        #ifdef GUCEF_LINUX_BUILD
        Int32 index = _Find_Char( 0, 0, '/', path, max );
        if ( index < 0 ) return 0;
        if ( index == max )
        {
        	index = _Find_Char( 1, 0, '/', path, max );
        }
        if ( index < max ) index++;
        return index;
        #else
        return 0;
        #endif /* GUCEF_LINUX_BUILD */
        #endif /* WIN32_BUILD */
}

/*---------------------------------------------------------------------------*/

UInt32
Strip_Last_Subdir( char *path )
{
        /*
         *	removes last subdir from path. If the path includes a filename then the
         *	filename will be removed aswell. Removal is done by changing chars into
         * 	'\0' and does not effect the actual string buffer size. The return value
         *	is the last non-zero char in the string.
         */
        Int32 index, findex, i, max;

        max = (Int32)strlen( path );

        /*
         *	First remove any filename if present
         */
        findex = _Find_Char( 0, 0, '.', path, max );
        if ( findex >= 0 )
        {
        	/*
                 *	Char we seek depends on OS since Linux and Win32 use
                 *      different seperation chars.
                 */
        	#ifdef GUCEF_MSWIN_BUILD
        	findex = _Find_Char( 0, 0, '\\', path, max );
                #else
                #ifdef GUCEF_LINUX_BUILD
                findex = _Find_Char( 0, 0, '/', path, max );
                #else
                return max;
                #endif /* GUCEF_LINUX_BUILD */
                #endif /* WIN32_BUILD */
                for ( i=findex; i<max; i++ )
                {
                	path[ i ] = '\0';
                }
        }
        else findex = max-1;

        /*
         *	Remove last subdir
         *	Char we seek depends on OS since Linux and Win32 use
         *      different seperation chars.
         */
        #ifdef GUCEF_MSWIN_BUILD
        index = _Find_Char( 0, 0, '\\', path, max );
        #else
        #ifdef GUCEF_LINUX_BUILD
        index = _Find_Char( 0, 0, '/', path, max );
        #else
        return max;
        #endif /* GUCEF_LINUX_BUILD */
        #endif /* WIN32_BUILD */
        if ( index < 0 ) return findex;
        for ( i=index; i<findex; i++ )
        {
                path[ i ] = '\0';
        }
        return index-1;
}

/*---------------------------------------------------------------------------*/

char*
Tab_To_Space( char *buffer, UInt32 numspace )
{
        /*
         *      Change all tabs into chars
         */
        UInt32 length = (UInt32)strlen( buffer )                                   ;
        int count = Count_Char( buffer, length, 9 )                     ;        
        char aux[ 2 ]                                                   ;
        UInt32 i,n, last = 0                                               ;
        char *newbuf = ( char* ) calloc( count*numspace -count + length+1, sizeof( char ) );
        aux[ 1 ] = 0                                                    ;
        for ( i=0; i<length; i++ )
        {
                if ( buffer[ i ] != 9 )
                {
                        /*
                         *      No tab thus leave unchanged
                         */
                        aux[ 0 ] = buffer[ i ]                          ;
                        strcat( newbuf, aux )                           ;
                }
                else
                {
                        /*
                         *      Entered desired number of spaces where
                         *      the tab used to be
                         */
                        for ( n=0; n<numspace; n++ )
                        {
                                newbuf[ i+last+n ] = 32                 ;
                        }
                        last += n-1                                     ;
                }
        }
        return( newbuf )                                                ;
}

/*--------------------------------------------------------------------------*/

UInt32
Check_If_IP( const char *buffer )
{
        /*
         *      Function that simply checks if the given string is an IP address in
         *      string form or something else, a host name for example.
         *      returns 1 if it's an IP address.
         */
        UInt32 length = (UInt32) strlen( buffer );
        UInt32 i;
        UInt32 numcount = 0;
        UInt32 dotcount = 0;

        /*
         *      check for minimal IP length
         */
        if ( length < 7 ) return 0;

        /*
         *      Check per buffer value if any IP syntaxis rules have been broken
         */
        i=0;
        while ( i<length )
        {
                /*
                 *      Check for valid IP character
                 */
                if ( ( ( buffer[ i ] < 48 ) || ( buffer[ i ] > 57 ) ) && ( buffer[ i ] != '.' ) )
                {
                        return 0;
                }

                /*
                 *      Increment counters
                 */
                if ( buffer[ i ] == '.' )
                {
                        dotcount++;
                        numcount=0;
                }
                else
                {
                        numcount++;
                        if ( numcount > 3 ) return 0;
                }
                if ( dotcount > 3 ) return 0;
                i++;
        }
        return 1;
}

/*--------------------------------------------------------------------------*/

UInt32
Count_Char( const char *buffer, UInt32 buffer_size, char t )
{
        /*
         *      Return the number of times this char occures
         *      buffer is array of chars to check
         *      buffer_size is the size of the char array
         *	t is the char we are counting.
         */
        UInt32 count=0, i;
        for ( i=0; i<buffer_size; i++ )
        {
        	if ( buffer[ i ] == t ) count++;
        }
        return count;
}

/*--------------------------------------------------------------------------*/

/**
 *      Converts any dir seperators that may be present in the given path string
 *      to a seperator which is valid for the current O/S. 
 */
void
Convert_Dir_Seperators( char *path )
{
        UInt32 i, len = (UInt32)strlen( path );
        for ( i=0; i<len; ++i )
        {
                #ifdef GUCEF_MSWIN_BUILD
                if ( *(path+i) == '/' )
                {
                        *(path+i) = '\\';
                }
                #else
                if ( *(path+i) == '\\' )
                {
                        *(path+i) = '/';
                }
                #endif
        }
}

/*--------------------------------------------------------------------------*/

/**
 *      Appends a new sub directory or filename to the path given.
 *      It is assumed path is large enough. Usefull if you don't want to type
 *      ifdef all the time because win32 and linux use different seperation chars
 *      Checks if there is already a seperator present as last char so you
 *      don't have to do this yourself.
 */
void 
Append_To_Path( char *path           ,
                const char *addition )
{
        /*
         *      Take care of any seperator that may be prefixed to
         *      the addition string.
         */
        UInt32 offset = 0, len = (UInt32)strlen( path );
        if ( ( *addition == '/' ) || ( *addition == '\\' ) )
        {
                offset = 1;
        }

        /*
         *      Now we make sure we have a dir seperator between the
         *      path and the addition strings.
         */
        #ifdef GUCEF_MSWIN_BUILD
        if ( path[ len-1 ] == '\\' )
        {
                strcat( path, addition+offset );
                Convert_Dir_Seperators( path );
                return;
        }
        strcat( path, "\\" );
        strcat( path, addition+offset );
        Convert_Dir_Seperators( path );
        #else
        #ifdef GUCEF_LINUX_BUILD
        if ( path[ len-1 ] == '/' )
        {
                strcat( path, addition+offset );
                Convert_Dir_Seperators( path );
                return;
        }
        strcat( path, "/" );
        strcat( path, addition+offset );
        Convert_Dir_Seperators( path );
        #endif /* GUCEF_LINUX_BUILD */
        #endif /* WIN32_BUILD */
}

/*--------------------------------------------------------------------------*/

/**
 *	Attempt to find and return the index of a null terminator in the given
 *	string. The maximum length of the string is provided. If no null
 *	terminator is found then -1 is returned.
 */
Int32
Find_Null( UInt32 start_at_front ,
           const char *src       ,
           UInt32 size           )
{
        Int32 i;
        if ( start_at_front )
        {
                for ( i=0; i<(Int32)size; i++ )
                {
                        if ( !src[ i ] ) return i;
                }
                return -1;
        }

        if ( !size ) return -1;
        for ( i=size-1; i>=0; --i )
        {
                if ( !src[ i ] ) return i;
        }
        return -1;
}

/*--------------------------------------------------------------------------*/

/**
 *	Removes the filename (if present) from the given file path and
 *	copy's the result into dest. If there is a seperator char prefexed to
 *	the filename then it will be removed aswell. The length of the path
 *	copyed into dest is returned.
 */
UInt32
_Strip_Filename( char *dest, const char *src_filepath )
{
        strcpy( dest, src_filepath ); 
        return Strip_Filename( dest );
}

/*--------------------------------------------------------------------------*/

/**
 *      Removes the filename (if present) from the given file path.
 *      Removal is done by means of setting char to '\0'. 
 *      If there is a seperator char prefexed to the filename then it 
 *      will be removed aswell. The new length of the path
 *      is returned.
 */
UInt32
Strip_Filename( char* filepath )
{
	#ifdef GUCEF_MSWIN_BUILD
        #define SEP_CHAR        '\\'
        #else
        #define SEP_CHAR        '/'
        #endif

        Int32 idx;
        idx = (UInt32)_Find_Char( 0, 0, SEP_CHAR, filepath, (UInt32)strlen( filepath ) );
        if ( idx > 0 )
        {
                filepath[ idx ] = '\0';
                return idx;
        }
        return 0;
}

/*--------------------------------------------------------------------------*/

Int32
PtrToInt32( const void* ptr )
{
        char ptrstr[ 64 ];
        sprintf( ptrstr, "%d", (Int64)ptr );
        return Str_To_Int( ptrstr );
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
