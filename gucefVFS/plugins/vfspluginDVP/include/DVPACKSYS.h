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

#ifndef DVPACKSYS_H
#define DVPACKSYS_H

/*
 *      Module for dealing with dvp and idx files which is the custom pack file
 *      format used by Dinand Vanvelzen.
 *      Note that in order to keep the index size to a minimum yet
 *      predictable size the name of an index entry is limited in length to
 *      DVP_MAX_INAME_LENGTH. If a longer name is is found the operation will
 *      fail.
 *
 *      This module contains no API calls or other non-ANSI code. It instead
 *      rely's on wrapper functions that wrap OS dependant API calls.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>           /* standard lib */
#include <stdio.h>            /* standard I/O */
#include <string.h>           /* general string utils */
#include <time.h>             /* needed for timestamp */
#include "gucefCORE_ETypes.h" /* simple types used */

#include "ioaccess.h"   /* source independant I/O */

#ifndef GUCEFVFS_MACROS_H
#include "gucefVFS_macros.h"     /* often used gucef macros */
#define GUCEFVFS_MACROS_H
#endif /* GUCEFVFS_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      DEFINES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Build switch to enable or remove support for creation and manipulation
 *      of pack files. If disabled only code for reading existing pack files 
 *      will be left.
 */
#define DVP_READ_ONLY_BUILD 1

/*---------------------------------------------------------------------------*/

#define DVP_MAJOR_VERSION       1                               /* mayor version number of this module */
#define DVP_MINOR_VERSION       1                               /* minor version number of this module */
#define DVP_RELEASE_VERSION     0                               /* release version number of this module */
#define DVP_PATCH_VERSION       0                               /* patch version number of this module */

#define DVP_BLOCK_SIZE          (1024*1024) /* 1 MB */          /* default block size when reading/writing files */
#define DVP_PACK_EXT            "dvp"                           /* file extension of pack files */
#define DVP_IDX_EXT	        "idx"                           /* file extension of index files */
#define DVP_CHECKSUM            843432536                       /* checksum, used to check for corrupt files */
#define DVP_MAX_INAME_LENGTH    32                              /* max allowed length of filename in the index */
#define DVP_ID_LENGTH           4                               /* max allowed size of the packfile ID */
#define DVP_HEADER_SIZE         (6 * 4 + DVP_ID_LENGTH)         /* size of the packfile header in bytes */
#define DVP_INDEX_ENTRY_SIZE    (3 * 4 + DVP_MAX_INAME_LENGTH)  /* size of an index entry in bytes */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Header format used for the Dinand Vanvelzen Pack files (.dvp)
 *      Don't use struct but seperate vars in I/O for size because of
 *      compiler packing !!!
 *
 *      - char id[4]               4 bytes
 *      - UInt32 version           4 bytes
 *      - UInt32 index_offset      4 bytes
 *      - UInt32 num_files         4 bytes
 *      - UInt32 num_dirs          4 bytes
 *      - UInt32 lastchange        4 bytes
 *      - UInt32 checksum          4 bytes
 *                                28 bytes total
 *
 *      Note that the number of index entry's is num_files + ( num_dirs*2 ) !!!
 *
 */
struct SDVP_File_Header
{
        char ID[ 4 ];               /* ID of packfile, can be used as an identity marker for grouping pack files */
        UInt32 version;             /* version of the pack file, this determines pack file layout */
        UInt32 index_offset;        /* offset in bytes of the index from the start of the file */
        UInt32 num_files;           /* total number of files packed */
        UInt32 num_dirs;            /* total number of directory's in packed dir tree */
        UInt32 lastchange;          /* timestamp indicating the last time the file was changed */
        UInt32 checksum;            /* this must equal DVP_CHECKSUM */
};

typedef struct SDVP_File_Header TDVP_File_Header;

/*---------------------------------------------------------------------------*/

/**
 *      Index entry format used for the .idx and thus .dvp files
 *      Don't use struct but seperate vars in I/O for size because of
 *      compiler packing !!!
 *
 *      size = 16 + DVP_MAX_INAME_LENGTH = DVP_INDEX_ENTRY_SIZE = atm 44
 */
struct SDVP_Index_Entry
{
        UInt32 offset;
        UInt32 size;
        char name[ DVP_MAX_INAME_LENGTH ];
        UInt32 timestamp;
};

typedef struct SDVP_Index_Entry TDVP_Index_Entry;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Note that these functions will return non-zero if they fail and zero
 *      on success.
 */

/*---------------------------------------------------------------------------*/

