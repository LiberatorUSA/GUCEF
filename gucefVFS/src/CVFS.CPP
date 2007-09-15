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

#ifndef GUCEFVFS_MACROS_H
#include "gucefVFS_macros.h"         /* often used gucefVFS macros */
#define GUCEFVFS_MACROS_H
#endif /* GUCEFVFS_MACROS_H ? */

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
          m_rootlist()
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
        if ( !_instance ) 
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

CVFSHandle*
CVFS::LoadFromDisk( const CORE::CString& file          ,
                    UInt32& errorcode                  ,
                    const char* mode /* = "rb" */      , 
                    const bool overwrite /* = false */ )
{GUCEF_TRACE;

    CORE::CString filepath;
    const TRootDir* entry = NULL;                
    bool needwriteable( ( strchr( mode, 'a' ) != NULL ) || ( strchr( mode, 'w' ) != NULL ) || ( strchr( mode, '+' ) != NULL )); 

    // Check if we can perform read-only access which allows us 
    // to share the resource
    if ( ( strcmp( mode, "rb" ) == 0 ) ||
         ( strcmp( mode, "r" ) == 0 )   )
    {
        // We try to locate the resource in one of the roots
        for ( UInt32 i=0; i<m_rootlist.size(); ++i )
        {
            // Create a file path for this root
            entry = &m_rootlist[ i ];
            filepath = entry->abspath;
            CORE::AppendToPath( filepath, file );
            
            // Check our cache for this file
            TFileMemCache::iterator n = m_diskCacheList.find( filepath );
            if ( n != m_diskCacheList.end() )
            {
                // We found the file in our cache, we will link to the existing buffer.
                TDynamicBufferPtr bufferPtr = (*n).second;
                return new CVFSHandle( new CORE::CDynamicBufferAccess( bufferPtr.GetPointer() , 
                                                                       false                  ) ,
                                       file                                                     ,
                                       filepath                                                 ,
                                       bufferPtr                                                );
            }
        }
    }

    // We try to locate the resource in one of the roots
    for ( UInt32 i=0; i<m_rootlist.size(); ++i )
    {
        // Create a file path for this root
        entry = &m_rootlist[ i ];
        filepath = entry->abspath;
        CORE::AppendToPath( filepath, file );                
        
        // Check if the file even exists on disk
        bool exists( CORE::File_Exists( filepath.C_String() ) > 0 );
        if ( exists )
        {                        
            // Attempt to get access to the file
            CORE::CIOAccess* fa = new CORE::CFileAccess( filepath, mode );
            if ( !fa->IsValid() )
            {
                // try a different root
                delete fa;
                continue;
            }
            
            if ( ( strcmp( mode, "rb" ) == 0 ) ||
                 ( strcmp( mode, "r" ) == 0 )   )
            {
                // check if we can load the file into memory
                UInt32 fsize = CORE::Filesize( filepath.C_String() );
                if ( fsize <= _maxmemloadsize )
                {
                    // Create the memory buffer
                    TDynamicBufferPtr bufferPtr( new CORE::CDynamicBuffer() );
                    CORE::CIOAccess* bufferAccess = new CORE::CDynamicBufferAccess( bufferPtr.GetPointer(), false );
                    
                    // Copy the file into the buffer
                    if ( fsize == bufferAccess->Write( *fa ) )
                    {
                        delete fa;
                        fa = NULL;
                        
                        // reset the carat so the user can access the file from the beginning
                        bufferAccess->Setpos( 0 );
                        
                        // Add the file to our buffered files list
                        m_diskCacheList.insert( std::pair< CORE::CString, TDynamicBufferPtr >( filepath, bufferPtr ) );
                        
                        // return the file handle
                        return new CVFSHandle( bufferAccess ,
                                               file         ,
                                               filepath     ,
                                               bufferPtr    );                  
                    }
                    else
                    {
                        // Something went wrong while trying to load the file into the buffer
                        // try a different root
                        fa->Setpos( 0 );
                        delete bufferAccess;
                    }
                }
                
                // return the file handle
                return new CVFSHandle( fa                  ,
                                       file                ,
                                       filepath            ,
                                       TDynamicBufferPtr() );                
            }
        }
    }
    
    return NULL;                                       
}

