/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2003.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD SWITCHES                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Disable this to remove command line argument support
 */
#define SUPPORT_ARGS    1

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>     // standard utils
#include <stdio.h>      // standard I/O
#include <conio.h>      // console i/o

#include "DVPACKSYS.h"  // Pack sys utils
#include "dvstrutils.h" // My own string tools

/*-------------------------------------------------------------------------//
//                                                                         //
//      DEFINES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#define VERSION_NUMBER                          1
#define MIN_ARCHIVE_NAME_LENGTH                 1

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/

void
Create_Archive_Wizard( void )
{
        char *rbuffer  = calloc( 1024, sizeof( char ) )                 ;
        char *archive  = NULL                                           ;
        char *dir_root = NULL                                           ;
        char *ID       = NULL                                           ;
        int key        = 0                                              ;

        /*
         *      Obtain required data from user
         */

        /*
         *      Get a archive name from user
         */
        rbuffer[ 0 ] = 0                                                ; 
        while ( strlen( rbuffer ) < MIN_ARCHIVE_NAME_LENGTH )
        {
                printf( "\nPlease enter a name for the archive :\n" )   ;
                scanf( "%s", rbuffer )                                  ;
                if ( strlen( rbuffer ) < MIN_ARCHIVE_NAME_LENGTH )
                {
                        printf( "minimum length for name is %i, please enter a new one\n", MIN_ARCHIVE_NAME_LENGTH );
                }

        }
        archive = calloc( strlen( rbuffer ), sizeof( char ) )           ;
        strcpy( archive, rbuffer )                                      ;
        rbuffer[ 0 ] = 0                                                ;

        /*
         *      Get a starting dir
         */
        printf( "\nPlease provide the path to the starting dir :\n" )   ;
        scanf( "%s", rbuffer )                                          ;
        dir_root = calloc( strlen( rbuffer ), sizeof( char ) )          ;
        strcpy( dir_root, rbuffer )                                     ;

        /*
         *      Get an archive ID from user
         */
        printf( "\nPlease enter a ID for the archive :\n" )             ;
        scanf( "%s", rbuffer )                                          ;
        ID = calloc( strlen( rbuffer ), sizeof( char ) )                ;
        strcpy( ID, rbuffer )                                           ;

        /*
         *      Get a key value from user
         */
        printf( "\nPlease enter a key to be used with archive\n" )      ;
        printf( "Enter 0 for no key protection\n" )                     ;
        scanf( "%i", &key )                                             ;
        if ( key == 0 )
        {
                printf( "The archive will not be key protected\n" )     ;
        }
        else
        {
                printf( "The archive will be key protected\n" )         ;
                printf( "The key value = %i\n", key )                   ;
        }

        /*
         *      free unneeded input buffer
         */
        free( rbuffer )                                                 ; 

        /*
         *      Do the actual creation of the archive
         */
        printf( "Creating Archive...\n" )                               ; 
        if ( !DVP_Create_Archive( archive, dir_root, key, ID ) )
        {
                printf( "Process completed\n" )                         ;
        }
        else
        {
                printf( "Error during process\n" )                      ;
        }

        /*
         *      Wait for exit
         */
        printf( "\nPress any key to exit\n" )                           ;
        getch()                                                         ;
}

/*---------------------------------------------------------------------------*/

void
Display_Header_Wizard( void )
{
        char filename[ 1024 ]   ;
        char ID[ DVP_ID_LENGTH+1 ];
        long version             ;
        long index_offset       ;
        long num_files           ;
        long num_dirs            ;
        int key                 ;
        long lastchange         ;
        FILE *fptr = NULL       ;

        /*
         *      Display header data to console of specified archive
         */
        printf( "\nPlease enter path and name of archive\n" )           ;
        scanf( "%s", filename )                                         ;

        /*
         *      Obtain key
         */
        printf( "\nPlease enter archive key to use\n" )                 ;
        scanf( "%i", &key )                                             ;

        fptr = fopen( filename, "rb" )                                  ;
        if ( fptr )
        {
                /*
                 *      Do the actual obtaining of header data
                 */
                ID[ DVP_ID_LENGTH ] = 0                                 ;
                if ( !DVP_Read_Header( fptr          ,
                                       ID            ,
                                       &version      ,
                                       &index_offset ,
                                       &num_files    ,
                                       &num_dirs     ,
                                       &lastchange   ,
                                       key           ) )
                {
                        printf( "\nHeader data obtained...\n\n" )       ;
                        printf( "ID          = %s\n", ID )              ;
                        printf( "version     = %i\n", version )         ;
                        printf( "nr of files = %i\n", num_files )       ;
                        printf( "nr of dirs  = %i\n", num_dirs )        ;
                        printf( "last change = %i\n", lastchange )      ;

                        /*
                         *      Wait for exit
                         */
                        printf( "\nPress any key to exit\n" )           ;
                        getch()                                         ;
                }
                else
                {
                        /*
                         *      An error occured.. wrong checksum
                         */
                        printf( "\nError obtaing header data\n" )       ;
                        printf( "The file may be invalid or corrupt\n") ;

                        /*
                         *      Wait for exit
                         */
                        printf( "\nPress any key to exit\n" )           ;
                        getch()                                         ;
                }
        }
        else
        {
                /*
                 *      Cannot open file
                 */
                printf( "Invalid path/filename\n" )                     ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
        }
        fclose( fptr )                                                  ;
        return                                                          ;
}

