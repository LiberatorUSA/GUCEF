/*
 *  gucefGUI_TestApp: Application for testing gucefGUI
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

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "gucefGUI_TestCommon.h"

#ifndef GUCEF_GUIDRIVERRMLUI_CRMLUICONTEXT_H
#include "guidriverRmlUI_CRmlUIContext.h"
#define GUCEF_GUIDRIVERRMLUI_CRMLUICONTEXT_H
#endif /* GUCEF_GUIDRIVERRMLUI_CRMLUICONTEXT_H ? */

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

GUI::CColour
PixelAt( const IMAGE::TPixelMapPtr& pixels, Int32 x, Int32 y )
{
    const UInt8* data = static_cast< const UInt8* >( pixels->GetDataPtr() ) + ( (size_t) y * pixels->GetWidthInPixels() + x ) * 4;
    return GUI::CColour( data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ] );
}

/** Same colour, give or take a little for the rounding of blending on a GPU */
bool
Near( const GUI::CColour& a, const GUI::CColour& b, Int32 tolerance = 2 )
{
    return abs( (Int32) a.r - (Int32) b.r ) <= tolerance && abs( (Int32) a.g - (Int32) b.g ) <= tolerance &&
           abs( (Int32) a.b - (Int32) b.b ) <= tolerance;
}

/** Counts the pixels of a colour in a rectangle */
UInt32
CountColour( const IMAGE::TPixelMapPtr& pixels, const GUI::CRect& rect, const GUI::CColour& colour )
{
    UInt32 count = 0;
    for ( Int32 y=rect.top; y<rect.Bottom(); ++y )
        for ( Int32 x=rect.left; x<rect.Right(); ++x )
            if ( Near( PixelAt( pixels, x, y ), colour ) )
                ++count;
    return count;
}

GUI::CWidget*
Screen( GUI::CGUIContext* context )
{
    GUI::CWidget* root = context->CreateWidget( "Panel", "Screen", GUCEF_NULL );
    root->SetProperty( "Size", "1,0 1,0" );
    context->ShowRoot( root, false );
    return root;
}

bool
Capture( GUI::CGUIContext* context, IMAGE::TPixelMapPtr& pixels )
{
    context->RenderFrame( 16 );
    return context->CaptureFrame( pixels ) && !pixels.IsNULL() && 640 == pixels->GetWidthInPixels() && 480 == pixels->GetHeightInPixels();
}

const GUI::CColour Black( 0, 0, 0, 255 );
const GUI::CColour Red( 255, 0, 0, 255 );
const GUI::CColour Green( 0, 255, 0, 255 );
const GUI::CColour Blue( 0, 0, 255, 255 );
const GUI::CColour White( 255, 255, 255, 255 );
const GUI::CColour Yellow( 255, 255, 0, 255 );

