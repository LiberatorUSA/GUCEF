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
{TRACE;
       
}
       
/*-------------------------------------------------------------------------*/
        
CVFS::~CVFS()
{TRACE;        
}

/*-------------------------------------------------------------------------*/

CVFS* 
CVFS::Instance( void )
{TRACE;
        DEBUGOUTPUT( "CVFS::Instance( void ) const" );
        
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
{TRACE;
        DEBUGOUTPUT( "CVFS::Deinstance( void )" );
        
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
{TRACE;

    DEBUGOUTPUT( "CVFS::LoadFromDisk( const CORE::CString& file )" );
    
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
{TRACE;
        DEBUGOUTPUT( "CVFS::GetFile( const CORE::CString& file, UInt32& errorcode )" );
        DEBUGOUTPUTss( "CVFS: Attempting to load file: ", file.C_String() );
        
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
        DEBUGOUTPUTss( "Cannot locate file: ", file.C_String() ); 
        return CVFSHandlePtr();                                
}

/*-------------------------------------------------------------------------*/

void
CVFS::DestroyObject( CVFSHandle* vfshandle )
{TRACE;

    DEBUGOUTPUT( "CVFS::Unload( CVFSHandle* vfshandle )" );
    
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
{TRACE;       

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
{TRACE;

    DEBUGOUTPUT( "CVFS::AddRoot( const CORE::CString& rootpath )" );
    
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
        DEBUGOUTPUTss( "CVFS: VFS root added: ", rootpath.C_String() );
                            
        m_datalock.Unlock();
    }                
}

/*-------------------------------------------------------------------------*/

void 
CVFS::SetMemloadSize( UInt32 bytesize )
{TRACE;
    
    DEBUGOUTPUT( "CVFS::SetMemloadSize( UInt32 bytesize )" );
    
    m_datalock.Lock();
    _maxmemloadsize = bytesize;
    m_datalock.Unlock();
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CVFS::GetMemloadSize( void ) const
{TRACE;

    DEBUGOUTPUT( "CVFS::GetMemloadSize( void ) const" );
    
    return _maxmemloadsize;
}

/*-------------------------------------------------------------------------*/

bool 
CVFS::SaveConfig( CORE::CDataNode& tree )
{TRACE;

    DEBUGOUTPUT( "CVFS::SaveConfig( CORE::CDataNode* tree )" );
    
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
{TRACE;

    DEBUGOUTPUT( "CVFS::LoadConfig( const CORE::CDataNode* tree )" );
    
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
{TRACE;

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
                       const CORE::CString& filter   ,
                       TStringList& outputList       ) const
{TRACE;

    DEBUGOUTPUTssss( "Getting file list with filter ", filter.C_String(), " for dir: ", root.C_String() );

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
                filename = CORE::DI_Name( did );     
                if ( FilterValidation( filename ,
                                       filter   ) )
                {
                    outputList.push_back( filename );
                }                                        
            }
            else
            {
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
                    GetListFromRoot( subdir     ,
                                     recursive  ,
                                     filter     ,
                                     outputList ); 
                }
            }        
        }
        while ( CORE::DI_Next_Dir_Entry( did ) );
        
        CORE::DI_Cleanup( did );
    }                
}

/*-------------------------------------------------------------------------*/

void 
CVFS::GetList( TStringList& outputList       ,
               const CORE::CString& location ,
               bool recursive                ,
               const CORE::CString& filter   ) const
{TRACE;
        
    m_datalock.Lock();
    
    /*
     *      Switch dir seperator chars if needed
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
        GetListFromRoot( rootdir    ,
                         recursive  ,
                         filter     ,
                         outputList );        
    }
    
    /*
     *      Now we do the same for the packs
     *  @TODO
     */
                  
    m_datalock.Unlock();          
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
