/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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

//-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------//

#include "CPing.h"           /* declaration of the class */

#ifdef GUCEF_MSWIN_BUILD

  /* #include <Icmpapi.h> -> this is the header for the functions that we dynamicly link */  
  #define WIN32_LEAN_AND_MEAN
  #include <winsock.h>                  /* windows networking API, used here for it's type declarations */
  
  #ifndef GUCEF_CORE_DVOSWRAP_H
  #include "DVOSWRAP.h"
  #define GUCEF_CORE_DVOSWRAP_H
  #endif /* GUCEF_CORE_DVOSWRAP_H ? */
  
  #ifndef GUCEF_MT_CACTIVEOBJECT_H
  #include "gucefMT_CActiveObject.h"
  #define GUCEF_MT_CACTIVEOBJECT_H
  #endif /* GUCEF_MT_CACTIVEOBJECT_H ? */

  #ifndef GUCEF_CORE_CTSGNOTIFIER_H
  #include "CTSGNotifier.h"
  #define GUCEF_CORE_CTSGNOTIFIER_H
  #endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

  #ifndef GUCEF_MT_CMUTEX_H
  #include "gucefMT_CMutex.h"
  #define GUCEF_MT_CMUTEX_H
  #endif /* GUCEF_MT_CMUTEX_H ? */
  
  #ifndef GUCEF_COMCORE_CSOCKET_H
  #include "CSocket.h"
  #define GUCEF_COMCORE_CSOCKET_H
  #endif /* GUCEF_COMCORE_CSOCKET_H ? */    

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CPing::PingStartedEvent = "GUCEF::COMCORE::CPing::PingStartedEvent";
const CORE::CEvent CPing::PingReponseEvent = "GUCEF::COMCORE::CPing::PingReponseEvent";
const CORE::CEvent CPing::PingTimeoutEvent = "GUCEF::COMCORE::CPing::PingTimeoutEvent";
const CORE::CEvent CPing::PingFailedEvent = "GUCEF::COMCORE::CPing::PingFailedEvent";
const CORE::CEvent CPing::PingStoppedEvent = "GUCEF::COMCORE::CPing::PingStoppedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

/*-------------------------------------------------------------------------*/

typedef struct ip_option_information {
  UCHAR Ttl;
  UCHAR Tos;
  UCHAR Flags;
  UCHAR OptionsSize;
  PUCHAR OptionsData;
} IP_OPTION_INFORMATION, 
 *PIP_OPTION_INFORMATION;
 
/*-------------------------------------------------------------------------*/
 
typedef struct {
  union {
    struct {
      u_char s_b1,s_b2,s_b3,s_b4;
    } S_un_b;
    struct {
      u_short s_w1,s_w2;
    } S_un_w;
    u_long S_addr;
  } S_un;
} IPAddr;

/*-------------------------------------------------------------------------*/

typedef struct icmp_echo_reply {
  IPAddr Address;
  ULONG Status;
  ULONG RoundTripTime;
  USHORT DataSize;
  USHORT Reserved;
  PVOID Data;
  IP_OPTION_INFORMATION Options;
} ICMP_ECHO_REPLY, 
 *PICMP_ECHO_REPLY;

/*-------------------------------------------------------------------------*/

typedef HANDLE ( WINAPI *TIcmpCreateFilePtr)( VOID );
typedef BOOL ( WINAPI *TIcmpCloseHandlePtr)( HANDLE IcmpHandle );
typedef DWORD ( WINAPI *TIcmpSendEchoPtr)( HANDLE IcmpHandle, IPAddr DestinationAddress, LPVOID RequestData, WORD RequestSize, PIP_OPTION_INFORMATION RequestOptions, LPVOID ReplyBuffer, DWORD ReplySize, DWORD Timeout );

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

/*-------------------------------------------------------------------------*/

/*
 *  icmp_echo_reply.Status is one of the following values
 */

