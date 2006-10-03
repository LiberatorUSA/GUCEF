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
GUCEFCORE_EXPORT_C TPAFILEControl*
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
GUCEFCORE_EXPORT_C void
padestroy( TPAFILEControl *control );

/*-------------------------------------------------------------------------*/

/**
 *      Creates an additional link to the file. After you are done using the
 *      file you should call paunlink().
 */
GUCEFCORE_EXPORT_C PAFILE*
palink( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Creates an additional link to the file. After you are done using the
 *      file you should call paunlink().
 */
GUCEFCORE_EXPORT_C PAFILE*
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
GUCEFCORE_EXPORT_C PAFILE*
palinkchunktocontrol( TPAFILEControl *control ,
                      UInt32 start_offset     ,
                      UInt32 chunk_size       );

/*-------------------------------------------------------------------------*/

/**
 *      Clean's up a PAFILE structure which was created using palink() or
 *      using palinktocontrol().
 */
GUCEFCORE_EXPORT_C void
paunlink( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Returns wheter the PAFILE is active. 1 equals true and 0 equals false
 *      in this context. If the PAFILE is no longer linked then the control
 *      has been destroyed and the PAFILE should be cleaned up using paunlink()
 */
GUCEFCORE_EXPORT_C UInt32
palinked( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Returns the current file pointer, which is a void*
 */
GUCEFCORE_EXPORT_C UInt32
paftell( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Returns the size of the file
 */
GUCEFCORE_EXPORT_C UInt32
pafsize( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Set the file pointer to the given offset from the origin
 *      provided, origin can be : SEEK_CUR, SEEK_END, SEEK_SET
 */
GUCEFCORE_EXPORT_C Int32
pafseek( PAFILE *pafile ,
         UInt32 offset  ,
         Int32 origin   );

/*-------------------------------------------------------------------------*/

/**
 *      Set the file pointer to the given offset from the beginning of
 *      the file
 */
GUCEFCORE_EXPORT_C Int32
pafsetpos( PAFILE *pafile ,
           UInt32 offset  );

/*-------------------------------------------------------------------------*/

/**
 *	MFILE version of getc()
 */
GUCEFCORE_EXPORT_C Int32
pafgetc( PAFILE *pafile );


/*-------------------------------------------------------------------------*/

/**
 *	Identical to mfgetc()
 */
GUCEFCORE_EXPORT_C Int32
pagetc( PAFILE *pafile );


/*-------------------------------------------------------------------------*/

/**
 *      Check if the filepointer has reached the end of the file
 *      if so returns true
 */
GUCEFCORE_EXPORT_C Int32
pafeof( PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Binary read the maximum number of elements of esize each
 *      from current position
 */
GUCEFCORE_EXPORT_C UInt32
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
GUCEFCORE_EXPORT_C UInt32
pafreadl( char **dest    ,
          PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Use ASCII : read a string. meaning read untill
 *      a whitespace (blanks, TABs, CRs, LFs),
 *      works from current position. Note that this function allocates
 *      memory that should be freed using mfree()
 */
GUCEFCORE_EXPORT_C UInt32
pafreads( char **dest    ,
          PAFILE *pafile );

/*-------------------------------------------------------------------------*/

/**
 *      Free a block of memory that was allocated using pafreadl() or
 *      pafreads()
 */
GUCEFCORE_EXPORT_C void
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