/*-------------------------------------------------------------------------*/

CVFS::CVFSHandlePtr 
CVFS::GetFile( const CORE::CString& file          ,
               UInt32& errorcode                  ,
               const char* mode /* = "rb" */      ,
               const bool overwrite /* = false */ )
{GUCEF_TRACE;
        
        CORE::CString filepath( file.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );
        
        m_datalock.Lock();
        
        /* 
         *      Try to locate the file using the rootdirs as 
         *      a prefix
         */
        CVFSHandle* fh = LoadFromDisk( filepath  ,
                                       errorcode ,
                                       mode      ,
                                       overwrite );

        m_datalock.Unlock();
        
        if ( fh != NULL )
        {
            return CVFSHandlePtr( fh, this );
        }

        /*
         *      If we get here then the file was not found in one 
         *      of the rootdirs. or in any of the packfiles
         */
        return CVFSHandlePtr();                                
}

/*-------------------------------------------------------------------------*/

void
CVFS::DestroyObject( CVFSHandle* vfshandle )
{GUCEF_TRACE;

    if ( vfshandle != NULL )
    {
        m_datalock.Lock();

        delete vfshandle->m_fileAccess;
        vfshandle->m_fileAccess = NULL;
        
        if ( vfshandle->IsLoadedInMemory() )
        {
            TFileMemCache::iterator n = m_diskCacheList.find( vfshandle->GetFilePath() );
            if ( n != m_diskCacheList.end() )
            {
                // We found the file in our cache, we will link to the existing buffer.
                if ( (*n).second.GetReferenceCount() == 1 )
                {
                    // nobody else is using the memory buffer anymore
                    m_diskCacheList.erase( n );
                }
            }            
        }
        
        delete vfshandle;

        m_datalock.Unlock();
    }
}

/*-------------------------------------------------------------------------*/

bool
CVFS::FileExists( const CORE::CString& file ) const
{GUCEF_TRACE;       

    /*
     *      Attempt to find the file on disk
     */
    CORE::CString filepath;
    for ( UInt32 i=0; i<m_rootlist.size(); ++i )
    {
        filepath = m_rootlist[ i ].abspath;
        CORE::AppendToPath( filepath, file );
        if ( CORE::File_Exists( filepath.C_String() ) )
        {
                return true;
        }
    }
                    
    return false;
}

/*-------------------------------------------------------------------------*/

