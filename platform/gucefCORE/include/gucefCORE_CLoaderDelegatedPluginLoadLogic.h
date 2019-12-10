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

#ifndef GUCEF_CORE_CLOADERDELEGATEDPLUGINLOADLOGIC_H
#define GUCEF_CORE_CLOADERDELEGATEDPLUGINLOADLOGIC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "EStructs.h"

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
 *  Plugin loader logic implementation which delegates to a gucefLOADER module
 *  which houses the actual loader logic. The gucefLOADER modules is a specialist
 *  module which keeps module loading centralized and stable by using C interfaces.
 */
class GUCEF_CORE_PRIVATE_CPP CLoaderDelegatedPluginLoadLogic : public CIPluginLoadLogic
{
    public:

    virtual void* LoadPlugin( const CString& rootDir            ,
                              const CString& moduleName         ,
                              const CString& groupName          ,
                              const TVersion* pluginVersion = 0 );

    virtual void UnloadPlugin( void* modulePtr );

    virtual CString GetLoaderLogicTypeName( void ) const;
    

    CLoaderDelegatedPluginLoadLogic( void );

    virtual ~CLoaderDelegatedPluginLoadLogic();

    private:

    void Initialize( void );
    
    CLoaderDelegatedPluginLoadLogic( const CLoaderDelegatedPluginLoadLogic& src );
    CLoaderDelegatedPluginLoadLogic& operator=( const CLoaderDelegatedPluginLoadLogic& src );

    void* m_cInterface;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CLOADERDELEGATEDPLUGINLOADLOGIC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
