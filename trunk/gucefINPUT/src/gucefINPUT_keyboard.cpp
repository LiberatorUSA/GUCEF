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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "gucefINPUT_keyboard.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CString
KeyModifierToString( const KeyModifier keyModifier )
{GUCEF_TRACE;

    switch ( keyModifier )
    {
        case KEYMOD_SHIFT :
        {
            return "SHIFT";
        }
	    case KEYMOD_CTRL :
	    {
	        return "CTRL";
        }
        case KEYMOD_ALT :
        {
            return "ALT";
        }
	    case KEYMOD_NONE :
	    {
	        return "NONE";
	    }
        default:
        {
            GUCEF_UNREACHABLE;
            return "";
        }
    }
}

/*-------------------------------------------------------------------------*/

KeyModifier
KeyModifierToString( const CString& keyModStr )
{GUCEF_TRACE;

    if ( keyModStr == "SHIFT" )
    {
        return KEYMOD_SHIFT;
    }
    else
    if ( keyModStr == "CTRL" )
    {
        return KEYMOD_CTRL;
    }
    else
    if ( keyModStr == "ALT" )
    {
        return KEYMOD_ALT;
    }

    return KEYMOD_NONE;
}

/*-------------------------------------------------------------------------*/

CString
KeyCodeToString( const KeyCode keyCode )
{GUCEF_TRACE;

    switch ( keyCode )
    {
        case KEYCODE_UNASSIGNED :
        {
            return "UNASSIGNED";
        }
        case KEYCODE_ESCAPE :
        {
            return "ESCAPE";
        }
        case KEYCODE_1 :
        {
            return "1";
        }
        case KEYCODE_2 :
        {
            return "2";
        }
        case KEYCODE_3 :
        {
            return "3";
        }
        case KEYCODE_4 :
        {
            return "4";
        } 
        case KEYCODE_5 :
        {
            return "5";
        } 
        case KEYCODE_6 :
        {
            return "6";
        } 
        case KEYCODE_7 :
        {
            return "7";
        } 
        case KEYCODE_8 :
        {
            return "8";
        } 
        case KEYCODE_9 :
        {
            return "9";
        }
        case KEYCODE_0 :
        {
            return "0";
        }                                                                                 
        case KEYCODE_MINUS :
        {
            return "MINUS";
        }
        case KEYCODE_EQUALS :
        {
            return "EQUALS";
        }
        case KEYCODE_BACK :
        {
            return "BACK";
        }
        case KEYCODE_TAB :
        {
            return "TAB";
        }
        case KEYCODE_Q :
        {
            return "Q";
        }
        case KEYCODE_W :
        {
            return "W";
        }
        case KEYCODE_E :
        {
            return "E";
        }
        case KEYCODE_R :
        {
            return "R";
        }
        case KEYCODE_T :
        {
            return "T";
        }
        case KEYCODE_Y :
        {
            return "Y";
        }
        case KEYCODE_U :
        {
            return "U";
        }
        case KEYCODE_I :
        {
            return "I";
        }
        case KEYCODE_O :
        {
            return "O";
        }
        case KEYCODE_P :
        {
            return "P";
        }
        case KEYCODE_LBRACKET :
        {
            return "LBRACKET";
        }
        case KEYCODE_RBRACKET :
        {
            return "RBRACKET";
        }
        case KEYCODE_RETURN :
        {
            return "RETURN";
        }
        case KEYCODE_LCONTROL :
        {
            return "LCONTROL";
        }
        case KEYCODE_A :
        {
            return "A";
        }
        case KEYCODE_S :
        {
            return "S";
        }
        case KEYCODE_D :
        {
            return "D";
        }
        case KEYCODE_F :
        {
            return "F";
        }
        case KEYCODE_G :
        {
            return "G";
        }
        case KEYCODE_H :
        {
            return "H";
        }                                                                                                                                                                
        case KEYCODE_J :
        {
            return "J";
        }
        case KEYCODE_K :
        {
            return "K";
        }                
        case KEYCODE_L :
        {
            return "L";
        }
        case KEYCODE_SEMICOLON :
        {
            return "SEMICOLON";
        }
        case KEYCODE_APOSTROPHE :
        {
            return "APOSTROPHE";
        }
        case KEYCODE_GRAVE :
        {
            return "GRAVE";
        }
        case KEYCODE_LSHIFT :
        {
            return "LSHIFT";
        }
        case KEYCODE_BACKSLASH :
        {
            return "BACKSLASH";
        }
        case KEYCODE_Z :
        {
            return "Z";
        }
        case KEYCODE_X :
        {
            return "X";
        }
        case KEYCODE_C :
        {
            return "C";
        }
        case KEYCODE_V :
        {
            return "V";
        }
        case KEYCODE_B :
        {
            return "B";
        }
        case KEYCODE_N :
        {
            return "N";
        }
        case KEYCODE_M :
        {
            return "M";
        }
        case KEYCODE_COMMA :
        {
            return "COMMA";
        }
        case KEYCODE_PERIOD :
        {
            return "PERIOD";
        }
        case KEYCODE_SLASH :
        {
            return "SLASH";
        }
        case KEYCODE_RSHIFT :
        {
            return "RSHIFT";
        }
        case KEYCODE_MULTIPLY :
        {
            return "MULTIPLY";
        }
        case KEYCODE_LMENU :
        {
            return "LMENU";
        }
        case KEYCODE_SPACE :
        {
            return "SPACE";
        }
        case KEYCODE_CAPITAL :
        {
            return "CAPITAL";
        }
        case KEYCODE_F1 :
        {
            return "F1";
        }
        case KEYCODE_F2 :
        {
            return "F2";
        }
        case KEYCODE_F3 :
        {
            return "F3";
        }
        case KEYCODE_F4 :
        {
            return "F4";
        }
        case KEYCODE_F5 :
        {
            return "F5";
        }
        case KEYCODE_F6 :
        {
            return "F6";
        }
        case KEYCODE_F7 :
        {
            return "F7";
        }
        case KEYCODE_F8 :
        {
            return "F8";
        }
        case KEYCODE_F9 :
        {
            return "F9";
        }
        case KEYCODE_F10 :
        {
            return "F10";
        }
        case KEYCODE_NUMLOCK :
        {
            return "NUMLOCK";
        }
        case KEYCODE_SCROLL :
        {
            return "SCROLL";
        }
        case KEYCODE_NUMPAD7 :
        {
            return "NUMPAD7";
        }
        case KEYCODE_NUMPAD8 :
        {
            return "NUMPAD8";
        }
        case KEYCODE_NUMPAD9 :
        {
            return "NUMPAD9";
        }
        case KEYCODE_SUBTRACT :
        {
            return "SUBTRACT";
        }
        case KEYCODE_NUMPAD4 :
        {
            return "NUMPAD4";
        }
        case KEYCODE_NUMPAD5 :
        {
            return "NUMPAD5";
        }
        case KEYCODE_NUMPAD6 :
        {
            return "NUMPAD6";
        }
        case KEYCODE_ADD :
        {
            return "ADD";
        }
        case KEYCODE_NUMPAD1 :
        {
            return "NUMPAD1";
        }
        case KEYCODE_NUMPAD2 :
        {
            return "NUMPAD2";
        }
        case KEYCODE_NUMPAD3 :
        {
            return "NUMPAD3";
        }
        case KEYCODE_NUMPAD0 :
        {
            return "NUMPAD0";
        }
        case KEYCODE_DECIMAL : // . on numeric keypad
        {
            return "DECIMAL";  
        }
        case KEYCODE_LEFTWINKEY :
        {
            return "LEFTWINKEY";
        }
        case KEYCODE_RIGHTWINKEY :
        {
            return "RIGHTWINKEY";
        }        
        case KEYCODE_OEM_102 : // < > | on UK/Germany keyboards
        {
            return "OEM_102";  
        }
        case KEYCODE_F11 :
        {
            return "F11";
        }
        case KEYCODE_F12 :
        {
            return "F12";
        }
        case KEYCODE_F13 :
        {
            return "F13";
        }
        case KEYCODE_F14 :
        {
            return "F14";
        }
        case KEYCODE_F15 :
        {
            return "F15";
        }
        case KEYCODE_KANA : // (Japanese keyboard)
        {
            return "KANA";  
        }
        case KEYCODE_ABNT_C1 : // / ? on Portuguese (Brazilian) keyboards
        {
            return "ABNT_C1"; 
        }
        case KEYCODE_CONVERT : // (Japanese keyboard)
        {
            return "CONVERT"; 
        }
        case KEYCODE_NOCONVERT : // (Japanese keyboard)
        {
            return "NOCONVERT";  
        }
        case KEYCODE_YEN :  // (Japanese keyboard)
        {
            return "YEN";
        }
        case KEYCODE_ABNT_C2 : // Numpad . on Portuguese (Brazilian) keyboards
        {
            return "ABNT_C2";
        }
        case KEYCODE_NUMPADEQUALS : // = on numeric keypad (NEC PC98)
        {
            return "NUMPADEQUALS";
        }
        case KEYCODE_PREVTRACK : // Previous Track (KEYCODE_CIRCUMFLEX on Japanese keyboard)
        {
            return "PREVTRACK";
        }
        case KEYCODE_AT : // (NEC PC98)
        {
            return "AT";
        }
        case KEYCODE_COLON : // (NEC PC98)
        {
            return "COLON";
        }
        case KEYCODE_UNDERLINE : // (NEC PC98)
        {
            return "UNDERLINE";
        }
        case KEYCODE_KANJI : // (Japanese keyboard)
        {
            return "KANJI";
        }
        case KEYCODE_STOP : // (NEC PC98)
        {
            return "STOP";
        }
        case KEYCODE_AX : // (Japan AX)
        {
            return "AX";
        }
        case KEYCODE_UNLABELED : // (J3100)
        {
            return "UNLABELED";
        }
        case KEYCODE_NEXTTRACK : // Next Track
        {
            return "NEXTTRACK";
        }                                                                        
        case KEYCODE_NUMPADENTER : // Enter on numeric keypad
        {
            return "NUMPADENTER";
        }
        case KEYCODE_RCONTROL :
        {
            return "RCONTROL";
        }
        case KEYCODE_MUTE :
        {
            return "MUTE";
        }
        case KEYCODE_CALCULATOR :
        {
            return "CALCULATOR";
        }
        case KEYCODE_PLAYPAUSE :
        {
            return "PLAYPAUSE";
        }
        case KEYCODE_MEDIASTOP :
        {
            return "MEDIASTOP";
        }
        case KEYCODE_VOLUMEDOWN :
        {
            return "VOLUMEDOWN";
        }
        case KEYCODE_VOLUMEUP :
        {
            return "VOLUMEUP";
        }
        case KEYCODE_WEBHOME :
        {
            return "WEBHOME";
        }
        case KEYCODE_NUMPADCOMMA :
        {
            return "NUMPADCOMMA";
        }
        case KEYCODE_DIVIDE :
        {
            return "DIVIDE";
        }
        case KEYCODE_SYSRQ :
        {
            return "SYSRQ";
        }
        case KEYCODE_RMENU :
        {
            return "RMENU";
        }
        case KEYCODE_PAUSE :
        {
            return "PAUSE";
        }
        case KEYCODE_HOME :
        {
            return "HOME";
        }
        case KEYCODE_UP :
        {
            return "UP";
        }
        case KEYCODE_PGUP :
        {
            return "PGUP";
        }
        case KEYCODE_LEFT :
        {
            return "LEFT";
        }
        case KEYCODE_RIGHT :
        {
            return "RIGHT";
        }
        case KEYCODE_END :
        {
            return "END";
        }
        case KEYCODE_DOWN :
        {
            return "DOWN";
        }
        case KEYCODE_PGDOWN :
        {
            return "PGDOWN";
        }
        case KEYCODE_INSERT :
        {
            return "INSERT";
        }
        case KEYCODE_DELETE :
        {
            return "DELETE";
        }
        case KEYCODE_LWIN :
        {
            return "LWIN";
        }
        case KEYCODE_RWIN :
        {
            return "RWIN";
        }
        case KEYCODE_APPS :
        {
            return "APPS";
        }
        case KEYCODE_POWER :
        {
            return "POWER";
        }
        case KEYCODE_SLEEP :
        {
            return "SLEEP";
        }
        case KEYCODE_WAKE :
        {
            return "WAKE";
        }
        case KEYCODE_WEBSEARCH :
        {
            return "WEBSEARCH";
        }
        case KEYCODE_WEBFAVORITES :
        {
            return "WEBFAVORITES";
        }
        case KEYCODE_WEBREFRESH :
        {
            return "WEBREFRESH";
        }
        case KEYCODE_WEBSTOP :
        {
            return "WEBSTOP";
        }
        case KEYCODE_WEBFORWARD :
        {
            return "WEBFORWARD";
        }
        case KEYCODE_WEBBACK :
        {
            return "WEBBACK";
        }
        case KEYCODE_MYCOMPUTER :
        {
            return "MYCOMPUTER";
        }
        case KEYCODE_MAIL :
        {
            return "MAIL";
        }
        case KEYCODE_MEDIASELECT :
        {
            return "MEDIASELECT";
        }                
        default:
        {
            GUCEF_UNREACHABLE;
            return "";
        }
    }
}

