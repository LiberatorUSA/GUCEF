/*
 *  GucefArchiver: Tool to manipulate archives
 *  Copyright (C) 2002 - 2015.  Dinand Vanvelzen
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

#include <stdio.h>

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_IMAGE_H
#include "gucefIMAGE.h"
#define GUCEF_IMAGE_H
#endif /* GUCEF_IMAGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SPcmWaveHeader
{
    // The RIFF chunk descriptor
    char ChunkID[ 4 ];              // should be "RIFF"
    CORE::Int32 ChunkSize;          // 36 + SubChunk2Size, or more precisely: 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size). This is the size of the rest of the chunk following this number.  This is the size of the entire file in bytes minus 8 bytes for the two fields not included in this count: ChunkID and ChunkSize.
    char Format[ 4 ];               // should be "WAVE"
    
    // The format sub chunk
    char Subchunk1ID[ 4 ];          // should be "fmt "
    CORE::Int32 Subchunk1Size;      // 16 for PCM. This is the size of the rest of the Subchunk which follows this number.
    CORE::Int16 AudioFormat;        // PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some form of compression.
    CORE::Int16 NumChannels;        // Mono = 1, Stereo = 2, etc.
    CORE::Int32 SampleRate;         // 8000, 44100, etc.
    CORE::Int32 ByteRate;           // SampleRate * NumChannels * BitsPerSample/8
    CORE::Int16 BlockAlign;         // NumChannels * BitsPerSample/8  The number of bytes for one sample including all channels. I wonder what happens when this number isn't an integer?
    CORE::Int16 BitsPerSample;      // 8 bits = 8, 16 bits = 16, etc.

    // The data sub chunk
    char Subchunk2ID[ 4 ];          // should be "data"
    CORE::Int32 Subchunk2Size;      // == NumSamples * NumChannels * BitsPerSample/8   This is the number of bytes in the data. You can also think of this as the size of the read of the subchunk following this number.
    // ? Data                       // The actual data block with all the raw data
};
typedef struct SPcmWaveHeader PcmWaveHeader;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
ConfigureITVPcmHeader( PcmWaveHeader& header, CORE::Int32 dataSize )
{
    memcpy( header.ChunkID, "RIFF", 4 );
    header.ChunkSize = 36 + dataSize;
    memcpy( header.Format, "WAVE", 4 );
    memcpy( header.Subchunk1ID, "fmt ", 4 );
    header.Subchunk1Size = 16;
    header.AudioFormat = 1; // ITV Audio is always PCM
    header.NumChannels = 1; // ITV PCM is always mono
    header.SampleRate = 22000; // ITV PCM is always 22KHz
    header.BitsPerSample = 8; // ITV PCM is always 8 bit sampling depth
    header.ByteRate = header.SampleRate * header.NumChannels * (header.BitsPerSample/8);
    header.BlockAlign = header.NumChannels * (header.BitsPerSample/8);
    memcpy( header.Subchunk2ID, "data", 4 );
    header.Subchunk2Size = dataSize;    
}

/*-------------------------------------------------------------------------*/

void
WritePcmWaveHeader( const PcmWaveHeader& header ,
                    CORE::CIOAccess& outStream  )
{
    outStream.Write( header.ChunkID, 4, 1 );
    outStream.Write( &header.ChunkSize, 4, 1 );
    outStream.Write( header.Format, 4, 1 );
    outStream.Write( &header.Subchunk1ID, 4, 1 );
    outStream.Write( &header.Subchunk1Size, 4, 1 );
    outStream.Write( &header.AudioFormat, 2, 1 );
    outStream.Write( &header.NumChannels, 2, 1 );
    outStream.Write( &header.SampleRate, 4, 1 );
    outStream.Write( &header.ByteRate, 4, 1 );
    outStream.Write( &header.BlockAlign, 2, 1 );
    outStream.Write( &header.BitsPerSample, 2, 1 );
    outStream.Write( header.Subchunk2ID, 4, 1 );
    outStream.Write( &header.Subchunk2Size, 4, 1 );
}

/*-------------------------------------------------------------------------*/

