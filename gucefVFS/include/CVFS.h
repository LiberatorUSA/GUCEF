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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef GUCEF_VFS_CVFS_H
#define GUCEF_VFS_CVFS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#include "CIConfigurable.h"
#include "CDVString.h"

#ifndef GUCEFMT_H
#include "gucefMT.h"                 /* gucefMT library API */
#define GUCEFMT_H
#endif /* GUCEFMT_H ? */

#ifndef GUCEF_CORE_CTBASICSHAREDPTR_H
#include "CTBasicSharedPtr.h"
#define GUCEF_CORE_CTBASICSHAREDPTR_H
#endif /* GUCEF_CORE_CTBASICSHAREDPTR_H ? */

#ifndef GUCEFVFS_MACROS_H
#include "gucefVFS_macros.h"         /* often used gucefVFS macros */
#define GUCEFVFS_MACROS_H
#endif /* GUCEFVFS_MACROS_H ? */

#ifndef CVFSHANDLE_H
#include "CVFSHandle.h"              /* handle for VFS ref counted recources */
#define CVFSHANDLE_H
#endif /* CVFSHANDLE_H ? */

#ifndef CICONFIGURABLE_H
#include "CIConfigurable.h"          /* abstract base class interface for configurable objects */
#define CICONFIGURABLE_H
#endif /* CICONFIGURABLE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Threadsafe Virtual File System
 */
class EXPORT_CPP CVFS : public CORE::CIConfigurable                          ,
                        private CORE::CTDynamicDestructorBase< CVFSHandle >
{
    public:
    
    typedef CORE::CTBasicSharedPtr< CVFSHandle > CVFSHandlePtr;
    typedef std::vector< CORE::CString >         TStringList;
    
    static CVFS* Instance( void );
    
    void AddRoot( const CORE::CString& rootdir ,
                  const bool writeable = false );
    
    CVFSHandlePtr GetFile( const CORE::CString& file    ,
                           UInt32& errorcode            ,
                           const char* mode = "rb"      ,
                           const bool overwrite = false );
                                  
    void GetList( TStringList& outputList            ,
                  const CORE::CString& location      , 
                  bool recursive = false             ,
                  bool includePathInFilename = false ,
                  const CORE::CString& filter = ""   ) const;
    
    bool FileExists( const CORE::CString& filePath ) const;
    
    UInt32 GetFileSize( const CORE::CString& filePath ) const;

    void SetMemloadSize( UInt32 bytesize );
    
    UInt32 GetMemloadSize( void ) const;
    
    /**
     *      Attempts to store the given tree in the file
     *      given according to the method of the codec metadata
     *
     *      @param tree the data tree you wish to store
     *      @return wheter storing the tree was successfull
     */
    virtual bool SaveConfig( CORE::CDataNode& tree );
                                
    /**
     *      Attempts to load data from the given file to the 
     *      root node given. The root data will be replaced 
     *      and any children the node may already have will be deleted.
     *
     *      @param treeroot pointer to the node that is to act as root of the data tree
     *      @return whether building the tree from the given file was successfull.
     */                                    
    virtual bool LoadConfig( const CORE::CDataNode& treeroot );                                             

    private:
    friend class CGUCEFVFSModule;
    
    static void Deinstance( void );
    
    private:
    
    virtual void DestroyObject( CVFSHandle* sharedPointer );

    private:
    
    static CVFS* _instance;
    static MT::CMutex m_datalock;
    
    CVFS( void );
    CVFS( const CVFS& src );
    virtual ~CVFS();
    CVFS& operator=( const CVFS& src );

    CVFSHandle* LoadFromDisk( const CORE::CString& file, UInt32& errorcode, const char* mode = "rb", const bool overwrite = false );
    void GetListFromRoot( const CORE::CString& root, bool recursive, bool includePathInFilename, const CORE::CString& filter, TStringList& outputList ) const;
    bool FilterValidation( const CORE::CString& filename, const CORE::CString& filter ) const; 

    private:

    struct SRootDir
    {
        CORE::CString abspath;
        CORE::CString path;
        bool writeable;
    };
    typedef struct SRootDir TRootDir;
    
    typedef std::vector< TRootDir >                      TRootList;
    typedef CORE::CTSharedPtr< CORE::CDynamicBuffer >    TDynamicBufferPtr;
    typedef std::map< CORE::CString, TDynamicBufferPtr > TFileMemCache;
    
    TRootList m_rootlist;
    TFileMemCache m_diskCacheList;
    UInt32 _maxmemloadsize;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_VFS_CVFS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-08-2005 :
        - Dinand: implemented GetNewFile()
        - Dinand: reworked the AddRoot() and root administration mechanisms. This allows
          support for writeable roots and storage of the relative paths without
          data duplication.
        - Dinand: Added support for new writeable files (exclusive access only)
        - Dinand: Added support for read & write files (exclusive access only)
- 18-05-2005 :
        - Dinand: Fixed a bug in FileExists(): If the recource didn't exist the code would get
          stuck in a loop.
- 01-05-2005 :
        - Dinand: Fixed array bounds write violation in LoadFromDiskCache()
        - Dinand: Fixed a bug in Unload(): The last cached item's access object was
          deleted twice.
        - Dinand: Unload() no longer has to search for the handle entry saving us some time           
- 26-04-2005 :
        - Dinand: Fixed a bug in LoadFromDisk(): file information storage accessed an
          invalid heap entry if no storage space was available.
        - Dinand: Fixed a bug in Unload(): The last cached item's access object was
          deleted twice.  
- 12-02-2005 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/