/** Panels, borders, clipping, opacity, scaled and tinted pictures, text, lines and points in one picture */
void
BuildMixedScene( GUI::CGUIContext* context )
{
    context->RegisterFont( "Block", MakeTestFont( 6, 9 ) );
    IMAGE::TPixelMapPtr checker = MakeSolidImage( 4, 4, 255, 255, 255, 255 );
    UInt8* data = static_cast< UInt8* >( checker->GetDataPtr() );
    for ( UInt32 i=0; i<16; ++i )
    {
        if ( 0 == ( ( i % 4 ) + ( i / 4 ) ) % 2 )
        {
            data[ i*4 ] = 200; data[ i*4+1 ] = 30; data[ i*4+2 ] = 60; data[ i*4+3 ] = ( i % 3 ) ? 255 : 128;
        }
    }
    context->RegisterImage( "Checker", checker );

    GUI::CWidget* screen = Screen( context );
    screen->SetColourProperty( "BackgroundColour", GUI::CColour( 20, 40, 60, 255 ) );

    GUI::CWidget* panel = context->CreateWidget( "Panel", "Panel", screen );
    panel->SetProperty( "Position", "0,13 0,17" );
    panel->SetProperty( "Size", "0,201 0,143" );
    panel->SetColourProperty( "BackgroundColour", GUI::CColour( 90, 120, 30, 200 ) );
    panel->SetColourProperty( "BorderColour", White );
    panel->SetBoolProperty( "Clip", true );
    GUI::CWidget* inner = context->CreateWidget( "Panel", "Inner", panel );
    inner->SetProperty( "Position", "0,150 0,100" );
    inner->SetProperty( "Size", "0,90 0,70" );
    inner->SetColourProperty( "BackgroundColour", GUI::CColour( 250, 200, 10, 255 ) );
    inner->SetFloatProperty( "Opacity", 0.6f );

    GUI::CWidget* frame = context->CreateWidget( "ImageFrame", "Checker", screen );
    frame->SetProperty( "Position", "0,240 0,20" );
    frame->SetProperty( "Size", "0,48 0,36" );
    frame->SetProperty( "Image", "Checker" );
    frame->SetProperty( "ScaleMode", "Stretch" );

    GUI::CWidget* label = context->CreateWidget( "Label", "Label", screen );
    label->SetProperty( "Position", "0,300 0,40" );
    label->SetProperty( "Size", "0,120 0,40" );
    label->SetProperty( "Font", "Block" );
    label->SetProperty( "Text", "Hello there wide world" );
    label->SetBoolProperty( "WordWrap", true );
    label->SetColourProperty( "TextColour", GUI::CColour( 120, 220, 255, 230 ) );

    GUI::CDrawSurface* surface = static_cast< GUI::CDrawSurface* >( context->CreateWidget( "DrawSurface", "Surface", screen ) );
    surface->SetProperty( "Position", "0,20 0,200" );
    surface->SetProperty( "Size", "0,300 0,200" );
    surface->FillRect( GUI::CRect( 5, 5, 40, 30 ), GUI::CColour( 255, 0, 255, 160 ) );
    surface->StrokeRect( GUI::CRect( 50, 5, 40, 30 ), Green, 3 );
    surface->DrawLine( 0, 199, 299, 50, Yellow );
    surface->DrawLine( 10, 60, 120, 190, Red, 3 );
    surface->DrawLine( 150, 120, 150, 180, White );
    GUI::CDrawSurface::TPointVector points;
    for ( Int32 i=0; i<30; ++i )
    {
        GUI::CDrawSurface::SPoint point;
        point.x = 200 + i * 3;
        point.y = 20 + ( i * 7 ) % 40;
        points.push_back( point );
    }
    surface->DrawPoints( points, White );
    surface->PushClip( GUI::CRect( 100, 60, 50, 50 ) );
    surface->DrawImage( "Checker", GUI::CRect( 90, 50, 70, 70 ), GUI::CRect( 1, 1, 3, 3 ), GUI::CColour( 255, 255, 128, 200 ) );
    surface->PopClip();
    surface->DrawString( "Block", "Text!", 170, 150, GUI::CColour( 255, 160, 0, 255 ) );
    surface->Commit();
}

/*-------------------------------------------------------------------------*/

