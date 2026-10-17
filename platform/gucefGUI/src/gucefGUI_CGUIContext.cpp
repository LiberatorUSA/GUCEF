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

#include <algorithm>

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

#ifndef GUCEF_IMAGE_CIMAGE_H
#include "gucefIMAGE_CImage.h"
#define GUCEF_IMAGE_CIMAGE_H
#endif /* GUCEF_IMAGE_CIMAGE_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_GUI_CGUIGLOBAL_H
#include "gucefGUI_CGuiGlobal.h"
#define GUCEF_GUI_CGUIGLOBAL_H
#endif /* GUCEF_GUI_CGUIGLOBAL_H ? */

#ifndef GUCEF_GUI_CWIDGETTYPEREGISTRY_H
#include "gucefGUI_CWidgetTypeRegistry.h"
#define GUCEF_GUI_CWIDGETTYPEREGISTRY_H
#endif /* GUCEF_GUI_CWIDGETTYPEREGISTRY_H ? */

#ifndef GUCEF_GUI_CGUIDRIVER_H
#include "gucefGUI_CGUIDriver.h"
#define GUCEF_GUI_CGUIDRIVER_H
#endif /* GUCEF_GUI_CGUIDRIVER_H ? */

#ifndef GUCEF_GUI_CFORMEX_H
#include "gucefGUI_CFormEx.h"
#define GUCEF_GUI_CFORMEX_H
#endif /* GUCEF_GUI_CFORMEX_H ? */

#include "gucefGUI_CGUIContext.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CGUIContext::FrameStartedEvent = "GUCEF::GUI::CGUIContext::FrameStartedEvent";
const CORE::CEvent CGUIContext::FrameRenderedEvent = "GUCEF::GUI::CGUIContext::FrameRenderedEvent";
const CORE::CEvent CGUIContext::FormShownEvent = "GUCEF::GUI::CGUIContext::FormShownEvent";
const CORE::CEvent CGUIContext::FormHiddenEvent = "GUCEF::GUI::CGUIContext::FormHiddenEvent";

