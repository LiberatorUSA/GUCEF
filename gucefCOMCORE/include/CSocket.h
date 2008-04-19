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

#ifndef GUCEF_COMCORE_CSOCKET_H
#define GUCEF_COMCORE_CSOCKET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* macros and build config for the COMCORE library */
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

/*
 *	Forward declarations of classes used
 */
class CCom;                /* network manager, allows the sockets to do there job */
class CActiveComPump;

/*-------------------------------------------------------------------------*/

/**
 *  Base class for all our socket classes.
 */
class GUCEF_COMCORE_EXPORT_CPP CSocket : public CORE::CObservingNotifier
{
    public:              
    
    UInt32 GetSocketID( void ) const;
    

    /**
     *      Attempts to resolve the given destination address and port
     *      information and stores the result in the dest struct.
     *      You should store the result for future packet sends so
     *      that the dns ect. no longer has to be resolved with each
     *      packet send.
     *
     *      @param destaddrstr the destination address in string form that you wish to resolve
     *      @param destport the port on the remote host you wish to address
     *      @param resolvedDest structure that will hold the resolved version of the given data
     *      @return returns whether the given data could be resolved.      
     */
    static bool ConvertToIPAddress( const CORE::CString& destaddrstr ,
                                    const UInt16 destport            ,  
                                    CIPAddress& resolvedDest         );
                                    
    /**
     *      Attempts to resolve the source information into a more human-friendly
     *      version of the information. 
     *
     *      @param src the information about the data source in network format
     *      @param srcaddrstr output variable for the source IP/DNS
     *      @param srcport output variable for the source port                     
     */
    static bool ConvertFromIPAddress( const CIPAddress& src     ,
                                      CORE::CString& srcaddrstr ,
                                      UInt16& srcport           );
        
    virtual ~CSocket();
    
    protected:
    
    CSocket();                
    
    protected:
    friend class CCom;               
    friend class CActiveComPump;
    
    virtual void Update( void ) {};
                                                              
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

#endif /* GUCEF_COMCORE_CSOCKET_H ? */

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
