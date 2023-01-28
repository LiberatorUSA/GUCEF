/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "dvcppfileutils.h"

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#include "gucefCORE_CFileSystemUriResourceAccessor.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CFileSystemUriResourceAccessor::SchemeName = "file";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CFileSystemUriResourceAccessor::CFileSystemUriResourceAccessor( void )
    : CUriResourceAccessor()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CFileSystemUriResourceAccessor::CFileSystemUriResourceAccessor( const CFileSystemUriResourceAccessor& src )
    : CUriResourceAccessor( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CFileSystemUriResourceAccessor::~CFileSystemUriResourceAccessor()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CFileSystemUriResourceAccessor& 
CFileSystemUriResourceAccessor::operator=( const CFileSystemUriResourceAccessor& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::CreateResource( const CUri& uri ,
                                                CIOAccess& src  )
{GUCEF_TRACE;

    if ( SchemeName != uri.GetScheme() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:CreateResource: Unsupported scheme " + uri.GetScheme() );
        return false;
    }

    CString fsPath = uri.GetAuthorityAndPath();
    bool fileExists = FileExists( fsPath );

    if ( !fileExists )
    {
        CFileAccess fsFile;
        if ( fsFile.Open( fsPath, "wb" ) )
        {
            fsFile.Write( src );
            fsFile.Close();
            GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:CreateResource: Wrote file to " + fsPath );
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:CreateResource: Cannot open file for writing at: " + fsPath );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:CreateResource: Cannot create resource since the file already exists: " + fsPath );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::GetResource( const CUri& uri        ,
                                             CIOAccess& destination )
{GUCEF_TRACE;

    if ( SchemeName != uri.GetScheme() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetResource: Unsupported scheme " + uri.GetScheme() );
        return false;
    }

    CString fsPath = uri.GetAuthorityAndPath();
    bool fileExists = FileExists( fsPath );

    if ( fileExists )
    {
        CFileAccess fsFile;
        if ( fsFile.Open( fsPath, "rb" ) )
        {
            destination.Write( fsFile );
            fsFile.Close();
            GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetResource: Wrote file to " + fsPath );
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetResource: Cannot open file for reading at: " + fsPath );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetResource: file does not exist: " + fsPath );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::GetPartialResource( const CUri& uri        ,
                                                    UInt64 byteOffset      ,
                                                    Int64 bytesToGet       ,
                                                    CIOAccess& destination )
{GUCEF_TRACE;

    if ( SchemeName != uri.GetScheme() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetPartialResource: Unsupported scheme " + uri.GetScheme() );
        return false;
    }

    CString fsPath = uri.GetAuthorityAndPath();
    bool fileExists = FileExists( fsPath );

    if ( fileExists )
    {
        CFileAccess fsFile;
        if ( fsFile.Open( fsPath, "rb" ) )
        {
            if ( 0 != fsFile.Seek( SEEK_SET, byteOffset ) )
            {            
                destination.Write( fsFile, bytesToGet );
                fsFile.Close();
                GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetPartialResource: Wrote partial file to " + fsPath );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetPartialResource: Failed to seek to offset " + ToString( byteOffset ) + " in file : " + fsPath );
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetPartialResource: Cannot open file for reading at: " + fsPath );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetPartialResource: file does not exist: " + fsPath );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::GetResourceMetaData( const CUri& uri             ,
                                                     CResourceMetaData& metaData )
{GUCEF_TRACE;

    if ( SchemeName != uri.GetScheme() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetResourceMetaData: Unsupported scheme " + uri.GetScheme() );
        return false;
    }

    CString fsPath = uri.GetAuthorityAndPath();
    metaData.resourceExists = FileExists( fsPath );

    if ( metaData.resourceExists )
    {
        metaData.creationDateTime = GetFileCreationTime( fsPath );
        metaData.hasCreationDateTime = true;
        metaData.modifiedDateTime = GetFileModificationTime( fsPath );
        metaData.hasModifiedDateTime = true;
        metaData.lastAccessedDateTime = CDateTime::Empty;
        metaData.hasLastAccessedDateTime = false;
        metaData.version.Clear();
        metaData.hasVersion = false;
        metaData.name = fsPath;
        metaData.hasName = true;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::UpdateResource( const CUri& uri ,
                                                CIOAccess& src  )
{GUCEF_TRACE;

    CString fsPath = uri.GetAuthorityAndPath();

    CFileAccess fsFile;
    if ( fsFile.Open( fsPath, "wb" ) )  // simply overwrite the file if it exists
    {
        fsFile.Write( src );
        fsFile.Close();
        GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdateResource: Wrote file to " + fsPath );
        return true;
    }
    else
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdateResource: Cannot open file for writing at: " + fsPath );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::UpdatePartialResource( const CUri& uri    ,
                                                       UInt64 byteOffset  ,
                                                       Int64 bytesToWrite ,
                                                       CIOAccess& src     )
{GUCEF_TRACE;

    if ( SchemeName != uri.GetScheme() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: Unsupported scheme " + uri.GetScheme() );
        return false;
    }

    CString fsPath = uri.GetAuthorityAndPath();
    bool fileExists = FileExists( fsPath );

    if ( fileExists )
    {
        CFileAccess fsFile;
        if ( fsFile.Open( fsPath, "rb+" ) )
        {
            if ( 0 != fsFile.Seek( SEEK_SET, byteOffset ) )
            {            
                fsFile.Write( src, bytesToWrite );
                fsFile.Close();
                GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: Wrote partial file to " + fsPath );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: Failed to seek to offset " + ToString( byteOffset ) + " in file : " + fsPath );
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: Cannot open file for alteration at: " + fsPath );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: file does not exist: " + fsPath );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::UpdatePartialResource( const CUri& uri    ,
                                                       CIOAccess& src     )
{GUCEF_TRACE;

    if ( SchemeName != uri.GetScheme() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: Unsupported scheme " + uri.GetScheme() );
        return false;
    }

    CString fsPath = uri.GetAuthorityAndPath();
    bool fileExists = FileExists( fsPath );

    if ( fileExists )
    {
        CFileAccess fsFile;
        if ( fsFile.Open( fsPath, "wb+" ) ) // open the file in append mode
        {
            src.Write( fsFile, -1 );
            fsFile.Close();
            GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: Appended to file " + fsPath );
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: Cannot open file for reading at: " + fsPath );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:UpdatePartialResource: file does not exist: " + fsPath );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::GetSupportedOperations( const CUri& uri                     ,
                                                        CUriResourceAccessorOperations& ops )
{GUCEF_TRACE;

    if ( SchemeName != uri.GetScheme() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:GetSupportedOperations: Unsupported scheme " + uri.GetScheme() );
        return false;
    }

    CString fsPath = uri.GetAuthorityAndPath();
    bool fileExists = FileExists( fsPath );

    ops.createResource = !fileExists;
    ops.deleteResource = fileExists;
    ops.getPartialResource = fileExists;
    ops.getResource = fileExists;
    ops.getResourceMetaData = fileExists;
    ops.updatePartialResource = fileExists;
    ops.updateResource = fileExists;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemUriResourceAccessor::DeleteResource( const CUri& uri )
{GUCEF_TRACE;

    if ( SchemeName != uri.GetScheme() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "FileSystemUriResourceAccessor:DeleteResource: Unsupported scheme " + uri.GetScheme() );
        return false;
    }

    return DeleteFile( uri.GetAuthorityAndPath() );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
