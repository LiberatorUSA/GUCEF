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