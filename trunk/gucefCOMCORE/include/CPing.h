/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CPING_H
#define GUCEF_COMCORE_CPING_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"       /* build defines */
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"                    /* our event class */
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#ifndef GUCEF_CORE_CFORWARDINGNOTIFIER_H
#include "gucefCORE_CForwardingNotifier.h"
#define GUCEF_CORE_CFORWARDINGNOTIFIER_H
#endif /* GUCEF_CORE_CFORWARDINGNOTIFIER_H ? */

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
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class that implements ping functionality for the target platform
 *  You can ping a remote host and an event will be emitted when a response is
 *  received. If no response is received a timeout will occur.
 */
class GUCEF_COMCORE_EXPORT_CPP CPing : public CORE::CForwardingNotifier
{
    public:
    
    static const CORE::CEvent PingStartedEvent;
    static const CORE::CEvent PingReponseEvent;
    static const CORE::CEvent PingTimeoutEvent;
    static const CORE::CEvent PingFailedEvent;
    static const CORE::CEvent PingStoppedEvent;

    typedef CORE::TCloneableUInt32  TPingReponseEventData;
    
    static void RegisterEvents( void );
    
    public:
    
    CPing( void );
    
    virtual ~CPing();

    /**
     *  
     */
    bool Start( const CORE::CString& remoteHost     ,
                const UInt32 maxPings = 0           ,
                const UInt32 bytesToSend = 32       ,
                const UInt32 timeout = 1000         ,
                const UInt32 minimalPingDelta = 500 );

    void Stop( void );
    
    bool IsActive( void ) const;
    
    const CORE::CString& GetRemoteHost( void ) const;
    
    UInt32 GetMaxPings( void ) const;
    
    void SetUserData( void* userData );
    
    void* GetUserData( void ) const;
    
    private:
    
    CPing( const CPing& src );
    CPing& operator=( const CPing& src );
    
    private:
    
    bool m_isActive;
    CORE::CString m_remoteHost;
    UInt32 m_maxPings;
    UInt32 m_bytesToSend;
    UInt32 m_timeout;
    UInt32 m_minimalPingDelta;
    void* m_osData;
    void* m_userData;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//


---------------------------------------------------------------------------*/