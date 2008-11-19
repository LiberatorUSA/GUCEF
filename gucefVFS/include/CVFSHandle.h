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

#ifndef GUCEF_VFS_CVFSHANDLE_H
#define GUCEF_VFS_CVFSHANDLE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_VFS_MACROS_H
#include "gucefVFS_macros.h"         /* often used gucefVFS macros */
#define GUCEF_VFS_MACROS_H
#endif /* GUCEF_VFS_MACROS_H ? */

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

class EXPORT_CPP CVFSHandle
{
    public:

    typedef CORE::CTSharedPtr< CORE::CDynamicBuffer > TDynamicBufferPtr;

    CVFSHandle( CORE::CIOAccess* fileAccess   ,
                const CORE::CString& filename ,
                const CORE::CString& filePath );
    
    CVFSHandle( CORE::CIOAccess* fileAccess   ,
                const CORE::CString& filename ,
                const CORE::CString& filePath ,
                TDynamicBufferPtr& bufferPtr  );

    ~CVFSHandle();
    
    CORE::CIOAccess* GetAccess( void );
    
    const CORE::CString& GetFilename( void ) const;
    
    const CORE::CString& GetFilePath( void ) const;
    
    bool IsLoadedInMemory( void ) const;

    private:

    CVFSHandle( void );                              /**< not implemented */
    CVFSHandle( const CVFSHandle& src );             /**< not implemented */
    CVFSHandle& operator=( const CVFSHandle& src );  /**< not implemented */
    
    private:

    TDynamicBufferPtr m_bufferPtr;
    CORE::CIOAccess* m_fileAccess;
    CORE::CString m_filename;
    CORE::CString m_filePath;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_VFS_CVFSHANDLE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
