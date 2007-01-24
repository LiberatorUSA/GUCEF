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

#ifndef CPFILEACCESS_H
#include "CPFileAccess.h"
#define CPFILEACCESS_H
#endif /* CPFILEACCESS_H ? */

#ifndef CPFILECHUNKACCESS_H
#include "CPFileChunkAccess.h"
#define CPFILECHUNKACCESS_H
#endif /* CPFILECHUNKACCESS_H ? */

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

#ifndef CFPCODECPLUGINMANAGER_H
#include "CFPCodecPluginManager.h"   /* manager for codecs loaded as FPCodec plugins */
#define CFPCODECPLUGINMANAGER_H
#endif /* CFPCODECPLUGINMANAGER_H ? */

#ifndef CFILEPACKAGE_H
#include "CFilePackage.h"        /* class for storaging package info */
#define CFILEPACKAGE_H
#endif /* CFILEPACKAGE_H ? */

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
MT::CMutex CVFS::_datalock;

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SDiskFileRef
{
        Int32 loadcount;
        CORE::CString filename;
        CORE::CIOAccess* access;
        bool memloaded;
        bool exclusive;
        void* memptr;                 
};
typedef struct SDiskFileRef TDiskFileRef;

/*-------------------------------------------------------------------------*/

struct SRootDir
{
        CORE::CString abspath;
        CORE::CString path;
        bool writeable;
};
typedef struct SRootDir TRootDir;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CVFS::CVFS( void )
        : CIConfigurable( true ) ,
          _maxmemloadsize( 1024 ) 
{TRACE;
       
}
        
/*-------------------------------------------------------------------------*/
        
CVFS::CVFS( const CVFS& src )
{TRACE;
        DEBUGOUTPUT( "CVFS::CVFS( const CVFS& src )" );
        
        /* dummy, do not use */
}
        
/*-------------------------------------------------------------------------*/
        
CVFS::~CVFS()
{TRACE;        
}
        
/*-------------------------------------------------------------------------*/
        
CVFS& 
CVFS::operator=( const CVFS& src )
{TRACE;
        DEBUGOUTPUT( "CVFS::operator=( const CVFS& src )" );
        
        /* dummy, do not use */
        return *_instance;
}

/*-------------------------------------------------------------------------*/