void
WriteITVPcm( CORE::CIOAccess& rawStream , 
             CORE::Int32 rawDataSize    , 
             CORE::CIOAccess& outStream )
{
    PcmWaveHeader header;
    ConfigureITVPcmHeader( header, rawDataSize );
    WritePcmWaveHeader( header, outStream );
    outStream.Write( rawStream );
}

/*-------------------------------------------------------------------------*/

bool
LoadConfig( const CORE::CString& configPath ,
            CORE::CValueList& keyValueList  )
{GUCEF_TRACE;

    #ifdef GUCEF_DEBUG_MODE
    const CORE::CString configFile = "itvExporter_d.ini";
    #else
    const CORE::CString configFile = "itvExporter.ini";
    #endif

    CORE::CString configFilePath;
    bool foundViaParam = false;
    if ( !configPath.IsNULLOrEmpty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configPath );
        foundViaParam = CORE::FileExists( configPath );
        configFilePath = configPath;
    }

    if ( !foundViaParam )
    {
        CORE::CString configFilePath = CORE::CombinePath( "$CURWORKDIR$", configFile );
        configFilePath = CORE::RelativePath( configFilePath );

        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
        if ( !CORE::FileExists( configFilePath ) )
        {
            configFilePath = CORE::CombinePath( "$MODULEDIR$", configFile );
            configFilePath = CORE::RelativePath( configFilePath );

            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
            if ( !FileExists( configFilePath ) )
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Unable to locate any config file, will rely on params" );
                return false;
            }
        }
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Located config file @ " + configFilePath );

    keyValueList.SetConfigNamespace( "Main/AppArgs" );
    keyValueList.SetUseGlobalConfig( true );
    keyValueList.SetAllowDuplicates( false );
    keyValueList.SetAllowMultipleValues( true );

    CORE::CConfigStore& configStore = CORE::CCoreGlobal::Instance()->GetConfigStore();
    configStore.SetConfigFile( configFilePath );
    return configStore.LoadConfig();
}

/*-------------------------------------------------------------------------*/

void
ParseParams( const int argc                 ,
             char* argv[]                   ,
             CORE::CValueList& keyValueList )
{GUCEF_TRACE;

    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        // Combine the argument strings back into a single string because we don't want to use
        // a space as the deliminator
        for ( int i=1; i<argc; ++i )
        {
            argString += ' ' + CORE::CString( argv[ i ] );
        }

        // Parse the param list based on the ' symbol
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Application parameters: " + argString );
        keyValueList.SetMultiple( argString, '\'' );
    }
}

