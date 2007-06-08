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

#ifndef GUCEF_CORE_COBSERVERPUMP_H
#define GUCEF_CORE_COBSERVERPUMP_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>
#include "CGUCEFAppSubSystem.h"

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVERPUMP_CPP
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

class CPumpedObserver;
class CGUCEFCOREModule;

/*-------------------------------------------------------------------------*/

class GUCEFCORE_EXPORT_CPP CObserverPump : public CGUCEFAppSubSystem
{
    public:
    
    static CObserverPump* Instance( void );
    
    protected:
    
    virtual void OnUpdate( const UInt64 tickCount               ,
                           const Float64 updateDeltaInMilliSecs );

    private:
    friend class CPumpedObserver;
    
    void RegisterObserver( CPumpedObserver* observer );
    
    void UnregisterObserver( CPumpedObserver* observer );

    void RequestNewPumpCycle( void );
    
    private:
    friend class CGUCEFCOREModule;
    
    static void Deinstance( void );   
    
    private:
    
    CObserverPump( void );
    
    CObserverPump( const CObserverPump& src );
    
    CObserverPump& operator=( const CObserverPump& src );
    
    virtual ~CObserverPump();    
    
    private:
    typedef std::set<CPumpedObserver*> TObserverList;
    
    static CObserverPump* m_instance;
    
    TObserverList m_observerList;
    MT::CMutex m_dataLock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVERPUMP_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_COBSERVERPUMP_H  ? */