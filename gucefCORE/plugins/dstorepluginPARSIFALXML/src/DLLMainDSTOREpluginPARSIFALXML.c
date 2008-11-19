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

#include <stdlib.h>     /* memory management */
#include <stdio.h>      /* standard I/O utils */
#include <string.h>     /* standard string utils */

#ifndef PARSIFAL__H
#include "parsifal.h"   /* Parsifal XML Parser API */
#define PARSIFAL__H
#endif /* PARSIFAL__H ? */

/* Parsifal defines BYTE in the global namesapce which can cause problems :( */
#undef BYTE

#include "DLLMainDSTOREpluginPARSIFALXML.h"    /* gucefCORE DSTORE codec plugin API */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* gucefCORE macros, used here for the export and callspec macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define NODE_STORE_INDENT       1
#define INDENT_CHAR             9  /* horizontal tab */

#define VERSION_MAJOR_FIELD     1
#define VERSION_MINOR_FIELD     1
#define VERSION_PATCH_FIELD     0
#define VERSION_RELEASE_FIELD   0

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SDestFileData
{
        TIOAccess* fptr;
        Int32 indent;
        char* line;
        UInt32 linelen;
};

typedef struct SDestFileData TDestFileData;

/*---------------------------------------------------------------------------*/

struct SSrcFileData
{
        TIOAccess* access;
        LPXMLPARSER parser;
        TReadHandlers handlers;
        void* privdata; 
};

typedef struct SSrcFileData TSrcFileData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static int
ParsifalReader( BYTE* buf         , 
                int cbytes        , 
                int *cbytesactual , 
                void* inputdata   )
{
        *cbytesactual = (int) ((TIOAccess*)inputdata)->read( ((TIOAccess*)inputdata), buf, 1, cbytes );
        return (*cbytesactual < cbytes);
}

/*---------------------------------------------------------------------------*/

static void
ParsifalError( struct tagXMLPARSER *parser )
{
        TSrcFileData* sd = (TSrcFileData*) parser->UserData;
        (*sd->handlers.OnError)( sd->privdata, parser->ErrorCode, parser->ErrorString );
}

/*---------------------------------------------------------------------------*/

static int
ParsifalDocumentStart( void* userdata )
{
        TSrcFileData* sd = (TSrcFileData*) userdata;
        (*sd->handlers.OnTreeBegin)( sd->privdata );
        return XML_OK;        
}

/*---------------------------------------------------------------------------*/

static int
ParsifalDocumentEnd( void* userdata )
{
        TSrcFileData* sd = (TSrcFileData*) userdata;
        (*sd->handlers.OnTreeEnd)( sd->privdata );
        return XML_OK;        
}

/*---------------------------------------------------------------------------*/

static int
ParsifalElementStart( void* userdata         , 
                      const XMLCH *uri       , 
                      const XMLCH *localname , 
                      const XMLCH *qname     ,
                      LPXMLVECTOR atts       )
{
        Int32 i;
        LPXMLRUNTIMEATT att;
        TSrcFileData* sd = (TSrcFileData*) userdata;
        (*sd->handlers.OnNodeBegin)( sd->privdata, qname );
        if ( atts->length )
        {                
                for ( i=0; i<atts->length; ++i )
                {
                        att = (LPXMLRUNTIMEATT) XMLVector_Get( atts, i );
                        (*sd->handlers.OnNodeAtt)( sd->privdata , 
                                                   qname        ,
                                                   att->qname   , 
                                                   att->value   );
                }
        }                
        return XML_OK;               
}

/*---------------------------------------------------------------------------*/

