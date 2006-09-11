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

#include <string.h>             /* needed for strcmp() */
#include <malloc.h>             /* memory management */

#ifndef ESTRUCTS_H
#include "EStructs.h"           /* generic GUCEF structures */
#define ESTRUCTS_H
#endif /* ESTRUCTS_H ? */  

#ifndef GUI_FILEUTILS_H
#include "gui_fileutils.h"     /* Utils for handling the Galaxy Unlimited Image file format */
#define GUI_FILEUTILS_H
#endif /* GUI_FILEUTILS_H ? */

#ifndef IMGPLUGLINK_H
#include "imgpluglink.h"      /* data types used for storing the link's */
#define IMGPLUGLINK_H
#endif /* IMGPLUGLINK_H ? */

#include "GUCEFimgloaders.h"    /* prototypes of the externally available functions implemented here */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
            
const char* loadextlist[ 2 ];
const char* saveextlist[ 2 ];

#define MAJOR_VERSION   1
#define MINOR_VERSION   0
#define PATCH_VERSION   0
#define RELEASE_VERSION 0

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static struct SVersion version;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Check if the recource provided is of a type we can load using
 *      this plugin. the extension string given should be checked against
 *      the known extension of the loadable types.
 *      A return value of 0 indicates failure. A return value of > 0 indicates
 *      success (I recommend using 1).
 */
UInt32
GUCEF_IMGPLUG_Check_If_Loadable_Using_Ext( const char *ext ,
                                           UInt32 *hidx    )
{
        /*
         *      This plugin supports Galaxy Unlimited Image format loading.
         */
        if ( strcmp( ext, "gui" ) == 0 )
        {
                return 1;
        }              
        return 0;
}

/*-------------------------------------------------------------------------*/

/**
 *      Check if the recource provided is of a type we can load using
 *      this plugin. The common way to do this check would be to check for
 *      some kind of identifing feature in the file header.
 *      A return value of 0 indicates failure. A return value of > 0 indicates
 *      success (I recommend using 1).
 */
UInt32
GUCEF_IMGPLUG_Check_If_Loadable( TIOAccess *access ,
                                 UInt32 *hidx      )
{
        if ( GUIF_Test_If_GUI_File( access ) )
        {
                /*
                 *      This appears to be a valid Galaxy Unlimited Image file.
                 */
                *hidx = IMGFORMAT_GALAXY_UNLIMITED_IMAGE;
                return 1;
        }
        return 0;
}

/*-------------------------------------------------------------------------*/

/**
 *      This function should check if the given format is supported by the
 *      plugin loader
 */