//-----------------------------------------------------------------------------

void
Display_Index_Wizard( void )
{
        char *rbuffer = ( char* ) calloc( 1024, sizeof( char ) )        ;
        char *ext = NULL        ;
        char *archive = NULL    ;
        int key = 0             ;
        long tmp                 ;
        long files = 0           ;
        long dirs  = 0           ;
        long lastchange         ;
        long offset             ;
        long pos = 0            ;
        FILE *fptr = NULL       ;

        /*
         *      Output archive index to console
         */
        printf( "\nPlease enter the archive's path + name :\n" )        ;
        scanf( "%s", rbuffer )                                          ;
        archive = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) ) ;
        strcpy( archive, rbuffer )                                      ;

        /*
         *      Attempt to open file
         */
        fptr = fopen( archive, "rb" )                                   ;
        if ( !fptr )
        {
                printf("\nError opening file\n" )                       ;
                return                                                  ;
        }

        printf( "\nPlease enter the archive key :\n" )                  ;
        scanf( "%i", &key )                                             ;

        /*
         *      Check file extension
         */
        ext = Extract_File_Ext( archive )                               ;

        /*
         *      Check for index file
         */
        if ( !strcmp( ext, DVP_IDX_EXT ) )
        {
                /*
                 *      Index file so we don't use a header
                 */
                fseek( fptr , 0 , SEEK_END )                            ;
                fflush( fptr )                                          ;
                pos = ftell( fptr )                                     ;
                pos /= DVP_INDEX_ENTRY_SIZE                             ;
                files = pos                                             ;
                offset = 0                                              ;
        }
        else
        {
                /*
                 *      Get index offset and entry count
                 */
                if ( DVP_Read_Header( fptr               ,
	                              rbuffer  	         ,
        	                      &tmp 	         ,
        	                      &offset            ,
                	              &files             ,
	                              &dirs	         ,
                                      &lastchange        ,
        	                      key		 ) )
                {
                        printf( "\nError obtaing header data\n" )       ;
                        printf( "The file may be invalid or corrupt\n") ;

                        /*
                         *      Wait for exit
                         */
                        printf( "\nPress any key to exit\n" )           ;
                        getch()                                         ;
                        return                                          ;
                }
        }
        free( ext )                                                     ;

        /*
         *      Free unneeded buffer
         */
        free( rbuffer )                                                 ; 

        /*
         *      Output index to console
         */
        printf( "\n------------------------------------\n" )            ;
        printf( "-      ARCHIVE CONTENTS            -\n" )              ;
        printf( "------------------------------------\n" )              ;
        if ( DVP_Display_Index( fptr          ,
                                offset        ,
                                files+(dirs*2) ) )
        {
                printf( "\nError while reading index\n" )               ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                return                                                  ;
        }

        printf( "\n...Process completed\n" )                            ;
        fclose( fptr )                                                  ;

        /*
         *      Wait for exit
         */
        printf( "\nPress any key to exit\n" )                           ;
        getch()                                                         ;
        return                                                          ;
}

//-----------------------------------------------------------------------------