/*
 *      Prevent C++ name mangling
 */
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

/**
 *      Read the archive header data
 *      The position of the file pointer will be reset to the original position
 *      after completion. The file fp should be open in rb mode.
 */
EXPORT_C UInt32
_DVP_IOA_Read_Header( TIOAccess *access        ,
                      TDVP_File_Header *header );

/*---------------------------------------------------------------------------*/

/**
 *      Read the archive header data
 *      The position of the file pointer will be reset to the original position
 *      after completion. The file fp should be open in rb mode.
 */
EXPORT_C UInt32
_DVP_Read_Header( FILE *fp                 ,
                  TDVP_File_Header *header );

/*---------------------------------------------------------------------------*/                        

/**
 *      Read the archive header data
 *      The position of the file pointer will be reset to the original position
 *      after completion. The file fp should be open in rb mode.
 */
EXPORT_C UInt32
DVP_IOA_Read_Header( TIOAccess *access       ,
                     char   *ID              ,
                     UInt32 *version         ,
                     UInt32   *index_offset  ,
                     UInt32   *num_files     ,
                     UInt32   *num_dirs      ,
                     UInt32  *lastchange     );

/*---------------------------------------------------------------------------*/                        

/**
 *      Read the archive header data
 *      The position of the file pointer will be reset to the original position
 *      after completion. The file fp should be open in rb mode.
 */
EXPORT_C UInt32
DVP_Read_Header( FILE *fptr              ,
                 char   *ID              ,
                 UInt32 *version         ,
                 UInt32   *index_offset  ,
                 UInt32   *num_files     ,
                 UInt32   *num_dirs      ,
                 UInt32  *lastchange     );

/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Write the header to the archive
 *      The position of the file pointer will be reset to the original position
 *      after completion. The file fp_out should be open in wb mode.
 */
EXPORT_C UInt32
DVP_Write_Header( FILE *fptr               ,
                  const char *ID           ,
                  UInt32 index_offset      ,
                  UInt32 num_of_files      ,
                  UInt32 num_of_dirs       );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Extract the index of the archive into an external file
 */
EXPORT_C UInt32
DVP_IOA_Extract_Index_File( TIOAccess *access   ,
                            const char *hfout   ,
                            UInt32 index_offset ,
                            UInt32 num_entrys   );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Extract the index of the archive into an external file
 */
EXPORT_C UInt32
DVP_Extract_Index_File( FILE *dfile         ,
                        const char *hfout   ,
                        UInt32 index_offset ,
                        UInt32 num_entrys   );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Verion of DVP_Extract_Index_File() for lazy users. obtains the 2 other
 *      parameters for DVP_Extract_Index_File() itself.
 */
EXPORT_C UInt32
_DVP_IOA_Extract_Index_File( TIOAccess *access    ,
                             const char *filename );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Verion of DVP_Extract_Index_File() for lazy users. obtains the 2 other
 *      parameters for DVP_Extract_Index_File() itself.
 */
EXPORT_C UInt32
_DVP_Extract_Index_File( FILE *dfile          ,
                         const char *filename );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Extracts a block ( file ) from the archive into a external file
 *      specified by filename
 */
EXPORT_C UInt32
DVP_IOA_Extract_File( TIOAccess *access       ,
                      UInt32 offset           ,
                      UInt32 filesize         ,
                      UInt32 blocksize        ,
                      const char *filename    );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Extracts a block ( file ) from the archive into a external file
 *      specified by filename
 */
EXPORT_C UInt32
DVP_Extract_File( FILE *fptr              ,
                  UInt32 offset           ,
                  UInt32 filesize         ,
                  UInt32 blocksize        ,
                  const char *filename    );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      This is more or less a version of DVP_Extract_File() for lazy users.
 *      it obtains the 3 other parameters that DVP_Extract_File() needs itself.
 */
EXPORT_C UInt32
_DVP_IOA_Extract_File( TIOAccess *access     ,
                       const char *filename  ,
                       const char *dest_file );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      This is more or less a version of DVP_Extract_File() for lazy users.
 *      it obtains the 3 other parameters that DVP_Extract_File() needs itself.
 */
EXPORT_C UInt32
_DVP_Extract_File( FILE *fptr            ,
                   const char *filename  ,
                   const char *dest_file );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Extracts all files stored within the archive with their respective
 *      paths starting in the specified dest_dir dir.
 */
