/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef GUCEF_CORE_CTIMER_H
#define GUCEF_CORE_CTIMER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>
#include "gucefCORE_ETypes.h"

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"           /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTIMER_CPP
    #pragma warning( push )
#endif

#pragma warning( disable: 4251 ) // 'classname' needs to have dll-interface to be used by clients of class 'classname'
#pragma warning( disable: 4284 ) // return type for operator -> is 'const *' (ie; not a UDT or reference to a UDT).
#pragma warning( disable: 4786 ) // identifier was truncated to 'number' characters

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

class CTimerPump;
class CITimerClient;

/*-------------------------------------------------------------------------*/

/**
 *  Timer implementation that attempts to provide a timing mechanism with a 
 *  millisecond resolution.
 *
 *  Note that timer calls are performed in the main application thread
 *  (Assuming you are calling the CXBApplication Update() from your main thread).
 *  As such operations by handlers will suspend the message handling 
 *  in the application for their duration. Keep this in mind when hooking
 *  up a lengthy process to a timer in an GUI based application.
 *
 *  Note that the timer resolution is NOT garanteed. An attempt is made to
 *  provide the theoretical minimum resolution of 1 millisecond. 
 */
class GUCEFCORE_EXPORT_CPP CTimer
{
    public:
    
    explicit CTimer( const UInt32 interval = 10 );        
    
    CTimer( const CTimer& src );
    
    ~CTimer();    

    CTimer& operator=( const CTimer& src );
    
    void SetInterval( const UInt32 interval );
    
    UInt32 GetInterval( void ) const;
    
    void SetEnabled( const bool enabled );
    
    bool GetEnabled( void ) const;
    
    /**
     *  Returns the tickcount since the start of the timer.
     *  
     *  Note that reseting the timer alters the timer count 
     *  but not the run ticks since the timer didn't actually stop.
     */
    UInt32 GetRunTicks( void ) const;

    /**
     *  Returns the current tickcount of the timer.
     *  The tickcount gets reset by either the start of a timer or
     *  trough the use of an explicit Reset() call.
     */
    UInt32 GetTickCount( void ) const;
	
	void Reset( void );
    
    void Subscribe( CITimerClient* client );
    
    void Unsubscribe( CITimerClient* client );

    private:
    friend class CTimerPump;

    void OnUpdate( const UInt32 tickCount  ,
                   const UInt32 deltaTicks );
    
    private:
    typedef std::set< CITimerClient* > TTimerClientSet;
    
    TTimerClientSet m_clients;
    UInt32 m_lastTimerCycle;
    UInt32 m_tickCount;
    UInt32 m_interval;
    UInt32 m_activationTickCount;
    bool m_enabled;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTIMER_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTIMER_H  ? */