void
Extract_Index_File_Wizard( void )
{
        /*
         *      Extract the index file out of an archive and save as seperate
         *      file.
         */

        int key = 0                                                     ;
        long num_files = 0                                               ;
        long num_dirs = 0                                                ;       
        long version                                                     ;
        long index_offset                                               ;
        long lastchange                                                 ;
        char *rbuffer = calloc( 1024, sizeof( char ) )                  ;
        char *archive = NULL                                            ;
        char *target = NULL                                             ;
        FILE *fptr = NULL                                               ;      

        /*
         *      Get needed data from user
         */
        printf( "\nPlease enter archive path + filename :\n" )          ;
        scanf( "%s", rbuffer )                                          ;
        archive = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) ) ;
        strcpy( archive, rbuffer )                                      ;
        printf( "\nPlease enter path to extract to :\n" )               ;
        scanf( "%s", rbuffer )                                          ;
        target = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) )  ;
        strcpy( target, rbuffer )                                       ;
        free( rbuffer )                                                 ;
        printf( "\nPlease enter key for archive :\n" )                  ;
        scanf( "%i", &key )                                             ;

        /*
         *      Attempt to open archive
         */
        fptr = fopen( archive, "rb" )                                   ;
        if ( !fptr )
        {
                printf( "\nError opening archive\n" )                   ;
                free( target )                                          ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                free( archive )                                         ;
                return                                                  ;
        }
        free ( archive )                                                ;

        /*
         *      Try to obtain header offset
         */
        rbuffer[ DVP_ID_LENGTH ] = 0                                    ;
        if ( DVP_Read_Header( fptr           ,
                               rbuffer       ,
                               &version      ,
                               &index_offset ,
                               &num_files    ,
                               &num_dirs     ,
                               &lastchange   ,
                               key           ) )
        {
                /*
                 *      An error occured.. wrong checksum ?
                 */
                printf( "\nError obtaing header data\n" )               ;
                printf( "The file may be invalid or corrupt\n")         ;

                /*
                 *      Wait for exit
                 */
                 printf( "\nPress any key to exit\n" )                  ;
                 getch()                                                ;
        }
        free( rbuffer )                                                 ;


        if ( DVP_Extract_Index_File( fptr                  ,
                                     target                ,
                                     index_offset          ,
                                     num_files+(num_dirs*2) ) )
        {
                /*
                 *      An error occured
                 */
                printf( "\nError extracting index file\n" )             ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                return                                                  ;
        }

        printf( "\n... process completed\n" )                           ;

        /*
         *      Wait for exit
         */
        printf( "\nPress any key to exit\n" )                           ;
        getch()                                                         ;
        return                                                          ;
}

//-----------------------------------------------------------------------------

void
Extract_All_Files_Wizard( void )
{
        char *rbuffer = calloc( 1024, sizeof( char ) )                  ;
        char *archive = NULL                                            ;
        char *target = NULL                                             ;
        int key = 0                                                     ;
        FILE *fptr = NULL                                               ;

        /*
         *      Get needed data from user
         */
        printf( "\nPlease enter archive path + filename :\n" )          ;
        scanf( "%s", rbuffer )                                          ;
        archive = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) ) ;
        strcpy( archive, rbuffer )                                      ;
        printf( "\nPlease enter path to extract to :\n" )               ;
        scanf( "%s", rbuffer )                                          ;
        target = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) )  ;
        strcpy( target, rbuffer )                                       ;
        free( rbuffer )                                                 ;
        printf( "\nPlease enter key for archive :\n" )                  ;
        scanf( "%i", &key )                                             ;

        /*
         *      Attempt to open archive
         */
        free( archive )                                                 ;
        fptr = fopen( archive, "rb" )                                   ;
        if ( !fptr )
        {
                printf( "\nError opening archive\n" )                   ;
                free( target )                                          ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                return                                                  ;
        }

        /*
         *      Do the actual extracting
         */
        if ( DVP_Extract_All_Files( fptr, target, key ) )
        {
                printf( "There has been an error\n" )                   ;
        }
        else
        {
                printf( "...process completed\n" )                      ;
        }

        /*
         *      Wait for exit
         */
        printf( "\nPress any key to exit\n" )                           ;
        getch()                                                         ;
        return                                                          ;
}

//-----------------------------------------------------------------------------

