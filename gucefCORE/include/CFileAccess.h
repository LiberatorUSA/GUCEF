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

#ifndef GUCEF_CORE_CFILEACCESS_H
#define GUCEF_CORE_CFILEACCESS_H 

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

class GUCEFCORE_EXPORT_CPP CFileAccess : public CIOAccess
{
        public:
        
        CFileAccess( const CString& file     ,
                     const char* mode = "rb" );

        virtual ~CFileAccess();

        /**
         *      open the resource for I/O
         */
        virtual void Open( void );

        /**
         *      close the recource
         */
        virtual void Close( void );

        /**
         *      is the recource opened for reading ?
         */
        virtual bool Opened( void ) const;

        /**
         *      read a line of text
         */
        virtual CString ReadLine( void );

        /**
         *      Reads a string from the recource
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

        virtual UInt32 Write( CIOAccess& sourceData );

        /**
         *      Get the current offset in bytes
         */
        virtual UInt32 Tell( void ) const;
        
        /**
         *      Get the size of the recource in bytes
         */        
        virtual Int32 GetSize( void ) const;

        /**
         *      jump to a different part of the recource
         */
        virtual Int32 Seek( Int32 offset ,
                            Int32 origin );

        /**
         *      jump to the given offset in the recource
         */
        virtual UInt32 Setpos( UInt32 position );

        /**
         *      Read a single character
         */
        virtual char GetChar( void );

        /**
         *      are we at the end of the recource ?
         */
        virtual bool Eof( void ) const;
        
        virtual bool IsReadOnly( void ) const;
        
        /**
         *      Is the only allowed access method writing ?
         *      if true the recource cannot be read.
         */
        virtual bool IsWriteOnly( void ) const;
        
        /**
         *      Are both read AND write operations possible on the recource ?         
         */
        virtual bool IsReadAndWrite( void ) const;        
        
        /**
         *      Is the access to the resource a valid one or
         *      has something gone wrong ?
         */
        virtual bool IsValid( void );
        
        virtual void Flush( void );
        
        virtual CICloneable* Clone( void ) const;
        
        const CString& GetFilename( void ) const;
        
        private:
        CFileAccess( void );

        bool _writeable;
        bool _readable;
        CString m_mode;        
        FILE* _file;
        CString _filename;
        UInt32 _size;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CFILEACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 25-08-2005 :
        - Replaced the read/write bools with a const char* for a mode.
          This allows for more options plus a more fopen() like parameter list
- 26-04-2005 :
        - Fixed an bug in Close(): Calling Close() twice caused an access violation
- 26-03-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
