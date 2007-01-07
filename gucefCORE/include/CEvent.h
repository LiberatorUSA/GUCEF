/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#ifndef GUCEF_CORE_CEVENT_H
#define GUCEF_CORE_CEVENT_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

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

class CNotificationIDRegistry;

/*-------------------------------------------------------------------------*/

/**
 *  Event object
 *  Used to identify an event within the GUCEF event system.
 *  
 *  For performance reasons numeric ID's are used for event comparison 
 *  operations. Note that a call must be made to Initialize() before the
 *  event object is used. This initialization procedure is required to give you
 *  the performance required plus the ability to define event objects as global
 *  variables. 
 */
class GUCEFCORE_EXPORT_CPP CEvent
{
    public:
    
    CEvent( void );
    
    CEvent( const CString& eventName );

    CEvent( const CEvent& src );

    ~CEvent();

    CEvent& operator=( const CEvent& src );
    
    CEvent& operator=( const CString& eventName );

    bool operator==( const CEvent& other ) const;
    
    bool operator!=( const CEvent& other ) const;
    
    /**
     *  This operator is only implemented for sorting/list
     *  containers and has no real meaning since the actual
     *  value of an event ID has no meaning other then a
     *  means of identification.
     */
    bool operator<( const CEvent& other ) const;

    UInt32 GetID( void ) const;
    
    CString GetName( void ) const;

    /**
     *  Initializes the event object
     *  This should always be called before the event object is ever used.
     *  It registers the event if needed and initializes the numeric event ID.
     */
    void Initialize( void ) const;
    
    bool IsInitialized( void ) const;
    
    private:
    friend class CNotificationIDRegistry;
    
    CEvent( const UInt32 eventID     ,
            const CString& eventName );
    
    private:
    
    UInt32 m_eventID;
    CString m_eventName;    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CEVENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-10-2006 :
        - Initial implementation
          
---------------------------------------------------------------------------*/