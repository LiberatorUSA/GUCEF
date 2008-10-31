/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef DVFILEUTILS_H
#include "dvfileutils.h"                /* Needed for dir itteration */
#define DVFILEUTILS_H
#endif /* DVFILEUTILS_H ? */

#ifndef DVCPPSTRINGUTILS_H 
#include "dvcppstringutils.h"           /* Needed for AppendToPath() */
#define DVCPPSTRINGUTILS_H
#endif /* DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef CMFILEACCESS_H
#include "CMFileAccess.h"
#define CMFILEACCESS_H
#endif /* CMFILEACCESS_H ? */

#ifndef CFILEACCESS_H
#include "CFileAccess.h"
#define CFILEACCESS_H
#endif /* CFILEACCESS_H ? */

#ifndef CDATANODE_H
#include "CDataNode.h"          /* node for data storage */
#define CDATANODE_H
#endif /* CDATANODE_H ? */

#ifndef DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */ 
#define DVCPPSTRINGUTILS_H
#endif /* DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_VFS_CFILESYSTEMARCHIVE_H
#include "gucefVFS_CFileSystemArchive.h"
#define GUCEF_VFS_CFILESYSTEMARCHIVE_H
#endif /* GUCEF_VFS_CFILESYSTEMARCHIVE_H ? */

#include "CVFS.h"                /* definition of the file implemented here */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CVFS* CVFS::_instance = NULL;
MT::CMutex CVFS::m_datalock;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CVFS::CVFS( void )
    : CIConfigurable( true ) ,
      _maxmemloadsize( 1024 ) ,
      m_mountList()
{GUCEF_TRACE;
       
}
       
/*-------------------------------------------------------------------------*/
        
CVFS::~CVFS()
{GUCEF_TRACE;
        
}

/*-------------------------------------------------------------------------*/

