/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_CEVENTPUMP_H
#define GUCEF_CORE_CEVENTPUMP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEVENTSTACK_H
#include "CEventStack.h"
#define GUCEF_CORE_CEVENTSTACK_H
#endif /* GUCEF_CORE_CEVENTSTACK_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

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

class CEventPumpClient;
class CEvent;

class EXPORT_CPP CEventPump
{
        public:

        static CEventPump* Instance( void );

        ~CEventPump();
        
        void Update( void );

        bool SendEvent( const CEvent& event );

        UInt32 RegisterClient( CEventPumpClient* newclient );

        void UnregisterClient( UInt32 clientid );

        UInt32 GetPumpClientCount( void );
        
        void SetIsEnabled( const bool enabled );
        
        bool GetIsEnabled( void ) const;
        
        bool SendEventAndLockMailbox( const CEvent& event );
        
        void SetMailboxIsLocked( const bool locked );
        
        bool GetMailboxIsLocked( void ) const;
        
        void SetPerformClientUpdates( const bool updateClients );
        
        bool GetPerformClientUpdates( void ) const;
        
        UInt32 GetLastUpdateTickCount( void ) const;

        private:

        CEventPump( void );
        CEventPump( const CEventPump& src );
        CEventPump& operator=( CEventPump& src );

        CEventStack _eventstack;
        CDynamicArray _clients;
        UInt32 _lastupdate; 
        bool m_enabled;
        bool m_mailLocked;
        bool m_updateClients;
        
        static MT::CMutex _mutex;
        static CEventPump* _instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CEVENTPUMP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/


