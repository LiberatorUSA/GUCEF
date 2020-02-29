/*
 *  dstoreJSONPARSER: GUCEF plugin for data tree management
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>     /* memory management */
#include <stdio.h>      /* standard I/O utils */
#include <string.h>     /* standard string utils */

#ifndef _JSON_H
#include "json.h"   /* json-parser JSON parser library in ANSI C */
#define _JSON_H
#endif /* _JSON_H ? */

#ifndef _JSON_BUILDER_H
#include "json-builder.h"  /* json-builder JSON serialization library in ANSI C */
#define _JSON_BUILDER_H
#endif /* _JSON_BUILDER_H ? */

#include "DLLMainDSTOREpluginJSONPARSER.h"    /* gucefCORE DSTORE codec plugin API */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* gucefCORE macros, used here for the export and callspec macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define VERSION_MAJOR_FIELD     0
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
    json_value* jsonDoc;
    json_value* currentJsonNode;
};
typedef struct SDestFileData TDestFileData;

/*---------------------------------------------------------------------------*/

struct SSrcFileData
{
    TIOAccess* access;
    json_value* jsonDoc;
    void* privdata;
    TReadHandlers handlers;
};
typedef struct SSrcFileData TSrcFileData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Init( void** plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    /* json-parser does not require global initialization */
    *plugdata = NULL;
    return 1;
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Shutdown( void** plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    /* json-parser does not require global cleanup */
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Dest_File_Open( void** plugdata    ,
                           void** filedata    ,
                           TIOAccess* outFile ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    *filedata = GUCEF_NULL;
    if ( outFile != GUCEF_NULL )
    {
        TDestFileData* fd = (TDestFileData*) malloc( sizeof( TDestFileData ) );
        memset( fd, 0, sizeof( TDestFileData ) );
        fd->fptr = outFile;
        *filedata = fd;
        return 1;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Dest_File_Close( void** plugdata ,
                            void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    if ( filedata == GUCEF_NULL )
        return;
    if ( *filedata == GUCEF_NULL )
        return;

    TDestFileData* fd = (TDestFileData*)*filedata;
    if ( fd->jsonDoc != GUCEF_NULL )
    {
        /* 
         * Thus far we have only created the DOM in json_value format
         * This is where we actually do the serialization from the DOM
         * we constructed
         */
            
        size_t measured = json_measure( fd->jsonDoc );
        char* serializationBuffer = (char*) malloc( measured );
        json_serialize( serializationBuffer, fd->jsonDoc );
        json_builder_free( fd->jsonDoc );
        fd->jsonDoc = fd->currentJsonNode = GUCEF_NULL;
            
       /*   
        *   the json_measure and json_serialize functions include a null terminator
        *   When writing to a file type media it is not appropriote to include a null terminator
        */
       if ( measured > 0 )
            --measured;
        
        fd->fptr->write( fd->fptr, serializationBuffer, 1, measured );
        fd->fptr->close( fd->fptr );
    }

    free( *filedata );
    *filedata = GUCEF_NULL;
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Begin_Node_Store( void** plugdata      ,
                             void** filedata      ,
                             const char* nodename ,
                             Int32 nodeType       ,
                             UInt32 attscount     ,
                             UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TDestFileData* fd = (TDestFileData*)*filedata;
    if ( GUCEF_NULL == fd->currentJsonNode )
    {
        fd->jsonDoc = fd->currentJsonNode = json_object_new( attscount + haschildren );
    }
    else
    {
        json_value* childNode = GUCEF_NULL;
        if ( nodeType == GUCEF_DATATYPE_ARRAY )
        {
            childNode = json_array_new( attscount + haschildren );
        }
        else
        if ( nodeType == GUCEF_DATATYPE_OBJECT )
        {
            childNode = json_object_new( attscount + haschildren );
        }

        if ( GUCEF_NULL != childNode )
        {
            if ( GUCEF_NULL == nodename )
                nodename = "noname";
            if ( fd->currentJsonNode->type == json_object )
                json_object_push( fd->currentJsonNode, nodename, childNode );
            else
            if ( fd->currentJsonNode->type == json_array )
                json_array_push( fd->currentJsonNode, childNode );
            fd->currentJsonNode = childNode;
        }
    }
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_End_Node_Store( void** plugdata      ,
                           void** filedata      ,
                           const char* nodename ,
                           UInt32 attscount     ,
                           UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TDestFileData* fd = (TDestFileData*)*filedata;
    fd->currentJsonNode = fd->currentJsonNode->parent;
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
                           int atttype          ,
                           UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TDestFileData* fd = (TDestFileData*)*filedata;

    if ( GUCEF_NULL == attname || 0 == attscount ) return;

    if ( NULL == attvalue )
    {
        attvalue = "";
    }
    if ( NULL == nodename )
    {
        nodename = "noname";
    }

    switch ( atttype )
    {
        case GUCEF_DATATYPE_BOOLEAN_STRING:
        {
            int isNotTrue = _stricmp( attvalue, "true" );
            int isNotFalse = _stricmp( attvalue, "false" );
            
            json_value* att = GUCEF_NULL;
            if ( isNotTrue == 0 )
                att = json_boolean_new( 1 );
            else
            if ( isNotFalse == 0 )
                att = json_boolean_new( 0 );
            
            if ( att != GUCEF_NULL )
            {
                if ( fd->currentJsonNode->type == json_array )
                    json_array_push( fd->currentJsonNode, att );
                else
                if ( fd->currentJsonNode->type == json_object )
                    json_object_push( fd->currentJsonNode, attname, att );
            }
            break;
        }
        case GUCEF_DATATYPE_BOOLEAN_INT32:
        {
            int boolInt = 0;
            if ( 0 != sscanf( attvalue, "%i", &boolInt ) )
            {
                json_value* att = json_boolean_new( boolInt );
                if ( fd->currentJsonNode->type == json_array )
                    json_array_push( fd->currentJsonNode, att );
                else
                if ( fd->currentJsonNode->type == json_object )
                    json_object_push( fd->currentJsonNode, attname, att );
            }
            break;
        }
        case GUCEF_DATATYPE_FLOAT32:
        case GUCEF_DATATYPE_FLOAT64:
        case GUCEF_DATATYPE_NUMERIC:
        {
            double number = 0.0;
            if ( 0 != sscanf( attvalue, "%lf", &number ) )
            {
                json_value* att = json_double_new( number );
                if ( fd->currentJsonNode->type == json_array )
                    json_array_push( fd->currentJsonNode, att );
                else
                if ( fd->currentJsonNode->type == json_object )
                    json_object_push( fd->currentJsonNode, attname, att );
            }
            break;
        }
        case GUCEF_DATATYPE_INT8:
        case GUCEF_DATATYPE_UINT8:
        case GUCEF_DATATYPE_INT16:
        case GUCEF_DATATYPE_UINT16:
        case GUCEF_DATATYPE_INT32:
        case GUCEF_DATATYPE_UINT32:
        case GUCEF_DATATYPE_INT64:
        case GUCEF_DATATYPE_UINT64:
        {
            long long integer = 0;
            if ( 0 != sscanf( attvalue, "%lli", &integer ) )
            {
                json_value* att = json_integer_new( integer );
                if ( fd->currentJsonNode->type == json_array )
                    json_array_push( fd->currentJsonNode, att );
                else
                if ( fd->currentJsonNode->type == json_object )
                    json_object_push( fd->currentJsonNode, attname, att );
            }
            break;
        }
        case GUCEF_DATATYPE_STRING:
        default:
        {
            json_value* att = json_string_new( attvalue );
            if ( fd->currentJsonNode->type == json_array )
                json_array_push( fd->currentJsonNode, att );
            else
            if ( fd->currentJsonNode->type == json_object )
                json_object_push( fd->currentJsonNode, attname, att );
            break;
        }
    }

}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Begin_Node_Children( void** plugdata      ,
                                void** filedata      ,
                                const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{

}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_End_Node_Children( void** plugdata      ,
                              void** filedata      ,
                              const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{

}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Src_File_Open( void** plugdata      ,
                          void** filedata      ,
                          TIOAccess* file      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TSrcFileData* sd = GUCEF_NULL;
    *plugdata = GUCEF_NULL;

    sd = (TSrcFileData*) malloc( sizeof(TSrcFileData) );
    memset( sd, 0, sizeof( TSrcFileData ) );
    sd->access = file;
    return 0;
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Src_File_Close( void** plugdata ,
                           void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    if ( *filedata != GUCEF_NULL )
    {
        TSrcFileData* sd = (TSrcFileData*) *filedata;
        if ( GUCEF_NULL != sd->jsonDoc )
            json_value_free( sd->jsonDoc );
        sd->access->close( sd->access );
        free( sd );
    }
}

/*---------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Set_Read_Handlers( void** plugdata                ,
                              void** filedata                ,
                              const TReadHandlers* rhandlers ,
                              void* privdata                 ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    if ( *filedata != GUCEF_NULL )
    {
        TSrcFileData* sd = (TSrcFileData*) *filedata;
        sd->handlers = *rhandlers;
        sd->privdata = privdata;
    }
}

/*---------------------------------------------------------------------------*/

/* forward declaration for recursive functions below */
static void process_value( TSrcFileData* sd, const char* objName, const char* name, json_value* value );

/*---------------------------------------------------------------------------*/

static void process_object( TSrcFileData* sd, const char* name, json_value* value )
{
    int length=0, x=0, childNodes=0;
    if ( GUCEF_NULL == value )
        return;

    sd->handlers.OnNodeBegin( sd->privdata, name, GUCEF_DATATYPE_OBJECT );
    length = value->u.object.length;
    for ( x=0; x<length; x++ ) 
    {
        if ( value->type != json_object && value->type != json_array )
            process_value( sd, name, value->u.object.values[x].name, value->u.object.values[x].value );
        else
            childNodes++;
    }
    if ( childNodes > 0 )
    {
        sd->handlers.OnNodeChildrenBegin( sd->privdata, name );
        for ( x=0; x<length; x++ ) 
        {
            if ( value->type == json_object || value->type == json_array )
                process_value( sd, name, value->u.object.values[x].name, value->u.object.values[x].value );
        }
        sd->handlers.OnNodeChildrenEnd( sd->privdata, name );
    }
    sd->handlers.OnNodeEnd( sd->privdata, name );
}

/*---------------------------------------------------------------------------*/

static void 
process_array( TSrcFileData* sd, const char* name, json_value* value )
{
    int length=0, x=0;
    char indexAsName[64];
            
    if ( GUCEF_NULL == value )
        return;
    
    sd->handlers.OnNodeBegin( sd->privdata, name, GUCEF_DATATYPE_ARRAY );
    sd->handlers.OnNodeChildrenBegin( sd->privdata, name );
    length = value->u.array.length;
    for ( x=0; x<length; x++ ) 
    {
        sprintf( indexAsName, "%i", x );
        sd->handlers.OnNodeBegin( sd->privdata, indexAsName, value->u.array.values[x]->type );
        process_value( sd, name, GUCEF_NULL, value->u.array.values[x] );
        sd->handlers.OnNodeEnd( sd->privdata, indexAsName );
    }
    sd->handlers.OnNodeChildrenEnd( sd->privdata, name );
    sd->handlers.OnNodeEnd( sd->privdata, name );
}

/*---------------------------------------------------------------------------*/

static void 
process_value( TSrcFileData* sd    , 
               const char* objName , 
               const char* name    , 
               json_value* value   )
{
    if ( GUCEF_NULL == value ) 
        return;

    switch ( value->type ) 
    {
        case json_none:
        {
            break;
        }
        case json_object:
        {
            process_object( sd, name, value );
            break;
        }
        case json_array:
        {
            process_array( sd, name, value );
            break;
        }
        case json_integer:
        { 
            char valueBuffer[64];
            sprintf( valueBuffer, "%lli", value->u.integer );

            sd->handlers.OnNodeAtt( sd->privdata, objName, name, valueBuffer, GUCEF_DATATYPE_INT64 );
            break;
        }
        case json_double:
        { 
            char valueBuffer[64];
            sprintf( valueBuffer, "%f", value->u.dbl );

            sd->handlers.OnNodeAtt( sd->privdata, objName, name, valueBuffer, GUCEF_DATATYPE_FLOAT64 );
            break;
        }
        case json_string:
        { 
            sd->handlers.OnNodeAtt( sd->privdata, objName, name, value->u.string.ptr, GUCEF_DATATYPE_STRING );
            break;
        }
        case json_boolean:
        { 
            char* boolValue = "true";
            if ( 0 == value->u.boolean )
                boolValue = "false";

            sd->handlers.OnNodeAtt( sd->privdata, objName, name, boolValue, GUCEF_DATATYPE_BOOLEAN_STRING );
            break;
        }
    }
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Start_Reading( void** plugdata ,
                          void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    if ( GUCEF_NULL != *filedata )
    {
        TSrcFileData* sd = (TSrcFileData*) *filedata;
        if ( GUCEF_NULL != sd )
        {
            UInt32 fileSize = 0, bytesRead = 0;
            json_char* fileBuffer = GUCEF_NULL;
            json_value* jsonDoc = GUCEF_NULL;

            sd->access->seek( sd->access, 0, SEEK_END );
            fileSize = sd->access->tell( sd->access );   
            sd->access->seek( sd->access, 0, SEEK_SET );

            fileBuffer = (json_char*) malloc( fileSize );
            bytesRead = sd->access->read( sd->access, fileBuffer, 1, fileSize );
            if ( bytesRead != fileSize )
            {
                free( fileBuffer );
                return 1;
            }

            jsonDoc = json_parse( fileBuffer, bytesRead );               
            free( fileBuffer );
            
            free( sd->jsonDoc );
            sd->jsonDoc = jsonDoc;
            
            /* Now we actually commence the reading itself 
               We treat the DOM tree in a SAX manner essentially */
            (*sd->handlers.OnTreeBegin)( sd->privdata );
            process_value( sd, "", "", jsonDoc );    
            (*sd->handlers.OnTreeEnd)( sd->privdata );    
        }
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Type( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "json";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Name( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "gucefCORE DSTORE codec plugin utilizing json-parser for parsing and json-builder for serializing";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Copyright( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "Copyright (C) Dinand Vanvelzen. 2002 - 2005. LGPLv3.";
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