void
Strip_Index_Wizard( void )
{
        /*
         *      Remove the index from the end of the archive,
         *      this requires creating a new temp file
         */
        char *rbuffer = calloc( 512, sizeof( char ) );
        int key = 0;
        char *archive;
        char *new_archive;

        printf( "\nPlease enter path + filename of archive :\n" )       ;
        scanf( "%s", rbuffer )                                          ;
        archive = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) ) ;
        strcpy( archive, rbuffer )                                      ;
        printf( "\nPlease enter path + filename of new archive :\n" )   ;
        scanf( "%s", rbuffer )                                          ;
        new_archive = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) ) ;
        strcpy( new_archive, rbuffer )                                  ;
        free( rbuffer );
        printf( "\nPlease enter archive key :\n" )                      ;
        scanf( "%i", &key )                                             ;

        /*
         *      Do the actual stripping
         */
        if ( DVP_Strip_Index( archive, new_archive, key ) )
        {
                printf( "There has been an error\n" )                   ;
        }
        else
        {
                printf( "...process completed\n" )                      ;
        }

        /*
         *      Wait for exit
         */
        printf( "\nPress any key to exit\n" )                           ;
        getch()                                                         ;
        return                                                          ;
}

//-----------------------------------------------------------------------------

void
Extract_File_Wizard( void )
{
        char *rbuffer = calloc( 1024, sizeof( char ) )   ;
        char *archive = NULL                             ;
        char *filename = NULL                            ;
        int key = 0                                      ;
        int blocksize                                    ;
        FILE *fptr = NULL                                ;
        TDVP_Index_Entry *entry = NULL                   ;

        /*
         *      Extract file specified
         */
        printf( "\nPlease enter path + filename of archive :\n" )       ;
        scanf( "%s", rbuffer )                                          ;
        archive = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) ) ;
        strcpy( archive, rbuffer )                                      ;
        printf( "\nPlease enter name of file to extract :\n" )          ;
        scanf( "%s", rbuffer )                                          ;
        filename = ( char* ) calloc( strlen( rbuffer ), sizeof( char ) );
        strcpy( filename, rbuffer )                                     ;
        free( rbuffer )                                                 ;
        printf( "\nPlease enter archive key :\n" )                      ;
        scanf( "%i", &key )                                             ;

        /*
         *      Open archive
         */
        fptr = fopen( archive, "rb" )                                   ;
        free( archive )                                                 ;
        if ( !fptr )
        {
                printf( "\nError opening file\n" )                      ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                return                                                  ;
        }

        /*
         *      Obtain file entry data
         */
        entry = DVP_Find_Entry_Data( fptr, filename, key  )             ;
        if ( !entry )
        {
                printf( "Unable to locate %s in archive\n", filename )  ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                return                                                  ;
        }
        if ( entry->size == 0 )
        {
                printf( "Error : Dir and not a file\n" )                ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                return                                                  ;
        }

        /*
         *      Do the actual extracting
         */
        blocksize = 1024                                                ; 
        if ( DVP_Extract_File( fptr, entry->offset, entry->size, blocksize, filename ) )
        {
                printf( "\nError extracting file\n" )                   ;
                
                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                fclose( fptr )                                          ;
                return                                                  ;
        }
        printf( "...process completed\n" )                              ;
        fclose( fptr )                                                  ;
        
        /*
         *      Wait for exit
         */
        printf( "\nPress any key to exit\n" )                           ;
        getch()                                                         ;
        return                                                          ;
}

//-----------------------------------------------------------------------------

void
Print_Info_Header( void )
{
        /*
         *  Build version string
         */
        char versionStr[ 16 ];
        sprintf( "%d.%d.%d.%d", DVP_MAJOR_VERSION, DVP_MINOR_VERSION, DVP_RELEASE_VERSION, DVP_PATCH_VERSION );
        
        /*
         *      Output logo to console
         */
        system( "cls" );
        printf( "*****************************************\n" )         ;
        printf( "*					*\n" )          ;
        printf( "*	DVP Pack tool 		        *\n" )          ;
        printf( "*					*\n" )          ;
        printf( "*    Copyright (C) Dinand Vanvelzen     *\n")          ;
        printf( "*    2002 - 2003. All rights reserved.  *\n")          ;
        printf( "*					*\n" )          ;
        printf( "*****************************************\n" )         ;
        printf( "\n" )                                                  ;
        printf( " - Tool Version %d\n" , VERSION_NUMBER )               ;
        printf( " - Module Version %d\n" , versionStr )         ;
        #ifdef SUPPORT_ARGS
                printf( " - Command line arguments are supported\n" )   ;
                printf( "   use the -args switch to view options\n" )   ;
        #else
                printf( " - Command line arguments are disabled\n" )    ;
        #endif        
        printf( "\n" )                                                  ;
}