static int
ParsifalElementEnd( void* userdata         , 
                    const XMLCH *uri       ,
                    const XMLCH *localname , 
                    const XMLCH *qname     )
{
        TSrcFileData* sd = (TSrcFileData*) userdata;
        (*sd->handlers.OnNodeEnd)( sd->privdata, qname );
        return XML_OK;
}                    

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Init( void** plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        /* parsifal does not require global initialization */
        *plugdata = NULL;
        return 1;
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Shutdown( void** plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        /* parsifal does not require global cleanup */
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Dest_File_Open( void** plugdata    , 
                           void** filedata    ,
                           TIOAccess* outFile ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{        
        *filedata = NULL;
        if ( outFile != NULL )
        {
                char outBuffer[ 250 ];
                TDestFileData* fd = (TDestFileData*) malloc( sizeof( TDestFileData ) );
                fd->fptr = outFile;
                fd->indent = 0;
                fd->line = NULL;
                fd->linelen = 0;
                *filedata = fd;
                
                sprintf( outBuffer, "<?xml version=\"1.0\"?>\r\n" );
                outFile->write( outFile, outBuffer, (UInt32)strlen( outBuffer ), 1 );
                sprintf( outBuffer, "<!-- Generated using dstorepluginXMLPARSIFAL (v%d.%d.%d.%d) for the GUCEF platform - Copyright (C) Dinand Vanvelzen. All rights reserved -->\r\n", VERSION_MAJOR_FIELD, VERSION_MINOR_FIELD, VERSION_PATCH_FIELD, VERSION_RELEASE_FIELD );
                outFile->write( outFile, outBuffer, (UInt32)strlen( outBuffer ), 1 );
                return 1;
        }
        return 0;
}                           
                           
/*---------------------------------------------------------------------------*/
                           
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Dest_File_Close( void** plugdata , 
                            void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        if ( filedata )
        { 
                if ( *filedata )
                {       
                        TDestFileData* fd = (TDestFileData*)*filedata;
                        fd->fptr->close( fd->fptr );
                        if ( fd->line && fd->linelen ) free( fd->line );
                        free( fd );
                }
        }                                
}                            
                            
/*---------------------------------------------------------------------------*/
                            
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Begin_Node_Store( void** plugdata      , 
                             void** filedata      , 
                             const char* nodename , 
                             UInt32 attscount     ,
                             UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TDestFileData* fd = (TDestFileData*)*filedata;        
        UInt32 max, len;
        
        if ( !fd ) return;

        if ( !nodename )
        {
                nodename = "noname";
        }
                
        len = (UInt32)strlen( nodename );
        max = len + 5*attscount + 6 + fd->indent;
        if ( fd->linelen < max )
        {
                fd->line = realloc( fd->line, max );
                *(fd->line) = 0;
                fd->linelen = max;
        }                
        memset( fd->line, INDENT_CHAR, fd->indent );
        if ( attscount > 0 )
        {
                sprintf( fd->line+fd->indent, "<%s ", nodename );
                return;
        }
        haschildren ? sprintf( fd->line+fd->indent, "<%s>\r\n", nodename ) : sprintf( fd->line+fd->indent, "<%s/>\r\n", nodename );
        fd->fptr->write( fd->fptr, fd->line, (UInt32)strlen( fd->line ), 1 );       
}                             

/*---------------------------------------------------------------------------*/
                             
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_End_Node_Store( void** plugdata      , 
                           void** filedata      , 
                           const char* nodename ,
                           UInt32 attscount     ,
                           UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        if ( !nodename )
        {
                nodename = "noname";
        }
        if ( haschildren )
        {
                TDestFileData* fd = (TDestFileData*)*filedata;
                memset( fd->line, INDENT_CHAR, fd->indent );
                sprintf( fd->line+fd->indent, "</%s>\r\n", nodename );
                fd->fptr->write( fd->fptr, fd->line, (UInt32)strlen( fd->line ), 1 );                
        }
}                           
                           
/*---------------------------------------------------------------------------*/
                           
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Store_Node_Att( void** plugdata      , 
                           void** filedata      , 
                           const char* nodename ,
                           UInt32 attscount     , 
                           UInt32 attindex      , 
                           const char* attname  , 
                           const char* attvalue ,
                           UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX  
{
        TDestFileData* fd = (TDestFileData*)*filedata;
        UInt32 linelen, len;
        
        if ( !attname || !attvalue || !attscount ) return;
        
        if ( !nodename )
        {
                nodename = "noname";
        }
        
        linelen = (UInt32)strlen( fd->line );
        len = (UInt32)strlen( attname )+strlen( attvalue )+2;
        if ( fd->linelen < linelen+10+len )
        {
                fd->line = realloc( fd->line, linelen+11+len );
        }
        if ( attindex+1 < attscount )
        {
                sprintf( fd->line+linelen, "%s=\"%s\" ", attname, attvalue );
                return;                
        }
        if ( haschildren ) 
        {
                sprintf( fd->line+linelen, "%s=\"%s\" >\r\n", attname, attvalue ); 
        }
        else
        {
                sprintf( fd->line+linelen, "%s=\"%s\" />\r\n", attname, attvalue );
        }                
        fd->fptr->write( fd->fptr, fd->line, (UInt32)strlen( fd->line ), 1 );
}                           
                           
/*---------------------------------------------------------------------------*/
                           
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Begin_Node_Children( void** plugdata      , 
                                void** filedata      , 
                                const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TDestFileData* fd = (TDestFileData*)*filedata;        
        fd->indent += NODE_STORE_INDENT;
}                                 
                                
/*---------------------------------------------------------------------------*/
                                
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_End_Node_Children( void** plugdata      , 
                              void** filedata      , 
                              const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TDestFileData* fd = (TDestFileData*)*filedata;       
        fd->indent -= NODE_STORE_INDENT;
}                              
                                  
/*---------------------------------------------------------------------------*/                                  
                                  
UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Src_File_Open( void** plugdata      , 
                          void** filedata      ,
                          TIOAccess* file      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{        
        TSrcFileData* sd;
        *plugdata = NULL;        
                
        sd = (TSrcFileData*) malloc( sizeof(TSrcFileData) );
        sd->access = file;        
         
        if ( XMLParser_Create( &sd->parser ) )
        {
                sd->privdata = NULL;
                memset( &sd->handlers, 0, sizeof(TReadHandlers) );
        
                sd->parser->startDocumentHandler = &ParsifalDocumentStart;
                sd->parser->endDocumentHandler = &ParsifalDocumentEnd;
                sd->parser->startElementHandler = &ParsifalElementStart;
                sd->parser->endElementHandler = &ParsifalElementEnd;
                sd->parser->errorHandler = &ParsifalError;
                sd->parser->UserData = sd;
                *filedata = sd;                               
                return 1;                
        }        
        free( sd );
        return 0;
}                          
                          
/*---------------------------------------------------------------------------*/
                          
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Src_File_Close( void** plugdata , 
                           void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        if ( *filedata )
        {
                TSrcFileData* sd = (TSrcFileData*) *filedata;                
                XMLParser_Free( sd->parser );
                sd->access->close( sd->access );
        }
}                           
                           
/*---------------------------------------------------------------------------*/
                           
void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Set_Read_Handlers( void** plugdata                , 
                              void** filedata                , 
                              const TReadHandlers* rhandlers , 
                              void* privdata                 ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        if ( *filedata )
        {
                TSrcFileData* sd = (TSrcFileData*) *filedata;
                sd->handlers = *rhandlers;
                sd->privdata = privdata;        
        }
}                              
                              
/*---------------------------------------------------------------------------*/
                             
UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Start_Reading( void** plugdata , 
                          void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        if ( *filedata )
        {
                TSrcFileData* sd = (TSrcFileData*) *filedata;
                if ( sd )
                {
                        XMLParser_Parse( sd->parser      ,
                                         &ParsifalReader ,
                                         sd->access      ,
                                         NULL            );
                }                                         
        }
        return 0;
}                          
                          
/*---------------------------------------------------------------------------*/                           
 
const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Type( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        return "xml";
}                                       
                  
/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Name( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        static char buffer[ 128 ];
        sprintf( buffer, "gucefCORE DSTORE codec plugin utilizing the Parsifal XML parser %s", XMLParser_GetVersionString() );
        return buffer;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Copyright( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        return "Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.";
}

/*---------------------------------------------------------------------------*/

const TVersion* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Version( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        static TVersion version;
        version.major = VERSION_MAJOR_FIELD;
        version.minor = VERSION_MINOR_FIELD;
        version.patch = VERSION_PATCH_FIELD;
        version.release = VERSION_RELEASE_FIELD;
        
        return &version;
}

/*---------------------------------------------------------------------------*/ 