/*-------------------------------------------------------------------------*/

KeyCode
StringToKeyCode( const CString& keyCodeStr )
{GUCEF_TRACE;

    if ( keyCodeStr == "UNASSIGNED" )
    {
        return KEYCODE_UNASSIGNED;
    }
    else
    if ( keyCodeStr == "ESCAPE" )
    {
        return KEYCODE_ESCAPE;
    }
    else
    if ( keyCodeStr == "1" )
    {
        return KEYCODE_1;
    }
    else
    if ( keyCodeStr == "2" )
    {
        return KEYCODE_2;
    }
    else
    if ( keyCodeStr == "3" )
    {
        return KEYCODE_3;
    }
    else
    if ( keyCodeStr == "4" )
    {
        return KEYCODE_4;
    }
    else
    if ( keyCodeStr == "5" )
    {
        return KEYCODE_5;
    }
    else
    if ( keyCodeStr == "6" )
    {
        return KEYCODE_6;
    }                
    else
    if ( keyCodeStr == "7" )
    {
        return KEYCODE_7;
    }
    else
    if ( keyCodeStr == "8" )
    {
        return KEYCODE_8;
    }
    else
    if ( keyCodeStr == "9" )
    {
        return KEYCODE_9;
    }
    else
    if ( keyCodeStr == "0" )
    {
        return KEYCODE_0;
    }
    else
    if ( keyCodeStr == "MINUS" )
    {
        return KEYCODE_MINUS;
    }    
    else
    if ( keyCodeStr == "EQUALS" )
    {
        return KEYCODE_EQUALS;
    }
    else
    if ( keyCodeStr == "BACK" )
    {
        return KEYCODE_BACK;
    }
    else
    if ( keyCodeStr == "TAB" )
    {
        return KEYCODE_TAB;
    }    
    else
    if ( keyCodeStr == "Q" )
    {
        return KEYCODE_Q;
    }
    else
    if ( keyCodeStr == "W" )
    {
        return KEYCODE_W;
    }
    else
    if ( keyCodeStr == "E" )
    {
        return KEYCODE_E;
    }
    else
    if ( keyCodeStr == "R" )
    {
        return KEYCODE_R;
    }
    else
    if ( keyCodeStr == "T" )
    {
        return KEYCODE_T;
    }                
    else
    if ( keyCodeStr == "Y" )
    {
        return KEYCODE_Y;
    }
    else
    if ( keyCodeStr == "U" )
    {
        return KEYCODE_U;
    }
    else
    if ( keyCodeStr == "I" )
    {
        return KEYCODE_I;
    }
    else
    if ( keyCodeStr == "O" )
    {
        return KEYCODE_O;
    }
    else
    if ( keyCodeStr == "P" )
    {
        return KEYCODE_P;
    }
    else
    if ( keyCodeStr == "LBRACKET" )
    {
        return KEYCODE_LBRACKET;
    }            
    else
    if ( keyCodeStr == "RBRACKET" )
    {
        return KEYCODE_RBRACKET;
    }
    else
    if ( keyCodeStr == "RETURN" )
    {
        return KEYCODE_RETURN;
    }
    else
    if ( keyCodeStr == "LCONTROL" )
    {
        return KEYCODE_LCONTROL;
    }
    else
    if ( keyCodeStr == "A" )
    {
        return KEYCODE_A;
    }
    else
    if ( keyCodeStr == "S" )
    {
        return KEYCODE_S;
    }
    else
    if ( keyCodeStr == "D" )
    {
        return KEYCODE_D;
    }
    else
    if ( keyCodeStr == "F" )
    {
        return KEYCODE_F;
    }            
    else
    if ( keyCodeStr == "G" )
    {
        return KEYCODE_G;
    }
    else
    if ( keyCodeStr == "H" )
    {
        return KEYCODE_H;
    }
    else
    if ( keyCodeStr == "J" )
    {
        return KEYCODE_J;
    }
    else
    if ( keyCodeStr == "K" )
    {
        return KEYCODE_K;
    }
    else
    if ( keyCodeStr == "L" )
    {
        return KEYCODE_L;
    }
    else
    if ( keyCodeStr == "SEMICOLON" )
    {
        return KEYCODE_SEMICOLON;
    }
    else
    if ( keyCodeStr == "APOSTROPHE" )
    {
        return KEYCODE_APOSTROPHE;
    }
    else
    if ( keyCodeStr == "GRAVE" )
    {
        return KEYCODE_GRAVE;
    }
    else
    if ( keyCodeStr == "LSHIFT" )
    {
        return KEYCODE_LSHIFT;
    }
    else
    if ( keyCodeStr == "BACKSLASH" )
    {
        return KEYCODE_BACKSLASH;
    }
    else
    if ( keyCodeStr == "Z" )
    {
        return KEYCODE_Z;
    }
    else
    if ( keyCodeStr == "X" )
    {
        return KEYCODE_X;
    }
    else
    if ( keyCodeStr == "C" )
    {
        return KEYCODE_C;
    }            
    else
    if ( keyCodeStr == "V" )
    {
        return KEYCODE_V;
    }
    else
    if ( keyCodeStr == "B" )
    {
        return KEYCODE_B;
    }    
    else
    if ( keyCodeStr == "N" )
    {
        return KEYCODE_N;
    }
    else
    if ( keyCodeStr == "M" )
    {
        return KEYCODE_M;
    }
    else
    if ( keyCodeStr == "COMMA" )
    {
        return KEYCODE_COMMA;
    }    
    else
    if ( keyCodeStr == "PERIOD" )
    {
        return KEYCODE_PERIOD;
    }
    else
    if ( keyCodeStr == "SLASH" )
    {
        return KEYCODE_SLASH;
    }
    else
    if ( keyCodeStr == "RSHIFT" )
    {
        return KEYCODE_RSHIFT;
    }
    else
    if ( keyCodeStr == "MULTIPLY" )
    {
        return KEYCODE_MULTIPLY;
    }
    else
    if ( keyCodeStr == "LMENU" )
    {
        return KEYCODE_LMENU;
    }
    else
    if ( keyCodeStr == "SPACE" )
    {
        return KEYCODE_SPACE;
    }

    // We have to break the if nesting here because compilers cannot 
    // handle so many nested if-else statements

    if ( keyCodeStr == "CAPITAL" )
    {
        return KEYCODE_CAPITAL;
    }
    else
    if ( keyCodeStr == "F1" )
    {
        return KEYCODE_F1;
    }
    else
    if ( keyCodeStr == "F2" )
    {
        return KEYCODE_F2;
    }
    else
    if ( keyCodeStr == "F3" )
    {
        return KEYCODE_F3;
    }
    else
    if ( keyCodeStr == "F4" )
    {
        return KEYCODE_F4;
    }
    else
    if ( keyCodeStr == "F5" )
    {
        return KEYCODE_F5;
    }
    else
    if ( keyCodeStr == "F6" )
    {
        return KEYCODE_F6;
    }
    else
    if ( keyCodeStr == "F7" )
    {
        return KEYCODE_F7;
    }                        
    else
    if ( keyCodeStr == "F8" )
    {
        return KEYCODE_F8;
    }
    else
    if ( keyCodeStr == "F9" )
    {
        return KEYCODE_F9;
    }
    else
    if ( keyCodeStr == "F10" )
    {
        return KEYCODE_F10;
    }
    else
    if ( keyCodeStr == "NUMLOCK" )
    {
        return KEYCODE_NUMLOCK;
    }
    else
    if ( keyCodeStr == "SCROLL" )
    {
        return KEYCODE_SCROLL;
    }
    else
    if ( keyCodeStr == "NUMPAD7" )
    {
        return KEYCODE_NUMPAD7;
    }    
    else
    if ( keyCodeStr == "NUMPAD8" )
    {
        return KEYCODE_NUMPAD8;
    }
    else
    if ( keyCodeStr == "NUMPAD9" )
    {
        return KEYCODE_NUMPAD9;
    }            
    else
    if ( keyCodeStr == "SUBTRACT" )
    {
        return KEYCODE_SUBTRACT;
    }
    else
    if ( keyCodeStr == "NUMPAD4" )
    {
        return KEYCODE_NUMPAD4;
    }
    else
    if ( keyCodeStr == "NUMPAD5" )
    {
        return KEYCODE_NUMPAD5;
    }
    else
    if ( keyCodeStr == "NUMPAD6" )
    {
        return KEYCODE_NUMPAD6;
    }
    else
    if ( keyCodeStr == "ADD" )
    {
        return KEYCODE_ADD;
    }
    else
    if ( keyCodeStr == "NUMPAD1" )
    {
        return KEYCODE_NUMPAD1;
    }            
    else
    if ( keyCodeStr == "NUMPAD2" )
    {
        return KEYCODE_NUMPAD2;
    }
    else
    if ( keyCodeStr == "NUMPAD3" )
    {
        return KEYCODE_NUMPAD3;
    }
    else
    if ( keyCodeStr == "NUMPAD0" )
    {
        return KEYCODE_NUMPAD0;
    }
    else
    if ( keyCodeStr == "DECIMAL" )
    {
        return KEYCODE_DECIMAL;
    }
    else
    if ( keyCodeStr == "LEFTWINKEY" )
    {
        return KEYCODE_LEFTWINKEY;
    }
    else
    if ( keyCodeStr == "RIGHTWINKEY" )
    {
        return KEYCODE_RIGHTWINKEY;
    }    
    else    
    if ( keyCodeStr == "OEM_102" )
    {
        return KEYCODE_OEM_102;
    }
    else
    if ( keyCodeStr == "F11" )
    {
        return KEYCODE_F11;
    }
    else
    if ( keyCodeStr == "F12" )
    {
        return KEYCODE_F12;
    }
    else
    if ( keyCodeStr == "F12" )
    {
        return KEYCODE_F12;
    }
    else
    if ( keyCodeStr == "F13" )
    {
        return KEYCODE_F13;
    }
    else
    if ( keyCodeStr == "F14" )
    {
        return KEYCODE_F14;
    }
    else
    if ( keyCodeStr == "F15" )
    {
        return KEYCODE_F15;
    }
    else
    if ( keyCodeStr == "KANA" )
    {
        return KEYCODE_KANA;
    }
    else
    if ( keyCodeStr == "ABNT_C1" )
    {
        return KEYCODE_ABNT_C1;
    }
    else
    if ( keyCodeStr == "CONVERT" )
    {
        return KEYCODE_CONVERT;
    }
    else
    if ( keyCodeStr == "NOCONVERT" )
    {
        return KEYCODE_NOCONVERT;
    }
    else
    if ( keyCodeStr == "YEN" )
    {
        return KEYCODE_YEN;
    }
    else
    if ( keyCodeStr == "ABNT_C2" )
    {
        return KEYCODE_ABNT_C2;
    }
    else
    if ( keyCodeStr == "NUMPADEQUALS" )
    {
        return KEYCODE_NUMPADEQUALS;
    }
    else
    if ( keyCodeStr == "PREVTRACK" )
    {
        return KEYCODE_PREVTRACK;
    }
    else
    if ( keyCodeStr == "AT" )
    {
        return KEYCODE_AT;
    }
    else
    if ( keyCodeStr == "COLON" )
    {
        return KEYCODE_COLON;
    }
    else
    if ( keyCodeStr == "UNDERLINE" )
    {
        return KEYCODE_UNDERLINE;
    }
    else
    if ( keyCodeStr == "KANJI" )
    {
        return KEYCODE_KANJI;
    }
    else
    if ( keyCodeStr == "STOP" )
    {
        return KEYCODE_STOP;
    }
    else
    if ( keyCodeStr == "AX" )
    {
        return KEYCODE_AX;
    }
    else
    if ( keyCodeStr == "UNLABELED" )
    {
        return KEYCODE_UNLABELED;
    }
    else
    if ( keyCodeStr == "NEXTTRACK" )
    {
        return KEYCODE_NEXTTRACK;
    }
    else
    if ( keyCodeStr == "NUMPADENTER" )
    {
        return KEYCODE_NUMPADENTER;
    }
    else
    if ( keyCodeStr == "RCONTROL" )
    {
        return KEYCODE_RCONTROL;
    }
    else
    if ( keyCodeStr == "MUTE" )
    {
        return KEYCODE_MUTE;
    }
    else
    if ( keyCodeStr == "CALCULATOR" )
    {
        return KEYCODE_CALCULATOR;
    }
    else
    if ( keyCodeStr == "PLAYPAUSE" )
    {
        return KEYCODE_PLAYPAUSE;
    }
    else
    if ( keyCodeStr == "MEDIASTOP" )
    {
        return KEYCODE_MEDIASTOP;
    }
    else
    if ( keyCodeStr == "VOLUMEDOWN" )
    {
        return KEYCODE_VOLUMEDOWN;
    }
    else
    if ( keyCodeStr == "VOLUMEUP" )
    {
        return KEYCODE_VOLUMEUP;
    }
    else
    if ( keyCodeStr == "WEBHOME" )
    {
        return KEYCODE_WEBHOME;
    }
    else
    if ( keyCodeStr == "NUMPADCOMMA" )
    {
        return KEYCODE_NUMPADCOMMA;
    }
    else
    if ( keyCodeStr == "DIVIDE" )
    {
        return KEYCODE_DIVIDE;
    }
    else
    if ( keyCodeStr == "SYSRQ" )
    {
        return KEYCODE_SYSRQ;
    }
    else
    if ( keyCodeStr == "RMENU" )
    {
        return KEYCODE_RMENU;
    }
    else
    if ( keyCodeStr == "PAUSE" )
    {
        return KEYCODE_PAUSE;
    }
    else
    if ( keyCodeStr == "HOME" )
    {
        return KEYCODE_HOME;
    }
    else
    if ( keyCodeStr == "UP" )
    {
        return KEYCODE_UP;
    }
    else
    if ( keyCodeStr == "PGUP" )
    {
        return KEYCODE_PGUP;
    }
    else
    if ( keyCodeStr == "LEFT" )
    {
        return KEYCODE_LEFT;
    }
    else
    if ( keyCodeStr == "RIGHT" )
    {
        return KEYCODE_RIGHT;
    }
    else
    if ( keyCodeStr == "END" )
    {
        return KEYCODE_END;
    }
    else
    if ( keyCodeStr == "DOWN" )
    {
        return KEYCODE_DOWN;
    }
    else
    if ( keyCodeStr == "PGDOWN" )
    {
        return KEYCODE_PGDOWN;
    }    
    else
    if ( keyCodeStr == "INSERT" )
    {
        return KEYCODE_INSERT;
    }
    else
    if ( keyCodeStr == "DELETE" )
    {
        return KEYCODE_DELETE;
    }
    else
    if ( keyCodeStr == "LWIN" )
    {
        return KEYCODE_LWIN;
    }
    else
    if ( keyCodeStr == "RWIN" )
    {
        return KEYCODE_RWIN;
    }
    else
    if ( keyCodeStr == "APPS" )
    {
        return KEYCODE_APPS;
    }
    else
    if ( keyCodeStr == "POWER" )
    {
        return KEYCODE_POWER;
    }
    else
    if ( keyCodeStr == "SLEEP" )
    {
        return KEYCODE_SLEEP;
    }
    else
    if ( keyCodeStr == "WAKE" )
    {
        return KEYCODE_WAKE;
    }
    else
    if ( keyCodeStr == "WEBSEARCH" )
    {
        return KEYCODE_WEBSEARCH;
    }
    else
    if ( keyCodeStr == "WEBFAVORITES" )
    {
        return KEYCODE_WEBFAVORITES;
    }
    else
    if ( keyCodeStr == "WEBREFRESH" )
    {
        return KEYCODE_WEBREFRESH;
    }
    else
    if ( keyCodeStr == "WEBSTOP" )
    {
        return KEYCODE_WEBSTOP;
    }
    else
    if ( keyCodeStr == "WEBFORWARD" )
    {
        return KEYCODE_WEBFORWARD;
    }
    else
    if ( keyCodeStr == "WEBBACK" )
    {
        return KEYCODE_WEBBACK;
    }
    else
    if ( keyCodeStr == "MYCOMPUTER" )
    {
        return KEYCODE_MYCOMPUTER;
    }
    else
    if ( keyCodeStr == "MAIL" )
    {
        return KEYCODE_MAIL;
    }
    else
    if ( keyCodeStr == "MEDIASELECT" )
    {
        return KEYCODE_MEDIASELECT;
    }
        	        
    return KEYCODE_UNASSIGNED;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