/** Two clicks within this time and distance make a double click */
static const UInt32 DOUBLECLICK_TIME_MS = 400;
static const Int32 DOUBLECLICK_DISTANCE = 4;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace {

UInt32
StateFromName( const CString& name )
{
    CString lower = name.Trim( true ).Trim( false ).Lowercase();
    if ( lower == "hover" || lower == "hovered" ) return CWidget::STATE_HOVERED;
    if ( lower == "pressed" ) return CWidget::STATE_PRESSED;
    if ( lower == "focused" || lower == "focus" ) return CWidget::STATE_FOCUSED;
    if ( lower == "disabled" ) return CWidget::STATE_DISABLED;
    if ( lower == "checked" ) return CWidget::STATE_CHECKED;
    return 0;
}

bool
HasStyleClass( const CWidget& widget, const CString& styleClass )
{
    CString classes = widget.GetExplicitProperties().count( "StyleClass" ) > 0 ? widget.GetExplicitProperties().find( "StyleClass" )->second : CString::Empty;
    if ( classes.IsNULLOrEmpty() )
        return false;
    CString::StringVector parts = classes.ParseElements( ' ', false );
    for ( UInt32 i=0; i<(UInt32)parts.size(); ++i )
        if ( parts[ i ] == styleClass )
            return true;
    return false;
}

Int32
Depth( const CWidget* widget )
{
    Int32 depth = 0;
    for ( const CWidget* p=widget->GetParentWidget(); GUCEF_NULL != p; p=p->GetParentWidget() )
        ++depth;
    return depth;
}

bool
IsDescendantOf( const CWidget* widget, const CWidget* ancestor )
{
    for ( const CWidget* p=widget; GUCEF_NULL != p; p=p->GetParentWidget() )
        if ( p == ancestor )
            return true;
    return false;
}

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIContext::SImageEntry::SImageEntry( void )
    : pixels()
    , revision( 0 )
{
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::RegisterEvents( void )
{GUCEF_TRACE;

    FrameStartedEvent.Initialize();
    FrameRenderedEvent.Initialize();
    FormShownEvent.Initialize();
    FormHiddenEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CGUIContext::CGUIContext( CGUIDriver* driver, TWindowContextPtr windowContext )
    : CIGUIContext()
    , CORE::CObservingNotifier()
    , m_driver( driver )
    , m_windowContext( windowContext )
    , m_widgets()
    , m_forms()
    , m_roots()
    , m_modalRoots()
    , m_dirtyWidgets()
    , m_updateWidgets()
    , m_images()
    , m_fonts()
    , m_handlerCache()
    , m_theme()
    , m_logicalWidth( 0 )
    , m_logicalHeight( 0 )
    , m_presentMode( PRESENTMODE_NEAREST )
    , m_cursorImage()
    , m_cursorHotspotX( 0 )
    , m_cursorHotspotY( 0 )
    , m_cursorVisible( true )
    , m_pointerX( 0 )
    , m_pointerY( 0 )
    , m_buttonStates( 0 )
    , m_modifiers( 0 )
    , m_hovered( GUCEF_NULL )
    , m_pressed( GUCEF_NULL )
    , m_pressedButton( MOUSEBUTTON_NONE )
    , m_focus( GUCEF_NULL )
    , m_popup( GUCEF_NULL )
    , m_lastClickTarget( GUCEF_NULL )
    , m_lastClickTime( 0 )
    , m_lastClickX( 0 )
    , m_lastClickY( 0 )
    , m_clickCount( 0 )
    , m_timeInMs( 0 )
    , m_hoverStartTime( 0 )
    , m_lastRealTick( 0 )
    , m_uniqueCounter( 0 )
    , m_layoutDirty( true )
    , m_rootOrderDirty( false )
    , m_inFlush( false )
    , m_dispatchDepth( 0 )
    , m_pendingDestroy()
{GUCEF_TRACE;

    RegisterEvents();
    if ( !m_windowContext.IsNULL() )
    {
        SubscribeTo( m_windowContext.GetPointerAlways(), CWindowContext::WindowContextRedrawEvent );
        SubscribeTo( m_windowContext.GetPointerAlways(), CWindowContext::WindowContextSizeEvent );
    }
}

/*-------------------------------------------------------------------------*/

CGUIContext::~CGUIContext()
{GUCEF_TRACE;

    UnsubscribeAllFromNotifier();
    DestroyAllWidgets();
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::DestroyAllWidgets( void )
{GUCEF_TRACE;

    // forms first (they destroy their root widgets), then whatever is left, parents first
    TFormSet forms = m_forms;
    for ( TFormSet::iterator i=forms.begin(); i!=forms.end(); ++i )
        delete (*i);
    m_forms.clear();

    m_dispatchDepth = 0;
    ProcessPendingDestroys();
    while ( !m_widgets.empty() )
    {
        CWidget* widget = *m_widgets.begin();
        while ( GUCEF_NULL != widget->GetParentWidget() )
            widget = widget->GetParentWidget();
        DestroyWidget( widget );
    }
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::CreateWidget( const CString& widgetType, const CString& widgetName, CWidget* parent )
{GUCEF_TRACE;

    CWidgetTypeRegistry& registry = CGuiGlobal::Instance()->GetWidgetTypeRegistry();
    CWidget* widget = registry.CreateWidget( widgetType );
    if ( GUCEF_NULL == widget )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GUIContext: Cannot create a widget of unknown type \"" << widgetType << "\"" );
        return GUCEF_NULL;
    }
    if ( GUCEF_NULL != parent && parent->m_context != this )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GUIContext: Cannot create widget \"" << widgetName << "\" below a widget of another context" );
        delete widget;
        return GUCEF_NULL;
    }

    widget->m_widgetTypeName = widgetType;
    widget->m_name = widgetName;
    widget->m_context = this;
    widget->m_handler = GetWidgetHandler( widgetType );
    if ( GUCEF_NULL == widget->m_handler )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "GUIContext: The driver has no handler for widget type \"" << widgetType << "\", widget \"" << widgetName << "\" will not be shown" );
    }
    m_widgets.insert( widget );

    if ( GUCEF_NULL != parent )
    {
        parent->m_children.push_back( widget );
        widget->m_parent = parent;
    }

    if ( GUCEF_NULL != widget->m_handler )
        widget->m_handler->CreateNative( *widget );

    // the native object starts at the values of every property
    CWidgetTypeRegistry::TDescriptorVector descriptors = registry.GetProperties( widgetType );
    for ( UInt32 i=0; i<(UInt32)descriptors.size(); ++i )
        widget->m_dirtyProperties.insert( descriptors[ i ]->name );
    widget->m_stateDirty = true;
    widget->m_rectDirty = true;
    m_dirtyWidgets.insert( widget );
    m_layoutDirty = true;
    return widget;
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::CreateWidget( const CString& widgetType )
{GUCEF_TRACE;

    return CreateWidget( widgetType, CString::Empty, GUCEF_NULL );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::DestroyWidget( CWidget* widget )
{GUCEF_TRACE;

    if ( GUCEF_NULL == widget || m_widgets.find( widget ) == m_widgets.end() )
        return;

    if ( m_dispatchDepth > 0 )
    {
        // an observer is still running for an event of this or another widget: take it out of
        // the tree now, delete it once the event is done
        for ( UInt32 i=0; i<(UInt32)m_pendingDestroy.size(); ++i )
            if ( m_pendingDestroy[ i ] == widget )
                return;
        ForgetInteraction( widget );
        if ( GUCEF_NULL == widget->m_parent )
            HideRoot( widget );
        else
        {
            widget->DetachFromParent();
            m_layoutDirty = true;
        }
        m_pendingDestroy.push_back( widget );
        return;
    }

    TWidgetVector children = widget->m_children;
    for ( UInt32 i=0; i<(UInt32)children.size(); ++i )
        DestroyWidget( children[ i ] );

    ForgetInteraction( widget );
    if ( GUCEF_NULL == widget->m_parent )
        HideRoot( widget );
    if ( GUCEF_NULL != widget->m_handler )
        widget->m_handler->DestroyNative( *widget );
    delete widget;
    m_layoutDirty = true;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::ProcessPendingDestroys( void )
{GUCEF_TRACE;

    if ( m_dispatchDepth > 0 )
        return;
    while ( !m_pendingDestroy.empty() )
    {
        TWidgetVector pending = m_pendingDestroy;
        m_pendingDestroy.clear();
        for ( UInt32 i=0; i<(UInt32)pending.size(); ++i )
            DestroyWidget( pending[ i ] );
    }
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::ForgetInteraction( CWidget* widget )
{GUCEF_TRACE;

    // the widget or one below it is going away: no hover, press, focus or popup may point into it
    if ( GUCEF_NULL != m_hovered && IsDescendantOf( m_hovered, widget ) )
        SetHovered( GUCEF_NULL );
    if ( GUCEF_NULL != m_pressed && IsDescendantOf( m_pressed, widget ) )
    {
        m_pressed->SetStateFlag( CWidget::STATE_PRESSED, false );
        m_pressed = GUCEF_NULL;
    }
    if ( GUCEF_NULL != m_focus && IsDescendantOf( m_focus, widget ) )
        SetFocus( GUCEF_NULL );
    if ( GUCEF_NULL != m_popup && IsDescendantOf( m_popup, widget ) )
        m_popup = GUCEF_NULL;
    if ( GUCEF_NULL != m_lastClickTarget && IsDescendantOf( m_lastClickTarget, widget ) )
        m_lastClickTarget = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CForm*
CGUIContext::CreateForm( const CString& formType )
{GUCEF_TRACE;

    CForm* form = GUCEF_NULL;
    if ( formType == "FormEx" )
        form = GUCEF_NEW CFormEx( this );
    else
    if ( formType == "Form" || formType.IsNULLOrEmpty() )
        form = GUCEF_NEW CForm( this );
    else
        return GUCEF_NULL;

    m_forms.insert( form );
    return form;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::DestroyForm( CForm* form )
{GUCEF_TRACE;

    if ( GUCEF_NULL == form )
        return;
    m_forms.erase( form );
    delete form;
}

/*-------------------------------------------------------------------------*/

CGUIContext::TStringSet
CGUIContext::GetAvailableFormTypes( void )
{GUCEF_TRACE;

    TStringSet types;
    types.insert( "Form" );
    types.insert( "FormEx" );
    return types;
}

/*-------------------------------------------------------------------------*/

CGUIContext::TStringSet
CGUIContext::GetAvailableWidgetTypes( void )
{GUCEF_TRACE;

    TStringSet available;
    CWidgetTypeRegistry::TStringSet types = CGuiGlobal::Instance()->GetWidgetTypeRegistry().GetTypeNames();
    for ( CWidgetTypeRegistry::TStringSet::iterator i=types.begin(); i!=types.end(); ++i )
        if ( GUCEF_NULL != GetWidgetHandler( *i ) )
            available.insert( *i );
    return available;
}

/*-------------------------------------------------------------------------*/

CFormBackend*
CGUIContext::CreateFormBackend( void )
{GUCEF_TRACE;

    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::DestroyFormBackend( CFormBackend* formBackend )
{GUCEF_TRACE;

    delete formBackend;
}

/*-------------------------------------------------------------------------*/

CGUIDriver*
CGUIContext::GetDriver( void )
{GUCEF_TRACE;

    return m_driver;
}

/*-------------------------------------------------------------------------*/

CGUIContext::TWidgetSet
CGUIContext::GetOwnedWidgets( void )
{GUCEF_TRACE;

    return m_widgets;
}

/*-------------------------------------------------------------------------*/

CGUIContext::TFormSet
CGUIContext::GetOwnedForms( void )
{GUCEF_TRACE;

    return m_forms;
}

/*-------------------------------------------------------------------------*/

TWindowContextPtr
CGUIContext::GetWindowContext( void ) const
{GUCEF_TRACE;

    return m_windowContext;
}

/*-------------------------------------------------------------------------*/

const CGUIContext::TWidgetVector&
CGUIContext::GetRootWidgets( void ) const
{GUCEF_TRACE;

    return m_roots;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::ShowRoot( CWidget* root, bool modal )
{GUCEF_TRACE;

    if ( GUCEF_NULL == root || root->m_context != this || GUCEF_NULL != root->m_parent )
        return false;

    for ( TWidgetVector::iterator i=m_roots.begin(); i!=m_roots.end(); ++i )
    {
        if ( (*i) == root )
        {
            m_roots.erase( i );
            break;
        }
    }
    m_roots.push_back( root );
    if ( modal )
        m_modalRoots.insert( root );
    else
        m_modalRoots.erase( root );

    // a modal root takes the keyboard focus away from everything outside of it
    if ( modal && GUCEF_NULL != m_focus && !IsDescendantOf( m_focus, root ) )
        SetFocus( GUCEF_NULL );

    m_rootOrderDirty = true;
    m_layoutDirty = true;
    CORE::TCloneableString eventData( root->GetName() );
    NotifyObservers( FormShownEvent, &eventData );
    UpdatePointerTarget();
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::HideRoot( CWidget* root )
{GUCEF_TRACE;

    for ( TWidgetVector::iterator i=m_roots.begin(); i!=m_roots.end(); ++i )
    {
        if ( (*i) == root )
        {
            m_roots.erase( i );
            m_modalRoots.erase( root );
            if ( m_popup == root )
                m_popup = GUCEF_NULL;
            ForgetInteraction( root );
            m_rootOrderDirty = true;
            CORE::TCloneableString eventData( root->GetName() );
            NotifyObservers( FormHiddenEvent, &eventData );
            UpdatePointerTarget();
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::IsModalRoot( const CWidget* root ) const
{GUCEF_TRACE;

    return m_modalRoots.find( const_cast< CWidget* >( root ) ) != m_modalRoots.end();
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::IsRootShown( const CWidget* root ) const
{GUCEF_TRACE;

    for ( UInt32 i=0; i<(UInt32)m_roots.size(); ++i )
        if ( m_roots[ i ] == root )
            return true;
    return false;
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::GetModalRoot( void ) const
{GUCEF_TRACE;

    for ( Int32 i=(Int32)m_roots.size()-1; i>=0; --i )
        if ( m_modalRoots.find( m_roots[ i ] ) != m_modalRoots.end() && m_roots[ i ]->IsVisible() )
            return m_roots[ i ];
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::ShowPopup( CWidget* popup, Int32 x, Int32 y )
{GUCEF_TRACE;

    if ( GUCEF_NULL == popup || GUCEF_NULL != popup->m_parent )
        return false;
    if ( GUCEF_NULL != m_popup && m_popup != popup )
        ClosePopup();

    popup->SetPosition( CUVector( CUDim( 0.0f, x ), CUDim( 0.0f, y ) ) );
    popup->SetVisibility( true );
    if ( !ShowRoot( popup, false ) )
        return false;
    m_popup = popup;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::ClosePopup( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_popup )
        return false;
    CWidget* popup = m_popup;
    m_popup = GUCEF_NULL;
    return HideRoot( popup );
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::GetPopup( void ) const
{GUCEF_TRACE;

    return m_popup;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::RegisterImage( const CString& alias, const IMAGE::TPixelMapPtr& pixels )
{GUCEF_TRACE;

    if ( alias.IsNULLOrEmpty() || pixels.IsNULL() )
        return false;

    IMAGE::TPixelMapPtr shared( pixels );
    if ( IMAGE::PSF_RGBA != shared->GetPixelStorageFormat() || GUCEF::DATATYPE_UINT8 != shared->GetPixelComponentDataType() )
    {
        // converted in place: the pixels are shared with the caller
        if ( !shared->ConvertFormatTo( IMAGE::PSF_RGBA, GUCEF::DATATYPE_UINT8 ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GUIContext: Image \"" << alias << "\" cannot be converted to RGBA" );
            return false;
        }
    }

    TImageEntry& entry = m_images[ alias ];
    entry.pixels = pixels;
    ++entry.revision;
    OnImageChanged( alias );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::UpdateImage( const CString& alias )
{GUCEF_TRACE;

    TImageMap::iterator i = m_images.find( alias );
    if ( i == m_images.end() )
        return false;
    ++(*i).second.revision;
    OnImageChanged( alias );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::UnregisterImage( const CString& alias )
{GUCEF_TRACE;

    if ( 0 == m_images.erase( alias ) )
        return false;
    OnImageChanged( alias );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::HasImage( const CString& alias ) const
{GUCEF_TRACE;

    return m_images.find( alias ) != m_images.end();
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::ResolveImage( const CString& reference, IMAGE::TPixelMapPtr& pixels, UInt32& revision )
{GUCEF_TRACE;

    if ( reference.IsNULLOrEmpty() )
        return false;

    TImageMap::iterator i = m_images.find( reference );
    if ( i != m_images.end() )
    {
        pixels = (*i).second.pixels;
        revision = (*i).second.revision;
        return true;
    }

    // not registered: a file in the VFS
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    VFS::TBasicVfsResourcePtr file = vfs.GetFile( reference );
    if ( file.IsNULL() || file->GetAccess().IsNULL() )
        return false;

    IMAGE::CImage image;
    CString imageType = CORE::ExtractFileExtention( reference ).Lowercase();
    if ( !image.Load( *file->GetAccess(), imageType ) || 0 == image.GetFrameCount() || image.GetFrame( 0 ).empty() )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "GUIContext: Cannot load image \"" << reference << "\"" );
        return false;
    }

    if ( !RegisterImage( reference, image.GetFrame( 0 )[ 0 ] ) )
        return false;
    TImageEntry& entry = m_images[ reference ];
    pixels = entry.pixels;
    revision = entry.revision;
    return true;
}

/*-------------------------------------------------------------------------*/

CString
CGUIContext::MakeUniqueImageAlias( const CString& prefix )
{GUCEF_TRACE;

    CString alias;
    do
    {
        alias = prefix + '#' + CORE::UInt32ToString( ++m_uniqueCounter );
    }
    while ( HasImage( alias ) );
    return alias;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::RegisterFont( const CString& name, const TBitmapFontPtr& font )
{GUCEF_TRACE;

    if ( name.IsNULLOrEmpty() || font.IsNULL() )
        return false;
    m_fonts[ name ] = font;
    m_layoutDirty = true;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::UnregisterFont( const CString& name )
{GUCEF_TRACE;

    return m_fonts.erase( name ) > 0;
}

/*-------------------------------------------------------------------------*/

TBitmapFontPtr
CGUIContext::GetBitmapFont( const CString& name ) const
{GUCEF_TRACE;

    TFontMap::const_iterator i = m_fonts.find( name );
    if ( i != m_fonts.end() )
        return (*i).second;
    return TBitmapFontPtr();
}

/*-------------------------------------------------------------------------*/

Int32
CGUIContext::GetLineHeight( const CString& fontName )
{GUCEF_TRACE;

    TBitmapFontPtr font = GetBitmapFont( fontName );
    if ( !font.IsNULL() && font->GetLineHeight() > 0 )
        return font->GetLineHeight();
    Int32 width = 0, height = 0;
    if ( MeasureText( fontName, "Mg", 0, width, height ) && height > 0 )
        return height;
    return 16;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::MeasureText( const CString& fontName, const CString& text, Int32 maxWidth, Int32& width, Int32& height )
{GUCEF_TRACE;

    TBitmapFontPtr font = GetBitmapFont( fontName );
    if ( font.IsNULL() )
    {
        width = 0;
        height = 0;
        return false;
    }
    font->MeasureText( text, maxWidth, width, height );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::SetTheme( const CORE::CDataNode& theme )
{GUCEF_TRACE;

    m_theme = theme;
    m_layoutDirty = true;

    // every property of every widget may now have another value
    for ( TWidgetSet::iterator i=m_widgets.begin(); i!=m_widgets.end(); ++i )
    {
        CWidget* widget = *i;
        CWidget::TStringVector names = widget->GetPropertyNames();
        for ( UInt32 n=0; n<(UInt32)names.size(); ++n )
            widget->m_dirtyProperties.insert( names[ n ] );
        m_dirtyWidgets.insert( widget );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
CGUIContext::GetTheme( void ) const
{GUCEF_TRACE;

    return m_theme;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::StyleMatches( const CORE::CDataNode& style, const CWidget& widget, bool ignoreState, Int32& score ) const
{GUCEF_TRACE;

    score = 0;
    CString type = style.GetAttributeValue( "type" ).AsString();
    if ( !type.IsNULLOrEmpty() )
    {
        if ( !CGuiGlobal::Instance()->GetWidgetTypeRegistry().IsA( widget.GetType(), type ) )
            return false;
        score += 1;
    }
    CString styleClass = style.GetAttributeValue( "class" ).AsString();
    if ( !styleClass.IsNULLOrEmpty() )
    {
        if ( !HasStyleClass( widget, styleClass ) )
            return false;
        score += 100;
    }
    CString name = style.GetAttributeValue( "name" ).AsString();
    if ( !name.IsNULLOrEmpty() )
    {
        if ( widget.GetName() != name )
            return false;
        score += 10000;
    }
    CString states = style.GetAttributeValue( "state" ).AsString();
    if ( !states.IsNULLOrEmpty() )
    {
        if ( ignoreState )
            return true;
        UInt32 widgetState = widget.GetState();
        CString::StringVector parts = states.ParseElements( ',', false );
        for ( UInt32 i=0; i<(UInt32)parts.size(); ++i )
        {
            UInt32 flag = StateFromName( parts[ i ] );
            if ( 0 == flag || 0 == ( widgetState & flag ) )
                return false;
            score += 10;
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::GetThemeProperty( const CWidget& widget, const CString& name, CString& value ) const
{GUCEF_TRACE;

    if ( 0 == m_theme.GetNrOfChildNodes() )
        return false;

    bool found = false;
    Int32 bestScore = -1;
    CORE::CDataNode::const_iterator i = m_theme.ConstBegin();
    while ( i != m_theme.ConstEnd() )
    {
        const CORE::CDataNode* style = (*i);
        ++i;
        if ( style->GetName() != "Style" )
            continue;
        Int32 score = 0;
        if ( !StyleMatches( *style, widget, false, score ) || score < bestScore )
            continue;

        CORE::CDataNode::const_iterator p = style->ConstBegin();
        while ( p != style->ConstEnd() )
        {
            const CORE::CDataNode* property = (*p);
            ++p;
            if ( property->GetName() == "Property" && property->GetAttributeValue( "name" ).AsString() == name )
            {
                value = property->GetAttributeValue( "value" ).AsString();
                bestScore = score;
                found = true;
            }
        }
    }
    return found;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::MarkThemeDependentDirty( CWidget& widget )
{GUCEF_TRACE;

    if ( 0 == m_theme.GetNrOfChildNodes() )
        return;

    CORE::CDataNode::const_iterator i = m_theme.ConstBegin();
    while ( i != m_theme.ConstEnd() )
    {
        const CORE::CDataNode* style = (*i);
        ++i;
        if ( style->GetName() != "Style" || style->GetAttributeValue( "state" ).AsString().IsNULLOrEmpty() )
            continue;
        Int32 score = 0;
        if ( !StyleMatches( *style, widget, true, score ) )
            continue;
        CORE::CDataNode::const_iterator p = style->ConstBegin();
        while ( p != style->ConstEnd() )
        {
            const CORE::CDataNode* property = (*p);
            ++p;
            if ( property->GetName() == "Property" )
                widget.m_dirtyProperties.insert( property->GetAttributeValue( "name" ).AsString() );
        }
        m_dirtyWidgets.insert( &widget );
    }
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::SetLogicalResolution( UInt32 width, UInt32 height )
{GUCEF_TRACE;

    m_logicalWidth = width;
    m_logicalHeight = height;
    m_layoutDirty = true;
}

/*-------------------------------------------------------------------------*/

UInt32
CGUIContext::GetLogicalWidth( void ) const
{GUCEF_TRACE;

    if ( 0 != m_logicalWidth || m_windowContext.IsNULL() )
        return m_logicalWidth;
    return m_windowContext->GetWidth();
}

/*-------------------------------------------------------------------------*/

UInt32
CGUIContext::GetLogicalHeight( void ) const
{GUCEF_TRACE;

    if ( 0 != m_logicalHeight || m_windowContext.IsNULL() )
        return m_logicalHeight;
    return m_windowContext->GetHeight();
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::SetPresentMode( TPresentMode mode )
{GUCEF_TRACE;

    m_presentMode = mode;
}

/*-------------------------------------------------------------------------*/

TPresentMode
CGUIContext::GetPresentMode( void ) const
{GUCEF_TRACE;

    return m_presentMode;
}

/*-------------------------------------------------------------------------*/

CRect
CGUIContext::GetPresentRect( void ) const
{GUCEF_TRACE;

    if ( m_windowContext.IsNULL() )
        return CRect( 0, 0, (Int32) GetLogicalWidth(), (Int32) GetLogicalHeight() );
    return CalculatePresentRect( GetLogicalWidth(), GetLogicalHeight(), m_windowContext->GetWidth(), m_windowContext->GetHeight(), m_presentMode );
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::WindowToLogical( Int32 windowX, Int32 windowY, Int32& x, Int32& y ) const
{GUCEF_TRACE;

    CRect present = GetPresentRect();
    if ( present.IsEmpty() )
    {
        x = windowX;
        y = windowY;
        return false;
    }
    x = (Int32) ( ( (Int64) ( windowX - present.left ) * (Int64) GetLogicalWidth() ) / present.width );
    y = (Int32) ( ( (Int64) ( windowY - present.top ) * (Int64) GetLogicalHeight() ) / present.height );
    return present.Contains( windowX, windowY );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::SetCursor( const CString& imageAlias, Int32 hotspotX, Int32 hotspotY )
{GUCEF_TRACE;

    m_cursorImage = imageAlias;
    m_cursorHotspotX = hotspotX;
    m_cursorHotspotY = hotspotY;

    // a picture of our own replaces the pointer of the system
    if ( !m_windowContext.IsNULL() )
        m_windowContext->SetSystemCursorVisible( m_cursorImage.IsNULLOrEmpty() );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::SetCursorVisible( bool visible )
{GUCEF_TRACE;

    m_cursorVisible = visible;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::IsCursorVisible( void ) const
{GUCEF_TRACE;

    return m_cursorVisible;
}

/*-------------------------------------------------------------------------*/

const CString&
CGUIContext::GetCursorImage( void ) const
{GUCEF_TRACE;

    return m_cursorImage;
}

/*-------------------------------------------------------------------------*/

Int32
CGUIContext::GetCursorHotspotX( void ) const
{GUCEF_TRACE;

    return m_cursorHotspotX;
}

/*-------------------------------------------------------------------------*/

Int32
CGUIContext::GetCursorHotspotY( void ) const
{GUCEF_TRACE;

    return m_cursorHotspotY;
}

/*-------------------------------------------------------------------------*/

Int32
CGUIContext::GetPointerX( void ) const
{GUCEF_TRACE;

    return m_pointerX;
}

/*-------------------------------------------------------------------------*/

Int32
CGUIContext::GetPointerY( void ) const
{GUCEF_TRACE;

    return m_pointerY;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::DispatchPointer( CWidget* target, const CORE::CEvent& eventId, TMouseButton button, UInt32 clickCount )
{GUCEF_TRACE;

    CPointerEventData eventData;
    eventData.x = m_pointerX;
    eventData.y = m_pointerY;
    eventData.button = button;
    eventData.buttonStates = m_buttonStates;
    eventData.modifiers = m_modifiers;
    eventData.clickCount = clickCount;
    if ( !target->DispatchEvent( eventId, eventData ) )
        target->OnDefaultAction( eventId, eventData );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::InjectMouseMove( Int32 x, Int32 y )
{GUCEF_TRACE;

    FlushLayoutIfNeeded();
    m_pointerX = x;
    m_pointerY = y;

    ++m_dispatchDepth;
    UpdatePointerTarget();
    CWidget* target = GUCEF_NULL != m_pressed ? m_pressed : HitTest( x, y );
    if ( GUCEF_NULL != target && AcceptsInput( target ) )
        DispatchPointer( target, CWidget::MouseMoveEvent, MOUSEBUTTON_NONE, 0 );
    --m_dispatchDepth;
    ProcessPendingDestroys();
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::InjectMouseButton( TMouseButton button, bool down )
{GUCEF_TRACE;

    FlushLayoutIfNeeded();
    UInt32 bit = 1u << (UInt32) button;
    ++m_dispatchDepth;

    if ( down )
    {
        m_buttonStates |= bit;
        CWidget* target = HitTest( m_pointerX, m_pointerY );

        // a press outside the popup closes it and is used up by that
        if ( GUCEF_NULL != m_popup && ( GUCEF_NULL == target || !IsDescendantOf( target, m_popup ) ) )
        {
            ClosePopup();
            --m_dispatchDepth;
            ProcessPendingDestroys();
            return;
        }

        if ( GUCEF_NULL != target && AcceptsInput( target ) )
        {
            // the press gives the focus to the first focusable widget at or above the target
            for ( CWidget* w=target; GUCEF_NULL != w; w=w->m_parent )
            {
                if ( w->IsFocusable() )
                {
                    SetFocus( w );
                    break;
                }
            }

            if ( GUCEF_NULL == m_pressed )
            {
                m_pressed = target;
                m_pressedButton = button;
                target->SetStateFlag( CWidget::STATE_PRESSED, true );
            }
            DispatchPointer( target, CWidget::MouseDownEvent, button, 0 );
        }
    }
    else
    {
        m_buttonStates &= ~bit;
        if ( GUCEF_NULL != m_pressed && button == m_pressedButton )
        {
            CWidget* pressed = m_pressed;
            m_pressed = GUCEF_NULL;
            m_pressedButton = MOUSEBUTTON_NONE;
            pressed->SetStateFlag( CWidget::STATE_PRESSED, false );
            DispatchPointer( pressed, CWidget::MouseUpEvent, button, 0 );

            // a click: released over the widget that was pressed (it may have gone away meanwhile)
            CWidget* hit = HitTest( m_pointerX, m_pointerY );
            if ( m_widgets.find( pressed ) != m_widgets.end() && GUCEF_NULL != hit && IsDescendantOf( hit, pressed ) && AcceptsInput( pressed ) )
            {
                Int32 dx = m_pointerX - m_lastClickX;
                Int32 dy = m_pointerY - m_lastClickY;
                if ( pressed == m_lastClickTarget && m_timeInMs - m_lastClickTime <= DOUBLECLICK_TIME_MS &&
                     dx >= -DOUBLECLICK_DISTANCE && dx <= DOUBLECLICK_DISTANCE && dy >= -DOUBLECLICK_DISTANCE && dy <= DOUBLECLICK_DISTANCE )
                {
                    m_clickCount = m_clickCount >= 3 ? 1 : m_clickCount + 1;
                }
                else
                {
                    m_clickCount = 1;
                }
                m_lastClickTarget = pressed;
                m_lastClickTime = m_timeInMs;
                m_lastClickX = m_pointerX;
                m_lastClickY = m_pointerY;

                UInt32 clickCount = m_clickCount;
                DispatchPointer( pressed, CWidget::MouseClickEvent, button, clickCount );
                if ( 2 == clickCount && m_widgets.find( pressed ) != m_widgets.end() )
                    DispatchPointer( pressed, CWidget::MouseDoubleClickEvent, button, clickCount );
                if ( 3 == clickCount && m_widgets.find( pressed ) != m_widgets.end() )
                    DispatchPointer( pressed, CWidget::MouseTripleClickEvent, button, clickCount );

                if ( MOUSEBUTTON_LEFT == button && m_widgets.find( pressed ) != m_widgets.end() && pressed->IsActivatable() )
                {
                    CRoutedEventData eventData;
                    if ( !pressed->DispatchEvent( CWidget::ActivatedEvent, eventData ) )
                        pressed->OnDefaultAction( CWidget::ActivatedEvent, eventData );
                }
            }
        }
        else
        {
            CWidget* target = HitTest( m_pointerX, m_pointerY );
            if ( GUCEF_NULL != target && AcceptsInput( target ) )
                DispatchPointer( target, CWidget::MouseUpEvent, button, 0 );
        }
    }

    UpdatePointerTarget();
    --m_dispatchDepth;
    ProcessPendingDestroys();
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::InjectMouseWheel( Int32 delta )
{GUCEF_TRACE;

    FlushLayoutIfNeeded();
    CWidget* target = HitTest( m_pointerX, m_pointerY );
    if ( GUCEF_NULL == target || !AcceptsInput( target ) || 0 == delta )
        return;

    ++m_dispatchDepth;
    CWheelEventData eventData;
    eventData.x = m_pointerX;
    eventData.y = m_pointerY;
    eventData.delta = delta;
    eventData.modifiers = m_modifiers;
    if ( !target->DispatchEvent( CWidget::MouseWheelEvent, eventData ) )
    {
        // the first widget at or above the target that does something with the wheel (scrolling)
        for ( CWidget* w=target; GUCEF_NULL != w; w=w->m_parent )
            if ( w->OnDefaultAction( CWidget::MouseWheelEvent, eventData ) )
                break;
    }
    --m_dispatchDepth;
    ProcessPendingDestroys();
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::GetKeyboardTarget( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_focus )
        return m_focus;
    if ( GUCEF_NULL != m_popup )
        return m_popup;
    CWidget* modal = GetModalRoot();
    if ( GUCEF_NULL != modal )
        return modal;
    for ( Int32 i=(Int32)m_roots.size()-1; i>=0; --i )
        if ( m_roots[ i ]->IsVisible() )
            return m_roots[ i ];
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::InjectKey( INPUT::KeyCode keyCode, bool down, UInt32 modifiers )
{GUCEF_TRACE;

    FlushLayoutIfNeeded();
    m_modifiers = modifiers;
    CWidget* target = GetKeyboardTarget();
    if ( GUCEF_NULL == target )
        return;

    ++m_dispatchDepth;
    CKeyEventData eventData;
    eventData.keyCode = keyCode;
    eventData.modifiers = modifiers;
    const CORE::CEvent& eventId = down ? CWidget::KeyDownEvent : CWidget::KeyUpEvent;
    if ( !target->DispatchEvent( eventId, eventData ) && down && !target->OnDefaultAction( eventId, eventData ) )
    {
        // keys nobody wanted become navigation
        bool shift = 0 != ( modifiers & INPUT::KEYMOD_SHIFT );
        switch ( keyCode )
        {
            case INPUT::KEYCODE_TAB: InjectNavigation( shift ? NAVIGATION_PREVIOUS : NAVIGATION_NEXT ); break;
            case INPUT::KEYCODE_UP: InjectNavigation( NAVIGATION_UP ); break;
            case INPUT::KEYCODE_DOWN: InjectNavigation( NAVIGATION_DOWN ); break;
            case INPUT::KEYCODE_LEFT: InjectNavigation( NAVIGATION_LEFT ); break;
            case INPUT::KEYCODE_RIGHT: InjectNavigation( NAVIGATION_RIGHT ); break;
            case INPUT::KEYCODE_RETURN:
            case INPUT::KEYCODE_NUMPADENTER: InjectNavigation( NAVIGATION_ACCEPT ); break;
            case INPUT::KEYCODE_ESCAPE: InjectNavigation( NAVIGATION_CANCEL ); break;
            default: break;
        }
    }
    --m_dispatchDepth;
    ProcessPendingDestroys();
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::InjectText( const CString& text )
{GUCEF_TRACE;

    if ( text.IsNULLOrEmpty() )
        return;
    FlushLayoutIfNeeded();
    CWidget* target = GetKeyboardTarget();
    if ( GUCEF_NULL == target )
        return;

    ++m_dispatchDepth;
    CTextEventData eventData;
    eventData.text = text;
    if ( !target->DispatchEvent( CWidget::TextInputEvent, eventData ) )
        target->OnDefaultAction( CWidget::TextInputEvent, eventData );
    --m_dispatchDepth;
    ProcessPendingDestroys();
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::InjectNavigation( TNavigation direction )
{GUCEF_TRACE;

    FlushLayoutIfNeeded();
    CWidget* target = GetKeyboardTarget();
    if ( GUCEF_NULL == target )
        return;

    ++m_dispatchDepth;
    CNavigationEventData eventData;
    eventData.direction = direction;
    if ( !target->DispatchEvent( CWidget::NavigationEvent, eventData ) && !target->OnDefaultAction( CWidget::NavigationEvent, eventData ) )
    {
        if ( NAVIGATION_ACCEPT == direction )
        {
            if ( GUCEF_NULL != m_focus && m_focus->IsActivatable() && AcceptsInput( m_focus ) )
            {
                CWidget* focus = m_focus;
                CRoutedEventData activated;
                if ( !focus->DispatchEvent( CWidget::ActivatedEvent, activated ) )
                    focus->OnDefaultAction( CWidget::ActivatedEvent, activated );
            }
        }
        else
        if ( NAVIGATION_CANCEL == direction )
        {
            if ( GUCEF_NULL != m_popup )
                ClosePopup();
        }
        else
        {
            Navigate( direction );
        }
    }
    --m_dispatchDepth;
    ProcessPendingDestroys();
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::ProcessWindowMouseMove( Int32 windowX, Int32 windowY )
{GUCEF_TRACE;

    Int32 x = 0, y = 0;
    WindowToLogical( windowX, windowY, x, y );
    InjectMouseMove( x, y );
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::HitTestWidget( CWidget* widget, Int32 x, Int32 y ) const
{GUCEF_TRACE;

    if ( !widget->IsVisible() )
        return GUCEF_NULL;
    bool inside = widget->m_rect.Contains( x, y );
    if ( !inside && widget->GetBoolProperty( "Clip" ) )
        return GUCEF_NULL;

    for ( Int32 i=(Int32)widget->m_children.size()-1; i>=0; --i )
    {
        CWidget* hit = HitTestWidget( widget->m_children[ i ], x, y );
        if ( GUCEF_NULL != hit )
            return hit;
    }
    if ( inside && !widget->GetBoolProperty( "InputTransparent" ) )
        return widget;
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::HitTest( Int32 x, Int32 y ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_popup )
    {
        CWidget* hit = HitTestWidget( m_popup, x, y );
        if ( GUCEF_NULL != hit )
            return hit;
    }

    CWidget* modal = GetModalRoot();
    if ( GUCEF_NULL != modal )
        return HitTestWidget( modal, x, y );

    for ( Int32 i=(Int32)m_roots.size()-1; i>=0; --i )
    {
        if ( m_roots[ i ] == m_popup )
            continue;
        CWidget* hit = HitTestWidget( m_roots[ i ], x, y );
        if ( GUCEF_NULL != hit )
            return hit;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::AcceptsInput( const CWidget* widget ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == widget || !widget->IsEffectivelyVisible() || !widget->IsEffectivelyEnabled() )
        return false;
    const CWidget* root = widget;
    while ( GUCEF_NULL != root->m_parent )
        root = root->m_parent;
    if ( !IsRootShown( root ) )
        return false;
    CWidget* modal = GetModalRoot();
    return GUCEF_NULL == modal || root == modal || root == m_popup;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::UpdatePointerTarget( void )
{GUCEF_TRACE;

    CWidget* hit = HitTest( m_pointerX, m_pointerY );
    if ( GUCEF_NULL != hit && !AcceptsInput( hit ) )
        hit = GUCEF_NULL;
    if ( GUCEF_NULL != m_pressed && hit != m_pressed )
        hit = GUCEF_NULL;  // while a widget is pressed only that one shows hover
    SetHovered( hit );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::SetHovered( CWidget* widget )
{GUCEF_TRACE;

    if ( widget == m_hovered )
        return;

    CWidget* old = m_hovered;
    m_hovered = widget;
    m_hoverStartTime = m_timeInMs;
    if ( GUCEF_NULL != old )
    {
        old->SetStateFlag( CWidget::STATE_HOVERED, false );
        CPointerEventData eventData;
        eventData.x = m_pointerX;
        eventData.y = m_pointerY;
        old->DispatchDirectEvent( CWidget::MouseFocusLostEvent, eventData );
        old->OnDefaultAction( CWidget::MouseFocusLostEvent, eventData );
    }
    if ( GUCEF_NULL != widget )
    {
        widget->SetStateFlag( CWidget::STATE_HOVERED, true );
        CPointerEventData eventData;
        eventData.x = m_pointerX;
        eventData.y = m_pointerY;
        widget->DispatchDirectEvent( CWidget::MouseFocusReceivedEvent, eventData );
        widget->OnDefaultAction( CWidget::MouseFocusReceivedEvent, eventData );
    }
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::GetFocus( void ) const
{GUCEF_TRACE;

    return m_focus;
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::GetHovered( void ) const
{GUCEF_TRACE;

    return m_hovered;
}

/*-------------------------------------------------------------------------*/

UInt64
CGUIContext::GetHoverDurationInMs( void ) const
{GUCEF_TRACE;

    return m_timeInMs - m_hoverStartTime;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::SetFocus( CWidget* widget )
{GUCEF_TRACE;

    if ( widget == m_focus )
        return true;
    if ( GUCEF_NULL != widget && ( widget->m_context != this || !widget->IsFocusable() ) )
        return false;

    CWidget* old = m_focus;
    m_focus = widget;
    if ( GUCEF_NULL != old )
    {
        old->SetStateFlag( CWidget::STATE_FOCUSED, false );
        CFocusEventData eventData;
        eventData.other = widget;
        old->DispatchDirectEvent( CWidget::KeyboardFocusLostEvent, eventData );
        old->OnDefaultAction( CWidget::KeyboardFocusLostEvent, eventData );
    }
    if ( GUCEF_NULL != widget && widget == m_focus )
    {
        widget->SetStateFlag( CWidget::STATE_FOCUSED, true );
        CFocusEventData eventData;
        eventData.other = old;
        widget->DispatchDirectEvent( CWidget::KeyboardFocusReceivedEvent, eventData );
        widget->OnDefaultAction( CWidget::KeyboardFocusReceivedEvent, eventData );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::CollectFocusable( CWidget* widget, TWidgetVector& focusable ) const
{GUCEF_TRACE;

    if ( !widget->IsVisible() || !widget->IsEnabled() )
        return;
    if ( widget->IsFocusable() )
        focusable.push_back( widget );
    for ( UInt32 i=0; i<(UInt32)widget->m_children.size(); ++i )
        CollectFocusable( widget->m_children[ i ], focusable );
}

/*-------------------------------------------------------------------------*/

CWidget*
CGUIContext::GetRootOf( CWidget* widget ) const
{GUCEF_TRACE;

    while ( GUCEF_NULL != widget && GUCEF_NULL != widget->m_parent )
        widget = widget->m_parent;
    return widget;
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::Navigate( TNavigation direction )
{GUCEF_TRACE;

    FlushLayoutIfNeeded();

    // the scope: the popup, the modal root, the root of the focus, else the topmost root
    CWidget* scope = m_popup;
    if ( GUCEF_NULL == scope )
        scope = GetModalRoot();
    if ( GUCEF_NULL == scope && GUCEF_NULL != m_focus )
        scope = GetRootOf( m_focus );
    if ( GUCEF_NULL == scope && !m_roots.empty() )
        scope = m_roots.back();
    if ( GUCEF_NULL == scope )
        return false;

    TWidgetVector focusable;
    CollectFocusable( scope, focusable );
    if ( focusable.empty() )
        return false;

    // tab order: widgets with a TabIndex >= 0 by that index, then the rest in tree order
    TWidgetVector ordered;
    GUCEF::vector< std::pair< Int32, UInt32 > > indexed;
    for ( UInt32 i=0; i<(UInt32)focusable.size(); ++i )
    {
        Int32 tabIndex = focusable[ i ]->GetIntProperty( "TabIndex" );
        if ( tabIndex >= 0 )
            indexed.push_back( std::pair< Int32, UInt32 >( tabIndex, i ) );
    }
    std::stable_sort( indexed.begin(), indexed.end() );
    for ( UInt32 i=0; i<(UInt32)indexed.size(); ++i )
        ordered.push_back( focusable[ indexed[ i ].second ] );
    for ( UInt32 i=0; i<(UInt32)focusable.size(); ++i )
        if ( focusable[ i ]->GetIntProperty( "TabIndex" ) < 0 )
            ordered.push_back( focusable[ i ] );

    if ( GUCEF_NULL == m_focus || !IsDescendantOf( m_focus, scope ) )
        return SetFocus( NAVIGATION_PREVIOUS == direction ? ordered.back() : ordered.front() );

    Int32 current = -1;
    for ( UInt32 i=0; i<(UInt32)ordered.size(); ++i )
        if ( ordered[ i ] == m_focus )
            current = (Int32) i;

    if ( NAVIGATION_NEXT == direction || NAVIGATION_PREVIOUS == direction )
    {
        Int32 count = (Int32) ordered.size();
        Int32 next = NAVIGATION_NEXT == direction ? ( current + 1 ) % count : ( current - 1 + count ) % count;
        return SetFocus( ordered[ next ] );
    }

    // an explicit neighbour first
    const char* neighbourProperty = NAVIGATION_UP == direction ? "NavUp" : NAVIGATION_DOWN == direction ? "NavDown" : NAVIGATION_LEFT == direction ? "NavLeft" : "NavRight";
    CString neighbourName = m_focus->GetProperty( neighbourProperty );
    if ( !neighbourName.IsNULLOrEmpty() )
    {
        CWidget* neighbour = scope->GetName() == neighbourName ? scope : scope->FindWidget( neighbourName );
        if ( GUCEF_NULL != neighbour && neighbour->IsFocusable() )
            return SetFocus( neighbour );
    }

    // else the nearest focusable widget in that direction, measured between centres; leaving the line costs double
    const CRect& from = m_focus->m_rect;
    Int32 fx = from.left + from.width / 2;
    Int32 fy = from.top + from.height / 2;
    CWidget* best = GUCEF_NULL;
    Int64 bestCost = 0;
    for ( UInt32 i=0; i<(UInt32)ordered.size(); ++i )
    {
        CWidget* candidate = ordered[ i ];
        if ( candidate == m_focus )
            continue;
        const CRect& to = candidate->m_rect;
        Int32 cx = to.left + to.width / 2;
        Int32 cy = to.top + to.height / 2;
        Int32 primary = 0;
        Int32 secondary = 0;
        switch ( direction )
        {
            case NAVIGATION_UP: primary = fy - cy; secondary = cx - fx; break;
            case NAVIGATION_DOWN: primary = cy - fy; secondary = cx - fx; break;
            case NAVIGATION_LEFT: primary = fx - cx; secondary = cy - fy; break;
            default: primary = cx - fx; secondary = cy - fy; break;
        }
        if ( primary <= 0 )
            continue;
        if ( secondary < 0 )
            secondary = -secondary;
        Int64 cost = (Int64) primary + 2 * (Int64) secondary;
        if ( GUCEF_NULL == best || cost < bestCost )
        {
            best = candidate;
            bestCost = cost;
        }
    }
    if ( GUCEF_NULL == best )
        return false;
    return SetFocus( best );
}

/*-------------------------------------------------------------------------*/

UInt64
CGUIContext::GetTimeInMs( void ) const
{GUCEF_TRACE;

    return m_timeInMs;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::RenderFrame( UInt32 deltaInMs )
{GUCEF_TRACE;

    m_timeInMs += deltaInMs;

    ++m_dispatchDepth;
    CORE::TCloneableUInt32 eventData( deltaInMs );
    NotifyObservers( FrameStartedEvent, &eventData );

    // widgets with time based behaviour: animations, video, the caret
    TWidgetVector updating;
    for ( TWidgetSet::iterator i=m_widgets.begin(); i!=m_widgets.end(); ++i )
        if ( (*i)->NeedsUpdate() )
            updating.push_back( *i );
    for ( UInt32 i=0; i<(UInt32)updating.size(); ++i )
        if ( m_widgets.find( updating[ i ] ) != m_widgets.end() )
            updating[ i ]->OnUpdate( deltaInMs );
    --m_dispatchDepth;
    ProcessPendingDestroys();

    FlushChanges();
    UpdatePointerTarget();
    FlushChanges();
    OnRender();
    NotifyObservers( FrameRenderedEvent );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::FlushLayoutIfNeeded( void )
{GUCEF_TRACE;

    if ( m_layoutDirty && !m_inFlush )
        UpdateLayout();
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::FlushChanges( void )
{GUCEF_TRACE;

    if ( m_inFlush )
        return;
    m_inFlush = true;

    if ( m_layoutDirty )
        UpdateLayout();

    // parents before children, so a native parent exists before its children are put in it
    UInt32 rounds = 0;
    while ( !m_dirtyWidgets.empty() && rounds++ < 8 )
    {
        TWidgetVector dirty( m_dirtyWidgets.begin(), m_dirtyWidgets.end() );
        m_dirtyWidgets.clear();
        GUCEF::vector< std::pair< Int32, CWidget* > > byDepth;
        for ( UInt32 i=0; i<(UInt32)dirty.size(); ++i )
            if ( m_widgets.find( dirty[ i ] ) != m_widgets.end() )
                byDepth.push_back( std::pair< Int32, CWidget* >( Depth( dirty[ i ] ), dirty[ i ] ) );
        std::stable_sort( byDepth.begin(), byDepth.end() );
        for ( UInt32 i=0; i<(UInt32)byDepth.size(); ++i )
            FlushWidget( *byDepth[ i ].second );
        if ( m_layoutDirty )
            UpdateLayout();
    }

    if ( m_rootOrderDirty )
    {
        m_rootOrderDirty = false;
        OnRootOrderChanged();
    }
    m_inFlush = false;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::FlushWidget( CWidget& widget )
{GUCEF_TRACE;

    CWidgetHandler* handler = widget.m_handler;
    CWidget::TStringSet properties;
    properties.swap( widget.m_dirtyProperties );

    if ( GUCEF_NULL == handler )
    {
        widget.m_parentDirty = widget.m_stateDirty = widget.m_rectDirty = widget.m_childOrderDirty = false;
        return;
    }

    if ( widget.m_parentDirty )
    {
        widget.m_parentDirty = false;
        handler->ApplyParent( widget );
    }
    for ( CWidget::TStringSet::iterator i=properties.begin(); i!=properties.end(); ++i )
    {
        const CString& name = *i;
        CString value = widget.GetProperty( name );
        CWidget::TApplierOverrideMap::iterator o = widget.m_applierOverrides.find( name );
        if ( o != widget.m_applierOverrides.end() && GUCEF_NULL != (*o).second )
            (*o).second( *handler, widget, value );
        else
            handler->ApplyProperty( widget, name, value );
    }
    if ( widget.m_stateDirty )
    {
        widget.m_stateDirty = false;
        handler->ApplyState( widget );
    }
    if ( widget.m_rectDirty )
    {
        widget.m_rectDirty = false;
        handler->ApplyLayout( widget, widget.m_rect );
    }
    if ( widget.m_childOrderDirty )
    {
        widget.m_childOrderDirty = false;
        handler->ApplyChildOrder( widget );
    }
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::InvalidateLayout( void )
{GUCEF_TRACE;

    m_layoutDirty = true;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::UpdateLayout( void )
{GUCEF_TRACE;

    m_layoutDirty = false;
    CRect screen( 0, 0, (Int32) GetLogicalWidth(), (Int32) GetLogicalHeight() );
    for ( TWidgetSet::iterator i=m_widgets.begin(); i!=m_widgets.end(); ++i )
    {
        CWidget* widget = *i;
        if ( GUCEF_NULL == widget->m_parent )
            PlaceWidget( *widget, screen, false );
    }
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::ResolveSize( const CWidget& widget, const CRect& area, Int32& width, Int32& height ) const
{GUCEF_TRACE;

    CString sizeValue = widget.GetProperty( "Size" );
    Int32 naturalWidth = 0, naturalHeight = 0;
    bool natural = sizeValue.IsNULLOrEmpty();
    if ( natural )
        widget.GetNaturalSize( naturalWidth, naturalHeight );

    if ( natural )
    {
        width = naturalWidth;
        height = naturalHeight;
        return;
    }
    CUVector size;
    CUVector::FromString( sizeValue, size );
    width = size.GetX().Resolve( area.width );
    height = size.GetY().Resolve( area.height );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::PlaceWidget( CWidget& widget, const CRect& area, bool sizeFromArea )
{GUCEF_TRACE;

    CRect rect;
    if ( sizeFromArea )
    {
        rect = area;
    }
    else
    {
        Int32 width = 0, height = 0;
        ResolveSize( widget, area, width, height );

        CUVector position = widget.GetUVectorProperty( "Position" );
        Int32 x = position.GetX().Resolve( area.width );
        Int32 y = position.GetY().Resolve( area.height );

        // the position may be the centre or far edge of the widget instead of its top left
        CString hAlign = widget.GetProperty( "HAlign" );
        if ( hAlign == "Center" )
            x -= width / 2;
        else if ( hAlign == "Right" )
            x -= width;
        CString vAlign = widget.GetProperty( "VAlign" );
        if ( vAlign == "Center" )
            y -= height / 2;
        else if ( vAlign == "Bottom" )
            y -= height;

        rect = CRect( area.left + x, area.top + y, width, height );
    }

    ApplyLayoutResult( widget, rect );
    LayoutChildren( widget );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::ApplyLayoutResult( CWidget& widget, const CRect& rect )
{GUCEF_TRACE;

    if ( widget.m_rect == rect )
        return;
    widget.m_rect = rect;
    widget.m_rectDirty = true;
    m_dirtyWidgets.insert( &widget );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::LayoutChildren( CWidget& widget )
{GUCEF_TRACE;

    if ( widget.m_children.empty() )
        return;

    CWidgetTypeRegistry& registry = CGuiGlobal::Instance()->GetWidgetTypeRegistry();
    const CString& type = widget.GetType();
    bool hbox = registry.IsA( type, "HBox" );
    bool vbox = registry.IsA( type, "VBox" );
    bool grid = registry.IsA( type, "Grid" );
    bool margin = registry.IsA( type, "Margin" );

    if ( registry.IsA( type, "TabControl" ) )
    {
        // the pages fill the area below the tabs
        Int32 header = widget.GetIntProperty( "TabHeight" );
        CRect pages( widget.m_rect.left, widget.m_rect.top + header, widget.m_rect.width, widget.m_rect.height - header );
        if ( pages.height < 0 )
            pages.height = 0;
        for ( UInt32 i=0; i<(UInt32)widget.m_children.size(); ++i )
        {
            CWidget& child = *widget.m_children[ i ];
            if ( registry.IsA( child.GetType(), "TabContentPane" ) )
                PlaceWidget( child, pages, true );
            else
                PlaceWidget( child, widget.m_rect, false );
        }
        return;
    }

    if ( !hbox && !vbox && !grid && !margin )
    {
        for ( UInt32 i=0; i<(UInt32)widget.m_children.size(); ++i )
            PlaceWidget( *widget.m_children[ i ], widget.m_rect, false );
        return;
    }

    // the containers: children are put in the content area (the rectangle less the padding)
    Int32 padding = widget.GetIntProperty( "Padding" );
    Int32 spacing = widget.GetIntProperty( "Spacing" );
    CRect content( widget.m_rect.left + padding, widget.m_rect.top + padding, widget.m_rect.width - 2*padding, widget.m_rect.height - 2*padding );
    if ( content.width < 0 ) content.width = 0;
    if ( content.height < 0 ) content.height = 0;

    if ( margin )
    {
        for ( UInt32 i=0; i<(UInt32)widget.m_children.size(); ++i )
            PlaceWidget( *widget.m_children[ i ], content, true );
        return;
    }

    if ( hbox || vbox )
    {
        Int32 offset = 0;
        for ( UInt32 i=0; i<(UInt32)widget.m_children.size(); ++i )
        {
            CWidget& child = *widget.m_children[ i ];
            Int32 w = 0, h = 0;
            ResolveSize( child, content, w, h );
            CRect cell;
            if ( hbox )
            {
                Int32 y = content.top;
                CString align = child.GetProperty( "VAlign" );
                if ( align == "Center" ) y = content.top + ( content.height - h ) / 2;
                else if ( align == "Bottom" ) y = content.Bottom() - h;
                cell = CRect( content.left + offset, y, w, h );
                if ( child.IsVisible() )
                    offset += w + spacing;
            }
            else
            {
                Int32 x = content.left;
                CString align = child.GetProperty( "HAlign" );
                if ( align == "Center" ) x = content.left + ( content.width - w ) / 2;
                else if ( align == "Right" ) x = content.Right() - w;
                cell = CRect( x, content.top + offset, w, h );
                if ( child.IsVisible() )
                    offset += h + spacing;
            }
            PlaceWidget( child, cell, true );
        }
        return;
    }

    // grid: equal columns, each row as high as its highest child
    Int32 columns = widget.GetIntProperty( "Columns" );
    if ( columns < 1 )
        columns = 1;
    Int32 cellWidth = ( content.width - ( columns - 1 ) * spacing ) / columns;
    Int32 y = content.top;
    UInt32 index = 0;
    TWidgetVector visible;
    for ( UInt32 i=0; i<(UInt32)widget.m_children.size(); ++i )
        if ( widget.m_children[ i ]->IsVisible() )
            visible.push_back( widget.m_children[ i ] );
        else
            PlaceWidget( *widget.m_children[ i ], CRect( content.left, content.top, 0, 0 ), true );
    while ( index < (UInt32) visible.size() )
    {
        Int32 rowHeight = 0;
        for ( Int32 c=0; c<columns && index + c < (UInt32) visible.size(); ++c )
        {
            Int32 w = 0, h = 0;
            ResolveSize( *visible[ index + c ], CRect( 0, 0, cellWidth, content.height ), w, h );
            if ( h > rowHeight )
                rowHeight = h;
        }
        for ( Int32 c=0; c<columns && index < (UInt32) visible.size(); ++c, ++index )
            PlaceWidget( *visible[ index ], CRect( content.left + c * ( cellWidth + spacing ), y, cellWidth, rowHeight ), true );
        y += rowHeight + spacing;
    }
}

/*-------------------------------------------------------------------------*/

bool
CGUIContext::CaptureFrame( IMAGE::TPixelMapPtr& pixels )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnRender( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnRootOrderChanged( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnImageChanged( const CString& alias )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CWidgetHandler*
CGUIContext::GetWidgetHandler( const CString& widgetType )
{GUCEF_TRACE;

    THandlerCache::iterator i = m_handlerCache.find( widgetType );
    if ( i != m_handlerCache.end() )
        return (*i).second;

    CWidgetHandler* handler = GUCEF_NULL;
    if ( GUCEF_NULL != m_driver )
    {
        GUCEF::vector< CString > chain = CGuiGlobal::Instance()->GetWidgetTypeRegistry().GetTypeChain( widgetType );
        for ( UInt32 c=0; c<(UInt32)chain.size() && GUCEF_NULL == handler; ++c )
            handler = m_driver->GetWidgetHandler( chain[ c ] );
    }
    m_handlerCache[ widgetType ] = handler;
    return handler;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnNotify( CORE::CNotifier* notifier                 ,
                       const CORE::CEvent& eventId               ,
                       CORE::CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;

    if ( CWindowContext::WindowContextRedrawEvent == eventId )
    {
        UInt64 tick = MT::PrecisionTickCount();
        UInt64 resolution = MT::PrecisionTimerResolution();
        UInt32 delta = 0;
        if ( 0 != m_lastRealTick && 0 != resolution )
            delta = (UInt32) ( ( tick - m_lastRealTick ) * 1000 / resolution );
        m_lastRealTick = tick;
        RenderFrame( delta );
    }
    else
    if ( CWindowContext::WindowContextSizeEvent == eventId )
    {
        m_layoutDirty = true;
    }
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnWidgetDirty( CWidget* widget )
{GUCEF_TRACE;

    m_dirtyWidgets.insert( widget );
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnWidgetDestroyed( CWidget* widget )
{GUCEF_TRACE;

    ForgetInteraction( widget );
    m_widgets.erase( widget );
    m_dirtyWidgets.erase( widget );
    m_updateWidgets.erase( widget );
    for ( TWidgetVector::iterator i=m_roots.begin(); i!=m_roots.end(); ++i )
    {
        if ( (*i) == widget )
        {
            m_roots.erase( i );
            m_rootOrderDirty = true;
            break;
        }
    }
    m_modalRoots.erase( widget );
    for ( TWidgetVector::iterator i=m_pendingDestroy.begin(); i!=m_pendingDestroy.end(); ++i )
    {
        if ( (*i) == widget )
        {
            m_pendingDestroy.erase( i );
            break;
        }
    }
    widget->m_context = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnWidgetParentChanged( CWidget* widget, CWidget* oldParent )
{GUCEF_TRACE;

    // a widget that moves below another one is no root any more
    if ( GUCEF_NULL != widget->m_parent )
    {
        for ( TWidgetVector::iterator i=m_roots.begin(); i!=m_roots.end(); ++i )
        {
            if ( (*i) == widget )
            {
                m_roots.erase( i );
                m_modalRoots.erase( widget );
                m_rootOrderDirty = true;
                break;
            }
        }
    }
    m_layoutDirty = true;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnWidgetVisibilityChanged( CWidget* widget )
{GUCEF_TRACE;

    if ( !widget->IsVisible() )
        ForgetInteraction( widget );
    m_layoutDirty = true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
