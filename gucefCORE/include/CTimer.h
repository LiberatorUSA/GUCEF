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

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

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

/*-------------------------------------------------------------------------*/

/**
 *  Timer implementation that attempts to provide a timing mechanism with a 
 *  millisecond resolution.
 *
 *  Note that timer calls are performed in the main application thread
 *  (Assuming you are calling the CGUCEFApplication Update() from your main thread).
 *  As such operations by handlers will suspend the message handling 
 *  in the application for their duration. Keep this in mind when hooking
 *  up a lengthy process to a timer in an GUI based application.
 *
 *  Note that the timer resolution is NOT guaranteed. An attempt is made to
 *  provide the theoretical minimum resolution of 1 millisecond. 
 */
class GUCEFCORE_EXPORT_CPP CTimer : public CNotifier
{
    public:
    
    static const CEvent TimerStartedEvent;
    static const CEvent TimerUpdateEvent;
    static const CEvent TimerStoppedEvent;
    static const CEvent TimerIntervalChangedEvent;
    
    struct STimerUpdateData
    {
        UInt64 tickCount;
        Float64 updateDeltaInMilliSecs;
    };
    typedef struct STimerUpdateData TTimerUpdateData;
    typedef CTCloneableObj< TTimerUpdateData > TimerUpdateEventData;
    typedef TCloneableUInt32 TimerIntervalChangedEventData;
        
    static void RegisterEvents( void );
    
    public:
    
    explicit CTimer( const UInt32 updateDeltaInMilliSecs = 10 );        
    
    CTimer( const CTimer& src );
    
    ~CTimer();    

    CTimer& operator=( const CTimer& src );
    
    void SetInterval( const UInt32 updateDeltaInMilliSecs );
    
    UInt32 GetInterval( void ) const;
    
    void SetEnabled( const bool enabled );
    
    bool GetEnabled( void ) const;
    
    /**
     *  Returns the tick count since the start of the timer.
     *  
     *  Note that reseting the timer alters the timer count 
     *  but not the run ticks since the timer didn't actually stop.
     */
    UInt64 GetRunTicks( void ) const;
    
    Float64 GetRunTimeInMilliSecs( void ) const;

    /**
     *  Returns the actual tick count of the timer.
     *
     *  Note that the tick count gets reset by either the start 
     *  of a timer or trough the use of an explicit Reset() call.
     */
    UInt64 GetTickCount( void ) const;
    
    Float64 GetTimeInMilliSecs( void ) const;
	
	void Reset( void );
	
	/**
	 *  Returns the approximated maximum timer resolution in milliseconds
	 *
	 *  Note that this is the theoretical maximum resolution of the timer in milliseconds
	 *  Application update delays and timer logic will increase the actual maximum resolution.
	 *
	 *  @return approximated maximum timer resolution in milliseconds
	 */
	static Float64 GetApproxMaxTimerResolutionInMilliSecs( void );
    
    private:
    friend class CTimerPump;

    void OnUpdate( void );
    
    private:

    CTimerPump* m_timerPump;
    Float64 m_timerFreq;
    UInt64 m_lastTimerCycle;
    UInt64 m_tickCount;
    UInt32 m_updateDeltaInMilliSecs;
    UInt64 m_activationTickCount;
    bool m_enabled;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section.
        - Dinand: Converted class from callback interface based mechanics to 
          notification

-----------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTIMER_H  ? */
