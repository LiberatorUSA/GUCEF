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

#ifndef GUCEF_INPUT_KEYBOARD_H
#define GUCEF_INPUT_KEYBOARD_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

//! Keyboard scan codes
enum KeyCode
{
	KEYCODE_UNASSIGNED  = 0x00,
	KEYCODE_ESCAPE      = 0x01,
	KEYCODE_1           = 0x02,
	KEYCODE_2           = 0x03,
	KEYCODE_3           = 0x04,
	KEYCODE_4           = 0x05,
	KEYCODE_5           = 0x06,
	KEYCODE_6           = 0x07,
	KEYCODE_7           = 0x08,
	KEYCODE_8           = 0x09,
	KEYCODE_9           = 0x0A,
	KEYCODE_0           = 0x0B,
	KEYCODE_MINUS       = 0x0C,    // - on main keyboard
	KEYCODE_EQUALS      = 0x0D,
	KEYCODE_BACK        = 0x0E,    // backspace
	KEYCODE_TAB         = 0x0F,
	KEYCODE_Q           = 0x10,
	KEYCODE_W           = 0x11,
	KEYCODE_E           = 0x12,
	KEYCODE_R           = 0x13,
	KEYCODE_T           = 0x14,
	KEYCODE_Y           = 0x15,
	KEYCODE_U           = 0x16,
	KEYCODE_I           = 0x17,
	KEYCODE_O           = 0x18,
	KEYCODE_P           = 0x19,
	KEYCODE_LBRACKET    = 0x1A,
	KEYCODE_RBRACKET    = 0x1B,
	KEYCODE_RETURN      = 0x1C,    // Enter on main keyboard
	KEYCODE_LCONTROL    = 0x1D,
	KEYCODE_A           = 0x1E,
	KEYCODE_S           = 0x1F,
	KEYCODE_D           = 0x20,
	KEYCODE_F           = 0x21,
	KEYCODE_G           = 0x22,
	KEYCODE_H           = 0x23,
	KEYCODE_J           = 0x24,
	KEYCODE_K           = 0x25,
	KEYCODE_L           = 0x26,
	KEYCODE_SEMICOLON   = 0x27,
	KEYCODE_APOSTROPHE  = 0x28,
	KEYCODE_GRAVE       = 0x29,    // accent
	KEYCODE_LSHIFT      = 0x2A,
	KEYCODE_BACKSLASH   = 0x2B,
	KEYCODE_Z           = 0x2C,
	KEYCODE_X           = 0x2D,
	KEYCODE_C           = 0x2E,
	KEYCODE_V           = 0x2F,
	KEYCODE_B           = 0x30,
	KEYCODE_N           = 0x31,
	KEYCODE_M           = 0x32,
	KEYCODE_COMMA       = 0x33,
	KEYCODE_PERIOD      = 0x34,    // . on main keyboard
	KEYCODE_SLASH       = 0x35,    // / on main keyboard
	KEYCODE_RSHIFT      = 0x36,
	KEYCODE_MULTIPLY    = 0x37,    // * on numeric keypad
	KEYCODE_LMENU       = 0x38,    // left Alt
	KEYCODE_SPACE       = 0x39,
	KEYCODE_CAPITAL     = 0x3A,
	KEYCODE_F1          = 0x3B,
	KEYCODE_F2          = 0x3C,
	KEYCODE_F3          = 0x3D,
	KEYCODE_F4          = 0x3E,
	KEYCODE_F5          = 0x3F,
	KEYCODE_F6          = 0x40,
	KEYCODE_F7          = 0x41,
	KEYCODE_F8          = 0x42,
	KEYCODE_F9          = 0x43,
	KEYCODE_F10         = 0x44,
	KEYCODE_NUMLOCK     = 0x45,
	KEYCODE_SCROLL      = 0x46,    // Scroll Lock
	KEYCODE_NUMPAD7     = 0x47,
	KEYCODE_NUMPAD8     = 0x48,
	KEYCODE_NUMPAD9     = 0x49,
	KEYCODE_SUBTRACT    = 0x4A,    // - on numeric keypad
	KEYCODE_NUMPAD4     = 0x4B,
	KEYCODE_NUMPAD5     = 0x4C,
	KEYCODE_NUMPAD6     = 0x4D,
	KEYCODE_ADD         = 0x4E,    // + on numeric keypad
	KEYCODE_NUMPAD1     = 0x4F,
	KEYCODE_NUMPAD2     = 0x50,
	KEYCODE_NUMPAD3     = 0x51,
	KEYCODE_NUMPAD0     = 0x52,
	KEYCODE_DECIMAL     = 0x53,    // . on numeric keypad
	KEYCODE_OEM_102     = 0x56,    // < > | on UK/Germany keyboards
	KEYCODE_F11         = 0x57,
	KEYCODE_F12         = 0x58,
	KEYCODE_F13         = 0x64,    //                     (NEC PC98)
	KEYCODE_F14         = 0x65,    //                     (NEC PC98)
	KEYCODE_F15         = 0x66,    //                     (NEC PC98)
	KEYCODE_KANA        = 0x70,    // (Japanese keyboard)
	KEYCODE_ABNT_C1     = 0x73,    // / ? on Portugese (Brazilian) keyboards
	KEYCODE_CONVERT     = 0x79,    // (Japanese keyboard)
	KEYCODE_NOCONVERT   = 0x7B,    // (Japanese keyboard)
	KEYCODE_YEN         = 0x7D,    // (Japanese keyboard)
	KEYCODE_ABNT_C2     = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
	KEYCODE_NUMPADEQUALS= 0x8D,    // = on numeric keypad (NEC PC98)
	KEYCODE_PREVTRACK   = 0x90,    // Previous Track (KEYCODE_CIRCUMFLEX on Japanese keyboard)
	KEYCODE_AT          = 0x91,    //                     (NEC PC98)
	KEYCODE_COLON       = 0x92,    //                     (NEC PC98)
	KEYCODE_UNDERLINE   = 0x93,    //                     (NEC PC98)
	KEYCODE_KANJI       = 0x94,    // (Japanese keyboard)
	KEYCODE_STOP        = 0x95,    //                     (NEC PC98)
	KEYCODE_AX          = 0x96,    //                     (Japan AX)
	KEYCODE_UNLABELED   = 0x97,    //                        (J3100)
	KEYCODE_NEXTTRACK   = 0x99,    // Next Track
	KEYCODE_NUMPADENTER = 0x9C,    // Enter on numeric keypad
	KEYCODE_RCONTROL    = 0x9D,
	KEYCODE_MUTE        = 0xA0,    // Mute
	KEYCODE_CALCULATOR  = 0xA1,    // Calculator
	KEYCODE_PLAYPAUSE   = 0xA2,    // Play / Pause
	KEYCODE_MEDIASTOP   = 0xA4,    // Media Stop
	KEYCODE_VOLUMEDOWN  = 0xAE,    // Volume -
	KEYCODE_VOLUMEUP    = 0xB0,    // Volume +
	KEYCODE_WEBHOME     = 0xB2,    // Web home
	KEYCODE_NUMPADCOMMA = 0xB3,    // , on numeric keypad (NEC PC98)
	KEYCODE_DIVIDE      = 0xB5,    // / on numeric keypad
	KEYCODE_SYSRQ       = 0xB7,
	KEYCODE_RMENU       = 0xB8,    // right Alt
	KEYCODE_PAUSE       = 0xC5,    // Pause
	KEYCODE_HOME        = 0xC7,    // Home on arrow keypad
	KEYCODE_UP          = 0xC8,    // UpArrow on arrow keypad
	KEYCODE_PGUP        = 0xC9,    // PgUp on arrow keypad
	KEYCODE_LEFT        = 0xCB,    // LeftArrow on arrow keypad
	KEYCODE_RIGHT       = 0xCD,    // RightArrow on arrow keypad
	KEYCODE_END         = 0xCF,    // End on arrow keypad
	KEYCODE_DOWN        = 0xD0,    // DownArrow on arrow keypad
	KEYCODE_PGDOWN      = 0xD1,    // PgDn on arrow keypad
	KEYCODE_INSERT      = 0xD2,    // Insert on arrow keypad
	KEYCODE_DELETE      = 0xD3,    // Delete on arrow keypad
	KEYCODE_LWIN        = 0xDB,    // Left Windows key
	KEYCODE_RWIN        = 0xDC,    // Right Windows key
	KEYCODE_APPS        = 0xDD,    // AppMenu key
	KEYCODE_POWER       = 0xDE,    // System Power
	KEYCODE_SLEEP       = 0xDF,    // System Sleep
	KEYCODE_WAKE        = 0xE3,    // System Wake
	KEYCODE_WEBSEARCH   = 0xE5,    // Web Search
	KEYCODE_WEBFAVORITES= 0xE6,    // Web Favorites
	KEYCODE_WEBREFRESH  = 0xE7,    // Web Refresh
	KEYCODE_WEBSTOP     = 0xE8,    // Web Stop
	KEYCODE_WEBFORWARD  = 0xE9,    // Web Forward
	KEYCODE_WEBBACK     = 0xEA,    // Web Back
	KEYCODE_MYCOMPUTER  = 0xEB,    // My Computer
	KEYCODE_MAIL        = 0xEC,    // Mail
	KEYCODE_MEDIASELECT = 0xED,    // Media Select
	
	KEYCODE_MAX         = 0xEE     // for iteration only, not a real key code
};

/*-------------------------------------------------------------------------*/

//! Enum of bit position of modifier
enum KeyModifier
{
	KEYMOD_SHIFT = 0x0000001 ,
	KEYMOD_CTRL  = 0x0000010 ,
	KEYMOD_ALT   = 0x0000100 ,
	
	KEYMOD_NONE  = 0x0000000
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CString GUCEF_INPUT_EXPORT_CPP KeyModifierToString( const KeyModifier keyModifier );

/*-------------------------------------------------------------------------*/

KeyModifier GUCEF_INPUT_EXPORT_CPP StringToKeyModifier( const CString& keyModStr );

/*-------------------------------------------------------------------------*/

CString GUCEF_INPUT_EXPORT_CPP KeyCodeToString( const KeyCode keyCode );

/*-------------------------------------------------------------------------*/

KeyCode GUCEF_INPUT_EXPORT_CPP StringToKeyCode( const CString& keyCodeStr );

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_KEYBOARD_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-09-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/