EXPORT_C UInt32
DVP_IOA_Extract_All_Files( TIOAccess *access      ,
                           const char *dest_dir   );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Extracts all files stored within the archive with their respective
 *      paths starting in the specified dest_dir dir.
 */
EXPORT_C UInt32
DVP_Extract_All_Files( FILE *fptr             ,
                       const char *dest_dir   );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Load a specified block ( file ) from the archive into a memory in a
 *      character buffer
 */
EXPORT_C UInt32
DVP_IOA_Load_File_Into_Mem( TIOAccess *access ,
                            UInt32  offset    ,
                            UInt32  filesize  ,
                            UInt32  *rbytes   ,
                            void *dest        );

#endif
/*---------------------------------------------------------------------------*/

/**
 *      Load a specified block ( file ) from the archive into a memory in a
 *      character buffer
 */
EXPORT_C UInt32
DVP_Load_File_Into_Mem( FILE *fpt        ,
                        UInt32  offset   ,
                        UInt32  filesize ,
                        UInt32  *rbytes  ,
                        void *dest       );


/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Appends the index file to the data file
 *      note that entrys is dirs*2 + files.
 *      The position of the file pointer will be reset to the original position
 *      after completion.
 */
EXPORT_C UInt32
DVP_Append_Index( FILE *datafile          ,
                  const char *indexfile   ,
                  UInt32 num_entrys       );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Packs a file into the archive data file and adds entry to index file
 */
EXPORT_C UInt32
DVP_Pack_File( FILE *dfpt            ,
               FILE *ifpt            ,
               const char *filespec  ,
               const char *filename  ,
               UInt32 *offset        ,
               UInt32 filesize       ,
               UInt32 time_write     );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Write an index entry to file
 *      Note that istr must have a minimum length of DVP_MAX_INAME_LENGTH
 *      file fpt must be open in write mode wb
 */
EXPORT_C UInt32
DVP_Write_Index_Entry( UInt32 offset    ,
                       UInt32 eoffset   ,
                       UInt32 size      ,
                       UInt32 tstamp    ,
                       const char *istr ,
                       FILE *fpt        );

#endif
/*---------------------------------------------------------------------------*/

/**
 *      Read a single entry out of an archive index
 *      file fpt must be open in read mode rb
 */
EXPORT_C UInt32
DVP_IOA_Read_Index_Entry( UInt32 offset     ,
                          UInt32 *size      ,
                          UInt32 *i1        ,
                          UInt32 *i2        ,
                          char *istr        ,
                          TIOAccess *access );

/*---------------------------------------------------------------------------*/

/**
 *      Read a single entry out of an archive index
 *      file fpt must be open in read mode rb
 */
EXPORT_C UInt32
DVP_Read_Index_Entry( UInt32 offset ,
                      UInt32 *size  ,
                      UInt32 *i1    ,
                      UInt32 *i2    ,
                      char *istr    ,
                      FILE *fpt     );

/*---------------------------------------------------------------------------*/

/**
 *      Read a single entry out of an archive index
 *      file fpt must be open in read mode rb
 */
EXPORT_C UInt32
_DVP_IOA_Read_Index_Entry( UInt32 offset           ,
                           TDVP_Index_Entry *entry ,
                           TIOAccess *access       );

/*---------------------------------------------------------------------------*/

/**
 *      Read a single entry out of an archive index
 *      file fpt must be open in read mode rb
 */
EXPORT_C UInt32
_DVP_Read_Index_Entry( UInt32 offset           ,
                       TDVP_Index_Entry *entry ,
                       FILE *fptr              );

/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Adds a directory and all files it contains to the archive,
 *      in other words : the directory gets packed.
 */
EXPORT_C UInt32
DVP_Pack_Directory( FILE *dfpt            ,
                    FILE *ifpt            ,
                    UInt32 *total_size    ,
                    const char *path      ,
                    UInt32 *num_files     ,
                    UInt32 *num_dirs      );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Appends a directory entry to a archive's index file
 */
EXPORT_C UInt32
DVP_Append_Directory( const char *path  ,
                      UInt32 total_size ,
                      FILE *fp          );

#endif
/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Creates an archive starting from the dir specified and
 *      with the name specified.
 */
EXPORT_C UInt32
DVP_Create_Archive( const char *archive_name ,
                    const char *src_dir      ,
                    const char *ID           );

#endif
/*---------------------------------------------------------------------------*/

/**
 *      Print archive index to console
 */
EXPORT_C UInt32
DVP_Display_Index( FILE *fptr           ,
                   UInt32 index_offset  ,
                   UInt32 entrys        );

