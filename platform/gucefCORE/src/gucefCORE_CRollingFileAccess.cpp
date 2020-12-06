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

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h" 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefCORE_CRollingFileAccess.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#define DEFAULT_MAXFILESIZE     10485760 // 10 MB max file size threshold for rollover

/*-------------------------------------------------------------------------*/

CRollingFileAccess::CRollingFileAccess( const CString& file ,
                                        const char* mode    )
    : CIOAccess()
    , m_maxFileSize( DEFAULT_MAXFILESIZE )
    , m_currentFileIndex( 0 )
    , m_offset( 0 )    
    , m_baseFilename( file )
    , m_fileMode()
    , m_currentFile()
    , m_maxRolloverFilesBeforeDeletion( -1 )
{GUCEF_TRACE;

    Open( file, mode );
}

/*-------------------------------------------------------------------------*/

CRollingFileAccess::CRollingFileAccess( void )
    : CIOAccess()
    , m_maxFileSize( DEFAULT_MAXFILESIZE )
    , m_currentFileIndex( 0 )
    , m_offset( 0 )    
    , m_baseFilename()
    , m_fileMode()
    , m_currentFile()
    , m_maxRolloverFilesBeforeDeletion( -1 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRollingFileAccess::CRollingFileAccess( const CRollingFileAccess& src )
    : CIOAccess( src )
    , m_maxFileSize( src.m_maxFileSize )
    , m_currentFileIndex( src.m_currentFileIndex )
    , m_offset( 0 )    
    , m_baseFilename( src.m_baseFilename )
    , m_fileMode( src.m_fileMode )
    , m_currentFile()
    , m_maxRolloverFilesBeforeDeletion( src.m_maxRolloverFilesBeforeDeletion )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRollingFileAccess::~CRollingFileAccess()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void 
CRollingFileAccess::SetRolloverSizeThreshold( UInt32 sizeThreshold )
{GUCEF_TRACE;

    m_maxFileSize = sizeThreshold;
}

/*-------------------------------------------------------------------------*/

UInt32 
CRollingFileAccess::GetRolloverSizeThreshold( void ) const
{GUCEF_TRACE;

    return m_maxFileSize;
}

/*-------------------------------------------------------------------------*/

CString
CRollingFileAccess::GenerateCurrentFilename( const CString& baseName, UInt32& fileIndex ) const
{GUCEF_TRACE;

    CString actualFilename = ResolveVars( baseName );
    CString fileExt = actualFilename.SubstrToChar( '.', false );
    if ( !fileExt.IsNULLOrEmpty() )
    {
        actualFilename = actualFilename.CutChars( fileExt.Length()+1, false );
        fileExt = '.' + fileExt;
    }
    
    CString testFilename = actualFilename + '.' + UInt32ToString( fileIndex ) + fileExt;
    while ( FileExists( testFilename ) )
    {
        ++fileIndex;
        testFilename = actualFilename + '.' + UInt32ToString( fileIndex ) + fileExt;
    }

    return testFilename;
}

/*-------------------------------------------------------------------------*/

bool 
CRollingFileAccess::Open( const CString& file ,
                          const char* mode    )
{GUCEF_TRACE;

    if ( !m_currentFile.Opened() )
    {    
        m_baseFilename = file;
        m_fileMode = mode;

        CString actualFilename = GenerateCurrentFilename( file, m_currentFileIndex );
        if ( m_currentFile.Open( actualFilename, mode ) )
        {
            m_offset = m_currentFile.Tell();
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void 
CRollingFileAccess::Open( void )
{GUCEF_TRACE;

    m_currentFile.Open();
    m_offset = m_currentFile.Tell();
}

/*-------------------------------------------------------------------------*/

void
CRollingFileAccess::Close( void )
{GUCEF_TRACE;

    m_currentFile.Close();
}

/*-------------------------------------------------------------------------*/

bool
CRollingFileAccess::Opened( void ) const
{GUCEF_TRACE;

    return m_currentFile.Opened();
}

/*-------------------------------------------------------------------------*/

CString 
CRollingFileAccess::ReadLine( void )
{GUCEF_TRACE;

    CString result = m_currentFile.ReadLine();
    m_offset = m_currentFile.Tell();
    return result;
}

/*-------------------------------------------------------------------------*/


CString
CRollingFileAccess::ReadString( void )
{GUCEF_TRACE;

    CString result = m_currentFile.ReadString();
    m_offset = m_currentFile.Tell();
    return result;
}

/*-------------------------------------------------------------------------*/

UInt32
CRollingFileAccess::Read( void *dest      ,
                          UInt32 esize    ,
                          UInt32 elements )
{GUCEF_TRACE;

    UInt32 result = m_currentFile.Read( dest, esize, elements );
    m_offset += result * esize;
    return result;
}

/*-------------------------------------------------------------------------*/

void
CRollingFileAccess::PerformRolledoverFilesCleanup( void )
{GUCEF_TRACE;

    if ( m_maxRolloverFilesBeforeDeletion < 1 )
        return;

    
}

/*-------------------------------------------------------------------------*/

void
CRollingFileAccess::EnforceFileSizeThreshold( void )
{GUCEF_TRACE;

    if ( m_offset >= m_maxFileSize )
    {
        Close();
        Open( m_baseFilename, m_fileMode.C_String() );
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CRollingFileAccess::Write( const void* srcdata ,
                           UInt32 esize        ,
                           UInt32 elements     )
{GUCEF_TRACE;

    UInt32 elementsWritten = m_currentFile.Write( srcdata, esize, elements );
    m_offset += elementsWritten * esize;
    EnforceFileSizeThreshold();
    return elementsWritten;
}

/*-------------------------------------------------------------------------*/

UInt32
CRollingFileAccess::Write( CIOAccess& sourceData )
{GUCEF_TRACE;

    UInt32 bytesWritten = m_currentFile.Write( sourceData );
    m_offset += bytesWritten;
    EnforceFileSizeThreshold();
    return bytesWritten;
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CRollingFileAccess::Write( const CString& string )
{GUCEF_TRACE;

    UInt32 bytesWritten = m_currentFile.Write( string );
    m_offset += bytesWritten;
    EnforceFileSizeThreshold();
    return bytesWritten;
}

/*-------------------------------------------------------------------------*/

UInt32
CRollingFileAccess::Tell( void ) const
{GUCEF_TRACE;

    return m_offset = m_currentFile.Tell();
}

/*-------------------------------------------------------------------------*/

UInt32
CRollingFileAccess::GetSize( void ) const
{GUCEF_TRACE;

    return m_currentFile.GetSize();
}

/*-------------------------------------------------------------------------*/

Int32
CRollingFileAccess::Seek( Int32 offset ,
                          Int32 origin )
{GUCEF_TRACE;

    UInt32 retVal = m_currentFile.Seek( offset, origin );
    if ( 0 == retVal ) // 0 indicates success
    {
        m_offset = m_currentFile.Tell();
    }
    return retVal;
}

/*-------------------------------------------------------------------------*/

UInt32
CRollingFileAccess::Setpos( UInt32 position )
{GUCEF_TRACE;

    UInt32 retVal = m_currentFile.Setpos( position );
    if ( 0 == retVal ) // 0 indicates success
    {
        m_offset = position;
    }
    return retVal;
}

/*-------------------------------------------------------------------------*/

char
CRollingFileAccess::GetChar( void )
{GUCEF_TRACE;

    return m_currentFile.GetChar();
}

/*-------------------------------------------------------------------------*/

bool
CRollingFileAccess::Eof( void ) const
{GUCEF_TRACE;

    return m_currentFile.Eof();
}

/*-------------------------------------------------------------------------*/

bool
CRollingFileAccess::IsReadable( void ) const
{GUCEF_TRACE;

    return m_currentFile.IsReadable();
}

/*-------------------------------------------------------------------------*/

bool
CRollingFileAccess::IsWriteable( void ) const
{GUCEF_TRACE;

    return m_currentFile.IsWriteable();
}

/*-------------------------------------------------------------------------*/

bool
CRollingFileAccess::IsValid( void )
{GUCEF_TRACE;

    return m_currentFile.IsValid();
}

/*-------------------------------------------------------------------------*/

void
CRollingFileAccess::Flush( void )
{GUCEF_TRACE;

    m_currentFile.Flush();
}

/*-------------------------------------------------------------------------*/

CICloneable*
CRollingFileAccess::Clone( void ) const
{GUCEF_TRACE;

    return new CRollingFileAccess( *this );
}

/*-------------------------------------------------------------------------*/

bool
CRollingFileAccess::SetFileToUse( const CString& filename  ,
                                  const char* mode         ,
                                  bool moveIfCurrentlyOpen )
{GUCEF_TRACE;

    m_baseFilename = filename;
    UInt32 index = m_currentFileIndex;
    CString actualFilename = GenerateCurrentFilename( filename, index );
    bool result = m_currentFile.SetFileToUse( actualFilename, mode, moveIfCurrentlyOpen );
    if ( result && moveIfCurrentlyOpen )
    {
        m_currentFileIndex = index;
        if ( m_currentFile.Opened() )
        {
            m_offset = m_currentFile.Tell();
        }
    }
    return result;
}

/*-------------------------------------------------------------------------*/
    
const CString& 
CRollingFileAccess::GetFilename( void ) const
{GUCEF_TRACE;

    return m_currentFile.GetFilename();
}

/*-------------------------------------------------------------------------*/

void
CRollingFileAccess::SetMaxRolloverFilesBeforeDeletion( Int32 deleteThreshold )
{GUCEF_TRACE;
    
    m_maxRolloverFilesBeforeDeletion = deleteThreshold;
}

/*-------------------------------------------------------------------------*/

Int32
CRollingFileAccess::GetMaxRolloverFilesBeforeDeletion( void ) const
{GUCEF_TRACE;
    
    return m_maxRolloverFilesBeforeDeletion;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
