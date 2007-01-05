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

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

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
 *  received. If no response is sent a timeout will occur.
 */
class GUCEF_COMCORE_EXPORT_CPP CPing : public CORE::CObservingNotifier
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
    bool Start( const CORE::CString& remoteHost ,
                const UInt32 maxPings = 0       ,
                const UInt32 bytesToSend = 32   ,
                const UInt32 timeout = 1000     );

    void Stop( void );
    
    bool IsActive( void ) const;
    
    const CORE::CString& GetRemoteHost( void ) const;
    
    UInt32 GetMaxPings( void ) const;
    
    private:
    
    CPing( const CPing& src );
    CPing& operator=( const CPing& src );
    
    private:
    
    bool m_isActive;
    CORE::CString m_remoteHost;
    UInt32 m_maxPings;
    UInt32 m_bytesToSend;
    UInt32 m_timeout;
    void* m_osData;
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