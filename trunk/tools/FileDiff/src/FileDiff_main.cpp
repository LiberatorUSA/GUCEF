/*
 *  GucefLogService: Independent logging service for GUCEF
 *
 *  Copyright (C) 2002 - 2009.  Dinand Vanvelzen
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

#ifndef GUCEF_OSMAIN_H
#include "gucef_osmain.h"
#define GUCEF_OSMAIN_H
#endif /* GUCEF_OSMAIN_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

const bool defaultShowConsoleState = true;

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SBlockMatch;
typedef struct SBlockMatch TBlockMatch;

struct SBlockMatch
{
    size_t sizeOfBlock;
    unsigned long offsetInFile;
    
    bool operator<( const TBlockMatch& other ) const
    {
        return offsetInFile < other.offsetInFile;
    }
};

typedef std::vector< TBlockMatch > TBlockMatchVector;
typedef std::map< TBlockMatch, TBlockMatchVector > TBlockMatchVectorMap;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
ParseParams( const int argc                 , 
             char* argv[]                   ,
             CORE::CValueList& keyValueList )
{GUCEF_TRACE;
    
    keyValueList.DeleteAll();
    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        // Combine the argument strings back into a single string because we don't want to use
        // a space as the seperator
        for ( int i=1; i<argc; ++i )
        {
            argString += ' ' + CORE::CString( argv[ i ] );
        }
        
        // Parse the param list based on the ' symbol
        keyValueList.SetMultiple( argString, '\'' );
    }
}

/*-------------------------------------------------------------------------*/

void
FindIdenticalBlock( FILE* file                          ,
                    void* data                          ,
                    unsigned int blockSize              ,
                    TBlockMatchVector& blockMatchVector )
{GUCEF_TRACE;

    void* fileBlock = new unsigned char[ blockSize ];
    size_t actuallyRead = 0;
    
    fseek( file, 0, SEEK_SET );
    while ( 0 != feof( file ) )
    {
        actuallyRead = fread( fileBlock, 1, blockSize, file );
        if ( actuallyRead > 0 )
        {
            if ( 0 == memcmp( data, fileBlock, actuallyRead ) )
            {
                TBlockMatch blockMatch;
                blockMatch.offsetInFile = ftell( file )-actuallyRead;
                blockMatch.sizeOfBlock = actuallyRead;
                blockMatchVector.push_back( blockMatch );
            }
        }
    }
    
    delete []fileBlock;
    fileBlock = NULL;
}

/*-------------------------------------------------------------------------*/

void
FindIdenticalBlocks( FILE* file1                               ,
                     FILE* file2                               ,
                     TBlockMatchVectorMap& blockMatchVectorMap )
{GUCEF_TRACE;
    
    const size_t testBlockSizeInBytes = 32;
    void* testBuffer = new char[ testBlockSizeInBytes ];
    memset( testBuffer, 0, testBlockSizeInBytes );
        
    size_t offsetInSrcFile = 0;
    while ( 0 == feof( file1 ) )
    {
        size_t actuallyRead = fread( testBuffer           , 
                                     1                    , 
                                     testBlockSizeInBytes , 
                                     file1                );         
        
        TBlockMatchVector blockMatches;
        FindIdenticalBlock( file2        , 
                            testBuffer   , 
                            actuallyRead , 
                            blockMatches );
                            
        if ( !blockMatches.empty() )
        {
            TBlockMatch sourceBlock;
            sourceBlock.offsetInFile = offsetInSrcFile;
            sourceBlock.sizeOfBlock = actuallyRead;

            blockMatchVectorMap[ sourceBlock ] = blockMatches;
        }
        offsetInSrcFile += actuallyRead;
    }
    
    delete []testBuffer;
    testBuffer = NULL; 
}

/*-------------------------------------------------------------------------*/

void
FindIdenticalBlocks( const CORE::CString& file1             ,
                     const CORE::CString& file2             ,
                     TBlockMatchVectorMap& blockMatchVector )
{GUCEF_TRACE;

    FILE* file1Ptr = fopen( file1.C_String(), "rb" );
    FILE* file2Ptr = fopen( file2.C_String(), "rb" );
    
    if ( NULL != file1Ptr && NULL != file2Ptr )
    {   
        FindIdenticalBlocks( file1Ptr         , 
                             file2Ptr         , 
                             blockMatchVector );
    }
    if ( NULL != file1Ptr )
    {
        fclose( file1Ptr );
    }
    if ( NULL != file2Ptr )
    {
        fclose( file2Ptr );
    }    
}

/*-------------------------------------------------------------------------*/

GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "This tool was compiled on: " __DATE__ " @ " __TIME__ );
    
    #ifdef GUCEF_CALLSTACK_TRACING
    CORE::GUCEF_LogStackTo( "GucefLogServiceApp_Callstack.cvs" );
    CORE::GUCEF_SetStackLoggingInCvsFormat( 1 );
    CORE::GUCEF_SetStackLogging( 1 );
    #endif /* GUCEF_CALLSTACK_TRACING ? */    
    
    CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( logFilename, "FileDiff_Log.txt" );
    CORE::CFileAccess logFileAccess( logFilename, "w" );
    
    CORE::CStdLogger logger( logFileAccess );
    CORE::CLogManager::Instance()->AddLogger( &logger );
    
    // Parse the application parameters
    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );

    // Do we want to display the console window?
    CORE::CPlatformNativeConsoleLogger* consoleOut = NULL;    
    bool showConsole = defaultShowConsoleState;    
    if ( keyValueList.HasKey( "showConsole" ) )
    {
        showConsole = CORE::StringToBool( keyValueList.GetValue( "showConsole" ) );
    }
    if ( showConsole )
    {
        consoleOut = new CORE::CPlatformNativeConsoleLogger();
        CORE::CLogManager::Instance()->AddLogger( consoleOut->GetLogger() );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FileDiff: Enabled console output" );
    }
    
    // Flush bootstrap logging now that we attached all our loggers
    CORE::CLogManager::Instance()->FlushBootstrapLogEntriesToLogs();
    
    
    delete consoleOut;
    consoleOut = NULL;
    
    return 1;
}
GUCEF_OSMAIN_END

/*---------------------------------------------------------------------------*/
