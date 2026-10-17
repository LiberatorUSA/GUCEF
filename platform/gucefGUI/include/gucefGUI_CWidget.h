/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_GUI_CWIDGET_H
#define GUCEF_GUI_CWIDGET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_GUI_CUVECTOR_H
#include "gucefGUI_CUVector.h"
#define GUCEF_GUI_CUVECTOR_H
#endif /* GUCEF_GUI_CUVECTOR_H ? */

#ifndef GUCEF_GUI_CUDIM_H
#include "gucefGUI_CUDim.h"
#define GUCEF_GUI_CUDIM_H
#endif /* GUCEF_GUI_CUDIM_H ? */

#ifndef GUCEF_GUI_TYPES_H
#include "gucefGUI_Types.h"
#define GUCEF_GUI_TYPES_H
#endif /* GUCEF_GUI_TYPES_H ? */

#ifndef GUCEF_GUI_CROUTEDEVENTDATA_H
#include "gucefGUI_CRoutedEventData.h"
#define GUCEF_GUI_CROUTEDEVENTDATA_H
#endif /* GUCEF_GUI_CROUTEDEVENTDATA_H ? */

#ifndef GUCEF_GUI_CWIDGETHANDLER_H
#include "gucefGUI_CWidgetHandler.h"
#define GUCEF_GUI_CWIDGETHANDLER_H
#endif /* GUCEF_GUI_CWIDGETHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CGUIContext;
class CPropertyDescriptor;

/**
 *  The facade every widget type extends. A widget keeps its property values itself; the driver of
 *  the context supplies a handler (CWidgetHandler) that keeps a native object in step with them.
 *  Game and editor code only ever talk to widgets, never to the backend.
 *
 *  Properties are strings, checked and normalized against the descriptors of the widget type
 *  (CWidgetTypeRegistry). The value of a property is, in this order: the value set on the widget,
 *  the value the theme of the context gives for the widget and its state, the default of the
 *  descriptor. Unknown properties are kept as well (a layout file may carry properties of a newer
 *  version or backend hints) and are passed to the handler, which may ignore them.
 *
 *  Input events are routed: see CRoutedEventData. All event data classes are in gucefGUI_CRoutedEventData.h:
 *  Mouse* events carry CPointerEventData, MouseWheelEvent CWheelEventData, Key* CKeyEventData,
 *  TextInputEvent CTextEventData, NavigationEvent CNavigationEventData, *FocusReceived/Lost CFocusEventData,
 *  ValueChangedEvent CValueChangedEventData, ItemActivatedEvent CItemEventData, ActivatedEvent CRoutedEventData.
 *  PropertyChangedEvent carries a TCloneableString with the name of the property.
 */
class GUCEF_GUI_PUBLIC_CPP CWidget : public CORE::CObservingNotifier
{
    public:

    static const CORE::CEvent PropertiesLoadedEvent;
    static const CORE::CEvent PropertiesSavedEvent;
    static const CORE::CEvent PropertyChangedEvent;
    static const CORE::CEvent MouseMoveEvent;
    static const CORE::CEvent MouseDownEvent;
    static const CORE::CEvent MouseUpEvent;
    static const CORE::CEvent MouseClickEvent;
    static const CORE::CEvent MouseDoubleClickEvent;
    static const CORE::CEvent MouseTripleClickEvent;
    static const CORE::CEvent MouseFocusReceivedEvent;      /**< the pointer entered the widget (not routed) */
    static const CORE::CEvent MouseFocusLostEvent;          /**< the pointer left the widget (not routed) */
    static const CORE::CEvent MouseWheelEvent;
    static const CORE::CEvent KeyDownEvent;
    static const CORE::CEvent KeyUpEvent;
    static const CORE::CEvent TextInputEvent;
    static const CORE::CEvent KeyboardFocusReceivedEvent;   /**< not routed */
    static const CORE::CEvent KeyboardFocusLostEvent;       /**< not routed */
    static const CORE::CEvent NavigationEvent;
    static const CORE::CEvent ActivatedEvent;               /**< a button was clicked with the left button or accepted with Enter / a gamepad */
    static const CORE::CEvent ValueChangedEvent;            /**< the user (or a command) changed the value of the widget */
    static const CORE::CEvent ItemActivatedEvent;           /**< an item of a list, menu or tree was chosen */

    static void RegisterEvents( void );

    public:

    typedef GUCEF::set< CWidget* > TWidgetSet;
    typedef GUCEF::vector< CWidget* > TWidgetVector;
    typedef GUCEF::map< CString, CString > TPropertyMap;
    typedef GUCEF::vector< CString > TStringVector;
    typedef GUCEF::set< CString > TStringSet;