UInt32
GUCEF_IMGPLUG_Check_If_Format_Loadable( Int32 format ,
                                        UInt32 *hidx )
{
        switch ( format )
        {
                case IMGFORMAT_GALAXY_UNLIMITED_IMAGE :
                {
                        *hidx = IMGFORMAT_GALAXY_UNLIMITED_IMAGE;
                        return 1;
                }
                default :
                {
                        return 0;
                }
        }   
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
GUCEF_IMGPLUG_Check_If_Saveable_Using_Ext( const char *filename , 
                                           UInt32 *hidx         )
{
        if ( strcmp( "gui", filename ) == 0 )
        {
                *hidx = IMGFORMAT_GALAXY_UNLIMITED_IMAGE;
                return 1;
        }
        return 0;
}                                           

/*-------------------------------------------------------------------------*/

UInt32
GUCEF_IMGPLUG_Check_If_Saveable( const TImageData* imgdata , 
                                 UInt32 *hidx              )
{
        /*
         *      We can always save as a gui file.
         */
        *hidx = IMGFORMAT_GALAXY_UNLIMITED_IMAGE; 
        return 1;
}                                 

/*-------------------------------------------------------------------------*/

/**
 *      Check if the plugin can save in the explicit image storage format given.
 */
UInt32 
GUCEF_IMGPLUG_Check_If_Format_Saveable( Int32 format ,
                                        UInt32 *hidx )
{
        switch ( format )
        {
                case IMGFORMAT_GALAXY_UNLIMITED_IMAGE :
                {
                        *hidx = IMGFORMAT_GALAXY_UNLIMITED_IMAGE;
                        return 1;
                }
                default :
                {
                        return 0;
                }
        }
        return 0;
}

/*-------------------------------------------------------------------------*/

/**
 *      This function should load the image data into the TImageData struct
 *      provided from the recource which is accessable using the functions
 *      available in the TIOAccess structure. ext may hold the extension of
 *      the original filename but it may also be NULL. plugdata may be used to
 *      pass data to the application for safekeeping untill
 *      IMGPLUG_Unload_Image_Data() is called, which will then recieve the data
 *      set as plugdata here. The plugin is free to use the plugdata pointer
 *      as it sees fit.
 */
UInt32           
GUCEF_IMGPLUG_Load_Image_Data( UInt32 hidx         ,
                               TIOAccess *access   ,
                               TImageData *imgdata ,
                               void **plugdata     )
{
        *plugdata = NULL;
        switch ( hidx )
        {
                case IMGFORMAT_GALAXY_UNLIMITED_IMAGE :
                {
                        return GUIF_Read_Image_Data( access, imgdata );
                }
                default :
                {
                        return 0;
                }
        }
        return 0;
}

/*-------------------------------------------------------------------------*/

/**
 *      This function should save the image data provided in the format
 *      provided to a file with the given filename. If format is one of the
 *      explicit types then the save must be in that format or the save
 *      operation should fail. if format is IMGFORMAT_DONT_CARE then the
 *      format of the output file can be decided uppon at the user's discression.
 *      A return value of 0 indicates failure. A return value of > 0 indicates
 *      success (I recommend using 1). compression is a percentage between 0
 *      and 100 indicating the desired amount of compression. 0 indicates the
 *      lowest level of compression the plugin can provide and 100 the highest.
 */
UInt32
GUCEF_IMGPLUG_Save_Image_Data( UInt32 hidx               ,
                               const char *filename      ,
                               UInt32 format             ,
                               UInt32 compression        ,
                               const TImageData *imgdata )
{
        switch ( hidx )
        {
                case IMGFORMAT_DONT_CARE :
                case IMGFORMAT_GALAXY_UNLIMITED_IMAGE :
                {
                        /*
                         *      The GUI format can handle any kind of data
                         *      format so we don't need to check it.
                         */
                        FILE* fptr = GUIF_Openfile( filename, "wb" ); 
                        UInt32 retval = GUIF_Save_Image_Data( fptr, imgdata );
                        GUIF_Closefile( fptr );
                        return retval;
                }
                default :
                {
                        return 0;
                }
        }
        return 0;
}

/*-------------------------------------------------------------------------*/

/**
 *      Cleanup data allocated by the IMGPLUG_Load_Image_Data() function.
 *      Data cannot be de-allocated elsewhere or the memory management will
 *      go out of scope. plugdata is the same pointer that was set in
 *      IMGPLUG_Load_Image_Data().
 */
void 
GUCEF_IMGPLUG_Unload_Image_Data( UInt32 hidx         ,
                                 TImageData *imgdata ,
                                 void *plugdata      )
{
        switch ( hidx )
        {
                case IMGFORMAT_GALAXY_UNLIMITED_IMAGE :
                {
                        GUIF_Free_Image_Data( imgdata );
                        return;
                }
                default :
                {
                        return;
                }
        }
}

/*-------------------------------------------------------------------------*/

const char**
GUCEF_IMGPLUG_Loadable_Ext_List( void )
{
        return loadextlist;        
}

/*-------------------------------------------------------------------------*/

const char**
GUCEF_IMGPLUG_Saveable_Ext_List( void )
{
        return saveextlist;
}

/*-------------------------------------------------------------------------*/

const char*
GUCEF_IMGPLUG_Copyright( void )
{
        return "Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved." 
               "This software is the property of Dinand Vanvelzen. You may not sell"
               "or otherwise commercially exploit the software or things you created based on"
               "the software.\n"
               "THE SOFTWARE IS PROVIDED \"AS-IS\" AND WITHOUT WARRANTY OF ANY KIND,"
               "EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY"
               "WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE."
               "IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,"
               "INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER"
               "RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF"
               "THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT"
               "OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.";
}              
/*-------------------------------------------------------------------------*/

const char*
GUCEF_IMGPLUG_Name( void )
{
        return "Default GUCEF image codec";
}

/*-------------------------------------------------------------------------*/

const char*
GUCEF_IMGPLUG_Description( void )
{
        return "Default GUCEF image codec capable of loading GUCEF's GUI file format";
}

/*-------------------------------------------------------------------------*/

const TVersion*
GUCEF_IMGPLUG_Version( void )
{
        return &version;
}

/*-------------------------------------------------------------------------*/

/**
 *      Internally used GUCEF function, do not export.
 *      This function links image loader plugin functions which are not really
 *      in a plugin but rather compiled into the GUCEF module. This is good
 *      for testing new loaders and providing basic loading capability's at all
 *      time.
 */
void
GUCEF_IMGPLUG_Link_Functions( struct SImgLoaderPlugin* linkdata )
{
        /*
         *      Init the version structure
         */
        version.mayor = MAJOR_VERSION; 
        version.minor = MINOR_VERSION;
        version.patch = PATCH_VERSION;
        version.release = RELEASE_VERSION;
         
        /*
         *      This is not a real plugin so we set the handle pointer to NULL
         */
        linkdata->sdl_sohandle = NULL;
        linkdata->loadcount = 0;
        
        /*
         *      Initialize load and save list information
         */
        loadextlist[ 0 ] = "gui";
        loadextlist[ 1 ] = NULL;
        saveextlist[ 0 ] = "gui";
        saveextlist[ 1 ] = NULL;

        /*
         *      Link the functions here to the plugin link storage structure.
         */
        linkdata->IMGPLUG_Check_If_Loadable_Using_Ext = GUCEF_IMGPLUG_Check_If_Loadable_Using_Ext;
        linkdata->IMGPLUG_Check_If_Loadable = GUCEF_IMGPLUG_Check_If_Loadable;
        linkdata->IMGPLUG_Check_If_Format_Loadable = GUCEF_IMGPLUG_Check_If_Format_Loadable;
        linkdata->IMGPLUG_Check_If_Saveable_Using_Ext = GUCEF_IMGPLUG_Check_If_Saveable_Using_Ext;
        linkdata->IMGPLUG_Check_If_Saveable = GUCEF_IMGPLUG_Check_If_Saveable;        
        linkdata->IMGPLUG_Check_If_Format_Saveable = GUCEF_IMGPLUG_Check_If_Format_Saveable;
        linkdata->IMGPLUG_Save_Image_Data = GUCEF_IMGPLUG_Save_Image_Data;
        linkdata->IMGPLUG_Load_Image_Data = GUCEF_IMGPLUG_Load_Image_Data;
        linkdata->IMGPLUG_Unload_Image_Data = GUCEF_IMGPLUG_Unload_Image_Data;
        linkdata->IMGPLUG_Loadable_Ext_List = GUCEF_IMGPLUG_Loadable_Ext_List;
        linkdata->IMGPLUG_Saveable_Ext_List = GUCEF_IMGPLUG_Saveable_Ext_List;
        linkdata->IMGPLUG_Name = GUCEF_IMGPLUG_Name;
        linkdata->IMGPLUG_Description = GUCEF_IMGPLUG_Description;
        linkdata->IMGPLUG_Copyright = GUCEF_IMGPLUG_Copyright;
        linkdata->IMGPLUG_Version = GUCEF_IMGPLUG_Version;      
}

/*-------------------------------------------------------------------------*/

