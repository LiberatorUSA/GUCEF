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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>

#include "CPropertyList.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

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

struct SProperty
{
        CString name;
        TPropertyType ptype;
        bool readonly;
        union
        {
                void* rw_value;
                const void* r_value;
        } value;
};

typedef struct SProperty TProperty;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPropertyList::CPropertyList( void )
{
}

/*-------------------------------------------------------------------------*/

CPropertyList::~CPropertyList()
{
        for ( Int32 i=0; i<=_properties.GetLast(); ++i )
        {
                delete (TProperty*) _properties[ i ];
        }
}

/*-------------------------------------------------------------------------*/

UInt32
CPropertyList::AddRW( const char* name   ,
                      TPropertyType type ,
                      void* property     )
{
        TProperty* p = new TProperty;
        p->name = name;
        p->ptype = type;
        p->value.rw_value = property;
        p->readonly = false;

        return _properties.AddEntry( p );
}

/*-------------------------------------------------------------------------*/

UInt32
CPropertyList::AddR( const char* name     ,
                     TPropertyType type   ,
                     const void* property )
{
        TProperty* p = new TProperty;
        p->name = name;
        p->ptype = type;
        p->value.r_value = property;
        p->readonly = true;

        return _properties.AddEntry( p );
}

/*-------------------------------------------------------------------------*/

bool
CPropertyList::Set( const CString& name  ,
                    const CString& value )
{
        TProperty* p;
        for ( Int32 i=0; i<=_properties.GetLast(); ++i )
        {
                p = (TProperty*) _properties[ i ];
                if ( p->name == name )
                {
                        if ( p->readonly ) return false;
                        return SetValue( value             ,
                                         p->value.rw_value ,
                                         p->ptype          );
                }
        }
        return false;
}


/*-------------------------------------------------------------------------*/

bool
CPropertyList::Set( UInt32 pindex        ,
                    const CString& value )
{
        TProperty* p = (TProperty*) _properties[ pindex ];
        if ( p )
        {
                if ( p->readonly ) return false;
                return SetValue( value             ,
                                 p->value.rw_value ,
                                 p->ptype          );
        }
        return false;
}

/*-------------------------------------------------------------------------*/

CString
CPropertyList::Get( const CString& name ) const
{
        TProperty* p;
        for ( Int32 i=0; i<=_properties.GetLast(); ++i )
        {
                p = (TProperty*)_properties[ i ];
                if ( p->name == name )
                {
                        return GetValue( p->value.r_value ,
                                         p->ptype         );
                }
        }
        CString str;
        return str;
}

/*-------------------------------------------------------------------------*/

CString
CPropertyList::Get( UInt32 pindex ) const
{
        TProperty* p = (TProperty*) _properties[ pindex ];
        return GetValue( p->value.r_value ,
                         p->ptype         );
}

/*-------------------------------------------------------------------------*/

CString
CPropertyList::operator[]( UInt32 index ) const
{
        TProperty* p = (TProperty*) _properties[ index ];
        return GetValue( p->value.r_value ,
                         p->ptype         );
}

/*-------------------------------------------------------------------------*/

UInt32
CPropertyList::Count( void ) const
{
        return _properties.GetLast()+1;
}

/*-------------------------------------------------------------------------*/

#pragma warning( disable: 4244 ) // conversion from 'UInt32' to 'var', possible loss of data

