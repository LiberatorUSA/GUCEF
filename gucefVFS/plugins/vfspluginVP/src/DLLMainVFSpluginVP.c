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

#include <stdlib.h>
#include <string.h>

#include "DLLMainVFSpluginVP.h"    /* gucefVFS plugin API */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define VERSION_MAYOR_FIELD     1
#define VERSION_MINOR_FIELD     0
#define VERSION_PATCH_FIELD     0
#define VERSION_RELEASE_FIELD   0

#define VP_HEADER_SIZE          16
#define VP_INDEX_ENTRY_SIZE     44

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SVPHeader
{
        char sig[ 4 ];      /* should always be "VPVP" */
        Int32 version;      /* should be 2 */
        UInt32 indexoffset; /* index offset from the start of the file in bytes */
        UInt32 idxentries;  /* number of index entries */
};
typedef struct SVPHeader TVPHeader;

/*---------------------------------------------------------------------------*/

struct SVPIndexEntry
{
        UInt32 offset;
        UInt32 size;
        char filename[ 32 ];
        Int32 timestamp;        
};
typedef struct SVPIndexEntry TVPIndexEntry;

/*---------------------------------------------------------------------------*/

struct SVPFileData
{
        TVPHeader header;
        TVPIndexEntry* entries;
};
typedef struct SVPFileData TVPFileData;

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
        TVPHeader header;
        packfile->open( packfile );
        if ( packfile->read( packfile, &header, VP_HEADER_SIZE, 1 ) == VP_HEADER_SIZE )
        {
                if ( ( memcmp( header.sig, "VPVP", 4 ) == 0 ) &&
                     ( header.version == 2 ) )
                {
                        /*
                         *      Allocate a file data structure and copy the header
                         *      data into it.
                         */
                        TVPFileData* fd = (TVPFileData*) malloc( sizeof( TVPFileData ) );
                        *packdata = fd;
                        fd->header = header;
                        
                        /*
                         *      Read the index
                         */
                        fd->entries = (TVPIndexEntry*) malloc( header.idxentries * VP_INDEX_ENTRY_SIZE );
                        if ( packfile->read( packfile, fd->entries, VP_INDEX_ENTRY_SIZE, header.idxentries ) == VP_INDEX_ENTRY_SIZE*header.idxentries )
                        {                                
                                return 1;
                        }
                        free( fd->entries );
                        free( fd );
                        *packdata = NULL;                        
                        return 0;                         
                }     
        }
        return 0;               
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Pack_Close( void** plugdata     ,
                    TIOAccess* packfile ,
                    void** packdata     ) PLUGIN_CALLSPEC_SUFFIX
{
        free( ( (TVPFileData*) *packdata )->entries );
        free( ( (TVPFileData*) *packdata ) );
        *packdata = NULL;
        
        packfile->close( packfile );
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
        TVPFileData* pd = (TVPFileData*) (*packdata);
        UInt32 i;
        for ( i=0; i<pd->header.idxentries; ++i )
        {
                if ( strcmp( pd->entries[ i ].filename, filename ) == 0 )
                {
                        if ( pd->entries[ i ].size )
                        {
                                
                        }        
                }
        }
        
        
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
        TVPFileData* pd = (TVPFileData*) (*packdata); 
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
        return NULL;//( (TVPFileData*) (*packdata) )->filelist;        
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
        return "Volition pack (Decent/Freespace) file reader";
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