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

#ifndef CMFILEACCESS_H
#define CMFILEACCESS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef MFILE_H
#include "MFILE.h"
#define MFILE_H
#endif /* MFILE_H ? */

#ifndef CIOACCESS_H
#include "CIOAccess.h"        /* base class for all media manipulators */
#define CIOACCESS_H
#endif /* CIOACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
CORE_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEFCORE_EXPORT_CPP CMFileAccess : public CIOAccess
{
        public:
        
        CMFileAccess( const void* memblock ,
                      UInt32 size          );

        virtual ~CMFileAccess();

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
         *      Get the current offset in bytes
         */
        virtual UInt32 Tell( void ) const;

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

        virtual TIOAccess* CStyleAccess( void );
        
        private:
        CMFileAccess( void );
        
        MFILE _mfile;
        TIOAccess _access;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CORE_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CMFILEACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
