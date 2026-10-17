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

#include "gucefGUI_CWidgetTypeRegistry.h"
#include "gucefGUI_CWidget.h"
#include "gucefGUI_CPanel.h"
#include "gucefGUI_CWindow.h"
#include "gucefGUI_CLabel.h"
#include "gucefGUI_CButton.h"
#include "gucefGUI_CPushButton.h"
#include "gucefGUI_CCheckbox.h"
#include "gucefGUI_CEditbox.h"
#include "gucefGUI_CTextbox.h"
#include "gucefGUI_CListBox.h"
#include "gucefGUI_CCombobox.h"
#include "gucefGUI_CSpinner.h"
#include "gucefGUI_CSlider.h"
#include "gucefGUI_CProgressBar.h"
#include "gucefGUI_CImageFrame.h"
#include "gucefGUI_CDrawSurface.h"
#include "gucefGUI_CRenderContext.h"
#include "gucefGUI_CVideoView.h"
#include "gucefGUI_CAudioPlayer.h"
#include "gucefGUI_CTabControl.h"
#include "gucefGUI_CTabContentPane.h"
#include "gucefGUI_CTreeview.h"
#include "gucefGUI_CMenuBar.h"
#include "gucefGUI_CPopupMenu.h"
#include "gucefGUI_CGridView.h"

#include "gucefGUI_StandardWidgetTypes.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace {

typedef CPropertyDescriptor PD;

const UInt32 DEFAULTFLAGS = PD::FLAG_DEFAULT;
const UInt32 LAYOUTFLAGS = PD::FLAG_DEFAULT | PD::FLAG_AFFECTSLAYOUT;
const UInt32 RUNTIMEFLAGS = PD::FLAG_RUNTIME;

PD
Prop( const char* name, PD::TValueType type, const char* defaultValue, UInt32 flags, const char* category, const char* description )
{
    return PD( name, type, defaultValue, flags, category, description );
}

PD
EnumProp( const char* name, const char* defaultValue, const char* values, UInt32 flags, const char* category, const char* description )
{
    PD descriptor( name, PD::VALUETYPE_ENUM, defaultValue, flags, category, description );
    CString::StringVector parts = CString( values ).ParseElements( ',', false );
    for ( UInt32 i=0; i<(UInt32)parts.size(); ++i )
        descriptor.enumValues.push_back( parts[ i ] );
    return descriptor;
}

CWidgetTypeInfo
Type( const char* name, const char* base, const char* category, CWidgetTypeInfo::TWidgetCreator creator, const char* description )
{
    CWidgetTypeInfo info;
    info.typeName = name;
    info.baseTypeName = base;
    info.category = category;
    info.creator = creator;
    info.description = description;
    return info;
}

/** The text properties several types share */
void
AddTextProperties( CWidgetTypeInfo& info, const char* textColour )
{
    info.AddProperty( Prop( "Font", PD::VALUETYPE_FONT, "", LAYOUTFLAGS, "Text", "The font, a name registered with the context or known to the driver; empty for the driver's default" ) );
    info.AddProperty( Prop( "TextColour", PD::VALUETYPE_COLOUR, textColour, DEFAULTFLAGS, "Text", "The colour of the text" ) );
}