CVFS* 
CVFS::Instance( void )
{GUCEF_TRACE;
        
    m_datalock.Lock();
    if ( NULL == _instance ) 
    {
        _instance = new CVFS();
    }
    m_datalock.Unlock();
    return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CVFS::Deinstance( void )
{GUCEF_TRACE;
        
    m_datalock.Lock();
    delete _instance;
    _instance = NULL;
    m_datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

CVFS::CVFSHandlePtr 
CVFS::GetFile( const CORE::CString& file          ,
               const char* mode /* = "rb" */      ,
               const bool overwrite /* = false */ )
{GUCEF_TRACE;

    m_datalock.Lock();

    CString filepath( file.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );
    bool fileMustExist = *mode == 'r';    
    
    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( filepath, mountLinks );

    // Find the file in the available archives
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        
        bool tryToGetFile = true;
        if ( fileMustExist )
        {
            if ( !mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
            {
                tryToGetFile = false;
            }
        }
        
        if ( tryToGetFile )
        {
            CVFSHandlePtr filePtr = mountLink.mountEntry->archive->GetFile( mountLink.remainder ,  
                                                                            mode                ,
                                                                            _maxmemloadsize     ,
                                                                            overwrite           );
            m_datalock.Unlock();
            return filePtr;        
        }

        ++i;
    }
    
    // Unable to load file
    m_datalock.Unlock();                
    return CVFSHandlePtr();                              
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MountArchive( const CString& archiveName  ,
                    const CString& archivePath  ,
                    const bool writeableRequest )
{GUCEF_TRACE;

    // Mount the archive using the archive path
    return MountArchive( archiveName      ,
                         archivePath      ,
                         archivePath      ,
                         writeableRequest );
}

/*-------------------------------------------------------------------------*/
                  
bool
CVFS::MountArchive( const CString& archiveName  ,
                    const CString& archivePath  ,
                    const CString& mountPoint   ,
                    const bool writeableRequest )
{GUCEF_TRACE;

    // Use the archive file extension as the archive type
    CString archiveType( archivePath.SubstrToChar( '.', false ) );
    return MountArchive( archiveName      ,
                         archivePath      ,
                         archiveType      ,
                         mountPoint       ,
                         writeableRequest );
}

/*-------------------------------------------------------------------------*/
    
bool
CVFS::MountArchive( const CString& archiveName  ,
                    const CString& archivePath  ,
                    const CString& archiveType  ,
                    const CString& mountPoint   ,
                    const bool writeableRequest )
{GUCEF_TRACE;

    m_datalock.Lock();
    
    CString actualArchivePath;
    if( GetActualFilePath( archivePath       , 
                           actualArchivePath ) )
    {
        // Found a compatible type,.. create an archive for the type
        m_datalock.Lock();
        CIArchive* archive = m_abstractArchiveFactory.Create( archiveType );
        if ( NULL != archive )
        {
            // Try to load from the resource
            if ( archive->LoadArchive( archiveName       ,
                                       actualArchivePath ,
                                       writeableRequest  ) )
            {
                // Successfully loaded/linked the archive
                // We will add it to our mount list
                TMountEntry archiveEntry;
                archiveEntry.path = actualArchivePath;
                archiveEntry.abspath = actualArchivePath;
                archiveEntry.writeable = writeableRequest;
                archiveEntry.archive = archive;
                archiveEntry.mountPath = mountPoint;
                
                m_mountList.push_back( archiveEntry );
                
                m_datalock.Unlock();
                return true;
            }
            else
            {
                delete archive;
            }
        }
        m_datalock.Unlock();
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::IsMountedArchive( const CString& location ) const
{GUCEF_TRACE;

    m_datalock.Lock();
    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& archiveEntry = (*i);
        if ( archiveEntry.mountPath == location )
        {
            m_datalock.Unlock();
            return true;
        }
        ++i;
    }    
    
    m_datalock.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

void
CVFS::GetMountedArchiveList( const CString& location ,
                             const CString& filter   ,
                             const bool recursive    ,
                             TStringSet& outputList  ) const
{GUCEF_TRACE;

    m_datalock.Lock();
    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& archiveEntry = (*i);
        
        if ( archiveEntry.mountPath.Length() > 0 )
        {        
            // Check if the location is a root for the mount path
            Int32 subSection = archiveEntry.mountPath.HasSubstr( location, true );
            if ( 0 == subSection )
            {
                // We now know the archive is mounted in either the root of "location"
                // or a sub directory
                if ( !recursive )
                {
                    // Check if we have multiple subdirs beyond the "location" to get to
                    // the archive. If so then we cannot add this archive because recursive 
                    // searching is not allowed.
                    CString remainder = archiveEntry.mountPath.CutChars( subSection, true );
                    if ( remainder.Length() > 0 )
                    {
                        // make sure we are using a single directory seperator scheme
                        remainder = remainder.ReplaceChar( '\\', '/' );
                        
                        // Dont count a starting directory seperator
                        if ( remainder[ 0 ] == '/' )
                        {
                            remainder = remainder.CutChars( 1, true );
                        }
                        
                        Int32 dirDelimter = remainder.HasChar( '/', true );
                        if ( dirDelimter > -1 )
                        {
                            // We found a directory seperator so now me must check if it happens 
                            // to be the last character
                            if ( remainder.Length() != dirDelimter+1 )
                            {
                                // The directory seperator was not the last character so we have multiple
                                // sub-dirs which is not allowed, we cannot add this item
                                ++i;
                                continue;
                            }
                        }
                    }
                }
                
                // Now we must validate the filename with the given filter
                CString filename = GUCEF::CORE::ExtractFilename( archiveEntry.mountPath );
                if ( FilterValidation( filename ,
                                       filter   ) )
                {
                    // Everything checks out, we have found a mounted archive that meets 
                    // the criterea so we add it to the list
                    outputList.insert( archiveEntry.mountPath );
                }
            }
        }
        ++i;
    }
    m_datalock.Unlock();    
}

/*-------------------------------------------------------------------------*/

bool
CVFS::GetActualFilePath( const CString& file ,
                         CString& path       ) const
{GUCEF_TRACE;

    m_datalock.Lock();
    
    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( file, mountLinks );    

    // Find the file in the available archives
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
        {
            path = mountLink.mountEntry->abspath;
            CORE::AppendToPath( path, mountLink.remainder );
             
            m_datalock.Unlock();
            return true;
        }
        ++i;
    }
    
    m_datalock.Unlock();                
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::FileExists( const CString& file ) const
{GUCEF_TRACE;       

    CString actualPath;
    return GetActualFilePath( file       , 
                              actualPath );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::UnmountArchiveByName( const CString& archiveName )
{GUCEF_TRACE;

    m_datalock.Lock();
    TMountVector::iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        TMountEntry& mountEntry = (*i);
        if ( mountEntry.archive->GetArchiveName() == archiveName )
        {
            if ( mountEntry.archive->UnloadArchive() )
            {
                m_mountList.erase( i );
                m_datalock.Unlock();
                return true;
            }
            return false;         
        }
        ++i;
    }
    
    m_datalock.Unlock();
    return false;        
}

/*-------------------------------------------------------------------------*/

void
CVFS::AddRoot( const CORE::CString& rootpath              ,
               const CString& archiveName                 ,
               const bool writeable                       ,
               const bool autoMountArchives /* = false */ )
{GUCEF_TRACE;

    if ( rootpath.Length() > 0 )
    {        
        TMountEntry mountEntry;
        mountEntry.path = rootpath;
        mountEntry.abspath = CORE::RelativePath( rootpath );
        mountEntry.writeable = writeable;
        
        if ( mountEntry.abspath.Length() == 0 )
        {
                mountEntry.abspath = rootpath;
        }
        
        mountEntry.archive = new CFileSystemArchive( archiveName        ,
                                                     mountEntry.abspath ,
                                                     writeable          );

        if ( autoMountArchives )
        {
            // Get a list of all constructable archive types
            m_datalock.Lock();
            TAbstractArchiveFactory::TKeySet keySet;
            m_abstractArchiveFactory.ObtainKeySet( keySet );
            m_datalock.Unlock();
            
            // Get a list of all files from the root onward
            TStringSet files;
            mountEntry.archive->GetList( files ,
                                         ""    ,
                                         true  ,
                                         true  );
                                         
            // Find mountable types
            TStringSet::iterator i = files.begin();
            while ( i != files.end() )
            {
                // Mount based on archive file extention type
                CString fileExtention = CORE::ExtractFileExtention( (*i) );
                if ( !fileExtention.IsNULLOrEmpty() )
                {
                    TAbstractArchiveFactory::TKeySet::iterator n = keySet.find( fileExtention );
                    if ( n != keySet.end() )
                    {
                        // Found a compatible type,.. create an archive for the type
                        m_datalock.Lock();
                        CIArchive* archive = m_abstractArchiveFactory.Create( fileExtention );
                        if ( NULL != archive )
                        {
                            // Try to load from the resource
                            if ( archive->LoadArchive( (*i)        ,
                                                       (*i)        ,
                                                       writeable   ) )
                            {
                                // Successfully loaded/linked the archive
                                // We will add it to our mount list
                                TMountEntry archiveEntry;
                                archiveEntry.abspath = (*i);
                                
                                // Get the path where the archive is located relative to the root mount
                                archiveEntry.path = archiveEntry.abspath.CutChars( mountEntry.abspath.Length(), true );
                                
                                // Git rid of any directory seperator prefix
                                if ( archiveEntry.path.Length() > 0 )
                                {
                                    if ( ( archiveEntry.path[ 0 ] == '\\' ) ||
                                         ( archiveEntry.path[ 0 ] == '/' )   )
                                    {
                                        archiveEntry.path = archiveEntry.path.CutChars( 1, true );
                                    }
                                }
                                archiveEntry.writeable = writeable;
                                archiveEntry.archive = archive;
                                archiveEntry.mountPath = archiveEntry.path;
                                m_mountList.push_back( archiveEntry );
                                
                                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS archive mounted automatically with name " + (*i) );
                            }
                            else
                            {
                                delete archive;
                            }
                        }
                        m_datalock.Unlock();
                    }
                }
                ++i;
            }
                                         
        }
        
        m_datalock.Lock();
        m_mountList.push_back( mountEntry );                
        m_datalock.Unlock();
        
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS root added as a mount with name " + archiveName );
    }                
}

/*-------------------------------------------------------------------------*/

void 
CVFS::SetMemloadSize( UInt32 bytesize )
{GUCEF_TRACE;

    m_datalock.Lock();
    _maxmemloadsize = bytesize;
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS maximum memory load size set to " + CORE::UInt32ToString( bytesize ) + " Bytes" );
    m_datalock.Unlock();
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CVFS::GetMemloadSize( void ) const
{GUCEF_TRACE;

    return _maxmemloadsize;
}

/*-------------------------------------------------------------------------*/

bool 
CVFS::SaveConfig( CORE::CDataNode& tree )
{GUCEF_TRACE;

    CORE::CDataNode* n = tree.Structure( "GUCEF%VFS%CVFS" ,
                                         '%'              );
    CORE::CString maxmem;
    maxmem.SetInt( _maxmemloadsize );                                        
    n->SetAttribute( "maxmemload" ,
                     maxmem       );
                     
    n->DelSubTree();                         
    UInt32 count = (UInt32) m_mountList.size();                         
    CORE::CDataNode pathnode( "vfsroot" );
    for ( UInt32 i=0; i<count; ++i )
    {
        pathnode.SetAttribute( "path", m_mountList[ i ].path );
        if ( m_mountList[ i ].archive->IsWriteable() )
        {
            pathnode.SetAttribute( "writeable", "true" );
        }
        else
        {
            pathnode.SetAttribute( "writeable", "false" );
        }                        
        n->AddChild( pathnode );                                 
    }                                 
    
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS configuration successfully saved" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CVFS::LoadConfig( const CORE::CDataNode& tree )
{GUCEF_TRACE;

    const CORE::CDataNode* n = tree.Search( "GUCEF%VFS%CVFS" ,
                                            '%'              ,
                                            false            );
                                             
    if ( n )
    {
        const CORE::CDataNode::TNodeAtt* att = n->GetAttribute( "maxmemload" );
        if ( att )
        {
            SetMemloadSize( att->value.GetInt() );
        }
        
        CORE::CString vfsroot( "vfsroot" );
        const CORE::CDataNode* c;
        CORE::CDataNode::const_iterator i = n->ConstBegin();
        bool writeable;
        
        while( i != n->ConstEnd() )
        {
            c = (*i);
            if ( c->GetName() == vfsroot )
            {
                CString path = c->GetAttributeValue( "Path" );
                if ( !path.IsNULLOrEmpty() )
                {
                    CString archiveName = c->GetAttributeValue( "ArchiveName" );
                    if ( archiveName.IsNULLOrEmpty() )
                    {
                        archiveName = path;
                    }
                    
                    bool mountArchives = CORE::StringToBool( c->GetAttributeValue( "MountArchives" ) ); 
                    writeable = CORE::StringToBool( c->GetAttributeValue( "Writeable" ) );                        
                    AddRoot( path          ,
                             archiveName   ,
                             writeable     ,
                             mountArchives );
                }
            }
            ++i;
        }
        
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS configuration successfully loaded" );
        
        return true;
    }                                                                                                  
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CVFS::FilterValidation( const CORE::CString& filename , 
                        const CORE::CString& filter   )
{GUCEF_TRACE;

    if ( filter.Length() > 0 )
    {
        if ( filter[ 0 ] == '*' )
        {
            if ( filter.Length() == 1 )
            {
                    return true;
            }
            
            if ( filename.HasSubstr( filter.C_String()+1, false ) == 0 )
            {
                    return true;
            }
            return false;
        }
        else
        {
            CString subfilter;
            subfilter.Set( filter.C_String(), filter.Length()-1 );                        
            if ( filename.HasSubstr( subfilter, true ) == 0 )
            {
                    return true;
            }
            return false;
        }                                        
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void
CVFS::GetEligableMounts( const CString& location                ,
                         TConstMountLinkVector& mountLinkVector ) const
{GUCEF_TRACE;
    
    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& mountEntry = (*i);
        if ( mountEntry.mountPath.Length() > 0 )
        {
            // Check to see if the mount path is the starting sub string of the location
            if ( location.HasSubstr( mountEntry.mountPath, true ) == 0 )
            {
                TConstMountLink mountLink;
                mountLink.remainder = location.CutChars( mountEntry.mountPath.Length(), true );
                
                // make sure the remainder does not have a path seperator prefix
                if ( mountLink.remainder.Length() > 0 )
                {
                    if ( ( mountLink.remainder[ 0 ] == '/' ) ||
                         ( mountLink.remainder[ 0 ] == '\\' ) )
                    {
                        // remove the dir seperator prefix
                        mountLink.remainder = mountLink.remainder.CutChars( 1, true );
                    }
                }                
                
                mountLink.mountEntry = &mountEntry;
                mountLinkVector.push_back( mountLink );
            }
        }
        else
        {
            TConstMountLink mountLink;
            mountLink.remainder = location;
            mountLink.mountEntry = &mountEntry;
            mountLinkVector.push_back( mountLink );
        }
        ++i;
    }    
}

/*-------------------------------------------------------------------------*/

void 
CVFS::GetList( TStringSet& outputList                   ,
               const CString& location                  ,
               bool recursive             /* = false */ ,
               bool includePathInFilename /* = false */ ,
               const CString& filter /* = "" */         ,
               bool addFiles /* = true */               ,
               bool addDirs /* = false */               ) const
{GUCEF_TRACE;
        
    m_datalock.Lock();
    
    // Switch dir separator chars if needed
    CString path( location.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );
    
    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, mountLinks ); 
        
    // Get a list from each mount
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        mountLink.mountEntry->archive->GetList( outputList            , 
                                                mountLink.remainder   ,
                                                recursive             ,
                                                includePathInFilename ,
                                                filter                ,
                                                addFiles              ,
                                                addDirs               );
        ++i;
    }
    
    m_datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CVFS::GetList( CORE::CDataNode& outputDataTree        ,
               const CORE::CString& location          , 
               bool recursive  /* = false */          ,
               const CORE::CString& filter /* = "" */ ,
               const bool addHash /* = false */       ) const
{GUCEF_TRACE;

    // First we get a list of files and their path
    TStringSet outputList;
    GetList( outputList ,
             location   ,
             recursive  ,
             true       ,
             filter     ,
             true       ,
             false      );
             
    // First we clear the tree
    outputDataTree.Clear();
    
    // Now we build our data tree using the list we obtained
    TStringSet::iterator i = outputList.begin();
    while ( i != outputList.end() )
    {
        // First we build the path structure in the data tree
        CString path = (*i).SubstrToChar( DIRSEPCHAR, false );
        CORE::CDataNode* pathRootNode = outputDataTree.Structure( "DIR", "Name", path, DIRSEPCHAR );
        
        // Extract the filename from the path
        CORE::CString filename( (*i).C_String() + path.Length() , 
                                (*i).Length() - path.Length()   );
        
        // Create the file node and set all the attributes
        CORE::CDataNode fileNode( "FILE" );
        fileNode.SetAttribute( "Name", filename );
        fileNode.SetAttribute( "Size", CORE::UInt32ToString( GetFileSize( (*i) ) ) );
        
        if ( addHash )
        {
            // Obtain the hash for the file, this can be a lengthy operation
            fileNode.SetAttribute( "Hash", GetFileHash( (*i) ) );
        }
        
        // Create a copy child node thus storing the file properties in the tree
        pathRootNode->AddChild( fileNode );
        
        ++i;
    }    
}

/*-------------------------------------------------------------------------*/

CORE::CString
CVFS::GetFileHash( const CORE::CString& file ) const
{GUCEF_TRACE;

    m_datalock.Lock();

    // Switch dir separator chars if needed
    CString path( file.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );
    
    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, mountLinks );
    
    // Search for a file and then get the hash
    CString hash;
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
        {
            CString hash = mountLink.mountEntry->archive->GetFileHash( mountLink.remainder );
            m_datalock.Unlock();
            return hash;
        }
        ++i;
    }    
    
    m_datalock.Unlock();
    return CString();
}

/*-------------------------------------------------------------------------*/

UInt32
CVFS::GetFileSize( const CORE::CString& file ) const
{GUCEF_TRACE;

    m_datalock.Lock();

    // Switch dir separator chars if needed
    CString path( file.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );
    
    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, mountLinks );
    
    // Search for a file and then get its size
    CString hash;
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
        {
            UInt32 fileSize = mountLink.mountEntry->archive->GetFileSize( mountLink.remainder );
            m_datalock.Unlock();
            return fileSize;
        }
        ++i;
    }    
    
    m_datalock.Unlock();
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CVFS::RegisterArchiveFactory( const CString& archiveType      ,
                              TArchiveFactory& archiveFactory )
{GUCEF_TRACE;

    m_abstractArchiveFactory.RegisterConcreteFactory( archiveType     ,
                                                      &archiveFactory );
                                                      
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Registered VFS archive factory for type \"" + archiveType + "\"" ); 
}

/*-------------------------------------------------------------------------*/

void
CVFS::UnregisterArchiveFactory( const CString& archiveType )
{GUCEF_TRACE;

    m_abstractArchiveFactory.UnregisterConcreteFactory( archiveType );
    
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Unregistered VFS archive factory for type \"" + archiveType + "\"" );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
