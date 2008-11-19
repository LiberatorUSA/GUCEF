/*
 *  vfspluginVP: Generic GUCEF VFS plugin for "Violation Pack" archives
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#include <map>

#ifndef GUCEF_VFS_CVFS_H
#include "CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_VFSPLUGIN_VP_MACROS_H
#include "vfspluginVP_macros.h"
#define GUCEF_VFSPLUGIN_VP_MACROS_H
#endif /* GUCEF_VFSPLUGIN_VP_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFSPLUGIN {
namespace VP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CVPArchive : public VFS::CIArchive
{
    public:
    
    CVPArchive( void );
    
    virtual ~CVPArchive();
    
    virtual CVFSHandlePtr GetFile( const VFS::CString& file          ,
                                   const char* mode = "rb"           ,
                                   const VFS::UInt32 memLoadSize = 0 ,
                                   const bool overwrite = false      );
                                  
    virtual void GetList( TStringSet& outputList             ,
                          const VFS::CString& location       , 
                          bool recursive = false             ,
                          bool includePathInFilename = false ,
                          const VFS::CString& filter = ""    ,
                          bool addFiles = true               ,
                          bool addDirs  = false              ) const;
    
    virtual bool FileExists( const VFS::CString& filePath ) const;
    
    virtual VFS::UInt32 GetFileSize( const VFS::CString& filePath ) const;
    
    virtual VFS::CString GetFileHash( const VFS::CString& file ) const;
    
    virtual const VFS::CString& GetArchiveName( void ) const;
    
    virtual bool IsWriteable( void ) const;
    
    virtual bool LoadArchive( const VFS::CString& archiveName ,
                              const VFS::CString& archivePath ,
                              const bool writableRequest );
                              
    virtual bool UnloadArchive( void );    

    virtual void DestroyObject( VFS::CVFSHandle* objectToBeDestroyed );

    private:
    
    CVPArchive( const CVPArchive& src );
    CVPArchive& operator=( const CVPArchive& src );

    CORE::CIOAccess* LoadFile( const VFS::CString& file      ,
                               const VFS::UInt32 memLoadSize ) const;
    
    private:
    
    struct SVPHeader
    {
        char sig[ 4 ];           /* should always be "VPVP" */
        VFS::Int32 version;      /* should be 2 */
        VFS::UInt32 indexoffset; /* index offset from the start of the file in bytes */
        VFS::UInt32 idxentries;  /* number of index entries */
    };
    typedef struct SVPHeader TVPHeader;

    struct SVPIndexEntry
    {
        VFS::UInt32 offset;
        VFS::UInt32 size;
        VFS::Int32 timestamp;        
    };
    typedef struct SVPIndexEntry TVPIndexEntry;
    
    typedef std::map< VFS::CString, TVPIndexEntry > TFileIndexMap;
    
    #pragma pack (1)
    TVPHeader m_header;
    TFileIndexMap m_index;
    VFS::CString m_archiveName;
    VFS::CString m_archivePath;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VP */
}; /* namespace VFSPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 04-05-2005 :
        - Dinand: Initial version.

-----------------------------------------------------------------------------*/
