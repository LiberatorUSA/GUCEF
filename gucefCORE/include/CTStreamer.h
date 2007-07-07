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

#ifndef GUCEF_CORE_CTSTREAMER_H
#define GUCEF_CORE_CTSTREAMER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_CSTREAMEREVENTS_H
#include "CStreamerEvents.h"
#define GUCEF_CORE_CSTREAMEREVENTS_H
#endif /* GUCEF_CORE_CSTREAMEREVENTS_H ? */

#ifndef GUCEF_CORE_STREAMABLES_H
#include "streamables.h"
#define GUCEF_CORE_STREAMABLES_H
#endif /* GUCEF_CORE_STREAMABLES_H ? */

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
 *  Template for easy creation of streamers
 *  Note that it only works for simple types that consist of a single block of 
 *  memory or types that have a template specialization for CTStreamableObj<>   
 *  in the streamables list.
 */
template < typename idType, typename valueType >
class CTStreamer : public CNotifier       ,
                   public CStreamerEvents   /* <- interface class */
{
    public:
    
    CTStreamer( void );
    
    CTStreamer( const idType& newID       ,
                const valueType& newValue );
    
    CTStreamer( const CTStreamer& src );
    
    virtual ~CTStreamer();
    
    CTStreamer& operator=( const CTStreamer& src );
    
    void SetID( const idType& newID );
    
    const idType& GetID( void ) const;
    
    void SetValue( const valueType& newValue );
    
    const valueType& GetValue( void ) const;
    
    private:
    CTStreamableObj< idType >    m_id;
    CTStreamableObj< valueType > m_value;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename idType, typename valueType >
CTStreamer< idType, valueType >::CTStreamer( void )
    : CNotifier()       ,
      CStreamerEvents() ,
      m_id()            ,
      m_value()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename idType, typename valueType >
CTStreamer< idType, valueType >::CTStreamer( const idType& newID       ,
                                             const valueType& newValue )
    : CNotifier()         ,
      CStreamerEvents()   ,
      m_id( newID )       ,
      m_value( newValue )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename idType, typename valueType >
CTStreamer< idType, valueType >::CTStreamer( const CTStreamer& src )
    : CNotifier( src )       ,
      CStreamerEvents( src ) ,
      m_id( src.m_id )       ,
      m_value( src.m_value )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename idType, typename valueType >
CTStreamer< idType, valueType >::~CTStreamer()
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

template < typename idType, typename valueType >
CTStreamer< idType, valueType >&
CTStreamer< idType, valueType >::operator=( const CTStreamer& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        // We intentionally do not invoke the assignment operator for the CNotifier
        // base class because typically you want to assign only the values and not
        // copy the subscriptions
        
        CStreamerEvents::operator=( src );
        
        m_id.GetDataRef() = src.m_id.GetConstDataRef();
        m_value.GetDataRef() = src.m_value.GetConstDataRef();
        
        struct SStreamEventData dataClamp = { &m_id, &m_value };
        TStreamEventData eData( dataClamp );        
        
        NotifyObservers( IDStreamEvent, &eData );
        NotifyObservers( DataStreamEvent, &eData );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template < typename idType, typename valueType >
void
CTStreamer< idType, valueType >::SetID( const idType& newID )
{GUCEF_TRACE;

    m_id.GetDataRef() = newID;
    
    struct SStreamEventData dataClamp = { &m_id, &m_value };
    TStreamEventData eData( dataClamp );
    
    NotifyObservers( IDStreamEvent, &eData );
}

/*-------------------------------------------------------------------------*/

template < typename idType, typename valueType >
const idType&
CTStreamer< idType, valueType >::GetID( void ) const
{GUCEF_TRACE;

    return m_id.GetConstDataRef();
}

/*-------------------------------------------------------------------------*/
    
template < typename idType, typename valueType >
void
CTStreamer< idType, valueType >::SetValue( const valueType& newValue )
{GUCEF_TRACE;

    m_value.GetDataRef() = newValue;
    
    struct SStreamEventData dataClamp = { &m_id, &m_value };
    TStreamEventData eData( dataClamp );
        
    NotifyObservers( DataStreamEvent, &eData );
}

/*-------------------------------------------------------------------------*/

template < typename idType, typename valueType >
const valueType&
CTStreamer< idType, valueType >::GetValue( void ) const
{GUCEF_TRACE;

    return m_value.GetConstDataRef();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTSTREAMER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-06-2007 :
        - Dinand: Added this header

-----------------------------------------------------------------------------*/
