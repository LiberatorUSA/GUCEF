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

#ifndef GUCEF_CORE_CEVENTPUMPCLIENT_H
#define GUCEF_CORE_CEVENTPUMPCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CEventPump;

class EXPORT_CPP CEventPumpClient
{
        public:

        CEventPumpClient( void );

        CEventPumpClient( const CEventPumpClient& other );

        CEventPumpClient& operator=( const CEventPumpClient& other );

        virtual ~CEventPumpClient();

        bool SendEvent( CEvent& event );
        
        /**
         *      Creates an event object initialized
         *      with the ID given and the dispatches the event.
         *      Saves you some effort if the event you are sending 
         *      has no data payload.
         *
         *      @param eventid ID of the event you wish to send
         */
        bool SendEvent( UInt32 eventid );
        
        bool SendEventAndLockMailbox( CEvent& event );
        
        bool SendEventAndLockMailbox( const UInt32 eventid );

        UInt32 GetPumpClientID( void ) const;

        protected:
        friend class CEventPump;
        
        virtual void OnProcessEvent( const CEvent& event );   
        
        /**
         *      Called each event pump update cycle after the events for 
         *      the current cycle have been pumped to all clients.
         *
         *      @param tickcount the tick count when the Update process commenced.
         *      @param deltaticks ticks since the last Update process commenced.
         */        
        virtual void OnUpdate( const UInt32 tickcount  ,
                               const UInt32 deltaticks );

        private:
        friend class CEventPump;

        void SetPumpClientID( UInt32 clientid );

        private:
        UInt32 _clientid;
        CEventPump* _pump;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CEVENTPUMPCLIENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
