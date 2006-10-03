/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
 *  Application sub-systems can derive from this class to recieve update
 *  pulses from the main application 'engine'.
 *  This class automaticly registers at the CGUCEFApplication singleton.
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
    
    void RequestUpdateInterval( const UInt32 deltaTicks );
    
    protected:
    friend class CGUCEFApplication;
    
    virtual void OnUpdate( const UInt32 applicationTicks ,
                           const UInt32 deltaTicks       );

    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const UInt32 eventid          ,
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
