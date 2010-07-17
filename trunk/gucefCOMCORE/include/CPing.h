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

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#include "CTSGNotifier.h"
#define GUCEF_CORE_CTSGNOTIFIER_H
#endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifdef GUCEF_MSWIN_BUILD

  #ifndef GUCEF_COMCORE_CPINGTASKCONSUMER_H
  #include "gucefCOMCORE_CPingTaskConsumer.h"
  #define GUCEF_COMCORE_CPINGTASKCONSUMER_H
  #endif /* GUCEF_COMCORE_CPINGTASKCONSUMER_H ? */
  
#endif /* GUCEF_MSWIN_BUILD ? */

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
class GUCEF_COMCORE_EXPORT_CPP CPing : public CORE::CTSGNotifier
{
    public:
    
    static const CORE::CEvent PingStartedEvent;
    static const CORE::CEvent PingReponseEvent;
    static const CORE::CEvent PingTimeoutEvent;
    static const CORE::CEvent PingFailedEvent;
    static const CORE::CEvent PingStoppedEvent;
    
    static void RegisterEvents( void );
    
    class GUCEF_COMCORE_EXPORT_CPP CPingEventData : public CORE::CICloneable
    {
        public: 
        
        CPingEventData( const CHostAddress& host   ,
                        const UInt32 echoSize      ,
                        const UInt32 roundTripTime );

        CPingEventData( const CPingEventData& src );
        
        virtual ~CPingEventData();
                
        virtual CORE::CICloneable* Clone( void ) const;
        
        const CHostAddress& GetHostAddress( void ) const;
        
        UInt32 GetEchoSize( void ) const;
        
        UInt32 GetRoundTripTime( void ) const;
        
        private:
        
        CHostAddress m_hostAddress;
        UInt32 m_echoSize;
        UInt32 m_roundTripTime;
    };
    
    public:
    
    typedef std::vector< CString > TStringVector;
    
    CPing( void );
    
    CPing( CORE::CPulseGenerator& pulsGenerator );
    
    virtual ~CPing();

    /**
     *  
     */
    bool Start( const CORE::CString& remoteHost     ,
                const Int32 maxPings = 0            ,
                const UInt32 bytesToSend = 32       ,
                const UInt32 timeout = 1000         ,
                const UInt32 minimalPingDelta = 500 );

    bool Start( const TStringVector& remoteHosts    ,
                const Int32 maxPings = 0            ,
                const UInt32 bytesToSend = 32       ,
                const UInt32 timeout = 1000         ,
                const UInt32 minimalPingDelta = 500 );

    void Stop( void );
    
    bool IsActive( void ) const;
    
    const TStringVector& GetRemoteHosts( void ) const;
    
    Int32 GetMaxPings( void ) const;
    
    void SetUserData( void* userData );
    
    void* GetUserData( void ) const;
    
    protected:
    
    virtual void OnPumpedNotify( CORE::CNotifier* notifier           ,
                                 const CORE::CEvent& eventid         ,
                                 CORE::CICloneable* eventdata = NULL );
    
    private:
    
    CPing( const CPing& src );
    CPing& operator=( const CPing& src );
    
    private:
    
    bool m_isActive;
    TStringVector m_remoteHosts;
    UInt32 m_maxPings;
    UInt32 m_bytesToSend;
    UInt32 m_timeout;
    UInt32 m_minimalPingDelta;
    void* m_osData;
    void* m_userData;
    UInt32 m_taskId;
    CPingTaskConsumer m_pingTaskConsumer;
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