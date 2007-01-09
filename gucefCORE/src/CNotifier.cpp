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

#include <assert.h>

#ifndef GUCEF_CORE_CNOTIFIERIMPLEMENTOR_H
#include "CNotifierImplementor.h"
#define GUCEF_CORE_CNOTIFIERIMPLEMENTOR_H
#endif /* GUCEF_CORE_CNOTIFIERIMPLEMENTOR_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

#include "CNotifier.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CEvent CNotifier::SubscribeEvent = "GUCEF::CORE::CNotifier::SubscribeEvent";
const CEvent CNotifier::UnsubscribeEvent = "GUCEF::CORE::CNotifier::UnsubscribeEvent";
const CEvent CNotifier::ModifyEvent = "GUCEF::CORE::CNotifier::ModifyEvent";
const CEvent CNotifier::DestructionEvent = "GUCEF::CORE::CNotifier::DestructionEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CNotifier::CNotifier( void )
    : CITypeNamed() ,
      m_imp( NULL )
{TRACE;

    RegisterEvents();
    
    m_imp = CNotifierImplementor::Create( this );
    assert( m_imp != NULL );
}

/*-------------------------------------------------------------------------*/

CNotifier::CNotifier( const CNotifier& src )
    : CITypeNamed() ,
      m_imp( NULL )
{TRACE;

    RegisterEvents();
   
    m_imp = CNotifierImplementor::Create( this, src );
    assert( m_imp != NULL );
}

/*-------------------------------------------------------------------------*/

CNotifier&
CNotifier::operator=( const CNotifier& src )
{TRACE;

    if ( this != &src )
    {
        (*m_imp) = (*src.m_imp);
    }
    return *this; 
}    

/*-------------------------------------------------------------------------*/

CNotifier::~CNotifier()
{TRACE;

      m_imp->OnDeathOfOwnerNotifier();
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::RegisterEvents( void )
{TRACE;

    ModifyEvent.Initialize();
    DestructionEvent.Initialize();
    SubscribeEvent.Initialize();
    UnsubscribeEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::Subscribe( CObserver* observer )
{TRACE;

    m_imp->Subscribe( observer );  
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::Subscribe( CObserver* observer   ,
                      const CEvent& eventid )
{TRACE;

    m_imp->Subscribe( observer ,
                      eventid  );
}

/*-------------------------------------------------------------------------*/

void
CNotifier::Unsubscribe( CObserver* observer )
{TRACE;

    m_imp->Unsubscribe( observer );                                  
}

/*-------------------------------------------------------------------------*/                
 
void 
CNotifier::Unsubscribe( CObserver* observer   ,
                        const CEvent& eventid )
{TRACE;

    m_imp->Unsubscribe( observer ,
                        eventid  );
}

/*-------------------------------------------------------------------------*/                        

bool 
CNotifier::NotifyObservers( void )
{TRACE;

    return m_imp->NotifyObservers();
}

/*-------------------------------------------------------------------------*/
    
bool 
CNotifier::NotifyObservers( const CEvent& eventid  ,
                            CICloneable* eventData )
{TRACE;

    return m_imp->NotifyObservers( eventid   ,
                                   eventData );   
}

/*-------------------------------------------------------------------------*/

bool
CNotifier::NotifyObservers( CNotifier& sender                   ,
                            const CEvent& eventid               ,
                            CICloneable* eventData /* = NULL */ )
{TRACE;

    return m_imp->NotifyObservers( sender    ,
                                   eventid   ,
                                   eventData );
}                            

/*-------------------------------------------------------------------------*/

void 
CNotifier::OnObserverDestroy( CObserver* observer )
{TRACE;

    m_imp->OnObserverDestroy( observer );                                 
}

/*-------------------------------------------------------------------------*/

CString 
CNotifier::GetType( void ) const
{TRACE;

    return "GUCEF::CORE::CNotifier";
}

/*-------------------------------------------------------------------------*/

void 
CNotifier::LockData( void ) const
{TRACE;

    /* can be implemented in a descending class to add thread-safety */
}

/*-------------------------------------------------------------------------*/
    
void 
CNotifier::UnlockData( void ) const
{TRACE;

    /* can be implemented in a descending class to add thread-safety */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
