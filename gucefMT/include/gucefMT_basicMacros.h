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

#ifndef GUCEF_MT_BASICMACROS_H
#define GUCEF_MT_BASICMACROS_H

/*
 *      File that has macros that are commonly used troughout applications.
 *      Any macro here should not be specific to a particular build
 *      configuration but must be generic.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Make sure NULL is defined
 */
#ifndef NULL
#define NULL    0UL
#endif

/*-------------------------------------------------------------------------*/

/*
 *      Macro for getting the corrent SDL key code.
 *      This macro assumes the event is actually a key event.
 *      when compiled into the application I had to use the mod value and when
 *      using a DLL the sym value. Thus this macro helps eleviate the problem.
 *      Event though this problem should not exist in the first place :|
 */
#define SDLKEYCODE( keyevent ) ( (keyevent).keysym.mod ? (keyevent).keysym.mod : (keyevent).keysym.sym )

/*-------------------------------------------------------------------------*/

/*
 *      Macro : Create a new pointer of type given with given offset of old
 *      pointer. Note that this is not a verry safe macro to use but it
 *      does allow you to get around some problems you would have if you
 *      implemented this as a function.
 */
#undef MAKEPTR
#define MAKEPTR(Type, Base, Offset) ((Type)((UInt32)(Base)) + ((UInt32)(Offset)))

/*-------------------------------------------------------------------------*/

/*
 *      Bit operations.
 *      Use the following macro's to set/get a bit in an integer.
 */
#undef GETBITX
#define GETBITX( var, x ) ( var & ( 1<<x ) )
#undef SETBITXON
#define SETBITXON( var, x ) ( var |= 1<<x )
#undef SETBITXOFF
#define SETBITXOFF( var, x ) ( var &= ~( 1<<x ) )

/*-------------------------------------------------------------------------*/

/*
 *      Macros to get the smallest or largest of 2 values.
 */
#define SMALLEST(a,b) (((a) < (b)) ? (a) : (b))
#define LARGEST(a,b) (((a) > (b)) ? (a) : (b))

/*-------------------------------------------------------------------------*/

/*
 *      Some math defines. These are offcourse faster then function calls.
 */
#ifndef RADIANS
#define RADIANS (3.14159/180.0)
#endif
    /*
#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif   */

#ifndef DEG2RAD
#define DEG2RAD( a ) ( (a) * ( PI/180 ) )
#endif
#ifndef RAD2DEG
#define RAD2DEG( a ) ( (a) * ( 180.0/PI ) )
#endif

/*-------------------------------------------------------------------------*/

/*
 *      Some macros for copying integers into a char buffer per byte. Note that
 *      for use in network communication you should set a endianess to be used
 *      for the protocol. 
 */
#define INT32INTOCHAR( dest_char_prt, src_int32 ) \
 { \
   { \
     ((char*)dest_char_prt)[ 0 ] = ((char*)&src_int32)[ 0 ]; \
     ((char*)dest_char_prt)[ 1 ] = ((char*)&src_int32)[ 1 ]; \
     ((char*)dest_char_prt)[ 2 ] = ((char*)&src_int32)[ 2 ]; \
     ((char*)dest_char_prt)[ 3 ] = ((char*)&src_int32)[ 3 ]; \
   } \
 }

#define CHARINTOINT32( dest_int32, src_char_prt ) \
 { \
   { \
     ((char*)&dest_int32)[ 0 ] = ((char*)src_char_prt)[ 0 ]; \
     ((char*)&dest_int32)[ 1 ] = ((char*)src_char_prt)[ 1 ]; \
     ((char*)&dest_int32)[ 2 ] = ((char*)src_char_prt)[ 2 ]; \
     ((char*)&dest_int32)[ 3 ] = ((char*)src_char_prt)[ 3 ]; \
   } \
 }

//#define INT16INTOCHAR

#endif /* GUCEF_MT_BASICMACROS_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-03-2004 :
        - Removed all build specific macros. These are macros that relyed on
          specific build switches to define there meaning. This required the
          include of a config include here which causes problems when exporting
          from one module and importing from another since they would both use
          the same switch. Thus only generic macros are allowed here from now
          on.
- 05-09-2003 :
        - Added the LARGEST and SMALLEST macros
- 13-08-2003 :
        - Added this section.
        - Added MAX_DIR_LENGTH and MAX_FILENAME_LENGTH

---------------------------------------------------------------------------*/



