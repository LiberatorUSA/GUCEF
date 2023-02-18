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

#ifndef GUCEF_CORE_MFILE_H
#define GUCEF_CORE_MFILE_H


/*
 *      Purpose of these functions and type is to give file like access to
 *      a chunck of memory, comes in handy with files that where loaded into
 *      memory.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"             /* simple types used */
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
 *      Stores data needed to access mem chunck as file
 */
struct MFILE
{
        const char* mchunk; /* pointer to mem chunk with data */
        UInt64 size;        /* size of mem chunck */
        UInt64 offset;      /* current 'file pointer' position */
        const char* curpos; /* current mem address of file pointer */
};

typedef struct MFILE MFILE;

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
 *      Hook to memmory chunck and create and init MFILE struct
 *      However it is recommended to staticly allocate MFILE and then use
 *      mfinit
 */
GUCEF_CORE_PUBLIC_C MFILE*
mfcreate( const void *mchunk  ,
          UInt64 size         );

/*-------------------------------------------------------------------------*/

/**
 *      Cleans up MFILE data storage, note :
 *      you must use this function for cleanup if you used mfcreate()
 *      However it is recommended to staticly allocate MFILE and then use
 *      mfinit
 */
GUCEF_CORE_PUBLIC_C void
mfdestroy( MFILE *mfile );

/*-------------------------------------------------------------------------*/

/**
 *      Initialize values of an existing MFILE struct
 */
GUCEF_CORE_PUBLIC_C void
mfinit( MFILE* file        ,
        const void *mchunk ,
        const UInt64 size  );

/*-------------------------------------------------------------------------*/

/**
 *      Use ASCII : read a line. meaning read untill \n
 *      from current position. Note that this function allocates
 *      memory that should be freed using mfree()
 */                   /* FIXME */
GUCEF_CORE_PUBLIC_C UInt32
mfreadl( char **dest  ,
         MFILE *mfile );

/*-------------------------------------------------------------------------*/

/**
 *      Use ASCII : read a string. meaning read untill
 *      a whitespace (blanks, TABs, CRs, LFs),
 *      works from current position. Note that this function allocates
 *      memory that should be freed using mfree()
 */
GUCEF_CORE_PUBLIC_C UInt32
mfreads( char **dest  ,
         MFILE *mfile );

/*-------------------------------------------------------------------------*/

/**
 *      Free a block of memory that was allocated using mfreadl() or
 *      mfreads()
 */
GUCEF_CORE_PUBLIC_C void
mfree( void *mem );

/*-------------------------------------------------------------------------*/

/**
 *      Binary read the maximum number of elements of esize each
 *      from current position
 */
GUCEF_CORE_PUBLIC_C UInt32
mfread( void *dest      ,
        UInt32 esize    ,
        UInt32 elements ,
        MFILE *mfile    );

/*-------------------------------------------------------------------------*/

/**
 *      Returns the current file pointer, which is a void*
 */
GUCEF_CORE_PUBLIC_C UInt64
mftell( const MFILE *mfile );

/*-------------------------------------------------------------------------*/

/**
 *      Set the file pointer to the given offset from the origin
 *      provided, origin can be : SEEK_CUR, SEEK_END, SEEK_SET
 */
GUCEF_CORE_PUBLIC_C Int32
mfseek( MFILE *mfile ,
        Int64 offset ,
        Int32 origin );

/*-------------------------------------------------------------------------*/

/**
 *      Set the file pointer to the given offset from the beginning of
 *      the file
 */
GUCEF_CORE_PUBLIC_C Int32
mfsetpos( MFILE *mfile   ,
          UInt64 offset  );

/*-------------------------------------------------------------------------*/

/**
 *	MFILE version of getc()
 */
GUCEF_CORE_PUBLIC_C char
mfgetc( MFILE *mfile );

/*-------------------------------------------------------------------------*/

/**
 *	Identical to mfgetc()
 */
GUCEF_CORE_PUBLIC_C char
mgetc( MFILE *mfile );


/*-------------------------------------------------------------------------*/

/**
 *      Check if the filepointer has reached the end of the file
 *      if so returns true
 */
GUCEF_CORE_PUBLIC_C Int32
mfeof( const MFILE *mfile );

/*-------------------------------------------------------------------------*/

/**
 *      Dump memmory chunk to file with name specified
 *      mode should be "w" or "wb"
 */
GUCEF_CORE_PUBLIC_C Int32
mfdumptofile( MFILE *mfile         ,
              const char *filename ,
              const char *mode     );

/*-------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_MFILE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-05-2005 :
        - Finally got arround to fixing a known bug in mfreadl(): the end of line
          delimiters where not correctly detected.
- 01-05-2005 :
        - Fixed a bug in mfsetpos(): It was possible to set the file position past
          the size of the file buffer under certain conditions.
        - Added range checking in mfseek() to avoid nasty bounds violations that
          could cause memory corruption.
        - Fixed a bug in mfgetc() that would cause bytes to be skipped.
- 05-03-2005 :
        - Changed void* into char*. Same result with less type-cast hassle.
- 29-02-2004 :
        - Added mfree(). This function should be used to free memory allocated
          trough the use of mfreadl() or mfreads().
- 14-12-2003 :
        - Added mgetc() and mfgetc() which is the MFILE version of fgetc() and
          getc()
- 18-08-2003 :
        - Changed EXPORT into GUCEF_CORE_PUBLIC_C
        - Disabled mfwrite untill I implement some new writing code which would
          need a seperate struct from MFILE because the memory block the MFILE
          is assigned to should never change.
- 13-08-2003 :
        - Updated layout of this file to new standard.
- 23-07-2003 :
	- Fixed a bug in mfseek() with origin argument SEEK_CUR. The file offset
          counter would be correct but not the actual pointer in memory which
          offcourse caused problems.
- 18-03-2003 :
        - Fixed a bug in the mfsetpos() function that caused mfile.curpos to
          have the wrong value.
- 23-01-2003 :
        - Added this info section.
        - Finally got around to adding mfseek(). This means that changing
          to in memory handling will only involve putting a m in front of
          regular stdio.h functions,.. Yay.
        - Changed DWORD into UInt32 type and started making use of ETypes.h


---------------------------------------------------------------------------*/


