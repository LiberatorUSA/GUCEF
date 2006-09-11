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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#ifndef GUCEF_CORE_CEVENTSTACK_H
#include "CEventStack.h"
#define GUCEF_CORE_CEVENTSTACK_H
#endif /* GUCEF_CORE_CEVENTSTACK_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SEventData
{                                                        
        UInt32 eventtype;
        UInt32 pumpclientid;
        UInt32 eventdataid;
};

typedef struct SEventData TEventData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventStack::CEventStack( void )

        : _estack( 50                  ,
                   8 + sizeof( void* ) )
{
        TRACE;
}

/*-------------------------------------------------------------------------*/

CEventStack::~CEventStack()
{
        TRACE;
}

/*-------------------------------------------------------------------------*/

bool
CEventStack::GetTop( CEvent& event )
{
        TRACE;       
        _mutex.Lock();
        TEventData ed;
        if ( _estack.GetTop( &ed ) )
        {
                event._eventtype = ed.eventtype;
                event._pumpclientid = ed.pumpclientid;
                event._eventdataid = ed.eventdataid;
                _mutex.Unlock();
                return true;
        }
        _mutex.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/

void
CEventStack::PushEvent( const CEvent& event )
{
        TRACE;
        _mutex.Lock();
        TEventData ed;
        ed.eventtype = event._eventtype;
        ed.pumpclientid = event._pumpclientid;
        ed.eventdataid = event._eventdataid;
        _estack.Push( &ed );
        _mutex.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