/** The same drawing tests for every driver that can capture what it draws */
void
RunRenderTests( const char* driverName, GUI::TGuiContextPtr ( *createContext )( void ) )
{
    CORE::CString prefix = CORE::CString( "Render (" ) + driverName + "): ";

    GUCEF_TESTFW_TESTCASE( ( prefix + "Panels fill and draw borders, children are cut off where Clip is set" ).C_String() )
    try
    {
        GUI::TGuiContextPtr contextPtr = createContext();
        GUI::CGUIContext* context = dynamic_cast< GUI::CGUIContext* >( contextPtr.GetPointerAlways() );
        ASSERT_TRUE( GUCEF_NULL != context );
        GUI::CWidget* screen = Screen( context );
        GUI::CWidget* panel = context->CreateWidget( "Panel", "Panel", screen );
        panel->SetProperty( "Position", "0,10 0,10" );
        panel->SetProperty( "Size", "0,100 0,50" );
        panel->SetColourProperty( "BackgroundColour", Blue );
        panel->SetColourProperty( "BorderColour", White );
        GUI::CWidget* child = context->CreateWidget( "Panel", "Child", panel );
        child->SetProperty( "Position", "0,80 0,20" );
        child->SetProperty( "Size", "0,50 0,10" );
        child->SetColourProperty( "BackgroundColour", Red );

        IMAGE::TPixelMapPtr pixels;
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 5, 5 ), Black ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 10, 10 ), White ) && Near( PixelAt( pixels, 109, 59 ), White ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 50, 30 ), Blue ) );
        // the child sticks out of the panel: shown outside without Clip
        ASSERT_TRUE( Near( PixelAt( pixels, 95, 35 ), Red ) && Near( PixelAt( pixels, 130, 35 ), Red ) );
        panel->SetBoolProperty( "Clip", true );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 95, 35 ), Red ) && Near( PixelAt( pixels, 130, 35 ), Black ) );

        // half transparent over blue
        child->SetProperty( "Position", "0,20 0,20" );
        child->SetFloatProperty( "Opacity", 0.5f );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 40, 35 ), GUI::CColour( 128, 0, 128, 255 ), 3 ) );

        // hidden: gone
        panel->SetVisibility( false );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 50, 30 ), Black ) );
        context->DestroyWidget( screen );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( prefix + "Picture buttons show the picture of their state, images scale, cut out and tint" ).C_String() )
    try
    {
        GUI::TGuiContextPtr contextPtr = createContext();
        GUI::CGUIContext* context = static_cast< GUI::CGUIContext* >( contextPtr.GetPointerAlways() );
        context->RegisterImage( "Normal", MakeSolidImage( 20, 10, 255, 0, 0, 255 ) );
        context->RegisterImage( "Hover", MakeSolidImage( 20, 10, 0, 255, 0, 255 ) );
        context->RegisterImage( "Pushed", MakeSolidImage( 20, 10, 0, 0, 255, 255 ) );
        GUI::CWidget* screen = Screen( context );
        GUI::CWidget* button = context->CreateWidget( "Button", "Button", screen );
        button->SetProperty( "Position", "0,100 0,100" );
        button->SetProperty( "NormalImage", "Normal" );
        button->SetProperty( "HoverImage", "Hover" );
        button->SetProperty( "PushedImage", "Pushed" );

        IMAGE::TPixelMapPtr pixels;
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( 200 == CountColour( pixels, GUI::CRect( 100, 100, 20, 10 ), Red ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 120, 105 ), Black ) );
        context->InjectMouseMove( 105, 105 );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 105, 105 ), Green ) );
        context->InjectMouseButton( GUI::MOUSEBUTTON_LEFT, true );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 105, 105 ), Blue ) );
        context->InjectMouseButton( GUI::MOUSEBUTTON_LEFT, false );

        // a 2x2 picture of four colours, made larger: nearest pixel, no blurring
        IMAGE::TPixelMapPtr quad = MakeSolidImage( 2, 2, 0, 0, 0, 255 );
        UInt8* q = static_cast< UInt8* >( quad->GetDataPtr() );
        q[ 0 ] = 255;                 // top left red
        q[ 5 ] = 255;                 // top right green
        q[ 10 ] = 255;                // bottom left blue
        q[ 12 ] = q[ 13 ] = q[ 14 ] = 255;  // bottom right white
        context->RegisterImage( "Quad", quad );
        GUI::CWidget* frame = context->CreateWidget( "ImageFrame", "Frame", screen );
        frame->SetProperty( "Position", "0,200 0,200" );
        frame->SetProperty( "Size", "0,40 0,20" );
        frame->SetProperty( "Image", "Quad" );
        ASSERT_TRUE( Capture( context, pixels ) );
        // stretched
        ASSERT_TRUE( 200 == CountColour( pixels, GUI::CRect( 200, 200, 20, 10 ), Red ) );
        ASSERT_TRUE( 200 == CountColour( pixels, GUI::CRect( 220, 210, 20, 10 ), White ) );
        // fitted: 20x20 in the middle of 40x20
        frame->SetProperty( "ScaleMode", "Fit" );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 205, 205 ), Black ) && Near( PixelAt( pixels, 212, 205 ), Red ) && Near( PixelAt( pixels, 227, 215 ), White ) );
        // one pixel of it, tinted
        frame->SetProperty( "ScaleMode", "Stretch" );
        frame->SetProperty( "SubRect", "1,1,1,1" );
        frame->SetColourProperty( "Tint", GUI::CColour( 255, 255, 0, 255 ) );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( 800 == CountColour( pixels, GUI::CRect( 200, 200, 40, 20 ), Yellow ) );
        context->DestroyWidget( screen );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( prefix + "Text in a bitmap font is drawn in its colour, aligned in the label" ).C_String() )
    try
    {
        GUI::TGuiContextPtr contextPtr = createContext();
        GUI::CGUIContext* context = static_cast< GUI::CGUIContext* >( contextPtr.GetPointerAlways() );
        context->RegisterFont( "Block", MakeTestFont( 8, 10 ) );
        GUI::CWidget* screen = Screen( context );
        GUI::CWidget* label = context->CreateWidget( "Label", "Label", screen );
        label->SetProperty( "Position", "0,100 0,50" );
        label->SetProperty( "Size", "0,100 0,30" );
        label->SetProperty( "Font", "Block" );
        label->SetProperty( "Text", "abc" );
        label->SetColourProperty( "TextColour", Yellow );
        label->SetProperty( "HTextAlign", "Center" );
        label->SetProperty( "VTextAlign", "Center" );

        IMAGE::TPixelMapPtr pixels;
        ASSERT_TRUE( Capture( context, pixels ) );
        // 24x10 of text in the middle of 100x30: from (138,60)
        ASSERT_TRUE( 240 == CountColour( pixels, GUI::CRect( 100, 50, 100, 30 ), Yellow ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 138, 60 ), Yellow ) && Near( PixelAt( pixels, 161, 69 ), Yellow ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 137, 60 ), Black ) && Near( PixelAt( pixels, 162, 60 ), Black ) );

        // a picture with transparent parts lets the background through
        IMAGE::TPixelMapPtr holes = MakeSolidImage( 2, 1, 255, 0, 0, 255 );
        static_cast< UInt8* >( holes->GetDataPtr() )[ 7 ] = 0;
        context->RegisterImage( "Holes", holes );
        GUI::CWidget* frame = context->CreateWidget( "ImageFrame", "Holes", screen );
        frame->SetProperty( "Position", "0,300 0,300" );
        frame->SetProperty( "Size", "0,20 0,10" );
        frame->SetProperty( "Image", "Holes" );
        screen->SetColourProperty( "BackgroundColour", Blue );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 305, 305 ), Red ) && Near( PixelAt( pixels, 315, 305 ), Blue ) );
        context->DestroyWidget( screen );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( ( prefix + "A draw surface draws its committed commands within its clip, roots stack in order, the cursor is on top" ).C_String() )
    try
    {
        GUI::TGuiContextPtr contextPtr = createContext();
        GUI::CGUIContext* context = static_cast< GUI::CGUIContext* >( contextPtr.GetPointerAlways() );
        context->RegisterFont( "Block", MakeTestFont( 8, 10 ) );
        context->RegisterImage( "Dot", MakeSolidImage( 3, 3, 0, 255, 0, 255 ) );
        GUI::CWidget* screen = Screen( context );
        GUI::CDrawSurface* surface = static_cast< GUI::CDrawSurface* >( context->CreateWidget( "DrawSurface", "Map", screen ) );
        surface->SetProperty( "Position", "0,10 0,10" );
        surface->SetProperty( "Size", "0,200 0,100" );
        surface->FillRect( GUI::CRect( 0, 0, 10, 10 ), Red );
        surface->DrawLine( 20, 5, 29, 5, White );
        GUI::CDrawSurface::TPointVector points( 1 );
        points[ 0 ].x = 40; points[ 0 ].y = 40;
        surface->DrawPoints( points, Yellow );
        surface->DrawImage( "Dot", GUI::CRect( 50, 50, 6, 6 ) );
        surface->PushClip( GUI::CRect( 100, 0, 5, 5 ) );
        surface->FillRect( GUI::CRect( 100, 0, 10, 10 ), Blue );
        surface->PopClip();
        surface->DrawString( "Block", "x", 150, 0, Yellow );
        // outside the surface: cut off
        surface->FillRect( GUI::CRect( 190, 90, 20, 20 ), Red );
        surface->Commit();

        IMAGE::TPixelMapPtr pixels;
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( 100 == CountColour( pixels, GUI::CRect( 10, 10, 10, 10 ), Red ) );
        ASSERT_TRUE( 10 == CountColour( pixels, GUI::CRect( 30, 15, 10, 1 ), White ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 50, 50 ), Yellow ) );
        ASSERT_TRUE( 36 == CountColour( pixels, GUI::CRect( 60, 60, 6, 6 ), Green ) );
        ASSERT_TRUE( 25 == CountColour( pixels, GUI::CRect( 110, 10, 10, 10 ), Blue ) );
        ASSERT_TRUE( 80 == CountColour( pixels, GUI::CRect( 160, 10, 8, 10 ), Yellow ) );
        ASSERT_TRUE( 100 == CountColour( pixels, GUI::CRect( 200, 100, 20, 20 ), Red ) );

        // a second root over the first, then the cursor over both
        GUI::CWidget* top = context->CreateWidget( "Panel", "Top", GUCEF_NULL );
        top->SetProperty( "Position", "0,10 0,10" );
        top->SetProperty( "Size", "0,5 0,5" );
        top->SetColourProperty( "BackgroundColour", White );
        context->ShowRoot( top, false );
        context->RegisterImage( "Cursor", MakeSolidImage( 4, 4, 255, 0, 255, 255 ) );
        context->SetCursor( "Cursor", 1, 1 );
        context->InjectMouseMove( 300, 300 );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( Near( PixelAt( pixels, 12, 12 ), White ) && Near( PixelAt( pixels, 16, 16 ), Red ) );
        ASSERT_TRUE( 16 == CountColour( pixels, GUI::CRect( 299, 299, 4, 4 ), GUI::CColour( 255, 0, 255, 255 ) ) );
        context->SetCursorVisible( false );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( 0 == CountColour( pixels, GUI::CRect( 299, 299, 4, 4 ), GUI::CColour( 255, 0, 255, 255 ) ) );

        // the tooltip comes when the pointer rests
        top->SetProperty( "Tooltip", "hi" );
        top->SetProperty( "Font", "Block" );
        context->InjectMouseMove( 12, 12 );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( 0 == CountColour( pixels, GUI::CRect( 24, 28, 24, 14 ), GUI::CColour( 250, 250, 210, 255 ) ) );
        context->RenderFrame( 700 );
        ASSERT_TRUE( Capture( context, pixels ) );
        ASSERT_TRUE( CountColour( pixels, GUI::CRect( 24, 28, 24, 14 ), GUI::CColour( 250, 250, 210, 255 ) ) > 0 );
        context->DestroyWidget( top );
        context->DestroyWidget( screen );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END
}

}

