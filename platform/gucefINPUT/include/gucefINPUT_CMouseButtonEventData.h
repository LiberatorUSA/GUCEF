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

#ifndef GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H
#define GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_INPUT_MACROS_H
#include "gucefINPUT_macros.h"
#define GUCEF_INPUT_MACROS_H
#endif /* GUCEF_INPUT_MACROS_H ? */

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

class GUCEF_INPUT_PUBLIC_CPP CMouseButtonEventData : public CORE::CICloneable
{
    public:
    
    CMouseButtonEventData( const Int32 contextId    ,
                           const UInt32 deviceID    ,
                           const UInt32 buttonIndex ,
                           const bool pressedState  ,
                           const Int32 xPos         ,
                           const Int32 yPos         );
    
    CMouseButtonEventData( const CMouseButtonEventData& src );
                        
    virtual ~CMouseButtonEventData();
    
    Int32 GetContextId( void ) const;
    
    UInt32 GetDeviceID( void ) const;
        
    UInt32 GetButtonIndex( void ) const;
    
    bool GetPressedState( void ) const;
    
    Int32 GetXPos( void ) const;
    
    Int32 GetYPos( void ) const;
    
    virtual CORE::CICloneable* Clone( void ) const;
    
    private:
    
    CMouseButtonEventData( void );                                         /**< not implemented, can't use */
    CMouseButtonEventData& operator=( const CMouseButtonEventData& src );  /**< not implemented, no need */
    
    private:
    
    Int32 m_contextId;
    UInt32 m_deviceID;
    UInt32 m_buttonIndex;
    bool m_pressedState;
    Int32 m_xPos;
    Int32 m_yPos;    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-09-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/