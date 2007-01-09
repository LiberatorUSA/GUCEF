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

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#define GUCEF_CORE_CNOTIFIEROBSERVINGCOMPONENT_CPP
#include "CNotifierObservingComponent.h"

#include "CObservingNotifier.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void 
CNotifierObservingComponent::OnNotify( CNotifier* notifier                 ,
                                       const CEvent& eventid               ,
                                       CICloneable* eventdata /* = NULL */ )
{TRACE;

    m_owner->OnNotify( notifier  ,
                       eventid   ,
                       eventdata );
}                                         

/*-------------------------------------------------------------------------*/

void 
CNotifierObservingComponent::LockData( void )
{TRACE;

    m_owner->LockData();
}

/*-------------------------------------------------------------------------*/
    
void 
CNotifierObservingComponent::UnlockData( void )
{TRACE;

    m_owner->UnlockData();
}

/*-------------------------------------------------------------------------*/

CNotifierObservingComponent::CNotifierObservingComponent( void )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent::CNotifierObservingComponent( const CNotifierObservingComponent& src )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent::~CNotifierObservingComponent()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent& 
CNotifierObservingComponent::operator=( const CNotifierObservingComponent& src )
{TRACE;

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CNotifierObservingComponent::SetOwner( CObservingNotifier* owner )
{TRACE;

    m_owner = owner;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
