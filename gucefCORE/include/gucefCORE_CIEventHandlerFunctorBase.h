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

#ifndef GUCEF_CORE_CIEVENTHANDLERFUNCTORBASE_H
#define GUCEF_CORE_CIEVENTHANDLERFUNCTORBASE_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CIOBSERVER_H
#include "gucefCORE_CIObserver.h"
#define GUCEF_CORE_CIOBSERVER_H
#endif /* GUCEF_CORE_CIOBSERVER_H ? */

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
 *  Interface class for all instantiations of the CTEventHandlerFunctor template
 *  It provides an identifier and the combined observer and cloneable interface
 */
class GUCEF_CORE_EXPORT_CPP CIEventHandlerFunctorBase : public CIObserver  ,
                                                        public CICloneable
{
    public:

    CIEventHandlerFunctorBase( void );
    
    CIEventHandlerFunctorBase( const CIEventHandlerFunctorBase& src );
    
    virtual ~CIEventHandlerFunctorBase();
    
    CIEventHandlerFunctorBase& operator=( const CIEventHandlerFunctorBase& src );
    
    virtual CICloneable* Clone( void ) const = 0;
    
    protected:
    friend class CNotifierImplementor;
    
    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined user data
     */    
    virtual void OnNotify( CNotifier* notifier          ,
                           const CEvent& eventID        ,
                           CICloneable* evenData = NULL ) = 0;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIEVENTHANDLERFUNCTORBASE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
