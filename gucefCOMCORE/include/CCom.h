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

#ifndef GUCEF_COMCORE_CCOM_H
#define GUCEF_COMCORE_CCOM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#include "CGUCEFAppSubSystem.h"
#define GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#endif /* GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"            /* <- deprecated */
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* build switches and macros */
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

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

class CActiveComPump;
class CGUCEFCOMCOREModule;

/*-------------------------------------------------------------------------*/

/**
 *      Singleton communication manager for sockets. 
 *      Provides a global control point for all sockets.
 */
class GUCEF_COMCORE_EXPORT_CPP CCom : public CORE::CGUCEFAppSubSystem
{
    public:
    
    struct SSocketStats 
    {
            UInt32 bytes_sent;       /** the total number of bytes sent by all sockets */
            UInt32 bytes_recieved;   /** the total number of bytes received by all sockets */
            UInt32 bps_sent;         /** current sent bytes per second ratio of this update cycle */
            UInt32 bps_recieved;     /** current received bytes per second ratio of this update cycle */
    };
    typedef struct SSocketStats TSocketStats;
   
    /**
     *      return an instance pointer of the singleton CCom manager.
     *      This creates the manager object if it doesnt exist yet
     *      as standard for a singleton. 
     *
     *      @return the global CCom manager object        
     */
    static CCom* Instance( void );
    
    void SetPumpThreading( bool thread );
    
    bool GetPumpThreading( void ) const;

    /**
     *	returns the total number of sockets that are currently registered
     *	at this manager.
     *
     *      @return the current number of registered sockets
     */
    UInt32 GetSocketCount( void ) const;
    
    /**
     *      
     */
    void SetUseGlobalStats( bool keep_gstats );
    
    /**
     *      
     */        
    bool GetUseGlobalStats( void ) const;
                 
    /**
     *      
     */                     
    void ResetGlobalStats( void );
    
    /**
     *      
     */        
    const TSocketStats& GetGlobalStats( void ) const;
    
    
    /**
     *  Attempts to set the active state of a proxy server for the given protocol
     *  If no proxy server is defined for the given protocol the call will fail
     *
     *  @param protocol the protocol for which to set the active state
     *  @param active the new value for the active state for the given protocol proxy
     *  @return false is returned upon failure
     */
    bool SetSystemWideProxyServer( const CORE::CString& protocol ,
                                   const bool active             );

    /**
     *  Attempts to set the active state of a proxy server for the given protocol
     *  If no proxy server is defined for the given protocol the call will fail
     *
     *  @param protocol the protocol for which to set the active state
     *  @param remoteHost IP or hostname of the proxy server
     *  @param remotePort port of the proxy server where the proxy service is hosted
     *  @param active the active state for the given protocol proxy
     *  @return false is returned upon failure
     */
    bool SetSystemWideProxyServer( const CORE::CString& protocol   ,
                                   const CORE::CString& remoteHost ,
                                   const UInt16 remotePort         ,
                                   const bool active               );

    bool GetSystemWideProxyServer( const CORE::CString& protocol ,
                                   CORE::CString& remoteHost     ,
                                   UInt16& remotePort            ,
                                   bool& active                  ) const;
    
   /**
     *  Attempts to retrieve the active state of a proxy server for the given protocol
     *  If no proxy server is defined for the given protocol the call will return false
     *
     *  @return the active state for the given protocol proxy, false if no proxy is defined
     */
    bool IsSystemWideProxyServerActive( const CORE::CString& protocol ) const;
    
    protected:
    
    /**
     *      Calls Update() for all registered socket objects.
     *
     *      @param tickcount the tick count when the Update process commenced.
     *      @param deltaticks ticks since the last Update process commenced.         
     */
    virtual void OnUpdate( const UInt64 tickcount               ,
                           const Float64 updateDeltaInMilliSecs );
                         
    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined user data
     */
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );
                                                    
    private:
    friend class CGUCEFCOMCOREModule;
    
    static void Deinstance( void );                              
            
    private:
    friend class CSocket;      

    /**
     *      Registers a socket at the manager to receive update events.
     *      Should be used when creating a socket object by the socket object.
     *
     *      @param socket the socket object that you wish to register      
     */
    void RegisterSocketObject( CSocket* socket );
                     
    /**
     *      Unregisters a socket at the manager to receive update events
     *      Should be used when destroying a socket object by the socket object.         
     *
     *      @param socket the socket object that you wish to unregister      
     */                                  
    void UnregisterSocketObject( const CSocket* socket );

    private:
    friend class CActiveComPump;
          
    CORE::CDynamicArray _sockets;       /** our socket object heap */
    static MT::CMutex _mutex;           /** global CCom mutex */ 
            
    private:
    
    struct SProxyServer
    {
        CORE::CString host;
        UInt16 port;
        bool active;
    };
    typedef struct SProxyServer TProxyServer;
    typedef std::map< CORE::CString, TProxyServer > TProxyList;
             
    static CCom* _instance;             /** global CCom instance pointer */
    CCom( void );                       /** does nothing atm */
    CCom( const CCom& src );            /** dummy implementation */
    ~CCom();                            /** does nothing atm */
    CCom& operator=( const CCom& src ); /** dummy implementation */
    TSocketStats _stats;                /** global socket traffic stats */
    bool _keep_gstats;                  /** whether or not to keep track of global stats */
    UInt32 _scount;                     /** current number of registered sockets */
    bool _pumpthread;                   /** use a thread to update the sockets ? */
    CActiveComPump* _threadedpump;      /** threading based update pump */
    CORE::CTimer m_timerPump;           /** timer based update pump */
            
    TProxyList m_proxyList;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CCOM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added a timer to allow socket updates if an application driver
          is used. not the most efficient solution but it will do for now.
        - Dinand: Added system wide proxy settings, com systems can retrieve 
          settings here if so desired. The mswin O/S proxy read is implemented
          for HTTP using the registry.
