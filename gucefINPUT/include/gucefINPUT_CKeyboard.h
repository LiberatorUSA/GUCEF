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

#ifndef GUCEF_INPUT_CKEYBOARD_H
#define GUCEF_INPUT_CKEYBOARD_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_INPUT_KEYBOARD_H
#include "gucefINPUT_keyboard.h"
#define GUCEF_INPUT_KEYBOARD_H
#endif /* GUCEF_INPUT_KEYBOARD_H ? */

#ifndef GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H
#include "gucefINPUT_CKeyStateChangedEventData.h"
#define GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H
#endif /* GUCEF_INPUT_CKEYSTATECHANGEDEVENTDATA_H ? */

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

class CInputController;

/*-------------------------------------------------------------------------*/

/**
 *  In-software representation of a keyboard hardware input device
 */
class GUCEF_INPUT_EXPORT_CPP CKeyboard : public CORE::CNotifier
{
    public:
    
    static const CORE::CEvent KeyStateChangedEvent;
    static const CORE::CEvent KeyModStateChangedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    typedef std::map< KeyCode, bool > TKeyStates;
    
    bool IsKeyDown( const KeyCode key ) const;
    
    bool IsKeyModifierActive( const KeyModifier keyMod ) const;
    
    const TKeyStates& GetKeyStates( void ) const;
    
    UInt32 GetKeyModStates( void ) const;
    
    /**
     *  Attempts to convert the given KeyCode into a unicode 
     *  compatible value.
     *
     *  @param keyCode the keyCode to translate into unicode
     *  @param keyModifiers placeholder for the key modifier flags
     *  @param unicode the output variable for the unicode translation
     *  @return whether the KeyCode could be converted 
     */
    virtual bool GetUnicodeForKeyCode( const KeyCode keyCode     ,
                                       const UInt32 keyModifiers ,
                                       UInt32& unicode           ) const;

    virtual const CString& GetType( void ) const;
    
    private:
    friend class CInputController;
    
    CKeyboard( CInputController* controller );
    
    virtual ~CKeyboard();
    
    void SetKeyState( const KeyCode key       , 
                      const bool pressedState );

    void SetKeyModState( const KeyModifier keyMod , 
                         const bool pressedState  );
                         
    void ResetKeyboardState( void );
       
    private:
    
    CKeyboard( const CKeyboard& src );
    CKeyboard& operator=( const CKeyboard& src );
    
    private:
    
    TKeyStates m_keyState;
    UInt32 m_keyModStates;
    CInputController* m_controller;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CKEYBOARD_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-09-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/