/** Counts the log messages that contain a text */
class CLogTextCounter : public CORE::CILogger
{
    public:

    CORE::CString m_text;
    UInt32 m_count;
    MT::CMutex m_lock;

    CLogTextCounter( const CORE::CString& text ) : CORE::CILogger(), m_text( text ), m_count( 0 ), m_lock() {}

    virtual void Log( const CORE::TLogMsgType logMsgType, const Int32 logLevel, const CORE::CString& logMessage, const UInt32 threadId, const CORE::CTimestamp& timestamp ) GUCEF_VIRTUAL_OVERRIDE
    {
        MT::CScopeMutex lock( m_lock );
        if ( logMessage.HasSubstr( m_text ) >= 0 )
            ++m_count;
    }

    virtual void Log( const CORE::TLogMsgType logMsgType, const Int32 logLevel, const CORE::CVariantStream& logMessage, const UInt32 threadId, const CORE::CTimestamp& timestamp ) GUCEF_VIRTUAL_OVERRIDE
    {
        Log( logMsgType, logLevel, logMessage.ToString(), threadId, timestamp );
    }

    virtual void LogWithoutFormatting( const CORE::TLogMsgType logMsgType, const Int32 logLevel, const CORE::CString& logMessage, const UInt32 threadId, const CORE::CTimestamp& timestamp ) GUCEF_VIRTUAL_OVERRIDE
    {
        Log( logMsgType, logLevel, logMessage, threadId, timestamp );
    }

