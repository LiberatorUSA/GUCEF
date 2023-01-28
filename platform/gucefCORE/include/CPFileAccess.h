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

#ifndef GUCEF_CORE_CPFILEACCESS_H
#define GUCEF_CORE_CPFILEACCESS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_PAFILE_H
#include "PAFILE.h"          /* C implementation of paralell file access */
#define GUCEF_CORE_PAFILE_H
#endif /* GUCEF_CORE_PAFILE_H ? */

#ifndef GUCEF_CORE_CROIOACCESS_H
#include "CROIOAccess.h"     /* base class for all read-only media manipulators */
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

class CPFileChunkAccess;

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CPFileAccess : public CROIOAccess
{
        public:

        CPFileAccess( const CString& file );

        virtual ~CPFileAccess() GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      open the resource for I/O
         */
        virtual void Open( void ) GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      close the resource
         */
        virtual void Close( void ) GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      is the resource opened for reading ?
         */
        virtual bool Opened( void ) const GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      read a line of text
         */
        virtual CString ReadLine( void ) GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      Reads a string from the resource
         */
        virtual CString ReadString( void ) GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      Attempts to read the specified number of bytes from the recourse
         */
        virtual UInt32 Read( void *dest      ,
                             UInt32 esize    ,
                             UInt32 elements ) GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      Get the current offset in bytes
         */
        virtual UInt64 Tell( void ) const GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      jump to a different part of the resource
         */
        virtual Int32 Seek( Int64 offset ,
                            Int32 origin ) GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      jump to the given offset in the resource
         */
        virtual UInt32 Setpos( UInt64 position ) GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      returns the size of the resource in bytes
         */
        virtual UInt64 GetSize( void ) const GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      Read a single character
         */
        virtual char GetChar( void ) GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      are we at the end of the resource ?
         */
        virtual bool Eof( void ) const GUCEF_VIRTUAL_OVERRIDE;

        /**
         *      Is the access to the resource a valid one or
         *      has something gone wrong ?
         */
        virtual bool IsValid( void ) GUCEF_VIRTUAL_OVERRIDE;

        virtual TIOAccess* CStyleAccess( void ) GUCEF_VIRTUAL_OVERRIDE;

        virtual CORE::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

        private:
        friend class CPFileChunkAccess;

        CPFileAccess( void );

        TPAFILEControl* _pafilecontrol;
        PAFILE* _pafile;
        CString _filename;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPFILEACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