CVFS* 
CVFS::Instance( void )
{TRACE;
        DEBUGOUTPUT( "CVFS::Instance( void ) const" );
        
        _datalock.Lock();
        if ( !_instance ) 
        {
                _instance = new CVFS();
        }
        _datalock.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CVFS::Deinstance( void )
{TRACE;
        DEBUGOUTPUT( "CVFS::Deinstance( void )" );
        
        _datalock.Lock();
        delete _instance;
        _instance = NULL;
        _datalock.Unlock();
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
        const TRootDir* entry;
        CORE::CIOAccess* access = NULL;
        TDiskFileRef* dfr;                
        bool needwriteable( ( strchr( mode, 'a' ) != NULL ) || ( strchr( mode, 'w' ) != NULL ) || ( strchr( mode, '+' ) != NULL )); 
                
                
        for ( UInt32 i=0; i<m_rootlist.GetCount(); ++i )
        {
                entry = static_cast< const TRootDir* >(  m_rootlist[ i ] );
                filepath = entry->abspath;
                CORE::AppendToPath( filepath, file );                
                
                bool exists( CORE::File_Exists( filepath.C_String() ) > 0 );
                if ( ( exists && ( strcmp( mode, "rb" ) == 0 ) ) )
                {                                         
                        UInt32 fsize = CORE::Filesize( filepath.C_String() );                        
                        if ( fsize <= _maxmemloadsize )
                        {
                                FILE* fptr = fopen( filepath.C_String(), "rb" );
                                if ( !fptr ) continue;
                                
                                dfr = new TDiskFileRef;                                
                                dfr->memptr = new UInt8[ fsize ];
                                fread( dfr->memptr, 1, fsize, fptr );
                                fclose( fptr ); 
                                dfr->access = new CORE::CMFileAccess( dfr->memptr ,
                                                                      fsize       );
                                dfr->memloaded = true;
                                dfr->exclusive = false;
                                access = new CORE::CMFileAccess( *(static_cast<CORE::CMFileAccess*>( dfr->access )) );
                        }
                        else                        
                        {
                                CORE::CPFileAccess* fa = new CORE::CPFileAccess( filepath );
                                if ( !fa->IsValid() )
                                {
                                        delete fa;
                                        continue;
                                }
                                
                                dfr = new TDiskFileRef;
                                dfr->memptr = NULL;
                                dfr->access = fa;
                                dfr->memloaded = false;
                                dfr->exclusive = false;
                                access = new CORE::CPFileChunkAccess( *(static_cast<CORE::CPFileAccess*>( dfr->access ))         ,
                                                                      0                                                          ,
                                                                      static_cast<CORE::CPFileAccess*>( dfr->access )->GetSize() );                                
                        }
                }
                else
                {
                        if ( ( strcmp( mode, "r+" ) != 0  ) &&
                             ( strcmp( mode, "wb" ) != 0  )  )
                        {
                                continue;
                        }                             
                        
                        if ( ( strchr( mode, 'w' ) != NULL ) )
                        {
                                if ( ( exists && !overwrite ) ||
                                     ( !entry->writeable )     )
                                {
                                        continue;
                                }
                        }
                        else
                        if ( ( strchr( mode, 'a' ) != NULL ) ||
                             ( strchr( mode, '+' ) != NULL )  )
                        {       
                                if ( !entry->writeable )
                                {
                                        continue;
                                }
                        }
                        
                        /*
                         *      The r+ and wb mix with rb is covered by the O/S
                         *      since r+ and wb require exclusive access.
                         */                        
                        access = new CORE::CFileAccess( filepath ,
                                                        mode     );
                        if ( !access->IsValid() )
                        {
                                delete access;
                                access = NULL;
                                continue;
                        }
                        
                        dfr = new TDiskFileRef;
                        dfr->memptr = NULL;
                        dfr->access = access;
                        dfr->memloaded = false;                        
                        dfr->exclusive = true;
                }
                                
                if ( access )
                {                                            
                        dfr->filename = file;
                        dfr->loadcount = 1;
                        Int32 index = _loaded.FindNULLEntry();
                        CVFSHandle* fh;
                        if ( index >= 0 )
                        {                        
                                fh = new CVFSHandle( access  ,
                                                file    ,
                                                false   ,
                                                index   );
                                _loaded.SetEntry( index, dfr );
                        }
                        else
                        {
                                index = _loaded.AppendEntry( NULL );
                                fh = new CVFSHandle( access ,
                                                file   ,
                                                false  ,
                                                index  );
                                _loaded.SetEntry( index, dfr );
                        } 
                        DEBUGOUTPUTss( "Loaded new file: ", file.C_String() );
                        DEBUGOUTPUTss( "Access mode: ", mode );                                
                        return fh;                     
                }
        }
        return NULL;                                       
}

/*-------------------------------------------------------------------------*/

CVFSHandle*
CVFS::LoadFromDiskCache( const CORE::CString& file ,
                         UInt32& errorcode         )
{TRACE;
        DEBUGOUTPUT( "CVFS::LoadFromDiskCache( const CORE::CString& file )" );
        
        /*
         *      Check if the file is already loaded
         *      from disk
         */
        TDiskFileRef* dfr; 
        for ( Int32 i=0; i<=_loaded.GetLast(); ++i )
        {
                if ( _loaded[ i ] )
                {
                        dfr = (TDiskFileRef*) _loaded[ i ];
                        if ( dfr->filename == file )
                        {
                                DEBUGOUTPUTss( "Refrencing cached file: ", file.C_String() );
                                
                                if ( dfr->exclusive )
                                {
                                        DEBUGOUTPUTs( "Failure: Requisted cache file currently has exclusive access" );
                                        return NULL;
                                }
                                
                                CORE::CIOAccess* access;
                                if ( dfr->memloaded )
                                {
                                        access = new CORE::CMFileAccess( *(static_cast<CORE::CMFileAccess*>( dfr->access )) );
                                }
                                else
                                {
                                        access = new CORE::CPFileChunkAccess( *(static_cast<CORE::CPFileAccess*>( dfr->access ))         ,
                                                                              0                                                          ,
                                                                              static_cast<CORE::CPFileAccess*>( dfr->access )->GetSize() );
                                }
                                ++dfr->loadcount;
                                Int32 index = _loaded.FindNULLEntry();
                                if ( index >= 0 )
                                {                                
                                        CVFSHandle* fh = new CVFSHandle( access ,
                                                                         file   ,
                                                                         false  ,
                                                                         index  );
                                        _loaded.SetEntry( index, dfr );
                                        return fh;
                                }
                                
                                index = _loaded.AppendEntry( NULL );
                                CVFSHandle* fh = new CVFSHandle( access ,
                                                                 file   ,
                                                                 false  ,
                                                                 index  );
                                _loaded.SetEntry( index, dfr );
                                return fh;                                                                        
                        }
                }
        }
        return NULL;        
}

/*-------------------------------------------------------------------------*/

CVFSHandle*
CVFS::LoadFromPack( const CORE::CString& file ,
                    UInt32& errorcode         )
{TRACE;
        DEBUGOUTPUT( "CVFS::LoadFromPack( const CORE::CString& file )" );
        
        /*
         *      Try and get the file from a pack
         */
        CFilePackage* fp;
        CVFSHandle* fh;
        for ( Int32 i=0; i<=_packlist.GetLast(); ++i )
        {         
                fp = static_cast<CFilePackage*>( _packlist[ i ] );
                fh = fp->GetFile( file      ,
                                  errorcode );
                if ( fh )
                {                        
                        return fh;        
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
        
        #ifdef GUCEF_MSWIN_BUILD
        CORE::CString filepath( file.ReplaceChar( '/', '\\' ) );
        #else
        CORE::CString filepath( file.ReplaceChar( '\\', '/' ) );
        #endif
        
        _datalock.Lock();
        
        CVFSHandle* fh(NULL);
        if ( 0 == strcmp( mode, "rb" ) )
        {
                /*
                 *      Check if the file is already loaded
                 *      from disk
                 */
                fh = LoadFromDiskCache( filepath  , 
                                        errorcode );
                if ( fh )
                {
                        _datalock.Unlock();
                        return CVFSHandlePtr( fh, this );    
                }         
                        
                /*
                 *      Try and get the file from a pack
                 */
                fh = LoadFromPack( filepath  ,
                                errorcode );
                if ( fh )
                {
                        _datalock.Unlock();
                        return CVFSHandlePtr( fh, this );       
                }       
        }

        /* 
         *      Try to locate the file using the rootdirs as 
         *      a prefix
         */
        fh = LoadFromDisk( filepath  ,
                           errorcode ,
                           mode      ,
                           overwrite );
        if ( fh )
        {
                _datalock.Unlock();
                return CVFSHandlePtr( fh, this );                        
        }          
        
        /*
         *      If we get here then the file was not found in one 
         *      of the rootdirs. or in any of the packfiles
         */
        DEBUGOUTPUTss( "Cannot locate file: ", file.C_String() ); 
        _datalock.Unlock();
        return CVFSHandlePtr();                                
}

/*-------------------------------------------------------------------------*/

void
CVFS::DestroyObject( CVFSHandle* vfshandle )
{TRACE;
        DEBUGOUTPUT( "CVFS::Unload( CVFSHandle* vfshandle )" );
        
        _datalock.Lock();
        if ( vfshandle )
        {
                if ( vfshandle->_packed )
                {
                        static_cast<CFilePackage*>( _packlist[ vfshandle->_index ] )->Unload( vfshandle );        
                }
                else
                {
                        TDiskFileRef* dfr; 
                        dfr = static_cast<TDiskFileRef*>( _loaded[ vfshandle->_index ] );
                        if ( dfr )
                        {          
                                DEBUGOUTPUTss( "Unload request for file: ", dfr->filename.C_String() );
                                delete vfshandle->_fileaccess;
                                
                                --dfr->loadcount;
                                if ( dfr->loadcount <= 0 )
                                {
                                        DEBUGOUTPUTss( "Unloading file: ", dfr->filename.C_String() );
                                        _loaded.SetEntry( vfshandle->_index, NULL );
                                        if ( !dfr->exclusive )
                                        {
                                                delete dfr->access;
                                                delete []dfr->memptr;
                                        }                                                                                                                                                                                                                                                          
                                        delete dfr;                                                              
                                }                                    
                                delete vfshandle;                                                        
                                _datalock.Unlock();
                                return;                                                        
                        }                                                                     
                }
        }
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CVFS::FileExists( const CORE::CString& file ) const
{TRACE;       
        /*
         *      Attempt to find the file in a pack file
         */
         
         // make me    @TODO    
        
        /*
         *      Attempt to find the file on disk
         */
        CORE::CString filepath;
        const TRootDir* entry; 
        for ( UInt32 i=0; i<m_rootlist.GetCount(); ++i )
        {
                entry = static_cast< const TRootDir* >(  m_rootlist[ i ] );
                filepath = entry->abspath;
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
        
        if ( rootpath.Length() )
        {        
                _datalock.Lock();
                
                TRootDir* newroot = new TRootDir;
                newroot->abspath = CORE::RelativePath( rootpath );
                newroot->path = rootpath;
                newroot->writeable = writeable;
                
                if ( !newroot->abspath.Length() )
                {
                        newroot->abspath = rootpath;
                }
                
                TRootDir* entry;
                for ( UInt32 i=0; i<m_rootlist.GetCount(); ++i )
                {
                        entry = static_cast< TRootDir* >(  m_rootlist[ i ] );
                        if ( entry->path.Lowercase() == rootpath.Lowercase() )
                        {
                                /*
                                 *      rootpath already exists                                 
                                 */
                                delete newroot;
                                _datalock.Unlock();
                                return; 
                        }
                }
                
                m_rootlist.AppendEntry( newroot );
                DEBUGOUTPUTss( "CVFS: VFS root added: ", rootpath.C_String() );
                                    
                _datalock.Unlock();
        }                
}

/*-------------------------------------------------------------------------*/

void 
CVFS::SetMemloadSize( UInt32 bytesize )
{TRACE;
        DEBUGOUTPUT( "CVFS::SetMemloadSize( UInt32 bytesize )" );
        
        _datalock.Lock();
        _maxmemloadsize = bytesize;
        CFilePackage* fp;
        for ( Int32 i=0; i<=_packlist.GetLast(); ++i )
        {         
                fp = static_cast<CFilePackage*>( _packlist[ i ] );
                if ( fp )
                {                        
                        fp->SetMemloadSize( bytesize );        
                }                                  
        }
        _datalock.Unlock();
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
        UInt32 count = m_rootlist.GetCount();                         
        CORE::CDataNode pathnode( "vfsroot" ); 
        TRootDir* entry;
        for ( UInt32 i=0; i<count; ++i )
        {                   
                entry = static_cast< TRootDir* >( m_rootlist[ i ] );
                pathnode.SetAttribute( "path", entry->path );
                if ( entry->writeable )
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
        if ( filter.Length() )
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
                       CORE::CStringList& list       ) const
{TRACE;

        DEBUGOUTPUTssss( "Getting file list with filter ", filter.C_String(), " for dir: ", root.C_String() );

        CORE::CString filename;
        struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( root.C_String() );
        if ( did )
        {                
                /*
                 *      Iterate the dir content adding the file entrys from
                 *      the dir to the list
                 */
                do
                {
                        if ( CORE::DI_Is_It_A_File( did ) )
                        {                           
                                filename = CORE::DI_Name( did );     
                                if ( FilterValidation( filename ,
                                                       filter   ) )
                                {
                                        list.Append( filename );
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
                                        GetListFromRoot( subdir    ,
                                                         recursive ,
                                                         filter    ,
                                                         list      ); 
                                }
                        }        
                }
                while ( CORE::DI_Next_Dir_Entry( did ) );
                
                CORE::DI_Cleanup( did );
        }                
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CVFS::GetList( const CORE::CString& location ,
               bool recursive                ,
               const CORE::CString& filter   ) const
{TRACE;
        _datalock.Lock();
        CORE::CStringList list;
        
        /*
         *      Switch dir sep chars if needed
         */
        CORE::CString loc( location.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );              
        
        /*
         *      Get a file list from each (root+location) path
         */
        TRootDir* entry; 
        for ( UInt32 i=0; i<m_rootlist.GetCount(); ++i )
        {
                /*
                 *      Make the combo path string
                 */
                entry = static_cast< TRootDir* >( m_rootlist[ i ] );
                CORE::CString rootdir( entry->abspath );
                AppendToPath( rootdir, location );                 
        
                /*
                 *      Process the root
                 */
                GetListFromRoot( rootdir    ,
                                 recursive  ,
                                 filter     ,
                                 list       );        
        }
        
        /*
         *      Now we do the same for the packs
         */
         
         
        _datalock.Unlock(); 
        return list;          
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
