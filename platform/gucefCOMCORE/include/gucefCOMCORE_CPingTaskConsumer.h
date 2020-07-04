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

#ifndef GUCEF_COMCORE_CPINGTASKCONSUMER_H
#define GUCEF_COMCORE_CPINGTASKCONSUMER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

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

class GUCEF_COMCORE_EXPORT_CPP CPingTaskConsumer : public CORE::CTaskConsumer
{
    public:
    
    static const CORE::CEvent PingStartedEvent;
    static const CORE::CEvent PingReponseEvent;
    static const CORE::CEvent PingTimeoutEvent;
    static const CORE::CEvent PingFailedEvent;
    static const CORE::CEvent PingStoppedEvent;
    
    static void RegisterEvents( void );
    
    class GUCEF_COMCORE_EXPORT_CPP CEchoReceivedEventData : public CORE::CICloneable
    {
        public: 
        
        CEchoReceivedEventData( const CHostAddress& host   ,
                                const UInt32 echoSize      ,
                                const UInt32 roundTripTime );

        CEchoReceivedEventData( const CEchoReceivedEventData& src );
        
        virtual ~CEchoReceivedEventData();
                
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
    
    class GUCEF_COMCORE_EXPORT_CPP CPingTaskData : public CORE::CICloneable
    {
        public:
        
        CPingTaskData( const TStringVector& remoteHosts    ,
                       const Int32 maxPings = 0            ,
                       const UInt32 bytesToSend = 32       ,
                       const UInt32 timeout = 1000         ,
                       const Int32 minimalPingDelta = 500  );
        
        CPingTaskData( const CPingTaskData& src );
        
        virtual CORE::CICloneable* Clone( void ) const;
        
        void SetRemoteHosts( const TStringVector& hostList );
        
        const TStringVector& GetRemoteHosts( void ) const;
        
        void SetMaxPings( const Int32 maxPings );
        
        Int32 GetMaxPings( void ) const;
        
        void SetBytesToSend( const UInt32 bytesToSend );
        
        UInt32 GetBytesToSend( void ) const;
        
        void SetTimeout( const UInt32 timeout );
        
        UInt32 GetTimeout( void ) const;
        
        void SetMinimalPingDelta( const UInt32 minimalPingDelta );
        
        UInt32 GetMinimalPingDelta( void ) const;
        
        private:
        
        TStringVector m_hostList;
        Int32 m_maxPings;
        UInt32 m_bytesToSend;
        UInt32 m_timeout;
        UInt32 m_minimalPingDelta;
    };
    
    CPingTaskConsumer( void );
    
    virtual ~CPingTaskConsumer();

    virtual CString GetType( void ) const;
    
    static const CString& GetTypeString( void );
    
    virtual const CString& GetClassTypeName( void ) const;
    
    virtual bool OnTaskStart( CORE::CICloneable* taskdata ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CORE::CICloneable* taskdata ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnd( CORE::CICloneable* taskdata ) GUCEF_VIRTUAL_OVERRIDE;
    
    private:
    
    CPingTaskConsumer( const CPingTaskConsumer& src );
    CPingTaskConsumer& operator=( const CPingTaskConsumer& src );
    
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    static void IcmpCallback( void* vdata );
    
    #endif

    private:
    
    struct SPingEntry
    {
        // generic ping entry attributes
        UInt32 pingCount;
        UInt64 ticksAtLastPing;
        bool areWeWaitingForPingResult;
        
        // attributes used for ping callbacks
        CPingTaskConsumer* taskConsumer;
        const CHostAddress* host;
        void* replyBuffer;
        UInt32 echoSize;
    };
    typedef struct SPingEntry TPingEntry;
    typedef std::map< CHostAddress, TPingEntry > TPingCounters;

    void* m_platformData;
    bool m_notDone;
    CPingTaskData* m_taskData;
    TPingCounters m_pingCounters;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPINGTASKCONSUMER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-12-2006 :
        - Dinand: Initial version

----------------------------------------------------------------------------*/
