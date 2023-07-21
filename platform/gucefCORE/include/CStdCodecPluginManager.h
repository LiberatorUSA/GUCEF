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

#ifndef GUCEF_CORE_CSTDCODECPLUGINMANAGER_H
#define GUCEF_CORE_CSTDCODECPLUGINMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CPLUGINMANAGER_H
#include "CPluginManager.h"
#define GUCEF_CORE_CPLUGINMANAGER_H
#endif /* GUCEF_CORE_CPLUGINMANAGER_H ? */

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
 *  plugin manager for GUCEF codec plugins
 */
class GUCEF_CORE_PUBLIC_CPP CStdCodecPluginManager : public CPluginManager
{
    public:

    static const CEvent StdCodecRegisteredEvent;
    static const CEvent StdCodecUnregisteredEvent;

    static void RegisterEvents( void );

    virtual CString GetPluginType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual TPluginPtr RegisterPlugin( void* modulePtr                   ,
                                       TPluginMetaDataPtr pluginMetaData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool UnregisterPlugin( TPluginPtr plugin ) GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CCoreGlobal;

    CStdCodecPluginManager( void );

    virtual ~CStdCodecPluginManager() GUCEF_VIRTUAL_OVERRIDE;

    private:

    CStdCodecPluginManager( const CStdCodecPluginManager& src );

    CStdCodecPluginManager& operator=( const CStdCodecPluginManager& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTDCODECPLUGINMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 30-09-2006 :
        - Dinand: Recoded after crash wiped out the new code

-----------------------------------------------------------------------------*/
