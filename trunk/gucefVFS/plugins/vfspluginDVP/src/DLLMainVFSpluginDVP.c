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

#include "DLLMainVFSpluginDVP.h"    /* gucefVFS plugin API */

#ifndef DVPACKSYS_H     
#include "DVPACKSYS.h"  /* package handling code */
#define DVPACKSYS_H
#endif /* DVPACKSYS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define VERSION_MAYOR_FIELD     1
#define VERSION_MINOR_FIELD     0
#define VERSION_PATCH_FIELD     0
#define VERSION_RELEASE_FIELD   0

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SPackData
{
        TDVP_File_Header header;
        TDVP_Index_Entry* entries;
        char** filelist;
};

typedef struct SPackData TPackData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
 
UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Pack_Open( void** plugdata     ,
                   TIOAccess* packfile ,
                   void** packdata     ) PLUGIN_CALLSPEC_SUFFIX
{
        TPackData* pdata; 
        UInt32 offset, i, count;
                
        /*
         *      Allocate our package info storage 
         */
        *packdata = malloc( sizeof( TPackData ) );                
        pdata = (TPackData*) *packdata;
        
        /*
         *      Read the package header
         */
        _DVP_IOA_Read_Header( packfile       ,
                              &pdata->header );
                                
        /* 
         *      Read all index entries
         */       
        offset = pdata->header.index_offset;
        count = pdata->header.num_dirs + pdata->header.num_files;
        pdata->entries = malloc( sizeof(TDVP_Index_Entry) * count ); 
        for ( i=0; i<count; ++i )
        {
                _DVP_IOA_Read_Index_Entry( offset               ,
                                           &pdata->entries[ i ] ,
                                           packfile             );       
        }
        
        /*
         *      Hook up the filelist 
         */
        pdata->filelist = (char**) malloc( (count+1)*sizeof(char*) );
        for ( i=0; i<count; ++i )
        {
                pdata->filelist[ i ] = pdata->entries[ i ].name;
        }
        pdata->filelist[ count ] = NULL; 
        
        return 0;               
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Pack_Close( void** plugdata     ,
                    TIOAccess* packfile ,
                    void** packdata     ) PLUGIN_CALLSPEC_SUFFIX
{
        free( ( (TPackData*) *packdata )->entries );
        free( ( (TPackData*) *packdata )->filelist );
        *packdata = NULL;
        
        return 0;
}

/*---------------------------------------------------------------------------*/ 
 
UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Init( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        *plugdata = NULL;
        return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Shutdown( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        return 0;
}

/*---------------------------------------------------------------------------*/

TIOAccess* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Load( void** plugdata      ,
              TIOAccess* packfile  , 
              void** packdata      ,
              const char* filename ,                             
              UInt32* errorcode    ) PLUGIN_CALLSPEC_SUFFIX
{
        TPackData* pd = (TPackData*) (*packdata);
        *errorcode = 0;                      
        return NULL;
}              
              
/*---------------------------------------------------------------------------*/              
              
TIOAccess* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Load_Writeable( void** plugdata      ,
                        TIOAccess* packfile  ,
                        void** packdata      ,
                        const char* filename ,                                                
                        UInt32* errorcode    ) PLUGIN_CALLSPEC_SUFFIX
{                                            
        TPackData* pd = (TPackData*) (*packdata); 
        *errorcode = 0;
        return NULL;        
}                        
                        
/*---------------------------------------------------------------------------*/

void PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Unload( void** plugdata      ,
                TIOAccess* packfile  ,
                void** packdata      ,
                TIOAccess* file      , 
                const char* filename ,
                UInt32* errorcode    ) PLUGIN_CALLSPEC_SUFFIX
{                        
        TPackData* pd = (TPackData*) (*packdata); 
        *errorcode = 0;
}                
                
/*---------------------------------------------------------------------------*/

const char** PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Filelist( void** plugdata     ,
                  TIOAccess* packfile ,                  
                  void** packdata     ,
                  UInt32* errorcode   ) PLUGIN_CALLSPEC_SUFFIX
{          
        *errorcode = 0;
        return ( (TPackData*) (*packdata) )->filelist;        
}

/*---------------------------------------------------------------------------*/

void PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Free_Filelist( void** plugdata     ,  
                       TIOAccess* packfile , 
                       void** packdata     , 
                       char** filelist     , 
                       UInt32* errorcode   ) PLUGIN_CALLSPEC_SUFFIX
{
        *errorcode = 0;        
}                                        
                  
/*---------------------------------------------------------------------------*/

const char* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Name( void** _plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        return "gucefVFS plugin utilizing DVPACKSYS";
}

/*---------------------------------------------------------------------------*/

const char* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Copyright_EULA( void** _plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        return "Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.";
}

/*---------------------------------------------------------------------------*/

const TVersion* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Version( void** _plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        static TVersion version;
        version.mayor = VERSION_MAYOR_FIELD;
        version.minor = VERSION_MINOR_FIELD;
        version.patch = VERSION_PATCH_FIELD;
        version.release = VERSION_RELEASE_FIELD;
        
        return &version;
}

/*---------------------------------------------------------------------------*/ 