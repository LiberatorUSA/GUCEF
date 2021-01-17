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

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H

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

class CDynamicBuffer;

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CDynamicBufferAccess : public CIOAccess
{
    public:

    CDynamicBufferAccess( void );
    
    CDynamicBufferAccess( CDynamicBuffer* buffer                         ,
                          const bool deleteBufferUponDestruction = false );

    CDynamicBufferAccess( const CDynamicBuffer& buffer );

    virtual ~CDynamicBufferAccess();

    bool LoadContentFromFile( const CString& filePath       ,
                              const UInt32 offsetInFile = 0 ,
                              const Int32 bytesToRead = -1  );

    /**
     *  Shortcut for creating a linked dynamic buffer with IO access.
     *  Uses DynamicBuffer::LinkTo
     */
    void LinkTo( const void* externalBuffer ,
                 UInt32 bufferSize          );

    void LinkTo( CDynamicBuffer* externalBuffer   ,
                 bool deleteBufferUponDestruction );

    /**
     *      open the resource for I/O
     */
    virtual void Open( void );

    /**
     *      close the resource
     */
    virtual void Close( void );

    /**
     *      is the resource opened for reading ?
     */
    virtual bool Opened( void ) const;

    /**
     *      read a line of text
     */
    virtual CString ReadLine( void );

    /**
     *      Reads a string from the resource
     */
    virtual CString ReadString( void );

    /**
     *      Attempts to read the specified number of bytes from the recourse
     */
    virtual UInt32 Read( void *dest      ,
                         UInt32 esize    ,
                         UInt32 elements );

    /**
     *      Attempts to write the specified number of bytes to the recourse
     *      using srcdata as the data source.
     */
    virtual UInt32 Write( const void* srcdata ,
                          UInt32 esize        ,
                          UInt32 elements     );

    /**
     *      Get the current offset in bytes
     */
    virtual UInt32 Tell( void ) const;

    /**
     *      Get the size of the resource in bytes
     */
    virtual UInt32 GetSize( void ) const;

    /**
     *      jump to a different part of the resource
     */
    virtual Int32 Seek( Int32 offset ,
                        Int32 origin );

    /**
     *      jump to the given offset in the resource
     */
    virtual UInt32 Setpos( UInt32 position );

    /**
     *      Read a single character
     */
    virtual char GetChar( void );

    /**
     *      are we at the end of the resource ?
     */
    virtual bool Eof( void ) const;

    virtual bool IsReadable( void ) const;

    /**
     *      Is the only allowed access method writing ?
     *      if true the resource cannot be read.
     */
    virtual bool IsWriteable( void ) const;

    /**
     *      Is the access to the resource a valid one or
     *      has something gone wrong ?
     */
    virtual bool IsValid( void );

    virtual void Flush( void );

    virtual CICloneable* Clone( void ) const;

    private:

    UInt32 m_carat;
    CDynamicBuffer* m_buffer;
    bool m_deleteBufferUponDestruction;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-07-2006 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
