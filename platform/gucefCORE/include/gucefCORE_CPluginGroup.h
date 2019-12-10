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

#ifndef GUCEF_CORE_CPLUGINGROUP_H
#define GUCEF_CORE_CPLUGINGROUP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIPLUGINMETADATA_H
#include "gucefCORE_CIPluginMetaData.h"
#define GUCEF_CORE_CIPLUGINMETADATA_H
#endif /* GUCEF_CORE_CIPLUGINMETADATA_H ? */

#ifndef GUCEF_CORE_CIPLUGIN_H
#include "CIPlugin.h"
#define GUCEF_CORE_CIPLUGIN_H
#endif /* GUCEF_CORE_CIPLUGIN_H ? */

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
 *  Logical grouping class for plugins
 *  Allows plugins to be managed as groups
 */
class GUCEF_CORE_PRIVATE_CPP CPluginGroup
{
    public:

    typedef std::set< TPluginPtr > TPluginSet;
    typedef std::set< TPluginMetaDataPtr > TPluginMetaDataSet;
    
    CPluginGroup( void );

    CPluginGroup( const CPluginGroup& src );

    virtual ~CPluginGroup();

    CPluginGroup& operator=( const CPluginGroup& src );

    TPluginSet& GetPlugins( void );

    TPluginMetaDataSet& GetPluginMetaData( void );

    bool HasPluginWithFileName( const CString& moduleFileName ) const;

    private:

    TPluginSet m_plugins;
    TPluginMetaDataSet m_pluginMetaData;
};

/*-------------------------------------------------------------------------*/

typedef CTSharedPtr< CPluginGroup > TPluginGroupPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPLUGINGROUP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
