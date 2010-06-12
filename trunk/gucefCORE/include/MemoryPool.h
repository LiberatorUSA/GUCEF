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

#ifndef GUCEF_CORE_CMEMORYPOOL_H
#define GUCEF_CORE_CMEMORYPOOL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_ETypes.h"

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
 *  Abstract base class for memory pool implementations.
 *  Provides a basic interface of manditory functionality and
 *  certain optional functionality. By default all strategy
 *  specific member functions simply call the default version.
 *  Descending classes should implement at least one of the
 *  allocation strategies and direct the Allocate() member
 *  function to the member function for the chosen default
 *  strategy for that specific pool implementation.
 */
class GUCEF_CORE_PUBLIC_CPP CMemoryPool
{
	public:

    /**
     *  Does nothing special at the moment
     */
    CMemoryPool( void );

    /**
     *  Does nothing special at the moment
     */
    virtual ~CMemoryPool();

    /**
     *  Standard memory allocation strategy.
     *  Implementation is manditory for descending classes.
     */
	virtual void* Allocate( unsigned long bytesize ) = 0;

    /**
     *  Override with a specific handler if desired.
     *  At this level the default version is simply called
     */
    virtual void* AllocateBestFit( unsigned long bytesize );

    /**
     *  Override with a specific handler if desired.
     *  At this level the default version is simply called
     */
    virtual void* AllocateFirstFit( unsigned long bytesize );

    /**
     *  Override with a specific handler if desired.
     *  At this level the default version is simply called
     */
    virtual void* AllocateWorstFit( unsigned long bytesize );

    /**
     *  Special case Allocate member function which is meant to
     *  provide an interface for reclaiming a previously used
     *  pool segment.
     *
     *  @param bytesize size of the memory block you wish to allocate
     *  @param preferredaddress the preferred block starting address
     *  @param allowotheraddress whether to allow allocation at a different
     *  @param allowotheraddress address if the given one is not available
     */
    virtual void* Allocate( unsigned long bytesize ,
                            void* preferredaddress ,
                            bool allowotheraddress ) = 0;

    /**
     *  Standard memory deallocation strategy.
     *  Implementation is manditory for descending classes.
     */
	virtual void Deallocate( void* ptr ) = 0;

    /**
     *  Override with a specific handler if desired.
     *  At this level the default version is simply called
     */
    virtual void DeallocateLogicalUnit( void* ptr );

    /**
     *  Override with a specific handler if desired.
     *  At this level the default version is simply called
     */
    virtual void DeallocateAndMerge( void* ptr );

    /**
     *  Deallocates everything that was allocated in the pool
     *  object's buffer. Use this functionality with care because
     *  any outstanding pointers will be invalidated !!!
     */
    virtual void Empty( void ) = 0;

    /**
     *  Set whether to keep the pool memory allocated even
     *  if all used blocks are deallocated.
     */
    virtual void SetAlwaysClaimMemory( bool claimalways ) = 0;

    /**
     *  Returns the state of the always claim flag which indicates
     *  whether the pool's hold on it's memory buffer is maintaned
     *  even though there are no longer any outstanding allocations.
     */
    virtual bool GetAlwaysClaimMemory( void ) const = 0;

    /**
     *  Simply returns the current size of the memory pool
     */
    virtual unsigned long GetPoolSize( void ) const = 0;

    /**
     *  Simply returns the number of bytes allocated in the memory pool
     */
    virtual unsigned long GetUsedBytes( void ) const = 0;

    /**
     *  Returns the number of free useable bytes.
     *
     *  For example: Certain indexing methods may add an overhead cost per allocation
     *  reducing the number of useable bytes in a non 1:1 ratio.
     *
     *  Note that not all of the bytes returned may be useable. As a guideline you
     *  should assume that this is the number of usable bytes when all remaining
     *  memory is allocated using 1 byte allocations.
     */
    virtual unsigned long GetFreeBytes( void ) const = 0;

    /**
     *  As the name suggests it simply returns the size of the
     *  largest free block. No garantee is given on the speed and/or
     *  efficiency of this member function.
     */
    virtual unsigned long GetLargestFreeBlockSize( void ) const = 0;

    private:

    CMemoryPool( const CMemoryPool& src );            /**< private because: sanity check, why would you want to copy a memory pool */
    CMemoryPool& operator=( const CMemoryPool& src ); /**< private because: sanity check, why would you want to copy a memory pool */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CMEMORYPOOL_H ? */