/*---------------------------------------------------------------------------*/

/**
 *      Get entry data by name of entry
 *      if there are multiple occurences of the name the first instance
 *      will be returned. if not found 1 is returned otherwise 0.
 */
EXPORT_C UInt32
DVP_IOA_Find_Entry_Data( TIOAccess *access       ,
                         const char *name        ,
                         TDVP_Index_Entry *entry );

/*---------------------------------------------------------------------------*/

/**
 *      Get entry data by name of entry
 *      if there are multiple occurences of the name the first instance
 *      will be returned. if not found 1 is returned otherwise 0.
 */
EXPORT_C UInt32
DVP_Find_Entry_Data( FILE *fptr             ,
                     const char *name        ,
                     TDVP_Index_Entry *entry );

/*---------------------------------------------------------------------------*/
#if !DVP_READ_ONLY_BUILD

/**
 *      Create new archive that does not have an index at the end.
 *      Might be usefull if you perhaps want to hardcode an index in memory
 *      preventing external reading from archive ( or at least making it harder )
 */
EXPORT_C UInt32
DVP_Strip_Index( const char *archive     ,
                 const char *new_archive );

#endif
/*---------------------------------------------------------------------------*/

/**
 *      Return current module version, usefull for comparing the module
 *      version an application was built with to the actually linked module
 *      version.
 */
EXPORT_C UInt32
DVP_Version( void );

/*---------------------------------------------------------------------------*/

/**
 *      Since we cannot pass AND use a file with a normal file pointer in both
 *      the main application and in a DLL we have the following functions.
 *      the file pointer itself can be passed without problems but you can only
 *      use the file pointer in the module that opened the file. !!!
 */
EXPORT_C FILE*
DVP_Open_File( const char *filename, const char *mode );

EXPORT_C void
DVP_Close_File( FILE *fptr );

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
};
#endif

/*---------------------------------------------------------------------------*/

#endif /* DVPACKSYS_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-04-2005 :
        - Added build switch that will allow read only builds to be made using 
          the same header and source. Read only builds have a lot less dependancies.
        - Got rid of debug output  
- 07-03-2004 :
        - Added IOA versions of several read functions. This functionality had
          to be doubled because the two versions simply cannot be mixed with
          running into problems like the FILE* being passed across DLL boundries. 
- 18-08-2003 :
        - Changed EXPORT into EXPORT_C.
- 17-08-2003 :
        - Removed if defined BUILD_DLL from DVP_Open_File() and DVP_Close_File()
          so that these can always be used in order to make it easy to switch
          form staticly linking the module to dynamicly linking it.
        - Changed the return type of DVP_Load_File_Into_Mem() into UInt32. It
          used to be void.
- 14-08-2003 :
        - Added DVP_Close_Output() which in a debug dll build closes the debug
          file, you should call this at program exit. This function will be
          removed in normal release versions which will have no debug output.
        - Added DVP_Open_File() and DVP_Close_File() because you cannot use
          file pointers for file operations in both the main app and dll
          without including a memory manager of some sort which i don't want to
          do. Rule of thumb: You can only use the file handle in the module
          that opened the file. You can however store it anywhere since it's
          just a pointer. The reason for passing file pointers is so that we
          don't have to open and close the file all the time. The reason why
          i did not make the file pointer global should be obvious. First and
          formost because you should never use global variables and second
          because this way multiple files can be manipulated parallel with this
          lib this way, offcourse not the same file in paralell threads ;)
        - Added _DVP_Extract_File() and _DVP_Extract_Index_File() which obtain
          some parameters for there sibling funtions themselves. Not verry
          efficient if used repeatedly.
        - Changed to new version number system.
        - This version will be the first released version of this module.
          version number is 1110.
        - Added DVP_Version() so that the module version with which a application
          was built can be compared to the actually linked version.
- 13-08-2003 :
        - Implemented DVP_Extract_All_Files() and fixed a small bug in
          DVP_Extract_File().
        - Now uses my own cross-platform functions for dir itteration ect.
          instead of Win32 API calls.
- 12-08-2003 :
        - Added this section. Funfact: this was the first module I coded when I
          started on the GUCEF project.
        - replaced old disclaimer with the new one
        - replaced all simple types ( int ect. ) with my own.
        - Added const keywords to strings where needed
        - Removed they key feature. It's much better to crypt/decrypt outside
          of this module.
        - Extracting all files is not implemented yet, nor is writing date/time
          timestamp which currently simply gets set to 0.

---------------------------------------------------------------------------*/
