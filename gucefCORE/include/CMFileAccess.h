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

#ifndef GUCEF_CORE_CMFILEACCESS_H
#define GUCEF_CORE_CMFILEACCESS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_MFILE_H
#include "MFILE.h"
#define GUCEF_CORE_MFILE_H
#endif /* GUCEF_CORE_MFILE_H ? */

#ifndef GUCEF_CORE_CROIOACCESS_H
#include "CROIOAccess.h"        /* base class for all RO media manipulators */
#define GUCEF_CORE_CROIOACCESS_H
#endif /* GUCEF_CORE_CROIOACCESS_H ? */

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

class GUCEFCORE_EXPORT_CPP CMFileAccess : public CROIOAccess
{
        public:
        
        CMFileAccess( const void* memblock ,
                      const UInt32 size    );
                      
        CMFileAccess( const CMFileAccess& src );                      

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
         *      jump to a different part of the resource
         */
        virtual Int32 Seek( Int32 offset ,
                            Int32 origin );

        /**
         *      jump to the given offset in the resource
         */
        virtual UInt32 Setpos( UInt32 position );
        
        /**
         *      returns the size of the resource in bytes
         */
        virtual UInt32 GetSize( void ) const;         

        /**
         *      Read a single character
         */
        virtual char GetChar( void );

        /**
         *      are we at the end of the resource ?
         */
        virtual bool Eof( void ) const;       
        
        /**
         *      Is the access to the resource a valid one or
         *      has something gone wrong ?
         */
        virtual bool IsValid( void );

        virtual TIOAccess* CStyleAccess( void );
        
        virtual CICloneable* Clone( void ) const;
        
        private:
        CMFileAccess( void );
        
        MFILE _mfile;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CMFILEACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
