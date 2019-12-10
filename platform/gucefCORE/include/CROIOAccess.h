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

#ifndef GUCEF_CORE_CROIOACCESS_H
#define GUCEF_CORE_CROIOACCESS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"          /* abstract base for IO Access */
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

/**
 *      Abstract base class for read-only media independant recource access
 *      Read-only media access implementations can inherit from this class
 *      to avoid having to implement the write interface for each decending
 *      implementation.
 */
class GUCEF_CORE_PUBLIC_CPP CROIOAccess : public CIOAccess
{
        public:

        CROIOAccess( void );

        virtual ~CROIOAccess();

        /**
         *      Is the only allowed access method reading ?
         *      If true the recource cannot be written to
         *      Always returns true
         */
        virtual bool IsReadable( void ) const;

        /**
         *      Is the only allowed access method writing ?
         *      if true the recource cannot be read.
         *      Always returns false
         */
        virtual bool IsWriteable( void ) const;

        /**
         *      Attempts to write the specified number of bytes to the recourse
         *      using srcdata as the data source.
         *      Always returns 0, no actual write operation will be performed.
         */
        virtual UInt32 Write( const void* srcdata ,
                              UInt32 esize        ,
                              UInt32 elements     );

        virtual void Flush( void );

        protected:

        TIOAccess _access;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CROIOACCESS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-05-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
