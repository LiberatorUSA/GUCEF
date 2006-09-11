/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#include "CDBQuery.h"   /* definition of this class */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDBQuery::CDBQuery(void)
{TRACE;
        column = NULL;
        field = NULL;

        columns = 0;
        rows = 0;
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::Columns(UInt32 amount)
{TRACE;        
        columns = amount;

        column = new CDBColumn[amount];
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::Rows(UInt32 amount)
{TRACE;
        rows = amount;

        field = new CDBField[rows * columns];
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::FreeColumns(void)
{TRACE;

        if(column != NULL) {
                delete[] column;
                column = NULL;
        }

        columns = 0;
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::FreeRows(void)
{TRACE;

        if(field != NULL) {
                delete[] field;
                field = NULL;
        }

        rows = 0;
}

/*--------------------------------------------------------------------------*/

void
CDBQuery::Wait_Untill_Done( UInt32 timeout )
{TRACE;

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
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