//-----------------------------------------------------------------------------

#ifdef SUPPORT_ARGS

//-----------------------------------------------------------------------------

void
Display_Argument_Options( void )
{
        /*
         *      Display possible arguments
         */
        printf( " # Display these options :\n" )                                                ;
        printf( " Usage : <program_name.exe> -args\n" )                                         ;
        printf( "\n" )                                                                          ;
        printf( " # Creating an Archive :\n" )                                                  ;
        printf( " Usage: <program_name.exe> -p <archive_name> <src_dir> <id> <key>\n" )         ;
        printf( " Creates an archive named <archive_name> out of the <src_dir> data tree\n" )   ;
        printf( "\n" )                                                                          ;
        printf( " # Unpack an Archive :\n" )                                                    ;
        printf( " Usage: <program_name.exe> -e <archive_name> <des_dir> <key>\n" )              ;
        printf( " Extracts all files stored in an %s archive\n", DVP_PACK_EXT )                 ;
        printf( "\n" )                                                                          ;
        printf( " # Displaying the Archive Index :\n" )                                         ;
        printf( " Usage: <program_name.exe> -i <archive_name> <key>\n" )                        ;
        printf( " Reads and displays the archive index\n" )                                     ;
        printf( "\n" )                                                                          ;
        printf( " # Viewing the header :\n" )                                                   ;
        printf( " Usage: <program_name.exe> -h <archive_name> <key>\n" )                        ;
        printf( " Displays the Header information of the %s file\n", DVP_PACK_EXT )             ;
        printf( "\n" )                                                                          ;
        printf( " # Extracting a file :\n" )                                                    ;
        printf( " Usage: <program_name.exe> -ef <filename>\n" )                                 ;
        printf( " Extracts the file out of the %s file\n", DVP_PACK_EXT )                       ;
        printf( "\n" )                                                                          ;
        printf( " # Extracting a %s file :\n", DVP_IDX_EXT )                                    ;
        printf( " Usage: <program_name.exe> -ei <archive_name>\n" )                             ;
        printf( " Extracts the %s index file out of the %s file\n", DVP_IDX_EXT, DVP_PACK_EXT ) ;
        printf( " Then displays the index to provide a visual check\n" )                        ;
        printf( "\n" )                                                                          ;
        printf( "Press any key to exit...\n" )                                                  ;
        getch()                                                                                 ;
        return                                                                                  ;
}

//-----------------------------------------------------------------------------

void
Extract_All_Files( char *argv[] )
{
        /*
         *      Extract all files out of archive, unpacking it
         */
        FILE *fptr = NULL                                               ;
        fptr = fopen( argv[2], "rb" )                                   ;
        if ( !fptr )
        {
                printf( "\nUnable to open file\n" )                     ;
                return                                                  ;
        }

        /*
         *      Do the actual extraction
         */
        if ( DVP_Extract_All_Files( fptr, argv[3], Str_To_Int( argv[4] ) ) )
        {
                printf( "\nAn error occured\n" )                        ;
                return                                                  ;
        }
        printf( "\n... process completed successfully\n" )              ;
        fclose( fptr )                                                  ;
        return                                                          ;
}

//-----------------------------------------------------------------------------

void
Display_Header( char *argv[] )
{
        /*
         *      Output archive header data
         */
        long index_offset                                               ;
        long  version                                                    ;
        long  num_files                                                  ;
        long  num_dirs                                                   ;
        long lastchange = 0                                             ;
        char ID[ DVP_ID_LENGTH+1 ]                                      ;
        int key = Str_To_Int( argv[3] )                                 ; 
        FILE *fptr = fopen( argv[2], "rb" )                             ;
        if ( fptr )
        {
                /*
                 *      Do the actual obtaining of header data
                 */
                ID[ DVP_ID_LENGTH ] = 0                                 ;
                if ( !DVP_Read_Header( fptr          ,
                                       ID            ,
                                       &version      ,
                                       &index_offset ,
                                       &num_files    ,
                                       &num_dirs     ,
                                       &lastchange   ,
                                       key           ) )
                {
                        printf( "\nHeader data obtained...\n\n" )       ;
                        printf( "ID          = %s\n", ID )              ;
                        printf( "version     = %i\n", version )         ;
                        printf( "nr of files = %i\n", num_files )       ;
                        printf( "nr of dirs  = %i\n", num_dirs )        ;

                        /*
                         *      Wait for exit
                         */
                        printf( "\nPress any key to exit\n" )           ;
                        getch()                                         ;
                }
                else
                {
                        /*
                         *      An error occured.. wrong checksum
                         */
                        printf( "\nError obtaing header data\n" )       ;
                        printf( "The file may be invalid or corrupt\n") ;

                        /*
                         *      Wait for exit
                         */
                        printf( "\nPress any key to exit\n" )           ;
                        getch()                                         ;
                }
        }
        else
        {
                printf( "\nUnable to open file\n" )                     ;
        }
        return                                                          ;

}

