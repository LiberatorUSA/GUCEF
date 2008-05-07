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
                TAbstractArchiveFactory::TKeySet::iterator n = keySet.find( fileExtention );
                if ( n != keySet.end() )
                {
                    // Found a compatible type,.. create an archive for the type
                    m_datalock.Lock();
                    CIArchive* archive = m_abstractArchiveFactory.Create( fileExtention );
                    if ( NULL != archive )
                    {
                        // Try to load from the resource
                        if ( archive->LoadArchive( archiveName ,
                                                   (*i)        ,
                                                   writeable   ) )
                        {
                            // Successfully loaded/linked the archive
                            // We will add it to our mount list
                            TMountEntry archiveEntry;
                            archiveEntry.path = (*i);
                            archiveEntry.abspath = archiveEntry.path;
                            archiveEntry.writeable = writeable;
                            archiveEntry.archive = archive;
                            archiveEntry.mountPath = archiveEntry.path;
                            m_mountList.push_back( archiveEntry );
                        }
                        else
                        {
                            delete archive;
                        }
                    }
                    m_datalock.Unlock();
                }
                ++i;
            }
                                         
        }
        
        m_datalock.Lock();
        m_mountList.push_back( mountEntry );        
        m_datalock.Unlock();
    }                
}

/*-------------------------------------------------------------------------*/

void 
CVFS::SetMemloadSize( UInt32 bytesize )
{GUCEF_TRACE;

    m_datalock.Lock();
    _maxmemloadsize = bytesize;
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
        const CORE::CDataNode::TNodeAtt* att2;
        bool writeable;
        
        while( i != n->ConstEnd() )
        {
            c = (*i);
            if ( c->GetName() == vfsroot )
            {
                att = c->GetAttribute( "path" );
                att2 = c->GetAttribute( "writeable" );
                if ( att )
                {
                        writeable = false;
                        if ( att2 ) writeable = CORE::StringToBool( att2->value );
                        
                        AddRoot( att->value ,
                                 att->value ,
                                 writeable  );
                }
            }
            ++i;
        }
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
        mountLink.mountEntry->archive->GetList( outputList, mountLink.remainder );
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
