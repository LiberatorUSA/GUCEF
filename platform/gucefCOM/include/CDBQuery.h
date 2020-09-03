/*
 *  gucefCOM: GUCEF module providing communication implementations 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#ifndef CDBQUERY_H
#define CDBQUERY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDBFIELD_H
#include "CDBField.h"	/* class representing a single database field */
#define CDBFIELD_H
#endif /* CDBFIELD_H */

#ifndef CDBCOLUMN_H
#include "CDBColumn.h"  /* class representing a single database column */
#define CDBCOLUMN_H
#endif /* CDBCOLUMN_H */

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
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

enum
{
        CDB_ERROR = 0,
        CDB_GETTING_DATA = 1,
        CDB_SELECT,
        CDB_CHANGED
};

/**
 *	Forward declarations of classes used
 */
class CMySQLClient;	/* MySQL client implementation */

/*--------------------------------------------------------------------------*/

/**
 *	Class which functions as a container of database fields from a single
 *	column.
 */
class GUCEF_COM_PUBLIC_CPP CDBQuery
{
	friend class CMySQLClient;

	private:
        UInt8 state, error, effected_rows, insert_id;
        UInt16 status;
        UInt32 columns, rows;
        char *description;
        CDBField* field;
        CDBColumn* column;

        void Columns(UInt32 amount);
        void Rows(UInt32 amount);

        void FreeColumns(void);
        void FreeRows(void);
        void FreeMemory(void) { FreeRows(); FreeColumns(); }

	public:
        UInt8 State(void) { return state; }

        void Wait_Untill_Done( UInt32 timeout );

        UInt32 Columns(void) { return columns; }
        UInt32 Rows(void) { return rows; }
        UInt8 Error(void) { return error; }
        UInt8 EffectedRows(void) { return effected_rows; }
        UInt8 InsertID(void) { return insert_id; }

        UInt16 Status(void) { return status; }
        
        const char *Description(void) { return description; }

        CDBColumn *Column(UInt32 i) { return &column[i]; }
        CDBField *Field(UInt32 row_num, UInt32 column_num) { return &field[row_num * columns + column_num]; }

        CDBQuery(void);
        virtual ~CDBQuery() { FreeMemory(); }
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* CDBQUERY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 31-05-2005 :
        - Commenced integration into new GUCEF codebase
- 18-04-2004 :
        - Changed the values of the CDB ennum values. Error is now 0 which is
          nice for testing with an if. 
- 11-04-2004 :
        - Added Wait_Untill_Done().
- 14-10-2003 :
        - Started integrating this class into the DVNETCOM codebase.
          This class was contributed to the codebase by Logan, many thanx to him
          for this usefull class.
        - Placed this class in a seperate file and changed layout to conform
          with my standard.          

---------------------------------------------------------------------------*/
 