#define IP_SUCCESS                  11000
#define IP_BUF_TOO_SMALL            11001
#define IP_DEST_NET_UNREACHABLE     11002
#define IP_DEST_HOST_UNREACHABLE    11003
#define IP_DEST_PROT_UNREACHABLE    11004
#define IP_DEST_PORT_UNREACHABLE    11005
#define IP_NO_RESOURCES             11006
#define IP_BAD_OPTION               11007
#define IP_HW_ERROR                 11008
#define IP_PACKET_TOO_BIG           11009
#define IP_REQ_TIMED_OUT            11010
#define IP_BAD_REQ                  11011
#define IP_BAD_ROUTE                11012
#define IP_TTL_EXPIRED_TRANSIT      11013
#define IP_TTL_EXPIRED_REASSEM      11014
#define IP_PARAM_PROBLEM            11015
#define IP_SOURCE_QUENCH            11016
#define IP_OPTION_TOO_BIG           11017
#define IP_BAD_DESTINATION          11018
#define IP_DEST_UNREACHABLE         11019
#define IP_TIME_EXCEEDED            11020
#define IP_BAD_HEADER               11021
#define IP_UNRECOGNIZED_NEXT_HEADER 11022
#define IP_ICMP_ERROR               11023
#define IP_DEST_SCOPE_MISMATCH      11024

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
MT::CMutex globalMutex;
UInt32 iphlpapiDllRefCount = 0;
void* iphlpapiDllHandle = NULL;
TIcmpCreateFilePtr IcmpCreateFile = NULL;
TIcmpCloseHandlePtr IcmpCloseHandle = NULL;
TIcmpSendEchoPtr IcmpSendEcho = NULL;
#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

/**
 *  private task class for handling ping behavior on ms windows
 */
class CMSWinPingTask : public MT::CActiveObject   ,
                       private CORE::CTSGNotifier
{
    protected:
    
    /*---------------------------------------------------------------------*/
    
    virtual bool OnTaskStart( void* taskdata )
    {TRACE;
    
        if ( m_replyBuffer == NULL )
        {
            m_icmpHandle = IcmpCreateFile();
            if ( m_icmpHandle == NULL )
            {
                // failed to create ICMP access
                return false;
            }
           
            // resolve the DNS name if we have one and get the network byte order ip
            if ( CSocket::ConvertToIPAddress( m_remoteHost ,
                                              80           ,
                                              m_ip         ) )
            {
                // Allocate the buffers we will need
                m_replyBuffer = (VOID*) malloc( m_replyBufferSize );
                m_sendBuffer = (VOID*) malloc( m_bytesToSend );
            }
            
            NotifyObserversFromThread( CPing::PingStartedEvent );
            
            return true;
        }
        return false;
    }
    
    /*---------------------------------------------------------------------*/

    virtual bool OnTaskCycle( void* taskdata )
    {TRACE;
    
        if ( ( m_maxPings == 0 )              ||
             ( m_pingsCompleted < m_maxPings ) )
        {
            // perform the actual 'ping', this is a blocking call
            DWORD dwRetVal;
            IPAddr ipAddr;
            ipAddr.S_un.S_addr = m_ip.netaddr;
            if ( ( dwRetVal = IcmpSendEcho( (HANDLE)m_icmpHandle    ,
                                            ipAddr                  ,
                                            m_sendBuffer            ,
                                            (WORD)m_bytesToSend     ,
                                            NULL                    , 
                                            m_replyBuffer           ,
                                            (WORD)m_replyBufferSize ,
                                            (DWORD)m_timeout        ) ) > 0 ) 
            {
                // Get the RTT (Round Trip Time)
                ICMP_ECHO_REPLY& reply = ( *(ICMP_ECHO_REPLY*) m_replyBuffer );
                
                if ( ( reply.Status == 0  )               ||
                     ( reply.Status == IP_SUCCESS )       || 
                     ( reply.Status == IP_SOURCE_QUENCH ) )
                {
                    // Notify our observers with a ping response event
                    CPing::TPingReponseEventData eData( reply.RoundTripTime );
                    NotifyObserversFromThread( CPing::PingReponseEvent, &eData );
                }
                else
                {
                    if ( ( reply.Status == IP_TIME_EXCEEDED ) || ( reply.Status == IP_REQ_TIMED_OUT ) )
                    {
                         NotifyObserversFromThread( CPing::PingTimeoutEvent );
                    }
                    else
                    {
                        NotifyObserversFromThread( CPing::PingFailedEvent );
                    }
                }
            }
            else 
            {
                NotifyObserversFromThread( CPing::PingFailedEvent );
            }
            
            ++m_pingsCompleted;        
            return false;
        }
        else
        {
            // we are done, end the task
            return true;
        }
    }
    
    /*---------------------------------------------------------------------*/

    virtual void OnTaskEnd( void* taskdata )
    {TRACE;

        IcmpCloseHandle( m_icmpHandle );
        
        free( m_replyBuffer );
        m_replyBuffer = NULL;
        free( m_sendBuffer );
        m_sendBuffer = NULL;
        
        NotifyObserversFromThread( CPing::PingStoppedEvent );
    }
    
    /*---------------------------------------------------------------------*/
            
    private:
    friend class CPing;
    
    /*---------------------------------------------------------------------*/
    
    CMSWinPingTask( void )
        : CActiveObject()           ,
          CTSGNotifier()            ,
          m_replyBuffer( NULL )     ,
          m_sendBuffer( NULL )      ,
          m_icmpHandle( NULL )      ,
          m_remoteHost()            ,
          m_maxPings( 0 )           ,
          m_bytesToSend( 32 )       ,
          m_replyBufferSize( 0 )    ,
          m_timeout( 1000 )         ,
          m_pingsCompleted( 0 )
    {TRACE;
    }
    
    /*---------------------------------------------------------------------*/
    
    virtual ~CMSWinPingTask()
    {TRACE;
    }

    /*---------------------------------------------------------------------*/
        
    bool SetTaskData( const CORE::CString& remoteHost     ,
                      const UInt32 maxPings = 0           ,
                      const UInt32 bytesToSend = 32       ,
                      const UInt32 timeout = 1000         )
    {TRACE;
    
        if ( !IsActive() )
        {
            m_remoteHost = remoteHost;
            m_maxPings = maxPings;
            m_timeout = timeout;
            m_bytesToSend = bytesToSend;
            m_replyBufferSize = sizeof( ICMP_ECHO_REPLY ) + max( m_bytesToSend, 8 );
            free( m_replyBuffer );
            m_replyBuffer = NULL;
            free( m_sendBuffer );
            m_sendBuffer = NULL;
            return true;
        }
        return false;
    }
    
    /*---------------------------------------------------------------------*/
    
    private:
    
    CORE::CString m_remoteHost;
    UInt32 m_maxPings;
    UInt32 m_pingsCompleted;
    UInt32 m_bytesToSend;
    UInt32 m_replyBufferSize;
    UInt32 m_timeout;
    LPVOID m_replyBuffer;
    LPVOID m_sendBuffer;
    HANDLE m_icmpHandle;
    CSocket::TIPAddress m_ip;
};

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

