/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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
{TRACE;

}

/*-------------------------------------------------------------------------*/

CPatchSetFileEngine::~CPatchSetFileEngine()
{TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPatchSetFileEngine::Start( const TFileList& fileList            ,
                            const CORE::CString& localRoot       ,
                            const CORE::CString& tempStorageRoot )

{TRACE;

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
{TRACE;

    if ( !m_stopSignalGiven && m_isActive )
    {
        // Get our current file
        const TFileEntry* curFile = &((*m_fileList)[ m_curFileIndex ]);
        
        // Create a path string to the file
        CORE::CString filePath = m_localRoot;
        CORE::AppendToPath( filePath, curFile->name );
        
        // Check if the file exists
        if ( 0 != CORE::File_Exists( filePath.C_String() ) )
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
                            // We can now proceed to the next file if there is any
                            if ( m_curFileIndex+1 < m_fileList->size() )
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
                }  
            }
        }    
        
        // If we get here then the file was found but it is not up-to-date or
        // the file does not even exist locally, either way,.. we have to update it
        CORE::CString tmpFilePath = m_tempStorageRoot;
        CORE::AppendToPath( tmpFilePath, curFile->name );            
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
            NotifyObservers( FileStorageErrorEvent );
            m_stopSignalGiven = false;
            m_isActive = false;                
            return false;
        }       
    }
    else
    {
        NotifyObservers( FileListProcessingAbortedEvent );
        m_stopSignalGiven = false;
        m_isActive = false;
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/
    
void
CPatchSetFileEngine::Stop( void )
{TRACE;

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
{TRACE;

    return m_isActive;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetFileEngine::TryNextFileLocation( void )
{TRACE;

    m_fileAccess->Close();

    const TFileEntry* curFile = &(*m_fileList)[ m_curFileIndex ];
    do
    {
        // if we get here then we where unable to retrieve the file using the given URL
        // We will try the next one,.. if there is a next one
        if ( m_curFileLocIndex+1 < curFile->fileLocations.size() )
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
    while ( m_curFileLocIndex < curFile->fileLocations.size() );
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetFileEngine::ProceedToNextFile( void )
{TRACE;

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
{TRACE;

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