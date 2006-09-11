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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <malloc.h>                     /* memory managerment */

#include "gui_fileutils.h"              /* function protoypes */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUIF_MAGIC_NUMBER       87238

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Tests wether or not the recource is a GUI file.
 *      Note that this function assumes the current location in the recource is
 *      the start of the file. The location will not have changed after
 *      completing this operation.
 */
UInt32
GUIF_Test_If_GUI_File( TIOAccess *access )
{
        UInt32 fpos = access->tell( access );
        UInt32 testval;
        if ( access->read( access, &testval, 4, 1 ) < 4 )
        {
                access->seek( access, fpos, SEEK_SET );
                return 0;
        }
        access->seek( access, fpos, SEEK_SET );
        return testval == GUIF_MAGIC_NUMBER;
}

/*-------------------------------------------------------------------------*/

/**
 *      Function that reads all the data from the GUI file.
 *      NULL is returned if an error occured. This function assumes the current
 *      location in the recource is the start of the GUI file. This allows you
 *      to embed a GUI file in another type of file format. Note that this
 *      function allocates memory that should be freed using
 *      GUIF_Free_Image_Data().
 */
UInt32
GUIF_Read_Image_Data( TIOAccess *access ,
                      TImageData *dest  )
{
        UInt32 magicnr;

        /*
         *      Check magic number
         */
        access->read( access, &magicnr, 1, 4 );
        if ( magicnr != GUIF_MAGIC_NUMBER ) return 0;

        /*
         *     read header
         */
        access->read( access, &dest->dtype, 1, 1 );
        access->read( access, &dest->vsize, 1, 4 );
        access->read( access, &dest->channels, 1, 1 );
        access->read( access, &dest->has_alpha, 1, 1 );
        access->read( access, &dest->width, 1, 4 );
        access->read( access, &dest->height, 1, 4 );
        access->read( access, &dest->bbp, 1, 1 );
        access->read( access, &dest->stride, 1, 4 );
        access->read( access, &dest->dsize, 1, 4 );
        access->read( access, &dest->format, 1, 1 );
        access->read( access, &dest->frames, 1, 4 );
        access->read( access, &dest->icf, 1, 1 );        

        /*
         *      read image pixmap.
         */
        dest->idata = malloc( dest->dsize );
        access->read( access, dest->idata, dest->dsize, 1 );

        return 1;
}

/*-------------------------------------------------------------------------*/

UInt32
GUIF_Save_Image_Data( FILE *fptr            ,
                      const TImageData *src )
{
        UInt32 magicnr = GUIF_MAGIC_NUMBER;

        /*
         *      Write magic number
         */
        if ( fwrite( &magicnr, 1, 4, fptr ) != 4 ) return 0;

        /*
         *      Write header
         */
        fwrite( &src->dtype, 1, 1, fptr );
        fwrite( &src->vsize, 1, 4, fptr );
        fwrite( &src->channels, 1, 1, fptr );
        fwrite( &src->has_alpha, 1, 1, fptr );
        fwrite( &src->width, 1, 4, fptr );
        fwrite( &src->height, 1, 4, fptr );
        fwrite( &src->bbp, 1, 1, fptr );
        fwrite( &src->stride, 1, 4, fptr );
        fwrite( &src->dsize, 1, 4, fptr );
        fwrite( &src->format, 1, 1, fptr );
        fwrite( &src->frames, 1, 4, fptr );
        fwrite( &src->icf, 1, 1, fptr );        

        /*
         *      Write image pixmap
         */
        fwrite( src->idata, 1, src->dsize, fptr );
        return 1;
}

/*-------------------------------------------------------------------------*/

/**
 *      Deallocates memory that was allocated with GUIF_Read_Image_Data().
 */
void
GUIF_Free_Image_Data( TImageData *data )
{
        free( data->idata );
}

/*-------------------------------------------------------------------------*/

/**
 *      Simply opens a file and returns the FILE*.
 *      Needed because C file pointers cannot be used across DLL boundries. 
 */
FILE*
GUIF_Openfile( const char *filename ,
               const char *mode     )
{
        return fopen( filename, mode );
}

/*-------------------------------------------------------------------------*/

/**
 *      Simply closes a file.
 *      Needed because C file pointers cannot be used across DLL boundries. 
 */
void
GUIF_Closefile( FILE *fptr )
{
        fclose( fptr );
}

/*-------------------------------------------------------------------------*/
 