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

#ifndef GUCEF_VFS_CIARCHIVE_H
#define GUCEF_VFS_CIARCHIVE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include <set>

#ifndef GUCEF_CORE_CTBASICSHAREDPTR_H
#include "CTBasicSharedPtr.h"
#define GUCEF_CORE_CTBASICSHAREDPTR_H
#endif /* GUCEF_CORE_CTBASICSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CDATETIME_H
#include "gucefCORE_CDateTime.h"
#define GUCEF_CORE_CDATETIME_H
#endif /* GUCEF_CORE_CDATETIME_H ? */

#ifndef GUCEF_VFS_CVFSHANDLE_H
#include "gucefVFS_CVFSHandle.h"     /* handle for VFS ref counted recources */
#define GUCEF_VFS_CVFSHANDLE_H
#endif /* GUCEF_VFS_CVFSHANDLE_H ? */

#ifndef GUCEF_VFS_CARCHIVESETTINGS_H
#include "gucefVFS_CArchiveSettings.h"
#define GUCEF_VFS_CARCHIVESETTINGS_H
#endif /* GUCEF_VFS_CARCHIVESETTINGS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_VFS_PUBLIC_CPP CIArchive : public CORE::CTDynamicDestructorBase< CVFSHandle >
{
    public:

    typedef CORE::CTBasicSharedPtr< CVFSHandle > CVFSHandlePtr;
    typedef std::vector< CString >         TStringList;
    typedef std::set< CString >            TStringSet;
    
    CIArchive( void );
    
    CIArchive( const CIArchive& src );
    
    virtual ~CIArchive();
    
    CIArchive& operator=( const CIArchive& src );
    
    virtual CVFSHandlePtr GetFile( const CString& file          ,
                                   const char* mode = "rb"      ,
                                   const UInt32 memLoadSize = 0 ,
                                   const bool overwrite = false ) = 0;

    virtual bool StoreAsFile( const CORE::CString& filepath    ,
                              const CORE::CDynamicBuffer& data ,
                              const CORE::UInt64 offset        ,
                              const bool overwrite             ) = 0;

    virtual void GetList( TStringSet& outputList             ,
                          const CString& location            , 
                          bool recursive = false             ,
                          bool includePathInFilename = false ,
                          const CString& filter = ""         ,
                          bool addFiles = true               ,
                          bool addDirs  = false              ) const = 0;
    
    virtual bool FileExists( const CString& filePath ) const = 0;
    
    virtual UInt32 GetFileSize( const CString& filePath ) const = 0;
    
    virtual CString GetFileHash( const CString& file ) const = 0;
    
    virtual CORE::CDateTime GetFileModificationTime( const CString& filePath ) const = 0;
    
    virtual const CString& GetArchiveName( void ) const = 0;
    
    virtual bool IsWriteable( void ) const = 0;
    
    virtual bool LoadArchive( const CArchiveSettings& settings ) = 0;

    virtual bool LoadArchive( const CString& archiveName  ,
                              CVFSHandlePtr vfsResource   ,
                              const bool writeableRequest ) = 0;
                              
    virtual bool UnloadArchive( void ) = 0;
    
    virtual const CString& GetType( void ) const = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_VFS_CIARCHIVE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-08-2005 :
        - Dinand: implemented

---------------------------------------------------------------------------*/