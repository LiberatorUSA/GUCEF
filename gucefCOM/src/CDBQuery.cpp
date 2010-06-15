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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef DVOSWRAP_H
#include "dvoswrap.h"
#define DVOSWRAP_H
#endif /* DVOSWRAP_H ? */

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CDBQuery.h"   /* definition of this class */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDBQuery::CDBQuery(void)
{GUCEF_TRACE;
        column = NULL;
        field = NULL;

        columns = 0;
        rows = 0;
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::Columns(UInt32 amount)
{GUCEF_TRACE;        
        columns = amount;

        column = new CDBColumn[amount];
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::Rows(UInt32 amount)
{GUCEF_TRACE;
        rows = amount;

        field = new CDBField[rows * columns];
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::FreeColumns(void)
{GUCEF_TRACE;

        if(column != NULL) {
                delete[] column;
                column = NULL;
        }

        columns = 0;
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::FreeRows(void)
{GUCEF_TRACE;

        if(field != NULL) {
                delete[] field;
                field = NULL;
        }

        rows = 0;
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::Wait_Untill_Done( UInt32 timeout )
{GUCEF_TRACE;

        UInt32 time = CORE::GUCEFGetTickCount();
        
        while ( ( state != CDB_CHANGED ) &&
                ( state != CDB_ERROR ) )
        {
                if ( timeout && ( CORE::GUCEFGetTickCount()-time > timeout ) )
                {
                        return;
                }
                MT::ThreadDelay( 10 );
        }

        #ifdef DEBUG_MODE
        switch ( state )
        {
                case CDB_CHANGED :
                {
                        CORE::tsprintf( "CDBQuery: Database data changed\n" );
                        return;
                }
                case CDB_ERROR :
                {
                        CORE::tsprintf( "CDBQuery: Error\n" );
                        return;
                }

        }
        #endif /* DEBUG_MODE */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
