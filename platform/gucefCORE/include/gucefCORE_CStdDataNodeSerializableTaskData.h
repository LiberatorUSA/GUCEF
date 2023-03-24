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

#ifndef GUCEF_CORE_CSTDDATANODESERIALIZABLETASKDATA_H
#define GUCEF_CORE_CSTDDATANODESERIALIZABLETASKDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H
#include "gucefCORE_CIDataNodeSerializableTaskData.h"
#define GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H
#endif /* GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H ? */

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
 *  Standard out-of-the-box implementation of a class that can hold all relevant task data 
 *  which is data node serializable. It cuts down on the amount of work involved in creating data defined tasks.
 *  Some tasks do not have any task data to speak of as its more a case of triggering logic to execute, for such cases you should be able to 
 *  use this base class as-is without the need of a decending class.
 */
class GUCEF_CORE_PUBLIC_CPP CStdDataNodeSerializableTaskData : public CIDataNodeSerializableTaskData
{
    public:

    static const CString ClassTypeName;

    CStdDataNodeSerializableTaskData( void );

    CStdDataNodeSerializableTaskData( const CStdDataNodeSerializableTaskData& src );

    virtual ~CStdDataNodeSerializableTaskData();

    CStdDataNodeSerializableTaskData& operator=( const CStdDataNodeSerializableTaskData& src );

    virtual bool Serialize( CDataNode& domRootNode                        ,
                            const CDataNodeSerializableSettings& settings ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Deserialize( const CDataNode& domRootNode                  ,
                              const CDataNodeSerializableSettings& settings ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SerializeTaskData( CDataNode& domRootNode                        ,
                                    const CDataNodeSerializableSettings& settings ) const;

    virtual bool DeserializeTaskData( const CDataNode& domRootNode                  ,
                                      const CDataNodeSerializableSettings& settings );

    /**
     *  Provides the name of the task type to which this data applies
     *  In order for a task to use data defined such as this it needs to be provided to a consumer of the correct type
     *  The task type provided in the data should match the type name under which the data factory and consumer factory are registered
     */
    virtual const CString& GetTaskTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Provides the name of the thread pool where this task is desired to be executed
     *  If no name is provided via an empty string the assumption will be made that the default thread pool is sufficient
     */
    virtual const CString& GetThreadPoolName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Provides a flag whether it is Ok from a requestor standpoint to dynamically create a new threadpool if the referenced pool does not yet exist
     *  threadpool / task manager settings also influence whether new pool creation will in the end be permitted or not
     */
    virtual bool GetOnlyUseExistingThreadPool( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Provides a flag whether it is Ok from a requestor standpoint to queue the work until a thread is available to perform the work.
     *  If this is not the case it will depend on threadpool / task manager settings whether an immediate execution of the work will be permitted
     */
    virtual bool GetTaskCanBeQueued( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    CString m_threadPoolName;
    CString m_taskTypeName;
    bool m_onlyUseExistingThreadPool;
    bool m_taskCanBeQueued;
    CDataNode m_rawTaskData;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTDDATANODESERIALIZABLETASKDATA_H ? */
