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

#ifndef CVFS_H
#define CVFS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

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
                        private CORE::CTSharedObjectDestructor< CVFSHandle >
{
        public:
        
        typedef CORE::CTBasicSharedPtr< CVFSHandle > CVFSHandlePtr;
        
        static CVFS* Instance( void );
        
        void AddRoot( const CORE::CString& rootdir ,
                      const bool writeable = false );
        
        CVFSHandlePtr GetFile( const CORE::CString& file    ,
                               UInt32& errorcode            ,
                               const char* mode = "rb"      ,
                               const bool overwrite = false );
                                      
        CORE::CStringList GetList( const CORE::CString& location    , 
                                   bool recursive = false           ,
                                   const CORE::CString& filter = "" ) const;
        
        bool FileExists( const CORE::CString& file ) const;

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
        
        virtual void DestroySharedObject( CVFSHandle* sharedPointer );

        private:
        static CVFS* _instance;
        static MT::CMutex _datalock;
        
        CVFS( void );
        CVFS( const CVFS& src );
        ~CVFS();
        CVFS& operator=( const CVFS& src );
        
        CVFSHandle* LoadFromDiskCache( const CORE::CString& file, UInt32& errorcode );
        CVFSHandle* LoadFromPack( const CORE::CString& file, UInt32& errorcode );
        CVFSHandle* LoadFromDisk( const CORE::CString& file, UInt32& errorcode, const char* mode = "rb", const bool overwrite = false );
        void GetListFromRoot( const CORE::CString& root, bool recursive, const CORE::CString& filter, CORE::CStringList& list ) const;
        bool FilterValidation( const CORE::CString& filename, const CORE::CString& filter ) const; 
        
        CORE::CDynamicArray m_rootlist;
        CORE::CDynamicArray _packlist;
        CORE::CDynamicArray _loaded;
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
          
#endif /* CVFS_H ? */

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
