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

#ifndef GUCEF_CORE_CSIMPLISTICPLUGINLOADLOGIC_H
#define GUCEF_CORE_CSIMPLISTICPLUGINLOADLOGIC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIPLUGINLOADLOGIC_H
#include "gucefCORE_CIPluginLoadLogic.h"
#define GUCEF_CORE_CIPLUGINLOADLOGIC_H
#endif /* GUCEF_CORE_CIPLUGINLOADLOGIC_H ? */

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
 *  Plugin loader logic implementation which performs a very simplistic load.
 *  it does not care about grouName or pluginVersion's it simply loads the module
 *  from the path provided. If you don't need to perform any versioning (side-by-side)
 *  or grouping of your plugins then this simplistic approach can work well.
 */
class GUCEF_CORE_PRIVATE_CPP CSimplisticPluginLoadLogic : public CIPluginLoadLogic
{
    public:

    virtual void* LoadPlugin( const CString& rootDir            ,
                              const CString& moduleName         ,
                              const CString& groupName          ,
                              const TVersion* pluginVersion = 0 );

    virtual void UnloadPlugin( void* modulePtr );

    virtual CString GetLoaderLogicTypeName( void ) const;
    
    CSimplisticPluginLoadLogic( void );

    virtual ~CSimplisticPluginLoadLogic();

    private:

    CSimplisticPluginLoadLogic( const CSimplisticPluginLoadLogic& src );
    CSimplisticPluginLoadLogic& operator=( const CSimplisticPluginLoadLogic& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSIMPLISTICPLUGINLOADLOGIC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
