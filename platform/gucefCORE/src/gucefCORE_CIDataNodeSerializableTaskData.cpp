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

#include "gucefCORE_CIDataNodeSerializableTaskData.h"   

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CIDataNodeSerializableTaskData::CIDataNodeSerializableTaskData( void )
    : CIDataNodeSerializable()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDataNodeSerializableTaskData::CIDataNodeSerializableTaskData( const CIDataNodeSerializableTaskData& src )
    : CIDataNodeSerializable( src ) 
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDataNodeSerializableTaskData::~CIDataNodeSerializableTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDataNodeSerializableTaskData&
CIDataNodeSerializableTaskData::operator=( const CIDataNodeSerializableTaskData& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
