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

#include <set>
#include "CEventPumpClient.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTIMERPUMP_CPP
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

class CTimer;
class CGUCEFCOREModule;

/*-------------------------------------------------------------------------*/

class EXPORT_CPP CTimerPump : public CEventPumpClient
{
    public:
    
    static CTimerPump* Instance( void );
    
    protected:
    
    virtual void OnUpdate( const UInt32 tickCount  ,
                           const UInt32 deltaTicks );
    
    private:
    friend class CTimer;
    
    void RegisterTimer( CTimer* timer );
    
    void UnregisterTimer( CTimer* timer );
    
    private:
    friend class CGUCEFCOREModule;
    
    static void Deinstance( void );   
    
    private:
    
    CTimerPump( void );
    
    CTimerPump( const CTimerPump& src );
    
    CTimerPump& operator=( const CTimerPump& src );
    
    virtual ~CTimerPump();
    
    private:
    typedef std::set< CTimer* > TTimerList;
    
    static CTimerPump* m_instance;
    
    TTimerList m_timerList;
    UInt32 m_minimalResolution;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTIMERPUMP_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTIMERPUMP_H  ? */