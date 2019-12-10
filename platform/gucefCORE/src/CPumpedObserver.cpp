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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#include "CPumpedObserver.h"

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
 *  Internally used data container class used for mailing
 *  extra data using the CMailBox class.
 */
class CMailElement : public CICloneable
{
    private:
    friend class CPumpedObserver;

    CMailElement( CNotifier* notifier ,
                  CICloneable* eventdata )
        : m_notifier( notifier )  ,
          m_eventdata( eventdata )
    {GUCEF_TRACE;

    }

    CMailElement( const CMailElement& src )
        : m_notifier( src.m_notifier )  ,
          m_eventdata( src.m_eventdata )
    {GUCEF_TRACE;

    }

    virtual ~CMailElement()
    {GUCEF_TRACE;

    }

    CMailElement& operator=( const CMailElement& src )
    {GUCEF_TRACE;

        if ( this != &src )
        {
            m_notifier = src.m_notifier;
            m_eventdata = src.m_eventdata;
        }
        return *this;
    }

    inline CNotifier* GetNotifier( void ) const
    {GUCEF_TRACE;

        return m_notifier;
    }

    inline CICloneable* GetData( void ) const
    {GUCEF_TRACE;

        return m_eventdata;
    }

    virtual CICloneable* Clone( void ) const
    {GUCEF_TRACE;

        return new CMailElement( *this );
    }

    private:

    CMailElement( void )
    {GUCEF_TRACE;

        assert( 0 );
        /* dummy, do not use */
    }

    CICloneable* m_eventdata;
    CNotifier* m_notifier;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPumpedObserver::CPumpedObserver( void )
    : CObserver()                                                      ,
      m_pulsGenerator( &CCoreGlobal::Instance()->GetPulseGenerator() ) ,
      m_mutex()
{GUCEF_TRACE;

    TEventCallback callback( this, &CPumpedObserver::OnPulse );
    SubscribeTo( m_pulsGenerator             ,
                 CPulseGenerator::PulseEvent ,
                 callback                    );
    TEventCallback callback2( this, &CPumpedObserver::OnPulseGeneratorDestruction );
    SubscribeTo( m_pulsGenerator                   ,
                 CPulseGenerator::DestructionEvent ,
                 callback2                         );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::CPumpedObserver( CPulseGenerator& pulsGenerator )
    : CObserver()                       ,
      m_pulsGenerator( &pulsGenerator ) ,
      m_mutex()
{GUCEF_TRACE;

    TEventCallback callback( this, &CPumpedObserver::OnPulse );
    SubscribeTo( m_pulsGenerator             ,
                 CPulseGenerator::PulseEvent ,
                 callback                    );
    TEventCallback callback2( this, &CPumpedObserver::OnPulseGeneratorDestruction );
    SubscribeTo( m_pulsGenerator                   ,
                 CPulseGenerator::DestructionEvent ,
                 callback2                         );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::CPumpedObserver( const CPumpedObserver& src )
    : CObserver( src )        ,
      m_pulsGenerator( NULL ) ,
      m_mutex()
{GUCEF_TRACE;

    // This is an aggregate relationship that does not affect the source object
    // thus a const cast is acceptable.
    m_pulsGenerator = const_cast< CPumpedObserver& >( src ).m_pulsGenerator;

    // The source should not be in a state of destruction causing this
    assert( NULL != m_pulsGenerator );

    TEventCallback callback( this, &CPumpedObserver::OnPulse );
    SubscribeTo( m_pulsGenerator             ,
                 CPulseGenerator::PulseEvent ,
                 callback                    );
    TEventCallback callback2( this, &CPumpedObserver::OnPulseGeneratorDestruction );
    SubscribeTo( m_pulsGenerator                   ,
                 CPulseGenerator::DestructionEvent ,
                 callback2                         );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::~CPumpedObserver()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPumpedObserver&
CPumpedObserver::operator=( const CPumpedObserver& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnPulse( CNotifier* notifier                 ,
                          const CEvent& eventid               ,
                          CICloneable* eventdata /* = NULL */ )

{GUCEF_TRACE;

    CEvent mailEventID;
    CICloneable* dataptr( NULL );
    CMailElement* maildata( NULL );
    while ( m_mailbox.GetMail( mailEventID ,
                               &dataptr    ) )
    {
        maildata = static_cast< CMailElement* >( dataptr );
        OnPumpedNotify( maildata->GetNotifier() ,
                        mailEventID             ,
                        maildata->GetData()     );

        delete maildata->GetData();
        delete maildata;
    }
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnPulseGeneratorDestruction( CNotifier* notifier                 ,
                                              const CEvent& eventid               ,
                                              CICloneable* eventdata /* = NULL */ )

{GUCEF_TRACE;

    if ( notifier == m_pulsGenerator )
    {
        m_pulsGenerator = NULL;
    }
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnNotify( CNotifier* notifier                 ,
                           const CEvent& eventid               ,
                           CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( eventdata )
    {
        eventdata = eventdata->Clone();
    }

    CMailElement maildata( notifier  ,
                           eventdata );

    m_mailbox.AddMail( eventid   ,
                       &maildata );

    if ( NULL != m_pulsGenerator )
    {
        m_pulsGenerator->RequestPulse();
    }
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::OnPumpedNotify( CNotifier* notifier                 ,
                                 const CEvent& eventid               ,
                                 CICloneable* eventdata /* = NULL */ )

{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::LockData( void ) const
{GUCEF_TRACE;

    m_mutex.Lock();
}

/*-------------------------------------------------------------------------*/

void
CPumpedObserver::UnlockData( void ) const
{GUCEF_TRACE;

    m_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

const CString&
CPumpedObserver::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CPumpedObserver";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
