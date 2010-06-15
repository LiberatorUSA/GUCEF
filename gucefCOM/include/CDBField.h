/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

#ifndef CDBFIELD_H
#define CDBFIELD_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef DVSTRUTILS_H
#include "dvstrutils.h" /* my verry own string utils */
#define DVSTRUTILS_H
#endif /* DVSTRUTILS_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *	Forward declarations of classes used
 */
class CMySQLClient;	/* MySQL client implementation */

/*--------------------------------------------------------------------------*/

/**
 *	Inline implementation of a database field. Initial use is for the
 *	CMySQLClient as data storage.
 */
class GUCEFCOM_EXPORT_CPP CDBField
{
	private:
        friend class CMySQLClient;
        char *value;

	public:
        const char *AsString( void ) { return value; }

        Int32 AsInteger( void ) { return CORE::Str_To_Int( value ); }

        CDBField( void ) { value = NULL; }
        virtual ~CDBField() { if(value != NULL) delete[] value; }
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* CDBFIELD_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 31-05-2005 :
        - Commenced integration into new GUCEF codebase
- 14-10-2003 :
        - Started integrating this class into the DVNETCOM codebase.
          This class was contributed to the codebase by Logan, many thanx to him
          for this usefull class.
        - Placed this class in a seperate file and changed layout to conform
          with my standard.

---------------------------------------------------------------------------*/
