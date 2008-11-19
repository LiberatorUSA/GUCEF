/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CORE_CPROPERTYLIST
#define GUCEF_CORE_CPROPERTYLIST

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"             /* Simple types used */
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_Macros.h"       /* Library specific & generic macros */
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"          /* String class implementation */
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"      /* generic dynamic array */ 
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef enum
{
        PROPERTYTYPE_UINT8   = 1 ,
        PROPERTYTYPE_INT8        ,
        PROPERTYTYPE_UINT16      ,
        PROPERTYTYPE_INT16       ,
        PROPERTYTYPE_UINT32      ,
        PROPERTYTYPE_INT32       ,
        PROPERTYTYPE_UINT64      ,
        PROPERTYTYPE_INT64       ,
        PROPERTYTYPE_FLOAT8      ,
        PROPERTYTYPE_FLOAT16     ,
        PROPERTYTYPE_FLOAT32     ,
        PROPERTYTYPE_FLOAT64     ,
        PROPERTYTYPE_STRING      
} TPropertyType;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

//@deprecated
class GUCEFCORE_EXPORT_CPP CPropertyList
{

        public:

        CPropertyList( void );

        ~CPropertyList();

        /**
         *      @brief  Adds a mutable property to the list.
         *      @param  name    The name of the property
         *      @param  type    Type of the property value
         *      @param  value   the mutable value storage object
         *      @return Index of the property in the list
         */
        UInt32
        AddRW( const char* name   ,
               TPropertyType type ,
               void* value        );

        /**
         *      @brief  Adds a read-only property to the list.
         *      @param  name    The name of the property
         *      @param  type    Type of the property value
         *      @param  value   the read-only value storage object
         *      @return Index of the property in the list
         */
        UInt32
        AddR( const char* name     ,
              TPropertyType type   ,
              const void* value    );

        /**
         *      @brief  If found sets the named property to the given value
         *      @param  name    name of the property  
         *      @param  value   the new value you wish to set
         *      @return Wheter setting the property was successfull.     
         */
        bool
        Set( const CString& name  ,
             const CString& value );

        /**
         *      @brief  If found sets the named property to the given value
         *      @param  pindex  index of the property in the list.  
         *      @param  value   the new value you wish to set
         *      @return Wheter setting the property was successfull.     
         */
        bool
        Set( UInt32 pindex        ,
             const CString& value );

        CString
        Get( const CString& name ) const;

        CString
        Get( UInt32 pindex ) const;

        CString
        operator[]( UInt32 index ) const;

        UInt32 Count( void ) const;

        private:

        CPropertyList( const CPropertyList& src ); /* Don't make copys */
        CPropertyList& operator=( const CPropertyList& src );     /* Don't make copys */

        bool
        SetValue( const CString& newvalue ,
                  void* pvalue            ,
                  TPropertyType ptype     );

        CString
        GetValue( const void* value   ,
                  TPropertyType ptype ) const;

        CDynamicArray _properties;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPROPERTYLIST ? */

 