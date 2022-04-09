/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef GUCEF_INPUT_CTCONCRETEACTIONEVENTDATA_H
#define GUCEF_INPUT_CTCONCRETEACTIONEVENTDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_INPUT_CACTIONEVENTDATA_H
#include "gucefINPUT_CActionEventData.h"
#define GUCEF_INPUT_CACTIONEVENTDATA_H
#endif /* GUCEF_INPUT_CACTIONEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Because input events tend to happen rather frequently this class was created
 *  to allow the event data to be copied without dynamic allocation on a per-event basis
 *  It is an optimization.
 */
template < class T >
class CTConcreteActionEventData : public CActionEventData
{
    public:
    
    CTConcreteActionEventData( const UInt32 actionID        ,
                               const CORE::CEvent& orgEvent ,
                               const T& data                );
    
    CTConcreteActionEventData( const CTConcreteActionEventData& src );
    
    virtual ~CTConcreteActionEventData();
    
    virtual const CORE::CICloneable* GetEventData( void ) const;
    
    virtual CORE::CICloneable* Clone( void ) const;
    
    private:
    
    CTConcreteActionEventData( void );
    CTConcreteActionEventData& operator=( const CTConcreteActionEventData& src );
    
    private:
    
    T m_data;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < class T >
CTConcreteActionEventData< T >::CTConcreteActionEventData( const UInt32 actionID        ,
                                                           const CORE::CEvent& orgEvent ,
                                                           const T& data                )
    : CActionEventData( actionID ,
                        orgEvent ) ,
      m_data( data )
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

template < class T >
CTConcreteActionEventData< T >::CTConcreteActionEventData( const CTConcreteActionEventData& src )
    : CActionEventData( src ) ,
      m_data( src.m_data )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

template < class T >
CTConcreteActionEventData< T >::~CTConcreteActionEventData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < class T >
const CORE::CICloneable*
CTConcreteActionEventData< T >::GetEventData( void ) const
{GUCEF_TRACE;
    
    return &m_data;
}

/*-------------------------------------------------------------------------*/

template < class T >
CORE::CICloneable*
CTConcreteActionEventData< T >::Clone( void ) const
{GUCEF_TRACE;

    return new CTConcreteActionEventData< T >( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CTCONCRETEACTIONEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/
