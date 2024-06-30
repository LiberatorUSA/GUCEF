/*
 *  gucefMT: GUCEF module providing multithreading solutions
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
//#define SDLKEYCODE( keyevent ) ( (keyevent).keysym.mod ? (keyevent).keysym.mod : (keyevent).keysym.sym )

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
 *      Macros to get the smallest or largest of 2 values.
 */
#define GUCEF_SMALLEST(a,b) (((a) < (b)) ? (a) : (b))
#define GUCEF_LARGEST(a,b) (((a) > (b)) ? (a) : (b))

#define SMALLEST    GUCEF_SMALLEST
#define LARGEST     GUCEF_LARGEST

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