bool
LinkICMPForXPAndHigher( void )
{
    globalMutex.Lock();
    if ( iphlpapiDllHandle == NULL )
    {
        iphlpapiDllHandle = CORE::LoadModuleDynamicly( "iphlpapi.dll" );
        if ( iphlpapiDllHandle != NULL )
        {
            IcmpCreateFile = (TIcmpCreateFilePtr) CORE::GetFunctionAddress( iphlpapiDllHandle ,
                                                                            "IcmpCreateFile"  ,
                                                                            0                 );
            IcmpCloseHandle = (TIcmpCloseHandlePtr) CORE::GetFunctionAddress( iphlpapiDllHandle ,
                                                                              "IcmpCloseHandle" ,
                                                                              sizeof( HANDLE )  );
            IcmpSendEcho = (TIcmpSendEchoPtr) CORE::GetFunctionAddress( iphlpapiDllHandle ,
                                                                        "IcmpSendEcho"    ,
                                                                        sizeof( HANDLE ) + sizeof( IPAddr ) + 2*sizeof( LPVOID ) + sizeof( WORD ) + sizeof( PIP_OPTION_INFORMATION ) + 2*sizeof( DWORD ) );
                                                     
            bool foundFunctions = ( IcmpCreateFile != NULL ) && ( IcmpCloseHandle != NULL ) && ( IcmpSendEcho != NULL );
            if ( !foundFunctions )
            {
                CORE::UnloadModuleDynamicly( iphlpapiDllHandle );
                iphlpapiDllHandle = NULL;
                iphlpapiDllRefCount = 0;
                
                globalMutex.Unlock();
                return false;            
            }
        
            // Successfully linked the module and functions
        }
        else
        {
            iphlpapiDllHandle = NULL;
            iphlpapiDllRefCount = 0;
            
            globalMutex.Unlock();
            return false;            
        }
    }
    
    ++iphlpapiDllRefCount;
    
    globalMutex.Unlock();
    return true;
}

