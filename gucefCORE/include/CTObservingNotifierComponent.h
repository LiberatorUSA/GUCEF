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

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIERCOMPONENT_H
#define GUCEF_CORE_CTOBSERVINGNOTIFIERCOMPONENT_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CObservingNotifier.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIERCOMPONENT_CPP
    #pragma warning( push )
#endif

#pragma warning( disable: 4146 ) // unary minus operator applied to unsigned type, result still unsigned
#pragma warning( disable: 4251 ) // 'classname' needs to have dll-interface to be used by clients of class 'classname'
#pragma warning( disable: 4284 ) // return type for operator -> is 'const *' (ie; not a UDT or reference to a UDT).
#pragma warning( disable: 4786 ) // identifier was truncated to 'number' characters

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

/**
 *  Template for encapsulating a CObservingNotifier instance in another class
 *  It will proxy the manditory overloads to the component owner, allowing instances
 *  to be created without having to implement the pure virtuals.
 *
 *  Note that as a user you will most likely only want to use the
 *  CTObservingNotifierExpansion template. Check it out if all you want to do
 *  is add notification to your class.
 *
 *  Note: When using this template you must implement OnNotify() in your class with the
 *  same semantics as it is implemented here and provide the template instantiation
 *  with access to your OnNotify() (if so desired with a friend relation).
 *  You must also place 'varName.SetOwner( this )' in your class constructor where
 *  'varName' is the name the instance of this class template.
 */
template < class TOwnerClass >
class CTObservingNotifierComponent : public CObservingNotifier
{
    public:
    
    CTObservingNotifierComponent( void );

    CTObservingNotifierComponent( const CTObservingNotifierComponent& src );
    
    virtual ~CTObservingNotifierComponent();

    CTObservingNotifierComponent& operator=( const CTObservingNotifierComponent& src );
    
    /**
     *  Sets the callback owner class pointer.
     *  must be set in your class contructor using
     *      'SetOwner( this );'
     *
     *  @param ownerInstance pointer to the owner of the compound instance.
     */
    void SetOwner( TOwnerClass* ownerInstance );
    
    protected:
    
    /**
     *  Event callback member function.
     *  Implement this in your owner class to handle
     *  notification events. This template will proxy the 
     *  callback to the owner class.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const UInt32 eventid          ,
                           CICloneable* eventdata = NULL );
    
    private:
    
    TOwnerClass* m_componentOwner;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < class TOwnerClass >
CTObservingNotifierComponent< TOwnerClass >::CTObservingNotifierComponent( void )
    : CObservingNotifier()     ,
      m_componentOwner( NULL )
{

}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
CTObservingNotifierComponent< TOwnerClass >::CTObservingNotifierComponent( const CTObservingNotifierComponent& src )
    : CObservingNotifier( src ) ,
      m_componentOwner( NULL )
{
    
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
CTObservingNotifierComponent< TOwnerClass >::~CTObservingNotifierComponent( void )
{
    UnsubscribeAllFromObserver();
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
CTObservingNotifierComponent< TOwnerClass >&
CTObservingNotifierComponent< TOwnerClass >::operator=( const CTObservingNotifierComponent& src )
{
    CObservingNotifier::operator=( src );
    
    if ( &src != this )
    {
        /* don't do anything, leave the owner pointer as is */
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
void 
CTObservingNotifierComponent< TOwnerClass >::SetOwner( TOwnerClass* ownerInstance )
{
    m_componentOwner = ownerInstance;
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
void
CTObservingNotifierComponent< TOwnerClass >::OnNotify( CNotifier* notifier                 ,
                                                       const UInt32 eventid                ,
                                                       CICloneable* eventdata /* = NULL */ )
{
    /*
     *  If you crash here then you forgot to call SetOwner() in your
     *  class contructor.
     */
    assert( m_componentOwner );
    
    /*
     *  Simply proxy the call to the owner class.
     *  This is the purpose of this template 
     */
    m_componentOwner->OnNotify( notifier  ,
                                eventid   ,
                                eventdata );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIERCOMPONENT_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTOBSERVINGNOTIFIERCOMPONENT_H  ? */
