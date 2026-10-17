/*
 *  gucefGUI_TestApp: Application for testing gucefGUI
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_INPUT_H
#include "gucefINPUT.h"
#define GUCEF_INPUT_H
#endif /* GUCEF_INPUT_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "gucefGUI_TestCommon.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

namespace {

/*-------------------------------------------------------------------------*/

/** An input driver without hardware: the tests feed it input the way a window's input driver does */
class CScriptedInputDriver : public INPUT::CInputDriver
{
    public:

    virtual const CORE::TVersion* GetVersion( void ) GUCEF_VIRTUAL_OVERRIDE
    {
        static CORE::TVersion version = { 1, 0, 0, 0 };
        return &version;
    }

    virtual CORE::CString GetName( void ) const GUCEF_VIRTUAL_OVERRIDE { return "ScriptedInput"; }

    void Mouse( const Int32 deviceId ) { AddMouse( deviceId ); }
    void NoMouse( const Int32 deviceId ) { RemoveMouse( deviceId ); }
    void Keyboard( const Int32 deviceId ) { AddKeyboard( deviceId ); }
    void NoKeyboard( const Int32 deviceId ) { RemoveKeyboard( deviceId ); }

    void Move( const Int32 contextId, const Int32 deviceId, const Int32 x, const Int32 y ) { InjectMouseMove( contextId, deviceId, x, y, 0, 0 ); }
    void Button( const Int32 contextId, const Int32 deviceId, const UInt32 button, const bool pressed ) { InjectMouseButtonChange( contextId, deviceId, button, pressed ); }
    void Wheel( const Int32 contextId, const Int32 deviceId, const Int32 value, const Int32 delta ) { InjectMouseVariableChange( contextId, deviceId, INPUT::CMouse::MOUSEVARIABLE_WHEEL, value, delta ); }
    void Key( const Int32 contextId, const Int32 deviceId, const INPUT::KeyCode key, const bool pressed ) { InjectKeyboardKeyChange( contextId, deviceId, key, pressed ); }
    void Text( const Int32 contextId, const Int32 deviceId, const CORE::CString& text ) { InjectKeyboardText( contextId, deviceId, text ); }

    void Click( const Int32 contextId, const Int32 deviceId, const Int32 x, const Int32 y )
    {
        Move( contextId, deviceId, x, y );
        Button( contextId, deviceId, 0, true );
        Button( contextId, deviceId, 0, false );
    }

    protected:

    virtual bool OnUpdate( const UInt64 tickcount, const Float64 updateDeltaInMilliSecs, INPUT::CInputContext* context ) GUCEF_VIRTUAL_OVERRIDE { return true; }

    virtual INPUT::CInputContext* CreateContext( const CORE::CValueList& params ) GUCEF_VIRTUAL_OVERRIDE { return GUCEF_NEW INPUT::CInputContext( *this, params ); }

    virtual void DeleteContext( INPUT::CInputContext* context ) GUCEF_VIRTUAL_OVERRIDE { GUCEF_DELETE context; }
};

/*-------------------------------------------------------------------------*/

/** Counts the target phase of one routed event of one widget */
class CTargetCount : public CORE::CObserver
{
    public:

    CORE::CEvent eventId;
    UInt32 count;
    Int32 lastWheelDelta;
    GUI::TMouseButton lastButton;

    CTargetCount( CORE::CNotifier* notifier, const CORE::CEvent& id )
        : CORE::CObserver(), eventId( id ), count( 0 ), lastWheelDelta( 0 ), lastButton( GUI::MOUSEBUTTON_NONE )
    {
        SubscribeTo( notifier, id );
    }

    virtual ~CTargetCount() { UnsubscribeAllFromObserver(); }

    virtual void OnNotify( CORE::CNotifier* notifier, const CORE::CEvent& id, CORE::CICloneable* eventData ) GUCEF_VIRTUAL_OVERRIDE
    {
        if ( id != eventId )
            return;
        GUI::CRoutedEventData* routed = dynamic_cast< GUI::CRoutedEventData* >( eventData );
        if ( GUCEF_NULL != routed && routed->source != notifier )
            return;
        ++count;
        GUI::CWheelEventData* wheel = dynamic_cast< GUI::CWheelEventData* >( eventData );
        if ( GUCEF_NULL != wheel )
            lastWheelDelta = wheel->delta;
        GUI::CPointerEventData* pointer = dynamic_cast< GUI::CPointerEventData* >( eventData );
        if ( GUCEF_NULL != pointer )
            lastButton = pointer->button;
    }
};

/*-------------------------------------------------------------------------*/

const Int32 TestMouseId = 9101;
const Int32 TestKeyboardId = 9102;

