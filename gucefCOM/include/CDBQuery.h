/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2003.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#ifndef CDBQUERY_H
#define CDBQUERY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDBFIELD_H
#include "CDBFIELD.h"	/* class representing a single database field */
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

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

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
class EXPORT_CPP CDBQuery
{
	friend class CMySQLClient;

	private:
        UInt8 state, error, effected_rows, insert_id;
        UInt16 status;
        UInt32 columns, rows;
        char *description;
        CDBField *field;
        CDBColumn *column;

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

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

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
 