//-----------------------------------------------------------------------------

void
Create_Archive( char *argv[] )
{
        /*
         *      Create an archive
         */
        int key = Str_To_Int( argv[ 5 ] )                               ;
        if ( !DVP_Create_Archive( argv[ 2 ], argv[ 3 ], key, argv[ 4 ] ) )
        {
                printf( "...Process completed\n" )                      ;
        }
        else
        {
                printf( "An error occured\n" )                          ;
        }

        /*
         *      Exit
         */
        printf( "\nPress any key to exit...\n" )                        ;
        getch()                                                         ;
        return                                                          ;
}

//-----------------------------------------------------------------------------

void
Display_Index( char *argv[] )
{
        char *ext = NULL                                                ;
        long pos = 0                                                    ;
        long offset                                                     ;
        long files = 0                                                   ;
        long lastchange = 0                                             ;
        long dirs = 0                                                    ;
        long tmp                                                         ;
        char ID[ DVP_ID_LENGTH+1 ]                                      ;
        FILE *fptr = NULL                                               ;
        int key = Str_To_Int( argv[3] )                                 ;

        fptr = fopen( argv[2], "rb" )                                   ;
        if ( !fptr )
        {
                printf( "\nError opening file\n" )                      ;
                return                                                  ;
        }

        /*
         *      Check file extension
         */
        ext = Extract_File_Ext( argv[2] )                               ;

        /*
         *      Check for index file
         */
        if ( !strcmp( ext, DVP_IDX_EXT ) )
        {
                /*
                 *      Index file so we don't use a header
                 */
                fseek( fptr , 0 , SEEK_END )                            ;
                fflush( fptr )                                          ;
                pos = ftell( fptr )                                     ;
                pos /= DVP_INDEX_ENTRY_SIZE                             ;
                files = pos                                             ;
                offset = 0                                              ;
        }
        else
        {
                /*
                 *      Get index offset and entry count
                 */
                ID[ DVP_ID_LENGTH ] = 0                                 ;                 
                if ( DVP_Read_Header( fptr               ,
	                              ID  	         ,
        	                      &tmp 	         ,
        	                      &offset            ,
                	              &files             ,
	                              &dirs	         ,
                                      &lastchange        ,
        	                      key		 ) )
                {
                        printf( "\nError obtaing header data\n" )       ;
                        printf( "The file may be invalid or corrupt\n") ;

                        /*
                         *      Wait for exit
                         */
                        printf( "\nPress any key to exit\n" )           ;
                        getch()                                         ;
                        return                                          ;
                }
        }
        free( ext )                                                     ;

        /*
         *      Output index to console
         */
        printf( "\n------------------------------------\n" )            ;
        printf( "-      ARCHIVE CONTENTS            -\n" )              ;
        printf( "------------------------------------\n" )              ;
        if ( DVP_Display_Index( fptr          ,
                                offset        ,
                                files+dirs    ) )
        {
                printf( "\nError while reading index\n" )               ;

                /*
                 *      Wait for exit
                 */
                printf( "\nPress any key to exit\n" )                   ;
                getch()                                                 ;
                return                                                  ;
        }

        printf( "\n...Process completed\n" )                            ;
        fclose( fptr )                                                  ;

        /*
         *      Wait for exit
         */
        printf( "\nPress any key to exit\n" )                           ;
        getch()                                                         ;
        return                                                          ;

}

//-----------------------------------------------------------------------------

#endif // SUPPORT_ARGS ?

//-----------------------------------------------------------------------------