void
CVFS::AddRoot( const CORE::CString& rootpath ,
               const bool writeable          )
{GUCEF_TRACE;

    if ( rootpath.Length() > 0 )
    {        
        m_datalock.Lock();
        
        TRootDir newroot;
        newroot.abspath = CORE::RelativePath( rootpath );
        newroot.path = rootpath;
        newroot.writeable = writeable;
        
        if ( !newroot.abspath.Length() )
        {
                newroot.abspath = rootpath;
        }
        
        TRootDir* entry;
        for ( UInt32 i=0; i<m_rootlist.size(); ++i )
        {
            entry = &m_rootlist[ i ];
            if ( entry->path.Lowercase() == rootpath.Lowercase() )
            {
                /*
                 *      rootpath already exists                                 
                 */
                m_datalock.Unlock();
                return; 
            }
        }
        
        m_rootlist.push_back( newroot );       
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
    UInt32 count = (UInt32) m_rootlist.size();                         
    CORE::CDataNode pathnode( "vfsroot" );
    for ( UInt32 i=0; i<count; ++i )
    {
        pathnode.SetAttribute( "path", m_rootlist[ i ].path );
        if ( m_rootlist[ i ].writeable )
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
                                 writeable );
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
                        const CORE::CString& filter   ) const
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
            CORE::CString subfilter;
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
CVFS::GetListFromRoot( const CORE::CString& root     ,
                       bool recursive                ,
                       bool includePathInFilename    ,
                       const CORE::CString& filter   ,
                       TStringSet& outputList        ,
                       bool addFiles                 ,
                       bool addDirs                  ) const
{GUCEF_TRACE;

    CORE::CString filename;
    struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( root.C_String() );
    if ( did != NULL )
    {                
        /*
         *      Iterate the dir content adding the file entries from
         *      the dir to the list
         */
        do
        {
            if ( CORE::DI_Is_It_A_File( did ) != 0 )
            {                           
                if ( addFiles )
                {
                    filename = CORE::DI_Name( did );     
                    if ( filename != '.' && filename != ".." )
                    {
                        if ( FilterValidation( filename ,
                                               filter   ) )
                        {
                            if ( !includePathInFilename )
                            {
                                outputList.insert( filename );
                            }
                            else
                            {
                                CORE::CString filePath( root );
                                CORE::AppendToPath( filePath, filename );
                                outputList.insert( filePath );                        
                            }
                        }
                    }
                }                                        
            }
            else
            {
                if ( addDirs )
                {
                    CORE::CString dirName = CORE::DI_Name( did );
                    if ( dirName != '.' && dirName != ".." )
                    {
                        outputList.insert( dirName );
                    }
                }
                
                if ( recursive )
                {
                    /*
                     *      Build the path including the sub-dir
                     */
                    CORE::CString subdir( root );
                    CORE::AppendToPath( subdir, CORE::DI_Name( did ) );
                    
                    /*
                     *      Recursively process the sub-dir
                     */
                    GetListFromRoot( subdir                ,
                                     recursive             ,
                                     includePathInFilename ,
                                     filter                ,
                                     outputList            ,
                                     true                  ,
                                     false                 ); 
                }
            }        
        }
        while ( CORE::DI_Next_Dir_Entry( did ) );
        
        CORE::DI_Cleanup( did );
    }                
}

/*-------------------------------------------------------------------------*/

void 
CVFS::GetList( TStringSet& outputList                   ,
               const CORE::CString& location            ,
               bool recursive             /* = false */ ,
               bool includePathInFilename /* = false */ ,
               const CORE::CString& filter /* = "" */   ,
               bool addFiles /* = true */               ,
               bool addDirs /* = false */               ) const
{GUCEF_TRACE;
        
    m_datalock.Lock();
    
    /*
     *      Switch dir separator chars if needed
     */
    CORE::CString loc( location.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );              
    
    /*
     *      Get a file list from each (root+location) path
     */ 
    for ( UInt32 i=0; i<m_rootlist.size(); ++i )
    {
        /*
         *      Make the combo path string
         */
        CORE::CString rootdir( m_rootlist[ i ].abspath );
        AppendToPath( rootdir, location );                 

        /*
         *      Process the root
         */
        GetListFromRoot( rootdir               ,
                         recursive             ,
                         includePathInFilename ,
                         filter                ,
                         outputList            ,
                         addFiles              ,
                         addDirs               );
    }
    
    /*
     *      Now we do the same for the packs
     *  @TODO
     */
                  
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
        CORE::CString path = (*i).SubstrToChar( DIRSEPCHAR, false );
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
    
    CORE::CString filepath;
    for ( UInt32 i=0; i<m_rootlist.size(); ++i )
    {
        filepath = m_rootlist[ i ].abspath;
        CORE::AppendToPath( filepath, file );
        if ( CORE::File_Exists( filepath.C_String() ) != 0 )
        {
            CORE::CFileAccess fileAccess( filepath );
            
            UInt8 md5Hash[ 16 ];
            if ( CORE::md5frommfile( fileAccess.CStyleAccess() , 
                                     md5Hash                   ) == 1 )
            {
                m_datalock.Unlock();
                return CORE::MD5ToString( md5Hash );
            }
        }
    }
    
    m_datalock.Unlock();
    return CORE::CString();
}

/*-------------------------------------------------------------------------*/

UInt32
CVFS::GetFileSize( const CORE::CString& file ) const
{GUCEF_TRACE;

    m_datalock.Lock();
    
    CORE::CString filepath;
    for ( UInt32 i=0; i<m_rootlist.size(); ++i )
    {
        filepath = m_rootlist[ i ].abspath;
        CORE::AppendToPath( filepath, file );
        if ( CORE::File_Exists( filepath.C_String() ) != 0 )
        {
            m_datalock.Unlock();
            return CORE::Filesize( filepath.C_String() );
        }
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
