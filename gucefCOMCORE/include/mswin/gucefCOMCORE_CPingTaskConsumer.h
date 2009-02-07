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

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_CBUSYWAITPULSEGENERATORDRIVER_H
#include "gucefCORE_CBusyWaitPulseGeneratorDriver.h"
#define GUCEF_CORE_CBUSYWAITPULSEGENERATORDRIVER_H
#endif /* GUCEF_CORE_CBUSYWAITPULSEGENERATORDRIVER_H ? */

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COMCORE_CPING_H
#include "CPing.h"
#define GUCEF_COMCORE_CPING_H
#endif /* GUCEF_COMCORE_CPING_H ? */

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
    
    typedef std::vector< CString > TStringVector;
    
    class CPingTaskData : public CORE::CICloneable
    {
        CPingTaskData( const TStringVector& remoteHosts    ,
                       const UInt32 maxPings = 0           ,
                       const UInt32 bytesToSend = 32       ,
                       const UInt32 timeout = 1000         ,
                       const UInt32 minimalPingDelta = 500 );
        
        CPingTaskData( const CPingTaskData& src );
        
        virtual CORE::CICloneable* Clone( void );
        
        void SetRemoteHosts( const TStringVector& hostList );
        
        const TStringVector& GetRemoteHosts( void ) const;
        
        void SetMaxPings( const UInt32 maxPings );
        
        UInt32 GetMaxPings( void ) const;
        
        void SetBytesToSend( const UInt32 bytesToSend );
        
        UInt32 GetBytesToSend( void ) const;
        
        void SetTimeout( const UInt32 timeout );
        
        UInt32 GetTimeout( void ) const;
        
        void SetMinimalPingDelta( const minimalPingDelta );
        
        const UInt32 GetMinimalPingDelta( void ) const;
        
        private:
        
        TStringVector m_hostList;
        UInt32 m_maxPings;
        UInt32 m_bytesToSend;
        UInt32 m_timeout;
        UInt32 m_minimalPingDelta;
    }
    
    CPingTaskConsumer( void );
    
    virtual ~CPingTaskConsumer();
    
    const CString& GetTaskName( void ) const;

    virtual CString GetType( void ) const;
    
    static const CString& GetTypeString( void );
    
    virtual bool ProcessTask( CORE::CICloneable* taskData );
    
    private:
    
    CPatchTaskConsumer( const CPatchTaskConsumer& src  );
    CPatchTaskConsumer& operator=( const CPatchTaskConsumer& src );
    
    static void IcmpCallback( void* vdata );

    private:
        
    CORE::CPulseGenerator m_pulseGenerator;
    CORE::CBusyWaitPulseGeneratorDriver m_pulseDriver;
    void* m_pingEvent;
    void* m_icmpHandle;
    bool m_notDone;
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
