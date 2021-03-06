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

#ifndef GUCEF_CORE_CIOACCESS_H
#define GUCEF_CORE_CIOACCESS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdio.h>

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"          /* GUCEF string implementation */
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_IOACCESS_H
#include "ioaccess.h"           /* C implementation wrapped by this class */
#define GUCEF_CORE_IOACCESS_H
#endif /* GUCEF_CORE_IOACCESS_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

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

/**
 *      Abstract base class for media independent resource access
 */
class GUCEF_CORE_PUBLIC_CPP CIOAccess : public CICloneable
{
        public:

        CIOAccess( void );

        CIOAccess( const CIOAccess& src );

        virtual ~CIOAccess();

        CIOAccess& operator=( const CIOAccess& src );

        /**
         *      open the resource for I/O
         */
        virtual void Open( void ) = 0;

        /**
         *      close the resource
         */
        virtual void Close( void ) = 0;

        /**
         *      is the resource opened for reading ?
         */
        virtual bool Opened( void ) const = 0;

        /**
         *      read a line of text
         *      The delimiter is the platform text file line
         *      delimiter.
         */
        virtual CString ReadLine( void ) = 0;

        /**
         *      reads data until the specified delimiter is reached.
         *      The data is written into the destination buffer until the
         *      delimiter is reached or the end of the file is reached.
         *      The delimiter itself is not written to the destination buffer
         *
         *      @param dest the destination buffer for the data
         *      @param bsize size of the destination buffer
         *      @param delimiter data segment delimiter
         *      @param size of the data segment delimiter
         *      @return number of bytes written into dest
         */
        UInt32 ReadUntill( void *dest            ,
                           UInt32 bsize          ,
                           const void* delimiter ,
                           UInt32 delimsize      );

        /**
         *      Skips bytesuntil the delimiter is reached or
         *      until the end of the file is reached.
         *      the actual number of bytes skipped is returned.
         *
         *      @param delimiter the delimiter bytes
         *      @param delimsize size of the delimiter segment
         *      @return the actual number of bytes skipped.
         */
        UInt32 SkipUntill( const void* delimiter ,
                           UInt32 delimsize      );

        /**
         *      Reads a string from the resource
         */
        virtual CString ReadString( void ) = 0;

        /**
         *      Attempts to read the specified number of bytes from the resource
         */
        virtual UInt32 Read( void *dest      ,
                             UInt32 esize    ,
                             UInt32 elements ) = 0;

        UInt32 Read( CDynamicBuffer& dest ,
                     UInt32 esize         ,
                     UInt32 elements      );

        UInt32 Read( CDynamicBuffer& dest ,
                     UInt32 esize         );

        /**
         *      Attempts to write the specified number of bytes to the resource
         *      using srcdata as the data source.
         */
        virtual UInt32 Write( const void* srcdata ,
                              UInt32 esize        ,
                              UInt32 elements     );

        virtual UInt32 Write( CIOAccess& sourceData );
        
        virtual UInt32 Write( const CString& string );

        /**
         *      Get the current offset in bytes
         */
        virtual UInt32 Tell( void ) const = 0;

        /**
         *      jump to a different part of the resource
         */
        virtual Int32 Seek( Int32 offset ,
                            Int32 origin ) = 0;

        /**
         *      jump to the given offset in the resource
         */
        virtual UInt32 Setpos( UInt32 position );

        /**
         *      Read a single character
         */
        virtual char GetChar( void ) = 0;

        /**
         *      are we at the end of the resource ?
         */
        virtual bool Eof( void ) const = 0;

        /**
         *      Can we read from this resource?
         */
        virtual bool IsReadable( void ) const = 0;

        /**
         *      Can we write to this resource?
         */
        virtual bool IsWriteable( void ) const = 0;

        /**
         *      Is the access to the resource a valid one or
         *      has something gone wrong ?
         */
        virtual bool IsValid( void ) = 0;

        /**
         *  @return returns the size of the resource if possible.
         */
        virtual UInt32 GetSize( void ) const;

        /**
         *  Flushes all outstanding mutations on the I/O device
         */
        virtual void Flush( void ) = 0;

        virtual TIOAccess* CStyleAccess( void );

        private:

        void LinkCStyleAccess( void );

        private:

        TIOAccess m_cStyleAccess;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIOACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 01-05-2005 :
        - Added ReadUntill()
          This member function will read data untill the given delimiter is
          reached or the end of the file is reached.
- 25-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