int
Main_Menu( void )
{
        int mchoice     ;

        /*
         *      Output main menu to console
         */
        printf( " 1. Create an archive\n" )                             ;
        printf( " 2. Unpack an archive\n" )                             ;
        printf( " 3. Display archive index\n" )                         ;
        printf( " 4. Strip archive index\n" )                           ;
        printf( " 5. View archive header data\n" )                      ;
        printf( " 6. Extract a file\n" )                                ;
        printf( " 7. Extract a %s file\n", DVP_IDX_EXT )                ;
        printf( " 8. Exit\n" )                                          ;
        printf( "\n" )                                                  ;
        printf( "Please choose an option listed above\n" )              ;
        scanf( "%i", &mchoice )                                         ;
        return( mchoice )                                               ;
}

//-----------------------------------------------------------------------------

#ifdef SUPPORT_ARGS

int main(int argc, char *argv[] )
{

#else
int main( void )
{

#endif // SUPPORT_ARGS ?

        int mchoice = -1        ;
        
        /*
         *      Print Logo
         */
        Print_Info_Header()                                             ;

        #ifdef SUPPORT_ARGS

        /*
         *      if no arguments go into menu mode
         */
	if ( argc < 2 )
	{

        #endif // SUPPORT_ARGS ?
        
                /*
                 *      Start at main menu
                 */
                mchoice = Main_Menu()                                   ;
                switch ( mchoice )
                {
                        case 1  :
                        {
                                /*
                                 *      run create archive wizard
                                 */
                                Create_Archive_Wizard()                 ;
                                break                                   ; 
                        }
                        case 2  :
                        {
                                /*
                                 *      Unpack an archive
                                 */
                                Extract_All_Files_Wizard()              ;
                                break                                   ;                                 
                        }
                        case 3  :
                        {
                                /*
                                 *      run display index wizard
                                 */
                                Display_Index_Wizard()                  ;
                                break                                   ;
                        }
                        case 4  :
                        {
                                /*
                                 *      run strip index data wizard
                                 */
                                Strip_Index_Wizard();
                                break;
                        }
                        case 5  :
                        {
                                /*
                                 *      run Display header data wizard
                                 */
                                Display_Header_Wizard()                 ;
                                break                                   ;
                        }
                        case 6  :
                        {
                                /*
                                 *      Extract index file out of archive
                                 */
                                Extract_File_Wizard()                   ;
                                break                                   ;  
                        }
                        case 7  :
                        {
                                /*
                                 *      Extract index file out of archive
                                 */
                                Extract_Index_File_Wizard()             ;
                                break;  
                        }
                        case 8  :
                        {
                                /*
                                 *      Exit program
                                 */
                                return 0                                ;
                        }
                        default :
                        {
                                return 0                                ;
                        }
                }

        #ifdef SUPPORT_ARGS

	}
        else
        {
                /*
                 *      handle the comand line swicthes 
                 */
                if ( strcmp( argv[ 1 ], "-args" ) == 0  )
                {
                        Display_Argument_Options()                      ;
                }                               
                else
                {
                        if ( strcmp( argv[ 1 ], "-p" ) == 0 )
                        {
                                /*
                                 *      Create a new archive
                                 */
                                if ( argc == 6 )
                                {
                                        Create_Archive( argv )          ;
                                }
                                else
                                {
                                        printf( "incorrect number of arguments\n" );
                                }
                        }
                        else
                        {
                                if ( strcmp( argv[ 1 ], "-e" ) == 0 )
                                {
                                        /*
                                         *      Extract all files from archive
                                         */
                                        if ( argc == 5 )
                                        {
                                                Extract_All_Files( argv );
                                        }
                                        else
                                        {
                                                printf( "incorrect number of arguments\n" );
                                        }
                                }
                                else
                                {
                                        if ( strcmp( argv[ 1 ], "-i" ) == 0 )
                                        {
                                                /*
                                                 *      Display archive index 
                                                 */
                                                if ( argc == 4 )
                                                {
                                                        Display_Index( argv );
                                                }
                                                else
                                                {
                                                        printf( "incorrect number of arguments\n" );
                                                }
                                        }
                                        else
                                        {
                                                if ( strcmp( argv[ 1 ], "-h" ) == 0 )
                                                {
                                                        /*
                                                         *      Display archive header data
                                                         */
                                                        if ( argc == 4 )
                                                        {
                                                                Display_Header( argv );
                                                        }
                                                        else
                                                        {
                                                                printf( "incorrect number of arguments\n" );
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }

        #endif // SUPPORT_ARGS ?

	return 0;
}

//---------------------------------------------------------------------------
