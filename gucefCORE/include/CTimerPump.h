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

#ifndef GUCEF_CORE_CTIMERPUMP_H
#define GUCEF_CORE_CTIMERPUMP_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#include "CGUCEFAppSubSystem.h"
#define GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#endif /* GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H ? */

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

class CTimer;
class CGUCEFCOREModule;

/*-------------------------------------------------------------------------*/

/**
 *  Internally used class for updating timers
 */
class GUCEFCORE_EXPORT_CPP CTimerPump : public CGUCEFAppSubSystem
{
    public:
    
    static CTimerPump* Instance( void );
    
    protected:
    
    virtual void OnUpdate( const UInt64 tickCount               ,
                           const Float64 updateDeltaInMilliSecs );
    
    private:
    friend class CTimer;
    
    void RegisterTimer( CTimer* timer );
    
    void UnregisterTimer( CTimer* timer );
    
    void TimerSetRequiresUpdates( CTimer* timer              ,
                                  const bool requiresUpdates );
    
    private:
    friend class CGUCEFCOREModule;
    
    static void Deinstance( void );   
    
    private:
    
    CTimerPump( void );
    
    CTimerPump( const CTimerPump& src );
    
    CTimerPump& operator=( const CTimerPump& src );
    
    virtual ~CTimerPump();
    
    private:
    typedef std::map< CTimer*, bool > TTimerList;
    
    static CTimerPump* m_instance;
    
    TTimerList m_timerList;
    UInt32 m_minimalResolution;
    bool m_isATimerActive;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTIMERPUMP_H  ? */