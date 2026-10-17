/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *
 *  Copyright (C) 1998 - 2026.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefGUI_CRoutedEventData.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CRoutedEventData::CRoutedEventData( void )
    : MT::CICloneable()
    , source( GUCEF_NULL )
    , phase( EVENTPHASE_TARGET )
    , handled( false )
{
}

CRoutedEventData::~CRoutedEventData()
{
}

MT::CICloneable*
CRoutedEventData::Clone( void ) const
{
    return GUCEF_NEW CRoutedEventData( *this );
}

/*-------------------------------------------------------------------------*/

CPointerEventData::CPointerEventData( void )
    : CRoutedEventData()
    , x( 0 )
    , y( 0 )
    , localX( 0 )
    , localY( 0 )
    , button( MOUSEBUTTON_NONE )
    , buttonStates( 0 )
    , modifiers( 0 )
    , clickCount( 0 )
{
}

CPointerEventData::~CPointerEventData()
{
}

MT::CICloneable*
CPointerEventData::Clone( void ) const
{
    return GUCEF_NEW CPointerEventData( *this );
}

/*-------------------------------------------------------------------------*/

CWheelEventData::CWheelEventData( void )
    : CRoutedEventData()
    , x( 0 )
    , y( 0 )
    , localX( 0 )
    , localY( 0 )
    , delta( 0 )
    , modifiers( 0 )
{
}

CWheelEventData::~CWheelEventData()
{
}

MT::CICloneable*
CWheelEventData::Clone( void ) const
{
    return GUCEF_NEW CWheelEventData( *this );
}

/*-------------------------------------------------------------------------*/

CKeyEventData::CKeyEventData( void )
    : CRoutedEventData()
    , keyCode( INPUT::KEYCODE_UNASSIGNED )
    , modifiers( 0 )
{
}

CKeyEventData::~CKeyEventData()
{
}

MT::CICloneable*
CKeyEventData::Clone( void ) const
{
    return GUCEF_NEW CKeyEventData( *this );
}

/*-------------------------------------------------------------------------*/

CTextEventData::CTextEventData( void )
    : CRoutedEventData()
    , text()
{
}

CTextEventData::~CTextEventData()
{
}

MT::CICloneable*
CTextEventData::Clone( void ) const
{
    return GUCEF_NEW CTextEventData( *this );
}

/*-------------------------------------------------------------------------*/

CNavigationEventData::CNavigationEventData( void )
    : CRoutedEventData()
    , direction( NAVIGATION_NEXT )
{
}

CNavigationEventData::~CNavigationEventData()
{
}

MT::CICloneable*
CNavigationEventData::Clone( void ) const
{
    return GUCEF_NEW CNavigationEventData( *this );
}

/*-------------------------------------------------------------------------*/

CFocusEventData::CFocusEventData( void )
    : CRoutedEventData()
    , other( GUCEF_NULL )
{
}

CFocusEventData::~CFocusEventData()
{
}

MT::CICloneable*
CFocusEventData::Clone( void ) const
{
    return GUCEF_NEW CFocusEventData( *this );
}

/*-------------------------------------------------------------------------*/

CValueChangedEventData::CValueChangedEventData( void )
    : CRoutedEventData()
    , propertyName()
    , oldValue()
    , newValue()
{
}

CValueChangedEventData::~CValueChangedEventData()
{
}

MT::CICloneable*
CValueChangedEventData::Clone( void ) const
{
    return GUCEF_NEW CValueChangedEventData( *this );
}

/*-------------------------------------------------------------------------*/

CItemEventData::CItemEventData( void )
    : CRoutedEventData()
    , index( -1 )
    , item()
{
}

CItemEventData::~CItemEventData()
{
}

MT::CICloneable*
CItemEventData::Clone( void ) const
{
    return GUCEF_NEW CItemEventData( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
