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

#ifndef GUCEF_CORE_CMEMORYBLOCKPOOL_H
#define GUCEF_CORE_CMEMORYBLOCKPOOL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"               /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

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

class GUCEF_CORE_PUBLIC_CPP CMemoryBlockPool
{
        public:

    CMemoryBlockPool( void );

    CMemoryBlockPool( UInt32 capacity  ,
                      UInt32 blocksize );

    CMemoryBlockPool( const CMemoryBlockPool& src );

    ~CMemoryBlockPool();

    CMemoryBlockPool& operator=( const CMemoryBlockPool& src );

    void SetBlock( UInt32 blockindex ,
                   const void* src   ,
                   UInt32 size       );

    void SetBlock( UInt32 blockindex ,
                   const void* src   );

    void* GetBlock( UInt32 blockindex ) const;

    void SetCapacity( UInt32 capacity );

    UInt32 GetCapacity( void ) const;

    bool SetBlockSize( UInt32 blocksize ,
                       bool movedata    );

    UInt32 GetBlockSize( void ) const;

    void ShiftBlocksUp( void );

    void ShiftBlocksDown( void );

    private:
    UInt32 _capacity;
    UInt32 _blocksize;
    UInt8* _pool;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CMEMORYBLOCKPOOL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-11-2004 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
