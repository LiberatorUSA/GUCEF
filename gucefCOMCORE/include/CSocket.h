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

#ifndef CSOCKET_H
#define CSOCKET_H

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

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* macros and build config for the COMCORE library */
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

/*
 *	Forward declarations of classes used
 */
class CCom;                /* network manager, allows the sockets to do there job */
class CActiveComPump;

/*-------------------------------------------------------------------------*/

/**
 *  Base class for all our socket classes.
 */
class EXPORT_CPP CSocket : public CORE::CNotifier
{
    public:
    
    struct SIPAddress
    {
            UInt16 port;            /**< port in network byte order */
            UInt32 netaddr;         /**< address in network byte order */
    };
    typedef struct SIPAddress TIPAddress;        
    
    typedef enum ESocketError
    {
            SOCKERR_NO_ERROR = 0      ,
            SOCKERR_INTERNAL_ERROR    ,
            SOCKERR_INVALID_SOCKET    ,
            SOCKERR_OUT_OF_MEMORY     ,
            SOCKERR_UDP_SEND_FAILED   ,
            SOCKERR_INVALID_ADDRESS   ,
            SOCKERR_CANT_FIND_ADDRESS ,
            SOCKERR_CANT_SEND_DATA    ,
            SOCKERR_CANT_OPEN_SOCKET  
            
    } TSocketError;              
    
    UInt32 GetSocketID( void ) const;
    

    /**
     *      Attempts to resolve the given destination address and port
     *      information and stores the result in the dest struct.
     *      You should store the result for furture packet sends so
     *      that the dns ect. no longer has to be resolved with each
     *      packet send.
     *
     *      @param destaddrstr the destination address in string form that you wish to resolve
     *      @param destport the port on the remote host you wish to address
     *      @param resolvedDest structure that will hold the resolved version of the given data
     *      @return returns wheter the given data could be resolved.      
     */
    static bool ConvertToIPAddress( const CORE::CString& destaddrstr ,
                                    const UInt16 destport            ,  
                                    TIPAddress& resolvedDest         );
                                    
    /**
     *      Attempts to resolve the source information into a more human-friendly
     *      version of the information. 
     *
     *      @param src the information about the data source in network format
     *      @param srcaddrstr output variable for the source IP/DNS
     *      @param srcport output variable for the source port                     
     */
    static bool ConvertFromIPAddress( const TIPAddress& src     ,
                                      CORE::CString& srcaddrstr ,
                                      UInt16& srcport           );
        
    virtual ~CSocket();
    
    protected:
    
    CSocket();                
    
    protected:
    friend class CCom;               
    friend class CActiveComPump;
    
    /** 
     *      Decending classes should implement this updater to 
     *      poll the socket ect. as needed and update stats.
     *
     *      @param tickcount the tick count when the Update process commenced.
     *      @param deltaticks ticks since the last Update process commenced.          
     */
    virtual void Update( UInt32 tickcount  ,
                         UInt32 deltaticks ) = 0;
                                                              
    private:
    friend class CCom;
    
    void SetSocketID( UInt32 sid );                                                                                                                                                                        
                         
    private:
    CSocket( const CSocket& src );             /* making a copy of a socket doesnt make sense */
    CSocket& operator=( const CSocket& src );  /* making a copy of a socket doesnt make sense */
    
    UInt32 _sid;          
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CSOCKET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-03-2005 :
       - Stripped everything related to the old system that used SDL_net
- 11-02-2004 :
       - Added Stop()
- 5-1-2004 :
       - Added a get function for the socket's threading method.
- 3-11-2003 :
       - Added friend relationship for the com pointer so that the com manager
         can set the pointer to NULL when it's destroyed to that the
         deconstruction order socket or manager makes no difference.
- 30-10-2003 :
        - Added type var to store decended socket type.
        - Added parameters to the contructor which should be set by the
          decending socket's initializer list. This new method of construction
          allows a user to construct the socket object whereever the user
          wishes instead of getting an instance from the socket manager.
          The socket still needs to be know to the manager offcourse in order
          to be able to function, so the socket will register itself with the
          com manager given. 
- 22-10-2003 :
        - Fixed a memory leak in the TM_CONSUMER_THREAD event pushing code.
          It was caused by a redundant allocation resulting from the added
          Push_Event() member function in CSocketConsumerTask.
- 23-8-2003 :
        - Removed friend with itself relationship
- 11-8-2003:
        - Added Stop_And_Wait() which besides waiting for all threads to end
          before returning also sets the stop flag. This means that when the
          threading method is TM_CONSUMER_THREAD the consumer task will end.
          If you just use Wait_Untill_All_Done() then the callig thread will
          wait untill the threads end on there own which would be never if the
          threading method is TM_CONSUMER_THREAD. Which in turn would cause the
          calling thread to wait forever.
- 4-8-2003 :
        - Added this section.
        - Moved com thread management here because this allows me to set
          the threading method used per socket instead of globally.

---------------------------------------------------------------------------*/
