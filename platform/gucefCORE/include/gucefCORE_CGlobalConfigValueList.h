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

#ifndef GUCEF_CORE_CGLOBALCONFIGVALUELIST_H
#define GUCEF_CORE_CGLOBALCONFIGVALUELIST_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#include "gucefCORE_CGloballyConfigurable.h"
#define GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#endif /* GUCEF_CORE_CGLOBALLYCONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Helper class for cases where you have simplistic config needs and yet still 
 *  wish to tie into the global config loading machinery
 *
 *  Adds a tie-in to global config loading to the regular CValueList 
 */
class GUCEF_CORE_PUBLIC_CPP CGlobalConfigValueList : public CValueList            ,
                                                     public CGloballyConfigurable
{
    public:

    CGlobalConfigValueList( void );

    CGlobalConfigValueList( const CValueList& src );

    CGlobalConfigValueList& operator=( const CValueList& src );

    virtual ~CGlobalConfigValueList();

    virtual const CString& GetClassTypeName( void ) const;

    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;
                                             
    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CGLOBALCONFIGVALUELIST_H */
