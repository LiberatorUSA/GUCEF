/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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
 
#ifndef GUCEF_CORE_CTASKDELEGATOR_H
#define GUCEF_CORE_CTASKDELEGATOR_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CACTIVEOBJECT_H
#include "gucefMT_CActiveObject.h"
#define GUCEF_MT_CACTIVEOBJECT_H
#endif /* GUCEF_MT_CACTIVEOBJECT_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CTNUMERICID_H
#include "CTNumericID.h"
#define GUCEF_CORE_CTNUMERICID_H
#endif /* GUCEF_CORE_CTNUMERICID_H ? */

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

class CTaskManager;
class CTaskConsumer;

/*-------------------------------------------------------------------------*/

/** 
 *  This is an internally used class for the CTaskManager class.
 *  It is not meant to be used beyond that use-case.
 */
class GUCEF_CORE_EXPORT_CPP CTaskDelegator : public MT::CActiveObject
{ 
    public:
    
    typedef CTNumericID< UInt32 > TTaskID;
    
    CTaskManager& GetTaskManager( void ) const;
    
    CTaskConsumer* GetTaskConsumer( void );
    
    protected:
    friend class CTaskManager;
    
    CTaskDelegator( void );
    
    virtual ~CTaskDelegator();
    
    virtual bool OnTaskStart( void* taskdata );

    virtual bool OnTaskCycle( void* taskdata );

    virtual void OnTaskEnd( void* taskdata );    
    
    void PerformTaskCleanup( CTaskConsumer* taskConsumer ,
                             CICloneable* taskData        ) const;
                             
    void SetTaskData( TTaskID& taskId             ,
                      CTaskConsumer* taskConsumer );

    private:
    
    CTaskDelegator( const CTaskDelegator& src );
    CTaskDelegator& operator=( const CTaskDelegator& src );
    
    private:
    
    CTaskConsumer* m_taskConsumer;
    CTaskManager* m_taskManager;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTASKDELEGATOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-02-2005 :
        - Dinand: Added this class, it is based on some older C implementation
          I made once. Ported but not tested.

----------------------------------------------------------------------------*/