/*-------------------------------------------------------------------------*/

void
UnlinkICMPForXPAndHigher( void )
{
    globalMutex.Lock();    
    if ( iphlpapiDllHandle != NULL )
    {
        if ( iphlpapiDllRefCount == 1 )
        {
            CORE::UnloadModuleDynamicly( iphlpapiDllHandle );
            iphlpapiDllHandle = NULL;            
            --iphlpapiDllRefCount;
        }
    }
    globalMutex.Unlock();
}

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------*/

void
CPing::RegisterEvents( void )
{TRACE;
    
    PingStartedEvent.Initialize();
    PingReponseEvent.Initialize();
    PingTimeoutEvent.Initialize();
    PingFailedEvent.Initialize();
    PingStoppedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPing::CPing( void )
    : CObservingNotifier()     ,
      m_isActive( false )      ,
      m_remoteHost()           ,
      m_maxPings( 0 )          ,
      m_bytesToSend( 32 )      ,
      m_timeout( 1000 )        ,
      m_minimalPingDelta( 500 )
{TRACE;

    RegisterEvents();    
    
    #ifdef GUCEF_MSWIN_BUILD
    
    LinkICMPForXPAndHigher();
    
    AddEventForwarding( PingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PingReponseEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PingTimeoutEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PingFailedEvent, EVENTORIGINFILTER_TRANSFER );
    AddEventForwarding( PingStoppedEvent, EVENTORIGINFILTER_TRANSFER );
    #endif /* GUCEF_MSWIN_BUILD ? */
}

/*-------------------------------------------------------------------------*/
    
CPing::~CPing()
{TRACE;

    Stop();
    
    #ifdef GUCEF_MSWIN_BUILD
    UnlinkICMPForXPAndHigher();
    #endif /* GUCEF_MSWIN_BUILD ? */
}

/*-------------------------------------------------------------------------*/

bool
CPing::Start( const CORE::CString& remoteHost           ,
              const UInt32 maxPings /* = 0 */           ,
              const UInt32 bytesToSend /* = 32 */       ,
              const UInt32 timeout /* = 1000 */         ,
              const UInt32 minimalPingDelta /* = 500 */ )
{TRACE;

    if ( !m_isActive && ( remoteHost.Length() > 0 ) )
    {
        m_remoteHost = remoteHost;
        m_maxPings = maxPings;
        m_timeout = timeout;
        m_bytesToSend = bytesToSend;
        m_minimalPingDelta = minimalPingDelta;
        
        #ifdef GUCEF_MSWIN_BUILD
        
        CMSWinPingTask* pingTask = new CMSWinPingTask();
        if ( pingTask != NULL )
        {
            if ( iphlpapiDllHandle != NULL )
            {
                m_isActive = true;
                
                // configure our task and subscribe to it
                m_osData = pingTask;
                pingTask->SetTaskData( remoteHost       ,
                                       maxPings         ,
                                       bytesToSend      ,
                                       timeout          );                                   
                SubscribeTo( pingTask );
                
                // here we go...
                if ( minimalPingDelta > 0 )
                {
                    pingTask->Activate( NULL                 ,
                                        minimalPingDelta / 2 ,
                                        minimalPingDelta     );
                }
                else
                {
                    pingTask->Activate();
                }
                return true;
            }
            
            // This windows version does not support this functionality
            delete pingTask;
            return false;
        }
        
        #else /* GUCEF_MSWIN_BUILD ? */
        
        // No implementation is available for the target platform
        return false;
        #endif
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPing::Stop( void )
{TRACE;
    
    if ( m_isActive )
    {
        #ifdef GUCEF_MSWIN_BUILD        
        static_cast< CMSWinPingTask* >( m_osData )->Deactivate( true );
        delete  static_cast< CMSWinPingTask* >( m_osData );       
        #endif /* GUCEF_MSWIN_BUILD ? */
        
        m_osData = NULL;
    }
}

/*-------------------------------------------------------------------------*/
    
bool
CPing::IsActive( void ) const
{TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CPing::GetRemoteHost( void ) const
{TRACE;

    return m_remoteHost;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CPing::GetMaxPings( void ) const
{TRACE;

    return m_maxPings;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/