/*
 *  gucefPATCHER: GUCEF RAD module providing a patch delivery system
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#include "gucefPATCHER_CPatchSetFileEngine.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetFileEngine::CPatchSetFileEngine( void )
    : CObservingNotifier()        ,
      CPatchSetFileEngineEvents() ,
      m_isActive( false )         ,
      m_stopSignalGiven( false )  ,
      m_localRoot()               ,
      m_tempStorageRoot()         ,
      m_dataRetriever()           ,
      m_fileAccess( NULL )        ,
      m_fileList()                ,
      m_curFileLocIndex( 0 )      ,
      m_curFileIndex( 0 )
{GUCEF_TRACE;

    SubscribeTo( &m_dataRetriever );
}

/*-------------------------------------------------------------------------*/

CPatchSetFileEngine::~CPatchSetFileEngine()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPatchSetFileEngine::Start( const TFileList& fileList            ,
                            const CORE::CString& localRoot       ,
                            const CORE::CString& tempStorageRoot )

{GUCEF_TRACE;

    // The user should explicitly stop first if we are already busy
    if ( !IsActive() )
    {
        // parameter sanity check
        if ( ( fileList.size() > 0 )   &&
             ( localRoot.Length() > 0 ) )
        {
            m_isActive = true;
            m_stopSignalGiven = false;
            
            // Copy and link params for later use
            m_fileList = fileList;
            m_localRoot = localRoot;
            m_tempStorageRoot = tempStorageRoot;
            m_curFileIndex = 0;
            m_curFileLocIndex = 0;
            
            NotifyObservers( FileListProcessingStartedEvent );
            
            // Set things in motion
            return ProcessCurrentFile();
        }
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetFileEngine::ProcessCurrentFile( void )
{GUCEF_TRACE;

    if ( !m_stopSignalGiven                 && 
         m_isActive                         && 
         m_fileList.size() > m_curFileIndex )
    {
        // Get our current file
        const TFileEntry* curFile = &m_fileList[ m_curFileIndex ];
        
        // Create a path string to the file
        CORE::CString filePath = m_localRoot;
        CORE::AppendToPath( filePath, curFile->name );
        
        // Check if the file exists
        if ( CORE::FileExists( filePath ) )
        {
            // The file exists, let's see if it's up to date
            UInt32 fileSize = CORE::Filesize( filePath.C_String() );
            if ( fileSize == curFile->sizeInBytes )
            {
                // The file matches in byte size, it may be up-to-date
                // We will try a hash comparison to make sure
                UInt8 md5Digest[ 16 ];
                CORE::CFileAccess fileAccess( filePath, "rb" );
                fileAccess.Open();
                if ( fileAccess.Opened() && fileAccess.IsValid() )
                {
                    if ( 0 != CORE::md5frommfile( fileAccess.CStyleAccess() , 
                                                  md5Digest                 ) )
                    {
                        // This may have been a lengthy operation so we re-check the stop signal
                        if ( m_stopSignalGiven || !m_isActive )
                        {
                            m_stopSignalGiven = false;
                            m_isActive = false;
                            return false;
                        }                        
                        
                        // We have now generated an md5 hash from the file
                        // We will now compare it to the one an up-to-date file should have
                        CORE::CString md5String( CORE::MD5ToString( md5Digest ) );
                        if ( curFile->hash == md5String )
                        {
                            // Woohoo, this file is already up-to-date
                            NotifyObservers( LocalFileIsOKEvent );
                            
                            // We can now proceed to the next file if there is any
                            if ( m_curFileIndex+1 < m_fileList.size() )
                            {
                                ++m_curFileIndex;
                                return ProcessCurrentFile();
                            }
                            else
                            {
                                // We are finished,.. engine stopping
                                NotifyObservers( FileListProcessingCompleteEvent );
                                m_isActive = false;
                            }
                            return true;
                        }
                    }
                    else
                    {
                        NotifyObservers( LocalFileHashMismatchEvent );
                    }
                }  
            }
            else
            {
                NotifyObservers( LocalFileSizeMismatchEvent );
            }
        }
        else
        {
            NotifyObservers( LocalFileNotFoundEvent );
        }    
        
        // If we get here then the file was found but it is not up-to-date or
        // the file does not even exist locally, either way,.. we have to update it
        CORE::CString tmpFilePath = m_tempStorageRoot;
        CORE::AppendToPath( tmpFilePath, curFile->name );
        
        // make sure the path exists
        CORE::Create_Directory( CORE::StripFilename( tmpFilePath ).C_String() );
        
        // Try to create the file
        m_fileAccess = new CORE::CFileAccess( tmpFilePath, "wb" );
        m_fileAccess->Open();
        if ( m_fileAccess->Opened() && m_fileAccess->IsValid() )
        {
            // We now have write access to our temp new file.
            // We will now tell our URL buddy to get the file for us
            m_dataRetriever.GetURL().SetURL( curFile->fileLocations[ 0 ].URL );
            m_dataRetriever.SetIOAccess( m_fileAccess );
            while ( m_curFileLocIndex < curFile->fileLocations.size() )
            {
                if ( m_dataRetriever.GetURL().Activate() )
                {
                    // The data retrieval process has now commenced
                    // This is an a-sync operation, we wait for notification
                    NotifyObservers( FileRetrievalStartedEvent );
                    return true;
                }
                else
                {
                    NotifyObservers( FileRetrievalErrorEvent );
                    
                    // if we get here then we where unable to activate the given URL
                    // We will try the next one,.. if there is a next one
                    if ( m_curFileLocIndex+1 < curFile->fileLocations.size() )
                    {
                        // we ran out of options,.. the end
                        delete m_fileAccess;
                        m_fileAccess = NULL;
                        NotifyObservers( FileListProcessingAbortedEvent );
                        m_stopSignalGiven = false;
                        m_isActive = false;
                        return false;
                    }
                }
                ++m_curFileLocIndex;
            }
        }
        else
        {
            // We failed to obtain writable access to our temp file storage
            // we ran out of options,.. the end
            delete m_fileAccess;
            m_fileAccess = NULL;
            NotifyObservers( FileStorageErrorEvent );
            m_stopSignalGiven = false;
            m_isActive = false;                
            return false;
        }       
    }
    else
    {
        if ( m_fileList.size() < m_curFileIndex )
        {
            NotifyObservers( FileListProcessingAbortedEvent );
            m_stopSignalGiven = false;
            m_isActive = false;
        }
        else
        {
            // We are finished,.. engine stopping
            NotifyObservers( FileListProcessingCompleteEvent );
            m_isActive = false;
            m_stopSignalGiven = false;        
        }
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/
    
void
CPatchSetFileEngine::Stop( void )
{GUCEF_TRACE;

    if ( m_isActive )
    {
        m_stopSignalGiven = true;
        
        if ( m_dataRetriever.GetURL().IsActive() )
        {
            m_dataRetriever.GetURL().Deactivate();
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetFileEngine::IsActive( void ) const
{GUCEF_TRACE;

    return m_isActive;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetFileEngine::TryNextFileLocation( void )
{GUCEF_TRACE;

    m_fileAccess->Close();

    const TFileEntry& curFile = m_fileList[ m_curFileIndex ];
    do
    {
        // if we get here then we where unable to retrieve the file using the given URL
        // We will try the next one,.. if there is a next one
        if ( m_curFileLocIndex+1 < curFile.fileLocations.size() )
        {
            // we ran out of options,.. the end
            m_stopSignalGiven = false;
            m_isActive = false;
            return false;
        }
        
        ++m_curFileLocIndex;
        
        m_fileAccess->Open();
        if ( m_dataRetriever.GetURL().Activate() )
        {
            // The data retrieval process has now commenced
            // This is an a-sync operation, we wait for notification
            return true;
        }
    }
    while ( m_curFileLocIndex < curFile.fileLocations.size() );
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetFileEngine::ProceedToNextFile( void )
{GUCEF_TRACE;

    // Close the last file if it is still opened
    if ( m_fileAccess != NULL )
    {
        m_fileAccess->Close();
        delete m_fileAccess;
        m_fileAccess = NULL;
    }
    
    m_curFileLocIndex = 0;
    ++m_curFileIndex;
    
    return ProcessCurrentFile();
}

/*-------------------------------------------------------------------------*/

void
CPatchSetFileEngine::OnNotify( CORE::CNotifier* notifier                 ,
                               const CORE::CEvent& eventid               ,
                               CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Call base-class version
        CORE::CObservingNotifier::OnNotify( notifier  ,
                                            eventid   ,
                                            eventdata );
                                            
        if ( notifier == &m_dataRetriever )
        {
            if ( eventid == CORE::CURLDataRetriever::URLAllDataRecievedEvent )
            {
                // Check if we stored the file in an alternate location
                if ( m_tempStorageRoot != m_localRoot )
                {
                    // safety check,.. just in case
                    assert( m_fileAccess != NULL );
                    if ( m_fileAccess != NULL )
                    {
                        // Create the destination path
                        CORE::CString destPath( m_localRoot );
                        CORE::AppendToPath( destPath, CORE::ExtractFilename( m_fileAccess->GetFilename() ) );
                        
                        // If a file already exists at the target location attempt to delete it
                        if ( CORE::FileExists( destPath ) )
                        {
                            if ( 0 == CORE::Delete_File( destPath.C_String() ) )
                            {
                                // We failed to delete the old file,.. the end
                                NotifyObservers( FileStorageErrorEvent );
                                m_stopSignalGiven = false;
                                m_isActive = false;
                                return;                            
                            }
                        }
                        
                        // Move the new file to the desired final location
                        if ( 0 == CORE::Move_File( destPath.C_String(), m_fileAccess->GetFilename().C_String() ) )
                        {
                            // Moving the file failed,.. the end
                            NotifyObservers( FileStorageErrorEvent );
                            m_stopSignalGiven = false;
                            m_isActive = false;
                            return;                             
                        }
                        
                        // We successfully replaced/patched a local file
                        NotifyObservers( LocalFileReplacedEvent );
                    }
                }
                
                ProceedToNextFile();
            }
            else
            if ( eventid == CORE::CURLDataRetriever::URLDataRetrievalErrorEvent )
            {   
                TryNextFileLocation();
            }
        }
    }
    else
    {
        NotifyObservers( FileListProcessingAbortedEvent );        
        m_stopSignalGiven = false;
        m_isActive = false;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/