- 30-12-2004 :
        - Dinand: Rewrote this class to use my own event system and back-end 
          networking code. As such this class no longer depends on SDL_net
          or NET2.
- 12-06-2004 :
        - Dinand: Fixed a bug in the socket unregister code.
        - Dinand: Implemented the NET2_Delay() member function.
- 11-01-2004 :
        - Dinand: Fixed a bug in maintaining an accurate active socket count.
- 03-11-2003 :
        - Dinand: Added Unlink_Sockets() so that sockets that are destroyed after the
          manager is destroyed don't cause any problems.
- 30-10-2003 :
        - Dinand: Added Register_Socket_Object() and Unregister_Socket_Object().
          This allows sockets to be created by the user instead of by this com
          manager.
        - Dinand: Fixed 2 bugs in Close_And_Wait() which could potentially cause an
          access violation.
        - Dinand: Removed all public member functions that allowed the user to change
          socket heap sizes and access sockets though the manager, this way
          access rights to sockets can be determined by the user.
- 01-09-2003 :
        - Dinand: Added USE_NETWORK_COM switch to remove all networking code.  
- 05-08-2003 :
        - Dinand: Added const keyword here and there.
        - Dinand: Added a new Wait_Untill_All_Done()
- 04-08-2003 :
        - Dinand: Moved thread management from this class to the CSocket class.
          this allowed me to set the threading method used per socket instead of
          globally.
- 01-07-2003 :
        - Dinand: Removed Get_ and Set_ prefixes from member function identifiers.
- 09-05-2003 :
        - Dinand: Added Threading_Method() which allows a user to specify the threading
          method used. I did this because which method is best depends on what
          you are using the CCom class for. You can set this for TCP and UDP
          seperat. if you have a high rate of separate transmissions that do
          not require a lot of processing you might be better of with a consumer
          thread which is default for UDP. If the number of transmissions is not
          very frequent and/or requires a lot of processing then you might be
          better of with a separate thread for each event which is default for
          TCP.
- 02-05-2003 :
        - Dinand: Modified thread handling, now uses the CThreadManager class instead
          of a CCom internal thread spawner. this makes threading more uniform.
          Another advantage is that i can now make certain attributes private
          that where public before because they had to be accessible from a C
          function.
        - Dinand: The addition of the tread manager means you MUST set Max_Tasks() before
          using the com manager.
- 22-04-2003 :
        - Dinand: Made this com manager thread safe, maybe someone has the bright idea
          of calling any of the heap resize events from another thread in which
          case an access violation may occur. This won't happen now because the
          normal caller (the application task) also passes trough mutexes now.
- 21-04-2003 :
        - Dinand: Because i made the socket classes a bit more independent of this
          manager CCom now depends on an index value that's set for the server
          or client sockets, this is public. if it is changed ccom cant do it's
          job properly.
- 18-04-2003 :
        - Dinand: Added build defines to remove either TCP or UDP if desired, this is
          done to reduce code bloat.
- 05-04-2003 :
        - Dinand: No longer a descendant of CNode 
- 21-02-2003 :
        - Dinand: Modified NET2 further to give me the ability to get rid of delay
          caused by linking the NET2 socket to my socket handling class.
          This is done with NET2_SetIndex() and NET2_GetIndex()
- 14-02-2003 :
        - Dinand: Socket event handlers now get called from a separate task. This means
          that the program execution continues while the task's socket event
          handler does it's thing. tasks are implemented as SDL_thread's
- 11-02-2003 :
        - Dinand: Added this info section.
        - Dinand: Designed and implemented this class.
        - Dinand: malloc and realloc and free are used for array memory management because
          we want to enlarge mem blocks sometimes without having to copy every
          entry manually. The socket objects themselves are allocated with new
          and delete.


-----------------------------------------------------------------------------*/