void
AddRowProperties( CWidgetTypeInfo& info )
{
    info.AddProperty( Prop( "RowHeight", PD::VALUETYPE_INT, "0", DEFAULTFLAGS, "Rows", "Height of a row in pixels; 0 for the line height of the font" ) );
    info.AddProperty( Prop( "ScrollPosition", PD::VALUETYPE_INT, "0", DEFAULTFLAGS, "Rows", "The first row shown" ) );
    info.AddProperty( Prop( "SelectionColour", PD::VALUETYPE_COLOUR, "60,90,160,255", DEFAULTFLAGS, "Rows", "Background of the selected row" ) );
    info.AddProperty( Prop( "HoverColour", PD::VALUETYPE_COLOUR, "", DEFAULTFLAGS, "Rows", "Outline of the row under the pointer; empty for none" ) );
    info.AddProperty( Prop( "HoverIndex", PD::VALUETYPE_INT, "-1", RUNTIMEFLAGS, "State", "The row under the pointer" ) );
}

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
RegisterStandardWidgetTypes( CWidgetTypeRegistry& registry )
{GUCEF_TRACE;

    CWidgetTypeInfo widget = Type( "Widget", "", "Base", GUCEF_NULL, "What every widget has" );
    widget.AddProperty( Prop( "Visible", PD::VALUETYPE_BOOL, "true", LAYOUTFLAGS, "Common", "Whether the widget and its children are shown" ) );
    widget.AddProperty( Prop( "Enabled", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Common", "Whether the widget reacts to input" ) );
    widget.AddProperty( Prop( "Position", PD::VALUETYPE_UVECTOR, "0,0 0,0", LAYOUTFLAGS, "Layout", "Position in the parent: per axis a part of the parent's size plus pixels" ) );
    widget.AddProperty( Prop( "Size", PD::VALUETYPE_UVECTOR, "", LAYOUTFLAGS, "Layout", "Size: per axis a part of the parent's size plus pixels; empty for the natural size" ) );
    widget.AddProperty( EnumProp( "HAlign", "Left", "Left,Center,Right", LAYOUTFLAGS, "Layout", "Which part of the widget is at the horizontal position" ) );
    widget.AddProperty( EnumProp( "VAlign", "Top", "Top,Center,Bottom", LAYOUTFLAGS, "Layout", "Which part of the widget is at the vertical position" ) );
    widget.AddProperty( Prop( "Clip", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Layout", "Children are cut off at the edge of the widget" ) );
    widget.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    widget.AddProperty( Prop( "TabIndex", PD::VALUETYPE_INT, "-1", DEFAULTFLAGS, "Input", "Place in the Tab order; -1 for after the numbered ones, in tree order" ) );
    widget.AddProperty( Prop( "NavUp", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Input", "The widget (name or path) the focus moves to on Up" ) );
    widget.AddProperty( Prop( "NavDown", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Input", "The widget the focus moves to on Down" ) );
    widget.AddProperty( Prop( "NavLeft", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Input", "The widget the focus moves to on Left" ) );
    widget.AddProperty( Prop( "NavRight", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Input", "The widget the focus moves to on Right" ) );
    widget.AddProperty( Prop( "InputTransparent", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Input", "Pointer input goes to what is below the widget" ) );
    widget.AddProperty( Prop( "Tooltip", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Common", "Text shown when the pointer rests on the widget" ) );
    widget.AddProperty( Prop( "StyleClass", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Common", "Theme classes, separated by spaces" ) );
    widget.AddProperty( Prop( "Opacity", PD::VALUETYPE_FLOAT, "1", DEFAULTFLAGS, "Common", "1 opaque, 0 invisible" ) );
    widget.AddProperty( Prop( "BackgroundColour", PD::VALUETYPE_COLOUR, "", DEFAULTFLAGS, "Look", "Fills the widget; empty for none" ) );
    registry.RegisterType( widget );

    CWidgetTypeInfo panel = Type( "Panel", "Widget", "Containers", &CPanel::Create, "A rectangle holding other widgets" );
    panel.AddProperty( Prop( "BackgroundImage", PD::VALUETYPE_IMAGE, "", LAYOUTFLAGS, "Look", "A picture filling the panel" ) );
    panel.AddProperty( Prop( "BorderColour", PD::VALUETYPE_COLOUR, "", DEFAULTFLAGS, "Look", "Colour of the border; empty for none" ) );
    panel.AddProperty( Prop( "BorderWidth", PD::VALUETYPE_INT, "1", DEFAULTFLAGS, "Look", "Width of the border in pixels" ) );
    registry.RegisterType( panel );

    registry.RegisterType( Type( "Form", "Panel", "Containers", &CPanel::Create, "The usual root of a layout" ) );

    CWidgetTypeInfo hbox = Type( "HBox", "Panel", "Containers", &CPanel::Create, "Places its children side by side, left to right" );
    hbox.AddProperty( Prop( "Padding", PD::VALUETYPE_INT, "0", LAYOUTFLAGS, "Layout", "Pixels between the edge and the children" ) );
    hbox.AddProperty( Prop( "Spacing", PD::VALUETYPE_INT, "0", LAYOUTFLAGS, "Layout", "Pixels between two children" ) );
    registry.RegisterType( hbox );

    CWidgetTypeInfo vbox = hbox;
    vbox.typeName = "VBox";
    vbox.description = "Places its children below each other, top to bottom";
    registry.RegisterType( vbox );

    CWidgetTypeInfo grid = hbox;
    grid.typeName = "Grid";
    grid.description = "Places its children in rows of equal columns";
    grid.AddProperty( Prop( "Columns", PD::VALUETYPE_INT, "1", LAYOUTFLAGS, "Layout", "The number of columns" ) );
    registry.RegisterType( grid );

    CWidgetTypeInfo margin = hbox;
    margin.typeName = "Margin";
    margin.description = "Gives every child its area less the padding";
    registry.RegisterType( margin );

    CWidgetTypeInfo window = Type( "Window", "Panel", "Containers", &CWindow::Create, "A panel with a title bar" );
    window.AddProperty( Prop( "Title", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Window", "The text in the title bar" ) );
    window.AddProperty( Prop( "TitleVisible", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Window", "Whether the title bar is shown" ) );
    window.AddProperty( Prop( "TitleHeight", PD::VALUETYPE_INT, "20", DEFAULTFLAGS, "Window", "Height of the title bar" ) );
    window.AddProperty( Prop( "TitleBarColour", PD::VALUETYPE_COLOUR, "40,60,110,255", DEFAULTFLAGS, "Window", "Colour of the title bar" ) );
    window.AddProperty( Prop( "TitleColour", PD::VALUETYPE_COLOUR, "255,255,255,255", DEFAULTFLAGS, "Window", "Colour of the title" ) );
    registry.RegisterType( window );

    CWidgetTypeInfo label = Type( "Label", "Widget", "Common", &CLabel::Create, "Text" );
    label.AddProperty( Prop( "Text", PD::VALUETYPE_STRING, "", LAYOUTFLAGS, "Text", "The text, '\\n' breaks a line" ) );
    AddTextProperties( label, "255,255,255,255" );
    label.AddProperty( EnumProp( "HTextAlign", "Left", "Left,Center,Right", DEFAULTFLAGS, "Text", "Horizontal place of the text in the label" ) );
    label.AddProperty( EnumProp( "VTextAlign", "Top", "Top,Center,Bottom", DEFAULTFLAGS, "Text", "Vertical place of the text in the label" ) );
    label.AddProperty( Prop( "WordWrap", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Text", "Lines too wide for the label are broken between words" ) );
    registry.RegisterType( label );

    CWidgetTypeInfo button = Type( "Button", "Widget", "Common", &CButton::Create, "A button with text, pictures or both" );
    button.AddProperty( Prop( "Text", PD::VALUETYPE_STRING, "", LAYOUTFLAGS, "Text", "The text on the button" ) );
    AddTextProperties( button, "255,255,255,255" );
    button.AddProperty( Prop( "NormalImage", PD::VALUETYPE_IMAGE, "", LAYOUTFLAGS, "Look", "The picture of the button" ) );
    button.AddProperty( Prop( "HoverImage", PD::VALUETYPE_IMAGE, "", DEFAULTFLAGS, "Look", "The picture while the pointer is on the button" ) );
    button.AddProperty( Prop( "PushedImage", PD::VALUETYPE_IMAGE, "", DEFAULTFLAGS, "Look", "The picture while the button is pressed" ) );
    button.AddProperty( Prop( "DisabledImage", PD::VALUETYPE_IMAGE, "", DEFAULTFLAGS, "Look", "The picture while the button is disabled" ) );
    button.AddProperty( Prop( "ClickSound", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Common", "A sound id for the application to play when the button is activated" ) );
    button.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    registry.RegisterType( button );

    registry.RegisterType( Type( "PushButton", "Button", "Common", &CPushButton::Create, "A button (the same as Button)" ) );

    CWidgetTypeInfo checkbox = Type( "Checkbox", "Button", "Common", &CCheckbox::Create, "A button that is on or off" );
    checkbox.AddProperty( Prop( "Checked", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Common", "Whether it is on" ) );
    checkbox.AddProperty( Prop( "CheckedImage", PD::VALUETYPE_IMAGE, "", DEFAULTFLAGS, "Look", "The picture while it is on" ) );
    registry.RegisterType( checkbox );

    CWidgetTypeInfo editbox = Type( "Editbox", "Widget", "Common", &CEditbox::Create, "One line of text the user types" );
    editbox.AddProperty( Prop( "Text", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Text", "The text" ) );
    AddTextProperties( editbox, "255,255,255,255" );
    editbox.AddProperty( Prop( "MaxLength", PD::VALUETYPE_INT, "0", DEFAULTFLAGS, "Text", "Most characters the text may have; 0 for no limit" ) );
    editbox.AddProperty( Prop( "ReadOnly", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Text", "The user cannot change the text" ) );
    editbox.AddProperty( Prop( "SelectAllOnFocus", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Text", "The text is selected when the box gets the focus, so typing replaces it" ) );
    editbox.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    editbox.AddProperty( Prop( "CaretVisible", PD::VALUETYPE_BOOL, "false", RUNTIMEFLAGS, "State", "Whether the blinking caret is shown now" ) );
    editbox.AddProperty( Prop( "Selected", PD::VALUETYPE_BOOL, "false", RUNTIMEFLAGS, "State", "Whether the text is selected" ) );
    registry.RegisterType( editbox );

    CWidgetTypeInfo textbox = Type( "Textbox", "Widget", "Common", &CTextbox::Create, "Several lines of text, with scrolling" );
    textbox.AddProperty( Prop( "Text", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Text", "The text, '\\n' breaks a line" ) );
    AddTextProperties( textbox, "255,255,255,255" );
    textbox.AddProperty( Prop( "WordWrap", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Text", "Lines too wide are broken between words" ) );
    textbox.AddProperty( Prop( "RowHeight", PD::VALUETYPE_INT, "0", DEFAULTFLAGS, "Rows", "Height of a line in pixels; 0 for the line height of the font" ) );
    textbox.AddProperty( Prop( "ScrollPosition", PD::VALUETYPE_INT, "0", DEFAULTFLAGS, "Rows", "The first line shown" ) );
    registry.RegisterType( textbox );

    CWidgetTypeInfo listbox = Type( "Listbox", "Widget", "Lists", &CListbox::Create, "A list of text rows to choose from" );
    listbox.AddProperty( Prop( "Items", PD::VALUETYPE_STRINGLIST, "", DEFAULTFLAGS, "Items", "The rows" ) );
    listbox.AddProperty( Prop( "SelectedIndex", PD::VALUETYPE_INT, "-1", DEFAULTFLAGS, "Items", "The selected row, -1 for none" ) );
    listbox.AddProperty( Prop( "MultiSelect", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Items", "Whether several rows can be selected (SelectedIndices)" ) );
    listbox.AddProperty( Prop( "SelectedIndices", PD::VALUETYPE_INTLIST, "", DEFAULTFLAGS, "Items", "The selected rows when MultiSelect is on" ) );
    listbox.AddProperty( Prop( "DisabledItems", PD::VALUETYPE_INTLIST, "", DEFAULTFLAGS, "Items", "Rows that cannot be chosen" ) );
    AddTextProperties( listbox, "255,255,255,255" );
    listbox.AddProperty( Prop( "DisabledTextColour", PD::VALUETYPE_COLOUR, "128,128,128,255", DEFAULTFLAGS, "Text", "The colour of the text of a disabled row" ) );
    listbox.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    AddRowProperties( listbox );
    registry.RegisterType( listbox );

    CWidgetTypeInfo combobox = Type( "Combobox", "Widget", "Lists", &CCombobox::Create, "One choice of a list that drops down" );
    combobox.AddProperty( Prop( "Items", PD::VALUETYPE_STRINGLIST, "", DEFAULTFLAGS, "Items", "The choices" ) );
    combobox.AddProperty( Prop( "SelectedIndex", PD::VALUETYPE_INT, "-1", DEFAULTFLAGS, "Items", "The choice, -1 for none" ) );
    combobox.AddProperty( Prop( "Text", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Text", "The text shown, the chosen item" ) );
    combobox.AddProperty( Prop( "DropDownRows", PD::VALUETYPE_INT, "8", DEFAULTFLAGS, "Items", "Most rows the open list shows" ) );
    AddTextProperties( combobox, "255,255,255,255" );
    combobox.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    combobox.AddProperty( Prop( "Expanded", PD::VALUETYPE_BOOL, "false", RUNTIMEFLAGS, "State", "Whether the list is open" ) );
    registry.RegisterType( combobox );

    CWidgetTypeInfo spinner = Type( "Spinner", "Widget", "Common", &CSpinner::Create, "A number stepped up and down" );
    spinner.AddProperty( Prop( "Value", PD::VALUETYPE_FLOAT, "0", DEFAULTFLAGS, "Value", "The number" ) );
    spinner.AddProperty( Prop( "Min", PD::VALUETYPE_FLOAT, "0", DEFAULTFLAGS, "Value", "The lowest value" ) );
    spinner.AddProperty( Prop( "Max", PD::VALUETYPE_FLOAT, "100", DEFAULTFLAGS, "Value", "The highest value" ) );
    spinner.AddProperty( Prop( "Step", PD::VALUETYPE_FLOAT, "1", DEFAULTFLAGS, "Value", "How much one step changes the value" ) );
    spinner.AddProperty( Prop( "Decimals", PD::VALUETYPE_INT, "0", DEFAULTFLAGS, "Value", "Digits shown after the point" ) );
    AddTextProperties( spinner, "255,255,255,255" );
    spinner.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    registry.RegisterType( spinner );

    CWidgetTypeInfo slider = Type( "Slider", "Widget", "Common", &CSlider::Create, "A value picked by dragging a thumb" );
    slider.AddProperty( Prop( "Value", PD::VALUETYPE_FLOAT, "0", DEFAULTFLAGS, "Value", "The value" ) );
    slider.AddProperty( Prop( "Min", PD::VALUETYPE_FLOAT, "0", DEFAULTFLAGS, "Value", "The lowest value" ) );
    slider.AddProperty( Prop( "Max", PD::VALUETYPE_FLOAT, "100", DEFAULTFLAGS, "Value", "The highest value" ) );
    slider.AddProperty( Prop( "Step", PD::VALUETYPE_FLOAT, "1", DEFAULTFLAGS, "Value", "Values are multiples of the step from Min; 0 for any value" ) );
    slider.AddProperty( EnumProp( "Orientation", "Horizontal", "Horizontal,Vertical", DEFAULTFLAGS, "Look", "Direction of the track" ) );
    slider.AddProperty( Prop( "ThumbSize", PD::VALUETYPE_INT, "12", DEFAULTFLAGS, "Look", "Length of the thumb in pixels" ) );
    slider.AddProperty( Prop( "TrackColour", PD::VALUETYPE_COLOUR, "80,80,80,255", DEFAULTFLAGS, "Look", "Colour of the track" ) );
    slider.AddProperty( Prop( "ThumbColour", PD::VALUETYPE_COLOUR, "200,200,200,255", DEFAULTFLAGS, "Look", "Colour of the thumb" ) );
    slider.AddProperty( Prop( "TrackImage", PD::VALUETYPE_IMAGE, "", DEFAULTFLAGS, "Look", "Picture of the track" ) );
    slider.AddProperty( Prop( "ThumbImage", PD::VALUETYPE_IMAGE, "", DEFAULTFLAGS, "Look", "Picture of the thumb" ) );
    slider.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    registry.RegisterType( slider );

    CWidgetTypeInfo scrollbar = Type( "Scrollbar", "Slider", "Common", &CSlider::CreateScrollbar, "A slider whose thumb shows the visible part of a range" );
    scrollbar.AddProperty( Prop( "PageSize", PD::VALUETYPE_FLOAT, "10", DEFAULTFLAGS, "Value", "How much of the range is visible; sets the length of the thumb" ) );
    scrollbar.AddProperty( EnumProp( "Orientation", "Vertical", "Horizontal,Vertical", DEFAULTFLAGS, "Look", "Direction of the track" ) );
    registry.RegisterType( scrollbar );

    CWidgetTypeInfo progress = Type( "ProgressBar", "Widget", "Common", &CProgressBar::Create, "Shows how far something is" );
    progress.AddProperty( Prop( "Value", PD::VALUETYPE_FLOAT, "0", DEFAULTFLAGS, "Value", "How far" ) );
    progress.AddProperty( Prop( "Min", PD::VALUETYPE_FLOAT, "0", DEFAULTFLAGS, "Value", "Nothing done" ) );
    progress.AddProperty( Prop( "Max", PD::VALUETYPE_FLOAT, "100", DEFAULTFLAGS, "Value", "All done" ) );
    progress.AddProperty( Prop( "BarColour", PD::VALUETYPE_COLOUR, "60,140,60,255", DEFAULTFLAGS, "Look", "Colour of the bar" ) );
    progress.AddProperty( Prop( "BarImage", PD::VALUETYPE_IMAGE, "", DEFAULTFLAGS, "Look", "Picture of the bar" ) );
    registry.RegisterType( progress );

    CWidgetTypeInfo image = Type( "ImageFrame", "Widget", "Media", &CImageFrame::Create, "A picture, still or animated" );
    image.AddProperty( Prop( "Image", PD::VALUETYPE_IMAGE, "", LAYOUTFLAGS, "Look", "The picture: a name registered with the context or a VFS path" ) );
    image.AddProperty( Prop( "SubRect", PD::VALUETYPE_RECT, "", LAYOUTFLAGS, "Look", "The part of the picture shown; empty for all of it" ) );
    image.AddProperty( EnumProp( "ScaleMode", "Stretch", "Stretch,None,Fit,Integer,Center", DEFAULTFLAGS, "Look", "How the picture fills the frame" ) );
    image.AddProperty( Prop( "Tint", PD::VALUETYPE_COLOUR, "255,255,255,255", DEFAULTFLAGS, "Look", "Multiplies the colours of the picture" ) );
    image.AddProperty( Prop( "Smooth", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Look", "Bilinear filtering when scaled; off keeps pixels sharp" ) );
    registry.RegisterType( image );

    CWidgetTypeInfo video = Type( "VideoView", "ImageFrame", "Media", &CVideoView::Create, "Plays a movie" );
    video.AddProperty( Prop( "Source", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Media", "The movie file, a VFS path" ) );
    video.AddProperty( Prop( "Loop", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Media", "Starts again at the end" ) );
    video.AddProperty( Prop( "SkipOnInput", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Media", "A click or a key ends the movie" ) );
    video.AddProperty( Prop( "AutoPlay", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Media", "Plays as soon as Source is set" ) );
    video.AddProperty( Prop( "Playing", PD::VALUETYPE_BOOL, "false", RUNTIMEFLAGS, "State", "Whether the movie plays" ) );
    video.AddProperty( EnumProp( "ScaleMode", "Fit", "Stretch,None,Fit,Integer,Center", DEFAULTFLAGS, "Look", "How the picture fills the frame" ) );
    registry.RegisterType( video );

    CWidgetTypeInfo audio = Type( "AudioPlayer", "Widget", "Media", &CAudioPlayer::Create, "Plays a sound file, with a play / pause control" );
    audio.AddProperty( Prop( "Source", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Media", "The sound file, a VFS path" ) );
    audio.AddProperty( Prop( "Volume", PD::VALUETYPE_FLOAT, "1", DEFAULTFLAGS, "Media", "0 silent to 1 full" ) );
    audio.AddProperty( Prop( "Loop", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Media", "Plays until stopped" ) );
    audio.AddProperty( Prop( "AutoPlay", PD::VALUETYPE_BOOL, "false", DEFAULTFLAGS, "Media", "Plays as soon as Source is set" ) );
    AddTextProperties( audio, "255,255,255,255" );
    audio.AddProperty( Prop( "BarColour", PD::VALUETYPE_COLOUR, "60,140,60,255", DEFAULTFLAGS, "Look", "Colour of the control" ) );
    audio.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    audio.AddProperty( Prop( "Playing", PD::VALUETYPE_BOOL, "false", RUNTIMEFLAGS, "State", "Whether the sound plays" ) );
    audio.AddProperty( Prop( "PositionInMs", PD::VALUETYPE_INT, "0", RUNTIMEFLAGS, "State", "How far it has played" ) );
    registry.RegisterType( audio );

    CWidgetTypeInfo draw = Type( "DrawSurface", "Widget", "Media", &CDrawSurface::Create, "Drawn by the application with 2D commands" );
    draw.AddProperty( Prop( "Clip", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Layout", "Nothing is drawn outside the widget" ) );
    draw.AddProperty( Prop( "Revision", PD::VALUETYPE_INT, "0", RUNTIMEFLAGS, "State", "Goes up with every committed drawing" ) );
    registry.RegisterType( draw );

    registry.RegisterType( Type( "RenderContext", "DrawSurface", "Media", &CRenderContext::Create, "A draw surface under its older name" ) );

    CWidgetTypeInfo tabs = Type( "TabControl", "Widget", "Containers", &CTabControl::Create, "Pages with tabs" );
    tabs.AddProperty( Prop( "Tabs", PD::VALUETYPE_STRINGLIST, "", LAYOUTFLAGS, "Items", "The tab titles, one per page" ) );
    tabs.AddProperty( Prop( "ActiveTab", PD::VALUETYPE_INT, "0", DEFAULTFLAGS, "Items", "The page shown" ) );
    tabs.AddProperty( Prop( "TabHeight", PD::VALUETYPE_INT, "20", LAYOUTFLAGS, "Look", "Height of the tabs" ) );
    tabs.AddProperty( Prop( "TabColour", PD::VALUETYPE_COLOUR, "50,50,50,255", DEFAULTFLAGS, "Look", "Colour of a tab" ) );
    tabs.AddProperty( Prop( "ActiveTabColour", PD::VALUETYPE_COLOUR, "90,90,90,255", DEFAULTFLAGS, "Look", "Colour of the active tab" ) );
    AddTextProperties( tabs, "255,255,255,255" );
    tabs.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    registry.RegisterType( tabs );

    registry.RegisterType( Type( "TabContentPane", "Panel", "Containers", &CTabContentPane::Create, "A page of a tab control" ) );

    CWidgetTypeInfo tree = Type( "Treeview", "Widget", "Lists", &CTreeview::Create, "A tree of items" );
    tree.AddProperty( Prop( "Items", PD::VALUETYPE_STRINGLIST, "", DEFAULTFLAGS, "Items", "One path per item, levels separated by '/'" ) );
    tree.AddProperty( Prop( "ExpandedItems", PD::VALUETYPE_STRINGLIST, "", DEFAULTFLAGS, "Items", "The paths that are open" ) );
    tree.AddProperty( Prop( "SelectedItem", PD::VALUETYPE_STRING, "", DEFAULTFLAGS, "Items", "The path of the selected item" ) );
    tree.AddProperty( Prop( "Indent", PD::VALUETYPE_INT, "12", DEFAULTFLAGS, "Look", "Pixels per level" ) );
    AddTextProperties( tree, "255,255,255,255" );
    tree.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    AddRowProperties( tree );
    registry.RegisterType( tree );

    CWidgetTypeInfo menubar = Type( "MenuBar", "Widget", "Menus", &CMenuBar::Create, "A bar of menus" );
    menubar.AddProperty( Prop( "Items", PD::VALUETYPE_STRINGLIST, "", LAYOUTFLAGS, "Items", "One \"Menu/Item\" path per menu item" ) );
    AddTextProperties( menubar, "255,255,255,255" );
    menubar.AddProperty( Prop( "HoverColour", PD::VALUETYPE_COLOUR, "70,70,70,255", DEFAULTFLAGS, "Look", "Background of the menu under the pointer" ) );
    menubar.AddProperty( Prop( "OpenMenu", PD::VALUETYPE_INT, "-1", RUNTIMEFLAGS, "State", "The open menu" ) );
    menubar.AddProperty( Prop( "HoverIndex", PD::VALUETYPE_INT, "-1", RUNTIMEFLAGS, "State", "The menu under the pointer" ) );
    registry.RegisterType( menubar );

    CWidgetTypeInfo popup = Type( "PopupMenu", "Widget", "Menus", &CPopupMenu::Create, "A menu on top of everything" );
    popup.AddProperty( Prop( "Items", PD::VALUETYPE_STRINGLIST, "", LAYOUTFLAGS, "Items", "The items; \"-\" is a separator" ) );
    popup.AddProperty( Prop( "DisabledItems", PD::VALUETYPE_INTLIST, "", DEFAULTFLAGS, "Items", "Items that cannot be chosen" ) );
    AddTextProperties( popup, "255,255,255,255" );
    popup.AddProperty( Prop( "DisabledTextColour", PD::VALUETYPE_COLOUR, "128,128,128,255", DEFAULTFLAGS, "Text", "The colour of a disabled item" ) );
    popup.AddProperty( Prop( "RowHeight", PD::VALUETYPE_INT, "0", LAYOUTFLAGS, "Rows", "Height of an item; 0 for the line height of the font" ) );
    popup.AddProperty( Prop( "HoverColour", PD::VALUETYPE_COLOUR, "60,90,160,255", DEFAULTFLAGS, "Look", "Background of the item under the pointer" ) );
    popup.AddProperty( Prop( "BackgroundColour", PD::VALUETYPE_COLOUR, "30,30,30,255", DEFAULTFLAGS, "Look", "Fills the menu" ) );
    popup.AddProperty( Prop( "HoverIndex", PD::VALUETYPE_INT, "-1", RUNTIMEFLAGS, "State", "The item under the pointer" ) );
    registry.RegisterType( popup );

    CWidgetTypeInfo gridview = Type( "GridView", "Widget", "Lists", &CGridView::Create, "A table of text cells" );
    gridview.AddProperty( Prop( "Columns", PD::VALUETYPE_STRINGLIST, "", DEFAULTFLAGS, "Items", "The column titles" ) );
    gridview.AddProperty( Prop( "ColumnWidths", PD::VALUETYPE_INTLIST, "", DEFAULTFLAGS, "Items", "Widths of the columns in pixels; empty for equal widths" ) );
    gridview.AddProperty( Prop( "Rows", PD::VALUETYPE_STRINGLIST, "", DEFAULTFLAGS, "Items", "One entry per row, the cells separated by a tab" ) );
    gridview.AddProperty( Prop( "SelectedRow", PD::VALUETYPE_INT, "-1", DEFAULTFLAGS, "Items", "The selected row, -1 for none" ) );
    gridview.AddProperty( Prop( "HeaderColour", PD::VALUETYPE_COLOUR, "50,50,50,255", DEFAULTFLAGS, "Look", "Background of the header row" ) );
    AddTextProperties( gridview, "255,255,255,255" );
    gridview.AddProperty( Prop( "Focusable", PD::VALUETYPE_BOOL, "true", DEFAULTFLAGS, "Input", "Whether the widget can get the keyboard focus" ) );
    AddRowProperties( gridview );
    registry.RegisterType( gridview );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
