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

#ifndef GUCEF_COMCORE_CCOM_H
#define GUCEF_COMCORE_CCOM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCORE_H
#include "gucefCORE.h"                /* gucefCORE library API */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFMT_H
#include "gucefMT.h"                  /* gucefMT library API */
#define GUCEFMT_H
#endif /* GUCEFMT_H ? */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* build switches and macros */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

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
                UInt32 bytes_recieved;   /** the total number of bytes recieved by all sockets */
                UInt32 bps_sent;         /** current sent bytes per second ratio of this update cycle */
                UInt32 bps_recieved;     /** current recieved bytes per second ratio of this update cycle */
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
         *	returns the total number of sockets that are currently registerd
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
        
        protected:
        
        /**
         *      Calls Update() for all registered socket objects.
         *
         *      @param tickcount the tick count when the Update process commenced.
         *      @param deltaticks ticks since the last Update process commenced.         
         */
        virtual void OnUpdate( const UInt32 tickcount  ,
                               const UInt32 deltaticks );
                             
                             
        private:
        friend class CGUCEFCOMCOREModule;
        
        static void Deinstance( void );
                                             

        private:         
        static CCom* _instance;             /** global CCom instance pointer */
        CCom( void );                       /** does nothing atm */
        CCom( const CCom& src );            /** dummy implementation */
        ~CCom();                            /** does nothing atm */
        CCom& operator=( const CCom& src ); /** dummy implementation */
        TSocketStats _stats;                /** global socket traffic stats */
        bool _keep_gstats;                  /** wheter or not to keep track of global stats */
        UInt32 _scount;                     /** current number of registered sockets */
        bool _pumpthread;                   /** use a thead to update the sockets ? */
        CActiveComPump* _threadedpump;
                
        private:
        friend class CActiveComPump;
              
        CORE::CDynamicArray _sockets;       /** our socket object heap */
        static MT::CMutex _mutex;           /** global CCom mutex */                                
                
        private:
        friend class CSocket;      

        /**
         *      Registers a socket at the manager to recieve update events.
         *      Should be used when creating a socket object by the socket object.
         *
         *      @param socket the socket object that you wish to register      
         */
        void RegisterSocketObject( CSocket* socket );
                         
        /**
         *      Unregisters a socket at the manager to recieve update events
         *      Should be used when destroying a socket object by the socket object.         
         *
         *      @param socket the socket object that you wish to unregister      
         */                                  
        void UnregisterSocketObject( const CSocket* socket );        
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

- 30-12-2004 :
        - Rewrote this class to use my own event system and back-end 
          networking code. As such this class no longer depends on SDL_net
          or NET2.
- 12-06-2004 :
        - Fixed a bug in the socket unregister code.
        - Implemented the NET2_Delay() member function.
- 11-01-2004 :
        - Fixed a bug in maintaining an accurate active socket count.
- 03-11-2003 :
        - Added Unlink_Sockets() so that sockets that are destroyed after the
          manager is destroyed dont cause any problems.
- 30-10-2003 :
        - Added Register_Socket_Object() and Unregister_Socket_Object().
          This allows sockets to be created by the user instead of by this com
          manager.
        - Fixed 2 bugs in Close_And_Wait() which could potentially cause an
          access violation.
        - Removed all public member functions that allowed the user to change
          socket heap sizes and access sockets though the manager, this way
          access rights to sockets can be determined by the user.
- 01-09-2003 :
        - Added USE_NETWORK_COM switch to remove all networking code.  
- 05-08-2003 :
        - Added const keyword here and there.
        - Added a new Wait_Untill_All_Done()
- 04-08-2003 :
        - Moved thread management from this class to the CSocket class.
          this allowed me to set the threading method used per socket instead of
          globally.
- 01-07-2003 :
        - Removed Get_ and Set_ prefixes from member function identifyers.
- 09-05-2003 :
        - Added Threading_Method() which allows a user to specify the threading
          method used. I did this because which method is best depends on what
          you are using the CCom class for. You can set this for TCP and UDP
          seperatly. if you have a high rate of seperate transmissions that do
          not require a lot of processing you might be better of with a consumer
          thread which is default for UDP. If the number of transmissions is not
          verry frequent and/or requires a lot of processing then you might be
          better of with a seperate thread for each event which is default for
          TCP.
- 02-05-2003 :
        - Modified thread handling, now uses the CThreadManager class instead
          of a CCom internal thread spawner. this makes threading more uniform.
          Another advantage is that i can now make certain atributes private
          that where public before because they had to be accessable from a C
          function.
        - The addition of the treadmanager means you MUST set Max_Tasks() before
          using the com manager.
- 22-04-2003 :
        - Made this com manager thread safe, maybe someone has the bright idea
          of calling any of the heap resize events from another thread in which
          case an access violation may occur. This won't happen now because the
          normal caller (the application task) also passes trough mutexes now.
- 21-04-2003 :
        - Because i made the socket classes a bit more independent of this
          manager CCom now depends on an index value that's set for the server
          or client sockets, this is public. if it is changed ccom cant do it's
          job properly.
- 18-04-2003 :
        - Added build defines to remove either TCP or UDP if desired, this is
          done to reduce code bloat.
- 05-04-2003 :
        - No longer a decendant of CNode 
- 21-02-2003 :
        - Modified NET2 further to give me the ability to get rid of delay
          caused by linking the NET2 socket to my socket handling class.
          This is done with NET2_SetIndex() and NET2_GetIndex()
- 14-02-2003 :
        - Socket event handlers now get called from a seperate task. This means
          that the program excecution continues while the task's socket event
          handler does it's thing. tasks are implemented as SDL_thread's
- 11-02-2003 :
        - Added this info section.
        - Designed and implemented this class.
        - malloc and realloc and free are used for array memory management because
          we want to enlarge mem blocks sometimes without having to copy every
          entry manually. The socket objetcs themselves are allocated with new
          and delete.


-----------------------------------------------------------------------------*/