    /** The interaction states a theme can select on */
    enum EState
    {
        STATE_HOVERED   = 1 ,
        STATE_PRESSED   = 2 ,
        STATE_FOCUSED   = 4 ,
        STATE_DISABLED  = 8 ,
        STATE_CHECKED   = 16
    };

    CWidget( void );

    CWidget( const CString& widgetTypeName );

    virtual ~CWidget();

    /*
     *  Typed access to the common properties
     */

    virtual bool SetVisibility( const bool isVisible );

    virtual bool IsVisible( void ) const;

    /** Shows the widget and keeps input inside it until it is hidden again */
    virtual bool ShowModal( void );

    virtual bool SetEnabled( const bool isEnabled );

    virtual bool IsEnabled( void ) const;

    virtual bool SetPosition( const CUVector& newPos );

    virtual bool GetPosition( CUVector& pos ) const;

    virtual bool SetSize( const CUVector& newSize );

    virtual bool GetSize( CUVector& size ) const;

    virtual bool SetWidth( const CUDim& newWidth );

    virtual bool GetWidth( CUDim& width ) const;

    virtual bool SetHeight( const CUDim& newHeight );

    virtual bool GetHeight( CUDim& height ) const;

    /** Puts the widget below another one (on top of its existing children); NULL makes it a root of its context */
    virtual bool SetParentWidget( CWidget* parentWidget );

    virtual CWidget* GetParentWidget( void ) const;

    /** A direct child with the given name */
    virtual CWidget* GetChildWidget( const CString& widgetName ) const;

    virtual bool GetChildWidgetSet( TWidgetSet& childSet ) const;

    const TWidgetVector& GetChildren( void ) const;

    /**
     *  Finds a widget below this one: a path of names separated by '/' starting at a child of this
     *  widget ("Panel/OkButton"), or a single name which is searched for at any depth (depth first,
     *  the first match wins)
     */
    CWidget* FindWidget( const CString& nameOrPath ) const;

    /** The names from the root down to this widget, separated by '/' */
    CString GetNamePath( void ) const;

    virtual CString GetName( void ) const;

    void SetName( const CString& name );

    /*
     *  Properties
     */

    /** Sets a property; returns false when the value does not fit the type of a known property */
    bool SetProperty( const CString& name, const CString& value );

    /** The value of the property: set on the widget, else from the theme, else the default */
    CString GetProperty( const CString& name ) const;

    /** The value set on the widget itself, if any */
    bool GetExplicitProperty( const CString& name, CString& value ) const;

    bool IsPropertySet( const CString& name ) const;

    /** Drops the value set on the widget, so the theme or the default applies again */
    bool ResetProperty( const CString& name );

    const TPropertyMap& GetExplicitProperties( void ) const;

    /** The names of every property the widget type has */
    TStringVector GetPropertyNames( void ) const;

    const CPropertyDescriptor* GetPropertyDescriptor( const CString& name ) const;

    bool GetBoolProperty( const CString& name ) const;
    Int32 GetIntProperty( const CString& name ) const;
    Float32 GetFloatProperty( const CString& name ) const;
    CColour GetColourProperty( const CString& name ) const;
    CUDim GetUDimProperty( const CString& name ) const;
    CUVector GetUVectorProperty( const CString& name ) const;
    CRect GetRectProperty( const CString& name ) const;
    TStringVector GetStringListProperty( const CString& name ) const;
    GUCEF::vector< Int32 > GetIntListProperty( const CString& name ) const;

    bool SetBoolProperty( const CString& name, bool value );
    bool SetIntProperty( const CString& name, Int32 value );
    bool SetFloatProperty( const CString& name, Float32 value );
    bool SetColourProperty( const CString& name, const CColour& value );
    bool SetStringListProperty( const CString& name, const TStringVector& value );
    bool SetIntListProperty( const CString& name, const GUCEF::vector< Int32 >& value );

    virtual bool SetPropertyValue( const CString& widgetProperty ,
                                   const CString& propertyValue  );

    virtual bool GetPropertyValue( const CString& widgetProperty ,
                                   CString& propertyValue        ) const;

    virtual bool SetProperties( const CORE::CValueList& list );

    /** Every property with its effective value */
    virtual bool GetProperties( CORE::CValueList& list ) const;

    virtual bool HasProperty( const CString& widgetProperty ) const;

    /** Reads "name=value" lines (the format SaveProperties() writes) */
    virtual bool LoadProperties( CORE::CIOAccess& propertyStorage );

