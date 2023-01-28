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

/*
 *      The functions in this header are meant to give a you the ability to
 *      make access of a file on hard disk threadsafe with multiple threads
 *      reading from the same file. This is not possible using a normal FILE*
 *      first and formost because the file pointer is moved after a read
 *      operation. The pacreate() and padestroy() are unlike the rest of the
 *      functions here NOT threadsafe. The PA prefix stands for paralell
 *      access. SDL's mutex wrappers are used internally to implement the
 *      threadsafety. Do not create multiple control's for the same physical
 *      file !!!. Create multiple PAFILE structures instead.
 */

#ifndef GUCEF_CORE_PAFILE_H
#define GUCEF_CORE_PAFILE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdio.h>              /* needed for the fseek enums */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"             /* simple types used */
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
 *      Storage for the Paralell access control mechanism
 */
struct SPAFILEControl;
typedef struct SPAFILEControl TPAFILEControl;

/*
 *      PA version of a FILE*
 */
struct SPAFILE;
typedef struct SPAFILE PAFILE;

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
 *      This opens a file on disk for parallell access reading. filename should
 *      be the filename and path for the file you want to create parallell access
 *      for offcourse. mode should be either "rb" or "r".
 *      If you no longer need the parallel file access you should call
 *      padestroy() to deallocate the data.
 *      This function is in itself not threadsafe.
 */
GUCEF_CORE_PUBLIC_C TPAFILEControl*
pacreate( const char *filename ,
          const char *mode     );

/*-------------------------------------------------------------------------*/

/**
 *      Clean's up a TPAFILEControl structure which was allocated using
 *      pacreate() or pacreatechunk(). All links to the file are disabled.
 *      So make sure your code stops using them before calling this function.
 *      The file on disk will be closed.
 *      This function is in itself not threadsafe.
 */
GUCEF_CORE_PUBLIC_C void
padestroy( TPAFILEControl *control );

/*-------------------------------------------------------------------------*/

/**
 *      Creates an additional link to the file. After you are done using the
 *      file you should call paunlink().
 */
GUCEF_CORE_PUBLIC_C PAFILE*
palink( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Creates an additional link to the file. After you are done using the
 *      file you should call paunlink().
 */
GUCEF_CORE_PUBLIC_C PAFILE*
palinktocontrol( TPAFILEControl *control );

/*-------------------------------------------------------------------------*/

/**
 *      Basicly the same as palinktocontrol() with the exception that you
 *      can create a PAFILE* that will appear to be a complete file but will
 *      in reality only provide access to a specified section of the physical
 *      file. This allows you to treat a chunk of a physical file as a seperate
 *      file. A chunk size of 0 causes the remainder of the file from the given
 *      offset to be used as the file size.
 */
GUCEF_CORE_PUBLIC_C PAFILE*
palinkchunktocontrol( TPAFILEControl *control ,
                      UInt64 start_offset     ,
                      UInt64 chunk_size       );

/*-------------------------------------------------------------------------*/

/**
 *      Clean's up a PAFILE structure which was created using palink() or
 *      using palinktocontrol().
 */
GUCEF_CORE_PUBLIC_C void
paunlink( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Returns wheter the PAFILE is active. 1 equals true and 0 equals false
 *      in this context. If the PAFILE is no longer linked then the control
 *      has been destroyed and the PAFILE should be cleaned up using paunlink()
 */
GUCEF_CORE_PUBLIC_C UInt32
palinked( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Returns the current file pointer, which is a void*
 */
GUCEF_CORE_PUBLIC_C UInt64
paftell( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Returns the size of the file
 */
GUCEF_CORE_PUBLIC_C UInt64
pafsize( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Set the file pointer to the given offset from the origin
 *      provided, origin can be : SEEK_CUR, SEEK_END, SEEK_SET
 */
GUCEF_CORE_PUBLIC_C Int32
pafseek( PAFILE *pafile ,
         Int64 offset   ,
         Int32 origin   );

/*-------------------------------------------------------------------------*/

/**
 *      Set the file pointer to the given offset from the beginning of
 *      the file
 */
GUCEF_CORE_PUBLIC_C Int32
pafsetpos( PAFILE *pafile ,
           UInt64 offset  );

/*-------------------------------------------------------------------------*/

/**
 *	MFILE version of getc()
 */
GUCEF_CORE_PUBLIC_C Int32
pafgetc( PAFILE *pafile );


/*-------------------------------------------------------------------------*/

/**
 *	Identical to mfgetc()
 */
GUCEF_CORE_PUBLIC_C Int32
pagetc( PAFILE *pafile );


/*-------------------------------------------------------------------------*/

/**
 *      Check if the filepointer has reached the end of the file
 *      if so returns true
 */
GUCEF_CORE_PUBLIC_C Int32
pafeof( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Binary read the maximum number of elements of esize each
 *      from current position
 */
GUCEF_CORE_PUBLIC_C UInt32
pafread( void *dest      ,
         UInt32 esize    ,
         UInt32 elements ,
         PAFILE *pafile  );

/*-------------------------------------------------------------------------*/

/**
 *      Use ASCII : read a line. meaning read untill \n
 *      from current position. Note that this function allocates
 *      memory that should be freed using mfree()
 */
GUCEF_CORE_PUBLIC_C UInt32
pafreadl( char **dest    ,
          PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Use ASCII : read a string. meaning read untill
 *      a whitespace (blanks, TABs, CRs, LFs),
 *      works from current position. Note that this function allocates
 *      memory that should be freed using mfree()
 */
GUCEF_CORE_PUBLIC_C UInt32
pafreads( char **dest    ,
          PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Free a block of memory that was allocated using pafreadl() or
 *      pafreads()
 */
GUCEF_CORE_PUBLIC_C void
pafree( void *mem );

/*-------------------------------------------------------------------------*/

/*
 *      End of the C++ name mangling protection
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

#endif /* GUCEF_CORE_PAFILE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-03-2004 :
        - Added the ability to use a specific chunk of a file as if it where a
          seperate file. This will be especially usefull when utilizing pack
          files. The pack file segment can be passed to the user without the
          user ever knowing the file is actually located in a pack file.
          You can use this ability through the palinkchunktocontrol() function.
- 05-03-2004 :
        - Created this set of functions to offer a way to have parallel access
          to a file on disk. Mind you, the access is readonly. But it is still
          a verry powerfull utility if you combine it with a file manager such
          as a VFS (which is why I created these functions).

---------------------------------------------------------------------------*/
