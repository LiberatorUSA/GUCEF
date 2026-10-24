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

#ifndef GUCEF_GUI_CSETTINGSPAGEFORM_H
#define GUCEF_GUI_CSETTINGSPAGEFORM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTABSTRACTFACTORY_H
#include "CTAbstractFactory.h"
#define GUCEF_CORE_CTABSTRACTFACTORY_H
#endif /* GUCEF_CORE_CTABSTRACTFACTORY_H ? */

#ifndef GUCEF_GUI_CFORMEX_H
#include "gucefGUI_CFormEx.h"
#define GUCEF_GUI_CFORMEX_H
#endif /* GUCEF_GUI_CFORMEX_H ? */

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

/**
 *  A page of settings for CSettingsForm: a prebaked set of widgets that shows some settings and applies
 *  what the user changed. A page builds its layout in code (BuildLayout()), so it needs no files, and the
 *  layout can be saved like any other.
 *
 *  ReadSettings() puts the settings in the widgets; ApplySettings() takes the values of the widgets and
 *  applies them. When the user changes a value the page notifies PageChangedEvent.
 */
class GUCEF_GUI_PUBLIC_CPP CSettingsPageForm : public CFormEx
{
    public:

    static const CORE::CEvent PageChangedEvent;     /**< the user changed a value on the page */
    static const CORE::CEvent SettingsAppliedEvent; /**< ApplySettings() applied the values of the page */

    static void RegisterEvents( void );

    public:

    CSettingsPageForm( void );

    virtual ~CSettingsPageForm();

    /** The text on the tab of the page */
    virtual CString GetTitle( void ) const = 0;

    /** Makes the widgets of the page below 'parent' (in the context of 'parent') and shows the settings in them */
    virtual bool BuildPage( CWidget* parent );

    /** Puts the settings in the widgets; afterwards the page has no changes */
    virtual void ReadSettings( void ) = 0;

    /** Applies the values of the widgets; afterwards the page has no changes */
    virtual bool ApplySettings( void ) = 0;

    /** Whether the user changed a value since the settings were read or applied */
    bool HasChanges( void ) const;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    /** Writes the layout of the page: 'layout' is an empty layout tree, see CFormLayout::InitLayout() */
    virtual void BuildLayout( CORE::CDataNode& layout ) = 0;

    /** Called when the user changed the value of the widget */
    virtual void OnValueChanged( CWidget& widget );

    /** Subscribes to the value changes of the widgets */
    virtual void OnPostLayoutLoad( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnNotify( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventId               ,
                           CORE::CICloneable* eventData = GUCEF_NULL ) GUCEF_VIRTUAL_OVERRIDE;

    /** Marks the page as changed or not; changes the widgets make while settings are read do not count */
    void SetHasChanges( bool hasChanges );

    /** Set while the settings are put in the widgets, so the value changes that causes are not the user's */
    void SetReadingSettings( bool readingSettings );

    bool IsReadingSettings( void ) const;

    /** The value as text of a widget of the page by name, empty when there is no such widget */
    CString GetWidgetProperty( const CString& widgetName, const CString& propertyName );

    bool SetWidgetProperty( const CString& widgetName, const CString& propertyName, const CString& value );

    private:

    CSettingsPageForm( const CSettingsPageForm& src );
    CSettingsPageForm& operator=( const CSettingsPageForm& src );

    private:

    bool m_hasChanges;
    bool m_readingSettings;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTAbstractFactory< CString, CSettingsPageForm, MT::CMutex >  CSettingsPageFactory;
typedef CSettingsPageFactory::TProductPtr                                  TSettingsPageFormPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CSETTINGSPAGEFORM_H ? */
