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

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#include "gucefCORE_CDataNodeSerializableSettings.h"   

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CDataNodeSerializableSettings::ClassTypeName = "GUCEF::CORE::CDataNodeSerializableSettings";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDataNodeSerializableSettings::CDataNodeSerializableSettings( void )
    : CIConfigurable()
    , levelOfDetail( DataNodeSerializableLod_Maximum )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeSerializableSettings::CDataNodeSerializableSettings( const CDataNodeSerializableSettings& src )
    : CIConfigurable( src )
    , levelOfDetail( src.levelOfDetail )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeSerializableSettings::~CDataNodeSerializableSettings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeSerializableSettings&
CDataNodeSerializableSettings::operator=( const CDataNodeSerializableSettings& src )
{GUCEF_TRACE;

    CIConfigurable::operator=( src );
    levelOfDetail = src.levelOfDetail;
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CDataNodeSerializableSettings::SaveConfig( CDataNode& cfg ) const
{GUCEF_TRACE;

    bool success = true;
    success = cfg.SetAttribute( "levelOfDetail", levelOfDetail ) && success;
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CDataNodeSerializableSettings::LoadConfig( const CDataNode& cfg )
{GUCEF_TRACE;

    levelOfDetail = cfg.GetAttributeValueOrChildValueByName( "levelOfDetail" ).AsUInt32( levelOfDetail, true );
    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CDataNodeSerializableSettings::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
