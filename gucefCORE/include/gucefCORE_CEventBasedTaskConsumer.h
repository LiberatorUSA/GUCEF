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

#ifndef GUCEF_CORE_CEVENTBASEDTASKCONSUMER_H
#define GUCEF_CORE_CEVENTBASEDTASKCONSUMER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

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
 *  If you want to do event based processing within a thread then this class
 *  offers a convenience interface beyond that of the CITaskConsumer interface
 */
class GUCEF_CORE_PUBLIC_CPP CEventBasedTaskConsumer : public CITaskConsumer
{
    public:

    struct SEventBasedTaskData
    {
        CNotifier* notifier;
        CEvent eventID;
        CICloneable* eventData;
    };
    typedef struct SEventBasedTaskData TEventBasedTaskData;
    typedef CTCloneableObj< TEventBasedTaskData > TCloneableEventBasedTaskData;

    virtual ~CEventBasedTaskConsumer();

    /**
     *  Implements ProcessTask() and extracts the event information
     *  from the task data. Note that taskData is expected to be of type
     *  TCloneableEventBasedTaskData
     */
    virtual bool ProcessTask( CICloneable* taskData );

    virtual bool ProcessTask( CNotifier* notifier    ,
                              const CEvent eventID   ,
                              CICloneable* eventData ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CEVENTBASEDTASKCONSUMER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-02-2005 :
        - Dinand: Added this class

----------------------------------------------------------------------------*/
