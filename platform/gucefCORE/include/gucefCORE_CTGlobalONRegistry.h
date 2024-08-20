/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CORE_CTGLOBALONREGISTRY_H
#define GUCEF_CORE_CTGLOBALONREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CTONREGISTRY_H
#include "CTONRegistry.h"
#define GUCEF_CORE_CTONREGISTRY_H
#endif /* GUCEF_CORE_CTONREGISTRY_H ? */

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
 *  Template that expands the normal CTONRegistry functionality with
 *  handlers for application wide events.
 * 
 *  Global registries used for dynamically registering abstracted functionality suffer from
 *  the problem that they need to be cleaned up when the application is shutting down in a timely manner.
 *  To avoid having to reimplement such handling for various usecases this template is provided.
 */
template < class T, class LockType >
class CTGlobalONRegistry : public CTONRegistry< T, LockType >
{
    public:

    CTGlobalONRegistry( void );

    CTGlobalONRegistry( const CTGlobalONRegistry& src );

    virtual ~CTGlobalONRegistry();

    CTGlobalONRegistry& operator=( const CTGlobalONRegistry& src );

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    typedef CTEventHandlerFunctor< CTGlobalONRegistry< T, LockType > >  TGlobalONRegistryEventCallback;

    void
    OnSomeShutdownEvent( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventid  ,
                         CORE::CICloneable* eventdata );

    void RegisterEventHandlers( void );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class T, class LockType >
CTGlobalONRegistry< T, LockType >::CTGlobalONRegistry( void )
    : CTONRegistry< T, LockType >()
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
CTGlobalONRegistry< T, LockType >::CTGlobalONRegistry( const CTGlobalONRegistry& src )
    : CTONRegistry< T, LockType >( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
CTGlobalONRegistry< T, LockType >::~CTGlobalONRegistry()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
CTGlobalONRegistry< T, LockType >&
CTGlobalONRegistry< T, LockType >::operator=( const CTGlobalONRegistry& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CTONRegistry< T, LockType >::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
const CString&
CTGlobalONRegistry< T, LockType >::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CTGlobalONRegistry< T, LockType >";
    return typeName;
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
void
CTGlobalONRegistry< T, LockType >::OnSomeShutdownEvent( CNotifier* notifier    ,
                                                        const CEvent& eventid  ,
                                                        CICloneable* eventdata )
{GUCEF_TRACE;

    CTONRegistry< T, LockType >::UnregisterAll();    
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
void
CTGlobalONRegistry< T, LockType >::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TGlobalONRegistryEventCallback callback( this, &CTGlobalONRegistry< T, LockType >::OnSomeShutdownEvent );
    CTONRegistry< T, LockType >::AsObserver().SubscribeTo( &CORE::CCoreGlobal::Instance()->GetApplication()  ,
                                                           CORE::CGUCEFApplication::AppShutdownCompleteEvent ,
                                                           callback                                         );
    TGlobalONRegistryEventCallback callback2( this, &CTGlobalONRegistry< T, LockType >::OnSomeShutdownEvent );
    CTONRegistry< T, LockType >::AsObserver().SubscribeTo( &CORE::CCoreGlobal::Instance()->GetPluginControl()       ,
                                                           CORE::CPluginControl::UnregisterOfAllPluginsStartedEvent ,
                                                           callback2                                                );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTONREGISTRY_H  ? */
