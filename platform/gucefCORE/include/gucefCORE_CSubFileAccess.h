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

#ifndef GUCEF_CORE_CSUBFILEACCESS_H
#define GUCEF_CORE_CSUBFILEACCESS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdio.h>

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"        /* base class for all media manipulators */
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CSubFileAccess : public CIOAccess
{
    public:

    typedef CTSharedPtr< CSubFileAccess, MT::CMutex >       SubFileAccessPtr;

    CSubFileAccess( void );

    virtual ~CSubFileAccess() GUCEF_VIRTUAL_OVERRIDE;

    bool Load( const CString& file ,
               const UInt64 offset ,
               const UInt64 size   );

    /**
     *      open the resource for I/O
     */
    virtual void Open( void ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      close the recource
     */
    virtual void Close( void ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      is the recource opened for reading ?
     */
    virtual bool Opened( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Attempts to read the specified number of bytes from the recourse
     */
    virtual UInt32 Read( void *dest      ,
                         UInt32 esize    ,
                         UInt32 elements ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Attempts to write the specified number of bytes to the recourse
     *      using srcdata as the data source.
     */
    virtual UInt32 Write( const void* srcdata ,
                          UInt32 esize        ,
                          UInt32 elements     ) GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt32 Write( CIOAccess& sourceData );

    /**
     *      Get the current offset in bytes
     */
    virtual UInt64 Tell( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Get the size of the resource in bytes
     */
    virtual UInt64 GetSize( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      jump to a different part of the recource
     */
    virtual Int32 Seek( Int64 offset ,
                        Int32 origin ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      jump to the given offset in the recource
     */
    virtual UInt32 Setpos( UInt64 position ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Read a single character
     */
    virtual char GetChar( void ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      are we at the end of the recource ?
     */
    virtual bool Eof( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsReadable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsWriteable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Is the access to the resource a valid one or
     *      has something gone wrong ?
     */
    virtual bool IsValid( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void Flush( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    const CString& GetFilename( void ) const;

    private:

    FILE* m_file;
    CString m_filename;
    UInt64 m_size;
    UInt64 m_totalSize;
    UInt64 m_offset;
};

typedef CSubFileAccess::SubFileAccessPtr    SubFileAccessPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSUBFILEACCESS_H ? */