    virtual void LogWithoutFormatting( const CORE::TLogMsgType logMsgType, const Int32 logLevel, const CORE::CVariantStream& logMessage, const UInt32 threadId, const CORE::CTimestamp& timestamp ) GUCEF_VIRTUAL_OVERRIDE
    {
        Log( logMsgType, logLevel, logMessage.ToString(), threadId, timestamp );
    }

    virtual void FlushLog( void ) GUCEF_VIRTUAL_OVERRIDE {}

    virtual void SetMinimalLogLevel( const Int32 logLevel ) GUCEF_VIRTUAL_OVERRIDE {}

    virtual Int32 GetMinimalLogLevel( void ) const GUCEF_VIRTUAL_OVERRIDE { return CORE::LOGLEVEL_EVERYTHING; }
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      TESTS                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformGuiRenderTests( void )
{GUCEF_TRACE;

    GUCEF_TESTFW_SUITE_SCOPE( "gucefGUI.Render" );

    // the null driver draws with the reference software renderer
    RunRenderTests( "Null", &CreateNullContext );

    // RmlUi drawing with OpenGL in an offscreen window (WebGL in a canvas in a browser): the same tests, then the
    // same pixels as the reference
    bool haveRmlUi = false;
    GUCEF_TESTFW_TESTCASE( "Render (RmlUi): The RmlUi driver makes a context in a window, offscreen with EGL or a WebGL canvas in a browser" )
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WASM_EMSCRIPTEN )
    if ( !IsInBrowser() )
        GUCEF_TESTFW_SKIP( "there is no web page to draw in outside a browser" );
    #endif
    try
    {
        GUI::TGuiContextPtr contextPtr = CreateRmlUiContext();
        GUI::CGUIContext* context = dynamic_cast< GUI::CGUIContext* >( contextPtr.GetPointerAlways() );
        ASSERT_TRUE( GUCEF_NULL != context );
        ASSERT_TRUE( GUCEF_NULL != context && "RmlUi" == context->GetDriver()->GetDriverName() );
        ASSERT_TRUE( GUCEF_NULL != context && context->GetDriver()->HasCapability( "CaptureFrame" ) );
        haveRmlUi = GUCEF_NULL != context;
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    if ( !haveRmlUi )
        return;
    RunRenderTests( "RmlUi", &CreateRmlUiContext );

    GUCEF_TESTFW_TESTCASE( "Render (RmlUi): A scene of every kind of drawing has the same pixels as the software reference" )
    try
    {
        GUI::TGuiContextPtr referencePtr = CreateNullContext();
        GUI::TGuiContextPtr rmlPtr = CreateRmlUiContext();
        GUI::CGUIContext* reference = static_cast< GUI::CGUIContext* >( referencePtr.GetPointerAlways() );
        GUI::CGUIContext* rml = static_cast< GUI::CGUIContext* >( rmlPtr.GetPointerAlways() );
        BuildMixedScene( reference );
        BuildMixedScene( rml );

        IMAGE::TPixelMapPtr expected, actual;
        ASSERT_TRUE( Capture( reference, expected ) );
        ASSERT_TRUE( Capture( rml, actual ) );
        UInt32 different = 0;
        Int32 firstX = -1, firstY = -1;
        for ( Int32 y=0; y<480; ++y )
        {
            for ( Int32 x=0; x<640; ++x )
            {
                if ( !Near( PixelAt( expected, x, y ), PixelAt( actual, x, y ) ) )
                {
                    if ( 0 == different )
                    {
                        firstX = x;
                        firstY = y;
                    }
                    ++different;
                }
            }
        }
        if ( 0 != different )
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RmlUi differs from the reference in " << different << " pixels, the first at " << firstX << "," << firstY );
        ASSERT_TRUE( 0 == different );

        // nothing changed: the second frame draws from the geometry and textures of the first
        UInt32 textures = static_cast< guidriverRmlUI::CRmlUIContext* >( rml )->GetTextureCreationCount();
        ASSERT_TRUE( Capture( rml, actual ) );
        ASSERT_TRUE( textures == static_cast< guidriverRmlUI::CRmlUIContext* >( rml )->GetTextureCreationCount() );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Render (RmlUi): A context that goes gives everything back to RmlUi before its renderer goes" )
    try
    {
        // RmlUi keeps what it made through a renderer until it is told to let go; a renderer deleted before that is
        // used after it is gone when RmlUi shuts down
        CLogTextCounter counter( "RenderInterface is being destroyed" );
        CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &counter );
        {
            GUI::TGuiContextPtr rmlPtr = CreateRmlUiContext();
            GUI::CGUIContext* rml = static_cast< GUI::CGUIContext* >( rmlPtr.GetPointerAlways() );
            BuildMixedScene( rml );
            IMAGE::TPixelMapPtr pixels;
            ASSERT_TRUE( Capture( rml, pixels ) );
        }
        CORE::CCoreGlobal::Instance()->GetLogManager().FlushLogs();
        CORE::CCoreGlobal::Instance()->GetLogManager().RemoveLogger( &counter );
        ASSERT_TRUE( 0 == counter.m_count );
    }
    catch ( ... )
    {
        ERRORHERE;
    }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/