bool
CPropertyList::SetValue( const CString& newvalue ,
                         void* pvalue            ,
                         TPropertyType ptype     )
{         
        switch ( ptype )
        {
                case PROPERTYTYPE_UINT8 :
                {
                        UInt32 tmpval;
                        sscanf( newvalue.C_String(), "%d", &tmpval );
                        *((UInt8*)pvalue) = tmpval;
                        return true;
                }
                case PROPERTYTYPE_INT8 :
                {
                        Int32 tmpval;
                        sscanf( newvalue.C_String(), "%d", &tmpval );
                        *((Int8*)pvalue) = tmpval;
                        return true;
                }
                case PROPERTYTYPE_UINT16 :
                {
                        UInt32 tmpval;
                        sscanf( newvalue.C_String(), "%d", &tmpval );
                        *((UInt16*)pvalue) = tmpval;
                        return true;
                }
                case PROPERTYTYPE_INT16 :
                {
                        Int32 tmpval;
                        sscanf( newvalue.C_String(), "%d", &tmpval );
                        *((Int16*)pvalue) = tmpval;
                        return true;
                }
                case PROPERTYTYPE_UINT32 :
                {
                        UInt32 tmpval;
                        sscanf( newvalue.C_String(), "%d", &tmpval );
                        *((UInt32*)pvalue) = tmpval;
                        return true;
                }
                case PROPERTYTYPE_INT32 :
                {
                        Int32 tmpval;
                        sscanf( newvalue.C_String(), "%d", &tmpval );
                        *((Int32*)pvalue) = tmpval;
                        return true;
                }
                case PROPERTYTYPE_UINT64 :
                {
                     /*   UInt64 tmpval;
                        sscanf( newvalue.C_String(), "%d", &tmpval );
                        *((UInt64*)pvalue) = tmpval;
                        return true; */
                        return false;
                }
                case PROPERTYTYPE_INT64 :
                {
                      /*  Int64 tmpval;
                        sscanf( newvalue.C_String(), "%d", &tmpval );
                        *((Int64*)pvalue) = tmpval;
                        return true; */
                        return false;
                }
                case PROPERTYTYPE_FLOAT8 :
                {
                        return false;
                }
                case PROPERTYTYPE_FLOAT16 :
                {
                        return false;
                }
                case PROPERTYTYPE_FLOAT32 :
                {
                        Float32 tmpval;
                        sscanf( newvalue.C_String(), "%f", &tmpval );
                        *((Float32*)pvalue) = tmpval;
                        return true;
                }
                case PROPERTYTYPE_FLOAT64 :
                {
                        Float64 tmpval;
                        sscanf( newvalue.C_String(), "%f", &tmpval );
                        *((Float64*)pvalue) = tmpval;
                        return true;
                }
                case PROPERTYTYPE_STRING :
                {
                        *((CString*)pvalue) = newvalue;
                        return true;
                }
                default :
                {
                        return false;
                }
        }    
}

/*-------------------------------------------------------------------------*/

CString
CPropertyList::GetValue( const void* value   ,
                         TPropertyType ptype ) const
{
        switch ( ptype )
        {
                case PROPERTYTYPE_UINT8 :
                {
                        char vstr[ 10 ];
                        sprintf( vstr, "%d", (*(UInt8*)value) );
                        CString str( vstr );
                        return str;
                }
                case PROPERTYTYPE_INT8 :
                {
                        char vstr[ 10 ];
                        sprintf( vstr, "%d", (*(Int8*)value) );
                        CString str( vstr );
                        return str;
                }
                case PROPERTYTYPE_UINT16 :
                {
                        char vstr[ 10 ];
                        sprintf( vstr, "%d", (*(UInt16*)value) );
                        CString str( vstr );
                        return str;
                }
                case PROPERTYTYPE_INT16 :
                {
                        char vstr[ 10 ];
                        sprintf( vstr, "%d", (*(Int16*)value) );
                        CString str( vstr );
                        return str;
                }
                case PROPERTYTYPE_UINT32 :
                {
                        char vstr[ 10 ];
                        sprintf( vstr, "%d", (*(UInt32*)value) );
                        CString str( vstr );
                        return str;
                }
                case PROPERTYTYPE_INT32 :
                {
                        char vstr[ 10 ];
                        sprintf( vstr, "%d", (*(Int32*)value) );
                        CString str( vstr );
                        return str;
                }
                case PROPERTYTYPE_UINT64 :
                {
                   /*     char vstr[ 20 ];
                        sprintf( vstr, "%d", (*(UInt64*)value) );
                        CString str( vstr ); */
                        CString str;
                        return str;
                }
                case PROPERTYTYPE_INT64 :
                {
                   /*     char vstr[ 20 ];
                        sprintf( vstr, "%d", (*(Int64*)value) );
                        CString str( vstr );
                        return str; */
                        CString str;
                        return str;
                }
                case PROPERTYTYPE_FLOAT8 :
                {
                        CString str;
                        return str;
                }
                case PROPERTYTYPE_FLOAT16 :
                {
                        CString str;
                        return str;
                }
                case PROPERTYTYPE_FLOAT32 :
                {
                        char vstr[ 10 ];
                        sprintf( vstr, "%f", (*(Float32*)value) );
                        CString str( vstr );
                        return str;
                }
                case PROPERTYTYPE_FLOAT64 :
                {
                        char vstr[ 20 ];
                        sprintf( vstr, "%f", (*(Float64*)value) );
                        CString str( vstr );
                        return str;
                }
                case PROPERTYTYPE_STRING :
                {
                        CString str( *((CString*)value) );
                        return str;
                }
                default :
                {
                        CString str;
                        return str;
                }
        }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


