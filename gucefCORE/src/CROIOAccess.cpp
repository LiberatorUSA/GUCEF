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

#include "CROIOAccess.h"     /* definition of the class implemented here */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CROIOAccess::CROIOAccess( void )
{
        memset( &_access, 0, sizeof( TIOAccess ) );
}

/*-------------------------------------------------------------------------*/

CROIOAccess::~CROIOAccess()
{
}

/*-------------------------------------------------------------------------*/
        
bool 
CROIOAccess::IsReadOnly( void ) const
{
        return true;
}
        
/*-------------------------------------------------------------------------*/

bool 
CROIOAccess::IsWriteOnly( void ) const
{
        return false;
}

/*-------------------------------------------------------------------------*/
        
bool 
CROIOAccess::IsReadAndWrite( void ) const
{
        return false;
}

/*-------------------------------------------------------------------------*/
       
UInt32 
CROIOAccess::Write( const void* srcdata ,
                    UInt32 esize        ,
                    UInt32 elements     )
{
        return 0;
}

/*-------------------------------------------------------------------------*/

void
CROIOAccess::Flush( void )                    
{
    // no-op
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
