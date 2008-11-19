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

#ifndef GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H
#define GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_INPUT_KEYBOARD_H
#include "gucefINPUT_keyboard.h"
#define GUCEF_INPUT_KEYBOARD_H
#endif /* GUCEF_INPUT_KEYBOARD_H ? */

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

class GUCEF_INPUT_EXPORT_CPP CKeyStateChangedEventData : public CORE::CICloneable
{
    public:
    
    typedef std::map< KeyModifier, bool > TKeyModStates;
    
    CKeyStateChangedEventData( const UInt32 deviceID     ,
                               const KeyCode keyCode     ,
                               const bool pressedState   ,
                               const UInt32 keyModStates );
    
    CKeyStateChangedEventData( const CKeyStateChangedEventData& src );
                        
    virtual ~CKeyStateChangedEventData();
        
    UInt32 GetDeviceID( void ) const;
    
    KeyCode GetKeyCode( void ) const;
    
    bool GetKeyPressedState( void ) const;
    
    bool GetKeyModPressedState( const KeyModifier keyModifier ) const;
    
    UInt32 GetKeyModPressedStates( void ) const;
    
    virtual CORE::CICloneable* Clone( void ) const;
    
    private:
    
    CKeyStateChangedEventData( void );                                             /**< not implemented, can't use */
    CKeyStateChangedEventData& operator=( const CKeyStateChangedEventData& src );  /**< not implemented, no need */
    
    private:
    
    UInt32 m_deviceID;
    KeyCode m_keyCode;
    bool m_pressedState;
    UInt32 m_modStates;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-09-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/