/*-------------------------------------------------------------------------*/

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      TESTS                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformGuiInputTests( void )
{GUCEF_TRACE;

    GUCEF_TESTFW_SUITE_SCOPE( "gucefGUI.Input" );

    GUCEF_TESTFW_TESTCASE( "Input: The adapter maps the gucefINPUT button indices to GUI buttons" )
    try
    {
        ASSERT_TRUE( GUI::MOUSEBUTTON_LEFT == GUI::CGUIInputAdapter::MapMouseButton( 0 ) );
        ASSERT_TRUE( GUI::MOUSEBUTTON_RIGHT == GUI::CGUIInputAdapter::MapMouseButton( 1 ) );
        ASSERT_TRUE( GUI::MOUSEBUTTON_MIDDLE == GUI::CGUIInputAdapter::MapMouseButton( 2 ) );
        ASSERT_TRUE( GUI::MOUSEBUTTON_X1 == GUI::CGUIInputAdapter::MapMouseButton( 3 ) );
        ASSERT_TRUE( GUI::MOUSEBUTTON_X2 == GUI::CGUIInputAdapter::MapMouseButton( 4 ) );
        ASSERT_TRUE( GUI::MOUSEBUTTON_NONE == GUI::CGUIInputAdapter::MapMouseButton( 9 ) );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Input: Mouse and keyboard input from a driver reaches the widgets through the adapter" )
    try
    {
        GUI::TGuiContextPtr contextPtr = CreateNullContext();
        GUI::CGUIContext* context = static_cast< GUI::CGUIContext* >( contextPtr.GetPointerAlways() );
        GUI::CWidget* root = context->CreateWidget( "Panel", "Screen", GUCEF_NULL );
        root->SetProperty( "Size", "1,0 1,0" );
        GUI::CEditbox* edit = static_cast< GUI::CEditbox* >( context->CreateWidget( "Editbox", "Name", root ) );
        edit->SetProperty( "Position", "0,10 0,10" );
        edit->SetProperty( "Size", "0,200 0,20" );
        GUI::CWidget* button = context->CreateWidget( "Button", "Ok", root );
        button->SetProperty( "Position", "0,10 0,100" );
        button->SetProperty( "Size", "0,80 0,30" );
        ASSERT_TRUE( context->ShowRoot( root, false ) );

        CTargetCount clicked( button, GUI::CWidget::MouseClickEvent );
        CTargetCount wheeled( edit, GUI::CWidget::MouseWheelEvent );

        CScriptedInputDriver driver;
        driver.Keyboard( TestKeyboardId );
        GUI::CGUIInputAdapter adapter( *context );
        adapter.Attach();
        ASSERT_TRUE( adapter.IsAttached() );

        // a mouse attached after the adapter is picked up too
        driver.Mouse( TestMouseId );

        driver.Click( 1, TestMouseId, 20, 15 );
        ASSERT_TRUE( 20 == context->GetPointerX() && 15 == context->GetPointerY() );
        ASSERT_TRUE( edit->HasFocus() );

        driver.Text( 1, TestKeyboardId, "Ab\xC3\xA9" );
        ASSERT_TRUE( "Ab\xC3\xA9" == edit->GetText() );
        driver.Key( 1, TestKeyboardId, INPUT::KEYCODE_BACK, true );
        driver.Key( 1, TestKeyboardId, INPUT::KEYCODE_BACK, false );
        ASSERT_TRUE( "Ab" == edit->GetText() );

        driver.Wheel( 1, TestMouseId, -1, -1 );
        ASSERT_TRUE( 1 == wheeled.count && -1 == wheeled.lastWheelDelta );

        driver.Click( 1, TestMouseId, 30, 110 );
        ASSERT_TRUE( 1 == clicked.count && GUI::MOUSEBUTTON_LEFT == clicked.lastButton );

        // gucefINPUT button 1 is the right button
        driver.Button( 1, TestMouseId, 1, true );
        driver.Button( 1, TestMouseId, 1, false );
        ASSERT_TRUE( 2 == clicked.count && GUI::MOUSEBUTTON_RIGHT == clicked.lastButton );

        // after Detach nothing arrives any more
        adapter.Detach();
        driver.Text( 1, TestKeyboardId, "x" );
        ASSERT_TRUE( "Ab" == edit->GetText() );

        driver.NoMouse( TestMouseId );
        driver.NoKeyboard( TestKeyboardId );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Input: An adapter for one input context ignores the input of other windows" )
    try
    {
        GUI::TGuiContextPtr contextPtr = CreateNullContext();
        GUI::CGUIContext* context = static_cast< GUI::CGUIContext* >( contextPtr.GetPointerAlways() );
        GUI::CWidget* root = context->CreateWidget( "Panel", "Screen", GUCEF_NULL );
        root->SetProperty( "Size", "1,0 1,0" );
        GUI::CEditbox* edit = static_cast< GUI::CEditbox* >( context->CreateWidget( "Editbox", "Name", root ) );
        edit->SetProperty( "Size", "0,200 0,20" );
        ASSERT_TRUE( context->ShowRoot( root, false ) );
        context->SetFocus( edit );

        CScriptedInputDriver driver;
        driver.Mouse( TestMouseId );
        driver.Keyboard( TestKeyboardId );
        GUI::CGUIInputAdapter adapter( *context, 7 );
        adapter.Attach();

        driver.Text( 3, TestKeyboardId, "other" );
        driver.Move( 3, TestMouseId, 300, 300 );
        ASSERT_TRUE( edit->GetText().IsNULLOrEmpty() );
        ASSERT_FALSE( 300 == context->GetPointerX() );

        driver.Text( 7, TestKeyboardId, "ours" );
        driver.Move( 7, TestMouseId, 300, 300 );
        ASSERT_TRUE( "ours" == edit->GetText() );
        ASSERT_TRUE( 300 == context->GetPointerX() && 300 == context->GetPointerY() );

        adapter.Detach();
        driver.NoMouse( TestMouseId );
        driver.NoKeyboard( TestKeyboardId );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/
