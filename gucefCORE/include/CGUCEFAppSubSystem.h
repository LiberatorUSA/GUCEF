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

#ifndef GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#define GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"
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

class CGUCEFApplication;

/*--------------------------------------------------------------------------*/

/**
 *  Application sub-systems can derive from this class to receive update
 *  pulses from the main application 'engine'.
 *
 *  This class can automatically register at the CGUCEFApplication singleton if
 *  you wish. You can also register at a later time depending on your needs.
 *  Registering the sub-system causes you to subscribes the sub-system to application 
 *  update pulses and subscribes to notifications
 */
class GUCEFCORE_EXPORT_CPP CGUCEFAppSubSystem : public CObservingNotifier
{
    public:
    
    CGUCEFAppSubSystem( const bool registerSubSystem = false );
    
    virtual ~CGUCEFAppSubSystem();
        
    bool IsInNeedOfAnUpdate( void ) const;
    
    bool ArePeriodicUpdatesRequired( void ) const;
    
    UInt32 GetDesiredUpdateInterval( void ) const;
    
    void RegisterSubSystem( void );
    
    void UnregisterSubSystem( void );
    
    bool IsSubSystemRegistered( void ) const;
    
    protected:
    
    void RequestUpdate( void );
    
    void RequestUpdateInterval( const UInt32 updateDeltaInMilliSecs );
    
    void SetPeriodicUpdateRequirement( const bool requiresPeriodicUpdates );
    
    protected:
    friend class CGUCEFApplication;
    
    virtual void OnUpdate( const UInt64 tickCount               ,
                           const Float64 updateDeltaInMilliSecs );

    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined user data
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );
   
    private:
    
    CGUCEFAppSubSystem( const CGUCEFAppSubSystem& src );
    CGUCEFAppSubSystem& operator=( const CGUCEFAppSubSystem& src );
    
    private:
    
    UInt32 m_updateInterval;
    bool m_inNeedOfAnUpdate;
    bool m_requiresPeriodicUpdates;
    bool m_isRegistered;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
