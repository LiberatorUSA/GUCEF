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

#ifndef GUCEF_GUI_CWIDGETTYPEREGISTRY_H
#define GUCEF_GUI_CWIDGETTYPEREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_GUI_MACROS_H
#include "gucefGUI_macros.h"
#define GUCEF_GUI_MACROS_H
#endif /* GUCEF_GUI_MACROS_H ? */

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

class CWidget;

/**
 *  Describes one property of a widget type. The descriptors are the single source for the
 *  editor property grid, for what a layout file writes, for what a driver must map and for
 *  checking layout files.
 */
class GUCEF_GUI_PUBLIC_CPP CPropertyDescriptor
{
    public:

    enum EValueType
    {
        VALUETYPE_STRING     = 0 ,
        VALUETYPE_BOOL       = 1 ,
        VALUETYPE_INT        = 2 ,
        VALUETYPE_FLOAT      = 3 ,
        VALUETYPE_UDIM       = 4 ,  /**< "relative,pixels" */
        VALUETYPE_UVECTOR    = 5 ,  /**< "relative,pixels relative,pixels" */
        VALUETYPE_COLOUR     = 6 ,  /**< "r,g,b,a" */
        VALUETYPE_RECT       = 7 ,  /**< "left,top,width,height" */
        VALUETYPE_ENUM       = 8 ,  /**< one of 'enumValues' */
        VALUETYPE_IMAGE      = 9 ,  /**< an image alias registered with the context, or a VFS path */
        VALUETYPE_FONT       = 10,  /**< a font name */
        VALUETYPE_STRINGLIST = 11,  /**< entries separated by '\n'; a layout file writes one Value node per entry */
        VALUETYPE_INTLIST    = 12   /**< integers separated by ',' */
    };
    typedef enum EValueType TValueType;

    enum EFlags
    {
        FLAG_DESIGNABLE     = 1  ,  /**< shown in an editor's property grid */
        FLAG_STORED         = 2  ,  /**< written to a layout file */
        FLAG_AFFECTSLAYOUT  = 4  ,  /**< a change makes the context lay out the widgets again */
        FLAG_INHERITABLE    = 8  ,  /**< an unset value is taken from the parent */
        FLAG_BINDABLE       = 16 ,  /**< may be bound to a data model */
        FLAG_RUNTIME        = 32 ,  /**< state kept by gucefGUI while running (hover index, caret): not stored, not designable */
        FLAG_DEFAULT        = FLAG_DESIGNABLE | FLAG_STORED | FLAG_BINDABLE
    };

    typedef GUCEF::vector< CString > TStringVector;

    CString name;
    TValueType valueType;
    CString defaultValue;
    UInt32 flags;
    CString category;
    CString description;
    TStringVector enumValues;

    CPropertyDescriptor( void );

    CPropertyDescriptor( const CString& propertyName  ,
                         TValueType type              ,
                         const CString& defaultVal    ,
                         UInt32 propertyFlags         ,
                         const CString& propertyCategory ,
                         const CString& propertyDescription );

    bool HasFlag( UInt32 flag ) const;

    /** Checks a value against the type of the property; on success 'normalized' holds the canonical form */
    bool Validate( const CString& value, CString& normalized ) const;

    static CString ValueTypeToString( TValueType type );
};

/*-------------------------------------------------------------------------*/

/**
 *  Everything gucefGUI knows about one widget type: its name, the type it extends, how to make the
 *  facade and the properties it adds to those of its base.
 */
class GUCEF_GUI_PUBLIC_CPP CWidgetTypeInfo
{
    public:

    typedef CWidget* (*TWidgetCreator)( void );
    typedef GUCEF::vector< CPropertyDescriptor > TPropertyVector;

    CString typeName;
    CString baseTypeName;
    CString category;           /**< for an editor palette: "Common", "Containers", "Media" ... */
    CString description;
    TWidgetCreator creator;
    TPropertyVector properties; /**< only the properties this type adds */

    CWidgetTypeInfo( void );

    CWidgetTypeInfo& AddProperty( const CPropertyDescriptor& descriptor );
};

/*-------------------------------------------------------------------------*/

/**
 *  The registry of widget types. gucefGUI registers its own types at start; applications and
 *  plugins can register more. A type can only be used when the active driver has a handler for it
 *  or for one of its base types.
 */
class GUCEF_GUI_PUBLIC_CPP CWidgetTypeRegistry
{
    public:

    typedef GUCEF::vector< const CPropertyDescriptor* > TDescriptorVector;
    typedef GUCEF::set< CString > TStringSet;

    CWidgetTypeRegistry( void );
    ~CWidgetTypeRegistry();

    bool RegisterType( const CWidgetTypeInfo& typeInfo );
    bool UnregisterType( const CString& typeName );

    const CWidgetTypeInfo* GetTypeInfo( const CString& typeName ) const;
    bool IsRegistered( const CString& typeName ) const;

    /** The type and every type it extends, most derived first */
    TStringSet GetTypeNames( void ) const;
    GUCEF::vector< CString > GetTypeChain( const CString& typeName ) const;

    /** True when 'typeName' is 'baseTypeName' or extends it */
    bool IsA( const CString& typeName, const CString& baseTypeName ) const;

    /** Every property of the type including inherited ones, base properties first */
    TDescriptorVector GetProperties( const CString& typeName ) const;

    /** The descriptor for the property of the type, searching the base types too. NULL when unknown */
    const CPropertyDescriptor* FindProperty( const CString& typeName, const CString& propertyName ) const;

    /** Makes a facade of the given type, NULL when the type is unknown */
    CWidget* CreateWidget( const CString& typeName ) const;

    private:

    CWidgetTypeRegistry( const CWidgetTypeRegistry& src );
    CWidgetTypeRegistry& operator=( const CWidgetTypeRegistry& src );

    typedef GUCEF::map< CString, CWidgetTypeInfo > TTypeMap;

    TTypeMap m_types;
    mutable MT::CMutex m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CWIDGETTYPEREGISTRY_H ? */
