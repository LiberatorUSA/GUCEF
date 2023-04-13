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

#ifndef GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H
#define GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CIDATANODESERIALIZABLE_H
#include "gucefCORE_CIDataNodeSerializable.h"
#define GUCEF_CORE_CIDATANODESERIALIZABLE_H
#endif /* GUCEF_CORE_CIDATANODESERIALIZABLE_H ? */

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
 *  Interface class for all task data which is data node serializable
 */
class GUCEF_CORE_PUBLIC_CPP CIDataNodeSerializableTaskData : public CIDataNodeSerializable
{
    public:

    CIDataNodeSerializableTaskData( void );

    CIDataNodeSerializableTaskData( const CIDataNodeSerializableTaskData& src );

    virtual ~CIDataNodeSerializableTaskData();

    CIDataNodeSerializableTaskData& operator=( const CIDataNodeSerializableTaskData& src );

    /**
     *  Provides the name of the task type to which this data applies
     *  In order for a task to use data defined such as this it needs to be provided to a consumer of the correct type
     *  The task type provided in the data should match the type name under which the data factory and consumer factory are registered
     */
    virtual const CString& GetTaskTypeName( void ) const = 0;

    /**
     *  Provides the name of the thread pool where this task is desired to be executed
     *  If no name is provided via an empty string the assumption will be made that the default thread pool is sufficient
     */
    virtual const CString& GetThreadPoolName( void ) const = 0;

    /**
     *  Provides a flag whether it is Ok from a requestor standpoint to dynamically create a new threadpool if the referenced pool does not yet exist
     *  threadpool / task manager settings also influence whether new pool creation will in the end be permitted or not
     */
    virtual bool GetOnlyUseExistingThreadPool( void ) const = 0;

    /**
     *  Provides a flag whether it is Ok from a requestor standpoint to queue the work until a thread is available to perform the work.
     *  If this is not the case it will depend on threadpool / task manager settings whether an immediate execution of the work will be permitted
     */
    virtual bool GetTaskCanBeQueued( void ) const = 0;

    /**
     *  Provides the ID of the task.
     *  A value of 0 means uninitialized / not associated with a task (as of yet)
     */
    virtual UInt32 GetTaskId( void ) const = 0;
};

/*-------------------------------------------------------------------------*/

typedef CTBasicSharedPtr< CIDataNodeSerializableTaskData, MT::CMutex >   CIDataNodeSerializableTaskDataBasicPtr;
typedef CTSharedPtr< CIDataNodeSerializableTaskData, MT::CMutex >        CIDataNodeSerializableTaskDataPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H ? */