/*---------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "This tool was compiled on: " __DATE__ " @ " __TIME__ );

    CORE::CCoreGlobal::Instance();
    VFS::CVfsGlobal::Instance();
    IMAGE::CImageGlobal::Instance();

    CORE::CPlatformNativeConsoleLogger console;
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( console.GetLogger() );

    // Check for config param first
    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );
    CORE::CString configPathParam = keyValueList.GetValueAlways( "configPath" );
    keyValueList.Clear();

    // Load settings from a config file (if any) and then override with params (if any)
    LoadConfig( configPathParam, keyValueList );
    ParseParams( argc, argv, keyValueList );

    CORE::Int32 minLogLevel = CORE::LOGLEVEL_BELOW_NORMAL;
    CORE::CString valueStr = keyValueList.GetValueAlways( "minimalLogLevel" );
    if ( !valueStr.IsNULLOrEmpty() )
    {
        minLogLevel = CORE::StringToInt32( valueStr );
        CORE::CCoreGlobal::Instance()->GetLogManager().SetMinLogLevel( minLogLevel );
    }

    CORE::CString outputDir = CORE::RelativePath( keyValueList.GetValueAlways( "outputDir" ) );
    if ( outputDir.IsNULLOrEmpty() )
    {
        outputDir = CORE::RelativePath( "$CURWORKDIR$" );
    }
    CORE::CreateDirs( outputDir );

    CORE::CString logFilename = CORE::CombinePath( outputDir, "itvExporter_Log.txt" );

    keyValueList.Set( "logfile", logFilename );

    CORE::CFileAccess logFileAccess( logFilename, "w" );
    CORE::CStdLogger logger( logFileAccess );
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &logger );

    CORE::CCoreGlobal::Instance()->GetLogManager().FlushBootstrapLogEntriesToLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Flushed to log @ " + logFilename );

    if ( argc > 0 )
    {
        CORE::CString archivePath = keyValueList.GetValueAlways( "archive" );
        archivePath = archivePath.Trim( true ).Trim( false );
        if ( !archivePath.IsNULLOrEmpty() )
        {
            CORE::CString archiveType = keyValueList.GetValueAlways( "archiveType" );
            if ( archiveType.IsNULLOrEmpty() )
            {
                archiveType = CORE::ExtractFileExtention( archivePath ).Lowercase();
            }
            archiveType = archiveType.Trim( true ).Trim( false );

            bool listArchiveContents = CORE::StringToBool( keyValueList.GetValueAlways( "list", "true" ) );
            bool extractArchiveContents = CORE::StringToBool( keyValueList.GetValueAlways( "extract", "false" ) );
            bool overwrite = CORE::StringToBool( keyValueList.GetValueAlways( "overwrite", "false" ) ); 
            bool extractImages = CORE::StringToBool( keyValueList.GetValueAlways( "extractImages", "true" ) );
            bool extractAudio = CORE::StringToBool( keyValueList.GetValueAlways( "extractAudio", "true" ) );
            bool extractAnimations = CORE::StringToBool( keyValueList.GetValueAlways( "extractAnimations", "true" ) );
            bool extractOther = CORE::StringToBool( keyValueList.GetValueAlways( "extractOther", "true" ) );

            CORE::CString archiveFilename = CORE::ExtractFilename( archivePath );
            CORE::CString archiveDir = CORE::StripFilename( archivePath );

            VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

            if ( archiveDir.IsNULLOrEmpty() )
            {
                archiveDir = CORE::RelativePath( "$CURWORKDIR$" );
            }

            // Add the archive dir as a VFS root named root
            vfs.AddRoot( archiveDir, "root", false, false );

            VFS::CVFS::TStringSet archiveFileNames;
            if ( -1 < archiveFilename.HasChar( '*' ) )
            {
                // Use the filename as a filter
                vfs.GetList( archiveFileNames, CORE::CString::Empty, false, false, archiveFilename, true, false );
            }
            else
            {
                // directly use the filename
                archiveFileNames.insert( archiveFilename );
            }

            GUCEF::IMAGE::CImage palette;  
            
            // Batch extract each of the archives
            VFS::CVFS::TStringSet::iterator n = archiveFileNames.begin();
            while ( n != archiveFileNames.end() )
            {            
                archiveFilename = (*n);
                
                if ( vfs.FileExists( archiveFilename ) )
                {   
                    CORE::CString mount = '[' + archiveFilename + ']';
                    
                    // Mount the archive
                    if ( vfs.MountArchive( mount, archiveFilename, archiveType, mount, false ) )
                    {
                        if ( listArchiveContents )
                        {
                            // Get the contents of the archive
                            VFS::CVFS::TStringSet archiveContent;
                            vfs.GetList( archiveContent, mount, true, true, CORE::CString::Empty, true, false );

                            // Write the contents to the log which includes the console
                            console.GetLogger()->SetFormatAsConsoleUI( true );
                            GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, "***** BEGIN ARCHIVE CONTENTS *****" );
                            CORE::UInt32 n=1;
                            VFS::CVFS::TStringSet::iterator i = archiveContent.begin();
                            while ( i != archiveContent.end() )
                            {
                                GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, CORE::UInt32ToString( n ) + ": " + (*i) );
                                ++i; ++n;
                            }
                            GUCEF_CONSOLE_LOG( CORE::LOGLEVEL_NORMAL, "***** END ARCHIVE CONTENTS *****" );
                            console.GetLogger()->SetFormatAsConsoleUI( false );
                        }

                        if ( extractArchiveContents )
                        {
                            // Get the contents of the archive
                            VFS::CVFS::TStringSet archiveContent;
                            vfs.GetList( archiveContent, mount, true, true, CORE::CString::Empty, true, false );
                            
                            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "archive contains " + CORE::UInt32ToString( archiveContent.size() ) + " entries" );

                            // prepare the output folder
                            CORE::CString archiveOutputDir = archiveFilename.ReplaceChar( '.', '_' );
                            archiveOutputDir = CORE::CombinePath( outputDir, archiveOutputDir );
                            if ( CORE::CreateDirs( archiveOutputDir ) )
                            {
                                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Output path " + archiveOutputDir + " is available for extraction" );
                            
                                // Obtain access to each of the resources in the archive and extract it
                                VFS::CVFS::TStringSet::iterator i = archiveContent.begin();
                                while ( i != archiveContent.end() )
                                {
                                    CORE::CString filename = CORE::ExtractFilename( (*i) );
                                    CORE::CString vfsFilePath = CORE::CombinePath( mount, filename );

                                    CORE::CString extractedFilePath = filename;
                                    extractedFilePath = CORE::CombinePath( archiveOutputDir, extractedFilePath );

                                    if ( !overwrite )
                                    {
                                        // Check if destination file already exists
                                        if ( CORE::FileExists( extractedFilePath ) )
                                        {
                                            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Skipping extraction of file since the destination path already exists and 'overwrite=false'. Path: " + extractedFilePath );
                                            ++i;
                                            continue;
                                        }
                                    }

                                    VFS::CVFS::CVFSHandlePtr file = vfs.GetFile( vfsFilePath, "rb", false );
                                    if ( !file.IsNULL() )
                                    {
                                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Obtained access to resource: " + (*i) );

                                        CORE::CString dataType = CORE::ExtractFileExtention( (*i) );
                                        if ( "itv10" == dataType )
                                        {
                                            if ( extractAudio )
                                            {
                                                extractedFilePath = extractedFilePath + ".wav";
                                                CORE::CFileAccess extractedFile;
                                                if ( extractedFile.Open( extractedFilePath, "wb" ) )
                                                {
                                                    WriteITVPcm( *file->GetAccess(), file->GetAccess()->GetSize(), extractedFile );
                                                    extractedFile.Close();
                                                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully wrote PCM audio as WAVE file to: " + extractedFilePath );
                                                }
                                                else
                                                {
                                                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to open target file for writing: " + extractedFilePath );
                                                }
                                            }
                                        }
                                        else
                                        if ( "itv19" == dataType )
                                        {
                                            if ( extractAnimations )
                                            {
                                                extractedFilePath = extractedFilePath + ".flic";
                                                CORE::CFileAccess extractedFile;
                                                if ( extractedFile.Open( extractedFilePath, "wb" ) )
                                                {
                                                    CORE::CIOAccess* fileAccess = file->GetAccess();
                                                    CORE::UInt32 fileSize = fileAccess->GetSize();

                                                    // Skip ITV header
                                                    fileAccess->Setpos( 55 );
                                                    fileSize -= 55;
                                                
                                                    // Dump remainder as FLIC file
                                                    CORE::UInt32 bytesExtracted = extractedFile.Write( *fileAccess );
                                                    extractedFile.Close();
                                        
                                                    if ( fileSize == bytesExtracted )
                                                    {
                                                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Extracted " + CORE::UInt32ToString( bytesExtracted ) + " bytes into FLIC file: " + extractedFilePath );
                                                    }
                                                    else
                                                    {
                                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Extracted " + CORE::UInt32ToString( bytesExtracted ) + " bytes instead of " +
                                                                 CORE::UInt32ToString( fileSize ) + " expected bytes into file: " + extractedFilePath + ". The file is probably corrupt" );
                                                    }

                                                    IMAGE::CImage flicFrames;
                                                    if ( flicFrames.Load( extractedFilePath ) )
                                                    {
                                                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded image data from FLIC file" );
                                                    }
                                                }
                                                else
                                                {
                                                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to open target file for writing: " + extractedFilePath );
                                                }
                                            }
                                        }
                                        else
                                        if ( "itv1" == dataType )
                                        {
                                            if ( extractImages )
                                            {                                            
                                                // Images are extracted as a common format instead
                                                IMAGE::CImage image;
                                                if ( image.Load( *file->GetAccess(), dataType ) )
                                                {
                                                    // lazy load pallette if neded
                                                    IMAGE::TPixelMapPtr palettePixels;
                                                    if ( !palette.TryGetPixelMap( 0, 0, palettePixels ) )
                                                    {
                                                        if ( vfs.FileExists( archiveFilename ) )
                                                        {   
                                                            CORE::CString palleteArchive = "IV_PAL.RES";
                                                            CORE::CString mount = '[' + palleteArchive + ']';
                    
                                                            // Mount the archive
                                                            if ( vfs.MountArchive( mount, palleteArchive, archiveType, mount, false ) )
                                                            {
                                                                CORE::CString subPath = mount + '\\' + "100.itv5";
                                                                VFS::CVFS::CVFSHandlePtr paletteFile = vfs.GetFile( subPath, "rb", false );
                                                                if ( paletteFile )
                                                                {
                                                                    if ( palette.Load( *paletteFile->GetAccess(), "itv5" ) )
                                                                    {
                                                                        palettePixels = palette.GetPixelMap(); 
                                                                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded palette for ITV images from " + subPath );
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    if ( palettePixels )
                                                    {
                                                        image.GetPixelMap()->FlipHorizontal();
                                                    
                                                        // Apply the palette so that we can save in another format
                                                        if ( image.GetPixelMap()->ApplyPalette( palettePixels ) )
                                                        {
                                                            image.GetPixelMap()->ApplyBrightness( 200 );
                                                        
                                                            extractedFilePath = extractedFilePath + ".png";
                                                            CORE::CFileAccess extractedFile;
                                                            if ( extractedFile.Open( extractedFilePath, "wb" ) )
                                                            {
                                                                if ( image.Save( extractedFile, "png" ) )
                                                                {
                                                                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully coverted image to PNG saved to " + extractedFilePath );
                                                                }
                                                            }                                                        
                                                        }
                                                        else
                                                        {
                                                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to apply palette to image " + file->GetFilename() );
                                                        } 
                                                    }
                                                    else
                                                    {
                                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to obtain palette for image " + file->GetFilename() );
                                                    }  
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if ( extractOther )
                                            {                                            
                                                CORE::CFileAccess extractedFile;
                                                if ( extractedFile.Open( extractedFilePath, "wb" ) )
                                                {
                                                    CORE::CIOAccess* fileAccess = file->GetAccess();
                                                    CORE::UInt32 fileSize = fileAccess->GetSize();
                                                    CORE::UInt32 bytesExtracted = extractedFile.Write( *fileAccess );
                                                    extractedFile.Close();
                                        
                                                    if ( fileSize == bytesExtracted )
                                                    {
                                                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Extracted " + CORE::UInt32ToString( bytesExtracted ) + " bytes into file: " + extractedFilePath );
                                                    }
                                                    else
                                                    {
                                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Extracted " + CORE::UInt32ToString( bytesExtracted ) + " bytes instead of " +
                                                                 CORE::UInt32ToString( fileSize ) + " expected bytes into file: " + extractedFilePath + ". The file is probably corrupt" );
                                                    }
                                                }
                                                else
                                                {
                                                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to open target file for writing: " + extractedFilePath );
                                                }
                                            }
                                        }                                                                                                                
                                    }
                                    else
                                    {
                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to obtain access to resource: " + filename );
                                    }
                                    ++i;
                                }
                            }                                                
                        }
                    
                        if ( !vfs.UnmountArchiveByName( mount ) )
                        {
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to unmount archive" );
                            break;
                        }
                    }
                    else
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Failed to mount archive" );
                    }
                }
                else
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Archive file does not exist, cannot proceed" );
                }

                ++n;
            }
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Missing minimal param \"archive\" thus no idea which archive to use" );
        }
    }
	
	GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Wrote log file to: " + logFilename );

    CORE::CCoreGlobal::Instance()->GetLogManager().ClearLoggers();

    return 0;
}
GUCEF_OSMAIN_END

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 25-08-2015 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