    /** Writes the properties set on the widget as "name=value" lines */
    virtual bool SaveProperties( CORE::CIOAccess& propertyStorage ) const;

    /*
     *  Layout, state and focus
     */

    /** Where the widget is, in logical pixels of the context, as of the last layout pass */
    const CRect& GetRect( void ) const;

    /** The size the widget has when no size is set (the size of its picture, of its text ...) */
    virtual bool GetNaturalSize( Int32& width, Int32& height ) const;

    /** Visible and every ancestor visible */
    bool IsEffectivelyVisible( void ) const;

    /** Enabled and every ancestor enabled */
    bool IsEffectivelyEnabled( void ) const;

    /** STATE_* flags */
    UInt32 GetState( void ) const;

    bool IsHovered( void ) const;
    bool IsPressed( void ) const;
    bool HasFocus( void ) const;

    bool SetFocus( void );

    /** Whether the widget can get the keyboard focus (the Focusable property, and visible and enabled) */
    virtual bool IsFocusable( void ) const;

    /** Whether a left click or Accept on the focused widget makes an ActivatedEvent */
    virtual bool IsActivatable( void ) const;

    /** Puts the widget on top of its siblings */
    bool BringToFront( void );

    bool SendToBack( void );

    /*
     *  Context, driver and events
     */

    CGUIContext* GetContext( void ) const;

    CWidgetHandler* GetHandler( void ) const;

    /** The native object of the backend, for features gucefGUI does not offer; opaque to gucefGUI */
    void* GetNativeHandle( void ) const;

    /** Runs a command on the native object through the handler */
    bool RunCommand( const CString& command, const CORE::CValueList& arguments );

    /**
     *  Routes an event: tunnel from the root to the parent, the target (this widget), then bubble back
     *  to the root, stopping when an observer marks it handled. Returns whether it was handled.
     */
    bool DispatchEvent( const CORE::CEvent& eventId, CRoutedEventData& eventData );

    /** Notifies only the observers of this widget (for events that are not routed) */
    void DispatchDirectEvent( const CORE::CEvent& eventId, CRoutedEventData& eventData );

    /** Replaces the handler's applier for one property of this widget only */
    void SetPropertyApplierOverride( const CString& name, CWidgetHandler::TPropertyApplier applier );

    void RemovePropertyApplierOverride( const CString& name );

    virtual void* GetImplementationPtr( void ) const;

    virtual void SetImplementationPtr( void* impPtr );

    /** The registered widget type, for example "Button" */
    virtual const CString& GetType( void ) const;

    virtual const CString& GetClassTypeName( void ) const;

    protected:

    friend class CGUIContext;

    /** A property changed value (it is already stored) */
    virtual void OnPropertyChanged( const CString& name, const CString& oldValue, const CString& newValue );

    /** Called every frame for widgets that return true from NeedsUpdate() */
    virtual void OnUpdate( UInt32 deltaInMs );

    virtual bool NeedsUpdate( void ) const;

    /**
     *  The default behaviour of the widget type for an event nobody handled, after routing:
     *  for example a list box selects the row that was clicked. Return true when it did something.
     */
    virtual bool OnDefaultAction( const CORE::CEvent& eventId, CRoutedEventData& eventData );

    /** Sets a property because of the user and routes a ValueChangedEvent when the value changed */
    bool ChangeValueByUser( const CString& name, const CString& value );

    /** Routes an ItemActivatedEvent */
    void ActivateItem( Int32 index, const CString& item );

    /** Marks a runtime property changed so the handler gets it again (for example a new picture revision) */
    void Touch( const CString& name );

    private:

    void MarkDirty( const CString& name );
    void SetStateFlag( UInt32 flag, bool on );
    void DetachFromParent( void );
    bool NotifyRouted( const CORE::CEvent& eventId, CRoutedEventData& eventData );

    private:

    CWidget( const CWidget& src );
    CWidget& operator=( const CWidget& src );

    typedef GUCEF::map< CString, CWidgetHandler::TPropertyApplier > TApplierOverrideMap;

    CString m_widgetTypeName;
    CString m_name;
    void* m_impPtr;
    CGUIContext* m_context;
    CWidgetHandler* m_handler;
    CWidget* m_parent;
    TWidgetVector m_children;
    TPropertyMap m_properties;
    TStringSet m_dirtyProperties;
    TApplierOverrideMap m_applierOverrides;
    CRect m_rect;
    UInt32 m_state;
    bool m_stateDirty;
    bool m_parentDirty;
    bool m_childOrderDirty;
    bool m_rectDirty;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CWIDGET_H ? */
